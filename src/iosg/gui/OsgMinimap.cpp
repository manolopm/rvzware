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

#include <iosg/gui/OsgMinimap.h>
#include <cpw/common/ApplicationLog.h>

using namespace cpw::iosg; 

//#define TERRAIN_WIDTH_MIN 163766
//#define TERRAIN_WIDTH_MAX 681338
//
//#define TERRAIN_HEIGHT_MIN 100000
//#define TERRAIN_HEIGHT_MAX 292810

#define TERRAIN_WIDTH_MIN 170842
#define TERRAIN_WIDTH_MAX 673808

#define TERRAIN_HEIGHT_MIN 48250.8f
#define TERRAIN_HEIGHT_MAX 259374


OsgIMinimap::OsgIMinimap(const std::string &url, cpw::Point3d<float> _position, cpw::Point3d<float> _size, cpw::Point3d<float> _rotation, const TAnchor &_anchor) : OsgIContainer(url, _position, _size, _rotation, _anchor), navigator(NULL)
{
	SetDefaultPath(url);

	rotation.x += 0.0f;
	rotation.y += 0.0f;
	rotation.z += 0.0f;
	
	is_hidden = false;
	being_animated = animated = false;
	status = -1;

	OsgIWidget::OsgIWidget(url, _position, _size, _rotation, _anchor);
	
	SetObject(this->GetDefaultPath() + "minimap.osg");

	cpw::Point3d<float> _position2;
	_position2.x = _position2.y = 0;
	_position2.z = 1;
	cpw::Point3d<float> _rotation2;
	_rotation2.x = _rotation2.y = _rotation2.z = 0;
	TAnchor _anchor2;
	_anchor2.x = _anchor2.y = _anchor2.w = _anchor2.h = 0;
	cpw::Point3d<float> _size2;
	_size2.x = 15.0f;
	_size2.y = 26.0f;
	_size2.z = 15.0f;
	osg_arrow = new OsgIImage(url, _position2, _size2, _rotation2, _anchor2, this->GetDefaultPath() + "red_arrow.osg");	

	this->AttachChild(osg_arrow); 
	pick_initial = true;

}

OsgIMinimap::~OsgIMinimap(void)
{
}

void OsgIMinimap::Update(osg::Matrix *mp)
{
	//minimap 1024x389
	if (being_animated) 
	{
		UpdateToggleAnimation();
	}
	if (camera == NULL)
		return;

	float factor_x, factor_y;
	factor_x = (642310.0f  - 205930.0f) / (188.564f-(-186.268f));
	factor_y = (241788.7f -  60596.3f);
	float fy = (73.3042f-(-76.8677f));
	factor_y /= fy;

	//cpw::Point3d<float> camera_pos((float)camera->GetPosition().x + OsgScene::GetAdaptOffset().x, (float)camera->GetPosition().y + OsgScene::GetAdaptOffset().y, osg_arrow->GetPosition().z + OsgScene::GetAdaptOffset().z);
	double x = OsgScene::GetAdaptOffset().x;
	double y = OsgScene::GetAdaptOffset().y;
	double z = OsgScene::GetAdaptOffset().z;
	cpw::Point3d<float> camera_pos((float)camera->GetPosition().x, (float)camera->GetPosition().y, osg_arrow->GetPosition().z);
	cpw::Point3d<float> norm_up((float)camera->GetUp().x, (float)camera->GetUp().y, (float)camera->GetUp().z);
	cpw::Point3d<float> norm_view((float)camera->GetView().x, (float)camera->GetView().y, (float)camera->GetView().z);
	cpw::Point3d<float> north(0.0f, 1.0f, 0.0f);
	cpw::Point3d<float> down(0.0f, 0.0f, -1.0f);
	cpw::Plane xy_plane(0.0, 0.0, 1.0, 0.0);	

	////log camera pos, raw
	//std::ostringstream debug_message;
	//debug_message << "cam real pos: " <<  camera_pos.x << ", " << camera_pos.y << ", " << camera_pos.z;	
	////cpw::ApplicationLog::GetInstance()->GetLogger()->NewLogMessage(debug_message.str());
	
	cpw::Math::NormalizeVec(norm_up);
	cpw::Math::NormalizeVec(norm_view);

	norm_up = xy_plane.ProjectVectorOnPlane(norm_up);
	float angle  = cpw::Math::AngleBetweenTwoVectorsInRadians(norm_up, north);
	float angle3 = (osg::PI/2) - cpw::Math::AngleBetweenTwoVectorsInRadians(norm_view, down);

	if (norm_up.x < 0)
		angle *= -1.0f;

	//debug_message << "pin: " <<  angle3 << norm_pin.x << ", " << norm_pin.y << ", " << norm_pin.z;	
	//cpw::ApplicationLog::GetInstance()->GetLogger()->NewLogMessage(debug_message.str());

	cpw::Point3d<float> rotation;
	rotation.x = rotation.y = rotation.z = 0.0;
	rotation.x = angle3;
	rotation.z = -1*angle;

	osg_arrow->SetRotation(rotation);
	float offset_x, offset_y;
	offset_x = -363.154f;
	offset_y = -127.0888f;
	cpw::Point3d<float> arrow_pos = cpw::Point3d<float>((camera_pos.x + x)/factor_x + offset_x, (camera_pos.y + y - 3000000.0)/factor_y + offset_y, 1);
	osg_arrow->SetPosition(arrow_pos);

	
	bool visible = osg_arrow->IsVisible();
	if (arrow_pos.x > -218.483f && arrow_pos.x < 218.483f && 
		arrow_pos.y > -89.233f  && arrow_pos.y < 89.233f )
	{
		if (! visible && this->IsVisible()) 
			osg_arrow->SetVisible(true);
	}
	else 
			osg_arrow->SetVisible(false);


	if (angle != 0.0)
		int debug = 1;

	OsgIContainer::Update(mp);
}

bool OsgIMinimap::MouseLeftDoubleClick(const int &x, const int &y)
{
	cpw::Point3d<float> i_point, local_i_point;
	std::string aux;

	local_i_point = GetLocalIntersectionPoint();
	cpw::Point3d<double> i_pointd(local_i_point.x, local_i_point.y, local_i_point.z);
	float x_max, x_min, y_max, y_min, terrain_width_inc, terrain_heigth_inc;

	terrain_width_inc = TERRAIN_WIDTH_MAX - TERRAIN_WIDTH_MIN;
	terrain_heigth_inc = TERRAIN_HEIGHT_MAX - TERRAIN_HEIGHT_MIN;
		
	GetBoundingBox(x_max, x_min, y_max, y_min); 

	double x_inc = x_max - x_min;
	double y_inc = y_max - y_min;

	double a = (local_i_point.x - x_min) / (double)x_inc;
	double b = (local_i_point.y - y_min) / (double)y_inc;
	
	double offsetx = OsgScene::GetAdaptOffset().x;
	double offsety = OsgScene::GetAdaptOffset().y;
	double offsetz = OsgScene::GetAdaptOffset().z;

	i_pointd.x = (a * terrain_width_inc) + TERRAIN_WIDTH_MIN - offsetx;
	i_pointd.y = (b * terrain_heigth_inc) + TERRAIN_HEIGHT_MIN - offsety + 3000000.0;
	i_pointd.z = 0.0;

	camera->GoToPanoramic(OsgScene::SceneCoordsToUTM(i_pointd), 3.0);
	

	return true;
}

void OsgIMinimap::StartToggleAnimation(const double &_animation_time)
{
	animation_time = _animation_time;
	last_t = osg::Timer::instance()->tick();
	being_animated = true;
	current_animation_time = 0.0;
	status++;
	status = status%3;

	if (pick_initial)
	{
		initial_size	 = GetSize();
		initial_scale	 = GetScale();
		initial_rotation = GetRotation();
		initial_position = GetPosition();
		initial_anchor   = GetAnchor();
		pick_initial = false;
	}

}
void OsgIMinimap::UpdateToggleAnimation()
{
	switch (status) 
	{
	case 0:
		{	
			OsgIContainer::SetVisible(false);
			SetPosition(initial_position);
			SetRotation(initial_rotation);
			SetScale(initial_scale);
			SetSize(initial_size);
			SetAnchor(initial_anchor.w,initial_anchor.h,initial_anchor.x,initial_anchor.y);
			being_animated = false;
		}
		break;
	case 1:
		{ 
			OsgIContainer::SetVisible(true);
			being_animated = false;
		}
		break;
	case 2:
		{
			current_t = osg::Timer::instance()->tick();
			double time_between_frames = (current_t - last_t) * osg::Timer::instance()->getSecondsPerTick();
			last_t = current_t;

			current_animation_time += time_between_frames;

			if (current_animation_time >= animation_time)
			{
				being_animated = false;
			}
			else
			{
				float current_position = 0.5;
				float factor_size = ( current_animation_time / animation_time );
				current_position  =  current_position * factor_size;
				float factor_offset = 1-factor_size;
				SetAnchor(initial_anchor.w+current_position,initial_anchor.h-current_position,initial_anchor.x*factor_offset,initial_anchor.y*factor_offset);
				cpw::Point3d<float> newsize;
				newsize.x = initial_scale.x + 1.7*factor_size;
				newsize.y = initial_scale.y + 1.7*factor_size;
				newsize.z = initial_scale.z;
				SetScale(newsize);
			}
		}
		break;
	}
}

void OsgIMinimap::SetVisible(const bool &_visible)
{
	if (_visible)
		status = -1;
	else
		status = 0;

	OsgIContainer::SetVisible(_visible);
}
