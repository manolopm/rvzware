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
#include "stdafx.h"

#include <controllers/ElementVectorialController.h>
#include <controllers/PersistentController.h>

#include <string>
#include <vector>
#include <wx/listbox.h>
#include <sstream>
#include <cpw/debugger.h>

#include <cpwutils/GraphicFireCallBack.h>
#include <cpw/ApplicationConfiguration.h>

#include <cpw/common/IdGenerator.h>
#include <cpw/common/types.h>
#include <cpw/common/IdGenerator.h>
#include <cpw/entity/EntityRegistry.h>
#include <cpw/entity/EntityFactory.h>
#include <cpw/graphic/ApplicationScene.h>

using namespace cpw::controllers;


/******************************************************************************************************************/
EVControllerEventReceiver::EVControllerEventReceiver(void): drag(false)
{
	
}

EVControllerEventReceiver::~EVControllerEventReceiver(void)
{

}

EVControllerEventReceiver::EVControllerEventReceiver(const EVControllerEventReceiver &dcer)
{

}

bool EVControllerEventReceiver::MouseLeftButtonDown(const int &x,const int &y)
{
	drag = false;

	return false;

}
bool EVControllerEventReceiver::MouseLeftButtonUp(const int &x,const int &y)
{

	if(!drag)
		evc->MouseEvent(x, y, wxMOUSE_BTN_LEFT);
		

	return false;
}

bool EVControllerEventReceiver::MouseDrag(const int &x0, const int &y0, const int &x1, const int &y1, const cpw::MouseButtonsState &mbs)
{
	drag = true;

	return false;
}

void EVControllerEventReceiver::SetDistanceController(ElementVectorialController *_ElementVectorialController)
{ 
	evc = _ElementVectorialController;
}


/******************************************************************************************************************/


ElementVectorialController::ElementVectorialController(wxWindow* _parent, cpw::LayerTree &tr_lay, cpw::IGraphicFactory *graphic_factory, cpw::INavigatorManager *nm, cpw::IGraphicObjectLoader *loader) : 
		parent(_parent), layer_tree(&tr_lay), navigator_manager(nm), ElementVectorial_entity(NULL), ElementVectorial_old(NULL), ElementVectorial_array(),
		graphic_object_loader(loader)
{
	ui_ElementVectorial = new cpw::gui::UIElementVectorial(this,_parent);

	// the mouse event callback
	mouse_event = new EVControllerEventReceiver();
	mouse_event->SetDistanceController(this);
	mouse_event->SetNavigatorManager(navigator_manager);
	cpw::INavigator *navigator = navigator_manager->GetFocusedOrFirstNavigator();
	int nc = navigator->GetEventReceiversCount();
	if (nc>0)
		navigator->SetEventReceiver(nc-1, mouse_event);

	graphic_object_controller = new GraphicObjectController(graphic_object_loader, navigator_manager);
}


ElementVectorialController::~ElementVectorialController()
{
	if (ui_ElementVectorial != NULL)
	{
		delete ui_ElementVectorial;
		ui_ElementVectorial = NULL;
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

	if(ElementVectorial_entity != NULL)
	{
		cpw::ElementVectorial *temp = (cpw::ElementVectorial*)cpw::EntityRegistry::GetInstance()->GetEntity(ElementVectorial_entity->GetId());
		if(temp == NULL)
		{
			delete ElementVectorial_entity;
			ElementVectorial_entity = NULL;
		}
	}

	if(ElementVectorial_old != NULL)
	{
		delete ElementVectorial_old;
		ElementVectorial_old = NULL;
	}
	

	ReleaseMouseCallBack();

	if(mouse_event != NULL)
	{
		delete mouse_event;
		mouse_event = NULL;
	}
}

void ElementVectorialController::ReleaseMouseCallBack()
{
	if(navigator_manager != NULL)
	{
		cpw::INavigator *navigator = navigator_manager->GetFocusedOrFirstNavigator();
		navigator->RemoveEventReceiver(mouse_event);
	}
}


void ElementVectorialController::MouseEvent(int x, int y, int z)
{
	float ipx, ipy, ipz;
	ipx=ipy=ipz=0.0f;
	
	switch (z)
	{
		case wxMOUSE_BTN_LEFT:
			{	
				std::string entity_id;
				//if (navigator_manager->GetFocusedOrFirstNavigator()->IntersectMouseWithEntities(x, y, entity_id))
				//	return;

				if (navigator_manager->GetFocusedOrFirstNavigator()->IntersectMouseWithScene(x,y, ipx, ipy, ipz))
				{
					ElementVectorial_array.push_back(cpw::Point3d<float>(ipx, ipy, ipz));
				
					ElementVectorial_entity->SetPoints(ElementVectorial_array);
					if(ElementVectorial_entity->GetNumPoints() <= 2)
						ElementVectorial_entity->GraphicInsert();
					else
					{
						ElementVectorial_entity->UpdatePoint(-1);
						ElementVectorial_entity->GraphicUpdate();
					}
						

					UpdateUI(cpw::Point3d<float>(ipx, ipy, ipz));
				}
				return;
			}
			break; //END LEFT BUTTON
		
		case wxMOUSE_BTN_MIDDLE:
			break;

		case wxMOUSE_BTN_RIGHT:
			break;
	}

}



void ElementVectorialController::CreateElementVectorial()
{
	cpw::ElementVectorial ElementVectorial;
	ElementVectorial_entity = (cpw::ElementVectorial *) cpw::EntityFactory::GetInstance()->CreateEntity(ElementVectorial.GetClassName());
	
	if (ElementVectorial_entity == NULL) return;
	
	std::ostringstream stro_id;
	parent_str_id = layer_tree->GetActiveParent()->GetUrl();

	std::string &icon_path = ApplicationConfiguration::GetInstance()->GetIconDirectory();
	std::string icon = icon_path + vicon;
	ElementVectorial_entity->SetIcon(icon);
	ElementVectorial_entity->SetRGBColour(255,255,255);
	
	ui_ElementVectorial->SetElementVectorialName(ElementVectorial_entity->GetName());
	ui_ElementVectorial->SetElementVectorialType(vname);
	ElementVectorial_entity->SetVectorialType(vname);    
	ui_ElementVectorial->SetElementVectorialIcon(vicon, vicon_child);
	ElementVectorial_entity->SetvectorialIcons(vicon, vicon_child);
	ui_ElementVectorial->Show(true);
}

void ElementVectorialController::Cancel()
{
	ReleaseMouseCallBack();

	if (ElementVectorial_entity != NULL)
	{
		ElementVectorial_entity->GraphicDelete();
		
		delete ElementVectorial_entity;
		ElementVectorial_entity = NULL;
	}
}

void ElementVectorialController::CancelModifications()
{
	ReleaseMouseCallBack();
	ElementVectorial_entity->GraphicDelete();

	ElementVectorial_entity->SetWidth(ElementVectorial_old->GetWidth());
	ElementVectorial_entity->SetColor(ElementVectorial_old->GetColor());
	ElementVectorial_entity->SetPoints(ElementVectorial_old->GetPoints());
	ElementVectorial_entity->GraphicInsert();

	delete ElementVectorial_old;
	ElementVectorial_old = NULL;
	
}

void ElementVectorialController::UpdateUI(const cpw::Point3d<float> &point)
{
	if(ElementVectorial_array.empty()) return;

	cpw::Point3d<float> p = ElementVectorial_array.back();

	ui_ElementVectorial->AddPoint();
	std::ostringstream aux_str;
	aux_str.precision(15);
	
	aux_str << p.x;
	ui_ElementVectorial->SetPositionX(aux_str.str());
	aux_str.str("");

	aux_str << p.y;
	ui_ElementVectorial->SetPositionY(aux_str.str());
	aux_str.str("");

	aux_str << p.z;
	ui_ElementVectorial->SetPositionZ(aux_str.str());
	aux_str.str("");		
}

void ElementVectorialController::UpdateUIPoint(const int &index, const bool &change_point)
{
	if(ElementVectorial_array.empty()) return;
	if((index <0)||(index > (int)ElementVectorial_array.size())) return;

	if(change_point)
	{
		cpw::Point3d<float> p = ElementVectorial_array[index];
		
		std::ostringstream aux_str;
		aux_str.precision(15);
		
		aux_str << p.x;
		ui_ElementVectorial->SetPositionX(aux_str.str());
		aux_str.str("");

		aux_str << p.y;
		ui_ElementVectorial->SetPositionY(aux_str.str());
		aux_str.str("");

		aux_str << p.z;
		ui_ElementVectorial->SetPositionZ(aux_str.str());
		aux_str.str("");		
	}
	else
	{
		cpw::Point3d<float> pos;

		if(!ui_ElementVectorial->GetCoordsFromEditControls(pos)) return;

		ElementVectorial_array[index] = pos;

		ElementVectorial_entity->UpdatePoint(index);
		ElementVectorial_entity->SetPoints(ElementVectorial_array);
		ElementVectorial_entity->GraphicUpdate();
	}
}


void ElementVectorialController::CreatePermanentElementVectorial(const std::string &ElementVectorial_name, const std::string &color, const int &width, const bool &modify)
{
	PersistentController persistent;
	ElementVectorial_entity->SetName(ElementVectorial_name);
	ElementVectorial_entity->SetColor(color);
	ElementVectorial_entity->SetWidth(width);
	ElementVectorial_entity->SetPoints(ElementVectorial_array);
	
	if(!modify)
	{
		Register(ElementVectorial_entity);
		layer_tree->AddToActiveLayer(ElementVectorial_entity);
	}

	cpw::PersistentError ferror = persistent.Save(parent, ElementVectorial_entity, ".cel", modify);

	ElementVectorial_entity = NULL;
}

void ElementVectorialController::Remove(const int &index)
{
	if(ElementVectorial_array.empty()) return;
	if((index <0)||(index > (int)(ElementVectorial_array.size()-1))) return;

	std::vector<cpw::Point3d<float>>::iterator iter=ElementVectorial_array.begin();
	int i=0;
	while(i!=index){i++; iter++;}

	ElementVectorial_array.erase(iter);
	ui_ElementVectorial->RemovePoint();

	ElementVectorial_entity->SetPoints(ElementVectorial_array);

	if(ElementVectorial_array.size() <= 1)
	{
		ElementVectorial_entity->GraphicDelete();
	}
	else
	{
		ElementVectorial_entity->GraphicInsert();
	}
}


void ElementVectorialController::ModifyProperties(cpw::Entity *ent)
{
	cpw::ElementVectorial* ElementVectorial = (cpw::ElementVectorial*)ent;

	ElementVectorial_old = new cpw::ElementVectorial(*ElementVectorial);

	ui_ElementVectorial->Modify(true);
	ui_ElementVectorial->SetElementVectorialName(ElementVectorial->GetName());	

	ElementVectorial_entity = ElementVectorial;
	ElementVectorial_array = ElementVectorial->GetPoints();

	ui_ElementVectorial->SetElementVectorialType(ElementVectorial_entity->GetVectorialType());
	std::string vicon1, vicon2;
	ElementVectorial_entity->GetVectorialIcons(vicon1, vicon2);
	ui_ElementVectorial->SetElementVectorialIcon(vicon1, vicon2);

	ui_ElementVectorial->FillUI(ElementVectorial_array.size(), ElementVectorial->GetColor(), ElementVectorial->GetWidth());
	
	ui_ElementVectorial->Show(true);
}

void ElementVectorialController::SetRGBColour(const int &r, const int &g, const int &b)
{
	ElementVectorial_entity->SetRGBColour(r,g,b);
}

void ElementVectorialController::SetVectorialName(const std::string &name)
{
	vname = name;
}

void ElementVectorialController::SetVectorialIcon(const std::string &icon, const std::string &icon2)
{
	vicon = icon;
	vicon_child = icon2;
}


bool ElementVectorialController::IsOpen()
{
	if(ui_ElementVectorial != NULL)
		return ui_ElementVectorial->IsShown();
	else
		return false;
}