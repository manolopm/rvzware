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

#ifndef _TWODIMESIONCAMERACONTROLLER_
#define _TWODIMESIONCAMERACONTROLLER_

#include <cpw/graphic/ICameraController.h>
#include <cpw/graphic/INavigatorManager.h>

#define KEY_A 65
#define KEY_Z 90

namespace cpw
{
	/** 
		\brief Por comentar ....
		\ingroup cpw
	*/
	class CPWEXPORT TwoDimensionCameraController : public ICameraController
	{
		public:

			TwoDimensionCameraController(void);
			~TwoDimensionCameraController(void);
			TwoDimensionCameraController(const TwoDimensionCameraController &cc);

			virtual ICameraController * Clone() { return new TwoDimensionCameraController(*this); }

			virtual bool MouseLeftDoubleClick(const int &x, const int &y);
						
			virtual bool MouseLeftButtonDown(const int &x,const int &y);//{ return false; }
			virtual bool MouseRightButtonDown(const int &x,const int &y);//{ return false; }
			virtual bool MouseMiddleButtonDown(const int &x,const int &y);//{ return false; }

			virtual bool MouseLeftButtonUp(const int &x,const int &y);//{ return false; }
			virtual bool MouseRightButtonUp(const int &x,const int &y);//{ return false; }
			virtual bool MouseMiddleButtonUp(const int &x,const int &y);//{ return false; }

			virtual bool MouseDrag(const int &x0, const int &y0, const int &x1, const int &y1, const MouseButtonsState &mbs);//{ return false; }
			virtual bool MouseWheel(const int &wheel_rotation, const int &wheel_delta, const int &x=0, const int &y=0);//{ return false; }

			virtual bool MouseLeaveWindow(const int &x0, const int &y0);//{ return false; }
			virtual bool MouseEnterWindow(const int &x0, const int &y0);//{ return false; }
					
			virtual bool KeyDown(const int &key);//{ return false; }
			virtual bool KeyUp(const int &key);//{ return false; }	

			virtual bool WindowResize(const int &x, const int &y);//{ return false; }

			virtual void SetCamera(ICamera *_camera) { camera = _camera; camera->SetCorrectUp(false); } 
			virtual ICamera * GetCamera() { return camera; }
			virtual void SetNavigatorManager(INavigatorManager *_navigator_manager) { navigator_manager = _navigator_manager; }
			virtual INavigatorManager * GetNavigatorManager() { return navigator_manager; }

			virtual void UpdateTimer() { if (camera != NULL) camera->UpdateTimer(); }

			virtual void GoTo(const cpw::Point3d<float> &position, const double &time);

		private:

			void UpdateMouse(const MouseButtonsState &mbs);
		
			ICamera *camera;
			INavigatorManager *navigator_manager;
			int window_size_x, window_size_y;

			bool left_button_pressed, middle_button_pressed, right_button_pressed;
			bool ctrl_key_pressed, left_shift_key_pressed;


	};
}

#endif


