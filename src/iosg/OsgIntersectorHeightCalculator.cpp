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

#include <osgUtil/IntersectVisitor>
#include <osgUtil/LineSegmentIntersector>

#include "stdafx.h"

#include <iosg/OsgIntersectorHeightCalculator.h>

#include <cpw/common/Math.h>


using namespace cpw::iosg;

OsgIntersectorHeightCalculator::OsgIntersectorHeightCalculator(osg::Group *_root_node,  OsgScene *_scene) : root_node(_root_node), scene(_scene)
{

}

OsgIntersectorHeightCalculator::~OsgIntersectorHeightCalculator(void)
{

}

bool OsgIntersectorHeightCalculator::GetHeight(const float &x, const float &y, float &height)
{
	osg::ref_ptr<osgUtil::LineSegmentIntersector> intersector;
	intersector = new osgUtil::LineSegmentIntersector( osg::Vec3(x, y, 100000.0f),
													   osg::Vec3(x, y, -100000.0f));
	
	osgUtil::IntersectionVisitor intersect_visitor( intersector.get() );
    root_node->accept(intersect_visitor);

	if (intersector->containsIntersections())
	{
		height = intersector.get()->getFirstIntersection().getWorldIntersectPoint().z();
		return true;
	}
	else
	{
		height = 0;
		return true;
	}
}

bool OsgIntersectorHeightCalculator::GetHeightUTM(const float &x, const float &y, float &height)
{
	cpw::Point3d<float> utm_to_scene_point(x, y, 0.0f);
	utm_to_scene_point = scene->UTMToSceneCoords(utm_to_scene_point);

	osg::ref_ptr<osgUtil::LineSegmentIntersector> intersector;
	intersector = new osgUtil::LineSegmentIntersector( osg::Vec3(utm_to_scene_point.x, utm_to_scene_point.y, 100000.0f),
													   osg::Vec3(utm_to_scene_point.x, utm_to_scene_point.y, -100000.0f));
	
	osgUtil::IntersectionVisitor intersect_visitor( intersector.get() );
    root_node->accept(intersect_visitor);

	if (intersector->containsIntersections())
	{
		osg::Vec3 vec3 = intersector.get()->getFirstIntersection().getWorldIntersectPoint();
		height = scene->SceneCoordsToUTM( cpw::Point3d<float>(vec3.x(), vec3.y(), vec3.z()) ).z;
		return true;
	}
	else
		return false;
	return 0.0f;
}
