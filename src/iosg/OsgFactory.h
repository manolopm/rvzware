/*
 * Rvzware based in CAPAWARE 3D
 *
 * Rvzware is free software; you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 2.1 of the License, or (at your option)
 * any later version.
 *
 * Rvzware is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this application; if not, write to the Free Software Foundation, Inc.,
 * 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 *
 * The Rvzware development team
 */

#ifndef _OSGFACTORY_
#define _OSGFACTORY_

#include <cpw/graphic/IGraphicFactory.h>
#include <cpw/IApplication.h>

#include <iosg/Export.h>


namespace cpw 
{ 
	namespace iosg 
	{

		/** 
			\brief Por comentar...
			\ingroup iosg
		*/
		class IOSGEXPORT OsgFactory : public cpw::IGraphicFactory
		{
			public:
				OsgFactory(cpw::IApplication *_app, bool lite_version_ = false);
				~OsgFactory();

				cpw::IValidator           * CreateValidator();
				cpw::IScene               * CreateScene();
				cpw::ISceneLoader         * CreateSceneLoader();
				cpw::INavigatorManager    * CreateNavigatorManager();
				cpw::IGraphicObjectLoader * CreateObjectLoader();
				cpw::ICamera              * CreateCamera();
				cpw::IOverlayImage        * CreateOverlayImage(const cpw::Point2d<float> &utm0,
														const cpw::Point2d<float> &utm1,
														const int &x, const int &y, 
														const cpw::IOverlayImage::CPW_PIXEL_FORMAT &pf);
				bool GetLiteVersion() {return lite_version;};
				
			private:

				cpw::IApplication *application;
				bool lite_version;

		};
	}
}

#endif 
