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

#ifndef _OSGIBUTTON_
#define _OSGIBUTTON_

#include <osg/Timer>

#include <iosg/gui/OsgWidget.h>


#include <iosg/Export.h>

namespace cpw 
{ 
	namespace iosg 
	{

		/** 
			\brief 
			\ingroup iosg
		*/
		class IOSGEXPORT OsgIButton : public OsgIWidget
		{
			public:	

				OsgIButton(const std::string &url, cpw::Point3d<float> _position, cpw::Point3d<float> _size, cpw::Point3d<float> _rotation, const TAnchor &_anchor, const std::string &filename="");
				~OsgIButton(void);

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

				virtual void StartPickAnimation(const double &_animation_time);
				virtual void UpdateAnimation();
				virtual bool & BeingAnimated() { return being_animated; }

				virtual void Update(osg::Matrix *mp=NULL);

				//virtual void Translate(const int &x, const int &y){}
				//virtual void Rotate(const int &x, const int &y){}
				//virtual void Scale(const int &x, const int &y){}

			private:

				double current_animation_time, animation_time;
				bool being_animated, pick_initial;

				osg::Timer_t last_t, actual_t;
				cpw::Point3d<float> initial_size, initial_rotation, initial_scale;
				

		};
	}
}

#endif
