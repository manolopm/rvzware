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
#ifndef _IEVENTRECEIVER_
#define _IEVENTRECEIVER_

#include <cpw/Export.h>

namespace cpw
{
	/** 
		\brief Registers the state of the mouse buttons 
		\ingroup cpw	
	*/
	struct CPWEXPORT MouseButtonsState
	{
		bool mouse_left_down,   ///< whether the left mouse button is pressed
			 mouse_middle_down, ///< whether the middle mouse button is pressed
			 mouse_right_down;  ///< whether the right mouse button is pressed
	};

	/** Interface for the classes that may receive events
	*/
	class CPWEXPORT IEventReceiver
	{
		public:	

			/** Destructor */
			~IEventReceiver(void) {}
			
			/** This method is used to determined if the left mouse button was clicked twice
				\param x position on the screen
				\param y position on the screen
			*/
			virtual bool MouseLeftDoubleClick(const int &x, const int &y){ return false; }

			/** This method is used to determined if the right mouse button was clicked twice
				\param x position on the screen
				\param y position on the screen
			*/
			virtual bool MouseRightDoubleClick(const int &x,const int &y){ return false; }

			/** This method is used to determined if the middle mouse button was clicked twice
				\param x position on the screen
				\param y position on the screen
			*/
			virtual bool MouseMiddleDoubleClick(const int &x,const int &y){ return false; }
			

			/** This method is used to determined if the left mouse button is down
				\param x position on the screen
				\param y position on the screen
			*/
			virtual bool MouseLeftButtonDown(const int &x,const int &y){ return false; }

			/** This method is used to determined if the right mouse button is down
				\param x position on the screen
				\param y position on the screen
			*/
			virtual bool MouseRightButtonDown(const int &x,const int &y){ return false; }

			/** This method is used to determined if the middle mouse button was is down
				\param x position on the screen
				\param y position on the screen
			*/
			virtual bool MouseMiddleButtonDown(const int &x,const int &y){ return false; }


			/** This method is used to determined if the left mouse button is up
				\param x position on the screen
				\param y position on the screen
			*/
			virtual bool MouseLeftButtonUp(const int &x,const int &y){ return false; }

			/** This method is used to determined if the right mouse button is up
				\param x position on the screen
				\param y position on the screen
			*/
			virtual bool MouseRightButtonUp(const int &x,const int &y){ return false; }

			/** This method is used to determined if the middle mouse button is up
				\param x position on the screen
				\param y position on the screen
			*/
			virtual bool MouseMiddleButtonUp(const int &x,const int &y){ return false; }


			/** This method is used to determined if the mouse is dragging
				\param x0 initial position on the screen
				\param y0 initial position on the screen
				\param x1 target position on the screen
				\param y1 target position on the screen
				\param mbs for the state of the mouse buttons
			*/
			virtual bool MouseDrag(const int &x0, const int &y0, const int &x1, const int &y1, const MouseButtonsState &mbs){ return false; }

			/** This method is used to determined if the wheel of the mouse is moving
				\param wheel_rotation 
				\param wheel_delta 
				\param x
				\param y
			*/
			virtual bool MouseWheel(const int &wheel_rotation, const int &wheel_delta, const int &x=0, const int &y=0){ return false; }

			/** This method is used to determined if the mouse have left the current window
				\param x position on the screen
				\param y position on the screen
			*/
			virtual bool MouseLeaveWindow(const int &x, const int &y){ return false; }

			/** This method is used to determined if the mouse has entered into a window
				\param x position on the screen
				\param y position on the screen
			*/
			virtual bool MouseEnterWindow(const int &x, const int &y){ return false; }
					
			/** This method is used to determined if the user has pressed a key
				\param key pressed by the user
			*/
			virtual bool KeyDown(const int &key){ return false; }

			/** This method is used to determined if the user has released a key
				\param key pressed by the user
			*/
			virtual bool KeyUp(const int &key){ return false; }

			/** This method is used to determined if the actual window has resized
				\param x position on the screen
				\param y position on the screen
			*/
			virtual bool WindowResize(const int &x, const int &y) { return false; }

			/** This method is used to determined if the control has the focus
			*/
			virtual bool HasFocus(){ return false; }
	};
}

#endif



