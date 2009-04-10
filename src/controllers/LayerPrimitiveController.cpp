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
#include <sstream>

#include "stdafx.h"

#include <controllers/LayerPrimitiveController.h>
#include <controllers/PersistentController.h>

#include <gui/UILayerPrimitive.h>

#include <cpw/entity/ContainerLayer.h>
#include <cpw/entity/EntityFactory.h>
#include <cpw/ApplicationConfiguration.h>


using namespace cpw::controllers;


LayerPrimitiveController::LayerPrimitiveController()
{
}

LayerPrimitiveController::~LayerPrimitiveController()
{
}

void LayerPrimitiveController::CreateContainerLayerPrimitive(wxWindow* parent)
{
	//Create the form for the new Container primitive
	
	cpw::gui::UILayerPrimitive new_dlg(parent,1, wxString("Container Layer Primitive"),wxDefaultPosition);

	std::stringstream name;
	name << "ContainerLayerPrimitive" << cpw::EntityFactory::GetInstance()->GetEntityNumber();
	new_dlg.SetContainerLayerPrimitiveName(name.str());

	if(new_dlg.ShowModal() == wxID_OK)
	{
		cpw::ContainerLayer lp;
		cpw::ContainerLayer *new_layer_primitive = (cpw::ContainerLayer*) cpw::EntityFactory::GetInstance()->CreateEntity(lp.GetClassName());
		
		// Read data form and create the new primitive element
		new_layer_primitive->SetName(new_dlg.GetPrimitiveName());
		wxIcon icon(wxT(new_dlg.GetIconPath()), wxBITMAP_TYPE_ANY);
		if (icon.IsOk())
			new_layer_primitive->SetIcon(new_dlg.GetIconPath());
		else
			new_layer_primitive->SetIcon(ApplicationConfiguration::GetInstance()->GetIconDirectory()+"default.png");		
		//if (new_dlg.GetIconPath()!="")
		//	new_layer_primitive->SetIcon(new_dlg.GetIconPath());
		//else
		//	new_layer_primitive->SetIcon(ApplicationConfiguration::GetInstance()->GetIconDirectory()+"default.png");
		new_layer_primitive->SetDescription(new_dlg.GetDescription());
		new_layer_primitive->SetAttributes(new_dlg.GetAttributes());

		PersistentController persistent;

		persistent.Save(parent, new_layer_primitive, ".cla", false, true);

		cpw::EntityFactory::GetInstance()->Register(new_layer_primitive);
	}
}

void LayerPrimitiveController::CreateRasterLayerPrimitive(wxWindow* parent)
{

}

void LayerPrimitiveController::CreateVectorialLayerPrimitive(wxWindow* parent)
{

}


bool LayerPrimitiveController::IsOpen()
{
	return false;
}