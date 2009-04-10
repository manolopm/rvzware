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
#include <osg/Drawable>
#include <osg/MatrixTransform>
#include <osg/Geode>
#include <osg/Texture>
#include <osg/Geometry>
#include <osg/TexMat>
#include <osg/TexEnvCombine>
#include <osgDB/ReadFile>
#include <osg/StateAttribute>
#include <osg/AlphaFunc>
#include <osg/Switch>
#include <osgDB/ReadFile>
#include <osg/Geometry>
#include <osg/BlendFunc>
#include <osg/PointSprite>
#include <osg/Point>
#include <osg/Texture2D>
#include <osg/Group>
#include <osg/Geode>

#include "stdafx.h"

#include <iosg/OsgSun.h>

#include <cpw/common/Math.h>


using namespace cpw::iosg;

#define CENTER_POSX 422758.0f
#define CENTER_POSY 153376.0f
//#define SUN_DISTANCE 5000000
//#define SUN_DISTANCE 7100000
#define SUN_DISTANCE 7100000
//#define SUN_DISTANCE 200000

#define AMBIENT_R 0.7f
#define AMBIENT_G 0.7f
#define AMBIENT_B 0.7f

#define AMBIENT_MIDDAY_R 0.90f
#define AMBIENT_MIDDAY_G 0.90f
#define AMBIENT_MIDDAY_B 0.90f

#define AMBIENT_SUNSET_R 0.90f
#define AMBIENT_SUNSET_G 0.75f
#define AMBIENT_SUNSET_B 0.7f

#define AMBIENT_MIDNIGHT_R 0.3f
#define AMBIENT_MIDNIGHT_G 0.1f
#define AMBIENT_MIDNIGHT_B 0.2f

#define DIFUSSE_R 1.0f
#define DIFUSSE_G 1.0f
#define DIFUSSE_B 1.0f

#define DIFUSSE_MIDDAY_R 1.0f
#define DIFUSSE_MIDDAY_G 1.0f
#define DIFUSSE_MIDDAY_B 1.0f

#define DIFUSSE_SUNSET_R 1.0f
#define DIFUSSE_SUNSET_G 0.7f
#define DIFUSSE_SUNSET_B 0.6f

#define DIFUSSE_MIDNIGHT_R 0.3f
#define DIFUSSE_MIDNIGHT_G 0.3f
#define DIFUSSE_MIDNIGHT_B 0.3f

OsgSun::OsgSun(const std::string &url) : default_directory(url), sun_distance(SUN_DISTANCE), sun_relative_pos(0.5f)
{
	scale.x = scale.y = scale.z = 1;
	traslation.x = CENTER_POSX;
	traslation.y = CENTER_POSY;
	traslation.z = 0;
	rotation.x = rotation.z = 0;
	rotation.y = osg::PI/4;

	light_group = new osg::Group;
	
	sunLight = new osg::Light;
	sunLight->setLightNum(0);
	sunLight->setPosition(osg::Vec4(0.0f,0.0f,0.0f,1.0f));
	sunLight->setAmbient(osg::Vec4(0.3f,0.1f,0.2f,1.0f));
	sunLight->setDiffuse(osg::Vec4(1.0f,1.0f,1.0f,1.0f));
	sunLight->setDirection(osg::Vec3(0.0f,0.0f,1.0f));
 	
	osg::LightSource* sun = new osg::LightSource;   
	sun->setLight(sunLight.get());
	sun->setLocalStateSetModes(osg::StateAttribute::ON);
 	
	light_group->addChild(sun);

	osg_matrix = new osg::MatrixTransform;
	t_matrix = osg_matrix->getMatrix();

	SetScale(cpw::Point3d<float>(60,60,60));
	
	Update();

	new_node = osgDB::readNodeFile(default_directory + "sun.osg");
	osg_matrix->addChild(new_node);
	osg_matrix->addChild(light_group.get());	
	osg_matrix->setName("SUN");

	osg::BlendFunc    *blendFunc = new osg::BlendFunc();
	blendFunc->setSource(osg::BlendFunc::SRC_ALPHA);
	blendFunc->setDestination(osg::BlendFunc::ONE);
	new_node->getOrCreateStateSet()->setAttributeAndModes(blendFunc,osg::StateAttribute::ON);
}


OsgSun::~OsgSun()
{
}

void OsgSun::SetRotation(const cpw::Point3d<float> &_rotation)
{
	rotation = _rotation;
	Update();
}

void OsgSun::SetRelativeRotation(const cpw::Point3d<float> &_rotation)
{
	rotation = _rotation;
	sun_relative_pos = rotation.y;
	rotation.y = osg::PI - (2*osg::PI * _rotation.y);
	Update();
}

void OsgSun::SetTraslation(const cpw::Point3d<float> &_position)
{
	traslation = _position;
	Update();
}

void OsgSun::SetScale(const cpw::Point3d<float> &_scale)
{
	scale = _scale;
	Update();
}


void OsgSun::Update()
{
	osg::Matrix trans_m, trans2_m, scale_m, orientation_mx, orientation_my, orientation_mz;
		
	trans_m.setTrans(osg::Vec3(0, 0, SUN_DISTANCE));
	trans2_m.setTrans(osg::Vec3(traslation.x, traslation.y, traslation.z));
	scale_m.identity();
	scale_m.makeScale(osg::Vec3(scale.x, scale.y, scale.z));
	orientation_mx.makeRotate(rotation.x, osg::Vec3(1.0, 0.0, 0.0));
	orientation_my.makeRotate(rotation.y, osg::Vec3(0.0, 1.0, 0.0));
	orientation_mz.makeRotate(rotation.z, osg::Vec3(0.0, 0.0, 1.0));
	
	t_matrix.makeIdentity();

	osg_matrix->setMatrix(scale_m * trans_m * orientation_mx * orientation_my * orientation_mz * trans2_m);

	CalculateAmbientLight();
}

void OsgSun::CalculateAmbientLight()
{
	float ambient_r,ambient_g,ambient_b;
	float percent = sun_relative_pos;
	if (percent > 0.5)
		percent= 1 - percent; 
	percent*=2;

	//ambient_r = AMBIENT_MIDNIGHT_R * (1-percent) + AMBIENT_MIDDAY_R* (percent);
	//ambient_g = AMBIENT_MIDNIGHT_G * (1-percent) + AMBIENT_MIDDAY_G* (percent);
	//ambient_b = AMBIENT_MIDNIGHT_B * (1-percent) + AMBIENT_MIDDAY_B* (percent);

	if (percent < 0.5)
	{
		percent*=2;
		ambient_r = AMBIENT_MIDNIGHT_R * (1-percent) + AMBIENT_SUNSET_R* (percent);
		ambient_g = AMBIENT_MIDNIGHT_G * (1-percent) + AMBIENT_SUNSET_G* (percent);
		ambient_b = AMBIENT_MIDNIGHT_B * (1-percent) + AMBIENT_SUNSET_B* (percent);
	}
	else
	{
		percent= (1-percent)*2;
		ambient_r = AMBIENT_SUNSET_R * (percent) + AMBIENT_MIDDAY_R* (1-percent);
		ambient_g = AMBIENT_SUNSET_G * (percent) + AMBIENT_MIDDAY_G* (1-percent);
		ambient_b = AMBIENT_SUNSET_B * (percent) + AMBIENT_MIDDAY_B* (1-percent);
	}
	sunLight->setAmbient(osg::Vec4(ambient_r,ambient_g,ambient_b,1.0f));

}
