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

#include <iosg/stdafx.h>

#include <iosg/OsgNavigatorWxFrame.h>

using namespace cpw::iosg;

BEGIN_EVENT_TABLE(OsgNavigatorWxFrame, wxPanel)
				EVT_MENU ( DRAW_WIREFRAME_MODE, OsgNavigatorWxFrame::DrawModeWireframe	)
				EVT_MENU ( DRAW_SOLID_MODE, OsgNavigatorWxFrame::DrawModeSolid	)
				EVT_IDLE        ( OsgNavigatorWxFrame::OnIdle				)
				EVT_CLOSE( OsgNavigatorWxFrame::OnClose )
				EVT_SIZE ( OsgNavigatorWxFrame::OnSize )
END_EVENT_TABLE()


OsgNavigatorWxFrame::OsgNavigatorWxFrame(wxWindow *padre, const wxChar *titulo, int xpos, int ypos, int ancho, int alto, long style)
				//: wxMDIChildFrame(padre, wxID_ANY, titulo, wxPoint(xpos, ypos), wxSize(ancho, alto), style)
				: wxPanel(padre, wxID_ANY)//, wxPoint(xpos, ypos), wxSize(ancho, alto), style)
{
	/*wxMenuBar *menu_bar = new wxMenuBar();
	wxMenu *draw_menu = new wxMenu;
	draw_menu->Append(DRAW_WIREFRAME_MODE, wxString(_T("Wireframe")), wxString(_T("Wireframe")));
	draw_menu->Append(DRAW_SOLID_MODE, wxString(_T("Solid")), wxString(_T("Solid")));
	menu_bar->Append(draw_menu, _T("Drawing mode"));
	this->SetMenuBar(menu_bar);*/
	//this->SetMinSize(wxSize(64, 64));

	//top_sizer = new wxBoxSizer(wxVERTICAL);
	//this->SetSizer(top_sizer);
	//top_sizer->Add(tree_ctrl,1,wxGROW);

	
	

}

OsgNavigatorWxFrame::~OsgNavigatorWxFrame(void)
{

}

void OsgNavigatorWxFrame::OnIdle(wxIdleEvent& event)
{
	//navigator->DrawMe();
	//event.RequestMore();
	//osg_viewer->frame();

}

void OsgNavigatorWxFrame::OnClose(wxCloseEvent& event)
{
	navigator->DeleteMe();
	delete this;
	
}


void OsgNavigatorWxFrame::DrawModeSolid(wxCommandEvent& WXUNUSED(event))
{
	osg::PolygonMode *pm = new osg::PolygonMode;
	pm->setMode(osg::PolygonMode::FRONT, osg::PolygonMode::FILL);
			
	osg::StateSet *st = osg_viewer->getCamera()->getOrCreateStateSet();
	st->setAttribute(pm);
}

void OsgNavigatorWxFrame::DrawModeWireframe(wxCommandEvent& WXUNUSED(event))
{
	osg::PolygonMode *pm = new osg::PolygonMode;
	pm->setMode(osg::PolygonMode::FRONT, osg::PolygonMode::LINE);
	
	osg::StateSet *st = osg_viewer->getCamera()->getOrCreateStateSet();
	st->setAttribute(pm);
}

void OsgNavigatorWxFrame::OnSize(wxSizeEvent& event)
{
	////navigator->GetGlCanvas()->tr
	//
	//int width, height;
 //   GetClientSize(&width, &height);
	//wxSizeEvent event2(wxSize(width, height));
	
    // update the window dimensions, in case the window has been resized.
    //navigator->GetGlCanvas()->OnSize(event2);
	//navigator->GetGlCanvas()->SetClientSize(width, height);
	//navigator->ReplaceCanvas(width, height);
	//osg_viewer->getCamera()->setViewport(0,2048-height,width,height);
	//resized(0,0,width,height);
}
