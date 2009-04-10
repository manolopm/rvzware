/*
 * CAPAWARE 3D Geographical Multilayer Framework Free Software
 * Copyright (C) 2009 Instituto Tecnológico de Canarias (ITC) and Universidad de Las Palmas de Gran Canaria (ULPGC)
 *
 * This file is part of CAPAWARE.
 *
 * CAPAWARE is free software; you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 2.1 of the License, or (at your option)
 * any later version.
 *
 * CAPAWARE is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this application; if not, write to the Free Software Foundation, Inc.,
 * 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 *
 * The CAPAWARE development team
*/
#include <string>
#include <vector>
#include <wx/listbox.h>
#include <sstream>

#include "stdafx.h"

#define TAG_IGNITION_POINT 1
#define TAG_KEYFLAMES 0

#include <controllers/FireController.h>
#include <controllers/PersistentController.h>

#include <cpwutils/GraphicFireCallBack.h>

#include <cpw/ApplicationConfiguration.h>

#include <cpw/entity/ElementParticleSystem.h>
#include <cpw/common/IdGenerator.h>
#include <cpw/common/types.h>
#include <cpw/common/IdGenerator.h>
#include <cpw/entity/EntityRegistry.h>
#include <cpw/entity/EntityFactory.h>
#include <cpw/graphic/ApplicationScene.h>

//fire/smoke coefficients for range
#define FIRE_ALPHA_RANGE_COEF 0.639f
#define FIRE_SPEED_RANGE_COEF 0.455f
#define FIRE_SIZE_RANGE_COEF  0.004f
#define FIRE_PARTICLESSEC_RANGE_COEF 0.717f

#define SMOKE_ALPHA_RANGE_COEF 0.0f
#define SMOKE_SPEED_RANGE_COEF 0.5f
#define SMOKE_SIZE_RANGE_COEF  0.149f
#define SMOKE_PARTICLESSEC_RANGE_COEF 0.6f

using namespace cpw::controllers;


/******************************************************************************************************************/
FireControllerEventReceiver::FireControllerEventReceiver(void): drag(false)
{
	
}

FireControllerEventReceiver::~FireControllerEventReceiver(void)
{

}

FireControllerEventReceiver::FireControllerEventReceiver(const FireControllerEventReceiver &fcer)
{

}

bool FireControllerEventReceiver::MouseLeftButtonDown(const int &x,const int &y)
{
	drag = false;

	return false;

}
bool FireControllerEventReceiver::MouseLeftButtonUp(const int &x,const int &y)
{

	if(!drag) 
		fc->MouseEvent(x, y, wxMOUSE_BTN_LEFT);
		
	return false;
}

bool FireControllerEventReceiver::MouseDrag(const int &x0, const int &y0, const int &x1, const int &y1, const cpw::MouseButtonsState &mbs)
{
	drag = true;

	return false;
}

void FireControllerEventReceiver::SetFireController(FireController *_fc)
{ 
	fc = _fc;
}


/******************************************************************************************************************/

FireController::FireController(wxWindow* _parent, cpw::LayerTree &tr_lay, cpw::IGraphicFactory *_graphic_factory, cpw::INavigatorManager *nm, cpw::IGraphicObjectLoader *loader) : 
		parent(_parent), layer_tree(&tr_lay), navigator_manager(nm), fire_entity(NULL), update_selected_flame(true), cancel_overwrite(false),
		graphic_object_loader(loader), fire_exists_in_scene(false), render_ts(-1), render_fl(-1), current_perimeter(-1), new_fire(true), graphic_factory(_graphic_factory), fire_entity_backup(NULL)
{
	ui_fire = new cpw::gui::UIFire(this,_parent);

	// the mouse event callback
	mouse_event = new FireControllerEventReceiver();
	mouse_event->SetFireController(this);
	mouse_event->SetNavigatorManager(navigator_manager);
	cpw::INavigator *navigator = navigator_manager->GetFocusedOrFirstNavigator();
	int nc = navigator->GetEventReceiversCount();
	if (nc>0)
		navigator->SetEventReceiver(nc-1, mouse_event);

	ui_fire->SetFireTexture(cpw::ApplicationConfiguration::GetInstance()->GetFireTexture());
	ui_fire->SetSmokeTexture(cpw::ApplicationConfiguration::GetInstance()->GetSmokeTexture());
	
	graphic_object_controller = new GraphicObjectController(graphic_object_loader, navigator_manager);

}


FireController::~FireController()
{
	if (ui_fire != NULL)
	{
		delete ui_fire;
		ui_fire = NULL;
	}
	
	if (graphic_object_controller != NULL)
	{
		delete graphic_object_controller;
		graphic_object_controller = NULL;
	}

	if(graphic_object_loader != NULL)
	{
		delete graphic_object_loader;
		graphic_object_loader = NULL;	
	}

	if(fire_entity != NULL)
	{
		cpw::utils::Fire *temp = (cpw::utils::Fire*)cpw::EntityRegistry::GetInstance()->GetEntity(fire_entity->GetId());
		if(temp == NULL)
		{
			delete fire_entity;
			fire_entity = NULL;
		}
	}

	ReleaseMouseCallBack();
	if(mouse_event != NULL)
	{
		delete mouse_event;
		mouse_event = NULL;
	}

	if(fire_entity_backup != NULL)
	{
		delete fire_entity_backup;
		fire_entity_backup = NULL;
	}
}

void FireController::ReleaseMouseCallBack()
{
	if(navigator_manager!=NULL)
	{
		cpw::INavigator *navigator = navigator_manager->GetFocusedOrFirstNavigator();
		navigator->RemoveEventReceiver(mouse_event);
	}
}

bool FireController::NewFlame(const cpw::Point3d<float> &flame_position, const int &time_slice_index)
{
	cpw::utils::TimeSlice *ts = fire_entity->GetTimeSlice(time_slice_index);
	
	if (ts == NULL) 
		return false;

	cpw::utils::Flame new_flame;
				
	new_flame.fire.SetAlpha(cpw::Range<float>(0.47f, 0.3f));
	new_flame.fire.SetSpeed(cpw::Range<float>(1.f, 22.0f));
	new_flame.fire.SetPhi(cpw::Range<float>(360.0f, 0.0f));
	new_flame.fire.SetTheta(cpw::Range<float>(17.0f, 0.0f));
	new_flame.fire.SetSize(cpw::Range<float>(60.550f, 0.2262f));
	new_flame.fire.SetNewParticlesSec(cpw::Range<int>(12.5f, 19.0f));
	new_flame.fire.SetAdditiveBlend(true);
	new_flame.fire.SetSenoidalInterpolator(false);
	new_flame.fire.SetParticlesLife(1.55f);
	new_flame.fire.SetInsertedInScene(false);
	new_flame.fire.AddParent(fire_entity);

	new_flame.smoke.SetAlpha(cpw::Range<float>(0.3f, 0.0f));
	new_flame.smoke.SetSpeed(cpw::Range<float>(220.0f, 120.0f));
	new_flame.smoke.SetPhi(cpw::Range<float>(360.0f, 0.0f));
	new_flame.smoke.SetTheta(cpw::Range<float>(17.0f, 0.0f));
	new_flame.smoke.SetSize(cpw::Range<float>(19.0f, 130.5f));
	new_flame.smoke.SetNewParticlesSec(cpw::Range<int>(4, 6));
	new_flame.smoke.SetAdditiveBlend(false);
	new_flame.smoke.SetSenoidalInterpolator(false);
	new_flame.smoke.SetParticlesLife(6.0f);
	new_flame.smoke.SetInsertedInScene(false);
	new_flame.smoke.AddParent(fire_entity);

	new_flame.fire.SetCoords(flame_position);
	new_flame.smoke.SetCoords(flame_position);

	ts->flames.push_back(new_flame);

	return true;
}

void FireController::MouseEvent(int x, int y, int z)
{
	bool flame_added = false;
	int active_perimeter = ui_fire->GetActiveKeyFlame();
	int active_flame = ui_fire->GetActiveFlame();
	int active_ignition_point = ui_fire->GetIgnitionPointsListBox()->GetSelection();

	if ((ui_fire->GetActiveNotePage() == TAG_KEYFLAMES) && (active_perimeter == -1))
		return;
	
	cpw::Point3d<float> intersection_point(0.0f, 0.0f, 0.0f);
	
	switch (z)
	{
		case wxMOUSE_BTN_LEFT:
			{
				switch(ui_fire->GetActiveNotePage())
				{
					case TAG_IGNITION_POINT:
					{
						std::string entity_id;
						if (graphic_object_controller->IntersectMouseWithEntities(x, y, entity_id))
							return;

						graphic_object_controller->IntersectMouseWithScene(x, y, intersection_point);
						fire_entity->AddIgnitionPoint(intersection_point);
						
						if (fire_exists_in_scene)
							fire_entity->GraphicDelete();

						/*fire_entity->GraphicInsert();
						fire_exists_in_scene = true;
						UpdateUI(active_ignition_point, active_perimeter, active_flame );

						return;*/
						
					}
					break;

					case TAG_KEYFLAMES:
					{
						std::string entity_id;
						if (graphic_object_controller->IntersectMouseWithEntities(x, y, entity_id))
							return;

						std::vector< cpw::Point3d<float> > new_points;
						cpw::Point3d<float> last_flame_coords;

						graphic_object_controller->IntersectMouseWithScene(x, y, intersection_point);

						cpw::utils::TimeSlice *ts = fire_entity->GetTimeSlice(ui_fire->GetActiveKeyFlame());

						if (ts != NULL)
						{
							NewFlame(intersection_point, ui_fire->GetActiveKeyFlame());
						}
		
						ts = fire_entity->GetTimeSlice(active_perimeter);
						if (ts != NULL)
							active_flame = (int)ts->flames.size()-1;
					}
					break;
				}
				
				fire_entity->SetPolygonal(true);

				if (!fire_exists_in_scene)
				{
					if (active_perimeter >= 0)
						fire_entity->MakeTimeSliceCurrent(active_perimeter);

					fire_entity->GraphicInsert();
					fire_exists_in_scene = true;
				}
				else
				{
					fire_entity->GraphicDelete();

					if (active_perimeter >= 0)
					{
						fire_entity->MakeTimeSliceCurrent(active_perimeter);
					
						GraphicFireUpdate(active_perimeter);
					}
					else
					{
						fire_entity->GraphicInsert();
					}
					
				}
					
				ui_fire->SetChanges(true);
				UpdateUI(active_ignition_point, active_perimeter, active_flame );

			}
			break; //END LEFT BUTTON
		
		case wxMOUSE_BTN_MIDDLE:
			break;

		case wxMOUSE_BTN_RIGHT:
			break;
	}

}


void FireController::CreateFire()
{
	float x, y, z;
	x = y = z = 0.0f;
	cpw::utils::Fire fire;
	fire_entity = (cpw::utils::Fire *) cpw::EntityFactory::GetInstance()->CreateEntity(fire.GetClassName());
	fire_entity->SetBeingEdited(true);
	fire_entity->SetGraphicFactory(graphic_factory);
	std::ostringstream stro_id;
	parent_str_id = layer_tree->GetActiveParent()->GetUrl();

	std::stringstream parent_group;
	parent_group << layer_tree->GetActiveParent()->GetId();
	fire_entity->AddParent(layer_tree->GetActiveParent());
	new_fire = true;
	fire_entity->SetSmokeTexture(ui_fire->GetSmokeTexture());
	fire_entity->SetFireTexture(ui_fire->GetFireTexture());
	stro_id << fire_entity->GetId();
	str_id = stro_id.str();
	fire_exists_in_scene = false;

	std::string &default_path = ApplicationConfiguration::GetInstance()->GetDefaultDirectory();
	fire_entity->SetIgnitionPointUrl(default_path + "ignition.osg");

	std::string &icon_path = ApplicationConfiguration::GetInstance()->GetIconDirectory();
	std::string icon = icon_path + std::string("flame.png");
	fire_entity->SetIcon(icon);
	
	ui_fire->SetFireName(fire_entity->GetName());
	ui_fire->Show(true);
}

void FireController::Cancel()
{
	ReleaseMouseCallBack();

	if (fire_entity != NULL)
	{
		//delete the fire from the scene
		fire_entity->GraphicDelete();
		delete fire_entity;
		fire_entity = NULL;

	}

	fire_exists_in_scene = false;
	
}

void FireController::UpdateUI(int active_ignition_point, int active_perimeter, int active_flame)
{
	std::vector<wxDateTime> date_times;

	ui_fire->SetCallUpdate(false);
	ui_fire->Clear();
	UpdateUIIgnitionPoints(active_ignition_point);
	UpdateUIFlames(active_perimeter, active_flame);
	
	ui_fire->CheckFlamesValuesInt();

	ui_fire->SetCallUpdate(true);


	for (int i=0; i < fire_entity->GetNumTimeSlice(); i++)
	{
		cpw::utils::TimeSlice *ts = fire_entity->GetTimeSlice(i);
		if (ts != NULL)
		{
			wxDateTime dt;
			dt.Set((time_t)ts->time_stamp.seconds);
			date_times.push_back(dt);
		}
	}

	ui_fire->UpdateTimeLine(date_times);

	return;
			
}

void FireController::UpdateUIIgnitionPoints(int &active_index)
{
	wxListBox *ui_listbox= ui_fire->GetIgnitionPointsListBox();

	if (active_index == -1)
	{
		active_index = ui_listbox->GetSelection();

		if (active_index == wxNOT_FOUND)
			active_index = -1;
	}
	
	ui_listbox->Clear();

	for (int i=0; i<fire_entity->GetNumIgnitionPoints(); i++)
	{
		std::ostringstream ignition_ostr;
		ignition_ostr << "Ignition point " << i;
		std::string ignition = ignition_ostr.str();
		ui_listbox->Append((const wxString &)ignition);
	}

	if (active_index != -1)
	{
		ui_listbox->SetSelection(active_index, true);
		if (ui_fire->GetActiveNotePage() == TAG_IGNITION_POINT)
		{
			cpw::Point3d<float> *ip = fire_entity->GetIgnitionPoint(active_index);
			if (ip != NULL)
			{
				std::ostringstream aux_str;
				aux_str.precision(15);
				
				aux_str << ip->x;
				std::string temp = aux_str.str();
				ui_fire->SetPositionX((std::string &)temp);
				aux_str.str("");

				aux_str << ip->y;
				temp = aux_str.str();
				ui_fire->SetPositionY((std::string &)temp);
				aux_str.str("");

				aux_str << ip->z;
				temp = aux_str.str();
				ui_fire->SetPositionZ((std::string &)temp);
				aux_str.str("");
	
				
			}
		}
	}
}

void FireController::UpdateUIFlames(int &active_perimeter, int &active_flame)
{
	if (active_perimeter == -1)
		active_perimeter = ui_fire->GetActiveKeyFlame();

	if (active_flame == -1)
		active_flame = ui_fire->GetActiveFlame();

	for (int i=0; i<fire_entity->GetNumTimeSlice(); i++)
	{
		ui_fire->AddPerimeter(false);
		for (unsigned int j=0; j<fire_entity->GetTimeSlice(i)->flames.size(); j++)
		{
			ui_fire->AddFlame(i);
		}
	}

	if (active_perimeter != -1)
	{
		ui_fire->SetActive(active_perimeter, active_flame);
		if (ui_fire->GetActiveNotePage() == TAG_KEYFLAMES)
		{
			if (active_perimeter == -1)
				return;

			cpw::utils::TimeSlice *ts = fire_entity->GetTimeSlice(active_perimeter);
			
			if (ts == NULL)
				return;

			wxDateTime time;
			time.Set((time_t)ts->time_stamp.seconds);

			ui_fire->SetDate(time);

			if (active_flame < 0)
				return;

			std::ostringstream aux_str;
			aux_str.precision(5);
			
			FillUIWithFlameData(ts->flames[active_flame]);
		}

	}

	
	if (active_flame >=0)
	{
		if (active_flame > 0)
			ui_fire->DisableEditControls();
		else
			ui_fire->EnableEditControls();
	}

}



void FireController::UpdateFlame(int active_perimeter, int active_flame)
{
	cpw::Point3d<float> position;
	float smoke_life, smoke_size, fire_life, fire_size, intensity_fire, intensity_smoke;
	std::ostringstream stro_id;
	std::string str_id;
	cpw::utils::TimeSlice *ts;
	cpw::utils::Flame *flame;

	if (!update_selected_flame)
		return;
	
	if ((active_flame < 0) || (active_perimeter < 0))
		return;

	ts = fire_entity->GetTimeSlice(active_perimeter);
	
	if (ts == NULL)
		return;

	if ((ts->flames.size()-1) < ((unsigned int)active_flame))
		return;

	flame = &ts->flames[active_flame];

	if (!ui_fire->GetCoordsFromEditControls(position, smoke_life, smoke_size, fire_life, fire_size, intensity_fire, intensity_smoke))
		return;

	flame->fire.SetCoords(cpw::Point3d<float>(position.x, position.y, position.z));
	flame->smoke.SetCoords(cpw::Point3d<float>(position.x, position.y, position.z));
	flame->fire.SetSize(cpw::Range<float>(fire_size, fire_size*FIRE_SIZE_RANGE_COEF));
	flame->fire.SetParticlesLife(fire_life);
	flame->smoke.SetSize(cpw::Range<float>(smoke_size*SMOKE_SIZE_RANGE_COEF, smoke_size));
	flame->smoke.SetParticlesLife(smoke_life);
	flame->fire.SetNewParticlesSec(cpw::Range<int>(intensity_fire*FIRE_PARTICLESSEC_RANGE_COEF, intensity_fire));
	flame->smoke.SetNewParticlesSec(cpw::Range<int>(intensity_smoke*SMOKE_PARTICLESSEC_RANGE_COEF/2, intensity_smoke));
	flame->fire.SetUpdatedInScene(false);
	flame->smoke.SetUpdatedInScene(false);

	UpdateRender(-1, active_perimeter, true);
}


void FireController::UpdateParticleSystemsTypes()
{
	if (fire_entity->GetNumFlames() <= 0)
		return;

	//delete the existing particle system in the scene
	//and create a new one of the required type
	fire_entity->SetPolygonal(true);
	fire_entity->GraphicDelete();
	fire_entity->GraphicInsert();
} 


bool FireController::CreatePermanentFire(const std::string &fire_name, const std::string &fire_texture,const std::string &smoke_texture, const bool &modify)
{
	cancel_overwrite = false;
	//check for invalid fires
	if (fire_entity->GetNumTimeSlice() <= 0)
		return false;

	for (int i=0; i<fire_entity->GetNumTimeSlice(); i++)
	{
		if (fire_entity->GetTimeSlice(i)->flames.size() < 2)
			return false;

	}


	PersistentController persistent;
	fire_entity->SetName(fire_name);
	fire_entity->SetPolygonal(true);
	fire_entity->SetFireTexture(fire_texture);
	fire_entity->SetSmokeTexture(smoke_texture);

	//get the start time
	if (fire_entity->GetNumTimeSlice() > 0)
	{
		cpw::utils::TimeSlice *ts = fire_entity->GetTimeSlice(0);
		fire_entity->SetStartTime(ts->time_stamp);
		fire_entity->SetEndTime(ts->time_stamp);
		for (int i=1; i<fire_entity->GetNumTimeSlice(); i++)
		{
			if (fire_entity->GetTimeSlice(i)->time_stamp.seconds < fire_entity->GetStartTime().seconds)
				fire_entity->SetStartTime(fire_entity->GetTimeSlice(i)->time_stamp);

			if (fire_entity->GetTimeSlice(i)->time_stamp.seconds > fire_entity->GetEndTime().seconds)
				fire_entity->SetEndTime(fire_entity->GetTimeSlice(i)->time_stamp);
		}

		//provisional hasta saber el end time real
		fire_entity->GetEndTime().seconds += 10;
	}

	fire_entity->SetGraphicFactory(graphic_factory);
	fire_entity->SetAnimationPreComputed(false);
	
	cpw::PersistentError ferror = persistent.Save(parent, fire_entity, ".cel", modify);

	if(ferror == cpw::PERSISTENTOK)
	{
		if(!modify)
		{
			Register(fire_entity);
			if (new_fire)
			{
				fire_entity->ClearAllParents();
				layer_tree->AddToActiveLayer(fire_entity);
			}
			
		}


		fire_entity->SetResetAnimation(true);
		fire_entity->SetBeingEdited(false);
		
		fire_entity->GraphicDelete();
		
		if (fire_entity->GetNumTimeSlice() > 0)
			fire_entity->MakeTimeSliceCurrent(0);

		fire_entity->GraphicInsert();

		fire_entity = NULL;
		return true;
	}
	cancel_overwrite = true;
	return false;
}


void FireController::Remove()
{
	switch(ui_fire->GetActiveNotePage())
	{	
		case TAG_IGNITION_POINT:
			RemoveIgnitionPoint();
			break;

		case TAG_KEYFLAMES:
			RemoveFlame();
			break;

		default:
			break;
	}
}

void FireController::RemoveIgnitionPoint()
{
	int ip_count;
	int selected_ip = ui_fire->GetActiveIgnitionPoint();
	
	fire_entity->DeleteIgnitionPoint(selected_ip);
	
	ip_count = fire_entity->GetNumIgnitionPoints();
	
	selected_ip--;

	if (ip_count > 0)
		UpdateUI(selected_ip, -1, -1);
	else
		UpdateUI(-1, -1, -1);

}

void FireController::RemoveFlame()
{
	int selected_flame = ui_fire->GetActiveFlame();
	int selected_time_slice = ui_fire->GetActiveKeyFlame();

	if (selected_flame == -1) //no flame selected, so delete the time slice
	{
		fire_entity->DeleteTimeSlice(selected_time_slice);
		fire_entity->ClearCurrentFlames();
		selected_time_slice--;
		UpdateUI(-1, selected_time_slice, selected_flame-1);
	}
	else
	{
		fire_entity->DeleteFlame(selected_time_slice, selected_flame);
		selected_flame--;
		cpw::utils::TimeSlice *ts = fire_entity->GetTimeSlice(selected_time_slice);
		if (ts != NULL)
		{
			int fl_size = ts->flames.size();
			
			if (fl_size <= 0)
				UpdateUI(-1, selected_time_slice, selected_flame);
			else
			{
				if (selected_flame < 0)
					selected_flame = 0;

				UpdateUI(-1, selected_time_slice, selected_flame);
			}
		}
	}

	fire_entity->GraphicDelete();

	if (selected_time_slice >= 0)
		fire_entity->MakeTimeSliceCurrent(selected_time_slice);

	GraphicFireUpdate(selected_time_slice);
}

void FireController::AddTimeSlice(const unsigned long int &_time)
{
	cpw::cpwTime time;
	time.seconds = _time;
	time.miliseconds = 0;

	if (fire_entity->GetNumTimeSlice() > 0 && (fire_entity->GetLastTimeSlice()->time_stamp).seconds > time.seconds)
	{
		time = fire_entity->GetLastTimeSlice()->time_stamp;
		time.seconds+=1;
	}

	fire_entity->AddTimeSlice();
	fire_entity->GetLastTimeSlice()->time_stamp = time;	
}


void FireController::FillUIWithFlameData(cpw::utils::Flame &flame)
{
	std::ostringstream aux_str;
	aux_str.precision(15);
	aux_str << flame.fire.GetCoords().x;
	std::string tmp = aux_str.str();
	ui_fire->SetPositionX((std::string &)tmp);
	aux_str.str("");
	
	aux_str << flame.fire.GetCoords().y;
	tmp = aux_str.str();
	ui_fire->SetPositionY((std::string &)tmp);
	aux_str.str("");

	aux_str << flame.fire.GetCoords().z;
	tmp = aux_str.str();
	ui_fire->SetPositionZ((std::string &)tmp);
	aux_str.str("");

	aux_str << flame.fire.GetParticlesLife();
	tmp = aux_str.str();
	ui_fire->SetFireLife((std::string &)tmp);
	aux_str.str("");

	aux_str << flame.smoke.GetParticlesLife();
	tmp = aux_str.str();
	ui_fire->SetSmokeLife((std::string &)tmp);
	aux_str.str("");

	aux_str << flame.fire.GetSize().min;
	tmp = aux_str.str();
	ui_fire->SetFireSize((std::string &)tmp);
	aux_str.str("");

	aux_str << flame.smoke.GetSize().max;
	tmp = aux_str.str();
	ui_fire->SetSmokeSize((std::string &)tmp);
	aux_str.str("");

	aux_str << flame.fire.GetNewParticlesSec().max;
	tmp = aux_str.str();
	ui_fire->SetFireIntensity((std::string &)tmp);
	aux_str.str("");

	aux_str << flame.smoke.GetNewParticlesSec().max;
	tmp = aux_str.str();
	ui_fire->SetSmokeIntensity((std::string &)tmp);
	aux_str.str("");
}

void FireController::UpdateRender(int active_ignition_point, int active_perimeter,  bool force_update)
{
	if ((current_perimeter != active_perimeter) || force_update)
	{
		fire_entity->GraphicDelete();

		GraphicFireUpdate(active_perimeter);

		current_perimeter = active_perimeter;
	}
}

void FireController::ChangeDate(const int &active_perimeter, const wxDateTime &new_time)
{
	if (active_perimeter < 0)
		return;

	cpw::utils::TimeSlice *ts = fire_entity->GetTimeSlice(active_perimeter);

	if (ts == NULL)
		return;

	ts->time_stamp.seconds = new_time.GetTicks();
	ts->time_stamp.miliseconds = 0;

	std::vector<wxDateTime> date_times;

	for (int i=0; i < fire_entity->GetNumTimeSlice(); i++)
	{
		cpw::utils::TimeSlice *ts = fire_entity->GetTimeSlice(i);
		if (ts != NULL)
		{
			wxDateTime dt;
			dt.Set((time_t)ts->time_stamp.seconds);
			date_times.push_back(dt);
		}
	}
	ui_fire->UpdateTimeLine(date_times);

}

void FireController::ChangeTextures(const std::string &fire_tex, const std::string &smoke_tex)
{
	if(fire_entity != NULL)
	{
		fire_entity->SetSmokeTexture(smoke_tex);
		fire_entity->SetFireTexture(fire_tex);
		fire_entity->GraphicDelete();
		fire_entity->GraphicInsert();
	}
}

int  FireController::GetFirstKeyFlameIdInTimePeriod(wxDateTime timeMin,wxDateTime timeMax)
{
	int result = -1;
	if(fire_entity != NULL)
	{
		for (int i=0; i < fire_entity->GetNumTimeSlice(); i++)
		{
			cpw::utils::TimeSlice *ts = fire_entity->GetTimeSlice(i);
			if (ts != NULL)
			{
				wxDateTime dt;
				dt.Set((time_t)ts->time_stamp.seconds);
				if (dt >= timeMin && dt <= timeMax)
					return i;
			}
		}
	}
	return result;
}

wxDateTime FireController::GetKeyFlameTimeStamp(int id)
{
	cpw::utils::TimeSlice *ts = fire_entity->GetTimeSlice(id);
	if (ts != NULL)
	{
		wxDateTime result((time_t)ts->time_stamp.seconds);
		result.SetMillisecond(ts->time_stamp.miliseconds);
		if (result.IsValid())
			return result;
	}
	return wxInvalidDateTime;
}
void FireController::SetKeyFlameTimeStamp(int id, wxDateTime time)
{
	cpw::utils::TimeSlice *ts = fire_entity->GetTimeSlice(id);
	if (ts != NULL)
	{
		ts->time_stamp.seconds = time.GetTicks();
		ts->time_stamp.miliseconds = time.GetMillisecond();
	}

}

void FireController::ModifyProperties(cpw::Entity *ent)
{
	cpw::utils::Fire* fire = (cpw::utils::Fire*)ent;
	fire->SetBeingEdited(true);
	new_fire = false;

	ui_fire->Modify(true);
	ui_fire->SetFireName(fire->GetName());
	std::string temp = fire->GetFireTexture();
	ui_fire->SetFireTexture((std::string &)temp);
	temp = fire->GetSmokeTexture();
	ui_fire->SetSmokeTexture((std::string &)temp);

	fire_entity = fire;

	fire_entity_backup = new cpw::utils::Fire(*fire_entity);

	UpdateUI();

	ui_fire->Show(true);

	
	fire_entity->GraphicDelete();
	GraphicFireUpdate(ui_fire->GetActiveKeyFlame());
	
}


void FireController::GraphicFireUpdate(const int &active_perimeter)
{
	std::string original_fire_texture = fire_entity->GetFireTexture();
	std::string original_smoke_texture = fire_entity->GetSmokeTexture();

	for (int i = 0; i < fire_entity->GetNumTimeSlice(); i++)
	{
		if (i == active_perimeter)
		{
			fire_entity->SetFireTexture(original_fire_texture);
			fire_entity->SetSmokeTexture(original_smoke_texture);
		}
		else
		{
			std::string default_directory = cpw::ApplicationConfiguration::GetInstance()->GetDefaultDirectory();
			std::string fire_text = default_directory + "/fire_fake.tif";
			std::string smoke_text = default_directory + "/smoke_fake.tif";

			fire_entity->SetFireTexture(fire_text);
			fire_entity->SetSmokeTexture(smoke_text);
		}
		
		fire_entity->ClearCurrentFlames();
		fire_entity->MakeTimeSliceCurrent(i);

		fire_entity->GraphicInsert();
	}

	fire_entity->SetFireTexture(original_fire_texture);
	fire_entity->SetSmokeTexture(original_smoke_texture);


}


void FireController::RestoreBackup()
{
	ReleaseMouseCallBack();

	fire_entity->ClearCurrentFlames();
	fire_entity->GraphicDelete();
	*fire_entity = *fire_entity_backup;
	
	delete fire_entity_backup;
	fire_entity_backup = NULL;

	fire_entity->SetAnimationPreComputed(false);
	fire_entity->SetResetAnimation(true);
	fire_entity->SetBeingEdited(false);
	
	fire_entity->MakeTimeSliceCurrent(0);
	fire_entity->GraphicInsert();
}

bool FireController::IsOpen()
{
	if(ui_fire != NULL)
		return ui_fire->IsShown();
	else
		return false;
}
