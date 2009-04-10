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
#ifndef _OSGIMINIMAP_
#define _OSGIMINIMAP_

#include <sstream>
#include <osg/Timer>

#include <iosg/gui/OsgContainer.h>
#include <iosg/gui/OsgImage.h>
#include <iosg/OsgCamera.h>

#include <iosg/Export.h>

namespace cpw 
{ 
	namespace iosg 
	{
		class OsgCamera;


		/** 
			\brief Minimap widget
			\ingroup iosg
		*/
		class IOSGEXPORT OsgIMinimap : public OsgIContainer
		{
			public:	

				OsgIMinimap(const std::string &url, cpw::Point3d<float> _position, cpw::Point3d<float> _size, cpw::Point3d<float> _rotation, const TAnchor &_anchor);
				~OsgIMinimap(void);

				virtual bool MouseLeftDoubleClick(const int &x, const int &y);
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
				virtual void SetVisible(const bool &_visible);

				virtual void OsgIMinimap::Update(osg::Matrix *mp = NULL);

				void SetCamera(OsgCamera *_camera) { camera = _camera; }
				OsgCamera * GetCamera() { return camera; }
				const OsgCamera * GetCamera() const { return camera; }

				void StartToggleAnimation(const double &_animation_time = 0.35);
				void UpdateToggleAnimation();
				

			private:

				OsgCamera *camera;			
				OsgNavigator *navigator;
				OsgIImage *osg_arrow;
				cpw::Point3d<float> rotation, terrain_width_minmax_inc, terrain_height_minmax_inc;

				cpw::Point3d<float> initial_scale, initial_size;
				cpw::Point3d<float> initial_rotation;
				cpw::Point3d<float> initial_position, preset_position;
				iosg::TAnchor initial_anchor;

				osg::Timer_t last_t, current_t;
				bool being_animated, animated, pick_initial;
				bool is_hidden;
				int status;
				double current_animation_time, animation_time;
				

		};
	}
}

#endif