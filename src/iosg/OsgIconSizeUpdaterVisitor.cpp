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
#include <osg/Point>
#include <osg/Group>
#include <osg/Geometry>
#include <math.h>

#include "stdafx.h"

#include <iosg/OsgIconSizeUpdaterVisitor.h>

using namespace cpw::iosg;

OsgIconSizeUpdaterVisitor::OsgIconSizeUpdaterVisitor(const float &_max_size, const float &_min_size,
									  const float &_max_distance, const float &_min_distance,
									  const float &_camera_pos_x, const float &_camera_pos_y,
									  const float &_camera_pos_z, const float &_max_height, const float &_min_height)
						  : osg::NodeVisitor( osg::NodeVisitor::TRAVERSE_ALL_CHILDREN ), max_size(_max_size), min_size(_min_size), max_distance(_max_distance),
						    min_distance(_min_distance), camera_pos_x(_camera_pos_x), camera_pos_y(_camera_pos_y), camera_pos_z(_camera_pos_z), max_height(_max_height), min_height(_min_height)
{

}

OsgIconSizeUpdaterVisitor::~OsgIconSizeUpdaterVisitor(void)
{
}

void OsgIconSizeUpdaterVisitor::apply( osg::Node& node )
{
	std::string node_name = node.getName();
	
	if (node_name == "-icon-")
	{
		//get the position of the icon by asking it´s parents
		osg::Group *father_node = node.getParent(0);
		if (std::string("MatrixTransform") == father_node->className())
		{
			osg::MatrixTransform *mt = (osg::MatrixTransform *) father_node;
			osg::Vec3d icon_position = mt->getMatrix().getTrans();
			osg::Matrixd md = mt->getMatrix();

			IconUserData *iud = (IconUserData *)node.getUserData();

			/*float distance = (float) sqrt(pow(icon_position.x() - camera_pos_x, 2) + 
								          pow(icon_position.y() - camera_pos_y, 2) + 
										  pow(icon_position.z() - camera_pos_z, 2));*/

			icon_position.set(osg::Vec3(iud->GetPosition().x, iud->GetPosition().y,	iud->GetPosition().z));


			float distance = (float) sqrt(pow(icon_position.x() - camera_pos_x, 2) + 
								          pow(icon_position.y() - camera_pos_y, 2) + 
										  pow(icon_position.z() - camera_pos_z, 2));

			float final_size = 0.0f;

			if (iud->GetHeight() != 0.0f)
				min_height = iud->GetHeight();
			
			cpw::Point3d<float> op = iud->GetPosition();

			if (distance <= min_distance)
			{
				final_size = max_size;
				md.setTrans(osg::Vec3d(op.x, op.y, op.z + min_height));
			}
			else if (distance >= max_distance)
			{
				final_size = min_size;
				md.setTrans(osg::Vec3d(op.x, op.y, op.z + max_height));
			}
			else if ((min_distance < distance) && (distance < max_distance))
			{
				float increment_distance = max_distance - min_distance;
				float size_difference = max_size - min_size;
				float height_difference = max_height - min_height;
				float final_height;

				final_size = max_size - (size_difference * ((distance-min_distance)/increment_distance));
				final_height = min_height + (height_difference * ((distance-min_distance)/increment_distance));
				md.setTrans(op.x, op.y, op.z + final_height);

				//update the matrix

			}
			
			osg::ref_ptr<osg::StateSet> set = node.getStateSet();
		
			/// Give some size to the points to be able to see the sprite
			osg::ref_ptr<osg::Point> point = new osg::Point();

			mt->setMatrix(md);
		
			point->setSize(final_size);
						
			set->setAttribute(point.get());
		}
	}

	//keep searching
	traverse(node);
}
