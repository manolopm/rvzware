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
#include <osg/CullFace>
#include <osg/Geometry>
#include <osg/Vec3>
#include <osg/Depth>

#include "stdafx.h"

#include <iosg/OsgOverlayImage.h>
#include <iosg/OsgScene.h>
#include <iosg/OsgNavigatorManager.h>

#include <cpw/graphic/ApplicationScene.h>
  
using namespace cpw::iosg; 

OsgOverlayImage::OsgOverlayImage(const cpw::Point2d<float> &_utm0, const cpw::Point2d<float> &_utm1,
				      const int &_resx, const int &_resy, const cpw::IOverlayImage::CPW_PIXEL_FORMAT &pf)
					  : _resx_(_resx), _resy_(_resy), utm0(_utm0), utm1(_utm1), visible(true)
{
	

	texture = new osg::Texture2D();
    texture->setTextureSize(_resx, _resy);
    texture->setInternalFormat(GL_RGBA);
    texture->setFilter(osg::Texture2D::MIN_FILTER,osg::Texture2D::LINEAR);
    texture->setFilter(osg::Texture2D::MAG_FILTER,osg::Texture2D::LINEAR);
	
	//drawable->getOrCreateStateSet()->setTextureAttribute(0, (osg::StateAttribute *)texture.get(), osg::StateAttribute::TEXTURE);
	((OsgScene *)cpw::ApplicationScene::GetInstance()->GetScene())->AddTextureLayer(texture.get(), utm0, utm1);


	
	//prepare the camera for the texture to render
	camera = new osg::Camera();
	camera->setName("Render To Texture Camera");
	camera->setViewport(0,0,_resx,_resy);
    camera->setRenderOrder(osg::Camera::PRE_RENDER);
	camera->attach(osg::Camera::COLOR_BUFFER, texture.get());
	
	camera->setProjectionMatrix(osg::Matrix::ortho2D(0,_resx,0,_resy));
    camera->setReferenceFrame(osg::Transform::ABSOLUTE_RF);
    camera->setViewMatrix(osg::Matrix::identity());
	
    camera->setClearMask( GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	camera->setClearColor(osg::Vec4(0.0f,0.0f,1.0f,0.0f));
	camera->setColorMask(true, true, true, true);
	camera->setRenderTargetImplementation(osg::Camera::FRAME_BUFFER_OBJECT);

	iosg::OsgScene *scene = (iosg::OsgScene *) cpw::ApplicationScene::GetInstance()->GetScene();

	//((osg::Group *)scene->GetRootNode())->addChild( camera.get() );
	scene->GetRenderToTextureCamerasGroup()->addChild( camera.get() );
	
	
	
	geode = new osg::Geode;
	geom = new osg::Geometry;
	vertices = new osg::Vec3Array;

	osg::ref_ptr<osg::Vec4Array> colors = new osg::Vec4Array();
	colors->push_back(osg::Vec4(0.0f, 0.0f, 0.0f, 0.75f));
	
	/*osg::ref_ptr<osg::Vec3Array> normals = new osg::Vec3Array();
	normals->push_back(osg::Vec3(0.0f, -1.0f, 0.0f));*/

	/*geom->setVertexArray(vertices.get());*/
	geom->setColorArray(colors.get());
	geom->setColorBinding(osg::Geometry::BIND_OVERALL);
	

	geode->addDrawable(geom.get());

	camera->addChild( geode.get() );
	
	geode->getOrCreateStateSet()->setAttributeAndModes( new osg::CullFace, osg::StateAttribute::OFF);
	geode->getOrCreateStateSet()->setMode(GL_LIGHTING, false);
	geode->getOrCreateStateSet()->setAttributeAndModes(new osg::Depth(osg::Depth::ALWAYS), osg::StateAttribute::ON);
	
}

OsgOverlayImage::~OsgOverlayImage(void)
{
	if (cpw::ApplicationScene::GetInstance()->GetScene() != NULL)
		((OsgScene *)cpw::ApplicationScene::GetInstance()->GetScene())->RemoveTextureLayer(texture.get());	
}

void OsgOverlayImage::Visualize(const bool &value)
{
	if (value != visible)
	{
		visible = value;

		if (visible)
			((OsgScene *)cpw::ApplicationScene::GetInstance()->GetScene())->AddTextureLayer(texture.get(), utm0, utm1);
		else
			((OsgScene *)cpw::ApplicationScene::GetInstance()->GetScene())->RemoveTextureLayer(texture.get());
	}

}

void OsgOverlayImage::GetSize(int &resx, int &resy) 
{ 
	_resx_ = resx;
	_resy_ = resy; 
}


bool OsgOverlayImage::DrawFirstNTriangleArrays(const unsigned int &n)
{
	vertices->clear();
	geom->removePrimitiveSet(0, geom->getPrimitiveSetList().size());
	int aux = 0;
	

	//for every triangle array
	for (unsigned int i=0; i<triangle_arrays.size() && i<n; i++)
	{
		std::vector<cpw::Triangle>::iterator iter = triangle_arrays[i].begin();
		for ( ; iter != triangle_arrays[i].end(); iter++)
		{
			vertices->push_back(osg::Vec3f(iter->v0.x, iter->v0.y, iter->v0.z));
			vertices->push_back(osg::Vec3f(iter->v1.x, iter->v1.y, iter->v1.z));
			vertices->push_back(osg::Vec3f(iter->v2.x, iter->v2.y, iter->v2.z));
		}

		geom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::TRIANGLES, aux, triangle_arrays[i].size() * 3));
		aux += triangle_arrays[i].size() * 3;		
	}

	geom->setVertexArray(vertices.get());


	//set the colors
	osg::ref_ptr<osg::Vec4Array> colors = new osg::Vec4Array();

	for (std::vector<cpw::Color>::iterator iter = p_colors.begin();
		 iter != p_colors.end();
		 iter++)
	{
		colors->push_back(osg::Vec4( iter->red, iter->green, iter->blue, iter->alpha));
	}
	
	geom->setColorArray(colors.get());
	geom->setColorBinding( osg::Geometry::BIND_PER_PRIMITIVE_SET );

	return true;
	
}

void OsgOverlayImage::Clear()
{
	vertices->clear();
	geom->removePrimitiveSet(0, geom->getPrimitiveSetList().size());	
}
