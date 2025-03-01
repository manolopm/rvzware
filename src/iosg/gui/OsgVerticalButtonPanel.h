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

#ifndef _OSGIVERTICALBUTTONPANEL_
#define _OSGIVERTICALBUTTONPANEL_

#include <iosg/gui/OsgContainer.h>
#include <iosg/gui/OsgButton.h>
#include <iosg/OsgVisitorFirstGeode.h>

#include <iosg/Export.h>


namespace cpw 
{ 
	namespace iosg 
	{

		/** 
			\brief Vertical Button Panel
			\ingroup iosg
			\param liteVersion Simple version with most important buttons
		*/
		class IOSGEXPORT OsgIVerticalButtonPanel : public OsgIContainer
		{
			public:	

				OsgIVerticalButtonPanel(const std::string &url, cpw::Point3d<float> _position, cpw::Point3d<float> _size, cpw::Point3d<float> _rotation, const TAnchor &_anchor, bool liteVersion = false);
				virtual ~OsgIVerticalButtonPanel(void);

				virtual bool MouseLeftDoubleClick(const int &x, const int &y){return false;}
				virtual bool MouseRightDoubleClick(const int &x,const int &y){return false;}
				virtual bool MouseMiddleDoubleClick(const int &x,const int &y){return false;}
				
				virtual bool MouseLeftButtonDown(const int &x,const int &y){return false;}
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

				virtual void Update(osg::Matrix *mp=NULL);

				OsgIButton * GetFireButton() { return fire_button; }
				OsgIButton * GetBrickButton() { return brick_button; }
				OsgIButton * GetMiniMapButton() { return minimap_button; }
				OsgIButton * GetCompassButton() { return compass_button; }
				OsgIButton * GetWMSButton() { return wms_button; }

			private:

				bool lite_version;
				cpw::Point3d<float> rotation;
				OsgIButton *button_array[2][4];
				OsgIButton *fire_button;
				OsgIButton *brick_button;
				OsgIButton *wms_button;
				OsgIButton *minimap_button;
				OsgIButton *spinl_button;
				OsgIButton *spinr_button;
				OsgIButton *compass_button;

				cpw::Point3d<float> switch_position;
				OsgIButton *fire_button2;
				OsgIButton *brick_button2;
				OsgIButton *wms_button2;
				OsgIButton *minimap_button2;

		};
	}
}

#endif
