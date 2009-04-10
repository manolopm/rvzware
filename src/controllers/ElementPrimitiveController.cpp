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

#include <controllers/ElementPrimitiveController.h>
#include <controllers/PersistentController.h>

#include <gui/UIElementPrimitive.h>

#include <cpw/common/Attribute.h>
#include <cpw/entity/Element3D.h>
#include <cpw/entity/EntityFactory.h>
#include <cpw/ApplicationConfiguration.h>



using namespace cpw::controllers;


ElementPrimitiveController::ElementPrimitiveController() 
{
}

ElementPrimitiveController::~ElementPrimitiveController()
{
}


void ElementPrimitiveController::CreateElementPrimitive(wxWindow* parent) {

	//Create the form for the new primitive
	cpw::gui::UIElementPrimitive new_dlg(parent,1, wxT("Element Primitive"),wxDefaultPosition);
	std::stringstream name;
	name << "ElementPrimitive" << cpw::EntityFactory::GetInstance()->GetEntityNumber();
	new_dlg.SetElementPrimitiveName(name.str());

	if(new_dlg.ShowModal() == wxID_OK)
	{
		cpw::Element3D ep;
		cpw::Element3D* new_element_primitive = (cpw::Element3D*)cpw::EntityFactory::GetInstance()->CreateEntity(ep.GetClassName());
		
		// Read data form and create the new primitive element
		new_element_primitive->SetName(new_dlg.GetPrimitiveName());
		((cpw::Element3D*)new_element_primitive)->SetModelUrl(new_dlg.GetModelPath());
		wxString wxstr(new_dlg.GetIconPath().c_str(), wxConvUTF8);
		wxIcon icon(wxstr, wxBITMAP_TYPE_ANY);
		if (icon.IsOk())
			new_element_primitive->SetIcon(new_dlg.GetIconPath());
		else
			new_element_primitive->SetIcon(ApplicationConfiguration::GetInstance()->GetIconDirectory()+"default.png");
		//if (new_dlg.GetIconPath() != "")
		//	new_element_primitive->SetIcon(new_dlg.GetIconPath());
		//else
		//	new_element_primitive->SetIcon(ApplicationConfiguration::GetInstance()->GetIconDirectory()+"default.png");
		((cpw::Element3D*)new_element_primitive)->SetFont(new_dlg.GetFontType());
		new_element_primitive->SetDescription(new_dlg.GetDescription());
		std::vector<cpw::Attribute> attr = new_dlg.GetAttributes();
		new_element_primitive->SetAttributes(attr);

		PersistentController persistent;

		persistent.Save(parent, new_element_primitive, ".cel", false, true);

		cpw::EntityFactory::GetInstance()->Register(new_element_primitive);
	}
}


bool ElementPrimitiveController::IsOpen()
{
	return false;	
}

