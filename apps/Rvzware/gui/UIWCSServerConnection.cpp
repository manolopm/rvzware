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


#include <stdafx.h>

#include <gui/UIWCSServerConnection.h>

#include <UIApplicationMainFrame.h>


enum
{
	ID_SRS_CHOICE = 1011,
	ID_FORMAT_CHOICE = 1010,
	ID_LAYERTREE_TREECTRL = 1009,
	ID_LAYERNAME_EDIT = 1008,
	ID_WXSTATICTEXT2 = 1007,
	ID_DESCRIPTION_MEMO = 1006,
	ID_WXSTATICTEXT1 = 1005,
	ID_COMBOBOXSERVER = 1004,
	ID_CONNECTBUTTON = 1003,
	ID_CANCELBUTTON = 1002,
	ID_NEXTBUTTON = 1001,
	ID_DUMMY_VALUE_ //don't remove this value unless you have other enum values
};


using cpw::gui::UIWCSServerConnection;


BEGIN_EVENT_TABLE(UIWCSServerConnection,wxDialog)	
	EVT_MOVE    (					 UIWCSServerConnection::OnMove)
	EVT_CLOSE	(					 UIWCSServerConnection::OnClose)
	EVT_BUTTON  (ID_CANCELBUTTON,    UIWCSServerConnection::OnButtonCancel)
	EVT_BUTTON  (ID_NEXTBUTTON,      UIWCSServerConnection::OnButtonNext)
	EVT_BUTTON  (ID_CONNECTBUTTON,   UIWCSServerConnection::OnButtonConnect)
	EVT_PAINT	(					 UIWCSServerConnection::OnPaint )
	EVT_TREE_SEL_CHANGED(ID_LAYERTREE_TREECTRL, UIWCSServerConnection::EvtTreeItemSelChanged)
	EVT_COMBOBOX   (ID_COMBOBOXSERVER, UIWCSServerConnection::OnConnect)
	EVT_TEXT_ENTER (ID_COMBOBOXSERVER, UIWCSServerConnection::OnConnect)
END_EVENT_TABLE()


UIWCSServerConnection::UIWCSServerConnection(cpw::controllers::WCSServerConnectionController* scc, wxWindow *parent, wxWindowID id, const wxString &title, const wxPoint &position, const wxSize& size, long style)
: wxDialog(parent, id, title, position, size, style), server_connection_controller(scc), paren(parent), connection_state(-1), modify(false), changes(false), view_srs(false)
{
	CreateGUIControls();
}

UIWCSServerConnection::~UIWCSServerConnection()
{
} 

void UIWCSServerConnection::CreateGUIControls()
{
	SetTitle(wxT("Server Connection"));
	SetIcon(wxNullIcon);
	SetSize(8,8,390,720-150-25+8);
	Center();
	int _col, _row;

	_col = 0;
	_row = 0;

	Layer_name_Ed = new wxTextCtrl(this, ID_LAYERNAME_EDIT, wxT("WCSLayer"), wxPoint(_col+85,_row+15), wxSize(276,23), 0, wxDefaultValidator, wxT("Layer_name_Ed"));
	Layer_name_Ed->SetFont(wxFont(8, wxSWISS, wxNORMAL,wxNORMAL, false, wxT("Tahoma")));

	wxArrayString arrayStringFor_server_combo_box;
	server_combo_box = new wxComboBox(this, ID_COMBOBOXSERVER, wxT(""), wxPoint(_col+85,_row+40), wxSize(276,23), arrayStringFor_server_combo_box, wxTE_PROCESS_ENTER, wxDefaultValidator, wxT("server_combo_box"));
	server_combo_box->SetFont(wxFont(8, wxSWISS, wxNORMAL,wxNORMAL, false, wxT("Tahoma")));

	_row -= 25-8;
	
	description_memo = new wxTextCtrl(this, ID_DESCRIPTION_MEMO, wxT(""), wxPoint(_col+20,_row+68+25+5), wxSize(340,150), wxTE_MULTILINE | wxTE_READONLY, wxDefaultValidator, wxT("description_memo"));
	description_memo->SetMaxLength(0);
	description_memo->AppendText(wxT(""));
	description_memo->SetFocus();
	description_memo->SetInsertionPointEnd();
	description_memo->SetFont(wxFont(8, wxSWISS, wxNORMAL,wxNORMAL, false, wxT("Tahoma")));
	if (!(cpw::ApplicationConfiguration::GetInstance()->IsThemed()))
	{
	  description_memo->SetBackgroundColour(wxColour(wxString(cpw::ApplicationConfiguration::GetInstance()->GetPageColour().c_str(),wxConvUTF8)));
	  description_memo->SetForegroundColour(wxColour(wxString(cpw::ApplicationConfiguration::GetInstance()->GetPageFontColour().c_str(),wxConvUTF8)));
	}

	_col += 0;
	_row += 248+25;

	//layer tree-----
	Layer_tree_TreeCtrl = new wxTreeCtrl(this, ID_LAYERTREE_TREECTRL, wxPoint(_col+20,_row/*+68*/), wxSize(340,188)/*, wxTR_HAS_BUTTONS*/);
	Layer_tree_TreeCtrl->SetFont(wxFont(8, wxSWISS, wxNORMAL,wxNORMAL, false, wxT("Tahoma")));
	img_list = new wxImageList(16, 16);
	Layer_tree_TreeCtrl->AssignImageList(img_list);
	if (!(cpw::ApplicationConfiguration::GetInstance()->IsThemed()))
	{
	  Layer_tree_TreeCtrl->SetBackgroundColour(wxColour(wxString(cpw::ApplicationConfiguration::GetInstance()->GetPageColour().c_str(),wxConvUTF8)));
	  Layer_tree_TreeCtrl->SetForegroundColour(wxColour(wxString(cpw::ApplicationConfiguration::GetInstance()->GetPageFontColour().c_str(),wxConvUTF8)));
	}
	
	//choice-----
	wxArrayString arrayStringFor_formatChoice;
	formatChoice = new wxChoice(this, ID_FORMAT_CHOICE, wxPoint(_col+85,_row+195), wxSize(276,23), arrayStringFor_formatChoice, 0, wxDefaultValidator, wxT("WxChoice1"));
	formatChoice->SetFont(wxFont(8, wxSWISS, wxNORMAL,wxNORMAL, false, wxT("Tahoma")));
	formatChoice->SetSelection(-1);
	//formatChoice->Show(false);

	_row = 360-150-25+8;

	//choice-----
	wxArrayString arrayStringFor_srsChoice;
	srsChoice = new wxChoice(this, ID_SRS_CHOICE, wxPoint(_col+85,_row+294), wxSize(100,23), arrayStringFor_formatChoice, 0, wxDefaultValidator, wxT("WxChoice2"));
	srsChoice->SetFont(wxFont(8, wxSWISS, wxNORMAL,wxNORMAL, false, wxT("Tahoma")));
	srsChoice->SetSelection(-1);
	srsChoice->Show(false);


	cancel_button = new wxButton(this, ID_CANCELBUTTON, wxT("Cancel"), wxPoint(287,_row+294), wxSize(75,23), 0, wxDefaultValidator, wxT("cancel_button"));
	cancel_button->SetFont(wxFont(8, wxSWISS, wxNORMAL,wxNORMAL, false, wxT("Tahoma")));

	next_button = new wxButton(this, ID_NEXTBUTTON, wxT("Finish"), wxPoint(203,_row+294), wxSize(75,23), 0, wxDefaultValidator, wxT("next_button"));
	next_button->SetFont(wxFont(8, wxSWISS, wxNORMAL,wxNORMAL, false, wxT("Tahoma")));
}

void UIWCSServerConnection::OnClose(wxCloseEvent& /*event*/)
{
	//Destroy();
	Close();
}


void UIWCSServerConnection::OnButtonCancel(wxCommandEvent& WXUNUSED(event))
{
	Close();
}

void UIWCSServerConnection::Close()
{
	if(changes)
	{
		wxMessageDialog message(this,wxT("Save changes before quit?"), wxT("Warning"),wxICON_EXCLAMATION |wxYES_NO |wxCANCEL);
		int modal = message.ShowModal();
		if(modal == wxID_YES)
		{		
		  //ButtonOK();
		  wxCommandEvent tmp = wxCommandEvent();
		  OnButtonNext((wxCommandEvent &)tmp);
		}
		if(modal == wxID_NO )
		{
			this->EndModal(wxID_CANCEL);		
		}
	}
	else
	{
		this->EndModal(wxID_CANCEL);		
	}

}

void UIWCSServerConnection::OnButtonNext(wxCommandEvent& WXUNUSED(event))
{
	if(connection_state == 0)
	{
	  std::string p = std::string(Layer_name_Ed->GetValue().mb_str());
		if((Layer_name_Ed->IsEmpty()) || (p.at(0) == ' ' ) || (formatChoice->GetSelection() == wxNOT_FOUND) /*WxListBox1->IsEmpty()*/ )
		{
			wxMessageDialog message(this,wxT("The layer needs a name and image format."), wxT("Warning"),wxICON_EXCLAMATION |wxOK);
			message.ShowModal();
		}
		else
		{	
			EndModal(wxID_OK);
		}
	}
	else
	{
		wxMessageDialog message(this,wxT("You need to connect to a server."), wxT("Warning"),wxICON_EXCLAMATION |wxCANCEL);
		int modal = message.ShowModal();
	}

}


void UIWCSServerConnection::SetConnectionMemo(std::string &server_url, std::string &abstr, int &c_state)
{
  server_combo_box->SetValue(wxString(server_url.c_str(),wxConvUTF8));
	connection_state = c_state;
	description_memo->SetValue(wxString(abstr.c_str(),wxConvUTF8));
}

void UIWCSServerConnection::OnButtonConnect(wxCommandEvent& WXUNUSED(event))
{
  std::string server_url= (std::string)server_combo_box->GetStringSelection().mb_str();
	if (server_url == "")
	  server_url = (std::string)server_combo_box->GetValue().mb_str();
	if (server_url != "")
	{
		std::string abstr;
		connection_state = server_connection_controller->Connect(server_url, &abstr);
		description_memo->SetValue(wxString(abstr.c_str(),wxConvUTF8));
		server_connection_controller->FillLayerTree();

		if ((connection_state==0) && (server_combo_box->GetSelection()==wxNOT_FOUND))
		{
			wxArrayString server_array;
			server_array = server_combo_box->GetStrings();
			server_array.Insert(server_combo_box->GetValue(),server_array.size());
			server_combo_box->Clear();
			server_combo_box->Append(server_array);
			server_combo_box->SetSelection(server_array.size()-1);
		}
	}
}

void UIWCSServerConnection::OnConnect(wxCommandEvent& WXUNUSED(event))
{
  std::string server_url= (std::string)server_combo_box->GetStringSelection().mb_str();
	if (server_url == "")
	  server_url = (std::string)server_combo_box->GetValue().mb_str();
	if (server_url != "")
	{
		std::string abstr;
		connection_state = server_connection_controller->Connect(server_url, &abstr);
		description_memo->SetValue(wxString(abstr.c_str(),wxConvUTF8));
		server_connection_controller->FillLayerTree();
		changes = true;
		if ((connection_state==0) && (server_combo_box->GetSelection()==wxNOT_FOUND))
		{			
			wxArrayString server_array;
			server_array = server_combo_box->GetStrings();
			server_array.Insert(server_combo_box->GetValue(),server_array.size());
			server_combo_box->Clear();
			server_combo_box->Append(server_array);
			server_combo_box->SetSelection(server_array.size()-1);
		}
		formatChoice->Clear();
		srsChoice->Clear();

	}
}

void UIWCSServerConnection::SetServerList(std::vector<std::string> &vector_server)
{
	wxArrayString server_array;
	std::vector<std::string>::iterator i = vector_server.begin();
	for(;i!=vector_server.end();i++)
	{
		if(*i != "")
		  server_array.Insert(wxString((*i).c_str(),wxConvUTF8),server_array.size());		
	}
	server_combo_box->Clear();
	server_combo_box->Append(server_array);
}

std::vector<std::string> UIWCSServerConnection::GetServerList()
{
	std::vector<std::string> servers;
	wxArrayString server_array;
	server_array = server_combo_box->GetStrings();
	int s_count = server_array.GetCount();
	for(int i=0; i<s_count; i++)
	{	
	  if(std::string(server_array[i].mb_str())!="")
	    servers.push_back(std::string(server_array[i].mb_str()));
	}
	return servers;
}

void UIWCSServerConnection::OnMove(wxMoveEvent& event)
{
}

void UIWCSServerConnection::OnPaint(wxPaintEvent& event)
{
	wxPaintDC dc(this);
	render(dc);
}


void UIWCSServerConnection::render(wxDC& dc)
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

	dc.DrawRotatedText(_T("Description:"),20,52+25+5-25+8, 0);
	dc.DrawRotatedText(_T("Layer name:"),20,15, 0);
	dc.DrawRotatedText(_T("Wcs server:"),20,40, 0);
	dc.DrawRotatedText(_T("Select layer:"),20,248+25-18-25+8, 0);
	dc.DrawRotatedText(_T("Format:"),20,248+25+195-25+8, 0);
	if(view_srs)
		dc.DrawRotatedText(_T("SRS:"),20,248+25+195-25+8+40, 0);
}


void UIWCSServerConnection::EvtTreeItemSelChanged(wxTreeEvent& WXUNUSED(event))
{
	wxTreeItemId item_selected = Layer_tree_TreeCtrl->GetSelection();
	if (item_selected.IsOk())
	{	
		changes = true;
		std::string title = (std::string)Layer_tree_TreeCtrl->GetItemText(item_selected).mb_str();
		std::string name = node_title_name[title];
		if (name == "") return;

		if(!modify) Layer_name_Ed->SetValue(wxString(title.c_str(),wxConvUTF8));

		std::vector<std::string> formats = server_connection_controller->GetImageFormats(name);


		wxArrayString formats_array;
		std::vector<std::string>::iterator i = formats.begin();
		for(;i!=formats.end();i++)
		{
			if(*i != "")
			  formats_array.Insert(wxString((*i).c_str(),wxConvUTF8),formats_array.size());							
		}

		formatChoice->Clear();
		formatChoice->Append(formats_array);
		formatChoice->SetSelection(0);


		if(view_srs)
		{
			std::vector<std::string> srs_list = server_connection_controller->GetSRS(name);
			wxArrayString srs_array;
			std::vector<std::string>::iterator j = srs_list.begin();
			for(;j!=srs_list.end();j++)
			{
				if(*j != "")
				  srs_array.Insert(wxString((*j).c_str(),wxConvUTF8),srs_array.size());							
			}

			srsChoice->Clear();
			srsChoice->Append(srs_array);
			srsChoice->SetSelection(0);
		}
	}
}

void UIWCSServerConnection::FillTree(std::vector<cpw::ogcwcs::Node*> &nodes, wxTreeItemId &parent_id)
{
	wxTreeItemId id;
   
	for(std::vector<cpw::ogcwcs::Node*>::iterator i=nodes.begin();i!=nodes.end();i++)
	{
	  id = Layer_tree_TreeCtrl->AppendItem(parent_id,wxString((*i)->Title().c_str(),wxConvUTF8));
		if((*i)->Type() == 1)	
		  {
		    std::vector<cpw::ogcwcs::Node*, std::allocator<cpw::ogcwcs::Node*> > tmp=  (*i)->GetNodes();
		    UIWCSServerConnection::FillTree((std::vector<cpw::ogcwcs::Node*, std::allocator<cpw::ogcwcs::Node*> > &)tmp,id);
			std::string &icon_path = ApplicationConfiguration::GetInstance()->GetUIIconDirectory();
			AddIcon(icon_path + "/folder.png", id);
		}
		else
		{
			node_title_name[(*i)->Title()] = (*i)->Name();
			std::string &icon_path = ApplicationConfiguration::GetInstance()->GetUIIconDirectory();
			AddIcon(icon_path + "/wms.png", id);
		}
	}	
}


void UIWCSServerConnection::SetLayers(cpw::ogcwcs::Node &root_node)
{		
	std::vector<ogcwcs::Node*> nodes = root_node.GetNodes();
	Layer_tree_TreeCtrl;
	Layer_tree_TreeCtrl->DeleteAllItems();
	if(nodes.size()>0)
	{
	  wxTreeItemId root_id = Layer_tree_TreeCtrl->AddRoot(wxString(root_node.Title().c_str(),wxConvUTF8));
		std::string &icon_path = ApplicationConfiguration::GetInstance()->GetUIIconDirectory();
		AddIcon(icon_path + "/folder.png", root_id);
		if (nodes[0]->GetNodes().size()!=0)
			nodes = nodes[0]->GetNodes();
		UIWCSServerConnection::FillTree(nodes, root_id);
	}
}

std::vector<std::string> UIWCSServerConnection::GetSelectedLayers()
{	
	std::vector<std::string> v;
	wxTreeItemId TreeElementId = Layer_tree_TreeCtrl->GetSelection();
	wxString TreeElementString = wxString(Layer_tree_TreeCtrl->GetItemText(TreeElementId).To8BitData(),wxConvUTF8);
	std::string name = (std::string)TreeElementString.mb_str();
	v.push_back(node_title_name[name]);
	return v;
}

void UIWCSServerConnection::AddIcon(const std::string url, wxTreeItemId id)
{
	wxBitmap icon(wxString(url.c_str(),wxConvUTF8), wxBITMAP_TYPE_ANY);
 	int img_index = img_list->Add(icon);
	Layer_tree_TreeCtrl->SetItemImage(id, img_index);
}
