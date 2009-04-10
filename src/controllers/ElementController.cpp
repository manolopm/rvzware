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

#include "stdafx.h"

#include <controllers/ElementController.h>
#include <controllers/PersistentController.h>

#include <cpw/entity/EntityFactory.h>
#include <cpw/entity/EntityRegistry.h>
#include <cpw/graphic/ApplicationScene.h>
#include <cpw/graphic/IScene.h>
#include <cpw/common/types.h>

using namespace cpw::controllers;


ElementController::ElementController(wxWindow* paren, cpw::LayerTree &tr_lay, 
									 cpw::IGraphicFactory *graphic_factory, 
									 cpw::INavigatorManager *nm,
									 HandlerController *_handler_controller, bool modify): 
		parent(paren), layer_tree(&tr_lay), navigator_manager(nm), 
		graphic_object_controller(), element3d(NULL), graphic(), new_element(true), old_el3d(), changing(false), primitive(NULL),
		handler_controller(_handler_controller)
{
	ui_element = new cpw::gui::UIElement(this, parent, wxID_ANY, wxString(_T("Element")), wxDefaultPosition, wxSize(390, 360)); 
	
	if (!modify)
	{
		name << "Element" << cpw::EntityFactory::GetInstance()->GetEntityNumber();
		ui_element->SetEntityName(name.str());
	}
			
	graphic_object_loader = graphic_factory->CreateObjectLoader();	
	
	graphic_object_controller = new cpw::controllers::GraphicObjectController(graphic_object_loader, navigator_manager);
}


ElementController::~ElementController()
{
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
	if (element3d != NULL) 
	{
		if (observing)
			element3d->Detach(this);

		cpw::Element3D *temp = (cpw::Element3D*)cpw::EntityRegistry::GetInstance()->GetEntity(element3d->GetId());
		if(temp == NULL)
		{
			delete element3d; 
			element3d = NULL;
		}
	}

	if(old_el3d != NULL)
	{
		delete old_el3d;
		old_el3d = NULL;
	}

	delete ui_element;
	ui_element = NULL;

}

bool ElementController::InstanceElement(const cpw::Entity *primitive)
{
		if (primitive !=NULL)
		{
			ui_element->SetAttributes(primitive->GetAttributes());

			CreateElement(primitive);

			InsertElementInScene();

			//show the element dialog		
			ui_element->SetAttributes(primitive->GetAttributes());
			ui_element->SetEntityName(element3d->GetName());

			return true;
		}
		else
			return false;

}


bool ElementController::ChangePrimitive(const cpw::TypeId &primitive_id)
{
		primitive = cpw::EntityFactory::GetInstance()->GetPrototype(primitive_id);

		return InstanceElement(primitive);
}


bool ElementController::ChangePrimitive(const std::string &primitive_url)
{
	
		primitive = cpw::EntityFactory::GetInstance()->GetPrototypeFromUrl(primitive_url);

		if (primitive == NULL && primitive_url != "")
		{
			PersistentController persistent;
			cpw::PersistentError error = persistent.Load((cpw::Persistent**)&primitive, primitive_url);
			if(error == cpw::PERSISTENTOK)
			{
				cpw::EntityFactory::GetInstance()->Register(primitive);
				UpdateUIPrimitives(primitive->GetName());
			}
			else
			{
				wxMessageDialog message1(NULL,wxString("Cannot load the primitive."), wxString("Capaware"),wxICON_WARNING |wxOK);				
				message1.ShowModal();
			}
		}
		else 
			UpdateUIPrimitives(primitive->GetName());


		return InstanceElement(primitive);
}


void ElementController::CreateElement() 
{
	UpdateUIPrimitives();
	ui_element->Show();
}

void ElementController::CreateElement(const cpw::Entity *primitive) 
{
	if (handler_controller != NULL)
		handler_controller->ClearActiveEntity();

	if (element3d!=NULL)
		DeleteElement();
		
	element3d = (cpw::Element3D*) cpw::EntityFactory::GetInstance()->CreateEntity(primitive->GetId());					
	//----------
	//std::stringstream name2;
	//name2 << "Element" << cpw::EntityFactory::GetInstance()->GetEntityNumber();
	//element3d->SetName(name2.str());
	//element3d->SetName(name.str());
	element3d->SetName(ui_element->GetEntityName());
	element3d->SetPrimitiveUrl(primitive->GetUrl());
	element3d->AddParent(layer_tree->GetActiveParent());
	new_element = true;
	
	cpw::EntityRegistry::GetInstance()->Add(element3d);

	//para el gizmo
	observing = true;
	element3d->Attach(this);
	changing=false;

	
	
	ui_element->SetLinkHeightCheck( element3d->GetAdjustToTerrainHeight() );
	ui_element->SetOverTerrainCheck( element3d->GetAllowUnderTerrain() );

}



void ElementController::InsertElementInScene()
{
	float x, y, z;
	x = y = z = 0.0f;

	cpw::Point3d<float> intersection_point;
	if (graphic_object_controller->IntersectCenterCoords(intersection_point))
	{
		x = intersection_point.x;
		y = intersection_point.y;
		z = intersection_point.z;
	}

	ui_element->SetPos(x, y, z);
					
	float utm[3]={x,y,z};
	float orientation[3]={0.0f, 0.0f, 0.0f};
	float scale[3]={1.0f, 1.0f, 1.0f};
	element3d->SetUtm(utm);
	element3d->SetOrientation(orientation);
	element3d->SetScale(scale);

	//element3d->GraphicDelete();
	element3d->GraphicInsert();

	UpdateHandlerProperties(true);
}


void ElementController::UpdateUIPrimitives(const std::string &selected_primitive) 
{
	cpw::Element3D el3d;

	std::map<cpw::TypeId, cpw::Entity *> prototypes = cpw::EntityFactory::GetInstance()->GetPrototypes(el3d.GetClassName());

	std::map<std::string, cpw::TypeId> uiv;		

	for(std::map<cpw::TypeId, cpw::Entity *>::iterator i = prototypes.begin(); i != prototypes.end(); i++)
		uiv[(i->second->GetName())] = i->first; 
	
	ui_element->SetPrimitivesUrl(uiv, selected_primitive);
}


bool ElementController::CreateElement(const std::string &name, const std::string &text, 
									  const std::string &html, const std::string &description,
								      float *utm, float *orientation, float *scale,
								      bool dynamic, std::vector<cpw::Attribute> &attributes, bool modify) 
{

	if(element3d != NULL) 
	{
		observing = false;
		element3d->Detach(this);		
		
		element3d->SetName(name);
		element3d->SetUtm(utm);
		element3d->SetOrientation(orientation);
		element3d->SetScale(scale);
		element3d->SetText(text);
		element3d->SetHtml(html);
		element3d->SetDynamic(dynamic);
		element3d->SetDescription(description);
		element3d->SetAttributes(attributes);

		element3d->SetAdjustToTerrainHeight( ui_element->GetLinkHeightCheck() );
		element3d->SetAllowUnderTerrain( ui_element->GetOverTerrainCheck() );


		if(!modify)
		{
			if (new_element)
			{
				element3d->ClearAllParents();
				Register(element3d);
				layer_tree->AddToActiveLayer(element3d);
				
			}
		}

		PersistentController persistent;

		cpw::PersistentError ferror = persistent.Save(parent, element3d, ".cel", modify);
		ui_element->Modify(false);
		element3d = NULL;

		handler_controller->ShowActiveHandler(false);
	}
	else
	{
		wxMessageDialog message1(NULL,wxString("Cannot allocate memory for the element.\nClose other applications and try again."), wxString("Warning"),wxICON_EXCLAMATION |wxOK);				
		message1.ShowModal();
		element3d = NULL;
		return false;
	}

	return true;
}


void ElementController::Cancel()
{
	DeleteElement();
}



void ElementController::CancelModifications()
{
	if (old_el3d == NULL) return;

	element3d->SetUtmd(old_el3d->GetUtmd());
	element3d->SetOrientation(old_el3d->GetOrientation());
	element3d->SetScale(old_el3d->GetScale());
	element3d->GraphicDelete();
	element3d->GraphicInsert();
	
	observing = false;
	element3d->Detach(this);	

	delete old_el3d;
	old_el3d = NULL;
}

void ElementController::SetPositionOrientationScale(float  pos_x, float  pos_y, float  pos_z, 
													float  orientation_x, float  orientation_y, float  orientation_z,
													float  scale_x, float  scale_y, float  scale_z)
{
	if (element3d!=NULL)
	{
		float utm[3]={pos_x, pos_y, pos_z};
		float orientation[3]={orientation_x, orientation_y, orientation_z};
		float scale[3]={scale_x, scale_y, scale_z};
		element3d->SetUtm(utm);
		element3d->SetOrientation(orientation);
		element3d->SetScale(scale);

		if (ui_element->GetOverTerrainCheck())
		{
			cpw::Point3d<float> p0, p1, ip;
			p0.x = utm[0]; p0.y = utm[1]; p0.z = utm[2]-10000.0f;
			p1.x = utm[0]; p1.y = utm[1]; p1.z = utm[2]+10000.0f;

			cpw::IScene *scene = cpw::ApplicationScene::GetInstance()->GetScene();
			
			if (scene->IntersectRayWithTerrain(p1, p0, ip, true))
			{
				if (ui_element->GetLinkHeightCheck() || ( utm[2] > element3d->GetUtm(2)))
				{
					utm[2] = ip.z + 0.5f;
					element3d->SetUtm(utm);
				}
			}
		}
		


		element3d->GraphicDelete();
		element3d->GraphicInsert();
	}

}

void ElementController::SetName(const std::string &name)
{
	if (element3d!=NULL)
	{
		element3d->SetName(name);
		element3d->GraphicDelete();
		element3d->GraphicInsert();
	}
}

void ElementController::SetNamePositionOrientationScale(const std::string &name, float  pos_x, float  pos_y, float  pos_z, 
													float  orientation_x, float  orientation_y, float  orientation_z,
													float  scale_x, float  scale_y, float  scale_z)
{
	if (element3d!=NULL)
	{
		float utm[3]={pos_x, pos_y, pos_z};
		float orientation[3]={orientation_x, orientation_y, orientation_z};
		float scale[3]={scale_x, scale_y, scale_z};
		element3d->SetUtm(utm);
		element3d->SetOrientation(orientation);
		element3d->SetScale(scale);
		element3d->SetName(name);

		element3d->GraphicDelete();
		element3d->GraphicInsert();
	}

}

void ElementController::ModifyProperties(cpw::Entity* ent)
{
	cpw::Element3D* el3d = (cpw::Element3D*)ent;
	element3d = el3d;

	old_el3d = new cpw::Element3D (*el3d);

	ui_element->Modify(true);
	ui_element->SetEntityName(el3d->GetName());
	ui_element->SetText(el3d->GetText());
	ui_element->SetHtml(el3d->GetHtml());
	ui_element->SetDescription(el3d->GetDescription());
	ui_element->SetAttributes(el3d->GetAttributes());
	ui_element->SetLinkHeightCheck(el3d->GetAdjustToTerrainHeight());
	ui_element->SetOverTerrainCheck(el3d->GetAllowUnderTerrain());
	new_element = false;

	ui_element->LockTestCoords();
	float x, y, z;
	x = el3d->GetUtm(0);
	y = el3d->GetUtm(1);
	z = el3d->GetUtm(2);
	ui_element->SetPos(x, y, z);
	ui_element->SetOrientation(el3d->GetOrientation(0),el3d->GetOrientation(1),el3d->GetOrientation(2));
	ui_element->SetScale(el3d->GetScale(0),el3d->GetScale(1),el3d->GetScale(2));
	ui_element->LockTestCoords();

	observing = true;
	el3d->Attach(this);
	changing=false;

	ui_element->Show(true);

	if (handler_controller != NULL)
	{
		handler_controller->SetActiveEntity(element3d->GetId());
		handler_controller->ShowDefaultGizmoOnActiveEntity();

		handler_controller->SetAllowUnderTerrainToAllHandles( ui_element->GetLinkHeightCheck(), false);
		handler_controller->SetAdaptPositionToTerrainHeightToAllHandles( ui_element->GetOverTerrainCheck(), false );
	}

}

void ElementController::Update(bool subject_deleted)
{
	if ((element3d != NULL) && (!changing))
	{
		changing = true;
		ui_element->SetCheckCoords(false);
		ui_element->SetPos( element3d->GetUtm(0), element3d->GetUtm(1), element3d->GetUtm(2) );
		ui_element->SetOrientation( element3d->GetOrientation(0), element3d->GetOrientation(1), element3d->GetOrientation(2) );
		ui_element->SetScale( element3d->GetScale(0), element3d->GetScale(1), element3d->GetScale(2) );
		ui_element->SetCheckCoords(true);
		changing = false;
	}
}

void ElementController::DeleteElement()
{
	if (element3d != NULL)
	{
		observing = false;
		element3d->GraphicDelete();
		element3d->Detach(this);		
		cpw::EntityRegistry::GetInstance()->DeleteEntity(element3d->GetId());
		element3d = NULL;

		handler_controller->ShowActiveHandler(false);
	}
}



void ElementController::UpdateHandlerProperties(bool show_default)
{
	if (element3d != NULL)
	{
		element3d->SetAllowUnderTerrain( ui_element->GetOverTerrainCheck() );
		element3d->SetAdjustToTerrainHeight(ui_element->GetLinkHeightCheck() );
	}

	if (handler_controller != NULL)
	{
		if (navigator_manager != NULL)
			handler_controller->UpdateCamera();

		handler_controller->SetActiveEntity(element3d->GetId());
		
		if(show_default) 
			handler_controller->ShowDefaultGizmoOnActiveEntity();
		
		handler_controller->SetAllowUnderTerrainToAllHandles( ui_element->GetOverTerrainCheck());
		handler_controller->SetAdaptPositionToTerrainHeightToAllHandles( ui_element->GetLinkHeightCheck(), true );
	}

	
}

bool ElementController::IsOpen()
{
	if(ui_element != NULL)
		return ui_element->IsShown();
	else
		return false;
}