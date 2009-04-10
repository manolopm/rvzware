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
#ifndef _GRAPHICFIRECALLBACK_
#define _GRAPHICFIRECALLBACK_

#include <cpw/callback/IGraphicCallBack.h>
#include <cpw/callback/ParticleSystemCallBack.h>
#include <cpw/callback/Model3DCallBack.h>
#include <cpw/common/types.h>

#include "FireAnimation.h"
#include "Exports.h"


namespace cpw 
{ 
	namespace utils
	{


		/** 
			\brief Por comentar...
			\ingroup utils 
		*/
		class CPWUTILS_EXPORT GraphicFireCallBack : public ParticleSystemCallBack, public Model3DCallBack
		{
			public:
				GraphicFireCallBack(void);
				GraphicFireCallBack(const GraphicFireCallBack &gfcb);
				~GraphicFireCallBack(void);
				
				virtual void Insert(const Callable* callable);
				virtual void Update(Callable* callable);
				virtual void Delete(Callable* callable);
				virtual void Animate(Callable* callable, cpwTime &time_stamp);
				virtual void Visualize(Callable* callable, const bool &value);

				//GraphicFireCallBack &operator = (const GraphicFireCallBack &gfcb){};
				virtual ICallBack* Clone() {return (ParticleSystemCallBack *) new GraphicFireCallBack(*this);}

			private:

				void DeleteCurrentFlames(Callable* callable);
				bool ignition_point_deleted;
				FireAnimation fire_animation;
				bool fire_inserted, animate_this_frame, reset_animation;

				int update_animation_every_n_frames, frames_count;
				
				
		};

	}

}

#endif
