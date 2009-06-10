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

#include <sstream>
#include <string>

#include <stdafx.h>

#include <gui/UIFire.h>

#include <UIApplicationMainFrame.h>
#include <cpw/ApplicationConfiguration.h>
#include <cpw/ApplicationConfiguration.h>

#include <cpw/graphic/ApplicationScene.h>
#include <cpw/graphic/IScene.h>
#include <cpw/common/types.h>


using namespace cpw::gui;

#define IM_FIREROOT 0
#define IM_KEYFLAME 1
#define IM_FLAME 2

#define TIMELINE_CORNERX 20
#define TIMELINE_CORNERY 125
#define TIMELINE_HEIGHT 77
#define TIMELINE_WIDTH 348
#define TIMELINE_ACTSIZE 328

#define TAG_IGNITION_POINT 1
#define TAG_KEYFLAMES 0

BEGIN_EVENT_TABLE(UIFire,wxDialog)  
EVT_PAINT     (UIFire::OnPaint)
EVT_BUTTON    (ID_LOADFIRETEXURE_BUTTON,      UIFire::OnButtonLoadFireTexture)
EVT_BUTTON    (ID_LOADSMOKETEXTURE_BUTTON,      UIFire::OnButtonLoadSmokeTexture)
EVT_BUTTON    (ID_CANCEL_BUTTON,      UIFire::OnButtonCancel)
EVT_BUTTON    (ID_FINISH_BUTTON,      UIFire::OnButtonFinish)
EVT_BUTTON    (ID_ADDKEYFLAME_BUTTON, UIFire::OnButtonAddKeyFlame)
EVT_BUTTON    (ID_QUITFLAMEBUTTON,    UIFire::OnButtonQuitFlame)
EVT_LISTBOX   (ID_WXLISTBOX1,         UIFire::OnListBoxSelectItem)
EVT_CLOSE     (UIFire::OnClose)
EVT_MOVE      (UIFire::OnMove)
EVT_TEXT      (ID_EDIT_FLAMES, UIFire::CheckFlamesValues)
EVT_TEXT      (ID_SMOKETEXTURE_EDIT, UIFire::OnTexturesChanges)
EVT_TEXT      (ID_FIRETEXTURE_EDIT, UIFire::OnTexturesChanges)
EVT_TREE_SEL_CHANGED (ID_KEYFLAMES_TREECTRL, UIFire::OnTreeSelectionChanged)
EVT_DATE_CHANGED (ID_TIMESTAMP_CALENDAR, UIFire::OnDateChange)
EVT_TREE_ITEM_EXPANDED(ID_KEYFLAMES_TREECTRL, UIFire::OnItemExpanded)
EVT_TREE_ITEM_COLLAPSED(ID_KEYFLAMES_TREECTRL, UIFire::OnItemCollapsed)
EVT_LEFT_DOWN     (UIFire::OnMouseLeftDown)
EVT_LEFT_UP     (UIFire::OnMouseLeftUp)
EVT_MOTION  (UIFire::OnMouseLeftDrag)
EVT_SPIN_UP   (ID_SPIN_SMOKE_LIFE,  UIFire::OnSmokeLifeSpinUp)
EVT_SPIN_UP   (ID_SPIN_SMOKE_SIZE,  UIFire::OnSmokeSizeSpinUp)
EVT_SPIN_UP   (ID_SPIN_SMOKE_INTENSITY,  UIFire::OnSmokeIntesitySpinUp)
EVT_SPIN_UP   (ID_SPIN_FIRE_LIFE,  UIFire::OnFireLifeSpinUp)
EVT_SPIN_UP   (ID_SPIN_FIRE_SIZE,  UIFire::OnFireSizeSpinUp)
EVT_SPIN_UP   (ID_SPIN_FIRE_INTENSITY,  UIFire::OnFireIntensitySpinUp)
EVT_SPIN_UP   (ID_SPIN_POS_X,  UIFire::OnPositionXSpinUp)
EVT_SPIN_UP   (ID_SPIN_POS_Y,  UIFire::OnPositionYSpinUp)
EVT_SPIN_UP   (ID_SPIN_POS_Z,  UIFire::OnPositionZSpinUp)
EVT_SPIN_UP   (ID_SPIN_HOUR,  UIFire::OnHourSpinUp)
EVT_SPIN_DOWN   (ID_SPIN_HOUR,  UIFire::OnHourSpinDown)
EVT_SPIN_DOWN   (ID_SPIN_SMOKE_LIFE,  UIFire::OnSmokeLifeSpinDown)
EVT_SPIN_DOWN   (ID_SPIN_SMOKE_SIZE,  UIFire::OnSmokeSizeSpinDown)
EVT_SPIN_DOWN   (ID_SPIN_SMOKE_INTENSITY,  UIFire::OnSmokeIntesitySpinDown)
EVT_SPIN_DOWN   (ID_SPIN_FIRE_LIFE,  UIFire::OnFireLifeSpinDown)
EVT_SPIN_DOWN   (ID_SPIN_FIRE_SIZE,  UIFire::OnFireSizeSpinDown)
EVT_SPIN_DOWN   (ID_SPIN_FIRE_INTENSITY,  UIFire::OnFireIntensitySpinDown)
EVT_SPIN_DOWN   (ID_SPIN_POS_X,  UIFire::OnPositionXSpinDown)
EVT_SPIN_DOWN   (ID_SPIN_POS_Y,  UIFire::OnPositionYSpinDown)
EVT_SPIN_DOWN   (ID_SPIN_POS_Z,  UIFire::OnPositionZSpinDown)
EVT_NOTEBOOK_PAGE_CHANGED (ID_NOTE_PANEL, UIFire::OnNotePageChanged)
END_EVENT_TABLE()

UIFire::UIFire(cpw::controllers::FireController *fc, wxWindow *parent, wxWindowID id, const wxString &title, const wxPoint &position, const wxSize& size, long style)
: wxDialog(parent, id, title, position, size, style), paren(parent), fire_controller(fc), update_on_changes(true), modify(false), changes(false)
{
  splife = spseize = spintensity = fpsize = fplife = fpintensity = posx = posy = posz = 0;

  if (!(cpw::ApplicationConfiguration::GetInstance()->IsThemed()))
    {
      SetFont(paren->GetFont());
      SetForegroundColour(paren->GetForegroundColour());
      SetBackgroundColour(paren->GetBackgroundColour());
    }
  CreateGUIControls();
}

UIFire::~UIFire()
{
} 

void UIFire::CreateGUIControls()
{
  SetTitle(wxT("Fire"));
  SetIcon(wxNullIcon);
  SetSize(8,8,388,600);
  Center();

  keyflameCounter = 0 ;
  m_isDragging = false;
  timelineActivePerimeter = -1;

  int _col = 465;
  int _row = 195;

  std::string &icon_path = ApplicationConfiguration::GetInstance()->GetUIIconDirectory();
  std::string tmp = icon_path + "grid.png";
  m_bmp = wxBitmap(wxImage(wxString(tmp.c_str(),wxConvUTF8)));

  Fire_name_Ed = new wxTextCtrl(this, ID_FIRENAME_EDIT, wxT("Fire"), wxPoint(97,10), wxSize(237,21), 0, wxDefaultValidator, wxT("Fire_name_Ed"));
  Fire_name_Ed->SetFont(wxFont(8, wxSWISS, wxNORMAL,wxNORMAL, false, wxT("Tahoma")));

  Fire_texture_Ed = new wxTextCtrl(this, ID_FIRETEXTURE_EDIT, wxT(""), wxPoint(97,35), wxSize(237,21), 0, wxDefaultValidator, wxT("Fire_texture_Ed"));
  Fire_texture_Ed->SetFont(wxFont(8, wxSWISS, wxNORMAL,wxNORMAL, false, wxT("Tahoma")));

  Load_fire_texture_button = new wxButton(this, ID_LOADFIRETEXURE_BUTTON, wxT("..."), wxPoint(340,35), wxSize(22,22), 0, wxDefaultValidator, wxT("Load_fire_texture_button"));
  Load_fire_texture_button->SetFont(wxFont(8, wxSWISS, wxNORMAL,wxNORMAL, false, wxT("Tahoma")));

  Smoke_texture_Button = new wxTextCtrl(this, ID_SMOKETEXTURE_EDIT, wxT(""), wxPoint(97,60), wxSize(237,21), 0, wxDefaultValidator, wxT("Smoke_texture_Button"));
  Smoke_texture_Button->SetFont(wxFont(8, wxSWISS, wxNORMAL,wxNORMAL, false, wxT("Tahoma")));

  Load_smoke_texture_button = new wxButton(this, ID_LOADSMOKETEXTURE_BUTTON, wxT("..."), wxPoint(340,60), wxSize(22,22), 0, wxDefaultValidator, wxT("Load_smoke_texture_button"));
  Load_smoke_texture_button->SetFont(wxFont(8, wxSWISS, wxNORMAL,wxNORMAL, false, wxT("Tahoma")));

  _col = TIMELINE_CORNERX + (TIMELINE_WIDTH/2 - 100); _row = TIMELINE_CORNERY;
  tmp = icon_path + "film_add.png";
  Add_KeyFlame_Button = new wxBitmapButton(this, ID_ADDKEYFLAME_BUTTON, wxBitmap(wxImage(wxString(tmp.c_str(),wxConvUTF8))), wxPoint(20,_row + TIMELINE_HEIGHT + 5), wxSize(22,22), wxBU_AUTODRAW, wxDefaultValidator, wxT("Quit_Flame_Button"));
  tmp = icon_path + "delete.png";
  Quit_Flame_Button = new wxBitmapButton(this, ID_QUITFLAMEBUTTON, wxBitmap(wxImage(wxString(tmp.c_str(),wxConvUTF8))), wxPoint(20+24,_row + TIMELINE_HEIGHT + 5), wxSize(22,22), wxBU_AUTODRAW, wxDefaultValidator, wxT("Quit_Flame_Button"));

  timestamp_calendar = new wxDatePickerCtrl(this, ID_TIMESTAMP_CALENDAR, wxDefaultDateTime, wxPoint(70 + 60 + _col,_row + TIMELINE_HEIGHT + 10 ),wxSize(80,19), wxDP_DEFAULT | wxDP_SHOWCENTURY | wxDP_DROPDOWN);
  wxString dt = MyTimeCtrl::GetCurrentTime();
  timeStamp_Edit = new MyTimeCtrl(this, ID_TIMESTAMP_EDIT, dt, wxPoint(70+_col+80+60,_row + TIMELINE_HEIGHT + 10 ), wxSize(50,19));
  wxSpinButton *hour_spin = new wxSpinButton(this, ID_SPIN_HOUR, wxPoint(70+_col+80+60+51,_row + TIMELINE_HEIGHT + 10 ), wxSize(10,19), wxSP_VERTICAL);

  _col = TIMELINE_CORNERX+12+30;	_row = 221+207+22;

  Position_x_Edit = new wxTextCtrl(this, ID_EDIT_FLAMES, wxT(""), wxPoint(_col-10,_row), wxSize(62,19), wxTE_READONLY, wxDefaultValidator, wxT("Position_x_Edit"));
  Position_x_Edit->SetFont(wxFont(8, wxSWISS, wxNORMAL,wxNORMAL, false, wxT("Tahoma")));
  wxSpinButton *position_x_spin = new wxSpinButton(this, ID_SPIN_POS_X, wxPoint(_col+53,_row), wxSize(10,19), wxSP_VERTICAL);

  Position_x_StaticText = new wxStaticText(this, ID_Position_X_STATICTEXT, wxT("X"), wxPoint(_col-20,_row), wxDefaultSize, 0, wxT("Position_x_StaticText"));
  Position_x_StaticText->SetFont(wxFont(8, wxSWISS, wxNORMAL,wxNORMAL, false, wxT("Tahoma")));

  Position_y_Edit = new wxTextCtrl(this, ID_EDIT_FLAMES, wxT(""), wxPoint(_col-10,_row+22), wxSize(62,19), wxTE_READONLY, wxDefaultValidator, wxT("Position_y_Edit"));
  Position_y_Edit->SetFont(wxFont(8, wxSWISS, wxNORMAL,wxNORMAL, false, wxT("Tahoma")));
  wxSpinButton *position_y_spin = new wxSpinButton(this, ID_SPIN_POS_Y, wxPoint(_col+53,_row+22), wxSize(10,19), wxSP_VERTICAL);

  Position_y_StaticText = new wxStaticText(this, ID_POSITIONY_STATICTEXT, wxT("Y"), wxPoint(_col-20,_row+22), wxDefaultSize, 0, wxT("Position_y_StaticText"));
  Position_y_StaticText->SetFont(wxFont(8, wxSWISS, wxNORMAL,wxNORMAL, false, wxT("Tahoma")));

  Position_z_Edit = new wxTextCtrl(this, ID_EDIT_FLAMES, wxT(""), wxPoint(_col-10,_row+44), wxSize(62,19), wxTE_READONLY, wxDefaultValidator, wxT("Position_z_Edit"));
  Position_z_Edit->SetFont(wxFont(8, wxSWISS, wxNORMAL,wxNORMAL, false, wxT("Tahoma")));
  wxSpinButton *position_z_spin = new wxSpinButton(this, ID_SPIN_POS_Z, wxPoint(_col+53,_row+44), wxSize(10,19), wxSP_VERTICAL);

  Position_z_StaticText = new wxStaticText(this, ID_POSITIONZ_STATICTEXT, wxT("Z"), wxPoint(_col-20,_row+44), wxDefaultSize, 0, wxT("Position_z_StaticText"));
  Position_z_StaticText->SetFont(wxFont(8, wxSWISS, wxNORMAL,wxNORMAL, false, wxT("Tahoma")));

  //Position_StaticBox = new wxStaticBox(this, ID_POSITION_STATICBOX, wxT("Position"), wxPoint(_col-30,_row-22), wxSize(100,90));
  //Position_StaticBox->SetFont(wxFont(8, wxSWISS, wxNORMAL,wxNORMAL, false, wxT("Tahoma")));

  _col = TIMELINE_CORNERX+116+8+30;	_row = 221+207+22;

  Fire_particles_life_Edit = new wxTextCtrl(this, ID_EDIT_FLAMES, wxT(""), wxPoint(_col,_row), wxSize(52,19), 0, wxDefaultValidator, wxT("Fire_particles_life_Edit"));
  Fire_particles_life_Edit->SetFont(wxFont(8, wxSWISS, wxNORMAL,wxNORMAL, false, wxT("Tahoma")));
  wxSpinButton *fire_particles_life_spin = new wxSpinButton(this, ID_SPIN_FIRE_LIFE, wxPoint(_col+53,_row), wxSize(10,19), wxSP_VERTICAL);

  Fire_particles_life_StaticText = new wxStaticText(this, ID_FIREPARTICLESLIFE_STATICTEXT, wxT("Life"), wxPoint(_col-20,_row), wxDefaultSize, 0, wxT("Particles_life_StaticText"));
  Fire_particles_life_StaticText->SetFont(wxFont(8, wxSWISS, wxNORMAL,wxNORMAL, false, wxT("Tahoma")));

  Fire_particles_size_Edit = new wxTextCtrl(this, ID_EDIT_FLAMES, wxT(""), wxPoint(_col,_row+22), wxSize(52,19), 0, wxDefaultValidator, wxT("Fire_particles_size_Edit"));
  Fire_particles_size_Edit->SetFont(wxFont(8, wxSWISS, wxNORMAL,wxNORMAL, false, wxT("Tahoma")));
  wxSpinButton *fire_particles_size_spin = new wxSpinButton(this, ID_SPIN_FIRE_SIZE, wxPoint(_col+53,_row+22), wxSize(10,19), wxSP_VERTICAL);

  Fire_particles_size_StaticText = new wxStaticText(this, ID_FIREPARTICLESSIZE_STATICTEXT, wxT("Size"), wxPoint(_col-20,_row+22), wxDefaultSize, 0, wxT("WxStaticText7"));
  Fire_particles_size_StaticText->SetFont(wxFont(8, wxSWISS, wxNORMAL,wxNORMAL, false, wxT("Tahoma")));

  Fire_Intesity_Edit = new wxTextCtrl(this, ID_EDIT_FLAMES, wxT(""), wxPoint(_col+18,_row+44), wxSize(34,19), 0, wxDefaultValidator, wxT("Intesity_Edit"));
  Fire_Intesity_Edit->SetFont(wxFont(8, wxSWISS, wxNORMAL,wxNORMAL, false, wxT("Tahoma")));
  wxSpinButton *fire_particles_intensity_spin = new wxSpinButton(this, ID_SPIN_FIRE_INTENSITY, wxPoint(_col+53,_row+44), wxSize(10,19), wxSP_VERTICAL);

  wxStaticText *Fire_Intensity_StaticText = new wxStaticText(this, ID_FIRE_INTENSITY_STATICTEXT, wxT("Intesity"), wxPoint(_col-20,_row+44), wxDefaultSize, 0, wxT("Intensity_StaticText"));
  Fire_Intensity_StaticText->SetFont(wxFont(8, wxSWISS, wxNORMAL,wxNORMAL, false, wxT("Tahoma")));

  //Fire_particles_StaticBox = new wxStaticBox(this, ID_FIREPARTICLES_STATICBOX, wxT("Fire Particles"), wxPoint(_col-30,_row-22), wxSize(100,90));
  //Fire_particles_StaticBox->SetFont(wxFont(8, wxSWISS, wxNORMAL,wxNORMAL, false, wxT("Tahoma")));

  _col = TIMELINE_CORNERX+232+4+30;	_row = 221+207+22;

  Smoke_particles_life_Edit = new wxTextCtrl(this, ID_EDIT_FLAMES, wxT(""), wxPoint(_col,_row), wxSize(52,19), 0, wxDefaultValidator, wxT("Smoke_particles_life_Edit"));
  Smoke_particles_life_Edit->SetFont(wxFont(8, wxSWISS, wxNORMAL,wxNORMAL, false, wxT("Tahoma")));
  wxSpinButton *Smoke_particles_life_spin = new wxSpinButton(this, ID_SPIN_SMOKE_LIFE, wxPoint(_col+53,_row), wxSize(10,19), wxSP_VERTICAL);

  Smoke_particles_life_StaticText = new wxStaticText(this, ID_SMOKEPARTICLESLIFE_STATICTEXT, wxT("Life"), wxPoint(_col-20,_row), wxDefaultSize, 0, wxT("Particles_life_StaticText"));
  Smoke_particles_life_StaticText->SetFont(wxFont(8, wxSWISS, wxNORMAL,wxNORMAL, false, wxT("Tahoma")));

  Smoke_particles_seize_Edit = new wxTextCtrl(this, ID_EDIT_FLAMES, wxT(""), wxPoint(_col,_row+22), wxSize(52,19), 0, wxDefaultValidator, wxT("Smoke_particles_seize_Edit"));
  Smoke_particles_seize_Edit->SetFont(wxFont(8, wxSWISS, wxNORMAL,wxNORMAL, false, wxT("Tahoma")));
  wxSpinButton *Smoke_particles_size_spin = new wxSpinButton(this, ID_SPIN_SMOKE_SIZE, wxPoint(_col+53,_row+22), wxSize(10,19), wxSP_VERTICAL);

  Smoke_particles_size_StaticText = new wxStaticText(this, ID_SMOKEPARTICLESSIZE_STATICTEXT, wxT("Size"), wxPoint(_col-20,_row+22), wxDefaultSize, 0, wxT("WxStaticText7"));
  Smoke_particles_size_StaticText->SetFont(wxFont(8, wxSWISS, wxNORMAL,wxNORMAL, false, wxT("Tahoma")));

  smoke_Intesity_Edit = new wxTextCtrl(this, ID_EDIT_FLAMES, wxT(""), wxPoint(_col+18,_row+44), wxSize(34,19), 0, wxDefaultValidator, wxT("Intesity_Edit"));
  smoke_Intesity_Edit->SetFont(wxFont(8, wxSWISS, wxNORMAL,wxNORMAL, false, wxT("Tahoma")));
  wxSpinButton *Smoke_particles_intensity_spin = new wxSpinButton(this, ID_SPIN_SMOKE_INTENSITY, wxPoint(_col+53,_row+44), wxSize(10,19), wxSP_VERTICAL);

  wxStaticText *smoke_Intensity_StaticText = new wxStaticText(this, ID_SMOKE_INTENSITY_STATICTEXT, wxT("Intesity"), wxPoint(_col-20,_row+44), wxDefaultSize, 0, wxT("Intensity_StaticText"));
  smoke_Intensity_StaticText->SetFont(wxFont(8, wxSWISS, wxNORMAL,wxNORMAL, false, wxT("Tahoma")));

  //Smoke_particles_StaticBox = new wxStaticBox(this, ID_SMOKEPARTICLES_STATICBOX, wxT("Smoke Particles"), wxPoint(_col-30,_row-22), wxSize(100,90));
  //if (!(cpw::ApplicationConfiguration::GetInstance()->IsThemed()))
  //{
  //	Smoke_particles_StaticBox->SetForegroundColour(paren->GetForegroundColour());
  //	Smoke_particles_StaticBox->SetBackgroundColour(paren->GetBackgroundColour());
  //}
  //Smoke_particles_StaticBox->SetFont(paren->GetFont());


  note_panel = new wxNotebook(this, ID_NOTE_PANEL, wxPoint(TIMELINE_CORNERX ,237), wxSize(TIMELINE_WIDTH,186), wxNB_TOP, _T("Note_Panel"));
  note_panel->SetFont(GetFont());
  note_panel->SetForegroundColour(GetForegroundColour());
  note_panel->SetBackgroundColour(GetBackgroundColour());

  wxArrayString arrayStringFor_WxListBox1;
  WxListBox1 = new wxListBox(note_panel, ID_WXLISTBOX1, wxDefaultPosition, wxDefaultSize, arrayStringFor_WxListBox1, wxLB_SINGLE);
  WxListBox1->SetFont(GetFont());
  WxListBox1->SetForegroundColour(GetForegroundColour());
  if (!(cpw::ApplicationConfiguration::GetInstance()->IsThemed()))
    WxListBox1->SetBackgroundColour(wxColour(wxString(cpw::ApplicationConfiguration::GetInstance()->GetPageColour().c_str(),wxConvUTF8)));

  img_list = new wxImageList(16, 16);
  tmp = icon_path + "fire.png";
  img_list->Add(wxBitmap(wxImage(wxString(tmp.c_str(),wxConvUTF8))));
  tmp = icon_path + "film.png";
  img_list->Add(wxBitmap(wxImage(wxString(tmp.c_str(),wxConvUTF8))));
  tmp = icon_path + "flame.png";
  img_list->Add(wxBitmap(wxImage(wxString(tmp.c_str(),wxConvUTF8))));

  Keyflames_TreeCtrl = new wxTreeCtrl(note_panel, ID_KEYFLAMES_TREECTRL, wxDefaultPosition, wxDefaultSize, wxTR_HAS_BUTTONS/* | wxTR_HIDE_ROOT*/ | wxTR_SINGLE);//, wxTR_DEFAULT_STYLE | wxTR_HIDE_ROOT/*, wxTR_HAS_BUTTONS*/);
  Keyflames_TreeCtrl->SetFont(GetFont());
  Keyflames_TreeCtrl->SetForegroundColour(GetForegroundColour());

  if (!(cpw::ApplicationConfiguration::GetInstance()->IsThemed()))
    {
      Keyflames_TreeCtrl->SetBackgroundColour(wxColour(wxString(cpw::ApplicationConfiguration::GetInstance()->GetPageColour().c_str(),wxConvUTF8)));
      Keyflames_TreeCtrl->SetForegroundColour(wxColour(wxString(cpw::ApplicationConfiguration::GetInstance()->GetPageFontColour().c_str(),wxConvUTF8)));
    }
  Keyflames_TreeCtrl->SetFont(wxFont(8, wxSWISS, wxNORMAL,wxNORMAL, false, wxT("Tahoma")));
  Keyflames_TreeCtrl->AssignImageList(img_list);
  Keyflames_TreeCtrl->AddRoot(_T("Fire"),IM_FIREROOT);
	
  note_panel->AddPage(Keyflames_TreeCtrl, _T("Perimeters"));
  note_panel->AddPage(WxListBox1,_T("Ingnition Points"));

  Finish_button = new wxButton(this, ID_FINISH_BUTTON, wxT("Finish"), wxPoint(201,534), wxSize(75,23), 0, wxDefaultValidator, wxT("Finish_button"));
  Finish_button->SetFont(wxFont(8, wxSWISS, wxNORMAL,wxNORMAL, false, wxT("Tahoma")));

  Cancel_button = new wxButton(this, ID_CANCEL_BUTTON, wxT("Cancel"), wxPoint(285,534), wxSize(75,23), 0, wxDefaultValidator, wxT("Cancel_button"));
  Cancel_button->SetFont(wxFont(8, wxSWISS, wxNORMAL,wxNORMAL, false, wxT("Tahoma")));

  Connect(ID_TIMESTAMP_EDIT, MyEVT_TIMECHANGE, (wxObjectEventFunction)(wxEventFunction)(wxCommandEventFunction) &UIFire::OnHourChanges);

  DisablePropertiesControls();
}

void UIFire::OnHourChanges(wxCommandEvent& event)
{
  wxDateEvent tmp = wxDateEvent();
  OnDateChange((wxDateEvent &)tmp);
}

void UIFire::OnClose(wxCloseEvent& /*event*/)
{
  //if (!modify) 
  //	fire_controller->Cancel();
  //else
  //	fire_controller->RestoreBackup();

  //this->EndDialog(wxID_CANCEL);	
  //fire_controller->ReleaseMouseCallBack();
  Close();
}

void UIFire::OnButtonLoadFireTexture(wxCommandEvent& WXUNUSED(event))
{
  wxFileDialog dialog (this,_T("Search Fire Texture"),_T("./config/default"),wxEmptyString,
		       _T("All files(*.*)|*.*") );

  if(dialog.ShowModal() == wxID_OK)
    {						
      std::string url = std::string(dialog.GetPath().mb_str());
      Fire_texture_Ed->SetValue(wxString(url.c_str(),wxConvUTF8)); 
      fire_texture = url;
      changes = true;
    }
}


void UIFire::OnButtonLoadSmokeTexture(wxCommandEvent& WXUNUSED(event))
{
  wxFileDialog dialog (this,_T("Search Smoke Texture"),_T("./config/default"),wxEmptyString,
		       _T("All files(*.*)|*.*") );

  if(dialog.ShowModal() == wxID_OK)
    {						
      std::string url = std::string(dialog.GetPath().mb_str());		
      Smoke_texture_Button->SetValue(wxString(url.c_str(),wxConvUTF8)); 
      smoke_texture = url;
      changes = true;
    }

}

void UIFire::OnTexturesChanges(wxCommandEvent& WXUNUSED(event))
{
  std::string ft, st;
  ft = std::string(Fire_texture_Ed->GetValue().mb_str());
  st = std::string(Smoke_texture_Button->GetValue().mb_str());
  if((ft != "") && (st != "") )
    fire_controller->ChangeTextures(ft, st);
  changes = true;
}

void UIFire::OnButtonCancel(wxCommandEvent& WXUNUSED(event))
{
  Close();
}

void UIFire::Close()
{
  if (changes)
    {
      wxMessageDialog message(this,wxT("Save changes before quit?"), wxT("Warning"),wxICON_EXCLAMATION |wxYES_NO |wxCANCEL);
      int modal = message.ShowModal();
      if(modal == wxID_YES)
	{
	  wxCommandEvent tmp = wxCommandEvent();
	  OnButtonFinish((wxCommandEvent &)tmp);
	}
      else if(modal == wxID_NO )
	{
	  if(!modify) 
	    fire_controller->Cancel();
	  else
	    fire_controller->RestoreBackup();
			
	  //fire_controller->ReleaseMouseCallBack();
	  EndDialog(wxID_CANCEL);
						
	}
      else if (modal == wxID_CANCEL)
	{
			
	}
    }
  else
    {
      if(!modify)
	fire_controller->Cancel();
      else
	fire_controller->RestoreBackup();

      EndDialog(wxID_CANCEL);	
    }

}

void UIFire::OnButtonFinish(wxCommandEvent& WXUNUSED(event))
{
  std::string p = std::string(Fire_name_Ed->GetValue().mb_str());
  if((Fire_name_Ed->IsEmpty()) || (p.at(0) == ' ' ) )
    {
      wxMessageDialog message(this,wxT("The fire needs a name."), wxT("Warning"),wxICON_EXCLAMATION |wxOK);
      message.ShowModal();
    }
  else
    {	
      if (fire_controller->CreatePermanentFire(std::string(Fire_name_Ed->GetValue().mb_str()),
					       std::string(Fire_texture_Ed->GetValue().mb_str()),
					       std::string(Smoke_texture_Button->GetValue().mb_str()), modify))
	{
	  EndDialog(wxID_OK);
	  fire_controller->ReleaseMouseCallBack();
	}
      else
	{
	  if(!fire_controller->GetOverwrite())
	    {
	      wxMessageDialog message(this,wxT("    The fire needs one perimeter at least\n and each perimeter must have one flame at least.\n    Also, each perimeter must have a minimun \nof two flames."), wxT("Warning"),wxICON_EXCLAMATION |wxOK);
	      message.ShowModal();
	    }			
	}
    }
}

void UIFire::OnMove(wxMoveEvent& event)
{
}

void UIFire::OnListBoxSelectItem(wxCommandEvent& WXUNUSED(event))
{
  if (update_on_changes)
    fire_controller->UpdateUI(WxListBox1->GetSelection(), GetActiveKeyFlame(), GetActiveFlame());
}

void UIFire::CheckFlamesValuesInt()
{
  if (!update_on_changes)
    return;

  double aux;

  if (Smoke_particles_seize_Edit->GetValue().Trim().ToDouble(&aux) && aux>=0)
    spseize = fabs((float) aux);
  else
    {
      std::stringstream aux2;
      aux2 << std::fixed << std::setprecision (5) << spseize;
      Smoke_particles_seize_Edit->ChangeValue(wxString(aux2.str().c_str(),wxConvUTF8));
    }


  if (Smoke_particles_life_Edit->GetValue().Trim().ToDouble(&aux) && aux>=0)
    splife = fabs((float) aux);
  else
    {
      std::stringstream aux2;
      aux2 << std::fixed << std::setprecision (5) << splife;
      Smoke_particles_life_Edit->ChangeValue(wxString(aux2.str().c_str(),wxConvUTF8));
    }

  if (Fire_particles_size_Edit->GetValue().Trim().ToDouble(&aux) && aux>=0)
    fpsize = fabs((float) aux);
  else
    {
      std::stringstream aux2;
      aux2 << std::fixed << std::setprecision (5) << fpsize;
      Fire_particles_size_Edit->ChangeValue(wxString(aux2.str().c_str(),wxConvUTF8));
    }

  if (Fire_particles_life_Edit->GetValue().Trim().ToDouble(&aux) && aux>=0)
    fplife = fabs((float) aux);
  else
    {
      std::stringstream aux2;
      aux2 << std::fixed << std::setprecision (5) << fplife;
      Fire_particles_life_Edit->ChangeValue(wxString(aux2.str().c_str(),wxConvUTF8));
    }

  if (Position_x_Edit->GetValue().Trim().ToDouble(&aux) && aux>=0)
    posx = fabs((float) aux);
  else
    {
      std::stringstream aux2;
      aux2 << std::fixed << std::setprecision (15) << posx;
      Position_x_Edit->ChangeValue(wxString(aux2.str().c_str(),wxConvUTF8));
    }

  if (Position_y_Edit->GetValue().Trim().ToDouble(&aux) && aux>=0)
    posy = fabs((float) aux);
  else
    {
      std::stringstream aux2;
      aux2 << std::fixed << std::setprecision (15) << posy;
      Position_y_Edit->ChangeValue(wxString(aux2.str().c_str(),wxConvUTF8));
    }

  if (Position_z_Edit->GetValue().Trim().ToDouble(&aux) && aux>=0)
    posz = fabs((float) aux);
  else
    {
      std::stringstream aux2;
      aux2 << std::fixed << std::setprecision (15) << posz;
      Position_z_Edit->ChangeValue(wxString(aux2.str().c_str(),wxConvUTF8));
    }

  if (smoke_Intesity_Edit->GetValue().Trim().ToDouble(&aux) && aux>=0)
    spintensity = fabs((float) aux);
  else
    {
      std::stringstream aux2;
      aux2 << std::fixed << std::setprecision (5) << spintensity;
      smoke_Intesity_Edit->ChangeValue(wxString(aux2.str().c_str(),wxConvUTF8));
    }

  if (Fire_Intesity_Edit->GetValue().Trim().ToDouble(&aux) && aux>=0)
    fpintensity = fabs((float) aux);
  else
    {
      std::stringstream aux2;
      aux2 << std::fixed << std::setprecision (5) << fpintensity;
      Fire_Intesity_Edit->ChangeValue(wxString(aux2.str().c_str(),wxConvUTF8));
    }

  fire_controller->UpdateFlame(GetActiveKeyFlame(), GetActiveFlame());

  changes = true;
}

void UIFire::CheckFlamesValues(wxCommandEvent& WXUNUSED(event))
{
  CheckFlamesValuesInt();
}

bool UIFire::GetCoordsFromEditControls(cpw::Point3d<float> &position, float &smoke_life, float &smoke_size, float &fire_life, float &fire_size, float &fire_intensity, float &smoke_intensity)
{
  double aux;

  if (!Smoke_particles_seize_Edit->GetValue().Trim().ToDouble(&aux))
    return false;

  smoke_size = (float) aux;

  if (!Smoke_particles_life_Edit->GetValue().Trim().ToDouble(&aux))
    return false;

  smoke_life = (float) aux;

  if (!Fire_particles_size_Edit->GetValue().Trim().ToDouble(&aux))
    return false;

  fire_size = (float) aux;

  if (!Fire_particles_life_Edit->GetValue().Trim().ToDouble(&aux))
    return false;

  fire_life = (float) aux;

  if (!Position_x_Edit->GetValue().Trim().ToDouble(&aux))
    return false;

  position.x = (float) aux;

  if (!Position_y_Edit->GetValue().Trim().ToDouble(&aux))
    return false;

  position.y = (float) aux;

  if (!Position_z_Edit->GetValue().Trim().ToDouble(&aux))
    return false;

  position.z = (float) aux;

  if (!Fire_Intesity_Edit->GetValue().Trim().ToDouble(&aux))
    return false;

  fire_intensity = (float) aux;

  if (!smoke_Intesity_Edit->GetValue().Trim().ToDouble(&aux))
    return false;

  smoke_intensity = (float) aux;

  return true;
}

void UIFire::DisableEditControls()
{
  Smoke_particles_seize_Edit->Enable(false);
  Smoke_particles_life_Edit->Enable(false);
  Fire_particles_size_Edit->Enable(false);
  Fire_particles_life_Edit->Enable(false);
  smoke_Intesity_Edit->Enable(false);
  Fire_Intesity_Edit->Enable(false);
}

void UIFire::EnableEditControls()
{
  Smoke_particles_seize_Edit->Enable(true);
  Smoke_particles_life_Edit->Enable(true);
  Fire_particles_size_Edit->Enable(true);
  Fire_particles_life_Edit->Enable(true);
  smoke_Intesity_Edit->Enable(true);
  Fire_Intesity_Edit->Enable(true);
}

void UIFire::EnablePropertiesControls()
{
  Smoke_particles_seize_Edit->Enable(true);
  Smoke_particles_life_Edit->Enable(true);
  Fire_particles_size_Edit->Enable(true);
  Fire_particles_life_Edit->Enable(true);
  Position_x_Edit->Enable(true);
  Position_y_Edit->Enable(true);
  Position_z_Edit->Enable(true);
  smoke_Intesity_Edit->Enable(true);
  Fire_Intesity_Edit->Enable(true);
  timeStamp_Edit->Enable(true);
  timestamp_calendar->Enable(true);
}
void UIFire::DisablePropertiesControls()
{
  Smoke_particles_seize_Edit->Enable(false);
  Smoke_particles_life_Edit->Enable(false);
  Fire_particles_size_Edit->Enable(false);
  Fire_particles_life_Edit->Enable(false);
  Position_x_Edit->Enable(false);
  Position_y_Edit->Enable(false);
  Position_z_Edit->Enable(false);
  smoke_Intesity_Edit->Enable(false);
  Fire_Intesity_Edit->Enable(false);
}
void UIFire::EnableParticleControls()
{
  Smoke_particles_seize_Edit->Enable(true);
  Smoke_particles_life_Edit->Enable(true);
  Fire_particles_size_Edit->Enable(true);
  Fire_particles_life_Edit->Enable(true);
  smoke_Intesity_Edit->Enable(true);
  Fire_Intesity_Edit->Enable(true);
  timeStamp_Edit->Enable(true);
  timestamp_calendar->Enable(true);
  Position_x_Edit->Enable(true);
  Position_y_Edit->Enable(true);
  Position_z_Edit->Enable(true);
}
void UIFire::DisableParticleControls()
{
  Smoke_particles_seize_Edit->Enable(false);
  Smoke_particles_life_Edit->Enable(false);
  Fire_particles_size_Edit->Enable(false);
  Fire_particles_life_Edit->Enable(false);
  smoke_Intesity_Edit->Enable(false);
  Fire_Intesity_Edit->Enable(false);
  timeStamp_Edit->Enable(false);
  timestamp_calendar->Enable(false);
  Position_x_Edit->Enable(false);
  Position_y_Edit->Enable(false);
  Position_z_Edit->Enable(false);
}

void UIFire::Clear()
{
  WxListBox1->Clear();
  Smoke_particles_seize_Edit->Clear();
  Smoke_particles_life_Edit->Clear();
  Fire_particles_size_Edit->Clear();
  Fire_particles_life_Edit->Clear();
  Fire_Intesity_Edit->Clear();
  smoke_Intesity_Edit->Clear();
  Position_x_Edit->Clear();
  Position_y_Edit->Clear();
  Position_z_Edit->Clear();
  keyflameCounter = 0;
  splife = spseize = spintensity = fpsize = fplife = fpintensity = posx = posy = posz = 0;

  Keyflames_TreeCtrl->DeleteChildren(Keyflames_TreeCtrl->GetRootItem());

}


void UIFire::OnButtonQuitFlame(wxCommandEvent& WXUNUSED(event))
{
  fire_controller->Remove();
  wxClientDC dc(this);
  render(dc);
  changes = true;
}

void UIFire::AddIcon(const std::string url, wxTreeItemId id)
{
  wxBitmap icon(wxString(url.c_str(),wxConvUTF8), wxBITMAP_TYPE_ANY);
  int img_index = img_list->Add(icon);
  Keyflames_TreeCtrl->SetItemImage(id, img_index);
}

void UIFire::OnButtonAddFlame(wxCommandEvent& WXUNUSED(event))
{
  if (note_panel->GetSelection() == TAG_KEYFLAMES)
    {
      if (wxTreeItemId item_id = Keyflames_TreeCtrl->GetSelection())
	{
	  wxString  item_label = Keyflames_TreeCtrl->GetItemText(item_id);
	  if (item_label(0,9) == wxT("Perimeter"))
	    {
	      Keyflames_TreeCtrl->AppendItem(item_id, _T("Flame"), IM_FLAME);
	      Keyflames_TreeCtrl->Expand(item_id);
	    }
	  if (item_label == wxT("Flame"))
	    Keyflames_TreeCtrl->InsertItem(Keyflames_TreeCtrl->GetItemParent(item_id), item_id, _T("Flame"), IM_FLAME);
	}
    }
  else
    {
      WxListBox1->AppendAndEnsureVisible(_T("Ignition point"));
    }
  changes = true;
}

void UIFire::OnButtonAddKeyFlame(wxCommandEvent& WXUNUSED(event))
{
  if (note_panel->GetSelection() == TAG_KEYFLAMES)
    {
      timelineActivePerimeter = ++keyflameCounter;

      std::ostringstream wop;
      wop << "Perimeter " << keyflameCounter;	
	
      wxTreeItemId id = Keyflames_TreeCtrl->AppendItem(Keyflames_TreeCtrl->GetRootItem(), wxString(wop.str().c_str(),wxConvUTF8), IM_KEYFLAME);
      Keyflames_TreeCtrl->Expand(Keyflames_TreeCtrl->GetRootItem());
      timeStamp_Edit->SetValue(MyTimeCtrl::GetCurrentTime());
      fire_controller->AddTimeSlice(timestamp_calendar->GetValue().GetTicks() +  GetTime());
      fire_controller->UpdateUI(-1,timelineActivePerimeter-1);
      fire_controller->UpdateRender(WxListBox1->GetSelection(), timelineActivePerimeter);
      collapsed_keyflames.push_back(false);
      changes = true;
    }
}

void UIFire::Clone_TreeCtrl_Childrens(wxTreeItemId from_item,wxTreeItemId to_item)
{
  wxTreeItemIdValue cookie;
  if (wxTreeItemId child_item = Keyflames_TreeCtrl->GetFirstChild(from_item,cookie))
    {
      wxString item_label = Keyflames_TreeCtrl->GetItemText(child_item);
      int		 item_image = Keyflames_TreeCtrl->GetItemImage(child_item);			
      Keyflames_TreeCtrl->AppendItem(to_item, item_label, item_image);
      for (int i = 0; i < (signed)Keyflames_TreeCtrl->GetChildrenCount(from_item) -1 ; i++)
	{		
	  child_item = Keyflames_TreeCtrl->GetNextSibling(child_item);
	  item_label = Keyflames_TreeCtrl->GetItemText(child_item);
	  item_image = Keyflames_TreeCtrl->GetItemImage(child_item);			
	  Keyflames_TreeCtrl->AppendItem(to_item, item_label, item_image);
	}
      Keyflames_TreeCtrl->Expand(to_item);
    }
}

int UIFire::GetActiveNotePage()
{
  return note_panel->GetSelection();
}

int UIFire::GetActiveKeyFlame()
{
  int pos = -1;
  bool flag_found = false;
  if (wxTreeItemId selection = Keyflames_TreeCtrl->GetSelection())
    {
      wxTreeItemId root = Keyflames_TreeCtrl->GetRootItem();
      wxTreeItemIdValue cookie;
      wxTreeItemId child = Keyflames_TreeCtrl->GetFirstChild(root,cookie);
      if (child.IsOk())
	{
	  pos = 0;
	  while (child.IsOk() && (child != selection) && !flag_found)
	    {
	      if (Keyflames_TreeCtrl->ItemHasChildren(child))		
		{
		  wxTreeItemIdValue cookie2;
		  wxTreeItemId grandChild = Keyflames_TreeCtrl->GetFirstChild(child,cookie2);
		  while (grandChild.IsOk() && (grandChild != selection))
		    {
		      grandChild = Keyflames_TreeCtrl->GetNextSibling(grandChild);
		    }
		  if (grandChild == selection) 
		    flag_found = true;
		}
	      if (!flag_found)
		{
		  child = Keyflames_TreeCtrl->GetNextSibling(child);
		  pos++;
		}
	    }
	  if (!child.IsOk()) pos = -1;
	}
    }
  return pos;
}

int UIFire::GetActiveFlame()
{
  int pos = -1;
  bool flag_found = false;
  if (wxTreeItemId selection = Keyflames_TreeCtrl->GetSelection())
    {
      wxTreeItemId root = Keyflames_TreeCtrl->GetRootItem();
      wxTreeItemIdValue cookie;
      wxTreeItemId child = Keyflames_TreeCtrl->GetFirstChild(root,cookie);
      while (child.IsOk() && !flag_found)
	{
	  if (Keyflames_TreeCtrl->ItemHasChildren(child))		
	    {
	      int pos_ = 0;
	      wxTreeItemIdValue cookie2;
	      wxTreeItemId grandChild = Keyflames_TreeCtrl->GetFirstChild(child,cookie2);
	      while (grandChild.IsOk() && (grandChild != selection))
		{
		  grandChild = Keyflames_TreeCtrl->GetNextSibling(grandChild);
		  pos_++;
		}
	      if (grandChild == selection) 
		{
		  pos = pos_;
		  flag_found = true;
		}
	    }
	  child = Keyflames_TreeCtrl->GetNextSibling(child);
	}
    }
  return pos;
}

void UIFire::SetActive(int perimeter, int flame)
{
  wxTreeItemId root = Keyflames_TreeCtrl->GetRootItem();
  wxTreeItemIdValue cookie;
  wxTreeItemId child = Keyflames_TreeCtrl->GetFirstChild(root,cookie);
	
  for ( int i = 0; i < perimeter ; i++ )
    child = Keyflames_TreeCtrl->GetNextSibling(child);

  if (child.IsOk())
    {
      if (flame<0)
	{
	  Keyflames_TreeCtrl->SelectItem(child);
	}
      else
	{
	  wxTreeItemIdValue cookie2;
	  wxTreeItemId grandChild = Keyflames_TreeCtrl->GetFirstChild(child,cookie2);
	  for ( int i = 0; i < flame ; i++ )
	    {
	      grandChild = Keyflames_TreeCtrl->GetNextSibling(grandChild);
	    }
	  if (grandChild.IsOk()) Keyflames_TreeCtrl->SelectItem(grandChild);
	}
    }
}

void UIFire::AddPerimeter(const bool &new_p)
{
  keyflameCounter++;
  std::ostringstream wop;
  wop << "Perimeter " << keyflameCounter;	

  Keyflames_TreeCtrl->AppendItem(Keyflames_TreeCtrl->GetRootItem(), wxString(wop.str().c_str(),wxConvUTF8), IM_KEYFLAME);
  Keyflames_TreeCtrl->Expand(Keyflames_TreeCtrl->GetRootItem());
	
  if (new_p)
    collapsed_keyflames.push_back(true);

}

void UIFire::AddFlame(int perimeter)
{
  wxTreeItemId root = Keyflames_TreeCtrl->GetRootItem();
  wxTreeItemIdValue cookie;
  wxTreeItemId child = Keyflames_TreeCtrl->GetFirstChild(root,cookie);
  for ( int i = 0; i < perimeter ; i++ )
    {
      child = Keyflames_TreeCtrl->GetNextSibling(child);
    }
  if (child.IsOk())
    {
      Keyflames_TreeCtrl->AppendItem(child, _T("Flame"), IM_FLAME);
    }
}

void UIFire::OnTreeSelectionChanged(wxTreeEvent& wxevent)
{
  UpdateOnChanges();	
}

void UIFire::OnNotePageChanged(wxNotebookEvent& event)
{
  if (GetActiveNotePage() != TAG_IGNITION_POINT)
    WxListBox1->SetSelection(wxNOT_FOUND);
  SetActive(-1,-1);
}

void UIFire::UpdateOnChanges()
{
  EnablePropertiesControls();
  if (GetActiveNotePage() == TAG_IGNITION_POINT)
    DisableParticleControls();
  else
    {
      if (GetActiveFlame() <0)
	DisablePropertiesControls();
    }

  if (update_on_changes)
    {
      timelineActivePerimeter = GetActiveKeyFlame();
      fire_controller->UpdateUI(WxListBox1->GetSelection(), timelineActivePerimeter, GetActiveFlame());
      fire_controller->UpdateRender(WxListBox1->GetSelection(), timelineActivePerimeter);
    }
}

void UIFire::OnDateChange(wxDateEvent& event)
{
  wxDateTime temp = timestamp_calendar->GetValue();
  wxDateTime out((double)(temp.GetTicks() + GetTime()));

  int prev_keyflame, next_keyflame;
  next_keyflame = (timelineActivePerimeter >= keyflameCounter-1)? keyflameCounter-1 : timelineActivePerimeter+1;
  prev_keyflame = (timelineActivePerimeter == 0)? 0 : timelineActivePerimeter-1;
  wxDateTime prev_time = fire_controller->GetKeyFlameTimeStamp(prev_keyflame);
  wxDateTime next_time = fire_controller->GetKeyFlameTimeStamp(next_keyflame);

  if ( ( out.IsValid() && prev_time.IsValid() && next_time.IsValid() ) && (
									   ((out < next_time) && (out > prev_time)) ||
									   ((timelineActivePerimeter == 0                ) && (out < next_time)) ||
									   ((timelineActivePerimeter == keyflameCounter-1) && (out > prev_time))    ))

    fire_controller->ChangeDate(GetActiveKeyFlame(), out);

  else
    UpdateOnChanges();
}


void UIFire::OnItemExpanded(wxTreeEvent& wxevent)
{
  unsigned int active_kflame = (unsigned int) GetActiveKeyFlame();

  if (collapsed_keyflames.size() == 0)
    return;

  if (collapsed_keyflames.size()-1 > active_kflame)
    collapsed_keyflames[active_kflame] = false;
}

void UIFire::OnItemCollapsed(wxTreeEvent& wxevent)
{
  unsigned int active_kflame =(unsigned int) GetActiveKeyFlame();

  if (collapsed_keyflames.size() == 0)
    return;

  if (collapsed_keyflames.size()-1 > active_kflame)
    collapsed_keyflames[active_kflame] = true;
}

void UIFire::Collapse()
{
  int pos=0;
	
  wxTreeItemId root = Keyflames_TreeCtrl->GetRootItem();
  wxTreeItemIdValue cookie;
  wxTreeItemId child = Keyflames_TreeCtrl->GetFirstChild(root,cookie);
  while (child.IsOk())
    {
      if(collapsed_keyflames[pos])
	Keyflames_TreeCtrl->Expand(child);

      child = Keyflames_TreeCtrl->GetNextSibling(child);
      pos++;
    }
}

void UIFire::CollapseAllKeyflamesExceptOne(wxTreeItemId &exception_id)
{
  wxTreeItemId root = Keyflames_TreeCtrl->GetRootItem();
  wxTreeItemIdValue cookie;

		
  wxTreeItemId current_keyflame = Keyflames_TreeCtrl->GetFirstChild(root, cookie);
	
  while (current_keyflame.IsOk())
    {
      Keyflames_TreeCtrl->Collapse(current_keyflame);
      current_keyflame = Keyflames_TreeCtrl->GetNextSibling(current_keyflame);
    }

  current_keyflame = Keyflames_TreeCtrl->GetFirstChild(root, cookie);
	
  while (current_keyflame.IsOk())
    {
      if (current_keyflame == exception_id) 
	{
	  Keyflames_TreeCtrl->Expand(current_keyflame);
	  return;
	}
      else
	{
	  wxTreeItemId flame_id = Keyflames_TreeCtrl->GetFirstChild(current_keyflame, cookie);
	  while (flame_id.IsOk())
	    {
	      if (flame_id == exception_id)
		{
		  Keyflames_TreeCtrl->Expand(current_keyflame);
		  return;
		}
	      else
		flame_id = Keyflames_TreeCtrl->GetNextSibling(flame_id);
	    }
	}
      current_keyflame = Keyflames_TreeCtrl->GetNextSibling(current_keyflame);
    }
}


unsigned long int UIFire::GetTime()
{ 
  wxString time = timeStamp_Edit->GetValue(); 
  wxString aux;
  unsigned long int hours, minutes, seconds;

  aux = time.BeforeFirst(':');
  if (!aux.ToULong(&hours))
    return 0;

  aux = time.BeforeLast(':');
  aux = aux.substr(3, 2);
  if (!aux.ToULong(&minutes))
    return 0;

  aux = time.AfterLast(':');
  if (!aux.ToULong(&seconds))
    return 0;

  return ((hours * 3600) + (minutes * 60) + seconds);
}

void UIFire::SetDate(const wxDateTime &new_date)
{
  std::ostringstream ss1, ss2, ss3;
  std::string st1, st2, st3, tvalue;
	
  int hours = new_date.GetHour();
  int minutes = new_date.GetMinute();
  int seconds = new_date.GetSecond();

  wxString hours_s, minutes_s, seconds_s;

  ss1 << hours;
  ss2 << minutes;
  ss3 << seconds;

  st1 = ss1.str();
  st2 = ss2.str();
  st3 = ss3.str();

  if (st1.size() < 2)
    st1 = "0" + st1;

  if (st2.size() < 2)
    st2 = "0" + st2;

  if (st3.size() < 2)
    st3 = "0" + st3;

  tvalue = st1 + ":" + st2 + ":" + st3;

  timeStamp_Edit->SetValue(wxString(tvalue.c_str(),wxConvUTF8));
  timestamp_calendar->SetValue(new_date);


}

void UIFire::OnPaint(wxPaintEvent& event)
{
  wxPaintDC dc(this);
  render(dc);
}

void UIFire::render(wxDC& dc)
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
      wxPoint box1_first(20-2,10-2);
      wxPoint box1_last(368,82+2);
      //dc.DrawRoundedRectangle(TIMELINE_CORNERX, 221+207-2, TIMELINE_WIDTH, 100,3.1f);
    }

  DrawStaticBox(dc,_T("Position"), wxPoint(20,430), wxSize(112,90));
  DrawStaticBox(dc,_T("Fire Particles"), wxPoint(139,430), wxSize(110,90));
  DrawStaticBox(dc,_T("Smoke Particles"), wxPoint(256,430), wxSize(112,90));
  dc.DrawRotatedText(_T("Fire name:"),20,10, 0);
  dc.DrawRotatedText(_T("Fire texture:"),20,35, 0);
  dc.DrawRotatedText(_T("Smoke texture:"),20,60, 0);
  dc.DrawRotatedText(_T("Time Stamp"),TIMELINE_CORNERX + (TIMELINE_WIDTH/2 - 100)+70,TIMELINE_CORNERY+3+ + TIMELINE_HEIGHT + 10, 0);
  dc.DrawBitmap(m_bmp, TIMELINE_CORNERX, TIMELINE_CORNERY, false);
  dc.DrawRotatedText(_T("Timeline:"),TIMELINE_CORNERX,TIMELINE_CORNERY-13, 0);
  dc.SetBrush((*wxGREEN_BRUSH));

  int minx=TIMELINE_CORNERX+3;
  int timelinesize=TIMELINE_WIDTH-20;
  double pos;

  wxTreeItemId root = Keyflames_TreeCtrl->GetRootItem();
  wxTreeItemIdValue cookie;
  wxTreeItemId child = Keyflames_TreeCtrl->GetFirstChild(root,cookie);

  if (root.IsOk())
    for (int i=0; (unsigned)i < v_date_times.size(); i++)
      {
	if (child.IsOk())
	  {
	    if (dt_max != dt_min) 
	      {
		double ax1 = (v_date_times[i] - dt_min).GetMilliseconds().ToDouble();
		double ax2 = (dt_max - dt_min).GetMilliseconds().ToDouble();
		pos = ax1 / ax2;
	      }
	    else
	      pos = 0;
	    int ax3 = (int)( minx+(pos*timelinesize));
	    if (timelineActivePerimeter == i)
	      dc.SetTextForeground(wxColour(235,194,114));
	    else 
	      dc.SetTextForeground(wxColour(0,0,0));
	    dc.DrawRotatedText(Keyflames_TreeCtrl->GetItemText(child), ax3, TIMELINE_CORNERY+TIMELINE_HEIGHT-12, 90);
	    ax3 += 7;
	    dc.DrawCircle(ax3,TIMELINE_CORNERY+TIMELINE_HEIGHT-7,2);
	    child = Keyflames_TreeCtrl->GetNextSibling(child);
	  }
      }
}

/*void UIFire::OnPaint()
  { 
  }*/

void UIFire::UpdateTimeLine(const std::vector<wxDateTime> date_times)
{
  if(date_times.size() <= 0)
    return;
  wxDateTime dt = date_times[0];
  v_date_times.clear();
  dt_min = dt;
  dt_max = dt;
  for (int i=0; (unsigned)i < date_times.size(); i++)
    {
      dt = date_times[i];
      v_date_times.push_back(dt);
      if (dt < dt_min) dt_min = dt;
      if (dt > dt_max) dt_max = dt;
    }
  Refresh();
  Update();
}

void UIFire::OnMouseLeftDown(wxMouseEvent &event)
{
  int x = event.m_x;
  int y = event.m_y;	
  if ( x > TIMELINE_CORNERX && x < TIMELINE_CORNERX + TIMELINE_WIDTH &&
       y > TIMELINE_CORNERY && y < TIMELINE_CORNERY + TIMELINE_HEIGHT &&
       dt_min.IsValid() && dt_max.IsValid())
    {
      hitx = event.m_x;
      hity = event.m_y;

      float percentmin = (x-10 - (TIMELINE_CORNERX+3.0f))/TIMELINE_ACTSIZE;
      float percentmax = (x+10 - (TIMELINE_CORNERX+3.0f))/TIMELINE_ACTSIZE;
      wxTimeSpan time_slice_size = dt_max - dt_min;

      float offset_f = time_slice_size.GetMilliseconds().ToDouble()*percentmin;
      wxTimeSpan offset_ts = offset_ts.Milliseconds((long)offset_f);
      wxDateTime xtimemin = dt_min + offset_ts;
      offset_f = time_slice_size.GetMilliseconds().ToDouble()*percentmax;
      offset_ts = offset_ts.Milliseconds((long)offset_f);
      wxDateTime xtimemax = dt_min + offset_ts;

      timelineActivePerimeter = fire_controller->GetFirstKeyFlameIdInTimePeriod(xtimemin,xtimemax);
      if (timelineActivePerimeter > -1)
	{
	  SetActive(timelineActivePerimeter,-1);
	}
      changes = true;
    }
  else
    timelineActivePerimeter = -1;
}

void UIFire::OnMouseLeftUp(wxMouseEvent &event)
{
  if (timelineActivePerimeter != -1)
    {
      int x = event.m_x;
      int y = event.m_y;	
		
      if ( x > TIMELINE_CORNERX && x < TIMELINE_CORNERX + TIMELINE_WIDTH &&
	   y > TIMELINE_CORNERY && y < TIMELINE_CORNERY + TIMELINE_HEIGHT &&
	   dt_min.IsValid() && dt_max.IsValid())
	{
	  float percent = (x-hitx)/(float)TIMELINE_ACTSIZE;
	  wxTimeSpan time_slice_size = dt_max - dt_min;
	  float offset_f = time_slice_size.GetMilliseconds().ToDouble()*percent;
	  wxTimeSpan offset_ts = offset_ts.Milliseconds((long)offset_f);
	  wxDateTime xtime = fire_controller->GetKeyFlameTimeStamp(timelineActivePerimeter);
			
	  int prev_keyflame, next_keyflame;
			
	  next_keyflame = (timelineActivePerimeter >= keyflameCounter-1)? keyflameCounter-1 : timelineActivePerimeter+1;
	  prev_keyflame = (timelineActivePerimeter == 0)? 0 : timelineActivePerimeter-1;
	  wxDateTime prev_time = fire_controller->GetKeyFlameTimeStamp(prev_keyflame);
	  wxDateTime next_time = fire_controller->GetKeyFlameTimeStamp(next_keyflame);
	  xtime += offset_ts;
	  xtime.SetMillisecond(0);
			
	  if (xtime.IsValid() && prev_time.IsValid() && next_time.IsValid() && (xtime < next_time) && (xtime > prev_time)) 
	    {
	      fire_controller->SetKeyFlameTimeStamp(timelineActivePerimeter, xtime);
	    }
	  SetActive(timelineActivePerimeter,-1);
	  changes = true;
	}
      else
	timelineActivePerimeter = -1;
    }
}

void UIFire::OnMouseLeftDrag(wxMouseEvent &event)
{
  if (timelineActivePerimeter != -1)
    {
      if (event.LeftIsDown())
	{
	  int x = event.m_x;
	  int y = event.m_y;
	  wxClientDC client_dc(this);
	  client_dc.SetClippingRegion(TIMELINE_CORNERX, TIMELINE_CORNERY, TIMELINE_WIDTH, TIMELINE_HEIGHT);

	  render(client_dc);
	  client_dc.SetPen((*wxWHITE_PEN));
	  client_dc.CrossHair(x,y);
	  changes = true;
	}
    }
}

void UIFire::OnSmokeLifeSpinUp(wxSpinEvent& event)
{
  std::ostringstream wop; 
  double f; 
  Smoke_particles_life_Edit->GetValue().ToDouble(&f);
  f+=0.1f;
  wop << std::fixed << std::setprecision(5) << f;  
  Smoke_particles_life_Edit->SetValue(wxString(wop.str().c_str(),wxConvUTF8)); 
  changes = true;
}

void UIFire::OnSmokeSizeSpinUp(wxSpinEvent& event)
{
  std::ostringstream wop; 
  double f; 
  Smoke_particles_seize_Edit->GetValue().ToDouble(&f);
  f+=1.0f;
  wop << std::fixed << std::setprecision(5) << f;  
  Smoke_particles_seize_Edit->SetValue(wxString(wop.str().c_str(),wxConvUTF8)); 
  changes = true;
}

void UIFire::OnSmokeIntesitySpinUp(wxSpinEvent& event)
{
  std::ostringstream wop; 
  double f; 
  smoke_Intesity_Edit->GetValue().ToDouble(&f);
  f+=1.0f;
  wop << std::fixed << std::setprecision(5) << f;  
  smoke_Intesity_Edit->SetValue(wxString(wop.str().c_str(),wxConvUTF8));
  changes = true;
}

void UIFire::OnFireLifeSpinUp(wxSpinEvent& event)
{
  std::ostringstream wop; 
  double f; 
  Fire_particles_life_Edit->GetValue().ToDouble(&f);
  f+=0.1f;
  wop << std::fixed << std::setprecision(5) << f;  
  Fire_particles_life_Edit->SetValue(wxString(wop.str().c_str(),wxConvUTF8)); 
  changes = true;
}
void UIFire::OnFireSizeSpinUp(wxSpinEvent& event)
{
  std::ostringstream wop; 
  double f; 
  Fire_particles_size_Edit->GetValue().ToDouble(&f);
  f+=1.0f;
  wop << std::fixed << std::setprecision(5) << f;  
  Fire_particles_size_Edit->SetValue(wxString(wop.str().c_str(),wxConvUTF8)); 
  changes = true;
}

void UIFire::OnFireIntensitySpinUp(wxSpinEvent& event)
{
  std::ostringstream wop; 
  double f; 
  Fire_Intesity_Edit->GetValue().ToDouble(&f);
  f+=1.0f;
  wop << std::fixed << std::setprecision(5) << f;  
  Fire_Intesity_Edit->SetValue(wxString(wop.str().c_str(),wxConvUTF8)); 
  changes = true;
}

void UIFire::OnSmokeLifeSpinDown(wxSpinEvent& event)
{
  std::ostringstream wop; 
  double f; 
  Smoke_particles_life_Edit->GetValue().ToDouble(&f);
  f-=0.1f;
  wop << std::fixed << std::setprecision(5) << f;  
  Smoke_particles_life_Edit->SetValue(wxString(wop.str().c_str(),wxConvUTF8)); 
  changes = true;
}
void UIFire::OnSmokeSizeSpinDown(wxSpinEvent& event)
{
  std::ostringstream wop; 
  double f; 
  Smoke_particles_seize_Edit->GetValue().ToDouble(&f);
  f-=1.0f;
  wop << std::fixed << std::setprecision(5) << f;  
  Smoke_particles_seize_Edit->SetValue(wxString(wop.str().c_str(),wxConvUTF8)); 
  changes = true;
}

void UIFire::OnSmokeIntesitySpinDown(wxSpinEvent& event)
{
  std::ostringstream wop; 
  double f; 
  smoke_Intesity_Edit->GetValue().ToDouble(&f);
  f-=1.0f;
  wop << std::fixed << std::setprecision(5) << f;  
  smoke_Intesity_Edit->SetValue(wxString(wop.str().c_str(),wxConvUTF8)); 
  changes = true;
}

void UIFire::OnFireLifeSpinDown(wxSpinEvent& event)
{
  std::ostringstream wop; 
  double f; 
  Fire_particles_life_Edit->GetValue().ToDouble(&f);
  f-=0.1f;
  wop << std::fixed << std::setprecision(5) << f;  
  Fire_particles_life_Edit->SetValue(wxString(wop.str().c_str(),wxConvUTF8)); 
  changes = true;
}
void UIFire::OnFireSizeSpinDown(wxSpinEvent& event)
{
  std::ostringstream wop; 
  double f; 
  Fire_particles_size_Edit->GetValue().ToDouble(&f);
  f-=1.0f;
  wop << std::fixed << std::setprecision(5) << f;  
  Fire_particles_size_Edit->SetValue(wxString(wop.str().c_str(),wxConvUTF8)); 
  changes = true;
}
void UIFire::OnFireIntensitySpinDown(wxSpinEvent& event)
{
  std::ostringstream wop; 
  double f; 
  Fire_Intesity_Edit->GetValue().ToDouble(&f);
  f-=1.0f;
  wop << std::fixed << std::setprecision(5) << f;  
  Fire_Intesity_Edit->SetValue(wxString(wop.str().c_str(),wxConvUTF8)); 
  changes = true;
}

void UIFire::OnPositionXSpinUp(wxSpinEvent& event)
{
  std::ostringstream wop; 
  double f; 
  Position_x_Edit->GetValue().ToDouble(&f);
  f+=10.0f;
  wop << std::fixed << std::setprecision(5) << f;  
  Position_x_Edit->SetValue(wxString(wop.str().c_str(),wxConvUTF8));
  changes = true;
}
void UIFire::OnPositionYSpinUp(wxSpinEvent& event)
{
  std::ostringstream wop; 
  double f; 
  Position_y_Edit->GetValue().ToDouble(&f);
  f+=10.0f;
  wop << std::fixed << std::setprecision(5) << f;  
  Position_y_Edit->SetValue(wxString(wop.str().c_str(),wxConvUTF8));
  changes = true;
}

void UIFire::OnPositionZSpinUp(wxSpinEvent& event)
{
  std::ostringstream wop; 
  double f; 
  Position_z_Edit->GetValue().ToDouble(&f);
  f+=10.0f;
  wop << std::fixed << std::setprecision(5) << f;  
  Position_z_Edit->SetValue(wxString(wop.str().c_str(),wxConvUTF8)); 
  changes = true;
}

void UIFire::OnPositionXSpinDown(wxSpinEvent& event)
{
  std::ostringstream wop; 
  double f; 
  Position_x_Edit->GetValue().ToDouble(&f);
  f-=10.0f;
  wop << std::fixed << std::setprecision(5) << f;  
  Position_x_Edit->SetValue(wxString(wop.str().c_str(),wxConvUTF8)); 
  changes = true;
}

void UIFire::OnPositionYSpinDown(wxSpinEvent& event)
{
  std::ostringstream wop; 
  double f; 
  Position_y_Edit->GetValue().ToDouble(&f);
  f-=10.0f;
  wop << std::fixed << std::setprecision(5) << f;  
  Position_y_Edit->SetValue(wxString(wop.str().c_str(),wxConvUTF8)); 
  changes = true;
}

void UIFire::OnPositionZSpinDown(wxSpinEvent& event)
{
  std::ostringstream wop; 
  double f; 
  Position_z_Edit->GetValue().ToDouble(&f);
  f-=10.0f;
  wop << std::fixed << std::setprecision(5) << f;  
  Position_z_Edit->SetValue(wxString(wop.str().c_str(),wxConvUTF8)); 
  changes = true;
}

void UIFire::OnHourSpinUp(wxSpinEvent& event)
{
  timeStamp_Edit->UpdateTextCtrl(POSITIVE);
  changes = true;
}
void UIFire::OnHourSpinDown(wxSpinEvent& event)
{
  timeStamp_Edit->UpdateTextCtrl(NEGATIVE);
  changes = true;
}

int UIFire::GetActiveIgnitionPoint()
{
  return WxListBox1->GetSelection();
}

void UIFire::OnEraseBackground(wxEraseEvent& event)
{
}

void UIFire::DrawStaticBox(wxDC& dc,const wxString& label, const wxPoint& pos, const wxSize& size)
{
  wxColour c_pen = dc.GetPen().GetColour();
  dc.SetPen(wxPen(dc.GetTextForeground()));
  dc.DrawRoundedRectangle(pos.x, pos.y, size.x, size.y,3.1f);
  dc.SetPen(wxPen(c_pen));
  dc.DrawRotatedText(label,pos.x+10,pos.y,0);
}

