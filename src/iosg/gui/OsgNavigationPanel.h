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

#ifndef _OSGINAVIGATIONPANEL_
#define _OSGINAVIGATIONPANEL_

#include <osg/Timer>
#include <iosg/gui/OsgPanel.h>
#include <iosg/gui/OsgCompass.h>
#include <iosg/gui/OsgLabel.h>
#include <iosg/gui/OsgUTMLabel.h>
#include <iosg/gui/OsgVerticalButtonPanel.h>
#include <iosg/OsgCamera.h>
#include <iosg/OsgVisitorFirstGeode.h>
#include <iosg/gui/OsgHUDController.h>

#include <cpw/graphic/ApplicationScene.h>

#include <iosg/Export.h>

namespace cpw 
{ 
	namespace iosg 
	{
		class OsgCamera;
		class OsgICompass;
		class OsgIUTMLabel;
		class OsgIPanel;
		class OsgIHUDController;


		/** 
			\brief 3D GUI Navigation Panel
			\ingroup iosg
		*/
		class IOSGEXPORT OsgINavigationPanel : public OsgIPanel
		{
                        private:
                                OsgIWidget *widget;
			public:	

				OsgINavigationPanel(const std::string &url, cpw::Point3d<float> _position, cpw::Point3d<float> _size, cpw::Point3d<float> _rotation, const TAnchor &_anchor, bool lite_version_ = false);
				virtual ~OsgINavigationPanel(void);

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

				void SetCamera(OsgCamera *_camera);
				OsgCamera * GetCamera() { return camera; }
				const OsgCamera * GetCamera() const { return camera; }

				virtual void Update(osg::Matrix *mp=NULL);

				void RegisterButtons(OsgIHUDController *ohc);

				void StartToggleAnimation(const double &_animation_time = 0.35);
				void UpdateToggleAnimation();

			private:

				int signo;
				cpw::Point3d<float> rotation;
				cpw::Point3d<float> rotation2;
				cpw::Point3d<float> initial_size;
				cpw::Point3d<float> initial_rotation;
				cpw::Point3d<float> initial_position, preset_position;

				osg::Timer_t last_t, current_t;
				bool being_animated;
				bool is_hidden;
				bool lite_version;
				double current_animation_time, animation_time;
				OsgCamera *camera;

				OsgICompass *osg_compass;
				OsgIButton *osg_barbutton;
				OsgIVerticalButtonPanel *verticalButtonBar_panel;
				OsgILabel *osg_labelUTM;
				OsgILabel *osg_labelUTMl;
				OsgILabel *osg_labelUTMlt;
				OsgILabel *osg_labelUTMltl;
				OsgILabel *osg_labelHeight;
				OsgILabel *osg_labelTime;
				OsgIUTMLabel *osg_UTMkk;
		};
	}
}

#endif
