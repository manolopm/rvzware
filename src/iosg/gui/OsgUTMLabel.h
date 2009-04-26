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
#ifndef _OSGIUTMLABEL_
#define _OSGIUTMLABEL_

#include <iosg/gui/OsgLabel.h>
#include <iosg/OsgCamera.h>

#include <cpw/graphic/ApplicationScene.h>

#include <iostream>
#include <sstream>

#include <osgText/FadeText>

#include <iosg/Export.h>


namespace cpw 
{ 
	namespace iosg 
	{
		
		class OSgCamera;
		

		/** 
			\brief 
			\ingroup iosg
		*/
		class IOSGEXPORT OsgIUTMLabel : public OsgILabel
		{
			public:	

				OsgIUTMLabel(const std::string &url, cpw::Point3d<float> _position, cpw::Point3d<float> _size, cpw::Point3d<float> _rotation, const TAnchor &_anchor);
				~OsgIUTMLabel(void);

				//virtual void SetVisible(const bool &_visible);

				virtual void Update(osg::Matrix *mp);

				void SetCamera(OsgCamera *_camera) { camera = _camera; }
				OsgCamera * GetCamera() { return camera; }
				const OsgCamera * GetCamera() const { return camera; }

			private:

				std::string longitude, latitude, height;
				cpw::Point3d<float> rotation;
				OsgCamera *camera;
				OsgILabel *osg_labelUTM;

		};
	}
}

#endif



//#ifndef _OSGUTMLABEL_
//#define _OSGUTMLABEL_
//
//#include <iosg/gui/OsgContainer.h>
//#include <iosg/OsgLabel.h>
//#include <iosg/../OsgCamera.h>
//#include <cpw/graphic/ApplicationScene.h>
//
//#include <osgtext/fadetext>
//
//namespace cpw { namespace iosg 
//{
//	
//	class OSgCamera;
//	
//	class OsgIUTMLabel : public OsgContainer
//	{
//		public:	
//
//			OsgIUTMLabel(cpw::Point3d<float> _position, cpw::Point3d<float> _size, cpw::Point3d<float> _rotation, const TAnchor &_anchor);
//			~OsgIUTMLabel(void);
//
//			virtual void SetVisible(const bool &_visible);
//
//			virtual void Update(osg::Matrix *mp);
//
//		private:
//
//			std::string longitude, latitude, height;
//			cpw::Point3d<float> rotation;
//			OsgCamera *camera;
//			OsgILabel *osg_labelUTM;
//
//	};
//}
//
//#endif
