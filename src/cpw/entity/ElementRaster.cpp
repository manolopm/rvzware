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

#include <cpw/entity/ElementRaster.h>


using namespace cpw;

ElementRaster::ElementRaster(const cpw::TypeId id, const std::string &class_name) : 
     Element(id, class_name)
{
}



ElementRaster::ElementRaster(const ElementRaster &element): Element(element)
{
}



ElementRaster::~ElementRaster()
{
}

void ElementRaster::SetValue(const std::string &f, const std::string &v)
{
	//if(f == "model_url")
	//	SetModelUrl(v);
	//else
	//	Element::SetValue(f, v);
}

ElementRaster &ElementRaster::operator = (const ElementRaster &element)
{
	cpw::Element::operator =(element);
	return *this;
}



int ElementRaster::CreatePersistence() {

	Element::CreatePersistence();

	return 0;
}

int ElementRaster::AdaptPersistence(Node *root)
{

	Element::AdaptPersistence(root);

	return 0;
}
