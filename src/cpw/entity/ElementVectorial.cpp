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

#include <cpw/entity/ElementVectorial.h>

using namespace cpw;

ElementVectorial::ElementVectorial(const cpw::TypeId id, const std::string &class_name) : 
Element(id, class_name), color("#FFFFFF"), width(1), points(), terrain_adapt(true),
red(255), green(255), blue(255), type(), parenticon(), pointicon(), update_point(-1), isline(false)
{
}



ElementVectorial::ElementVectorial(const ElementVectorial &element): 
Element(element), color(element.color), width(element.width), points(element.points), terrain_adapt(element.terrain_adapt),
red(element.red), green(element.green), blue(element.blue),type(element.type), parenticon(element.parenticon), pointicon(element.pointicon), isline(element.isline)
{
}



ElementVectorial::~ElementVectorial()
{
}

std::string &ElementVectorial::GetColor()
{
	return color;
}
int ElementVectorial::GetWidth()
{
	return width;
}
std::vector<cpw::Point3d<float>> &ElementVectorial::GetPoints()
{
	return points;
}

const std::string &ElementVectorial::GetColor() const
{
	return color;
}
const int ElementVectorial::GetWidth() const
{
	return width;
}
const std::vector<cpw::Point3d<float>> &ElementVectorial::GetPoints() const
{
	return points;
}

void ElementVectorial::SetColor(const std::string &col)
{
	color = col;
	Modified("color", color);
}

void ElementVectorial::SetWidth(const int w)
{
	width = w;
	Modified("width", width);
}

void ElementVectorial::SetPoints(const std::vector<cpw::Point3d<float>> &p)
{
	points = p;
}

void ElementVectorial::SetValue(const std::string &f, const std::string &v)
{
}

ElementVectorial &ElementVectorial::operator = (const ElementVectorial &element)
{
	cpw::Element::operator =(element);
	color = element.color;
	width = element.width;
	points = element.points;
	terrain_adapt = element.terrain_adapt;
	type = element.type;
	parenticon = element.parenticon;
	pointicon = element.pointicon;
	isline = element.isline;
	return *this;
}


void ElementVectorial::SetRGBColour(const int &r, const int &g, const int &b)
{
	red   = r;
	green = g;
	blue  = b;
}

void ElementVectorial::GetRGBColour(int &r, int &g, int &b)
{
	r = red;
	g = green;
	b = blue;
}


int ElementVectorial::CreatePersistence() {

	Element::CreatePersistence();

	std::stringstream auxt;
	auxt << terrain_adapt;
	AddPersistence((std::string)"terrain_adapt", auxt.str());

	AddPersistence((std::string)"color", color);

	std::stringstream r,g,b;
	r << red;
	g << green;
	b << blue;
	std::map<std::string,std::string> map_col;
	map_col["red"]=r.str();
	map_col["green"]=g.str();
	map_col["blue"]=b.str();
	AddPersistence("RGBColour",map_col);

	std::stringstream aux;
	aux << width;
	AddPersistence((std::string)"width", aux.str());

	AddPersistenceLevel("Points");
	std::vector<cpw::Point3d<float>>::iterator iter_point;
	for(iter_point=points.begin();iter_point!=points.end();iter_point++)
	{
		std::stringstream x,y,z;
		x.precision(15);
		y.precision(15);
		z.precision(15);
		x << (*iter_point).x;
		y << (*iter_point).y;
		z << (*iter_point).z;
		std::map<std::string,std::string> map_point;
		map_point["x"]=x.str();
		map_point["y"]=y.str();
		map_point["z"]=z.str();
		AddPersistence("Point",map_point);
	}
	RemovePersistenceLevel();

	AddPersistence((std::string)"type", type);
	AddPersistence((std::string)"parenticon",parenticon);
	AddPersistence((std::string)"pointicon", pointicon);	
	
	return 0;
}


int ElementVectorial::CreateProperties() 
{
	Element::CreateProperties();

	std::stringstream r;
	r << red << ", " << green << ", " << blue;

	std::stringstream aux;
	aux << width;

	AddPersistence((std::string)"Color", color);
	AddPersistence("RGBColour", r.str());
	AddPersistence((std::string)"Width", aux.str());
	AddPersistence((std::string)"Type", type);
	AddPersistence((std::string)"Parent icon",parenticon);
	AddPersistence((std::string)"Point icon", pointicon);	

	return 0;
}



int ElementVectorial::AdaptPersistence(Node *root)
{

	Element::AdaptPersistence(root);

	std::stringstream auxt (root->GetChildValue("terrain_adapt"));
	auxt >> terrain_adapt;

	color = (root->GetChildValue("color"));

	std::map<std::string, std::string> map_col = *(root->GetChildAttributes("RGBColour"));
	std::istringstream r(map_col["red"]);
	std::istringstream g(map_col["green"]);
	std::istringstream b(map_col["blue"]);
	r >> red;
	g >> green;
	b >> blue;

	std::stringstream aux (root->GetChildValue("width"));
	aux >> width;

	// POINTS VECTOR
	std::vector<cpw::Node*> p_children = root->GetChildren();
	std::vector<cpw::Node*>::iterator pc_iter = p_children.begin();
	while ((pc_iter != p_children.end()) && ((*pc_iter)->GetName())!= "Points" ){pc_iter++;}

	std::vector<cpw::Node*> pc_vector = (*pc_iter)->GetChildren();
	std::vector<cpw::Node*>::iterator pcv_iter = pc_vector.begin();

	for(; pcv_iter != pc_vector.end() ; pcv_iter++)
	{
		std::map<std::string, std::string> point_map = *(*pcv_iter)->GetAttributes();
		cpw::Point3d<float> p;
		std::istringstream px(point_map["x"]);
		std::istringstream py(point_map["y"]);
		std::istringstream pz(point_map["z"]);
		px.precision(15);
		py.precision(15);
		pz.precision(15);
		px >> p.x;
		py >> p.y;
		pz >> p.z;		
		points.push_back(p);
	}

	type = (root->GetChildValue("type"));
	parenticon = (root->GetChildValue("parenticon"));
	pointicon = (root->GetChildValue("pointicon"));

	return 0;
}
