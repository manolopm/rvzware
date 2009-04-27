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

#include <gui/UISplitterCanvas.h>

#include <cpw/graphic/INavigator.h>
#include <cpw/common/Math.h>
#include <cpw/graphic/ApplicationScene.h>

using namespace cpw::gui;


UISplitterCanvas::UISplitterCanvas(wxSplitterWindow* parent, cpw::INavigatorManager *navigator_manager)
  : mainParent_(parent), navigator_manager_(navigator_manager)
{
	active_navigators = 0;
	old_navigators = 0;
}

UISplitterCanvas::~UISplitterCanvas()
{
  ClearSubsplitters();
}

void UISplitterCanvas::Obliviate()
{

}

void UISplitterCanvas::Replace(wxSplitterWindow *splitter_new)
{
	if (old_field!=NULL)
	{
		for ( int i = active_navigators; i < old_navigators; i++ )
		{
			navigator_manager_->DeleteNavigator(i);
		}
	
		mainParent_->Show(false);

		old_field->Destroy();
		mainParent_->ReplaceWindow(old_field, splitter_new);
		UpdateSizes();

		navigator_manager_->Draw();
		mainParent_->Show(true);
	}
}
void UISplitterCanvas::UpdateSizes()
{
	if (!subsplitters.empty())
	{
		std::vector<wxSplitterWindow*>::const_iterator iter = subsplitters.begin();
		for(;iter != subsplitters.end(); iter++)
			(*iter)->UpdateSize();
	}
}

void UISplitterCanvas::ClearSubsplitters()
{
	if (!subsplitters.empty())
	{
		subsplitters.clear();
	}
}

wxSplitterWindow * UISplitterCanvas::CreateCanvasField(wxWindow* parent, wxWindowID id, const wxPoint& point, 
						       const wxSize& size, long style, const wxString& name)
{
	wxSplitterWindow *splitter_field = new wxSplitterWindow(parent, id, point, size, style, name);
	splitter_field->SetMinimumPaneSize(120);
	splitter_field->SetSashGravity(0.5);

	return splitter_field; 
}

wxSplitterWindow * UISplitterCanvas::CreateCanvas(wxWindow* parent, wxWindowID id, wxWindow **canvas1, 
						  cpw::INavigatorManager *navigator_manager, int viewID, 
						  const wxPoint& point, const wxSize& size, long style, 
						  const wxString& name)
{
	wxSplitterWindow *splitter_canvas1 = new wxSplitterWindow(parent, id, point, size, style);
	navigator_manager_->DeleteNavigator(viewID);
	(*canvas1)  = (wxWindow *)navigator_manager_->AddNavigator(splitter_canvas1, viewID);
	//navigator_manager_->

	return splitter_canvas1; 
}

void UISplitterCanvas::SetHorizontalRelationship(wxSplitterWindow **field, wxSplitterWindow *top, wxSplitterWindow *bottom)
{
	(*field)->SplitHorizontally(top, bottom);
}

void UISplitterCanvas::SetVerticalRelationship(wxSplitterWindow **field, wxSplitterWindow *left, wxSplitterWindow *right)
{
	(*field)->SplitVertically(left, right);
}

void UISplitterCanvas::SetInitializeCanvas(wxSplitterWindow **splitterCanvas, wxWindow *canvas)
{
	(*splitterCanvas)->Initialize(canvas);
}

wxSplitterWindow * UISplitterCanvas::Preset4ViewsA(wxWindowID id, const wxPoint& point, const wxSize& size, long style, const wxString& name)
{
	bool one_to_four_transition = false;
	cpw::ICamera *camera = NULL;
	cpw::INavigator *navigator = NULL;
	cpw::Point3d<double> camera0_position, camera1_position, camera2_position, camera3_position;
	cpw::Point3d<double> camera0_view, camera1_view, camera2_view, camera3_view;
	cpw::Point3d<double> camera0_up, camera1_up, camera2_up, camera3_up;
	cpw::Point3d<double> ip(0.0, 0.0, 0.0);
	
	navigator_manager_->Pause(true);
	
	wxWindow *canvas1, *canvas2, *canvas3, *canvas4;

	if (active_navigators == 1)
	{
		one_to_four_transition = true;
		navigator = navigator_manager_->GetFocusedOrFirstNavigator();

		float ix, iy, iz;
		int x_size, y_size;
		navigator->GetCenterScreenCoords(x_size, y_size);

		if (navigator->IntersectMouseWithScene(x_size, y_size, ix, iy, iz))
		{			
			ip.x = ix;
			ip.y = iy;
			ip.z = iz;
		}
		
		camera0_position = navigator->GetCameraController()->GetCamera()->GetPosition();
		camera0_view = navigator->GetCameraController()->GetCamera()->GetView();
		camera0_up = navigator->GetCameraController()->GetCamera()->GetUp();

		navigator_manager_->EveryNavigatorsToStartPosition();
		navigator_manager_->CameraParametersToNextNewNavigator(camera0_position, camera0_view, camera0_up);
	}
		
	ClearSubsplitters();
	wxSplitterWindow *splitter_field1 = CreateCanvasField(mainParent_, id, point, size, style);
	wxSplitterWindow *splitter_field2 = CreateCanvasField(splitter_field1, wxID_ANY, wxDefaultPosition, wxDefaultSize, style);
	wxSplitterWindow *splitter_field3 = CreateCanvasField(splitter_field1, wxID_ANY, wxDefaultPosition, wxDefaultSize, style);
	subsplitters.push_back(splitter_field1);
	subsplitters.push_back(splitter_field2);
	subsplitters.push_back(splitter_field3);
	
	wxSplitterWindow *splitter_canvas1 = CreateCanvas(splitter_field2, wxID_ANY, &canvas1, navigator_manager_, 0, wxDefaultPosition, wxDefaultSize, style);

	if (one_to_four_transition)
	{
		cpw::Point3d<double> p_aux(camera0_position);
		cpw::Point3d<double> v_aux(camera0_view);
		cpw::Point3d<double> u_aux(camera0_up);

		camera1_view = camera0_view;
		camera1_up = camera0_up;

		cpw::Point3d<double> point_to_rotate(camera0_position.x - ip.x, camera0_position.y - ip.y, camera0_position.z - ip.z);

		cpw::Math::RotatePoint(point_to_rotate, cpw::Point3d<double>(0.0, 0.0, 1.0), 180);
		cpw::Math::RotatePoint(camera1_view, cpw::Point3d<double>(0.0, 0.0, 1.0), 180);
		cpw::Math::RotatePoint(camera1_up, cpw::Point3d<double>(0.0, 0.0, 1.0), 180);
		cpw::Math::NormalizeVec(camera1_view);
		cpw::Math::NormalizeVec(camera1_up);

		camera1_position = point_to_rotate + ip;

		navigator_manager_->CameraParametersToNextNewNavigator(camera1_position, camera1_view, camera1_up);
	}

	wxSplitterWindow *splitter_canvas2 = CreateCanvas(splitter_field2, wxID_ANY, &canvas2, navigator_manager_, 1, wxDefaultPosition, wxDefaultSize, style);

	if (one_to_four_transition)
	{
		cpw::Point3d<double> p_aux(camera0_position);
		cpw::Point3d<double> v_aux(camera0_view);
		cpw::Point3d<double> u_aux(camera0_up);

		camera2_view = camera0_view;
		camera2_up = camera0_up;

		cpw::Point3d<double> point_to_rotate(camera0_position.x - ip.x, camera0_position.y - ip.y, camera0_position.z - ip.z);

		cpw::Math::RotatePoint(point_to_rotate, cpw::Point3d<double>(0.0, 0.0, 1.0), 90);
		cpw::Math::RotatePoint(camera2_view, cpw::Point3d<double>(0.0, 0.0, 1.0), 90);
		cpw::Math::RotatePoint(camera2_up, cpw::Point3d<double>(0.0, 0.0, 1.0), 90);
		cpw::Math::NormalizeVec(camera2_view);
		cpw::Math::NormalizeVec(camera2_up);

		camera2_position = point_to_rotate + ip;

		navigator_manager_->CameraParametersToNextNewNavigator(camera2_position, camera2_view, camera2_up);
	}

	wxSplitterWindow *splitter_canvas3 = CreateCanvas(splitter_field3, wxID_ANY, &canvas3, navigator_manager_, 2, wxDefaultPosition, wxDefaultSize, style);

	if (one_to_four_transition)
	{
		cpw::Point3d<double> p_aux(camera0_position);
		cpw::Point3d<double> v_aux(camera0_view);
		cpw::Point3d<double> u_aux(camera0_up);

		camera3_view = camera0_view;
		camera3_up = camera0_up;

		cpw::Point3d<double> point_to_rotate(camera0_position.x - ip.x, camera0_position.y - ip.y, camera0_position.z - ip.z);

		cpw::Math::RotatePoint(point_to_rotate, cpw::Point3d<double>(0.0, 0.0, 1.0), -90);
		cpw::Math::RotatePoint(camera3_view, cpw::Point3d<double>(0.0, 0.0, 1.0), -90);
		cpw::Math::RotatePoint(camera3_up, cpw::Point3d<double>(0.0, 0.0, 1.0), -90);
		cpw::Math::NormalizeVec(camera3_view);
		cpw::Math::NormalizeVec(camera3_up);

		camera3_position = point_to_rotate + ip;

		navigator_manager_->CameraParametersToNextNewNavigator(camera3_position, camera3_view, camera3_up);
	}

	wxSplitterWindow *splitter_canvas4 = CreateCanvas(splitter_field3, wxID_ANY, &canvas4, navigator_manager_, 3, wxDefaultPosition, wxDefaultSize, style);

	splitter_field1->SetSashSize(2);
	splitter_field2->SetSashSize(2);
	splitter_field3->SetSashSize(2);

	SetHorizontalRelationship(&splitter_field1, splitter_field2, splitter_field3);
	SetVerticalRelationship(&splitter_field3, splitter_canvas3, splitter_canvas4);
	SetVerticalRelationship(&splitter_field2, splitter_canvas1, splitter_canvas2);
	SetInitializeCanvas(&splitter_canvas1, canvas1);
	SetInitializeCanvas(&splitter_canvas2, canvas2);
	SetInitializeCanvas(&splitter_canvas3, canvas3);
	SetInitializeCanvas(&splitter_canvas4, canvas4);

	if (main_field!=NULL) old_field = main_field;
	main_field = splitter_field1;

	old_navigators = active_navigators;
	active_navigators = 4;
	
	navigator_manager_->Pause(false);

	return splitter_field1;
}

wxSplitterWindow * UISplitterCanvas::Preset4ViewsB(wxWindowID id, const wxPoint& point, const wxSize& size, long style, const wxString& name)
{
	wxWindow *canvas1, *canvas2, *canvas3, *canvas4;
		
	ClearSubsplitters();
	wxSplitterWindow *splitter_field1 = CreateCanvasField(mainParent_, id, point, size, style);
	wxSplitterWindow *splitter_field2 = CreateCanvasField(splitter_field1, wxID_ANY, wxDefaultPosition, wxDefaultSize, style);
	wxSplitterWindow *splitter_field3 = CreateCanvasField(splitter_field2, wxID_ANY, wxDefaultPosition, wxDefaultSize, style);
	subsplitters.push_back(splitter_field1);
	subsplitters.push_back(splitter_field2);
	subsplitters.push_back(splitter_field3);

	wxSplitterWindow *splitter_canvas1 = CreateCanvas(splitter_field1, wxID_ANY, &canvas1, navigator_manager_, 0, wxDefaultPosition, wxDefaultSize, style);
	wxSplitterWindow *splitter_canvas2 = CreateCanvas(splitter_field2, wxID_ANY, &canvas2, navigator_manager_, 1, wxDefaultPosition, wxDefaultSize, style);
	wxSplitterWindow *splitter_canvas3 = CreateCanvas(splitter_field3, wxID_ANY, &canvas3, navigator_manager_, 2, wxDefaultPosition, wxDefaultSize, style);
	wxSplitterWindow *splitter_canvas4 = CreateCanvas(splitter_field3, wxID_ANY, &canvas4, navigator_manager_, 3, wxDefaultPosition, wxDefaultSize, style);

	splitter_field1->SetSashSize(2);
	splitter_field2->SetSashSize(2);
	splitter_field3->SetSashSize(2);

	SetVerticalRelationship(&splitter_field1, splitter_field2, splitter_canvas1);
	SetHorizontalRelationship(&splitter_field3, splitter_canvas3, splitter_canvas4);
	SetHorizontalRelationship(&splitter_field2, splitter_canvas2, splitter_field3);
	SetInitializeCanvas(&splitter_canvas1, canvas1);
	SetInitializeCanvas(&splitter_canvas2, canvas2);
	SetInitializeCanvas(&splitter_canvas3, canvas3);
	SetInitializeCanvas(&splitter_canvas4, canvas4);

	if (main_field!=NULL) old_field = main_field;
	main_field = splitter_field1;

	old_navigators = active_navigators;
	active_navigators = 4;

	int width, height;
	if (old_navigators>0) 
	{
		old_field->GetSize(&width, &height);
		main_field->SetSize(width, height);
		main_field->UpdateSize();
	}
	
	return splitter_field1;
}

wxSplitterWindow * UISplitterCanvas::Preset4ViewsC(wxWindowID id, const wxPoint& point, const wxSize& size, long style, const wxString& name)
{
	wxWindow *canvas1, *canvas2, *canvas3, *canvas4;
		
	ClearSubsplitters();
	wxSplitterWindow *splitter_field1 = CreateCanvasField(mainParent_, id, point, size, style);
	wxSplitterWindow *splitter_field2 = CreateCanvasField(splitter_field1, wxID_ANY, wxDefaultPosition, wxDefaultSize, style);
	wxSplitterWindow *splitter_field3 = CreateCanvasField(splitter_field1, wxID_ANY, wxDefaultPosition, wxDefaultSize, style);
	subsplitters.push_back(splitter_field1);
	subsplitters.push_back(splitter_field2);
	subsplitters.push_back(splitter_field3);

	wxSplitterWindow *splitter_canvas1 = CreateCanvas(splitter_field2, wxID_ANY, &canvas1, navigator_manager_, 0, wxDefaultPosition, wxDefaultSize, style);
	wxSplitterWindow *splitter_canvas2 = CreateCanvas(splitter_field2, wxID_ANY, &canvas2, navigator_manager_, 1, wxDefaultPosition, wxDefaultSize, style);
	wxSplitterWindow *splitter_canvas3 = CreateCanvas(splitter_field3, wxID_ANY, &canvas3, navigator_manager_, 2, wxDefaultPosition, wxDefaultSize, style);
	wxSplitterWindow *splitter_canvas4 = CreateCanvas(splitter_field3, wxID_ANY, &canvas4, navigator_manager_, 3, wxDefaultPosition, wxDefaultSize, style);

	splitter_field1->SetSashSize(2);
	splitter_field2->SetSashSize(2);
	splitter_field3->SetSashSize(2);

	SetHorizontalRelationship(&splitter_field1, splitter_field2, splitter_field3);
	SetVerticalRelationship(&splitter_field3, splitter_canvas3, splitter_canvas4);
	SetVerticalRelationship(&splitter_field2, splitter_canvas1, splitter_canvas2);
	SetInitializeCanvas(&splitter_canvas1, canvas1);
	SetInitializeCanvas(&splitter_canvas2, canvas2);
	SetInitializeCanvas(&splitter_canvas3, canvas3);
	SetInitializeCanvas(&splitter_canvas4, canvas4);

	if (main_field!=NULL) old_field = main_field;
	main_field = splitter_field1;

	old_navigators = active_navigators;
	active_navigators = 4;

	return splitter_field1;
}

wxSplitterWindow * UISplitterCanvas::Preset4ViewsD(wxWindowID id, const wxPoint& point, const wxSize& size, long style, const wxString& name)
{
	wxWindow *canvas1, *canvas2, *canvas3, *canvas4;
		
	ClearSubsplitters();
	wxSplitterWindow *splitter_field1 = CreateCanvasField(mainParent_, id, point, size, style);
	wxSplitterWindow *splitter_field2 = CreateCanvasField(splitter_field1, wxID_ANY, wxDefaultPosition, wxDefaultSize, style);
	wxSplitterWindow *splitter_field3 = CreateCanvasField(splitter_field1, wxID_ANY, wxDefaultPosition, wxDefaultSize, style);
	subsplitters.push_back(splitter_field1);
	subsplitters.push_back(splitter_field2);
	subsplitters.push_back(splitter_field3);

	wxSplitterWindow *splitter_canvas1 = CreateCanvas(splitter_field2, wxID_ANY, &canvas1, navigator_manager_, 0, wxDefaultPosition, wxDefaultSize, style);
	wxSplitterWindow *splitter_canvas2 = CreateCanvas(splitter_field2, wxID_ANY, &canvas2, navigator_manager_, 1, wxDefaultPosition, wxDefaultSize, style);
	wxSplitterWindow *splitter_canvas3 = CreateCanvas(splitter_field3, wxID_ANY, &canvas3, navigator_manager_, 2, wxDefaultPosition, wxDefaultSize, style);
	wxSplitterWindow *splitter_canvas4 = CreateCanvas(splitter_field3, wxID_ANY, &canvas4, navigator_manager_, 3, wxDefaultPosition, wxDefaultSize, style);

	splitter_field1->SetSashSize(2);
	splitter_field2->SetSashSize(2);
	splitter_field3->SetSashSize(2);

	SetHorizontalRelationship(&splitter_field1, splitter_field2, splitter_field3);
	SetVerticalRelationship(&splitter_field3, splitter_canvas3, splitter_canvas4);
	SetVerticalRelationship(&splitter_field2, splitter_canvas1, splitter_canvas2);
	SetInitializeCanvas(&splitter_canvas1, canvas1);
	SetInitializeCanvas(&splitter_canvas2, canvas2);
	SetInitializeCanvas(&splitter_canvas3, canvas3);
	SetInitializeCanvas(&splitter_canvas4, canvas4);

	if (main_field!=NULL) old_field = main_field;
	main_field = splitter_field1;

	old_navigators = active_navigators;
	active_navigators = 4;

	return splitter_field1;
}

wxSplitterWindow * UISplitterCanvas::Preset4ViewsE(wxWindowID id, const wxPoint& point, const wxSize& size, long style, const wxString& name)
{
	wxWindow *canvas1, *canvas2, *canvas3, *canvas4;
		
	ClearSubsplitters();
	wxSplitterWindow *splitter_field1 = CreateCanvasField(mainParent_, id, point, size, style);
	wxSplitterWindow *splitter_field2 = CreateCanvasField(splitter_field1, wxID_ANY, wxDefaultPosition, wxDefaultSize, style);
	wxSplitterWindow *splitter_field3 = CreateCanvasField(splitter_field1, wxID_ANY, wxDefaultPosition, wxDefaultSize, style);
	subsplitters.push_back(splitter_field1);
	subsplitters.push_back(splitter_field2);
	subsplitters.push_back(splitter_field3);

	wxSplitterWindow *splitter_canvas1 = CreateCanvas(splitter_field2, wxID_ANY, &canvas1, navigator_manager_, 0, wxDefaultPosition, wxDefaultSize, style);
	wxSplitterWindow *splitter_canvas2 = CreateCanvas(splitter_field2, wxID_ANY, &canvas2, navigator_manager_, 1, wxDefaultPosition, wxDefaultSize, style);
	wxSplitterWindow *splitter_canvas3 = CreateCanvas(splitter_field3, wxID_ANY, &canvas3, navigator_manager_, 2, wxDefaultPosition, wxDefaultSize, style);
	wxSplitterWindow *splitter_canvas4 = CreateCanvas(splitter_field3, wxID_ANY, &canvas4, navigator_manager_, 3, wxDefaultPosition, wxDefaultSize, style);

	splitter_field1->SetSashSize(2);
	splitter_field2->SetSashSize(2);
	splitter_field3->SetSashSize(2);

	SetHorizontalRelationship(&splitter_field1, splitter_field2, splitter_field3);
	SetVerticalRelationship(&splitter_field3, splitter_canvas3, splitter_canvas4);
	SetVerticalRelationship(&splitter_field2, splitter_canvas1, splitter_canvas2);
	SetInitializeCanvas(&splitter_canvas1, canvas1);
	SetInitializeCanvas(&splitter_canvas2, canvas2);
	SetInitializeCanvas(&splitter_canvas3, canvas3);
	SetInitializeCanvas(&splitter_canvas4, canvas4);

	if (main_field!=NULL) old_field = main_field;
	main_field = splitter_field1;

	old_navigators = active_navigators;
	active_navigators = 4;

	return splitter_field1;
}

wxSplitterWindow * UISplitterCanvas::Preset3ViewsA(wxWindowID id, const wxPoint& point, const wxSize& size, long style, const wxString& name)
{
	wxWindow *canvas1, *canvas2, *canvas3;
		
	ClearSubsplitters();
	wxSplitterWindow *splitter_field1 = CreateCanvasField(mainParent_, id, point, size, style);
	wxSplitterWindow *splitter_field2 = CreateCanvasField(splitter_field1, wxID_ANY, wxDefaultPosition, wxDefaultSize, style);
	subsplitters.push_back(splitter_field1);
	subsplitters.push_back(splitter_field2);
	
	wxSplitterWindow *splitter_canvas1 = CreateCanvas(splitter_field1, wxID_ANY, &canvas1, navigator_manager_, 0, wxDefaultPosition, wxDefaultSize, style);
	wxSplitterWindow *splitter_canvas2 = CreateCanvas(splitter_field2, wxID_ANY, &canvas2, navigator_manager_, 1, wxDefaultPosition, wxDefaultSize, style);
	wxSplitterWindow *splitter_canvas3 = CreateCanvas(splitter_field2, wxID_ANY, &canvas3, navigator_manager_, 2, wxDefaultPosition, wxDefaultSize, style);

	splitter_field1->SetSashSize(2);
	splitter_field2->SetSashSize(2);

	SetVerticalRelationship(&splitter_field1, splitter_canvas1, splitter_field2);
	SetHorizontalRelationship(&splitter_field2, splitter_canvas2, splitter_canvas3);
	SetInitializeCanvas(&splitter_canvas1, canvas1);
	SetInitializeCanvas(&splitter_canvas2, canvas2);
	SetInitializeCanvas(&splitter_canvas3, canvas3);
	
	if (main_field!=NULL) old_field = main_field;
	main_field = splitter_field1;

	old_navigators = active_navigators;
	active_navigators = 3;

	return splitter_field1;
}

wxSplitterWindow * UISplitterCanvas::Preset3ViewsB(wxWindowID id, const wxPoint& point, const wxSize& size, long style, const wxString& name)
{
	wxWindow *canvas1, *canvas2, *canvas3;
		
	ClearSubsplitters();
	wxSplitterWindow *splitter_field1 = CreateCanvasField(mainParent_, id, point, size, style);
	wxSplitterWindow *splitter_field2 = CreateCanvasField(splitter_field1, wxID_ANY, wxDefaultPosition, wxDefaultSize, style);
	subsplitters.push_back(splitter_field1);
	subsplitters.push_back(splitter_field2);
	
	wxSplitterWindow *splitter_canvas1 = CreateCanvas(splitter_field1, wxID_ANY, &canvas1, navigator_manager_, 0, wxDefaultPosition, wxDefaultSize, style);
	wxSplitterWindow *splitter_canvas2 = CreateCanvas(splitter_field2, wxID_ANY, &canvas2, navigator_manager_, 1, wxDefaultPosition, wxDefaultSize, style);
	wxSplitterWindow *splitter_canvas3 = CreateCanvas(splitter_field2, wxID_ANY, &canvas3, navigator_manager_, 2, wxDefaultPosition, wxDefaultSize, style);

	splitter_field1->SetSashSize(2);
	splitter_field2->SetSashSize(2);

	SetHorizontalRelationship(&splitter_field1, splitter_canvas1, splitter_field2);
	SetVerticalRelationship(&splitter_field2, splitter_canvas2, splitter_canvas3);
	SetInitializeCanvas(&splitter_canvas1, canvas1);
	SetInitializeCanvas(&splitter_canvas2, canvas2);
	SetInitializeCanvas(&splitter_canvas3, canvas3);
	
	if (main_field!=NULL) old_field = main_field;
	main_field = splitter_field1;

	old_navigators = active_navigators;
	active_navigators = 3;

	return splitter_field1;
}

wxSplitterWindow * UISplitterCanvas::Preset3ViewsC(wxWindowID id, const wxPoint& point, const wxSize& size, long style, const wxString& name)
{
	wxWindow *canvas1, *canvas2, *canvas3;
		
	ClearSubsplitters();
	wxSplitterWindow *splitter_field1 = CreateCanvasField(mainParent_, id, point, size, style);
	wxSplitterWindow *splitter_field2 = CreateCanvasField(splitter_field1, wxID_ANY, wxDefaultPosition, wxDefaultSize, style);
	subsplitters.push_back(splitter_field1);
	subsplitters.push_back(splitter_field2);

	wxSplitterWindow *splitter_canvas1 = CreateCanvas(splitter_field1, wxID_ANY, &canvas1, navigator_manager_, 0, wxDefaultPosition, wxDefaultSize, style);
	wxSplitterWindow *splitter_canvas2 = CreateCanvas(splitter_field2, wxID_ANY, &canvas2, navigator_manager_, 1, wxDefaultPosition, wxDefaultSize, style);
	wxSplitterWindow *splitter_canvas3 = CreateCanvas(splitter_field2, wxID_ANY, &canvas3, navigator_manager_, 2, wxDefaultPosition, wxDefaultSize, style);

	splitter_field1->SetSashSize(2);
	splitter_field2->SetSashSize(2);
	
	SetVerticalRelationship(&splitter_field1, splitter_field2, splitter_canvas1);
	SetHorizontalRelationship(&splitter_field2, splitter_canvas2, splitter_canvas3);
	SetInitializeCanvas(&splitter_canvas1, canvas1);
	SetInitializeCanvas(&splitter_canvas2, canvas2);
	SetInitializeCanvas(&splitter_canvas3, canvas3);
	
	if (main_field!=NULL) old_field = main_field;
	main_field = splitter_field1;

	old_navigators = active_navigators;
	active_navigators = 3;

	return splitter_field1;
}

wxSplitterWindow * UISplitterCanvas::Preset3ViewsD(wxWindowID id, const wxPoint& point, const wxSize& size, long style, const wxString& name)
{
	wxWindow *canvas1, *canvas2, *canvas3;
		
	ClearSubsplitters();
	wxSplitterWindow *splitter_field1 = CreateCanvasField(mainParent_, id, point, size, style);
	wxSplitterWindow *splitter_field2 = CreateCanvasField(splitter_field1, wxID_ANY, wxDefaultPosition, wxDefaultSize, style);
	subsplitters.push_back(splitter_field1);
	subsplitters.push_back(splitter_field2);	

	wxSplitterWindow *splitter_canvas1 = CreateCanvas(splitter_field1, wxID_ANY, &canvas1, navigator_manager_, 0, wxDefaultPosition, wxDefaultSize, style);
	wxSplitterWindow *splitter_canvas2 = CreateCanvas(splitter_field2, wxID_ANY, &canvas2, navigator_manager_, 1, wxDefaultPosition, wxDefaultSize, style);
	wxSplitterWindow *splitter_canvas3 = CreateCanvas(splitter_field2, wxID_ANY, &canvas3, navigator_manager_, 2, wxDefaultPosition, wxDefaultSize, style);

	splitter_field1->SetSashSize(2);
	splitter_field2->SetSashSize(2);

	SetHorizontalRelationship(&splitter_field1, splitter_field2, splitter_canvas1);
	SetVerticalRelationship(&splitter_field2, splitter_canvas2, splitter_canvas3);
	SetInitializeCanvas(&splitter_canvas1, canvas1);
	SetInitializeCanvas(&splitter_canvas2, canvas2);
	SetInitializeCanvas(&splitter_canvas3, canvas3);
	
	if (main_field!=NULL) old_field = main_field;
	main_field = splitter_field1;

	old_navigators = active_navigators;
	active_navigators = 3;

	return splitter_field1;
}

wxSplitterWindow * UISplitterCanvas::Preset2ViewsH(wxWindowID id, const wxPoint& point, const wxSize& size, long style, const wxString& name)
{
	wxWindow *canvas1, *canvas2;
		
	ClearSubsplitters();
	wxSplitterWindow *splitter_field1 = CreateCanvasField(mainParent_, id, point, size, style);
	subsplitters.push_back(splitter_field1);

	wxSplitterWindow *splitter_canvas1 = CreateCanvas(splitter_field1, wxID_ANY, &canvas1, navigator_manager_, 0, wxDefaultPosition, wxDefaultSize, style);
	wxSplitterWindow *splitter_canvas2 = CreateCanvas(splitter_field1, wxID_ANY, &canvas2, navigator_manager_, 1, wxDefaultPosition, wxDefaultSize, style);

	splitter_field1->SetSashSize(2);
	
	SetHorizontalRelationship(&splitter_field1, splitter_canvas1, splitter_canvas2);
	SetInitializeCanvas(&splitter_canvas1, canvas1);
	SetInitializeCanvas(&splitter_canvas2, canvas2);

	if (main_field!=NULL) old_field = main_field;
	main_field = splitter_field1;

	old_navigators = active_navigators;
	active_navigators = 2;

	return splitter_field1;
}

wxSplitterWindow * UISplitterCanvas::Preset2ViewsV(wxWindowID id, const wxPoint& point, const wxSize& size, long style, const wxString& name)
{
	wxWindow *canvas1, *canvas2;
		
	ClearSubsplitters();
	wxSplitterWindow *splitter_field1 = CreateCanvasField(mainParent_, id, point, size, style);
	subsplitters.push_back(splitter_field1);

	wxSplitterWindow *splitter_canvas1 = CreateCanvas(splitter_field1, wxID_ANY, &canvas1, navigator_manager_, 0, wxDefaultPosition, wxDefaultSize, style);
	wxSplitterWindow *splitter_canvas2 = CreateCanvas(splitter_field1, wxID_ANY, &canvas2, navigator_manager_, 1, wxDefaultPosition, wxDefaultSize, style);

	splitter_field1->SetSashSize(2);
	

	SetVerticalRelationship(&splitter_field1, splitter_canvas1, splitter_canvas2);
	SetInitializeCanvas(&splitter_canvas1, canvas1);
	SetInitializeCanvas(&splitter_canvas2, canvas2);

	navigator_manager_->EveryNavigatorsToStartPosition();
//		this->DisableAllHUDCamerasButMine
//this->EveryNavigatorsToStartPosition

	if (main_field!=NULL) old_field = main_field;
	main_field = splitter_field1;

	old_navigators = active_navigators;
	active_navigators = 2;

	return splitter_field1;
}

wxSplitterWindow * UISplitterCanvas::PresetOnlyView(wxWindowID id, const wxPoint& point, const wxSize& size, long style, const wxString& name)
{
	wxWindow *canvas;
	
	ClearSubsplitters();
	wxSplitterWindow *splitter_field1 = CreateCanvas(mainParent_, wxID_ANY, &canvas, navigator_manager_, 0, wxDefaultPosition, wxDefaultSize, style);
	subsplitters.push_back(splitter_field1);
	
	SetInitializeCanvas(&splitter_field1, canvas);

	if (main_field!=NULL) old_field = main_field;
	main_field = splitter_field1;

	old_navigators = active_navigators;
	active_navigators = 1;

	return splitter_field1;
}

void UISplitterCanvas::TunePresetSashDivisions()
{
}


void UISplitterCanvas::Paint()
{
	if (navigator_manager_ != NULL)
		navigator_manager_->Draw();
}
