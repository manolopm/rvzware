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
#ifndef _OSGICOMPASSS_
#define _OSGICOMPASSS_

#include <iosg/gui/OsgContainer.h>
#include <iosg/OsgCamera.h>

#include <iosg/Export.h>

namespace cpw 
{ 
	namespace iosg 
	{
		class OsgCamera;


		/** 
			\brief Creates a 3D Compass widget
			\ingroup iosg
		*/
		class IOSGEXPORT OsgICompass : public OsgIContainer
		{
			public:	

				OsgICompass(const std::string &url, cpw::Point3d<float> _position, cpw::Point3d<float> _size, cpw::Point3d<float> _rotation, const TAnchor &_anchor);
				virtual ~OsgICompass(void);

				virtual bool MouseLeftDoubleClick(const int &x, const int &y){return false;}
				virtual bool MouseRightDoubleClick(const int &x,const int &y){return false;}
				virtual bool MouseMiddleDoubleClick(const int &x,const int &y){return false;}
				
				virtual bool MouseLeftButtonDown(const int &x,const int &y);
				virtual bool MouseRightButtonDown(const int &x,const int &y){return false;}
				virtual bool MouseMiddleButtonDown(const int &x,const int &y){return false;}

				virtual bool MouseLeftButtonUp(const int &x,const int &y){return false;}
				virtual bool MouseRightButtonUp(const int &x,const int &y){return false;}
				virtual bool MouseMiddleButtonUp(const int &x,const int &y){return false;}

				virtual bool MouseDrag(const int &x0, const int &y0, const int &x1, const int &y1){return false;}
				virtual bool MouseWheel(const int &wheel_rotation, const int &wheel_delta){return false;}

				virtual bool MouseLeaveWindow(const int &x0, const int &y0){return false;}
				virtual bool MouseEnterWindow(const int &x0, const int &y0){return false;}
						
				virtual bool KeyDown(const int &key){return false;}
				virtual bool KeyUp(const int &key){return false;}

				virtual bool HasFocus(){ return false; }

				void SetCamera(OsgCamera *_camera) { camera = _camera; }
				OsgCamera * GetCamera() { return camera; }
				const OsgCamera * GetCamera() const { return camera; }

				//OsgIRingButton * GetRingButton() { return ring_button; }

				virtual void Update(osg::Matrix *mp=NULL);

				virtual void SetVisible(const bool &_visible);



			private:

				OsgCamera *camera;
				cpw::Point3d<float> rotation;
				//OsgIRingButton *ring_button; 
				int status;
		};
	}
}

#endif