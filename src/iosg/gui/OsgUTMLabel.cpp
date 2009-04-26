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

#include <iosg/gui/OsgUTMLabel.h>
#include <sstream>

#include <cpw/debugger.h>

using namespace cpw::iosg; 

OsgIUTMLabel::OsgIUTMLabel(const std::string &url, cpw::Point3d<float> _position, cpw::Point3d<float> _size, cpw::Point3d<float> _rotation, const TAnchor &_anchor) : OsgILabel(url, _position, _size, _rotation, _anchor, ""), longitude("11"), latitude("22"), height("33"), camera(NULL)
{
	rotation.x = 0.0f;
	rotation.y = 0.0f;
	rotation.z = 0.0f;
	
	std::ostringstream wop;
	wop  << "UTM: Long " << "111111111111" << ", "
		       << "Lat " << "222222222222" << ", "
			   << "Hei " << "333333333333";
	OsgILabel::SetObject(wop.str());
}

OsgIUTMLabel::~OsgIUTMLabel(void)
{
}

void OsgIUTMLabel::Update(osg::Matrix *mp)
{
	OsgILabel::Update(mp);

	if (camera == NULL)
		return;

	//#ifdef _DEBUG 
	cpw::Point3d<double> camera_utm = OsgScene::SceneCoordsToUTM(camera->GetPosition());
	std::ostringstream wop,wop2,wop3;
	wop  << "UTM: Long ";

	wop  << std::fixed << /*std::setprecision(5) << */ camera_utm.x;
	wop  << ", "
		 << "Lat ";
	wop  << std::fixed << /*std::setsetprecision(5) << */ camera_utm.y;
	wop  << ", "
		 << "Hei ";
	wop  << std::fixed << camera_utm.z;
	OsgILabel::SetLabel(wop.str());
	//#endif
}




