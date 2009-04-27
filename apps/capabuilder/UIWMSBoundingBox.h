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

#ifndef _UIWMSBOUNDINGBOX_
#define _UIWMSBOUNDINGBOX_

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


#include "WMSBoundingBoxController.h"

#undef UIWMSBB_STYLE
#define UIWMSBB_STYLE wxCAPTION | wxSYSTEM_MENU | wxDIALOG_NO_PARENT | wxCLOSE_BOX

class WMSBoundingBoxController;

class UIWMSBoundingBox : public wxDialog
{
	public:
		
		UIWMSBoundingBox(WMSBoundingBoxController *_wms_cont,wxWindow *parent, wxWindowID id = 1, const wxString &title = wxT("BoundingBox"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = UIWMSBB_STYLE);
		~UIWMSBoundingBox(void);

		void SetLatMin(std::string& lat){ top_lat_left_text_ctrl->SetValue(lat);}
		void SetLonMin(std::string& lon){ top_lon_left_text_ctrl->SetValue(lon);}

		void SetLatMax(std::string& lat){ bottom_lat_right_text_ctrl->SetValue(lat);}
		void SetLonMax(std::string& lon){ bottom_lon_right_text_ctrl->SetValue(lon);}

		void SetURL(std::string &_url) {url->SetValue(_url);}
		std::string GetURL() {return std::string(url->GetValue());}

		void SetBBWidth(std::string& bbw){ width_text_ctrl->SetValue(bbw);}
		void SetBBHeight(std::string& bbh){ height_text_ctrl->SetValue(bbh);}


	private:

		void InitGUI();
		
		void OnClose(wxCloseEvent& event);
		void OnButtonOK(wxCommandEvent& WXUNUSED(event));
		void OnButtonCancel(wxCommandEvent& WXUNUSED(event));

		void OnButtonPreview(wxCommandEvent& WXUNUSED(event));
		void OnButtonPreviewURL(wxCommandEvent& WXUNUSED(event));

		void CheckWidthHeight(wxCommandEvent& WXUNUSED(event));

		bool CheckCoords();

		WMSBoundingBoxController *wms_cont;
		wxButton *button_ok, *button_cancel, *button_preview, *button_previewurl;
		wxStaticText *top_lat_left_text, *top_lon_left_text, *bottom_lat_right_text, *bottom_lon_right_text, *url_text, *width_text, *height_text;  
		wxTextCtrl *top_lat_left_text_ctrl, *top_lon_left_text_ctrl, *bottom_lat_right_text_ctrl, *bottom_lon_right_text_ctrl, *url, *width_text_ctrl, *height_text_ctrl;  

		DECLARE_EVENT_TABLE()
};

#endif