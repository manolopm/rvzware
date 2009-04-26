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
#include <stdafx.h>

#include <gui/UIElementVectorial.h>

#include <sstream>
#include <string>
#include <cpw/debugger.h>

#include <UIApplicationMainFrame.h>
#include <cpw/ApplicationConfiguration.h>

#include <cpw/graphic/ApplicationScene.h>
#include <cpw/graphic/IScene.h>
#include <cpw/common/types.h>

using namespace cpw::gui;

#define IM_ROOT 0
#define IM_TRACKPOINT 1

#define BOX_HEIGHT 186-20
#define BOX_WIDTH 348

BEGIN_EVENT_TABLE(UIElementVectorial,wxDialog)  
	EVT_BUTTON    (ID_CANCEL_BUTTON,      UIElementVectorial::OnButtonCancel)
	EVT_BUTTON    (ID_FINISH_BUTTON,      UIElementVectorial::OnButtonFinish)
	EVT_BUTTON    (ID_REMOVE_BUTTON,    UIElementVectorial::OnButtonRemovePoint)

	EVT_CLOSE     (UIElementVectorial::OnClose)
	EVT_TEXT      (ID_EDIT_FLAMES, UIElementVectorial::CheckPointsValues)
	EVT_TREE_SEL_CHANGED (ID_POINTS_TREECTRL, UIElementVectorial::OnTreeSelectionChanged)
	EVT_PAINT			(UIElementVectorial::OnPaint )

	EVT_SPIN_UP   (ID_SPIN_POS_X,  UIElementVectorial::OnPositionXSpinUp)
	EVT_SPIN_UP   (ID_SPIN_POS_Y,  UIElementVectorial::OnPositionYSpinUp)
	EVT_SPIN_UP   (ID_SPIN_POS_Z,  UIElementVectorial::OnPositionZSpinUp)
	//EVT_SPIN_UP   (ID_SPIN_BORDER,  UIElementVectorial::OnBorderSpinUp)

	//EVT_SPIN_DOWN   (ID_SPIN_BORDER,  UIElementVectorial::OnBorderSpinDown)
	EVT_SPIN_DOWN   (ID_SPIN_POS_X,  UIElementVectorial::OnPositionXSpinDown)
	EVT_SPIN_DOWN   (ID_SPIN_POS_Y,  UIElementVectorial::OnPositionYSpinDown)
	EVT_SPIN_DOWN   (ID_SPIN_POS_Z,  UIElementVectorial::OnPositionZSpinDown)

	//EVT_COLOURPICKER_CHANGED(ID_COLOUR_CTRL, UIElementVectorial::OnColourPickerChange)

END_EVENT_TABLE()


UIElementVectorial::UIElementVectorial(cpw::controllers::ElementVectorialController *evc, wxWindow *parent, wxWindowID id, const wxString &title, const wxPoint &position, const wxSize& size, long style)
: wxDialog(parent, id, title, position, size, style), paren(parent), ElementVectorial_controller(evc), update_on_changes(true), modify(false), changes(false),
vname("ElementVectorial"),vicon("elementvectorial.png"),vicon_child("elementvectorial_point.png")
{
	if (!(cpw::ApplicationConfiguration::GetInstance()->IsThemed()))
	{
		SetFont(paren->GetFont());
		SetForegroundColour(paren->GetForegroundColour());
		SetBackgroundColour(paren->GetBackgroundColour());
	}	
	
	CreateGUIControls();
}

UIElementVectorial::~UIElementVectorial()
{
} 

void UIElementVectorial::CreateGUIControls()
{
  SetTitle(wxString(vname.c_str(),wxConvUTF8));
	SetIcon(wxNullIcon);
	SetSize(8,8,388,422-60);
	Center();

	posx = posy = posz = 0.0;

	polygon_flag = true;

	int _col = 465;
	int _row = 195;

	std::string &icon_path = ApplicationConfiguration::GetInstance()->GetUIIconDirectory();

	ElementVectorial_name_Ed = new wxTextCtrl(this, ID_ELEMENTVECTORIALNAME_EDIT, wxT("ElementVectorial"), wxPoint(104,10), wxSize(232,21), 0, wxDefaultValidator);
	ElementVectorial_name_Ed->SetFont(wxFont(8, wxSWISS, wxNORMAL,wxNORMAL, false, wxT("Tahoma")));

	border_Edit = new wxTextCtrl(this, ID_EDIT_BORDER, wxT("1"), wxPoint(104,40), wxSize(65,21), 0, wxDefaultValidator, wxT("border_Edit"));
	border_Edit->SetFont(wxFont(8, wxSWISS, wxNORMAL,wxNORMAL, false, wxT("Tahoma")));
	border_Edit->Show(false);
	//wxSpinButton *border_spin = new wxSpinButton(this, ID_SPIN_BORDER, wxPoint(104+66,40), wxSize(10,21), wxSP_VERTICAL);


	colour_ctrl = new wxColourPickerCtrl(this, ID_COLOUR_CTRL, (*wxWHITE), wxPoint(260,40), wxSize(75,22), wxCLRP_SHOW_LABEL | wxCLRP_DEFAULT_STYLE);
	colour_ctrl->Disable();
	colour_ctrl->Show(false);

Remove_Button = new wxBitmapButton(this, ID_REMOVE_BUTTON, wxBitmap(wxImage(wxString((icon_path + "delete.png").c_str(),wxConvUTF8))), wxPoint(20,95-60), wxSize(22,22), wxBU_AUTODRAW, wxDefaultValidator, wxT("Remove_Button"));

	_col = 79-30-15;	_row = 353+25-45-20-60;

	Position_x_Edit = new wxTextCtrl(this, ID_EDIT_FLAMES, wxT(""), wxPoint(_col,_row), wxSize(52+15+25,19), 0, wxDefaultValidator, wxT("Position_x_Edit"));
	Position_x_Edit->SetFont(wxFont(8, wxSWISS, wxNORMAL,wxNORMAL, false, wxT("Tahoma")));
	wxSpinButton *position_x_spin = new wxSpinButton(this, ID_SPIN_POS_X, wxPoint(_col+53+15+25,_row), wxSize(10,19), wxSP_VERTICAL);

	Position_x_StaticText = new wxStaticText(this, ID_Position_X_STATICTEXT, wxT("X"), wxPoint(_col-10,_row), wxDefaultSize, 0, wxT("Position_x_StaticText"));
	Position_x_StaticText->SetFont(wxFont(8, wxSWISS, wxNORMAL,wxNORMAL, false, wxT("Tahoma")));

	Position_y_Edit = new wxTextCtrl(this, ID_EDIT_FLAMES, wxT(""), wxPoint(_col+92+10+15-3,_row), wxSize(52+15+25,19), 0, wxDefaultValidator, wxT("Position_y_Edit"));
	Position_y_Edit->SetFont(wxFont(8, wxSWISS, wxNORMAL,wxNORMAL, false, wxT("Tahoma")));
	wxSpinButton *position_y_spin = new wxSpinButton(this, ID_SPIN_POS_Y, wxPoint(_col+53+92+15+25+10+15-3,_row), wxSize(10,19), wxSP_VERTICAL);

	Position_y_StaticText = new wxStaticText(this, ID_POSITIONY_STATICTEXT, wxT("Y"), wxPoint(_col-10+92+10+15,_row), wxDefaultSize, 0, wxT("Position_y_StaticText"));
	Position_y_StaticText->SetFont(wxFont(8, wxSWISS, wxNORMAL,wxNORMAL, false, wxT("Tahoma")));

	Position_z_Edit = new wxTextCtrl(this, ID_EDIT_FLAMES, wxT(""), wxPoint(_col+184+20+15+10-2,_row), wxSize(52+15+25,19), 0, wxDefaultValidator, wxT("Position_z_Edit"));
	Position_z_Edit->SetFont(wxFont(8, wxSWISS, wxNORMAL,wxNORMAL, false, wxT("Tahoma")));
	Position_z_Edit->SetEditable(false);
	Position_z_Edit->Enable(false);

	wxSpinButton *position_z_spin = new wxSpinButton(this, ID_SPIN_POS_Z, wxPoint(_col+53+184+15+25+20+15+10-2,_row), wxSize(10,19), wxSP_VERTICAL);
	position_z_spin->Enable(false);

	Position_z_StaticText = new wxStaticText(this, ID_POSITIONZ_STATICTEXT, wxT("Z"), wxPoint(_col-10+184+20+15+10,_row), wxDefaultSize, 0, wxT("Position_z_StaticText"));
	Position_z_StaticText->SetFont(wxFont(8, wxSWISS, wxNORMAL,wxNORMAL, false, wxT("Tahoma")));

	//Position_StaticBox = new wxStaticBox(this, ID_POSITION_STATICBOX, wxT("Position"), wxPoint(_col-30+15,_row-22), wxSize(350,50));
	//Position_StaticBox->SetFont(wxFont(8, wxSWISS, wxNORMAL,wxNORMAL, false, wxT("Tahoma")));

	_col = 20;	_row = 140+25-45-60;

	img_list = new wxImageList(16, 16);
	img_list->Add(wxBitmap(wxImage(wxString((icon_path + vicon).c_str(),wxConvUTF8))));
	img_list->Add(wxBitmap(wxImage(wxString((icon_path + vicon_child).c_str(),wxConvUTF8))));

	Points_TreeCtrl = new wxTreeCtrl(this, ID_POINTS_TREECTRL, wxPoint(_col, _row), wxSize(BOX_WIDTH, BOX_HEIGHT), wxTR_HAS_BUTTONS | wxTR_SINGLE);
	Points_TreeCtrl->SetFont(wxFont(8, wxSWISS, wxNORMAL,wxNORMAL, false, wxT("Tahoma")));
	Points_TreeCtrl->AssignImageList(img_list);
	Points_TreeCtrl->AddRoot(_T("Element Points"),IM_ROOT);
	Points_TreeCtrl->ExpandAll();
	if (!(cpw::ApplicationConfiguration::GetInstance()->IsThemed()))
	{
	  Points_TreeCtrl->SetBackgroundColour(wxColour(wxString(cpw::ApplicationConfiguration::GetInstance()->GetPageColour().c_str(),wxConvUTF8)));
	  Points_TreeCtrl->SetForegroundColour(wxColour(wxString(cpw::ApplicationConfiguration::GetInstance()->GetPageFontColour().c_str(),wxConvUTF8)));
	}

	_col = 201;	_row = 326+70+25-45-20-60;
	Finish_button = new wxButton(this, ID_FINISH_BUTTON, wxT("Finish"), wxPoint(_col,_row), wxSize(75,23), 0, wxDefaultValidator, wxT("Finish_button"));
	Finish_button->SetFont(wxFont(8, wxSWISS, wxNORMAL,wxNORMAL, false, wxT("Tahoma")));

	Cancel_button = new wxButton(this, ID_CANCEL_BUTTON, wxT("Cancel"), wxPoint(_col+84,_row), wxSize(75,23), 0, wxDefaultValidator, wxT("Cancel_button"));
	Cancel_button->SetFont(wxFont(8, wxSWISS, wxNORMAL,wxNORMAL, false, wxT("Tahoma")));

}


void UIElementVectorial::OnClose(wxCloseEvent& /*event*/)
{
	//if (!modify) 
	//	ElementVectorial_controller->Cancel();
	//else
	//	ElementVectorial_controller->CancelModifications();

	//this->EndDialog(wxID_CANCEL);	
	//ElementVectorial_controller->ReleaseMouseCallBack();
	Exit();
}

void UIElementVectorial::OnButtonCancel(wxCommandEvent& WXUNUSED(event))
{
	Exit();
}	


void UIElementVectorial::Exit()
{
	if(changes)
	{
		wxMessageDialog message(this,wxT("Save changes before quit?"), wxT("Warning"),wxICON_EXCLAMATION |wxYES_NO |wxCANCEL);
		int modal = message.ShowModal();
		if(modal == wxID_YES)
		{		
			ButtonOK();
		}
		if(modal == wxID_NO )
		{
			if (!modify) 
				ElementVectorial_controller->Cancel();
			else
				ElementVectorial_controller->CancelModifications();

			this->EndDialog(wxID_CANCEL);	
			ElementVectorial_controller->ReleaseMouseCallBack();

		}
	}
	else
	{
		if (!modify) 
			ElementVectorial_controller->Cancel();
		else
			ElementVectorial_controller->CancelModifications();

		this->EndDialog(wxID_CANCEL);	
		ElementVectorial_controller->ReleaseMouseCallBack();

	}
}

void UIElementVectorial::OnButtonFinish(wxCommandEvent& WXUNUSED(event))
{
	ButtonOK();
}

void UIElementVectorial::ButtonOK()
{
  std::string p = std::string(ElementVectorial_name_Ed->GetValue().mb_str());
	if((ElementVectorial_name_Ed->IsEmpty()) || (p.at(0) == ' ' ) )
	{
		wxMessageDialog message(this,wxT("The element needs a name."), wxT("Warning"),wxICON_EXCLAMATION |wxOK);
		message.ShowModal();
	}
	else
	{	
		EndDialog(wxID_OK);
		int width;
		std::stringstream aux (std::string(border_Edit->GetValue().mb_str()));
		aux >> width;

		std::string color;
		color = std::string(colour_ctrl->GetColour().GetAsString(wxC2S_HTML_SYNTAX).mb_str());

		ElementVectorial_controller->CreatePermanentElementVectorial(std::string(ElementVectorial_name_Ed->GetValue().mb_str()), color, width, modify);

		ElementVectorial_controller->ReleaseMouseCallBack();
	}
}

void UIElementVectorial::CheckPointsValues(wxCommandEvent& WXUNUSED(event))
{
	changes = true;
	double aux;

	long int insertion_point_posx, insertion_point_posy, insertion_point_posz;

	insertion_point_posx = Position_x_Edit->GetInsertionPoint();
	insertion_point_posy = Position_y_Edit->GetInsertionPoint();
	insertion_point_posz = Position_z_Edit->GetInsertionPoint();

	if (Position_x_Edit->GetValue().Trim().ToDouble(&aux))
		posx = aux;
	{
		std::stringstream aux2;
		aux2 << std::fixed << std::setprecision (5) << posx;
		Position_x_Edit->ChangeValue(wxString(aux2.str().c_str(),wxConvUTF8));
	}

	if (Position_y_Edit->GetValue().Trim().ToDouble(&aux))
		posy = aux;
	{
		std::stringstream aux2;
		aux2 << std::fixed << std::setprecision (5) << posy;
		Position_y_Edit->ChangeValue(wxString(aux2.str().c_str(),wxConvUTF8));
	}

	if (Position_z_Edit->GetValue().Trim().ToDouble(&aux))
		posz = aux;
	{
		std::stringstream aux2;
		aux2 << std::fixed << std::setprecision (5) << posz;
		Position_z_Edit->ChangeValue(wxString(aux2.str().c_str(),wxConvUTF8));
	}

	//if (!update_on_changes)
	//return;
	if (update_on_changes)
	{
		wxTreeItemId root = Points_TreeCtrl->GetRootItem();
		wxTreeItemId item_selected = Points_TreeCtrl->GetSelection();
		if ((item_selected.IsOk()) && (item_selected != Points_TreeCtrl->GetRootItem()))
		{		
			int count = Points_TreeCtrl->GetChildrenCount(root);
			int i =0;
			wxTreeItemIdValue cookie;
			wxTreeItemId child = Points_TreeCtrl->GetFirstChild(root,cookie); 
			while((i<count)&&(child != item_selected))
			{
				child = Points_TreeCtrl->GetNextChild(root,cookie);
				i++;
			}
			if(child == item_selected)
				ElementVectorial_controller->UpdateUIPoint(i,false);	

		}
	}

	Position_x_Edit->SetInsertionPoint(insertion_point_posx);
	Position_y_Edit->SetInsertionPoint(insertion_point_posy);
	Position_z_Edit->SetInsertionPoint(insertion_point_posz);
}

bool UIElementVectorial::GetCoordsFromEditControls(cpw::Point3d<float> &pos)
{
	double aux;

	if (!Position_x_Edit->GetValue().Trim().ToDouble(&aux))
		return false;

	pos.x = (float)aux;

	if (!Position_y_Edit->GetValue().Trim().ToDouble(&aux))
		return false;

	pos.y = (float)aux;

	if (!Position_z_Edit->GetValue().Trim().ToDouble(&aux))
		return false;

	pos.z = (float)aux;

	return true;
}



void UIElementVectorial::OnButtonRemovePoint(wxCommandEvent& WXUNUSED(event))
{
	wxTreeItemId root = Points_TreeCtrl->GetRootItem();
	wxTreeItemId item_selected = Points_TreeCtrl->GetSelection();
	if ((item_selected.IsOk()) && (item_selected != Points_TreeCtrl->GetRootItem()))
	{
		changes = true;
		int count = Points_TreeCtrl->GetChildrenCount(root);
		int i =0;
		wxTreeItemIdValue cookie;
		wxTreeItemId child = Points_TreeCtrl->GetFirstChild(root,cookie); 
		while((i<count)&&(child != item_selected))
		{
			child = Points_TreeCtrl->GetNextChild(root,cookie);
			i++;
		}
		if(child == item_selected)
			ElementVectorial_controller->Remove(i);	
	}
}

void UIElementVectorial::AddPoint()
{
	changes = true;
	wxTreeItemId root = Points_TreeCtrl->GetRootItem();
	Points_TreeCtrl->AppendItem(root, _T("Point"), IM_TRACKPOINT);
	Points_TreeCtrl->ExpandAll();

	wxTreeItemId child = Points_TreeCtrl->GetLastChild(root);
	if(child.IsOk())
		Points_TreeCtrl->SelectItem(child);
}


void UIElementVectorial::RemovePoint()
{
	wxTreeItemId root = Points_TreeCtrl->GetRootItem();

	if(Points_TreeCtrl->GetChildrenCount(root) != 0)
	{
		wxTreeItemIdValue cookie;
		wxTreeItemId child = Points_TreeCtrl->GetFirstChild(root,cookie);		
		Points_TreeCtrl->Delete(child);

		child = Points_TreeCtrl->GetFirstChild(root,cookie);
		if(child.IsOk())
			Points_TreeCtrl->SelectItem(child);
		else
			Points_TreeCtrl->SelectItem(root);

		changes = true;

	}
}


void UIElementVectorial::FillUI(const int &nump, const std::string &color, const int &width)
{
	update_on_changes = false;
	std::stringstream aux;
	aux << width;
	border_Edit->SetValue(wxString(aux.str().c_str(),wxConvUTF8));

	colour_ctrl->SetColour(wxColour(wxString(color.c_str(),wxConvUTF8)));
	Points_TreeCtrl->DeleteAllItems();
	Points_TreeCtrl->AddRoot(_T("Element Points"),IM_ROOT);
	wxTreeItemId root = Points_TreeCtrl->GetRootItem();
	for(int i=0; i<nump; i++)
	{
		Points_TreeCtrl->AppendItem(root, _T("Point"), IM_TRACKPOINT);
	}
	Points_TreeCtrl->SelectItem(root);
	Points_TreeCtrl->ExpandAll();
	update_on_changes = true;
}

void UIElementVectorial::OnTreeSelectionChanged(wxTreeEvent& wxevent)
{
	if(!update_on_changes) return;

	wxTreeItemId root = Points_TreeCtrl->GetRootItem();
	wxTreeItemId item_selected = Points_TreeCtrl->GetSelection();
	if ((item_selected.IsOk()) && (item_selected != Points_TreeCtrl->GetRootItem()))
	{
		 Position_x_Edit->Enable(true);
		 Position_y_Edit->Enable(true);
		 
		int count = Points_TreeCtrl->GetChildrenCount(root);
		int i =0;
		wxTreeItemIdValue cookie;
		wxTreeItemId child = Points_TreeCtrl->GetFirstChild(root,cookie); 
		while((i<count)&&(child != item_selected))
		{
			child = Points_TreeCtrl->GetNextChild(root,cookie);
			i++;
		}
		if(child == item_selected)
			ElementVectorial_controller->UpdateUIPoint(i,true);	
	}
	else
	{
		if (item_selected.IsOk())
		{
			 Position_x_Edit->Clear();
			 Position_y_Edit->Clear();
			 Position_z_Edit->Clear();

			 Position_x_Edit->Enable(false);
			 Position_y_Edit->Enable(false);
			 
		}
	}
}

void UIElementVectorial::OnPaint(wxPaintEvent& event)
{
	wxPaintDC dc(this);
	render(dc);
}


void UIElementVectorial::render(wxDC& dc)
{
	int client_h = 0;
	int client_w = 0;
	GetClientSize (&client_w, &client_h);

	if (!(cpw::ApplicationConfiguration::GetInstance()->IsThemed()))
	{
	  wxColour c_pen   = wxString(cpw::ApplicationConfiguration::GetInstance()->GetBackgroundGradient2Colour().c_str(),wxConvUTF8);
	  wxColour c_backg = wxString(cpw::ApplicationConfiguration::GetInstance()->GetBackgroundGradient1Colour().c_str(),wxConvUTF8);	
	  wxColour c_brush = wxString(cpw::ApplicationConfiguration::GetInstance()->GetBackgroundColour().c_str(),wxConvUTF8);
	  dc.SetTextForeground(wxColour(wxString(cpw::ApplicationConfiguration::GetInstance()->GetFontLightColour().c_str(),wxConvUTF8)));
		dc.SetPen(wxPen(c_pen));
		dc.SetBrush(wxBrush(c_brush));
		dc.GradientFillLinear( wxRect(0,0,client_w,client_h), c_backg, c_pen, wxSOUTH);
	}

	DrawStaticBox(dc,_T("Position"), wxPoint(20,294-60), wxSize(350,50));
	wxPoint box1_first(20-2,10-2);
	wxPoint box1_last(368,82+2);
	dc.DrawRotatedText(_T("Name:"),20,10, 0);
	//dc.DrawRotatedText(_T("Border thickness"),20,40, 0);
	//dc.DrawRotatedText(_T("Border colour"),188,40, 0);
}
void UIElementVectorial::DrawStaticBox(wxDC& dc,const wxString& label, const wxPoint& pos, const wxSize& size)
{
	wxColour c_pen = dc.GetPen().GetColour();
	dc.SetPen(wxPen(dc.GetTextForeground()));
	dc.DrawRoundedRectangle(pos.x, pos.y, size.x, size.y,3.1f);
	dc.SetPen(wxPen(c_pen));
	dc.DrawRotatedText(label,pos.x+10,pos.y,0);
}

void UIElementVectorial::OnBorderSpinUp(wxSpinEvent& event)
{
	std::ostringstream wop; 
	double f; 
	border_Edit->GetValue().ToDouble(&f);
	f+=1.0f;
	wop << std::fixed << std::setprecision(5) << f;  
	border_Edit->SetValue(wxString(wop.str().c_str(),wxConvUTF8)); 
	changes = true;
}

void UIElementVectorial::OnBorderSpinDown(wxSpinEvent& event)
{
	std::ostringstream wop; 
	double f; 
	border_Edit->GetValue().ToDouble(&f);
	f-=1.0f;
	wop << std::fixed << std::setprecision(5) << f;  
	border_Edit->SetValue(wxString(wop.str().c_str(),wxConvUTF8)); 
	changes = true;
}

void UIElementVectorial::OnPositionXSpinUp(wxSpinEvent& event)
{
	std::ostringstream wop; 
	wop.precision(15);
	double f; 
	Position_x_Edit->GetValue().ToDouble(&f);
	f+=10.0f;
	wop << std::fixed << std::setprecision(5) << f;  
	Position_x_Edit->SetValue(wxString(wop.str().c_str(),wxConvUTF8)); 
	changes = true;
}
void UIElementVectorial::OnPositionYSpinUp(wxSpinEvent& event)
{
	std::ostringstream wop; 
	wop.precision(15);
	double f; 
	Position_y_Edit->GetValue().ToDouble(&f);
	f+=10.0f;
	wop << std::fixed << std::setprecision(5) << f;  
	Position_y_Edit->SetValue(wxString(wop.str().c_str(),wxConvUTF8)); 
	changes = true;
}

void UIElementVectorial::OnPositionZSpinUp(wxSpinEvent& event)
{
	std::ostringstream wop; 
	wop.precision(15);
	double f; 
	Position_z_Edit->GetValue().ToDouble(&f);
	f+=10.0f;
	wop << std::fixed << std::setprecision(5) << f;  
	Position_z_Edit->SetValue(wxString(wop.str().c_str(),wxConvUTF8)); 
}

void UIElementVectorial::OnPositionXSpinDown(wxSpinEvent& event)
{
	std::ostringstream wop; 
	wop.precision(15);
	double f; 
	Position_x_Edit->GetValue().ToDouble(&f);
	f-=10.0f;
	wop << std::fixed << std::setprecision(5) << f;  
	Position_x_Edit->SetValue(wxString(wop.str().c_str(),wxConvUTF8)); 
	changes = true;
}

void UIElementVectorial::OnPositionYSpinDown(wxSpinEvent& event)
{
	std::ostringstream wop; 
	wop.precision(15);
	double f; 
	Position_y_Edit->GetValue().ToDouble(&f);
	f-=10.0f;
	wop << std::fixed << std::setprecision(5) << f;  
	Position_y_Edit->SetValue(wxString(wop.str().c_str(),wxConvUTF8)); 
	changes = true;
}

void UIElementVectorial::OnPositionZSpinDown(wxSpinEvent& event)
{
	std::ostringstream wop; 
	wop.precision(15);
	double f; 
	Position_z_Edit->GetValue().ToDouble(&f);
	f-=10.0f;
	wop << std::fixed << std::setprecision(5) << f;  
	Position_z_Edit->SetValue(wxString(wop.str().c_str(),wxConvUTF8)); 
}

void UIElementVectorial::OnColourPickerChange(wxColourPickerEvent& event)
{
	wxColour col = colour_ctrl->GetColour();
	int r = col.Red();
	int g = col.Green();
	int b = col.Blue();
	ElementVectorial_controller->SetRGBColour(r,g,b);
}

void UIElementVectorial::OnEraseBackground(wxEraseEvent& event)
{
	//don't remove this method
}


void UIElementVectorial::SetElementVectorialIcon(std::string &parenticon, std::string &childicon)  
{ 
	if((vicon == "") || (vicon_child == "")) return;

	vicon = parenticon; 
	vicon_child = childicon; 
	std::string &icon_path = ApplicationConfiguration::GetInstance()->GetUIIconDirectory();
	img_list->RemoveAll();
	img_list->Add(wxBitmap(wxImage(wxString((icon_path + parenticon).c_str(),wxConvUTF8))));
	img_list->Add(wxBitmap(wxImage(wxString((icon_path + childicon).c_str(),wxConvUTF8))));
}
