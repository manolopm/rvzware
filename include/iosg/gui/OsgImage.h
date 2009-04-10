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
#ifndef _OSGIIMAGE_
#define _OSGIIMAGE_

#include <iosg/gui/OsgWidget.h>

#include <iosg/Export.h>


namespace cpw 
{ 
	namespace iosg 
	{

		/** 
			\brief Image type widget
			\ingroup iosg
		*/
		class IOSGEXPORT OsgIImage : public OsgIWidget
		{
			public:	

				OsgIImage(const std::string &url, cpw::Point3d<float> _position, cpw::Point3d<float> _size, cpw::Point3d<float> _rotation, const TAnchor &_anchor, const std::string &filename = "");
				~OsgIImage(void);

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

		};
	}
}

#endif