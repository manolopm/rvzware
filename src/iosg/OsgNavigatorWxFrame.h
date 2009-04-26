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
#ifndef _NAVIGATORWXFRAME_
#define _NAVIGATORWXFRAME_


#define DRAW_WIREFRAME_MODE 3000
#define DRAW_SOLID_MODE 3001

#include <wx/wx.h>
#include <wx/string.h>
#include <osgViewer/Viewer>
#include <osg/PolygonMode>
#include <iosg/OsgNavigator.h>

#include <iosg/Export.h>


namespace cpw 
{
	namespace iosg 
	{
		class OsgNavigator;


		/** 
			\brief Por comentar ....
			\ingroup iosg
		*/
		class IOSGEXPORT OsgNavigatorWxFrame : public wxPanel
		{
			public:
				
				OsgNavigatorWxFrame(wxWindow *padre, const wxChar *titulo, int xpos=0, int ypos=0, int ancho=256, int alto=256, long style = wxDEFAULT_FRAME_STYLE | wxFULL_REPAINT_ON_RESIZE);
				~OsgNavigatorWxFrame(void);
				void SetViewer(osgViewer::Viewer* _osg_viewer){osg_viewer = _osg_viewer;}
				void SetID(unsigned int id) { frame_id = id; }
				void SetNavigator(OsgNavigator *nav) { navigator = nav;}
				wxBoxSizer * GetTopSizer() {return top_sizer;}

			private:
				
				void OnIdle(wxIdleEvent& event);
				void OnClose(wxCloseEvent& event);
				void OnSize(wxSizeEvent& event);

				//Drawing Mode
				void DrawModeSolid(wxCommandEvent& WXUNUSED(event));
				void DrawModeWireframe(wxCommandEvent& WXUNUSED(event));

				wxBoxSizer *top_sizer;
				
				osgViewer::Viewer *osg_viewer; 
				OsgNavigator *navigator;
				DECLARE_EVENT_TABLE()
				unsigned int frame_id;
				
		};
	}
}

#endif
