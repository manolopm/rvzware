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

#ifndef _NAVIGATORWXGLCANVAS_
#define _NAVIGATORWXGLCANVAS_

#include <list>

#include <wx/glcanvas.h>
#include <wx/wx.h>
#include <osgViewer/Viewer>

#include <iosg/OsgNavigator.h>

#include <cpw/callback/IEventCallBack.h>
#include <cpw/callback/IEventReceiver.h>

//#include <../geviemer/Application.h>
//#include <../cpw::controllers/include/Application.h>
//#include <cpw::controllers/include/Application.h>

#include <iosg/Export.h>


namespace cpw 
{ 
	namespace iosg 
	{
		class OsgNavigator;

		/** 
			\brief Por comentar ....
			\ingroup iosg
			\todo Intentar quitar referencia al Appliaction
		*/
		class IOSGEXPORT OsgNavigatorWxGLCanvas :	public wxGLCanvas, public osgViewer::GraphicsWindow
		{
			public:
				OsgNavigatorWxGLCanvas(wxWindow *parent, wxWindowID id = wxID_ANY,
									const wxPoint& pos = wxDefaultPosition,
									const wxSize& size = wxDefaultSize,
									int *param = 0, long style = wxSUNKEN_BORDER,
									const wxString& name = wxT("GeviemerGLCanvas"));
				~OsgNavigatorWxGLCanvas(void);

				void init();

				void SetViewer(osgViewer::Viewer* _osg_viewer){osg_viewer = _osg_viewer;}

				//callback on mouse move
				void SetCallBackOnMouseEvent(cpw::IEventCallBack *_event_callback) { mouse_event_callback = _event_callback; }
				void ClearMouseEventCallBack() { mouse_event_callback = NULL; }

				void SetNavigator(OsgNavigator *_navigator) { navigator = _navigator; }
				
				void OnPaint(wxPaintEvent& event);
				void OnSize(wxSizeEvent& event);
				void OnKeyDown(wxKeyEvent &event);
				void OnKeyUp(wxKeyEvent &event);

				//mouse events
				void OnMouse(wxMouseEvent &event);
				void OnMouseWheel(wxMouseEvent &event);
				void OnMouseRightDown(wxMouseEvent &event);
				void OnMouseLeftDown(wxMouseEvent &event);
				void OnMouseMotion(wxMouseEvent &event);
				void OnMouseLDoubleClick(wxMouseEvent &event);
				void OnMouseEnterWindow(wxMouseEvent &event);
				void OnMouseLeaveWindow(wxMouseEvent &event);

				void grabFocus();
				void grabFocusIfPointerInWindow();
				void useCursor(bool cursorOn);
				bool makeCurrentImplementation();
				void swapBuffersImplementation();
				virtual bool valid() const { return true; }
				virtual bool realizeImplementation() { return true; }
				virtual bool isRealizedImplementation() const  { return true; }
				virtual void closeImplementation() {}
				virtual bool releaseContextImplementation() { return true; }
				void OnEraseBackground(wxEraseEvent& event);

				void GetWindowSize(int &width, int &height);

				void AddFirstEventReceiver(cpw::IEventReceiver *ev) { event_receivers.push_front(ev); }
				void AddLastEventReceiver(cpw::IEventReceiver *ev) { event_receivers.push_back(ev); }
				bool AddEventReceiverInPosition(const unsigned int &position, cpw::IEventReceiver *ev);
				virtual unsigned int GetEventReceiversCount(){ return event_receivers.size(); }

				bool RemoveEventReceiver(cpw::IEventReceiver *ev);

				void SetNavigatorFocus();

				void SetDefaultPath(const std::string &url) {default_directory = url;}

				void Clear();

				void SetKeepMouseInsideCanvas(const bool &_keep_mouse_inside_canvas) { keep_mouse_inside_canvas = _keep_mouse_inside_canvas; }
				bool GetKeepMouseInsideCanvas() { return keep_mouse_inside_canvas; }
			
			private:
				
				std::list<cpw::IEventReceiver *> event_receivers;
				OsgNavigator *navigator;
				cpw::IEventCallBack *mouse_event_callback;
				wxCursor _oldCursor;
				osgViewer::Viewer *osg_viewer;
				bool rotate_view_left, rotate_view_right, return_to_start_drag, keep_mouse_inside_canvas;
				
				bool coords_flag, dragging;
				int last_x, last_y, start_drag_x, start_drag_y;
				
				DECLARE_EVENT_TABLE()
				unsigned int id;

				wxCursor *opened_hand_cursor, *closed_hand_cursor;

				cpw::MouseButtonsState mbs;
				
				std::string default_directory;
		};
	}
}

#endif
