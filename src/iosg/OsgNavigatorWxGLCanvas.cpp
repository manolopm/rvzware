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

#include <math.h>
#include <wx/gdicmn.h>

#include <iosg/stdafx.h>

#include <iosg/OsgNavigatorWxGLCanvas.h>
#include <iosg/OsgNavigatorWxFrame.h>

#include <cpw/graphic/ApplicationScene.h>
#include <cpw/common/ApplicationLog.h>


using namespace cpw::iosg;

BEGIN_EVENT_TABLE        (OsgNavigatorWxGLCanvas, wxGLCanvas        ) 
    EVT_SIZE             (OsgNavigatorWxGLCanvas::OnSize            )
    EVT_PAINT            (OsgNavigatorWxGLCanvas::OnPaint           )
    EVT_ERASE_BACKGROUND (OsgNavigatorWxGLCanvas::OnEraseBackground )
    EVT_KEY_DOWN         (OsgNavigatorWxGLCanvas::OnKeyDown         )
    EVT_KEY_UP           (OsgNavigatorWxGLCanvas::OnKeyUp           )


	EVT_LEFT_DCLICK    ( OsgNavigatorWxGLCanvas::OnMouseLDoubleClick )
	EVT_MOUSEWHEEL     ( OsgNavigatorWxGLCanvas::OnMouseWheel     )
	EVT_RIGHT_DOWN     ( OsgNavigatorWxGLCanvas::OnMouseRightDown )
	EVT_LEFT_DOWN      ( OsgNavigatorWxGLCanvas::OnMouseLeftDown  )
	EVT_RIGHT_UP     ( OsgNavigatorWxGLCanvas::OnMouseRightDown )
	EVT_LEFT_UP      ( OsgNavigatorWxGLCanvas::OnMouseLeftDown  )
    EVT_MOTION         ( OsgNavigatorWxGLCanvas::OnMouseMotion    )
	EVT_ENTER_WINDOW ( OsgNavigatorWxGLCanvas::OnMouseEnterWindow)
    EVT_LEAVE_WINDOW ( OsgNavigatorWxGLCanvas::OnMouseLeaveWindow)
    
END_EVENT_TABLE()

OsgNavigatorWxGLCanvas::OsgNavigatorWxGLCanvas(wxWindow *parent, wxWindowID id,
										 const wxPoint& pos, const wxSize& size,
										 int *param, long style, const wxString& name)
        
: wxGLCanvas(parent, id, NULL, pos, size, style|wxFULL_REPAINT_ON_RESIZE | wxWANTS_CHARS, name, wxNullPalette), mouse_event_callback(NULL),
				     coords_flag(false), last_x(0), last_y(0), rotate_view_left(false), rotate_view_right(false), dragging(false), keep_mouse_inside_canvas(true)
{
		
	// default cursor to standard
    _oldCursor = *wxSTANDARD_CURSOR;
	_oldCursor = *wxCROSS_CURSOR;

    
    _traits = new GraphicsContext::Traits;
    _traits->x = pos.x;
    _traits->y = pos.y;
    _traits->width = size.x;
    _traits->height = size.y;
	//_traits->depth = 24;
	_traits->vsync = false;

	init();

}

OsgNavigatorWxGLCanvas::~OsgNavigatorWxGLCanvas(void)
{
}


void OsgNavigatorWxGLCanvas::init()
{
	if (valid())
    {
		//SetCurrent();
		setState( new osg::State );
        getState()->setGraphicsContext(this);

        if (_traits.valid() && _traits->sharedContext!=NULL)
        {
            getState()->setContextID( _traits->sharedContext->getState()->getContextID() );
            incrementContextIDUsageCount( getState()->getContextID() );   
        }
        else
        {
            getState()->setContextID( osg::GraphicsContext::createNewContextID() );
        }
    }
	//makeCurrentImplementation();
}


void OsgNavigatorWxGLCanvas::OnPaint( wxPaintEvent& WXUNUSED(event) )
{
	/* must always be here */
    wxPaintDC dc(this);
}

void OsgNavigatorWxGLCanvas::OnSize(wxSizeEvent& event)
{	
	//SetCurrent();
	// this is also necessary to update the context on some platforms
    wxGLCanvas::OnSize(event);

    // set GL viewport (not called by wxGLCanvas::OnSize on all platforms...)
	int width, height, posx, posy, a, b;
	GetClientSize(&width, &height);
	GetPosition(&posx, &posy);
	wxRect rect = this->GetRect();
	posx = rect.GetX();
	posy = rect.GetY();
	a    = rect.GetHeight();
	b = rect.GetWidth();
		
	//this->GetContext()->SetCurrent(*this);
    // update the window dimensions, in case the window has been resized.
	//getEventQueue()->windowResize(0, 0, width, height);
    //resized(0,0, width, height);
	getEventQueue()->windowResize(posx, posy, width, height);
    resized(posx, posy, width, height);
	osg_viewer->getCamera()->setViewport(posx,posy,width,height);
	navigator->UpdateProjectionMatrix(width, height);

	bool event_processed = false;
	for (std::list<cpw::IEventReceiver *>::iterator it = event_receivers.begin();
		(it != event_receivers.end()) && !event_processed;
		it++)
	{
		event_processed = (*it)->WindowResize(width, height);
	}
}

void OsgNavigatorWxGLCanvas::OnEraseBackground(wxEraseEvent& WXUNUSED(event))
{
    /* Do nothing, to avoid flashing on MSW */
	//int width, height;
	//GetClientSize(&width, &height);	
	//OnSize(wxSizeEvent(wxSize(width,height)));
	swapBuffersImplementation();
}

void OsgNavigatorWxGLCanvas::OnKeyDown(wxKeyEvent &event)
{
#if wxUSE_UNICODE
    int key = event.GetUnicodeKey();
#else
    int key = event.GetKeyCode();
#endif
    getEventQueue()->keyPress(key);

	bool event_processed = false;

	

	
	//osg::CullSettings::CullingMode cm = osg_viewer->getCamera()->getCullingMode();
	//cpw::ApplicationLog::GetInstance()->GetLogger()->printf ("---------modo anterior = 0x%x", cm);					
	switch (key) {
		/*case 49: cpw::ApplicationLog::GetInstance()->GetLogger()->printf ("tecla %d:", key);
				 osg_viewer->getCamera()->setCullingMode(cm ^ 0x80);
				 break;
		case 50: cpw::ApplicationLog::GetInstance()->GetLogger()->printf ("tecla %d:", key);
				 osg_viewer->getCamera()->setCullingMode(cm ^ 0x40);
				 break;
		case 51: cpw::ApplicationLog::GetInstance()->GetLogger()->printf ("tecla %d:", key);
				 osg_viewer->getCamera()->setCullingMode(cm ^ 0x20);
				 break;
		case 52: cpw::ApplicationLog::GetInstance()->GetLogger()->printf ("tecla %d:", key);
				 osg_viewer->getCamera()->setCullingMode(cm ^ 0x10);
				 break;
		case 53: cpw::ApplicationLog::GetInstance()->GetLogger()->printf ("tecla %d:", key);
				 osg_viewer->getCamera()->setCullingMode(cm ^ 0x08);
				 break;
		case 54: cpw::Ap0plicationLog::GetInstance()->GetLogger()->printf ("tecla %d:", key);
				 osg_viewer->getCamera()->setCullingMode(cm ^ 0x04);
				 break;*/
		case 55: cpw::ApplicationLog::GetInstance()->GetLogger()->printf ("tecla 7: PERMITE ACERCARSE AL TERRENO", key);
				 cpw::ApplicationLog::GetInstance()->GetLogger()->printf ("para recuperar modo normal con la tecla 9");
				 navigator->GetCamera()->SetDisp(10);
				 break;
		case 56: cpw::ApplicationLog::GetInstance()->GetLogger()->printf ("tecla 8: MODO NO RECORTE para datos LIDAR", key);
				 cpw::ApplicationLog::GetInstance()->GetLogger()->printf ("para recuperar modo normal con la tecla 9");
				 osg_viewer->getCamera()->setCullingMode(0x38);
				 break;
		case 57: cpw::ApplicationLog::GetInstance()->GetLogger()->printf ("tecla 9: MODO RECORTE NORMAL", key);
				 osg_viewer->getCamera()->setCullingMode(0x39);
				 navigator->GetCamera()->SetDisp(150);
				 break;
	}
				


	for (std::list<cpw::IEventReceiver *>::iterator it = event_receivers.begin();
		(it != event_receivers.end()) && !event_processed;
		it++)
	{
		event_processed = (*it)->KeyDown(key);
	}
	//if (navigator->GetCameraController() != NULL)
	//	navigator->GetCameraController()->KeyDown(key);

    event.ResumePropagation(10);
    event.Skip();
}

void OsgNavigatorWxGLCanvas::OnKeyUp(wxKeyEvent &event)
{
#if wxUSE_UNICODE
    int key = event.GetUnicodeKey();
#else
    int key = event.GetKeyCode();
#endif
    getEventQueue()->keyRelease(key);
//
//	if (navigator->GetCameraController() != NULL)
//		navigator->GetCameraController()->KeyUp(key);

	bool event_processed = false;

	for (std::list<cpw::IEventReceiver *>::iterator it = event_receivers.begin();
		(it != event_receivers.end()) && !event_processed;
		it++)
	{
		event_processed = (*it)->KeyUp(key);
	}
	
		
	
		
    // propagate event
	event.ResumePropagation(10);
    event.Skip();
}

void OsgNavigatorWxGLCanvas::OnMouse(wxMouseEvent& event)
{
	int width, height, flipped_x, flipped_y;
	GetClientSize(&width, &height);
	
	mbs.mouse_left_down = event.m_leftDown;
	mbs.mouse_middle_down = event.m_middleDown;
	mbs.mouse_right_down = event.m_rightDown;
	
	flipped_x = event.GetX();
	flipped_y = event.GetY();

	height /= 2;
	flipped_y = height - (flipped_y - height) ;
	
	if (!coords_flag)
	{
		last_x = (int) event.GetX();
		last_y = (int) event.GetY();
		coords_flag = true;
		return;
	}

	
	if (event.ButtonDown())
	{
		SetFocus();
				
		bool event_processed = false;
		for (std::list<cpw::IEventReceiver *>::iterator it = event_receivers.begin();
			(it != event_receivers.end()) && !event_processed;
			 it++)
		{
			switch (event.GetButton())
			{
				case wxMOUSE_BTN_LEFT:
					event_processed = (*it)->MouseLeftButtonDown(flipped_x, flipped_y);
					break;
				case wxMOUSE_BTN_MIDDLE:
					event_processed = (*it)->MouseMiddleButtonDown(flipped_x, flipped_y);
					break;
				case wxMOUSE_BTN_RIGHT:
					event_processed = (*it)->MouseRightButtonDown(flipped_x, flipped_y);
					break;
			}
		}
		
		//Osg events
		int button = event.GetButton();
		getEventQueue()->mouseButtonPress(event.GetX(), event.GetY(), button);
    }

	if (event.ButtonUp())
	{
		//camera events
		bool event_processed = false;
		for (std::list<cpw::IEventReceiver *>::iterator it = event_receivers.begin();
			(it != event_receivers.end()) && !event_processed;
			 it++)
		{
			switch (event.GetButton())
			{	
				case wxMOUSE_BTN_LEFT:
					event_processed = (*it)->MouseLeftButtonUp(flipped_x, flipped_y);
					break;
				case wxMOUSE_BTN_MIDDLE:
					event_processed = (*it)->MouseMiddleButtonUp(flipped_x, flipped_y);
					break;
				case wxMOUSE_BTN_RIGHT:
					event_processed = (*it)->MouseRightButtonUp(flipped_x, flipped_y);
					break;
			}
		}

		//osg events
		int button = event.GetButton();
		getEventQueue()->mouseButtonRelease(event.GetX(), event.GetY(), button);
	}
	
	if (event.Dragging()) 
	{
		getEventQueue()->mouseMotion(event.GetX(), event.GetY());
		
		bool event_processed = false;
		for (std::list<cpw::IEventReceiver *>::iterator it = event_receivers.begin();
			(it != event_receivers.end()) && !event_processed;
			 it++)
		{
			if ((last_x != event.GetX()) || (last_y != event.GetY()))
			{
				event_processed = (*it)->MouseDrag(last_x, height - (last_y - height) , flipped_x, flipped_y, mbs);
			}
		}	

		//if ((dragging == false) && (event.m_rightDown) && (event.m_leftDown)) //start dragging
		if (dragging == false)//start dragging
		{
			
			start_drag_x = event.GetX();
			start_drag_y = event.GetY();
			dragging = true;

			if ((event.m_rightDown) && (event.m_leftDown))
				return_to_start_drag = true;
			else
				return_to_start_drag = false;
		
		}
	}
		
	if (event.ButtonDown())
	{
		if (mouse_event_callback != NULL)
		{
			int button = event.GetButton();

			mouse_event_callback->OnMouse(flipped_x, flipped_y, button);
		}
	}
	
	navigator->MakeMeActive();
	last_x = (int) event.GetX();
	last_y = (int) event.GetY();
	
	
	if (dragging && !event.Dragging()) //stop dragging
	{
		dragging = false;
		
		if (return_to_start_drag)
			WarpPointer(start_drag_x, start_drag_y);

		last_x = start_drag_x;
		last_y = start_drag_y;
	}
	

	if (dragging)
	{
		useCursor(false);
		
		/*if ((event.m_rightDown) && (event.m_leftDown))
			useCursor(false);
		else
			SetCursor(*closed_hand_cursor);*/
		
		
	}
	else
	{
		//SetCursor(*opened_hand_cursor);
		//SetCursor(_oldCursor);
		SetCursor(*wxSTANDARD_CURSOR);
		//SetCursor(wxCURSOR_BULLSEYE);
		
		//useCursor(true);
		//cpw::ApplicationLog::GetInstance()->GetLogger()->NewLogMessage("si cursor");
	}

	//SetCursor(wxCURSOR_PAINT_BRUSH);
}

void OsgNavigatorWxGLCanvas::SetNavigatorFocus()
{
	SetFocus();
}


void OsgNavigatorWxGLCanvas::OnMouseWheel(wxMouseEvent &event)
{
	bool event_processed = false;
	int width, height, flipped_x, flipped_y;
	GetClientSize(&width, &height);

	flipped_x = event.GetX();
	flipped_y = event.GetY();

	height /= 2;
	flipped_y = height - (flipped_y - height) ;

	for (std::list<cpw::IEventReceiver *>::iterator it = event_receivers.begin();
		(it != event_receivers.end()) && !event_processed;
		 it++)
	{
		event_processed = (*it)->MouseWheel(event.GetWheelRotation(), event.GetWheelDelta(), flipped_x, flipped_y);
	}	
}

void OsgNavigatorWxGLCanvas::OnMouseRightDown(wxMouseEvent &event)
{
	OnMouse(event);
}
void OsgNavigatorWxGLCanvas::OnMouseLeftDown(wxMouseEvent &event)
{
	OnMouse(event);
}
void OsgNavigatorWxGLCanvas::OnMouseMotion(wxMouseEvent &event)
{
	OnMouse(event);
}

void OsgNavigatorWxGLCanvas::OnMouseLDoubleClick(wxMouseEvent &event)
{
	int width, height;
	
	GetClientSize(&width, &height);

	int flipped_x = event.GetX();
	int flipped_y = event.GetY();

	height /= 2;
	flipped_y = height - (flipped_y - height) ;

	bool event_processed = false;
	for (std::list<cpw::IEventReceiver *>::iterator it = event_receivers.begin();
		(it != event_receivers.end()) && !event_processed;
		 it++)
	{
		event_processed = (*it)->MouseLeftDoubleClick(flipped_x, flipped_y);
	}	

	
	
		
	
}

void OsgNavigatorWxGLCanvas::grabFocus()
{
    // focus this window
    SetFocus();
}

void OsgNavigatorWxGLCanvas::grabFocusIfPointerInWindow()
{
    // focus this window, if the pointer is in the window
    wxPoint pos = wxGetMousePosition();
    if (this == wxFindWindowAtPoint(pos)) {
        SetFocus();
    }
}

void OsgNavigatorWxGLCanvas::useCursor(bool cursorOn)
{
    if (cursorOn) {

        // show the old cursor
        //SetCursor(_oldCursor);
		SetCursor(*wxSTANDARD_CURSOR);
    }
    else {

        // remember the old cursor
        _oldCursor = GetCursor();

        // hide the cursor
        //    - can't find a way to do this neatly, so create a 1x1, transparent image
        wxImage image(1,1);
        image.SetMask(true);
        image.SetMaskColour(0, 0, 0);
        wxCursor cursor(image);
        SetCursor(cursor);
    }
}

bool OsgNavigatorWxGLCanvas::makeCurrentImplementation()
{
    SetCurrent();
    return true;
}

void OsgNavigatorWxGLCanvas::swapBuffersImplementation()
{
    SwapBuffers();
}


void OsgNavigatorWxGLCanvas::GetWindowSize(int &width, int &height)
{
	GetClientSize(&width, &height);
}

void OsgNavigatorWxGLCanvas::OnMouseEnterWindow(wxMouseEvent &event)
{
	navigator->GetCameraController()->MouseEnterWindow(event.GetX(), event.GetY());
	useCursor(true);
}
void OsgNavigatorWxGLCanvas::OnMouseLeaveWindow(wxMouseEvent &event)
{

	if (dragging && keep_mouse_inside_canvas)
	{
		int width, height;	
		GetClientSize(&width, &height);

		WarpPointer(width * 0.5f, height * 0.5f);
		last_x = width * 0.5f;
		last_y = height * 0.5f;
	}
	else
	{
		navigator->GetCameraController()->MouseLeaveWindow(event.GetX(), event.GetY());
	}
}


bool OsgNavigatorWxGLCanvas::RemoveEventReceiver(cpw::IEventReceiver *ev)
{
	for (std::list<cpw::IEventReceiver *>::iterator iter = event_receivers.begin();
		 iter != event_receivers.end();
		 iter++)
	{
		if ((*iter) == ev)
		{
			event_receivers.remove(*iter);
			return true;
		}
	}

	return false;
}


bool OsgNavigatorWxGLCanvas::AddEventReceiverInPosition(const unsigned int &position, cpw::IEventReceiver *ev)
{
	unsigned int i=0;

	for (std::list<cpw::IEventReceiver *>::iterator iter = event_receivers.begin();
		iter != event_receivers.end();
		iter++)
	{
		if (position == i)
		{
			event_receivers.insert(iter, ev);
			return true;
		}
		
		i++;
	}

	return false;
}

void OsgNavigatorWxGLCanvas::Clear() 
{
	//wxGLCanvas::SetColour(_T("Black"));
	//wxGLCanvas::SetBackgroundColour(wxColour(_T("Black")));
	//wxGLCanvas::SetForegroundColour(wxColour(_T("Black")));
	wxGLCanvas::ClearBackground();
	//wxGLCanvas::SwapBuffers();
	wxGLCanvas::Refresh();
	wxGLCanvas::Update();
	//wxGLCanvas::Hide();
	//wxGLCanvas::Show();
}
