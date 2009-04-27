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
#ifndef _FLAME_
#define _FLAME_

#include <cpw/entity/Layer.h>
#include <cpwutils/Exports.h>

namespace cpw 
{ 
	namespace utils
	{

		/** 
			\brief Por comentar...
			\ingroup cpw::utils 
		*/
		struct CPWUTILS_EXPORT Flame{
							Flame()
							{
								alpha_fire.max = 0.0f; alpha_fire.min = 1.0f;
								speed_fire.max = 600.0f;speed_fire.min = 550.0f;
								phi_fire.max = 360.0f; phi_fire.min = 0.0f;
								theta_fire.max = 12.0f; theta_fire.min = 0.0f;
								size_fire.max = 200.0f; size_fire.min = 120.0f;
								particles_sec_fire.max = 30; particles_sec_fire.min = 25;
								additive_blend_fire = true; senoidal_interpolator_fire = false;
								particles_life_fire = 1.0f;

		    					alpha_smoke.max = 0.5f; alpha_smoke.min = 0.0f;
								speed_smoke.max = 600.0f; speed_smoke.min = 550.0f;
								phi_smoke.max = 360.0f; phi_smoke.min = 0.0f;
								theta_smoke.max = 12.0f; theta_smoke.min = 0.0f;
								size_smoke.max = 300.0f; size_smoke.min = 250.0f;
								particles_sec_smoke.max = 20; particles_sec_smoke.min = 15;
								additive_blend_smoke = false;
								senoidal_interpolator_smoke = true;
								particles_life_smoke = 6.0f;

								coords.x = 0.0f; coords.y = 0.0f; coords.z = 0.0f;
							  }

							  cpw::Point3d<float> coords;
							  int id;

							  cpw::Range<float> alpha_fire;
							  cpw::Range<float> speed_fire;
							  cpw::Range<float> phi_fire;
							  cpw::Range<float> theta_fire;		   
							  cpw::Range<float> size_fire;
							  cpw::Range<int> particles_sec_fire;
							  bool senoidal_interpolator_fire;
							  float particles_life_fire;		
							  bool additive_blend_fire;

							  cpw::Range<float> alpha_smoke;
							  cpw::Range<float> speed_smoke;
							  cpw::Range<float> phi_smoke;
							  cpw::Range<float> theta_smoke;		   
							  cpw::Range<float> size_smoke;
							  cpw::Range<int> particles_sec_smoke;
							  bool senoidal_interpolator_smoke;
							  float particles_life_smoke;
							  bool additive_blend_smoke;

						   };

	}

}

#endif
