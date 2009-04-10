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
#ifndef _FIRE_
#define _FIRE_

#include <vector>

#include <cpw/entity/ElementParticleSystem.h>
#include <cpw/common/types.h>
#include <cpw/entity/Layer.h>
#include <cpw/graphic/IGraphicFactory.h>
#include <cpw/common/IdGenerator.h>

#include "Exports.h"
#include "FireAnimation.h"


/**
	\namespace cpw { namespace utils Capaware cpw::utils \brief Library for advanced functionalities of the Capaware library
    \defgroup cpw::utils Capaware utilities library 
*/


namespace cpw 
{ 
	namespace utils
	{					 
		class FireAnimation;

		/** 
			\brief Por comentar...
			\ingroup cpw::utils 
		*/
		struct CPWUTILS_EXPORT Flame{
						Flame() { 
									cpw::IdGenerator id_generator;
									smoke.SetID(id_generator.GetNewId());
									fire.SetID(id_generator.GetNewId());
								}
						Flame(const Flame &flame): smoke(flame.smoke),fire(flame.fire)
						{}				

						cpw::ElementParticleSystem smoke;
						cpw::ElementParticleSystem fire;  
		};

		struct CPWUTILS_EXPORT TimeSlice{
							std::vector<cpw::utils::Flame> flames;
							cpw::cpwTime time_stamp;
							
													
		};

		/** 
			\brief
			\ingroup cpw::utils 
		*/
		class CPWUTILS_EXPORT Fire : public cpw::Layer
		{
			public:
							
				Fire(const cpw::TypeId id = cpw::TypeId(), 
					 const std::string &class_name = std::string("Fire"));

				Fire(const Fire &fire);

				~Fire(void);

				Fire &operator = (const Fire &fire);

				//ignition points
				void AddIgnitionPoint(const cpw::Point3d<float> &new_ignition_point = cpw::Point3d<float>(0.0f, 0.0f, 0.0f));
				int GetNumIgnitionPoints() { return (int) ignition_points.size(); }
				cpw::Point3d<float> * GetLastIgnitionPoint();
				cpw::Point3d<float> * GetIgnitionPoint(const int &index);
				void DeleteIgnitionPoint(const int &index);

				//Time slices
				TimeSlice * GetTimeSlice(const int &index);
				TimeSlice * GetLastTimeSlice();
				int GetNumTimeSlice() { return ((int) flames_progression.size()); }
				void AddTimeSlice(const cpw::utils::TimeSlice &new_time_slice = cpw::utils::TimeSlice());
				void DeleteTimeSlice(const int &index);

				//flames
				int AddFlame(const cpw::utils::Flame &new_flame = cpw::utils::Flame(), const int &time_slice_index = 0);
				int AddToCurrentFlames(const cpw::utils::Flame &new_flame = cpw::utils::Flame());
				cpw::utils::Flame &GetLastFlame();
				cpw::utils::Flame &GetLastFlameFromCurrentFlames() { return current_flames.back();}
				int GetNumFlames() { return ((int) current_flames.size()); }
				cpw::utils::Flame * GetFlame(const int &ind);
				virtual Entity* Clone() { return new Fire(*this); }
				void DeleteFlame(const int &ts_index, const int &flame_index);

				void UpdateFlamesInScene();
				
				void SetFireTexture(const std::string &_fire_texture)   { fire_texture = _fire_texture; Modified("fire_texture", fire_texture);	}
				void SetSmokeTexture(const std::string &_smoke_texture) { smoke_texture = _smoke_texture; Modified("smoke_texture", smoke_texture);}
				std::string GetFireTexture() { return ComposeDirectory(fire_texture); }
				std::string GetSmokeTexture() { return ComposeDirectory(smoke_texture); }
				const std::string GetFireTexture() const { return ComposeDirectory(fire_texture); }
				const std::string GetSmokeTexture() const { return ComposeDirectory(smoke_texture); }
				void SetPolygonal(const bool &_polygonal);
				bool &GetPolygonal() { return polygonal; }
				const bool &GetPolygonal() const { return polygonal; }
				void DeleteFlame(const int flame_index);

				cpw::ElementParticleSystem *GetFlameParticleSystem(int flame_index);

				void SetCurrentTime(const cpw::cpwTime &_current) { current_time = _current; Modified("current_time", current_time.seconds, current_time.miliseconds);}
				//void SetEndTime(const cpw::cpwTime &et) { end_time = et;}
				//void SetStartTime(const cpw::cpwTime &st) { start_time = st;}

				cpw::cpwTime &GetCurrentTime() { return current_time; }
				

				void InitCurrentFlames();
				void ClearCurrentFlames() { current_flames.clear(); }
				bool UpdateCurrentFlames(const cpw::cpwTime &time);
				bool NeedsToUpdate(const cpw::cpwTime &time);
				void MakeTimeSliceCurrent(const unsigned int &index);


				void SetIgnitionPointUrl(const std::string &ign_url){url_ign_point = ign_url; Modified("url_ign_point", url_ign_point);}
				std::string GetIgnitionPointUrl() { return ComposeDirectory(url_ign_point); }
				const std::string GetIgnitionPointUrl() const { return ComposeDirectory(url_ign_point); }

				virtual void SetValue(const std::string &f, const std::string &v);

				int GetActiveInterval(const cpw::cpwTime &time, double &position_between_intervals);

				void SetLastInsertedFire(const std::string _last_inserted_fire) { last_inserted_fire = _last_inserted_fire; }
				void SetLastInsertedSmoke(const std::string _last_inserted_smoke) { last_inserted_smoke = _last_inserted_smoke; }

				std::string & GetLastInsertedFire() { return last_inserted_fire; }
				std::string & GetLastInsertedSmoke() { return last_inserted_smoke; }

				void SetResetAnimation(const bool &_reset_animation) { reset_animation = _reset_animation; }
				bool & GetResetAnimation() { return reset_animation; }

				cpw::IGraphicFactory * GetGraphicFactory() { return graphic_factory; }
				void SetGraphicFactory(cpw::IGraphicFactory *_graphic_factory) { graphic_factory = _graphic_factory; }

				bool & IsAnimationPreComputed() { return animation_precomputed; }
				void SetAnimationPreComputed(const bool &_animation_precomputed) { animation_precomputed = _animation_precomputed; }

				std::vector<TimeSlice> & GetFlamesProgression() { return flames_progression; }
			
			private:
				
				std::string url_ign_point;
						
				//this will store the interpolation flames result in a 
				//determined time stamp
				std::vector<cpw::utils::Flame> current_flames; 

				bool reset_animation;

				std::vector<cpw::Point3d<float> > ignition_points;
				std::vector<TimeSlice> flames_progression;
				std::string fire_texture;
				std::string smoke_texture;
				bool polygonal;
				cpw::cpwTime current_time;

				std::string last_inserted_fire, last_inserted_smoke;
				
				bool fvisible, animation_precomputed;

				cpw::IGraphicFactory *graphic_factory;

			protected:
				virtual int CreatePersistence();
				virtual int CreateProperties();
				virtual int AdaptPersistence(cpw::Node *root);
		};

	}

}

#endif
