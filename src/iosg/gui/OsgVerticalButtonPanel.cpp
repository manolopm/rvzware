/*
 * CAPAWARE 3D Geographical Multilayer Framework Free Software
 * Copyright (C) 2009 Instituto Tecnológico de Canarias (ITC) and Universidad de Las Palmas de Gran Canaria (ULPGC)
 *
 * This file is part of CAPAWARE.
 *
 * CAPAWARE is free software; you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 2.1 of the License, or (at your option)
 * any later version.
 *
 * CAPAWARE is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this application; if not, write to the Free Software Foundation, Inc.,
 * 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 *
 * The CAPAWARE development team
*/
#include "../stdafx.h"

#include <iosg/gui/OsgVerticalButtonPanel.h>

using namespace cpw::iosg; 

OsgIVerticalButtonPanel::OsgIVerticalButtonPanel(const std::string &url, cpw::Point3d<float> _position, cpw::Point3d<float> _size, cpw::Point3d<float> _rotation, const TAnchor &_anchor, bool liteVersion) : OsgIContainer(url, _position, _size, _rotation, _anchor), lite_version(liteVersion)
{
	SetDefaultPath(url);
	
	rotation.x = 0.0f;
	rotation.y = 0.0f;
	rotation.z = 0.0f;

	float _row = -2.2;
	float _offset = 6.5;
	cpw::Point3d<float> compassb_position;
	compassb_position.x = 0;
	compassb_position.z = 0;
	compassb_position.y = _row;
	cpw::Point3d<float> compassb_rotation;
	compassb_rotation.x = compassb_rotation.y = compassb_rotation.z = 0;
	TAnchor compassb_anchor;
	compassb_anchor.x = compassb_anchor.y = compassb_anchor.w = compassb_anchor.h = 0;
	cpw::Point3d<float> compassb_size;
	compassb_size.x = 5.2f;
	compassb_size.y = 5.2f;
	compassb_size.z = 1.2f;
	compass_button = new OsgIButton(url, compassb_position, compassb_size, compassb_rotation, compassb_anchor, url + "button_compass.osg");	
	compass_button->SetName("Compass Button");
	AttachChild(compass_button);	

	if (!lite_version)
	{
		_row-=_offset;
		cpw::Point3d<float> fireb_position;
		fireb_position.x = 0;
		fireb_position.z = 0;
		fireb_position.y = _row;
		cpw::Point3d<float> fireb_rotation;
		fireb_rotation.x = fireb_rotation.y = fireb_rotation.z = 0;
		TAnchor fireb_anchor;
		fireb_anchor.x = fireb_anchor.y = fireb_anchor.w = fireb_anchor.h = 0;
		cpw::Point3d<float> fireb_size;
		fireb_size.x = 5.2f;
		fireb_size.y = 5.2f;
		fireb_size.z = 1.2f;
		fire_button = new OsgIButton(url, fireb_position, fireb_size, fireb_rotation, fireb_anchor, url + "button_fire.osg");	
		fire_button->SetName("Fire Button");
		AttachChild(fire_button);

		//Element
		_row-=_offset;
		cpw::Point3d<float> brickb_position;
		brickb_position.x = 0;
		brickb_position.z = 0;
		brickb_position.y = _row;
		cpw::Point3d<float> brickb_rotation;
		brickb_rotation.x = brickb_rotation.y = brickb_rotation.z = 0;
		TAnchor brickb_anchor;
		brickb_anchor.x = brickb_anchor.y = brickb_anchor.w = brickb_anchor.h = 0;
		cpw::Point3d<float> brickb_size;
		brickb_size.x = 5.2f;
		brickb_size.y = 5.2f;
		brickb_size.z = 1.2f;
		brick_button = new OsgIButton(url, brickb_position, brickb_size, brickb_rotation, brickb_anchor, url + "button_brick.osg");	
		brick_button->SetName("Brick Button");
		AttachChild(brick_button);

		//WMS
		_row-=_offset;
		cpw::Point3d<float> wmsb_position;
		wmsb_position.x = 0;
		wmsb_position.z = 0;
		wmsb_position.y = _row;
		cpw::Point3d<float> wmsb_rotation;
		wmsb_rotation.x = wmsb_rotation.y = wmsb_rotation.z = 0;
		TAnchor wmsb_anchor;
		wmsb_anchor.x = wmsb_anchor.y = wmsb_anchor.w = wmsb_anchor.h = 0;
		cpw::Point3d<float> wmsb_size;
		wmsb_size.x = 5.2f;
		wmsb_size.y = 5.2f;
		wmsb_size.z = 1.2f;
		wms_button = new OsgIButton(url, wmsb_position, wmsb_size, wmsb_rotation, wmsb_anchor, url + "button_wms.osg");	
		wms_button->SetName("WMS Button");
		AttachChild(wms_button);
	}

	//Minimap
	_row-=_offset;
	cpw::Point3d<float> minimapb_position;
	minimapb_position.x = 0;
	minimapb_position.z = 0;
	minimapb_position.y = _row;
	cpw::Point3d<float> minimapb_rotation;
	minimapb_rotation.x = minimapb_rotation.y = minimapb_rotation.z = 0;
	TAnchor minimapb_anchor;
	minimapb_anchor.x = minimapb_anchor.y = minimapb_anchor.w = minimapb_anchor.h = 0;
	cpw::Point3d<float> minimapb_size;
	minimapb_size.x = 5.2f;
	minimapb_size.y = 5.2f;
	minimapb_size.z = 1.2f;
	minimap_button = new OsgIButton(url, minimapb_position, minimapb_size, minimapb_rotation, minimapb_anchor, url + "button_minimap.osg");	
	minimap_button->SetName("Minimap Button");
	AttachChild(minimap_button);
}

OsgIVerticalButtonPanel::~OsgIVerticalButtonPanel(void)
{
}

void OsgIVerticalButtonPanel::Update(osg::Matrix *mp)
{
	OsgIContainer::Update(mp);
}
