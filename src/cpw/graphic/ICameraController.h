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
#ifndef _ICAMERACONTROLLER_
#define _ICAMERACONTROLLER_

#include <cpw/graphic/ICamera.h>
#include <cpw/graphic/INavigatorManager.h>
#include <cpw/callback/IEventReceiver.h>

#define KEY_CURSOR_DOWN 317
#define KEY_CURSOR_UP 315
#define KEY_CURSOR_LEFT 314
#define KEY_CURSOR_RIGHT 316
#define KEY_A 65
#define KEY_Z 90
#define KEY_LCTRL 308
#define KEY_LEFT_SHIFT 306

namespace cpw
{
	class INavigatorManager;


	/** 
		\brief Por comentar ....
		\ingroup cpw
	*/
	class ICameraController : public IEventReceiver
	{
		public:

			ICameraController(){}
			virtual ~ICameraController(){}
			virtual ICameraController * Clone() = 0;
			
			virtual bool MouseLeftDoubleClick(const int &x, const int &y){ return false; }
			virtual bool MouseRightDoubleClick(const int &x,const int &y){ return false; }
			virtual bool MouseMiddleDoubleClick(const int &x,const int &y){ return false; }
			
			virtual bool MouseLeftButtonDown(const int &x,const int &y){ return false; }
			virtual bool MouseRightButtonDown(const int &x,const int &y){ return false; }
			virtual bool MouseMiddleButtonDown(const int &x,const int &y){ return false; }

			virtual bool MouseLeftButtonUp(const int &x,const int &y){ return false; }
			virtual bool MouseRightButtonUp(const int &x,const int &y){ return false; }
			virtual bool MouseMiddleButtonUp(const int &x,const int &y){ return false; }

			virtual bool MouseDrag(const int &x0, const int &y0, const int &x1, const int &y1, const MouseButtonsState &mbs){ return false; }
			virtual bool MouseWheel(const int &wheel_rotation, const int &wheel_delta){ return false; }

			virtual bool MouseLeaveWindow(const int &x0, const int &y0){ return false; }
			virtual bool MouseEnterWindow(const int &x0, const int &y0){ return false; }
					
			virtual bool KeyDown(const int &key){ return false; }
			virtual bool KeyUp(const int &key){ return false; }

			virtual bool HasFocus(){ return false; }

			virtual void GoTo(const cpw::Point3d<float> &position, const double &time){}

			virtual void UpdateTimer(){}

			virtual void SetCamera(ICamera *_camera) = 0;
			virtual ICamera * GetCamera() = 0;

			virtual void SetNavigatorManager(INavigatorManager *_navigator_manager) = 0;
			virtual INavigatorManager * GetNavigatorManager()  = 0;
	};
}

#endif



