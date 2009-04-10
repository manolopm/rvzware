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

#include <cpw/entity/ElementParticleSystem.h>


using namespace cpw;

ElementParticleSystem::ElementParticleSystem(const cpw::TypeId id, const std::string &class_name) : 
	Element(id, class_name), inserted_in_scene(false), updated_in_scene(false), max_perimeter_radius(0.0f)
{
}



ElementParticleSystem::ElementParticleSystem(const ElementParticleSystem &element): Element(element),
	alpha(element.alpha),
	speed(element.speed),
	phi(element.phi),
	theta(element.theta),		   
	size(element.size),
	coordinates(element.coordinates),
	particles_sec(element.particles_sec),
	senoidal_interpolator(element.senoidal_interpolator),
	particles_life(element.particles_life),		
	additive_blend(element.additive_blend),
	multi_point(element.multi_point),
	texture_filename(ComposeDirectory(element.texture_filename)),
	vertices(element.vertices),
	inserted_in_scene(element.inserted_in_scene),
	updated_in_scene(element.updated_in_scene),
	polygonal(element.polygonal),
	max_perimeter_radius(element.max_perimeter_radius)
{
}



ElementParticleSystem::~ElementParticleSystem()
{
}


ElementParticleSystem &ElementParticleSystem::operator = (const ElementParticleSystem &element)
{
	Element::operator =(element);
	alpha = element.alpha;
	speed = element.speed;
	phi = element.phi;
	theta = element.theta;		   
	size = element.size;
	coordinates = element.coordinates;
	particles_sec = element.particles_sec;
	senoidal_interpolator = element.senoidal_interpolator;
	particles_life = element.particles_life;		
	additive_blend = element.additive_blend;
	multi_point = element.multi_point;
	texture_filename = element.GetTextureFileName();
	vertices = element.vertices;
	inserted_in_scene = element.inserted_in_scene;
	updated_in_scene = element.updated_in_scene;
	polygonal =  element.polygonal;
	max_perimeter_radius = element.max_perimeter_radius;
	return *this;
}



int ElementParticleSystem::CreatePersistence() 
{

	Element::CreatePersistence();

	return 0;
}

int ElementParticleSystem::AdaptPersistence(Node *root)
{

	Element::AdaptPersistence(root);

	return 0;

}

void ElementParticleSystem::SetAlpha(const cpw::Range<float> &_alpha) 
{ 

	alpha = _alpha; 
	Modified("alpha", alpha.min, alpha.max);
}


void ElementParticleSystem::SetSpeed(const cpw::Range<float> &_speed) 
{ 
	speed = _speed; 

	Modified("speed", speed.min, speed.max);
}

void ElementParticleSystem::SetPhi(const cpw::Range<float> &_phi) 
{ 
	phi = _phi; 

	Modified("phi", phi.min, phi.max);
}

void ElementParticleSystem::SetTheta(const cpw::Range<float> &_theta) 
{ 
	theta = _theta; 

	Modified("theta", theta.min, theta.max);			
}

void ElementParticleSystem::SetSize(const cpw::Range<float> &_size) 
{ 	
	size = _size; 
	
	Modified("size", size.min, size.max);
}

void ElementParticleSystem::SetNewParticlesSec(const cpw::Range<int> _particles_sec) { 
	particles_sec = _particles_sec; 
	Modified("particles_sec", particles_sec.min, particles_sec.max);
}

void ElementParticleSystem::SetAdditiveBlend(const bool &_additive_blend) {
	additive_blend = _additive_blend;

	Modified("additive_blend", additive_blend);
}

void ElementParticleSystem::SetSenoidalInterpolator(const bool &_senoidal_interpolator) 
{ 
	senoidal_interpolator = _senoidal_interpolator; 
	Modified("senoidal_interpolator", senoidal_interpolator);
}

void ElementParticleSystem::SetParticlesLife(const float &_particles_life) 
{ 
	particles_life = _particles_life; 

	Modified("particles_life", particles_life);
}


void ElementParticleSystem::SetMultiPoint(bool mp) { 
	multi_point = mp; 
	
	Modified("multi_point", multi_point);
}

void ElementParticleSystem::SetTextureFileName(const std::string &_texture_filename) { 
	texture_filename = _texture_filename; 
	Modified("texture_filename", texture_filename);
}

void ElementParticleSystem::SetPolygonal(const bool &_polygonal) { 
	polygonal = _polygonal; 

	Modified("polygonal", polygonal);

}

void ElementParticleSystem::SetInsertedInScene(const bool &iis) { 
	inserted_in_scene = iis; 

	Modified("inserted_in_scene", inserted_in_scene);
}

void ElementParticleSystem::SetUpdatedInScene(const bool &uis) { 
	updated_in_scene = uis; 

	Modified("updated_in_scene", updated_in_scene);

}


void ElementParticleSystem::SetCoords(cpw::Point3d<float> _coordinates)
{
	coordinates.x = _coordinates.x;
	coordinates.y = _coordinates.y;
	coordinates.z = _coordinates.z;

	Modified("coordinates", coordinates.x, coordinates.y, coordinates.z);
}


void ElementParticleSystem::AddVertex(const cpw::Point3d<float> &new_vertex) { 
	vertices.push_back(new_vertex); 

	Modified("AddVertex", new_vertex.x, new_vertex.y, new_vertex.z);
}



void ElementParticleSystem::SetValue(const std::string &f, const std::string &v)
{
			
	if(f == "alpha") 
	{
		cpw::Range<float> r;
		std::istringstream str(v);
		str >> r.min >> r.max;
		SetAlpha(r); 
	}
	else if (f == "speed")
	{
		cpw::Range<float> r;
		std::istringstream str(v);
		str >> r.min >> r.max;
		SetSpeed(r); 
	}
	else if(f == "phi")
	{
		cpw::Range<float> r;
		std::istringstream str(v);
		str >> r.min >> r.max;
		SetPhi(r); 
	}
	else if(f == "theta")
	{
		cpw::Range<float> r;
		std::istringstream str(v);
		str >> r.min >> r.max;
		SetTheta(r); 
	}
	else if(f == "size")
	{
		cpw::Range<float> r;
		std::istringstream str(v);
		str >> r.min >> r.max;
		SetSize(r); 
	}
	else if(f == "coordinates")
	{
		cpw::Point3d<float> r;
		std::istringstream str(v);
		str >> r.x >> r.y >> r.z;
		SetCoords(r); 
	}
	else if(f == "particles_sec")
	{
		cpw::Range<int> r;
		std::istringstream str(v);
		str >> r.min >> r.max;
		SetNewParticlesSec(r); 
	}
	else if(f == "senoidal_interpolator")
	{
		bool r;
		std::istringstream str(v);
		str >> r;
		SetSenoidalInterpolator(r);
	}
	else if(f == "additive_blend")
	{
		bool r;
		std::istringstream str(v);
		str >> r;
		SetAdditiveBlend(r);
	}
	else if(f == "multi_point")
	{
		bool r;
		std::istringstream str(v);
		str >> r;
		SetMultiPoint(r);
	}
	else if(f == "particles_life")
	{
		float r;
		std::istringstream str(v);
		str >> r;
		SetParticlesLife(r);
	}
	else if(f == "polygonal")
	{
		bool r;
		std::istringstream str(v);
		str >> r;
		SetPolygonal(r);
	}
	else if(f == "inserted_in_scene")
	{
		bool r;
		std::istringstream str(v);
		str >> r;
		SetInsertedInScene(r);
	}
	else if(f == "updated_in_scene")
	{
		bool r;
		std::istringstream str(v);
		str >> r;
		SetUpdatedInScene(r);
	}
	else if(f == "texture_filename")
	{
		SetTextureFileName(v);
	}
	else if(f == "AddVertex")
	{
		cpw::Point3d<float> r;
		std::istringstream str(v);
		str >> r.x >> r.y >> r.z;
		AddVertex(r);
	}
	else 
		Element::SetValue(f, v);


}





bool ElementParticleSystem::GetVertex(int index, cpw::Point3d<float> &vertex)
{
	if (vertices.size() == 0)
		return false;

	if (index < 0)
		return false;
	
	if (index >= ((int)vertices.size()))
		return false;

	vertex.x = vertices[index].x;
	vertex.y = vertices[index].y;
	vertex.z = vertices[index].z;

	return true;

}