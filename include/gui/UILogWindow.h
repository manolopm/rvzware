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
#ifndef _UILOGWINDOW_
#define _UILOGWINDOW_


#include <wx/wx.h>
#include <wx/textctrl.h>
#include <OpenThreads/Mutex>
#include "UIApplicationMainFrame.h"

#include <cpw/common/Logger.h>

namespace cpw 
{ 
	
	namespace gui
	{

		/** 
			\brief Log Window that shows debug info
			\ingroup cpw
		*/
		class UIApplicationMainFrame;

		class UILogWindow : public wxPanel, public cpw::Logger
		{
			public:
				UILogWindow(wxWindow *parent, const int &sizex=256, const int &sizey=256, const int &posx=0, const int &posy=0);
				~UILogWindow(void);

				virtual void NewLogMessage(const std::string &new_message);
				virtual void ClearLog();
				virtual void printf (const char *format, ...);

			private:

				wxTextCtrl *log_text;
				OpenThreads::Mutex mutex;
		};
	}

}

#endif


