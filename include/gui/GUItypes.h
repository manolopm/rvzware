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
#ifndef _GUI_TYPES_
#define _GUI_TYPES_

#include <wx/wx.h>
#include <wx/colour.h>
#include <wx/calctrl.h>
#include <wx/datectrl.h>
#include <gui/MyTimeCtrl.h>

namespace cpw 
{ 
	namespace gui 
	{

		/** 
			\brief Types of the user interface
			\ingroup cpw::gui
		*/

		#define WX_COLOUR_ASH	wxColour(203,203,181)
		#define WX_COLOUR_ONION wxColour(255,255,249)
		#define WX_COLOUR_SILK_BLUE wxColour(148,165,187)
		#define WX_COLOUR_SILK_PALEBLUE wxColour(207,222,241)
		#define WX_COLOUR_SILK_DARKBLUE wxColour(131,151,175)
		#define WX_COLOUR_SILK_ORANGE wxColour(235,212,168)
		#define WX_COLOUR_GREEN wxColour(0,255,0)

		//black-set
		#define WX_COLOUR_BLACK wxColour(0,0,0)
		#define WX_COLOUR_GREY250 wxColour(250,250,250)
		#define WX_COLOUR_GREY180 wxColour(180,180,180)
		#define WX_COLOUR_GREY100 wxColour(100,100,100)
		#define WX_COLOUR_GREY60 wxColour(60,60,60)
		#define WX_COLOUR_GREY30 wxColour(30,30,30)

		typedef struct
		{
			wxDatePickerCtrl *Date;
			MyTimeCtrl *Hour;
		} abstractTime;

	}
}

#endif





