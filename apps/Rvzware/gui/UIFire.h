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

#ifndef __UIFIRE_h__
#define __UIFIRE_h__

#include <wx/wx.h>
#include <wx/dialog.h>

#include <wx/listbox.h>
#include <wx/button.h>
#include <wx/textctrl.h>
#include <wx/stattext.h>
#include <wx/notebook.h>
#include <wx/image.h>
#include <wx/treectrl.h>
#include <wx/spinbutt.h>
#include <wx/calctrl.h>
#include <wx/datectrl.h>
#include <wx/string.h>

#include <iostream>
#include <iomanip>

#include <cpw/common/Attribute.h>
#include <cpw/common/types.h>
#include <cpw/entity/Entity.h>
#include <controllers/FireController.h>

#include <gui/UIEAttributes.h>
#include <gui/MyTimeCtrl.h>

#undef UIFire_STYLE
#define UIFire_STYLE wxCAPTION | wxSYSTEM_MENU | wxDIALOG_NO_PARENT | wxCLOSE_BOX

namespace cpw 
{ 
	namespace controllers {class FireController; }

	namespace gui
	{


		/** 
			\brief Window panel for creating Fire elements
			\ingroup cwcontent
		*/
		class UIFire : public wxDialog
		{
			private:
				DECLARE_EVENT_TABLE();
				
			public:
				UIFire(controllers::FireController *fc,wxWindow *parent, wxWindowID id = 1, const wxString &title = wxT("Fire"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = UIFire_STYLE);
				void SetFireTexture(std::string &_fire_texture)   { Fire_texture_Ed->SetValue((wxString&)_fire_texture); fire_texture = _fire_texture;}
				void SetSmokeTexture(std::string &_smoke_texture) { Smoke_texture_Button->SetValue((wxString&)_smoke_texture); smoke_texture = _smoke_texture;}
				void AddFlame();
				virtual ~UIFire();

				wxListBox * GetIgnitionPointsListBox(){ return WxListBox1; }

				std::string &GetFireTexture() { return fire_texture; }
				std::string &GetSmokeTexture() { return smoke_texture; }

				void SetCallUpdate(const bool &_cupdate) { update_on_changes = _cupdate; }
				void SetDate(const wxDateTime &new_date);

				bool GetCoordsFromEditControls(cpw::Point3d<float> &position, float &smoke_life, float &smoke_size, float &fire_life, float &fire_size, float &fire_intensity, float &smoke_intensity);

				void SetPositionX(std::string &position_x) { std::stringstream aux; aux.precision(15); aux << position_x; 
                                                                             std::string tmp = aux.str();Position_x_Edit->SetValue((wxString&)tmp); }
				void SetPositionY(std::string &position_y) { std::stringstream aux; aux.precision(15); aux << position_y; 
                                                                             std::string tmp = aux.str();Position_y_Edit->SetValue((wxString&)tmp); }
				void SetPositionZ(std::string &position_z) { std::stringstream aux; aux.precision(15); aux << position_z; 
                                                                             std::string tmp = aux.str();Position_z_Edit->SetValue((wxString&)tmp); }
				void SetSmokeLife(std::string &smoke_life) { Smoke_particles_life_Edit->SetValue((wxString&)smoke_life);  }
				void SetSmokeSize(std::string &smoke_size) { Smoke_particles_seize_Edit->SetValue((wxString&)smoke_size); }
				void SetFireLife(std::string &fire_life)   { Fire_particles_life_Edit->SetValue((wxString&)fire_life);    }
				void SetFireSize(std::string &fire_size)   { Fire_particles_size_Edit->SetValue((wxString&)fire_size);    }
				void SetFireIntensity(std::string &intensity)  { Fire_Intesity_Edit->SetValue((wxString&)intensity);               }
				void SetSmokeIntensity(std::string &intensity)  { smoke_Intesity_Edit->SetValue((wxString&)intensity);               }

				void SetFireName(std::string &name)		   { Fire_name_Ed->SetLabel((wxString&)( name));                 }

				void DisableEditControls();
				void EnableEditControls();

				void AddIcon(const std::string url, wxTreeItemId id);
				void Clone_TreeCtrl_Childrens(wxTreeItemId from_item,wxTreeItemId to_item);

				int  GetActiveNotePage();
				int  GetActiveKeyFlame();
				int  GetActiveFlame();
				int  GetActiveIgnitionPoint();
				void SetActive(int perimeter, int flame);
				unsigned long int GetTime(); 

				void AddPerimeter(const bool &new_p=false);
				void AddFlame(int perimeter);
				void CheckFlamesValuesInt();

				void UpdateTimeLine(const std::vector<wxDateTime> date_times);
				void UpdateOnChanges();

				void Collapse();

				void Clear();

				void Modify(const bool &value) {modify = value; Fire_name_Ed->Enable(false); }

				void SetChanges(const bool &_changes) { changes = _changes; }
				bool GetChanges() { return changes; }
			
			private:

				bool modify;
				
				wxTextCtrl *smoke_Intesity_Edit;
				wxTextCtrl *Fire_Intesity_Edit;
				wxTextCtrl *Smoke_Intesity_Edit;
				wxDatePickerCtrl *timestamp_calendar;
				MyTimeCtrl *timeStamp_Edit;
				wxImageList *img_list;
				wxNotebook *note_panel;
				wxTreeCtrl *Keyflames_TreeCtrl;
				wxBitmapButton *Add_KeyFlame_Button;
				wxBitmapButton *Quit_Flame_Button;
				wxStaticBox *Smoke_particles_StaticBox;
				wxStaticBox *Fire_particles_StaticBox;
				wxStaticBox *Position_StaticBox;
				wxTextCtrl *Intesity_Edit;
				wxStaticText *Intensity_StaticText;
				wxTextCtrl *Smoke_particles_seize_Edit;
				wxTextCtrl *Smoke_particles_life_Edit;
				wxTextCtrl *Fire_particles_size_Edit;
				wxStaticText *Smoke_particles_size_StaticText;
				wxStaticText *Fire_particles_size_StaticText;
				wxTextCtrl *Fire_particles_life_Edit;
				wxStaticText *Smoke_particles_life_StaticText;
				wxStaticText *Fire_particles_life_StaticText;
				wxTextCtrl *Position_z_Edit;
				wxStaticText *Position_z_StaticText;
				wxTextCtrl *Position_y_Edit;
				wxStaticText *Position_y_StaticText;
				wxStaticText *Position_x_StaticText;
				wxTextCtrl *Position_x_Edit;
				wxListBox *WxListBox1;
				wxButton *Load_smoke_texture_button;
				wxTextCtrl *Smoke_texture_Button;
				wxButton *Load_fire_texture_button;
				wxTextCtrl *Fire_texture_Ed;
				wxButton *Finish_button;
				wxButton *Cancel_button;
				wxTextCtrl *Fire_name_Ed;
				
			private:
				enum
				{
					ID_SPIN_HOUR = 1095,
					ID_SPIN_POS_X = 1094,
					ID_SPIN_POS_Y = 1093,
					ID_SPIN_POS_Z = 1092,
					ID_SPIN_FIRE_INTENSITY = 1091,
					ID_SPIN_FIRE_SIZE = 1090,
					ID_SPIN_FIRE_LIFE = 1089,
					ID_SPIN_SMOKE_INTENSITY = 1088,
					ID_SPIN_SMOKE_SIZE = 1087,
					ID_SPIN_SMOKE_LIFE = 1086,
					ID_FIRE_INTENSITY_STATICTEXT = 1085,
					ID_SMOKE_INTENSITY_STATICTEXT = 1084,
					ID_TIMESTAMP_CALENDAR = 1083,
					ID_TIMESTAMP_EDIT = 1081,
					ID_ADDKEYFLAME_BUTTON = 1079,
					ID_KEYFLAMES_TREECTRL = 1078,
					ID_NOTE_PANEL = 1077,
					ID_EDIT_FLAMES = 1076,
					ID_QUITFLAMEBUTTON = 1072,
					ID_SMOKEPARTICLESLIFE_STATICTEXT = 1070,
					ID_SMOKEPARTICLESSIZE_STATICTEXT = 1069,
					ID_SMOKEPARTICLES_STATICBOX = 1068,
					ID_FIREPARTICLES_STATICBOX = 1067,
					ID_POSITION_STATICBOX = 1065,
					ID_FIREPARTICLESSIZE_STATICTEXT = 1052,
					ID_FIREPARTICLESLIFE_STATICTEXT = 1050,
					ID_POSITIONZ_STATICTEXT = 1019,
					ID_POSITIONY_STATICTEXT = 1016,
					ID_Position_X_STATICTEXT = 1015,
					ID_WXLISTBOX1 = 1011,
					ID_LOADSMOKETEXTURE_BUTTON = 1010,
					ID_SMOKETEXTURE_EDIT = 1009,
					ID_LOADFIRETEXURE_BUTTON = 1007,
					ID_FIRETEXTURE_EDIT = 1006,
					ID_FINISH_BUTTON = 1004,
					ID_CANCEL_BUTTON = 1003,
					ID_FIRENAME_EDIT = 1002,
					ID_DUMMY_VALUE_ //don't remove this value unless you have other enum values
				};
			
			private:
				
				bool update_on_changes, changes;
				bool m_isDragging;
				double 	splife, spseize, spintensity, fpsize, fplife, fpintensity, posx, posy, posz;

				void CreateGUIControls();

				wxWindow *paren;
				wxBitmap m_bmp;
				wxDateTime dt_min, dt_max;
				std::vector<wxDateTime> v_date_times;
				controllers::FireController *fire_controller;
				int keyflameCounter;
				int hitx,hity;
				int timelineActivePerimeter;

				void Close();

				void OnButtonLoadFireTexture(wxCommandEvent& WXUNUSED(event));
				void OnButtonLoadSmokeTexture(wxCommandEvent& WXUNUSED(event));
				void OnClose(wxCloseEvent& event);
				void OnMove(wxMoveEvent& event);
				void OnButtonCancel(wxCommandEvent& WXUNUSED(event));
				void OnButtonFinish(wxCommandEvent& WXUNUSED(event));
				
				void OnButtonAddFlame(wxCommandEvent& WXUNUSED(event));
				void OnButtonAddKeyFlame(wxCommandEvent& WXUNUSED(event));
				void OnButtonQuitFlame(wxCommandEvent& WXUNUSED(event));
				void OnListBoxSelectItem(wxCommandEvent& WXUNUSED(event));
				void CheckFlamesValues(wxCommandEvent& WXUNUSED(event));
				void OnTreeSelectionChanged(wxTreeEvent& wxevent);
				void OnDateChange(wxDateEvent& WXUNUSED(event));
				void OnItemExpanded(wxTreeEvent& wxevent);
				void OnItemCollapsed(wxTreeEvent& wxevent);
				void OnHourChanges(wxCommandEvent& WXUNUSED(event));
				void OnPaint(wxPaintEvent& event);
				//				void OnPaint();
				void OnMouseLeftDrag(wxMouseEvent &event);
				void OnMouseLeftDown(wxMouseEvent &event);
				void OnMouseLeftUp(wxMouseEvent &event);
				void OnSmokeLifeSpinUp(wxSpinEvent& event);

				void OnSmokeSizeSpinUp(wxSpinEvent& event);
				void OnFireLifeSpinUp(wxSpinEvent& event);
				void OnFireSizeSpinUp(wxSpinEvent& event);
				void OnSmokeIntesitySpinUp(wxSpinEvent& event);
				void OnFireIntensitySpinUp(wxSpinEvent& event);

				void OnSmokeLifeSpinDown(wxSpinEvent& event);
				void OnSmokeSizeSpinDown(wxSpinEvent& event);
				void OnFireLifeSpinDown(wxSpinEvent& event);
				void OnFireSizeSpinDown(wxSpinEvent& event);
				void OnSmokeIntesitySpinDown(wxSpinEvent& event);
				void OnFireIntensitySpinDown(wxSpinEvent& event);

				void OnPositionXSpinUp(wxSpinEvent& event);
				void OnPositionYSpinUp(wxSpinEvent& event);
				void OnPositionZSpinUp(wxSpinEvent& event);
				void OnPositionXSpinDown(wxSpinEvent& event);
				void OnPositionYSpinDown(wxSpinEvent& event);
				void OnPositionZSpinDown(wxSpinEvent& event);

				void OnHourSpinUp(wxSpinEvent& event);
				void OnHourSpinDown(wxSpinEvent& event);
				void OnNotePageChanged(wxNotebookEvent& event);

				void OnTexturesChanges(wxCommandEvent& WXUNUSED(event));
				void OnEraseBackground(wxEraseEvent& event);

				void render(wxDC& dc);
				void DrawStaticBox(wxDC& dc,const wxString& label, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize);
				//void Update();

				void EnablePropertiesControls();
				void DisablePropertiesControls();		
				void EnableParticleControls();
				void DisableParticleControls();

				std::string smoke_texture, fire_texture;
				std::vector<bool> collapsed_keyflames;

				void CollapseAllKeyflamesExceptOne(wxTreeItemId &exception_id);
		};
	}
}

#endif
