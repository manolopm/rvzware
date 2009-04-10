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

#include <cpwutils/Fire.h>
#include <cpw/common/Math.h>

#include <math.h>
#include <sstream>
#include <algorithm>


#define REDUCE_XML  

using cpw::utils::Fire;

Fire::Fire(const cpw::TypeId id, 
		   const std::string &class_name) : cpw::Layer(id, class_name), current_flames(),
		   fire_texture(), smoke_texture(), polygonal(), current_time(), url_ign_point(), fvisible(false),
		   reset_animation(false), animation_precomputed(false), graphic_factory(NULL)
{
	
}

Fire::Fire(const Fire &fire) : Layer(fire)
{
	current_flames = fire.current_flames;
	ignition_points = fire.ignition_points;
	flames_progression = fire.flames_progression;
	fire_texture = fire.GetFireTexture();
	smoke_texture = fire.GetSmokeTexture();
	polygonal = fire.polygonal;
	current_time = fire.current_time;
	url_ign_point = fire.GetIgnitionPointUrl();
	animation_precomputed = false;
	reset_animation = fire.reset_animation;
	fvisible = fire.fvisible;
	graphic_factory = fire.graphic_factory;
	Modified();
}

Fire::~Fire(void)
{
	
	
}

Fire &Fire::operator = (const Fire &fire)
{
	Layer::operator = (fire);
	current_flames = fire.current_flames;
	ignition_points = fire.ignition_points;
	flames_progression = fire.flames_progression;
	fire_texture = fire.GetFireTexture();
	smoke_texture = fire.GetSmokeTexture();
	polygonal = fire.polygonal;
	current_time = fire.current_time;
	url_ign_point = fire.GetIgnitionPointUrl();
	graphic_factory = fire.graphic_factory;
	animation_precomputed = false;
	Modified();
	return *this;	
}

void Fire::SetPolygonal(const bool &_polygonal)
{ 
	polygonal = _polygonal;
	Modified("polygonal", polygonal);

}

cpw::ElementParticleSystem *Fire::GetFlameParticleSystem(int flame_index)
{
	std::vector<cpw::utils::Flame>::iterator flame_pos;
	int i = 0;

	for (flame_pos = current_flames.begin(); flame_pos != current_flames.end(); flame_pos++)
	{
		if (flame_index == i)
			return &(*flame_pos).fire;
		else
			i++;
	}

	return NULL;
}

void Fire::DeleteFlame(const int flame_index)
{
	std::vector<cpw::utils::Flame>::iterator flame_pos = current_flames.begin();
	int i = flame_index;

	while(i && flame_pos != current_flames.end()) 
	{
		i--;
		flame_pos++;
	}
	
	if(flame_pos != current_flames.end())
	{			
		
		GraphicDelete();
		current_flames.erase(flame_pos);
		GraphicInsert();
		Modified("DeleteFlame");
		
	}
}

cpw::utils::Flame *Fire::GetFlame(const int &ind)
{
	if (current_flames.size() == 0)
		return NULL;

	if (ind < 0)
		return NULL;
	
	if (ind >= ((int)current_flames.size()))
		return NULL;

	return &current_flames[ind];
}


int Fire::AddFlame(const cpw::utils::Flame &new_flame, const int &time_slice_index)
{
	cpw::utils::TimeSlice *flames_progression = GetTimeSlice(time_slice_index);
	
	if (flames_progression != NULL)
	{
		flames_progression->flames.push_back(new_flame);
		Modified("AddFlame");
	}
	
	return 0;
}


cpw::utils::Flame & Fire::GetLastFlame()
{
	return current_flames.back();
}




void Fire::AddIgnitionPoint(const cpw::Point3d<float> &new_ignition_point)
{
	ignition_points.push_back(new_ignition_point);
	Modified("AddIgnitionPoint");

}

cpw::Point3d<float> * Fire::GetIgnitionPoint(const int &index)
{
	if (ignition_points.size() == 0)
		return NULL;

	if (index < 0)
		return NULL;
	
	if (index >= ((int)ignition_points.size()))
		return NULL;

	return &ignition_points[index];
}

cpw::Point3d<float> * Fire::GetLastIgnitionPoint()
{
	if (ignition_points.size() > 0)
	{
		return &(ignition_points.back());
	}
	else
		return NULL;
}

void Fire::DeleteIgnitionPoint(const int &index)
{
	std::vector<cpw::Point3d<float>>::iterator ignition_pos = ignition_points.begin();
	int i = index;

	while(i && ignition_pos != ignition_points.end()) 
	{
		i--;
		ignition_pos++;
	}
	
	if(ignition_pos != ignition_points.end())
	{			
		GraphicDelete();
		ignition_points.erase(ignition_pos);
		GraphicInsert();
		Modified("DeleteIgnitionPoint");
	}
}


////////   TIMESLICE  //////
cpw::utils::TimeSlice * Fire::GetTimeSlice(const int &index)
{
	if (flames_progression.size() == 0)
		return NULL;

	if (index < 0)
		return NULL;
	
	if (index >= ((int)flames_progression.size()))
		return NULL;

	return &flames_progression[index];
}

void Fire::DeleteTimeSlice(const int &index)
{
	std::vector<TimeSlice>::iterator time_slice_pos = flames_progression.begin();
	int i = index;

	while(i && time_slice_pos != flames_progression.end()) 
	{
		i--;
		time_slice_pos++;
	}
	
	if(time_slice_pos != flames_progression.end())
	{			
		GraphicDelete();
		flames_progression.erase(time_slice_pos);
		GraphicInsert();
		Modified("DeleteTimeSlice");
	}
}

void Fire::AddTimeSlice(const cpw::utils::TimeSlice &new_time_slice)
{
	flames_progression.push_back(new_time_slice);
	Modified("AddTimeSlice");

}

cpw::utils::TimeSlice * Fire::GetLastTimeSlice()
{
	if (flames_progression.size() <= 0)
		return NULL;
	else
		return &flames_progression.back();
}

void Fire::InitCurrentFlames()
{
	current_flames = flames_progression.front().flames;
	current_time = flames_progression.front().time_stamp;

}

bool Fire::UpdateCurrentFlames(const cpw::cpwTime &time)
{
	if((time.seconds >= GetStartTime().seconds) && (time.seconds <= GetEndTime().seconds))
	{
		std::vector<TimeSlice>::iterator time_slice_pos = flames_progression.begin();
		std::vector<TimeSlice>::iterator aux;
		bool entra = false;
		for( ; time_slice_pos != flames_progression.end(); time_slice_pos++)
		{
			if(time.seconds >= time_slice_pos->time_stamp.seconds )
			{
				aux = time_slice_pos;
				entra = true;
			}
		}
		if (entra)// && (current_time.seconds != aux->time_stamp.seconds))
		{
			current_flames = aux->flames;
			current_time = aux->time_stamp;
			return true;
		}
	}
	return false;
}

bool Fire::NeedsToUpdate(const cpw::cpwTime &time)
{
	if((time.seconds >= GetStartTime().seconds) && (time.seconds <= GetEndTime().seconds))
	{
		std::vector<TimeSlice>::iterator time_slice_pos = flames_progression.begin();
		std::vector<TimeSlice>::iterator aux;
		bool entra = false;
		
		for( ; time_slice_pos != flames_progression.end(); time_slice_pos++)
		{
			if(time.seconds >= time_slice_pos->time_stamp.seconds )
				
			{
				aux = time_slice_pos;
				entra = true;
		
			}
		}

		if (!fvisible)
		{
			fvisible = true;
			return true;
		}
		

		if (entra && (current_time.seconds != aux->time_stamp.seconds))
		{			
			fvisible = true;
			return true;
		}
	}
	else
	{
		fvisible = false;
	}
	return false;
}


void Fire::SetValue(const std::string &f, const std::string &v)
{
	if(f == "url_ign_point")
		SetIgnitionPointUrl(v);
	else if(f == "fire_texture")
		SetFireTexture(v);
	else if(f == "smoke_texture")
		SetSmokeTexture(v);
	else if(f == "polygonal")
	{
		bool p;
		std::istringstream str(v);
		str >> p;
		SetPolygonal(p);
	}
	/*else if(f == "polygonal_new_particles_sec")
	{
		int min, max;
		std::istringstream str(v);
		str >> min >> max;
		SetPolygonalNewParticlesSec(min, max);
	}*/
	else if(f == "current_time")
	{
		cpw::cpwTime t;
		std::istringstream str(v);
		str >> t.seconds >> t.miliseconds;
		SetCurrentTime(t);
	}
	else
		Layer::SetValue(f, v);
}


int Fire::CreatePersistence()
{
	Layer::CreatePersistence();

	AddPersistence(std::string("FireTexture"), RelativeDirectory(fire_texture));
	AddPersistence(std::string("SmokeTexture"), RelativeDirectory(smoke_texture));
	AddPersistence(std::string("IgnitionPointModel"), RelativeDirectory(url_ign_point));
	std::ostringstream flame_polygonal;
	flame_polygonal << polygonal;
	AddPersistence(std::string("Polygonal"), flame_polygonal.str());

	std::map<std::string, std::string> starttime_map;
	std::ostringstream st_s, st_m;
	st_s << GetStartTime().seconds;
	st_m << GetStartTime().miliseconds;
	starttime_map["seconds"] = st_s.str();
	starttime_map["miliseconds"] = st_m.str();
	AddPersistence(std::string("StartTime"), starttime_map);

	std::map<std::string, std::string> endtime_map;
	std::ostringstream et_s, et_m;
	et_s << GetEndTime().seconds;
	et_m << GetEndTime().miliseconds;
	endtime_map["seconds"] = et_s.str();
	endtime_map["miliseconds"] = et_m.str();
	AddPersistence(std::string("EndTime"), endtime_map);


#ifdef REDUCE_XML
	//// binary conversion of Fire perimeters and ignition points to reduce storage demands
	//// this conversion will not permit multiplatform compatibility
	//std::ostringstream binary_out; 
	//
	///*int size = sizeof(Point3d<float>) * ignition_points.size();
	//char *data = new char[size];
	//memcopy(data, ignition_points.begin(), size);
	//*/

	std::vector<cpw::Point3d<float>>::iterator ip_iter = ignition_points.begin();
	std::ostringstream ip_map(std::ostringstream::binary);

	ip_map << ignition_points.size() << std::endl;

	for( ; ip_iter != ignition_points.end() ; ip_iter++)
		ip_map << ip_iter->x << " " << ip_iter->y << " " << ip_iter->z << std::endl;

	AddPersistence("IgnitionPoints", ip_map.str());
	
	// TIME SLICE VECTOR
	std::ostringstream time_slices(std::ostringstream::binary);

	time_slices << flames_progression.size() << std::endl; 
	std::vector<TimeSlice>::iterator i;  
	for (i = flames_progression.begin(); i != flames_progression.end(); i++)
	{
		time_slices << i->time_stamp.seconds << " " << i->time_stamp.miliseconds << std::endl;

		std::vector<cpw::utils::Flame>::iterator j;
		
		time_slices << i->flames.size() << std::endl;
		for(j = i->flames.begin() ; j != i->flames.end() ; j++ )
		{
			time_slices.precision(15);
			time_slices << (*j).fire.GetCoords().x << " " << (*j).fire.GetCoords().y << " " << (*j).fire.GetCoords().z << " ";
			time_slices << (*j).fire.GetAlpha().min << " " << (*j).fire.GetAlpha().max << " ";
			time_slices << (*j).fire.GetSpeed().min << " " << (*j).fire.GetSpeed().max << " ";
			time_slices << (*j).fire.GetPhi().min << " " << (*j).fire.GetPhi().max << " ";
			time_slices << (*j).fire.GetTheta().min << " " << (*j).fire.GetTheta().max << " ";
			time_slices << (*j).fire.GetSize().min << " " << (*j).fire.GetSize().max << " ";
			time_slices << (*j).fire.GetNewParticlesSec().min << " " << (*j).fire.GetNewParticlesSec().max << " ";
			time_slices << (*j).fire.GetSenoidalInterpolator() << " ";
			time_slices << (*j).fire.GetParticlesLife() << " ";
			time_slices << (*j).fire.GetAdditiveBlend() << " ";
			time_slices << (*j).smoke.GetAlpha().min << " " << (*j).smoke.GetAlpha().max << " ";
			time_slices << (*j).smoke.GetSpeed().min << " " << (*j).smoke.GetSpeed().max << " ";
			time_slices << (*j).smoke.GetPhi().min << " " << (*j).smoke.GetPhi().max << " ";
			time_slices << (*j).smoke.GetTheta().min << " " << (*j).smoke.GetTheta().max << " ";
			time_slices << (*j).smoke.GetSize().min << " " << (*j).smoke.GetSize().max << " ";
			time_slices << (*j).smoke.GetNewParticlesSec().min << " " << (*j).smoke.GetNewParticlesSec().max << " ";
			time_slices << (*j).smoke.GetSenoidalInterpolator() << " ";			 
			time_slices << (*j).smoke.GetParticlesLife() << " ";
			time_slices << (*j).smoke.GetAdditiveBlend() << std::endl;
		}
	}

	AddPersistence("TimeSlices", time_slices.str());


#else
	// IGNITION POINTS VECTOR
	AddPersistenceLevel("IgnitionPoints");
	std::vector<cpw::Point3d<float>>::iterator ip_iter = ignition_points.begin();
	for( ; ip_iter != ignition_points.end() ; ip_iter++)
	{
		std::map<std::string, std::string> ip_map;
		std::ostringstream ipx, ipy, ipz;
		ipx << ip_iter->x;
		ipy << ip_iter->y;
		ipz << ip_iter->z;
		ip_map["x"] = ipx.str();
		ip_map["y"] = ipy.str();
		ip_map["z"] = ipz.str();
		AddPersistence("IgnitionPoint", ip_map);
	}
	RemovePersistenceLevel();
	

	// TIME SLICE VECTOR
	AddPersistenceLevel("TimeSlices");
	std::vector<TimeSlice>::iterator i;  
	for (i = flames_progression.begin(); i != flames_progression.end(); i++)
	{
		AddPersistenceLevel("TimeSlice");
		std::map<std::string, std::string> ts_map;
		std::ostringstream ts_s, ts_m;
		ts_s << i->time_stamp.seconds;
		ts_m << i->time_stamp.miliseconds;
		ts_map["seconds"] = ts_s.str();
		ts_map["miliseconds"] = ts_m.str();
		AddPersistence(std::string("TimeStamp"), ts_map);

		std::vector<cpw::utils::Flame>::iterator j;
		for(j = i->flames.begin() ; j != i->flames.end() ; j++ )
		{
			AddPersistenceLevel((std::string)"Flame");  

			std::ostringstream flame_position;
			flame_position.precision(15);
			flame_position << (*j).fire.GetCoords().x << " " << (*j).fire.GetCoords().y << " " << (*j).fire.GetCoords().z;

			std::ostringstream flame_alpha_fire;
			flame_alpha_fire << (*j).fire.GetAlpha().min << " " << (*j).fire.GetAlpha().max;

			std::ostringstream flame_speed_fire;
			flame_speed_fire << (*j).fire.GetSpeed().min << " " << (*j).fire.GetSpeed().max;
	  
			std::ostringstream flame_phi_fire;
			flame_phi_fire << (*j).fire.GetPhi().min << " " << (*j).fire.GetPhi().max;

			std::ostringstream flame_theta_fire;
			flame_theta_fire << (*j).fire.GetTheta().min << " " << (*j).fire.GetTheta().max;
							
			std::ostringstream flame_size_fire;
			flame_size_fire << (*j).fire.GetSize().min << " " << (*j).fire.GetSize().max;

			std::ostringstream flame_particles_sec_fire;
			flame_particles_sec_fire << (*j).fire.GetNewParticlesSec().min << " " << (*j).fire.GetNewParticlesSec().max;
						  
			std::ostringstream flame_senoidal_interpolator_fire;
			flame_senoidal_interpolator_fire << (*j).fire.GetSenoidalInterpolator();
						 
			std::ostringstream flame_particles_life_fire;
			flame_particles_life_fire << (*j).fire.GetParticlesLife();
						  
			std::ostringstream flame_additive_blend_fire;
			flame_additive_blend_fire << (*j).fire.GetAdditiveBlend();
						 		
			//smoke
					
			std::ostringstream flame_alpha_smoke;
			flame_alpha_smoke << (*j).smoke.GetAlpha().min << " " << (*j).smoke.GetAlpha().max;

			std::ostringstream flame_speed_smoke;
			flame_speed_smoke << (*j).smoke.GetSpeed().min << " " << (*j).smoke.GetSpeed().max;
	  
			std::ostringstream flame_phi_smoke;
			flame_phi_smoke << (*j).smoke.GetPhi().min << " " << (*j).smoke.GetPhi().max;

			std::ostringstream flame_theta_smoke;
			flame_theta_smoke << (*j).smoke.GetTheta().min << " " << (*j).smoke.GetTheta().max;
							
			std::ostringstream flame_size_smoke;
			flame_size_smoke << (*j).smoke.GetSize().min << " " << (*j).smoke.GetSize().max;

			std::ostringstream flame_particles_sec_smoke;
			flame_particles_sec_smoke << (*j).smoke.GetNewParticlesSec().min << " " << (*j).smoke.GetNewParticlesSec().max;
						  
			std::ostringstream flame_senoidal_interpolator_smoke;
			flame_senoidal_interpolator_smoke << (*j).smoke.GetSenoidalInterpolator();
						 
			std::ostringstream flame_particles_life_smoke;
			flame_particles_life_smoke << (*j).smoke.GetParticlesLife();
						  
			std::ostringstream flame_additive_blend_smoke;
			flame_additive_blend_smoke << (*j).smoke.GetAdditiveBlend();

			AddPersistence(std::string("Position"), flame_position.str());

			AddPersistence(std::string("Flame_alpha_fire"), flame_alpha_fire.str());
			AddPersistence(std::string("Flame_speed_fire"), flame_speed_fire.str());
			AddPersistence(std::string("Flame_phi_fire"), flame_phi_fire.str());
			AddPersistence(std::string("Flame_theta_fire"), flame_theta_fire.str());
			AddPersistence(std::string("Flame_size_fire"), flame_size_fire.str());
			AddPersistence(std::string("Flame_particles_sec_fire"), flame_particles_sec_fire.str());
			AddPersistence(std::string("Flame_senoidal_interpolator_fire"), flame_senoidal_interpolator_fire.str());
			AddPersistence(std::string("Flame_particles_life_fire"), flame_particles_life_fire.str());
			AddPersistence(std::string("Flame_additive_blend_fire"), flame_additive_blend_fire.str());

			AddPersistence(std::string("Flame_alpha_smoke"), flame_alpha_smoke.str());
			AddPersistence(std::string("Flame_speed_smoke"), flame_speed_smoke.str());
			AddPersistence(std::string("Flame_phi_smoke"), flame_phi_smoke.str());
			AddPersistence(std::string("Flame_theta_smoke"), flame_theta_smoke.str());
			AddPersistence(std::string("Flame_size_smoke"), flame_size_smoke.str());
			AddPersistence(std::string("Flame_particles_sec_smoke"), flame_particles_sec_smoke.str());
			AddPersistence(std::string("Flame_senoidal_interpolator_smoke"), flame_senoidal_interpolator_smoke.str());
			AddPersistence(std::string("Flame_particles_life_smoke"), flame_particles_life_smoke.str());
			AddPersistence(std::string("Flame_additive_blend_smoke"), flame_additive_blend_smoke.str());

			
			RemovePersistenceLevel();
		}
		RemovePersistenceLevel();
	}
	RemovePersistenceLevel();

#endif
	return 0;
}

int Fire::CreateProperties() 
{
	Layer::CreateProperties();

	AddPersistenceLevel("Fire Information");

	AddPersistence(std::string("Fire texture"), RelativeDirectory(fire_texture));
	AddPersistence(std::string("Smoke texture"), RelativeDirectory(smoke_texture));
	AddPersistence(std::string("Ignition point model"), RelativeDirectory(url_ign_point));

	std::map<std::string, std::string> starttime_map;
	std::ostringstream st_s, st_m;
	st_s << GetStartTime().seconds;
	st_m << GetStartTime().miliseconds;
	starttime_map["seconds"] = st_s.str();
	starttime_map["miliseconds"] = st_m.str();

	AddPersistence(std::string("Start Time"), starttime_map);

	std::map<std::string, std::string> endtime_map;
	std::ostringstream et_s, et_m;
	et_s << GetEndTime().seconds;
	et_m << GetEndTime().miliseconds;
	endtime_map["seconds"] = et_s.str();
	endtime_map["miliseconds"] = et_m.str();

	AddPersistence(std::string("Final time"), endtime_map);

	// Ignition points vector
	AddPersistenceLevel("Ignition points");
	std::vector<cpw::Point3d<float>>::iterator ip_iter = ignition_points.begin();
	for( ; ip_iter != ignition_points.end() ; ip_iter++)
	{
		std::map<std::string, std::string> ip_map;
		std::ostringstream ipx, ipy, ipz;
		ipx << ip_iter->x;
		ipy << ip_iter->y;
		ipz << ip_iter->z;
		ip_map["x"] = ipx.str();
		ip_map["y"] = ipy.str();
		ip_map["z"] = ipz.str();
		AddPersistence("Ignition Point", ip_map);
	}
	RemovePersistenceLevel();
	
	RemovePersistenceLevel();


	return 0;
}



int Fire::AdaptPersistence(cpw::Node *root)
{
	Layer::AdaptPersistence(root);

	fire_texture =  (root->GetChildValue("FireTexture"));
	smoke_texture =  (root->GetChildValue("SmokeTexture"));
	url_ign_point =  (root->GetChildValue("IgnitionPointModel"));

	std::istringstream flame_polygonal(root->GetChildValue("Polygonal"));
	flame_polygonal >> polygonal;

	std::map<std::string, std::string> starttime_map = *(root->GetChildAttributes("StartTime"));
	std::istringstream st_s(starttime_map["seconds"]);
	std::istringstream st_m(starttime_map["miliseconds"]);
	st_s >> GetStartTime().seconds;
	st_m >> GetStartTime().miliseconds;

	std::map<std::string, std::string> endtime_map = *(root->GetChildAttributes("EndTime"));
	std::istringstream et_s(endtime_map["seconds"]);
	std::istringstream et_m(endtime_map["miliseconds"]);
	et_s >> GetEndTime().seconds;
	et_m >> GetEndTime().miliseconds;

#ifdef REDUCE_XML
	// IGNITION POINTS VECTOR
	std::istringstream ig_points(root->GetChildValue("IgnitionPoints"), std::istringstream::binary);
	int size;

	ig_points >> size;
	ignition_points.resize(size);


	for(int i = 0; i < size ; i++)
	{
		cpw::Point3d<float> ip;
		ig_points >> ignition_points[i].x >> ignition_points[i].y >> ignition_points[i].z;		
	}


	// TIME SLICE VECTOR
	std::istringstream ti_slices(root->GetChildValue("TimeSlices"), std::istringstream::binary);

	ti_slices >> size;

	flames_progression.resize(size);
	for(int i = 0; i < size; i++)
	{

		ti_slices >> flames_progression[i].time_stamp.seconds;
		ti_slices >> flames_progression[i].time_stamp.miliseconds;


		int flame_size;
		ti_slices >> flame_size;
		flames_progression[i].flames.resize(flame_size);

		for(int j = 0; j < flame_size; j++)
		{			
			float max, min, auxx, auxy, auxz;
			int maxi, mini;
			bool aux_b;

			ti_slices >> auxx >> auxy >> auxz;
			flames_progression[i].flames[j].fire.SetCoords(cpw::Point3d<float>(auxx, auxy, auxz));
						
			ti_slices >> min >> max;
			flames_progression[i].flames[j].fire.SetAlpha(cpw::Range<float>(min, max));

			ti_slices >> min >> max;
			flames_progression[i].flames[j].fire.SetSpeed(cpw::Range<float>(min, max));

			ti_slices >> min >> max;
			flames_progression[i].flames[j].fire.SetPhi(cpw::Range<float>(min, max));

			ti_slices >> min >> max;
			flames_progression[i].flames[j].fire.SetTheta(cpw::Range<float>(min, max));

			ti_slices >> min >> max;
			flames_progression[i].flames[j].fire.SetSize(cpw::Range<float>(min, max));
			
			ti_slices >> mini >> maxi;
			flames_progression[i].flames[j].fire.SetNewParticlesSec(cpw::Range<int>(mini, maxi));

			ti_slices >> aux_b;
			flames_progression[i].flames[j].fire.SetSenoidalInterpolator(aux_b);

			ti_slices >> min;
			flames_progression[i].flames[j].fire.SetParticlesLife(min);
			
			ti_slices >> aux_b;
			flames_progression[i].flames[j].fire.SetAdditiveBlend(aux_b);

			flames_progression[i].flames[j].smoke.SetCoords(flames_progression[i].flames[j].fire.GetCoords());

			ti_slices >> min >> max;
			flames_progression[i].flames[j].smoke.SetAlpha(cpw::Range<float>(min, max));

			ti_slices >> min >> max;
			flames_progression[i].flames[j].smoke.SetSpeed(cpw::Range<float>(min, max));

			ti_slices >> min >> max;
			flames_progression[i].flames[j].smoke.SetPhi(cpw::Range<float>(min, max));

			ti_slices >> min >> max;
			flames_progression[i].flames[j].smoke.SetTheta(cpw::Range<float>(min, max));

			ti_slices >> min >> max;
			flames_progression[i].flames[j].smoke.SetSize(cpw::Range<float>(min, max));
			
			ti_slices >> mini >> maxi;
			flames_progression[i].flames[j].smoke.SetNewParticlesSec(cpw::Range<int>(mini, maxi));

			ti_slices >> aux_b;
			flames_progression[i].flames[j].smoke.SetSenoidalInterpolator(aux_b);

			ti_slices >> min;
			flames_progression[i].flames[j].smoke.SetParticlesLife(min);
			
			ti_slices >> aux_b;
			flames_progression[i].flames[j].smoke.SetAdditiveBlend(aux_b);

		}
	}

#else

	// IGNITION POINTS VECTOR
	std::vector<cpw::Node*> ip_children = root->GetChildren();
	std::vector<cpw::Node*>::iterator ipc_iter = ip_children.begin();
	while ((ipc_iter != ip_children.end()) && ((*ipc_iter)->GetName())!= "IgnitionPoints" ){ipc_iter++;}

	std::vector<cpw::Node*> ipc_vector = (*ipc_iter)->GetChildren();
	std::vector<cpw::Node*>::iterator ipcv_iter = ipc_vector.begin();

	for(; ipcv_iter != ipc_vector.end() ; ipcv_iter++)
	{
		std::map<std::string, std::string> ip_map = *(*ipcv_iter)->GetAttributes();
		cpw::Point3d<float> ip;
		std::istringstream ipx(ip_map["x"]);
		std::istringstream ipy(ip_map["y"]);
		std::istringstream ipz(ip_map["z"]);
		ipx >> ip.x;
		ipy >> ip.y;
		ipz >> ip.z;		
		ignition_points.push_back(ip);
	}


	// TIME SLICE VECTOR
	std::vector<cpw::Node*> ts_children = root->GetChildren();
	std::vector<cpw::Node*>::iterator tsc_iter = ts_children.begin();
	while ((tsc_iter != ts_children.end()) && ((*tsc_iter)->GetName())!= "TimeSlices" ){tsc_iter++;}


	std::vector<cpw::Node *> &root_children = (*tsc_iter)->GetChildren();
	std::string ts_str("TimeSlice");
	std::string flame_str("Flame");
	
	std::vector<cpw::Node *>::iterator i;
	int ts_index =0;
	flames_progression.resize(root_children.size());
	for(i = root_children.begin(); i != root_children.end(); i++, ts_index++)
	{
		std::map<std::string, std::string> timestamp_map = *((*i)->GetChildAttributes("TimeStamp"));
		std::istringstream ts_s(timestamp_map["seconds"]);
		std::istringstream ts_m(timestamp_map["miliseconds"]);
		ts_s >> flames_progression[ts_index].time_stamp.seconds;
		ts_m >> flames_progression[ts_index].time_stamp.miliseconds;
		

		std::vector<cpw::Node *> flames = (*i)->GetChildren();
		std::vector<cpw::Node *>::iterator iter_flames = flames.begin();
		flames_progression[ts_index].flames.resize(flames.size()-1);
		int pos = 0;
		for(; iter_flames != flames.end(); iter_flames++, pos++)
		{			
			if ((*iter_flames)->GetName() == "Flame" )
			{
				std::istringstream flame_position((*iter_flames)->GetChildValue("Position"));

				std::istringstream flame_alpha_fire((*iter_flames)->GetChildValue("Flame_alpha_fire"));
				std::istringstream flame_speed_fire((*iter_flames)->GetChildValue("Flame_speed_fire"));
				std::istringstream flame_phi_fire((*iter_flames)->GetChildValue("Flame_phi_fire"));
				std::istringstream flame_theta_fire((*iter_flames)->GetChildValue("Flame_theta_fire"));
				std::istringstream flame_size_fire((*iter_flames)->GetChildValue("Flame_size_fire"));
				std::istringstream flame_particles_sec_fire((*iter_flames)->GetChildValue("Flame_particles_sec_fire"));
				std::istringstream flame_senoidal_interpolator_fire((*iter_flames)->GetChildValue("Flame_senoidal_interpolator_fire"));
				std::istringstream flame_particles_life_fire((*iter_flames)->GetChildValue("Flame_particles_life_fire"));
				std::istringstream flame_additive_blend_fire((*iter_flames)->GetChildValue("Flame_additive_blend_fire"));

				std::istringstream flame_alpha_smoke((*iter_flames)->GetChildValue("Flame_alpha_smoke"));
				std::istringstream flame_speed_smoke((*iter_flames)->GetChildValue("Flame_speed_smoke"));
				std::istringstream flame_phi_smoke((*iter_flames)->GetChildValue("Flame_phi_smoke"));
				std::istringstream flame_theta_smoke((*iter_flames)->GetChildValue("Flame_theta_smoke"));
				std::istringstream flame_size_smoke((*iter_flames)->GetChildValue("Flame_size_smoke"));
				std::istringstream flame_particles_sec_smoke((*iter_flames)->GetChildValue("Flame_particles_sec_smoke"));
				std::istringstream flame_senoidal_interpolator_smoke((*iter_flames)->GetChildValue("Flame_senoidal_interpolator_smoke"));
				std::istringstream flame_particles_life_smoke((*iter_flames)->GetChildValue("Flame_particles_life_smoke"));
				std::istringstream flame_additive_blend_smoke((*iter_flames)->GetChildValue("Flame_additive_blend_smoke"));


				float max, min, auxx, auxy, auxz;
				int maxi, mini;
				bool aux_b;

				flame_position >> auxx >> auxy >> auxz;
				flames_progression[ts_index].flames[pos].fire.SetCoords(cpw::Point3d<float>(auxx, auxy, auxz));
							
				flame_alpha_fire >> min >> max;
				flames_progression[ts_index].flames[pos].fire.SetAlpha(cpw::Range<float>(min, max));

				flame_speed_fire >> min >> max;
				flames_progression[ts_index].flames[pos].fire.SetSpeed(cpw::Range<float>(min, max));

				flame_phi_fire >> min >> max;
				flames_progression[ts_index].flames[pos].fire.SetPhi(cpw::Range<float>(min, max));

				flame_theta_fire >> min >> max;
				flames_progression[ts_index].flames[pos].fire.SetTheta(cpw::Range<float>(min, max));

				flame_size_fire >> min >> max;
				flames_progression[ts_index].flames[pos].fire.SetSize(cpw::Range<float>(min, max));
				
				flame_particles_sec_fire >> mini >> maxi;
				flames_progression[ts_index].flames[pos].fire.SetNewParticlesSec(cpw::Range<int>(mini, maxi));

				flame_senoidal_interpolator_fire >> aux_b;
				flames_progression[ts_index].flames[pos].fire.SetSenoidalInterpolator(aux_b);

				flame_particles_life_fire >> min;
				flames_progression[ts_index].flames[pos].fire.SetParticlesLife(min);
				
				flame_additive_blend_fire >> aux_b;
				flames_progression[ts_index].flames[pos].fire.SetAdditiveBlend(aux_b);

				flames_progression[ts_index].flames[pos].smoke.SetCoords(flames_progression[ts_index].flames[pos].fire.GetCoords());

				flame_alpha_smoke >> min >> max;
				flames_progression[ts_index].flames[pos].smoke.SetAlpha(cpw::Range<float>(min, max));

				flame_speed_smoke >> min >> max;
				flames_progression[ts_index].flames[pos].smoke.SetSpeed(cpw::Range<float>(min, max));

				flame_phi_smoke >> min >> max;
				flames_progression[ts_index].flames[pos].smoke.SetPhi(cpw::Range<float>(min, max));

				flame_theta_smoke >> min >> max;
				flames_progression[ts_index].flames[pos].smoke.SetTheta(cpw::Range<float>(min, max));

				flame_size_smoke >> min >> max;
				flames_progression[ts_index].flames[pos].smoke.SetSize(cpw::Range<float>(min, max));
				
				flame_particles_sec_smoke >> mini >> maxi;
				flames_progression[ts_index].flames[pos].smoke.SetNewParticlesSec(cpw::Range<int>(mini, maxi));

				flame_senoidal_interpolator_smoke >> aux_b;
				flames_progression[ts_index].flames[pos].smoke.SetSenoidalInterpolator(aux_b);

				flame_particles_life_smoke >> min;
				flames_progression[ts_index].flames[pos].smoke.SetParticlesLife(min);
				
				flame_additive_blend_smoke >> aux_b;
				flames_progression[ts_index].flames[pos].smoke.SetAdditiveBlend(aux_b);

			}
			else pos--;
		}
	}
#endif

	current_flames=flames_progression[0].flames;
	current_time = GetStartTime();
	

	return 0;

}


int Fire::AddToCurrentFlames(const cpw::utils::Flame &new_flame)
{
	current_flames.push_back(new_flame);
	Modified("AddToCurrentFlames");

	return current_flames.size()-1;
}


void Fire::UpdateFlamesInScene()
{
}

void Fire::MakeTimeSliceCurrent(const unsigned int &index)
{
	if (index > flames_progression.size()-1)
		return;

	current_flames = flames_progression[index].flames;
}

void Fire::DeleteFlame(const int &ts_index, const int &flame_index)
{
	TimeSlice *ts = GetTimeSlice(ts_index);

	if (ts == NULL)
		return;

	if ((flame_index > ((int)ts->flames.size()-1)) || (flame_index < 0))
		return;

	std::vector<cpw::utils::Flame>::iterator i;
	int j=0;
	for (i = ts->flames.begin(); i != ts->flames.end(); i++)
	{
		if (j==flame_index)
		{
			ts->flames.erase(i);
			Modified("DeleteFlame");
			return;
		}
		
		j++;
	}
}


int Fire::GetActiveInterval(const cpw::cpwTime &time, double &position_between_intervals)
{
	//find the active interval and return the index
	//return -1 if there is no active interval

	int fp_size = flames_progression.size();
	position_between_intervals = -1;

	if ((flames_progression.begin()->time_stamp.seconds > time.seconds) || 
		(flames_progression[fp_size-1].time_stamp.seconds < time.seconds) )
	{
		if ( ( GetEndTime().seconds > time.seconds ) && ( GetStartTime().seconds < time.seconds ) )
		{
			return flames_progression.size()-1;
		}
		else
			return -1;
	}

	//there is an active interval
	std::vector<TimeSlice>::iterator iter = flames_progression.begin();

	for ( unsigned int i=0; i < (flames_progression.size()-1); i++)
	{
		if ( (flames_progression[i].time_stamp.seconds <= time.seconds) &&
			 (flames_progression[i+1].time_stamp.seconds >= time.seconds) )
		{
			//compute the position relative to active intervals
			double time_a = flames_progression[i].time_stamp.seconds + (flames_progression[i].time_stamp.miliseconds * 0.001f);
			double time_b = flames_progression[i+1].time_stamp.seconds + (flames_progression[i+1].time_stamp.miliseconds * 0.001f);
			double time_c = time.seconds + (time.miliseconds * 0.001f);
			double inc = time_b - time_a;						
			
			time_c -= time_a;
			position_between_intervals = time_c / inc;

			return (int) i;
		}
	}

	return -1;


}

