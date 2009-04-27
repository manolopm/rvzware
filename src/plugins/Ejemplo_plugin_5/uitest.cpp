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

#include "stdafx.h"
#include <vector>

#include <cpw/entity/Element3D.h>
#include <cpw/callback/Model3DCallBack.h>
#include <cpw/persistent/file/PersistentFileCallBack.h>


#include "UITest.h"

// para poder hacer el printf
#include <cpw/common/ApplicationLog.h>
#define printf cpw::ApplicationLog::GetInstance()->GetLogger()->printf



enum
{
	ID_BROWSEBUTTON = 1014,
	ID_PRIMITIVE_CHOICE = 1013,
	BUTTON_SELECT_HTML = 1011,
	ID_TC_DESCRIPTION = 1010,
	ID_WXSTATICTEXT3 = 1009,
	ID_BUTTON_CANCEL = 1008,
	ID_BUTTON_OK = 1007,
	BUTTON_ATTRIBUTES = 1005,
	ID_TC_HTML = 1004,
	ID_TC_NAME = 1003,
};

BEGIN_EVENT_TABLE(UITest,wxDialog)
	EVT_BUTTON  (wxID_OK,				UITest::OnButtonOK)
	EVT_BUTTON  (wxID_CANCEL,			UITest::OnButtonCancel)
	EVT_BUTTON  (BUTTON_ATTRIBUTES,		UITest::OnButtonAttributes)
	EVT_BUTTON  (BUTTON_SELECT_HTML,	UITest::OnButtonHtml)
	EVT_BUTTON	(ID_BROWSEBUTTON,		UITest::OnBrowsePrimitivesButton)
    EVT_MOVE    (UITest::OnMove										)
	EVT_PAINT	(						UITest::OnPaint )
	EVT_CHOICE		(ID_PRIMITIVE_CHOICE, UITest::OnChoiceChanged)
	EVT_IDLE	 ( UITest::OnIdle    )
	EVT_CLOSE     (UITest::OnClose)


END_EVENT_TABLE()

void UITest::OnMove(wxMoveEvent& event)
{
}


UITest::UITest(std::vector<cpw::Entity*> &_ventity, cpw::INavigatorManager *_navigator_manager, 
			   double posx, double posy,
			   wxWindow* _parent, wxWindowID id, const wxString &title, 
			   const wxPoint &position, const wxSize& size, long style)
: wxDialog(_parent, id, title, position, size, style), paren(NULL), modify(false),primitive_url(), 
				open_primitive(NULL), navigator_manager(_navigator_manager), parent(_parent),
				ventity(_ventity)
{
	
	SetMouseCallBack();

	CreateGUIControls(posx, posy);
}

UITest::~UITest()
{
	if (open_primitive!=NULL) delete open_primitive;
} 

void UITest::OnButtonCancel(wxCommandEvent& WXUNUSED(event))
{

		ReleaseMouseCallBack();

/*	if((!tc_name->IsEmpty())||(!tc_description->IsEmpty())||(!tc_html->IsEmpty()))
	{
		wxMessageDialog message(this,wxString("Save changes before quit?"), wxString("Warning"),wxICON_EXCLAMATION |wxYES_NO |wxCANCEL);
		int modal = message.ShowModal();
		if(modal == wxID_YES)
		{		
			ButtonOK();
		}
		if(modal == wxID_NO)
		{
			this->EndModal(wxID_CANCEL);
		}		
	}
	else
	{
		this->EndModal(wxID_CANCEL);
	}
	*/

	this->EndModal(wxID_CANCEL);
	//printf ("cancelando...");
}

void UITest::SetPrimitivesUrl(const std::map<std::string, cpw::TypeId> &vec_url)
{
	v_url = vec_url;
	std::map<std::string, cpw::TypeId>::const_iterator i = vec_url.begin();
	for(i; i!=vec_url.end();i++)
	{
		wxString* url = new wxString((wxString)(i->first));
		primitiveChoice->Insert(*url,0);
		delete url;
	}
}

void UITest::OnChoiceChanged(wxCommandEvent& WXUNUSED(event))
{
    primitive_url = "";
	if (primitiveChoice->GetSelection() != wxNOT_FOUND)
	{		
		std::string pname = primitiveChoice->GetString(primitiveChoice->GetSelection());
		std::map<std::string, cpw::TypeId>::iterator i = v_url.find(pname);
		if (i!= v_url.end())
		{
			primitive_id = i->second;
			button_ok->Enable(true);
		}
	}

}

void UITest::OnBrowsePrimitivesButton(wxCommandEvent& event)
{
    primitive_url = "";
	primitive_id = cpw::TypeId();
    if(open_primitive->ShowModal() == wxID_OK ) 
    {
        primitive_url = open_primitive->GetPath();
		std::string pname = primitiveChoice->GetString(primitiveChoice->GetSelection());
		primitiveChoice->Clear();
		button_ok->Enable(true);
	}
}


void UITest::OnClose(wxCloseEvent& event)
{
	ReleaseMouseCallBack();
}

void UITest::OnButtonOK(wxCommandEvent& WXUNUSED(event))
{
	ButtonOK();
}

void UITest::OnButtonHtml(wxCommandEvent& WXUNUSED(event))
{

		ReleaseMouseCallBack();

	std::string root_path;
	wxFileDialog dialog(this,_T("Open HTML file"),_T(root_path),wxEmptyString,
						   _T("File html(*.html)|*.html|All files(*.*)|*.*") );
	if(dialog.ShowModal() == wxID_OK)
	{		
		tc_html->SetValue(dialog.GetPath());
	}
}


void UITest::OnButtonAttributes(wxCommandEvent& WXUNUSED(event))
{
}



void UITest::OnPaint(wxPaintEvent& event)
{
	wxPaintDC dc(this);
	render(dc);
}


void UITest::render(wxDC& dc)
{
	int client_h = 0;
	int client_w = 0;
	GetClientSize (&client_w, &client_h);

	// aquí se ponen los label
	dc.DrawRotatedText(_T("Coord X: "),19,35-25+10-3-4+10, 0);
	dc.DrawRotatedText(_T("Coord Y: "),19,35+10-3-4+10, 0);
	dc.DrawRotatedText(_T("Segundos:"),19,35+25+10-3+10, 0);
	//dc.DrawRotatedText(_T("Description "),19,35+50+10+10, 0);
}


//*********************************************************************************

void UITest::OnIdle(wxIdleEvent& event)
{
	parent->OnInternalIdle();	
	event.RequestMore();
}


void UITest::SetMouseCallBack()
{
	mouse_event = new UITestEventReceiver();
	mouse_event->SetFireSimulatorController(this);
	mouse_event->SetNavigatorManager(navigator_manager);
	cpw::INavigator *navigator = navigator_manager->GetFocusedOrFirstNavigator();
	int nc = navigator->GetEventReceiversCount();
	if (nc>0)
		navigator->SetEventReceiver(nc-1, mouse_event);

}

void UITest::ReleaseMouseCallBack()
{
	cpw::INavigator *navigator = navigator_manager->GetFocusedOrFirstNavigator();
	navigator->RemoveEventReceiver(mouse_event);

	if(mouse_event != NULL)
	{
		delete mouse_event;
		mouse_event = NULL;
	}

}



UITestEventReceiver::UITestEventReceiver(void): drag(false)
{
	
}

UITestEventReceiver::~UITestEventReceiver(void)
{

}

UITestEventReceiver::UITestEventReceiver(const UITestEventReceiver &fscer)
{

}

bool UITestEventReceiver::MouseLeftButtonDown(const int &x,const int &y)
{
	drag = false;

	return false;

}
bool UITestEventReceiver::MouseLeftButtonUp(const int &x,const int &y)
{

	if(!drag) 
		fsc->MouseEvent(x, y, wxMOUSE_BTN_LEFT);
		
	return false;
}

bool UITestEventReceiver::MouseDrag(const int &x0, const int &y0, const int &x1, const int &y1, const cpw::MouseButtonsState &mbs)
{
	drag = true;

	return false;
}

void UITestEventReceiver::SetFireSimulatorController(UITest *_FireSimulationController)
{ 
	fsc = _FireSimulationController;
}

void UITest::ButtonOK()
{
	ReleaseMouseCallBack();
	EndModal(wxID_OK);
}



// ************************************************************************************
//****************************************************************************
//****************************************************************************

long UITest::GetSeconds()
{
	long segs;
	tc_name3->GetValue().Trim().ToLong(&segs);
	return segs;
}

void UITest::MouseEvent(int x, int y, int z)
{
	float ipx, ipy, ipz;

	switch (z)
	{
		case wxMOUSE_BTN_LEFT:
		{	
			char Texto_x[100], Texto_y[100];
			navigator_manager->GetFocusedOrFirstNavigator()->IntersectMouseWithScene(x,y, ipx, ipy, ipz);
			printf ("estoy picando en la escena!!  Punto UTM: (%.2f, %.2f, %.2f)  Texto: %s",
						ipx, ipy, ipz, tc_name->GetValue().c_str());	
			sprintf (Texto_x, "%.2f", ipx); 
			tc_name->SetValue (Texto_x);
			sprintf (Texto_y, "%.2f", ipy); 
			tc_name2->SetValue (Texto_y);
		}
					
		case wxMOUSE_BTN_MIDDLE:			
			break;

		case wxMOUSE_BTN_RIGHT:
			break;
	}
}


void UITest::CreateGUIControls(double posx, double posy)
{
	SetTitle(wxT("Test para el plugin"));
	SetIcon(wxNullIcon);
	SetSize(8,8,390,335-44);

	//Center();
	
	int col_= 20;int row_ = 20;

	//std::string primitive_path;
	//open_primitive =  new wxFileDialog(this, wxT("Choose a file"), wxT(primitive_path), wxT(""), wxT("*.cla"), wxOPEN);

	// campo de texto para la coordenada x
	char texto[100];
	tc_name= new wxTextCtrl(this, ID_TC_NAME, wxT(""), wxPoint(col_+100,row_), wxSize(200,19), 0, wxDefaultValidator, wxT(""));
	tc_name->SetFont(wxFont(8, wxSWISS, wxNORMAL,wxNORMAL, false, wxT("Tahoma")));
	tc_name->SetFocus();
	sprintf (texto, "%.3f", posx); 
	tc_name->SetValue (texto);
	row_ += 25;

	// campo de texto para la coordenada y
	tc_name2= new wxTextCtrl(this, ID_TC_NAME, wxT(""), wxPoint(col_+100,row_), wxSize(200,19), 0, wxDefaultValidator, wxT(""));
	tc_name2->SetFont(wxFont(8, wxSWISS, wxNORMAL,wxNORMAL, false, wxT("Tahoma")));
	//tc_name2->SetFocus();
	sprintf (texto, "%.3f", posy); 
	tc_name2->SetValue (texto);
	row_ += 25;

	// campo de texto para el numero de segundos de la simulación
	tc_name3= new wxTextCtrl(this, ID_TC_NAME, wxT(""), wxPoint(col_+100,row_), wxSize(200,19), 0, wxDefaultValidator, wxT(""));
	tc_name3->SetFont(wxFont(8, wxSWISS, wxNORMAL,wxNORMAL, false, wxT("Tahoma")));
	//tc_name2->SetFocus();
	row_ += 25;
	tc_name3->SetValue ("240");
	row_ += 85+80-24+5+10-25-25;

	// botón OK
	button_ok = new wxButton(this, wxID_OK, wxT("OK"), wxPoint(col_+183,row_+24), wxSize(75,23), 0, wxDefaultValidator, wxT("button_ok"));
	button_ok->SetFont(wxFont(8, wxSWISS, wxNORMAL,wxNORMAL, false, wxT("Tahoma")));

	// botón Cancel
	button_cancel = new wxButton(this, wxID_CANCEL, wxT("Cancel"), wxPoint(col_+267,row_+24), wxSize(75,23), 0, wxDefaultValidator, wxT("button_cancel"));
	button_cancel->SetFont(wxFont(8, wxSWISS, wxNORMAL,wxNORMAL, false, wxT("Tahoma")));
}




