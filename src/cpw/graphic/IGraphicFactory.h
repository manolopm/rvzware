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
#ifndef _IGRAPHICFACTORY_
#define _IGRAPHICFACTORY_

#include <cpw/graphic/IScene.h>
#include <cpw/graphic/IValidator.h>
#include <cpw/graphic/ISceneLoader.h>
#include <cpw/graphic/IGraphicObjectLoader.h>
#include <cpw/graphic/INavigatorManager.h>
#include <cpw/graphic/ICamera.h>
#include <cpw/graphic/IOverlayImage.h>
#include <cpw/Export.h>

namespace cpw 
{

	/** 
		\brief Por comentar ....
		\ingroup cpw
	*/
	class IGraphicFactory
	{
		public:

			virtual ~IGraphicFactory(){};
			
			virtual cpw::IValidator           * CreateValidator() = 0;
			virtual cpw::IScene               * CreateScene() = 0;
			virtual cpw::ISceneLoader         * CreateSceneLoader() = 0;
			virtual cpw::INavigatorManager    * CreateNavigatorManager() = 0;
			virtual cpw::IGraphicObjectLoader * CreateObjectLoader() = 0;
			virtual cpw::ICamera              * CreateCamera() = 0;
			virtual cpw::IOverlayImage        * CreateOverlayImage(const cpw::Point2d<float> &utm0,
															const cpw::Point2d<float> &utm1,
															const int &x, const int &y, 
															const cpw::IOverlayImage::CPW_PIXEL_FORMAT &pf) = 0;
	};
}

#endif

