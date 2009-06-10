
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

#ifndef __UI_ANIMATION_SCHEME__
#define __UI_ANIMATION_SCHEME__

#include <sstream>
#include <string>
#include <vector>
#include <cpw/entity/Entity.h>

#include <wx/wx.h>
#include <wx/datetime.h>
#include <wx/slider.h>
#include <wx/textctrl.h>
#include <wx/spinbutt.h>

#include <gui/GUItypes.h>

#include <cpw/ApplicationConfiguration.h>
#include <ApplicationTime.h>

#include <cpw/common/Observer.h>
#include <cpw/entity/EntityRegistry.h>

namespace cpw 
{ 
  namespace gui
  {

#define N_GRID_WIRES 20
#define INIT_COL 5
#define INIT_ROW 30

    //ANIMATED BOX
    /** 
	\brief Class for creating and drawing boxes inside a Movie Scheme
	\ingroup cpw
    */
    class AnimatedBox
    {
    public: 
			
      AnimatedBox(int row, const wxString &label, const wxDateTime &startt_local, const wxDateTime &endt_local);
      AnimatedBox(const wxString &label, const wxDateTime &startt_local, const wxDateTime &endt_local, const wxDateTime &startt_global, const wxDateTime &endt_global, int zoom = 1);
      AnimatedBox(const wxPoint &pos, const wxSize &size, const wxString &label, const wxDateTime &startt, const wxDateTime &endt, int zoom = 1);
      ~AnimatedBox();
			
      void SetPos			(const wxPoint &pos__)		{pos_    = pos__;  };
      void SetSize		(const wxSize &size__)		{size_   = size__; };
      void SetLabel		(const wxString &label__)	{label_  = label__;};
      void SetStartTime	(const wxDateTime &start__) {startt_ = start__;};
      void SetEndtTime	(const wxDateTime &endt__)	{endt_   = endt__; };
      int  GetVerticalOffset (              )         {return vertical_offset; };
      void SetVerticalOffset (const int &vof)         {vertical_offset = vof;  };

      void CalculateRelativeProperties(wxPoint virtualCanvas_pos, int zoom, wxDateTime global_start_time, wxDateTime global_end_time);
      void Draw(wxDC &dc);
      void AdjustTimeToWidth();

    private:

      int vertical_offset, row_;
      /** Position of the view window in seconds */
      unsigned long virtual_space_pos_;	
      /** Size of the view window in seconds */
      int			  virtual_space_size;	
      /** Current position in the view window in seconds */
      wxPoint pos_;						
      /** Size of the bar */
      wxSize size_;						
      /** Name of the entity */
      wxString label_;					
      /** Formated starting time */
      wxDateTime startt_;					
      /** formated ending time*/
      wxDateTime endt_;

    };
		
    //MOVIE SCHEME
    /** 
	\brief Class that manages and shows animated entities as a Movie Scheme Window
	\ingroup cpw
    */
    class UIMovieScheme : public wxWindow,  public cpw::Observer//,  public wxWindow
      {
      public:

	UIMovieScheme(wxWindow *parent, wxWindowID id = -1, const wxPoint& pos = wxDefaultPosition, 
		      const wxSize& size = wxDefaultSize, 
		      long style = 0 );
	~UIMovieScheme();

	void Init();
	void Fill();
	void Clear();
	void UpdatevirtualCanvas();
	void UpdateList();
	void AdjustTimeToWidth();
	void GoToCurrent();
	virtual void Update(bool subject_deleted = false);
			

      private:

#define SB_THUMB_SIZE 50
#define SB_RANGE 1000
#define SB_PAGE_SIZE 500
#define ROW_SIZE 30
#define BOX_LINE_HEIGHT 30
#define BOX_SCHEME_VALIGNMENT 42

	wxPoint m_pos;		//for mouse position
	wxPoint m_mov;		//mouse offsets

	double slider_relative_pos;
	int zoom, n_entities;

	wxPoint virtualSpace_pos;		//sec
	wxSize  virtualSpace_size;		//endtime-starttime 
	wxPoint virtualCanvas_pos;		//offset of view (sec)
	wxSize  virtualCanvas_size;		//view width

	wxDateTime current_time;
	wxDateTime start_time;
	wxDateTime end_time;
	wxMemoryDC virtualCanvas_memoryDC;

	wxBitmapButton *zoomIn_Button;
	wxBitmapButton *zoomOut_Button;
	wxBitmapButton *adjust_Button;
	wxBitmapButton *gotocurrent_Button;
	wxScrollBar *scrollBarH;
	wxScrollBar *scrollBarV;
	wxTextCtrl *zoomEdit;

	AnimatedBox *testBox;
	std::vector<AnimatedBox*> AnimatedBoxes;

	enum
	{
	  ID_GOTO_CURRENT = 1008,
	  ID_ADJUST_TIME_TO_WIDTH = 1007,
	  ID_SPIN_ZOOM = 1006,
	  ID_ZOOM_EDIT = 1005,
	  ID_SBV = 1004,
	  ID_SBH = 1003,
	  ID_MAGNIFIER_ZOOM_OUT = 1002,
	  ID_MAGNIFIER_ZOOM_IN  = 1001,
	  ID_TIME_SLIDER = 1000,
	  ID_DUMMY_VALUE_ //don't remove this value unless you have other enum values
	};

	void render(wxDC& dc);
	/** 
	    \brief Swaps render buffer
	*/
	void Flush();
	void InitGUI();
	void SetTimeFromSliderPos();
	void UpdateScrollBars();
	void UpdateZoom();
			
	void OnZoomIn(wxCommandEvent& WXUNUSED(event));
	void OnZoomOut(wxCommandEvent& WXUNUSED(event));
	void OnPaint(wxPaintEvent& event);
	void OnSize(wxSizeEvent& event);
	void OnMouseMove(wxMouseEvent& event);
	void OnMouseLeftDown(wxMouseEvent& event);
	void OnVScrollMove(wxScrollEvent& event);
	void OnHScrollMove(wxScrollEvent& event);
	void OnZoomEditChanges(wxCommandEvent& WXUNUSED(event));
	void OnZoomSpinDown(wxSpinEvent& event);
	void OnZoomSpinUp(wxSpinEvent& event);
	void OnEraseBackground( wxEraseEvent& event );
	void OnAdjust(wxCommandEvent& WXUNUSED(event));
	void OnGoToCurrent(wxCommandEvent& WXUNUSED(event));
	void OnHScrollLineUp(wxScrollEvent& event);
	void OnHScrollLineDown(wxScrollEvent& event);

	DECLARE_EVENT_TABLE()
	  };
  }
}

#endif
