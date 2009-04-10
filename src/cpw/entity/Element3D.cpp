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
//#include <stdafx.h>

#include <cpw/entity/Element3D.h>
#include <cpw/entity/EntityRegistry.h>



using namespace cpw;



void Element3D::GraphicInsertAndRegister()
{
	GraphicInsert();
	cpw::EntityRegistry::GetInstance()->Add (this);
}


void Element3D::GraphicDeleteAndUnregister()
{
	GraphicDelete();
	cpw::EntityRegistry::GetInstance()->Remove (GetId());
}



Element3D::Element3D(const cpw::TypeId id, const std::string &class_name) : 
     Element(id, class_name), model_url()
{
}



Element3D::Element3D(const Element3D &element): Element(element), model_url(ComposeDirectory(element.model_url))
{
}



Element3D::~Element3D()
{
}

std::string Element3D::GetModelUrl()
{
	return ComposeDirectory(model_url);
}

const std::string Element3D::GetModelUrl() const
{
	return ComposeDirectory(model_url);
}

void Element3D::SetModelUrl(const std::string &url)
{
	model_url = url;

	Modified("model_url", model_url);
}


void Element3D::SetValue(const std::string &f, const std::string &v)
{
	if(f == "model_url")
		SetModelUrl(v);
	else
		Element::SetValue(f, v);

}

Element3D &Element3D::operator = (const Element3D &element)
{
	cpw::Element::operator =(element);
	model_url = element.GetModelUrl();
	return *this;
}



int Element3D::CreatePersistence() {

	Element::CreatePersistence();

	AddPersistence((std::string)"model_url", RelativeDirectory(model_url));
	
	return 0;
}


int Element3D::CreateProperties() 
{
	Element::CreateProperties();

	AddPersistenceLevel("3D Model");

	AddPersistence((std::string)"Model", RelativeDirectory(model_url));

	RemovePersistenceLevel();

	return 0;
}


int Element3D::AdaptPersistence(Node *root)
{

	Element::AdaptPersistence(root);

	model_url = (root->GetChildValue("model_url"));

	return 0;

}
