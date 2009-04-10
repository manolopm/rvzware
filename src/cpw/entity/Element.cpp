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

#include <cpw/entity/Element.h>


using namespace cpw;

Element::Element(const cpw::TypeId id, const std::string &class_name) : 
	Entity(id, class_name), text(), allow_under_terrain(true), adjust_height_to_terrain(true) {
}


Element::Element(const cpw::TypeId id, const std::string &nam, 
				 const std::string &prim, 
			     const float *ut, const float *orient, const float *sca,
			     const std::string &tex, const std::string &htm,	
   		         const bool dyn, const bool vis, const bool anim, const std::string &desc, const std::vector<Attribute> &attr, 
				 const bool &_allow_under_terrain,
				 const bool &_adjust_height_to_terrain,
				 const std::string &class_name): 
		         Entity(id, nam, prim, htm, dyn, vis, anim, desc, attr, class_name), text(tex),
				 allow_under_terrain(_allow_under_terrain),
				 adjust_height_to_terrain(_adjust_height_to_terrain)
{

	utm[0]         = ut[0];
	utm[1]         = ut[1];
	utm[2]         = ut[2];
	orientation[0] = orient[0];
	orientation[1] = orient[1];
	orientation[2] = orient[2];
	scale[0]       = sca[0];
	scale[1]       = sca[1];
	scale[2]       = sca[2];

	
	
	//Modified();
}



Element::Element(const Element &element): Entity(element), 
										  text(element.text), 
										  allow_under_terrain(element.allow_under_terrain),
										  adjust_height_to_terrain(element.adjust_height_to_terrain)
{

	utm[0]         = element.utm[0];
	utm[1]         = element.utm[1];
	utm[2]         = element.utm[2];
	orientation[0] = element.orientation[0];
	orientation[1] = element.orientation[1];
	orientation[2] = element.orientation[2];
	scale[0]       = element.scale[0];
	scale[1]       = element.scale[1];
	scale[2]       = element.scale[2];
	
	//Modified();
}



Element::~Element(){

}


Element &Element::operator = (const Element &element) {

	Entity::operator =(element);

	utm[0]         = element.utm[0];
	utm[1]         = element.utm[1];
	utm[2]         = element.utm[2];
	orientation[0] = element.orientation[0];
	orientation[1] = element.orientation[1];
	orientation[2] = element.orientation[2];
	scale[0]       = element.scale[0];
	scale[1]       = element.scale[1];
	scale[2]       = element.scale[2];
	text	       = element.text;

	allow_under_terrain = element.allow_under_terrain;
	adjust_height_to_terrain = element.adjust_height_to_terrain;

	//Modified();

	return *this;
}



//float *Element::GetUtm() { return (float)utm; }

double *Element::GetUtmd() { return utm; }


float Element::GetUtm(int i) { return (float) utm[i]; }
double Element::GetUtmd(int i) { return utm[i]; }

float *Element::GetOrientation() { return orientation; }

float &Element::GetOrientation(int i) { return orientation[i]; }

float *Element::GetScale() { return scale; }

float &Element::GetScale(int i) { return scale[i]; }

std::string &Element::GetText() { return text; }

//const float *Element::GetUtm() const { return utm; }

const double *Element::GetUtmd() const { return utm; }

const float Element::GetUtm(int i) const { return (float) utm[i]; }

const float *Element::GetOrientation() const { return orientation; }

const float &Element::GetOrientation(int i) const { return orientation[i]; }

const float *Element::GetScale() const { return scale; }

const float &Element::GetScale(int i) const { return scale[i]; }

const std::string &Element::GetText() const { return text; }

void Element::SetUtm(const float *value) 
{ 
	SetUtm(value[0], value[1], value[2]); 

}

void Element::SetUtm(const float x,const float y,const float z) 
{
	utm[0] = x; utm[1] = y; utm[2] = z;

	Modified("utm", x, y, z);
}

void Element::SetUtmd(const double *value) 
{ 
	SetUtmd(value[0], value[1], value[2]); 

}

void Element::SetUtmd(const double x,const double y,const double z) 
{
	utm[0] = x; utm[1] = y; utm[2] = z;

	Modified("utm", x, y, z);
}

void Element::SetOrientation(const float *value) 
{ 
	orientation[0] = value[0]; orientation[1] = value[1]; orientation[2] = value[2]; 

	Modified("orientation", orientation[0], orientation[1], orientation[2]);
}

void Element::SetOrientation(const float &x, const float &y, const float &z) 
{ 
	orientation[0] = x; orientation[1] = y; orientation[2] = z; 

	Modified("orientation", orientation[0], orientation[1], orientation[2]);
}


void Element::SetScale(const float *value) 
{ 
	scale[0] = value[0]; scale[1] = value[1]; scale[2] = value[2]; 

	Modified("scale", scale[0], scale[1], scale[2]);
}
void Element::SetScale(const float &x, const float &y, const float &z)
{
	scale[0] = x; scale[1] = y; scale[2] = z; 

	Modified("scale", scale[0], scale[1], scale[2]);
}

void Element::SetText(const std::string &str) 
{ 
	text = str; 

	Modified("text", str); 
}


void Element::SetValue(const std::string &f, const std::string &v)
{
	float value[3];
	if(f == "utm")
	{
		std::istringstream str(v);
		str >> value[0] >> value[1] >> value[2];
		SetUtm(value);
	}
	else if (f == "orientation")
	{
		std::istringstream str(v);
		str >> value[0] >> value[1] >> value[2];
		SetOrientation(value);
	}
	else if(f == "scale")
	{
		std::istringstream str(v);
		str >> value[0] >> value[1] >> value[2];
		SetScale(value);
	}
	else if(f == "text")
		SetText(v); 
	else 
		Entity::SetValue(f, v);

}


Entity* Element::Clone() {return new Element(*this);}



int Element::CreatePersistence() {

	Entity::CreatePersistence();

	std::ostringstream str_utm, str_orient, str_scale;
	str_utm.precision(15);
	str_orient.precision(15);
	str_scale.precision(15);
	str_utm << utm[0] << " " << utm[1]<< " " << utm[2];
	AddPersistence((std::string)"utm", str_utm.str());
	str_orient << orientation[0] << " " << orientation[1]<< " " << orientation[2];
	AddPersistence((std::string)"orientation", str_orient.str());
	str_scale << scale[0] << " " << scale[1]<< " " << scale[2];
	AddPersistence((std::string)"scale", str_scale.str());
	AddPersistence((std::string)"text", text);

	if (allow_under_terrain)
		AddPersistence((std::string)"allow_under_terrain", std::string("1"));
	else
		AddPersistence((std::string)"allow_under_terrain", std::string("0"));

	if (adjust_height_to_terrain)
		AddPersistence((std::string)"adjust_height_to_terrain", std::string("1"));
	else
		AddPersistence((std::string)"adjust_height_to_terrain", std::string("0"));
	
	return 0;
}


int Element::CreateProperties() 
{
	Entity::CreateProperties();

	std::ostringstream str_utm, str_orient, str_scale;

	AddPersistenceLevel("Element information");

	str_utm << utm[0] << ", " << utm[1]<< ", " << utm[2];
	AddPersistence((std::string)"UTM", str_utm.str());

	str_orient << orientation[0] << ", " << orientation[1]<< ", " << orientation[2];
	AddPersistence((std::string)"Orientation", str_orient.str());

	str_scale << scale[0] << ", " << scale[1]<< ", " << scale[2];
	AddPersistence((std::string)"Scale", str_scale.str());

	AddPersistence((std::string)"Text", text);

	if (allow_under_terrain)
		AddPersistence((std::string)"allow_under_terrain", std::string("1"));
	else
		AddPersistence((std::string)"allow_under_terrain", std::string("0"));

	if (adjust_height_to_terrain)
		AddPersistence((std::string)"adjust_height_to_terrain", std::string("1"));
	else
		AddPersistence((std::string)"adjust_height_to_terrain", std::string("0"));


	RemovePersistenceLevel();

	return 0;
}




int Element::AdaptPersistence(Node *root)
{

	Entity::AdaptPersistence(root);

	std::istringstream str1 (root->GetChildValue("utm"));
	str1 >> utm[0] >> utm[1] >> utm[2];

	std::istringstream str2 (root->GetChildValue("scale"));
	str2 >> scale[0] >> scale[1] >> scale[2];

	std::istringstream str3 (root->GetChildValue("orientation"));
	str3 >> orientation[0] >> orientation[1] >> orientation[2];

	text =  (root->GetChildValue("text"));

	std::istringstream str4 (root->GetChildValue("allow_under_terrain"));
	str4 >> allow_under_terrain;

	std::istringstream str5 (root->GetChildValue("adjust_height_to_terrain"));
	str5 >> adjust_height_to_terrain;

	return 0;

}


void Element::SetAllowUnderTerrain(const bool &_allow_under_terrain)
{ 
	allow_under_terrain = _allow_under_terrain; 
	Modified("allow_under_terrain", _allow_under_terrain);
}

void Element::SetAdjustToTerrainHeight(const bool &_adjust_height_to_terrain)
{ 
	adjust_height_to_terrain = _adjust_height_to_terrain; 
	Modified("adjust_height_to_terrain", _adjust_height_to_terrain);
}
