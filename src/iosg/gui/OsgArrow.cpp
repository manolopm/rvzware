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

#include <iosg/gui/OsgArrow.h>
#include <iosg/OsgVisitorFirstGeode.h>


using namespace cpw::iosg;

OsgIArrow::OsgIArrow(const std::string &url, cpw::Point3d<float> _position, cpw::Point3d<float> _size, cpw::Point3d<float> _rotation, const TAnchor &_anchor) : OsgIImage(url, _position, _size, _rotation,_anchor)
{
	widget = new OsgIWidget(url, _position, _size, _rotation,_anchor);
	SetObject(url + "red_arrow.osg");
}

OsgIArrow::~OsgIArrow(void)
{
        delete widget;
        widget = NULL;
}
