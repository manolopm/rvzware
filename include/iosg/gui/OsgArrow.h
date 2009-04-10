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
#ifndef _OSGIARROW_
#define	_OSGIARROW_

#include <string>
#include <iosg/gui/OsgImage.h>

#include <iosg/Export.h>

/**
	\namespace cpw { namespace iosg  OSG Implementation \brief This library is an implementation of the graphic interface of Capaware using the OSG library
    \defgroup iosg OSG package for the graphic interface
 */

namespace cpw 
{ 
	namespace iosg 
	{

		/** 
			\brief Creates an Arrow 3D widget
			\ingroup iosg
		*/
		class IOSGEXPORT OsgIArrow : public OsgIImage
		{

			public:
				OsgIArrow(const std::string &url, cpw::Point3d<float> _position, cpw::Point3d<float> _size, cpw::Point3d<float> _rotation, const TAnchor &_anchor);
				~OsgIArrow(void);

				//virtual void Update();
		};
	}
}

#endif