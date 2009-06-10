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

#ifndef __UISPLITTERCANVAS__
#define __UISPLITTERCANVAS__

#include <vector>
#include <wx/wx.h>
#include <wx/splitter.h>

#include <cpw/graphic/INavigatorManager.h>
#include <cpw/ApplicationConfiguration.h>

namespace cpw 
{

	namespace gui
	{
		/** 
			\brief Splitter Windows for multiviewing OpenGL canvas
			\ingroup cpw
		*/
		class UISplitterCanvas
		{
			public:

				 UISplitterCanvas(wxSplitterWindow* parent, INavigatorManager *navigator_manager);
				~UISplitterCanvas(void);

				wxSplitterWindow *CreateCanvasField(wxWindow* parent, wxWindowID id, const wxPoint& point = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style= wxSP_3D, const wxString& name = (const wxChar*)"splitterCanvasManager");
				wxSplitterWindow *CreateCanvas(wxWindow* parent, wxWindowID id, wxWindow **canvas1, INavigatorManager *navigator_manager, int viewID, const wxPoint& point = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style= wxSP_3D, const wxString& name = (const wxChar*)"splitterCanvas");

				void SetHorizontalRelationship(wxSplitterWindow **field, wxSplitterWindow *top, wxSplitterWindow *bottom);
				void SetVerticalRelationship(wxSplitterWindow **field, wxSplitterWindow *left, wxSplitterWindow *right);
				
				//Must be used in order node->root
				void SetInitializeCanvas(wxSplitterWindow **splitterCanvas, wxWindow* canvas);

				wxSplitterWindow *PresetOnlyView();
				wxSplitterWindow *Preset2ViewsHorizontally();
				wxSplitterWindow *Preset2ViewsVertically();
				wxSplitterWindow *Preset3Views1Top2Bottom();

				wxSplitterWindow *Preset4ViewsA(wxWindowID id, const wxPoint& point = wxDefaultPosition, 
											  const wxSize& size = wxDefaultSize, long style= wxSP_3D, 
											  const wxString& name = (const wxChar*)"splitterCanvasManager");
				wxSplitterWindow *Preset4ViewsB(wxWindowID id, const wxPoint& point = wxDefaultPosition, 
											  const wxSize& size = wxDefaultSize, long style= wxSP_3D, 
											  const wxString& name = (const wxChar*)"splitterCanvasManager");
				wxSplitterWindow *Preset4ViewsC(wxWindowID id, const wxPoint& point = wxDefaultPosition, 
											  const wxSize& size = wxDefaultSize, long style= wxSP_3D, 
											  const wxString& name = (const wxChar*)"splitterCanvasManager");
				wxSplitterWindow *Preset4ViewsD(wxWindowID id, const wxPoint& point = wxDefaultPosition, 
											  const wxSize& size = wxDefaultSize, long style= wxSP_3D, 
											  const wxString& name = (const wxChar*)"splitterCanvasManager");
				wxSplitterWindow *Preset4ViewsE(wxWindowID id, const wxPoint& point = wxDefaultPosition, 
											  const wxSize& size = wxDefaultSize, long style= wxSP_3D, 
											  const wxString& name = (const wxChar*)"splitterCanvasManager");
				wxSplitterWindow *Preset3ViewsA(wxWindowID id, const wxPoint& point = wxDefaultPosition, 
											  const wxSize& size = wxDefaultSize, long style= wxSP_3D, 
											  const wxString& name = (const wxChar*)"splitterCanvasManager");
				wxSplitterWindow *Preset3ViewsB(wxWindowID id, const wxPoint& point = wxDefaultPosition, 
											  const wxSize& size = wxDefaultSize, long style= wxSP_3D, 
											  const wxString& name = (const wxChar*)"splitterCanvasManager");
				wxSplitterWindow *Preset3ViewsC(wxWindowID id, const wxPoint& point = wxDefaultPosition, 
											  const wxSize& size = wxDefaultSize, long style= wxSP_3D, 
											  const wxString& name = (const wxChar*)"splitterCanvasManager");
				wxSplitterWindow *Preset3ViewsD(wxWindowID id, const wxPoint& point = wxDefaultPosition, 
											  const wxSize& size = wxDefaultSize, long style= wxSP_3D, 
											  const wxString& name = (const wxChar*)"splitterCanvasManager");
				wxSplitterWindow *Preset2ViewsH(wxWindowID id, const wxPoint& point = wxDefaultPosition, 
											  const wxSize& size = wxDefaultSize, long style= wxSP_3D, 
											  const wxString& name = (const wxChar*)"splitterCanvasManager");
				wxSplitterWindow *Preset2ViewsV(wxWindowID id, const wxPoint& point = wxDefaultPosition, 
											  const wxSize& size = wxDefaultSize, long style= wxSP_3D, 
											  const wxString& name = (const wxChar*)"splitterCanvasManager");
				wxSplitterWindow *PresetOnlyView(wxWindowID id, const wxPoint& point = wxDefaultPosition, 
											  const wxSize& size = wxDefaultSize, long style= wxSP_3D, 
											  const wxString& name = (const wxChar*)"splitterCanvasManager");

				void TunePresetSashDivisions();
				void Obliviate();
				void Replace(wxSplitterWindow *splitter_new);
				void Paint();
				void UpdateSizes();

			private:

				wxSplitterWindow *main_field, *old_field;
				wxSplitterWindow *mainParent_;
				std::vector< wxSplitterWindow* > subsplitters;
				INavigatorManager *navigator_manager_;
				int active_navigators, old_navigators;

				void ClearSubsplitters();

		};
	}
}

#endif
