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

#include <iosg/stdafx.h>

#include <iosg/gui/OsgNavigationPanel.h>

using namespace cpw::iosg; 

OsgINavigationPanel::OsgINavigationPanel(const std::string &url, cpw::Point3d<float> _position, cpw::Point3d<float> _size, cpw::Point3d<float> _rotation, const TAnchor &_anchor, bool lite_version_) : OsgIPanel(url, _position, _size, _rotation, _anchor), camera(NULL), lite_version(lite_version_)
{
	SetDefaultPath(url);
	OsgIWidget::OsgIWidget(url, _position, _size, _rotation, _anchor);
	preset_position = GetPosition();
	is_hidden = false;
	being_animated = false;
	
	rotation.x = 0.0f;
	rotation.y = 0.0f;
	rotation.z = 0.0f;

	cpw::Point3d<float> barbutton_position (-2.7f, -11.15f, -220.2f);
	cpw::Point3d<float> barbutton_size(1.0f, 4.85f, 0.5f);
	cpw::Point3d<float> barbutton_rotation (0.0f, 0.0f, 0.0f);
	TAnchor barbutton_anchor;
	barbutton_anchor.x = barbutton_anchor.y = barbutton_anchor.w = barbutton_anchor.h = 0;
	osg_barbutton = new OsgIButton(url, barbutton_position, barbutton_size, barbutton_rotation, barbutton_anchor, this->GetDefaultPath() + "bar_but.osg");	
	osg_barbutton->SetName("Bar Button");
	this->AttachChild(osg_barbutton); 	
	

	cpw::Point3d<float> verticalButtonBar_position (0.0f, -2.0f, 0.2f);
	cpw::Point3d<float>	verticalButtonBar_size(2.0f , 3.0f , 5.0f) ;
	cpw::Point3d<float> verticalButtonBar_rotation(0.0f, 0.0f, 0.0f);
	TAnchor verticalButtonBar_anchor;
	verticalButtonBar_anchor.x = verticalButtonBar_anchor.y = verticalButtonBar_anchor.w = verticalButtonBar_anchor.h = 0;
	verticalButtonBar_panel = new OsgIVerticalButtonPanel(url, verticalButtonBar_position, verticalButtonBar_size, verticalButtonBar_rotation, verticalButtonBar_anchor,lite_version);	
	verticalButtonBar_panel->SetScale(cpw::Point3d<float>(0.6, 0.6, 1));
	this->AttachChild(verticalButtonBar_panel); 

}

OsgINavigationPanel::~OsgINavigationPanel(void)
{
}

void OsgINavigationPanel::SetCamera(OsgCamera *_camera) 
{ 
//	camera = _camera; 
//	osg_compass->SetCamera(camera);
}

void OsgINavigationPanel::Update(osg::Matrix *mp)
{
	if (being_animated)	UpdateToggleAnimation();
	
	//signo = 1;
	//if (rotation.x >= 120)
	//	signo = -240;
	//if (rotation.x < -120)
	//	signo = 1;

	rotation.x -= 0.02f;
	rotation.y -= 0.02f;
	rotation.z -= 0.02f;

	
	#ifdef _DEBUG 
	////////cpw::Point3d<double> camera_utm = OsgScene::SceneCoordsToUTM(camera->GetPosition());
	//////////camera_utm = iosg::OsgScene::SceneCoordsToUTM(camera->GetPosition());
	//////////camera_utm = cpw::ApplicationScene::GetInstance()->GetScene()->SceneCoordsToUTM(camera->GetPosition());
	//////////camera_utm.x = camera->GetPosition().x;
	//////////camera_utm.y = camera->GetPosition().y + 3000000.0f;
	//////////camera_utm.z = camera->GetPosition().z;
	////////std::ostringstream wop,wop2,wop3;
	////////wop  << /*"UTM: " <<*/ camera_utm.x ;
	////////wop2 << camera_utm.y;
	////////wop3 << "Height: " << camera_utm.z;
	////////osg_labelUTMl->SetLabel(wop.str());
	////////osg_labelUTMlt->SetLabel(wop2.str());
	////////osg_labelHeight->SetLabel(wop3.str());
	#endif

	OsgIPanel::Update(mp);
	
}

void OsgINavigationPanel::RegisterButtons(OsgIHUDController *ohc)
{
	if (!lite_version)
	{
		ohc->AddCallBack( verticalButtonBar_panel->GetFireButton()->GetId(), &iosg::OsgIHUDController::OpenFireDialog);
		ohc->AddCallBack( verticalButtonBar_panel->GetWMSButton()->GetId(), &iosg::OsgIHUDController::OpenWMSDialog);
		ohc->AddCallBack( verticalButtonBar_panel->GetBrickButton()->GetId(), &iosg::OsgIHUDController::OpenElement3dDialog);
	}
	ohc->AddCallBack( verticalButtonBar_panel->GetMiniMapButton()->GetId(), &iosg::OsgIHUDController::ToggleMinimap);
	ohc->AddCallBack( verticalButtonBar_panel->GetCompassButton()->GetId(), &iosg::OsgIHUDController::ToggleCompass);
	ohc->AddCallBack( osg_barbutton->GetId(), &iosg::OsgIHUDController::ToggleNavigationPanel);
}

void OsgINavigationPanel::StartToggleAnimation(const double &_animation_time)
{
	animation_time = _animation_time;
	initial_size = GetSize();
	initial_rotation = GetRotation();
	initial_position = GetPosition();
	last_t = osg::Timer::instance()->tick();
	being_animated = true;
	//is_hidden = !is_hidden;
	current_animation_time = 0.0;

}
void OsgINavigationPanel::UpdateToggleAnimation()
{
	current_t = osg::Timer::instance()->tick();
	double time_between_frames = (current_t - last_t) * osg::Timer::instance()->getSecondsPerTick();
	last_t = current_t;

	current_animation_time += time_between_frames;

	if (current_animation_time >= animation_time)
	{
		//SetRotation(initial_rotation);
		being_animated = false;
		is_hidden = !is_hidden;
		if (is_hidden==false) 
		{	
			SetPosition(preset_position);	
		}
		else
		{
			SetPosition(cpw::Point3d<float>(preset_position.x + initial_size.x, preset_position.y, preset_position.z ));
		}
	}
	else
	{
		float current_position = (is_hidden)? -initial_size.x : initial_size.x;
		current_position *= 1.05*current_animation_time / animation_time;
		SetPosition(cpw::Point3d<float>(initial_position.x + current_position, initial_position.y, initial_position.z ));
	}


}
