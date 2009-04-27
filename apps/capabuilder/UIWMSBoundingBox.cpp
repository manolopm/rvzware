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
#include "UIWMSBoundingBox.h"

#define ID_PREVIEW		9000
#define ID_PREVIEWURL	9001
#define ID_XMIN			9002
#define ID_YMIN			9003
#define ID_XMAX			9004
#define ID_YMAX			9005

BEGIN_EVENT_TABLE(UIWMSBoundingBox, wxDialog)
	EVT_BUTTON  (wxID_OK,       UIWMSBoundingBox::OnButtonOK)
	EVT_BUTTON  (wxID_CANCEL,   UIWMSBoundingBox::OnButtonCancel)
	EVT_BUTTON  (ID_PREVIEW,		UIWMSBoundingBox::OnButtonPreview)
	EVT_BUTTON  (ID_PREVIEWURL,		UIWMSBoundingBox::OnButtonPreviewURL)
	EVT_TEXT	(ID_XMIN,			UIWMSBoundingBox::CheckWidthHeight)
	EVT_TEXT	(ID_YMIN,			UIWMSBoundingBox::CheckWidthHeight)
	EVT_TEXT	(ID_XMAX,			UIWMSBoundingBox::CheckWidthHeight)
	EVT_TEXT	(ID_YMAX,			UIWMSBoundingBox::CheckWidthHeight)
	EVT_CLOSE	(					 UIWMSBoundingBox::OnClose)

	
END_EVENT_TABLE()

UIWMSBoundingBox::UIWMSBoundingBox(WMSBoundingBoxController *_wms_cont,wxWindow *parent, wxWindowID id, const wxString &title, const wxPoint& pos, const wxSize& size, long style)
				:wxDialog(parent, id, title, pos, size, wxDEFAULT_DIALOG_STYLE), wms_cont(_wms_cont)

{
	wxSize new_size(250, 300);
	SetSize( new_size );
	
	InitGUI();
}

void UIWMSBoundingBox::InitGUI()
{
	SetTitle(wxT("Wms Bounding Box"));
	SetSize(8,8,390,460);
	Center();


	top_lat_left_text = new wxStaticText(this, wxID_ANY, wxT("Bottom left longitude"), wxPoint(25, 25));
	top_lon_left_text = new wxStaticText(this, wxID_ANY, wxT("Bottom left lattitude"), wxPoint(25, 50));
	bottom_lat_right_text = new wxStaticText(this, wxID_ANY, wxT("Top right longitude"), wxPoint(25, 75));
	bottom_lon_right_text = new wxStaticText(this, wxID_ANY, wxT("Top right lattitude"), wxPoint(25, 100));
	
	top_lat_left_text_ctrl = new wxTextCtrl(this, ID_XMIN, wxT(""), wxPoint(140, 25), wxSize(120, 20));
	top_lon_left_text_ctrl = new wxTextCtrl(this, ID_YMIN, wxT(""), wxPoint(140, 50), wxSize(120, 20));
	bottom_lat_right_text_ctrl = new wxTextCtrl(this, ID_XMAX, wxT(""), wxPoint(140, 75), wxSize(120, 20));
	bottom_lon_right_text_ctrl = new wxTextCtrl(this, ID_YMAX, wxT(""), wxPoint(140, 100), wxSize(120, 20));

	width_text = new wxStaticText(this, wxID_ANY, wxT("Width:"), wxPoint(275, 25));
	height_text = new wxStaticText(this, wxID_ANY, wxT("Heigth:"), wxPoint(270, 75));
	width_text_ctrl = new wxTextCtrl(this, wxID_ANY, wxT("0"), wxPoint(310, 25), wxSize(50, 20));
	height_text_ctrl = new wxTextCtrl(this, wxID_ANY, wxT("0"), wxPoint(310, 75), wxSize(50, 20));
	width_text_ctrl->Enable(false);
	height_text_ctrl->Enable(false);

	button_preview = new wxButton(this, ID_PREVIEW, wxT("GetMap"), wxPoint(285, 125));

	url_text = new wxStaticText(this, wxID_ANY, wxT("URL"), wxPoint(25, 150));
	url = new wxTextCtrl(this, wxID_ANY, wxT(""), wxPoint(25, 170), wxSize(335, 200),wxTE_MULTILINE|wxTE_BESTWRAP);

	button_previewurl = new wxButton(this, ID_PREVIEWURL, wxT("GetMap URL"), wxPoint(285, 380));

	button_ok = new wxButton(this, wxID_OK, wxT("Ok"), wxPoint(210, 410));
	button_cancel = new wxButton(this, wxID_CANCEL, wxT("Cancel"), wxPoint(285, 410));
}

UIWMSBoundingBox::~UIWMSBoundingBox(void)
{
}


void UIWMSBoundingBox::OnButtonOK(wxCommandEvent& WXUNUSED(event))
{
	if (CheckCoords())
	{
		EndDialog(wxID_OK);
		wms_cont->ReleaseMouseCallBack();
		wms_cont->OK(std::string(url->GetValue()));
		
	}
	else
	{
		wxMessageDialog message(this,wxString("At least one coordinate is not valid"), wxString("Warning"), wxICON_EXCLAMATION |wxOK);
		message.ShowModal();
	}
}

void UIWMSBoundingBox::OnButtonCancel(wxCommandEvent& WXUNUSED(event))
{
	EndDialog(wxID_CANCEL);
	wms_cont->Cancel();
	wms_cont->ReleaseMouseCallBack();
}

void UIWMSBoundingBox::OnButtonPreview(wxCommandEvent& WXUNUSED(event))
{
	if (CheckCoords())
	{
		
		wms_cont->NewPetition();
	}
	else
	{
		wxMessageDialog message(this,wxString("At least one coordinate is not valid"), wxString("Warning"), wxICON_EXCLAMATION |wxOK);
		message.ShowModal();
	}

}

void UIWMSBoundingBox::OnButtonPreviewURL(wxCommandEvent& WXUNUSED(event))
{
	if (CheckCoords())
	{
		wms_cont->NewPetition(std::string(url->GetValue()));
	}
	else
	{
		wxMessageDialog message(this,wxString("At least one coordinate is not valid"), wxString("Warning"), wxICON_EXCLAMATION |wxOK);
		message.ShowModal();
	}

}

void UIWMSBoundingBox::OnClose(wxCloseEvent& /*event*/)
{
	EndDialog(wxID_CANCEL);
	wms_cont->Cancel();
	wms_cont->ReleaseMouseCallBack();
}

bool UIWMSBoundingBox::CheckCoords()
{
	double aux;
	wms_cont->SetCoordsOk(false);	

	if (top_lat_left_text_ctrl->GetValue().Trim().ToDouble(&aux))
		wms_cont->SetTopLeftLat(aux);
	else
		return false;

	if (top_lon_left_text_ctrl->GetValue().Trim().ToDouble(&aux))
		wms_cont->SetTopLeftLon(aux);
	else
		return false;

	if (bottom_lat_right_text_ctrl->GetValue().Trim().ToDouble(&aux))
		wms_cont->SetBotRightLat(aux);
	else
		return false;

	if (bottom_lon_right_text_ctrl->GetValue().Trim().ToDouble(&aux))
		wms_cont->SetBotRightLon(aux);
	else
		return false;

	wms_cont->SetCoordsOk(true);
	return true;
}

void UIWMSBoundingBox::CheckWidthHeight(wxCommandEvent& WXUNUSED(event))
{
	std::stringstream bbw, bbh;
	std::stringstream sxmin, symin, sxmax, symax;
	double xmin, ymin, xmax, ymax;

	sxmin << top_lat_left_text_ctrl->GetValue();
	sxmin >> xmin;
	symin << top_lon_left_text_ctrl->GetValue();
	symin >> ymin;
	sxmax << bottom_lat_right_text_ctrl->GetValue();
	sxmax >> xmax;
	symax << bottom_lon_right_text_ctrl->GetValue();
	symax >>ymax;

	bbw << (int)abs(xmin - xmax);
	bbh << (int)abs(ymin - ymax);

	width_text_ctrl->SetValue(bbw.str());
	height_text_ctrl->SetValue(bbh.str());
}
