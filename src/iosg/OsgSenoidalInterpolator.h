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
#ifndef _OSGSENOIDALINTERPOLATOR_
#define _OSGSENOIDALINTERPOLATOR_

#include <osgParticle/LinearInterpolator>
#include <math.h>

#include <iosg/Export.h>


namespace cpw 
{ 
	namespace iosg  
	{

		/** 
			\brief Por comentar ....
			\ingroup iosg
		*/
		class IOSGEXPORT OsgSenoidalInterpolator : public osgParticle::LinearInterpolator
		{
			public:
				OsgSenoidalInterpolator(void);
				~OsgSenoidalInterpolator(void);

			
			float interpolate (float t, float y1, float y2)const
			{
				float sin_value, difference;
				difference = y2 - y1;
				sin_value = sin(t * osg::PI); //da un valor entre cero y uno
				return ((difference * sin_value) + y1);
			}
		};
	}
}

#endif


