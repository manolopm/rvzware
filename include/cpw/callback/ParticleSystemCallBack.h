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
#ifndef _PARTICLESYSTEMCALLBACK_
#define _PARTICLESYSTEMCALLBACK_

#include <cpw/callback/IGraphicCallBack.h>
#include <cpw/Export.h>

namespace cpw
{

	/** 
		\brief This class implements the IGraphicCallBack interface for the ElementParticleSystem entity
		\ingroup cpw
	*/
	class CPWEXPORT ParticleSystemCallBack : public IGraphicCallBack
	{
		public:
			/** Default constructor */
			ParticleSystemCallBack();

			/** Copy constructor
				\param callback
			*/	
			ParticleSystemCallBack(const ParticleSystemCallBack &callback);

			/** Destructor */
			virtual ~ParticleSystemCallBack();

			/** Overload of operator = 
				\param callback
			*/
			ParticleSystemCallBack &operator = (const ParticleSystemCallBack &callback);
			
			/** Implementation of the Insert method
				\param callable
			*/
			virtual void Insert(const cpw::Callable* callable);

			/** Implementation of the Update method
				\param callable
			*/
			virtual void Update(cpw::Callable* callable) ;

			/** Implementation of the Delete method
				\param callable
			*/
			virtual void Delete(cpw::Callable* callable) ;

			/** Implementation of the Insert method
				\param callable
				\param time
			*/
			virtual void Animate(cpw::Callable* callable, cpw::cpwTime &time);

			/** Implementation of the Insert method
				\param callable
				\param value 
			*/
			virtual void Visualize(cpw::Callable* callable, const bool &value);
			

			/** Implementation of the Clone method */
			virtual ICallBack* Clone();
	};
}
#endif
