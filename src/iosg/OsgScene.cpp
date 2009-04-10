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
#include <osg/Referenced>
#include <osg/Drawable>
#include <osg/PagedLOD>
#include <osg/MatrixTransform>
#include <osg/Transform>
#include <osg/AutoTransform>
#include <osg/Geode>
#include <osg/Texture>
#include <osg/Geometry>
#include <osg/TexMat>
#include <osg/TexEnvCombine>
#include <osgDB/ReadFile>
#include <osg/StateAttribute>
#include <osg/Vec3>

#include <osg/AlphaFunc>
#include <osg/Switch>
#include <osgDB/ReadFile>
#include <osg/Geometry>
#include <osg/BlendFunc>
#include <osg/PointSprite>
#include <osg/Point>
#include <osg/Texture2D>
#include <osg/LOD>
#include <osg/Group>
#include <osg/Geode>
#include <osg/LineSegment>
#include <osg/LineWidth>
#include <osgParticle/Particle>
#include <osgParticle/ParticleSystem>
#include <osgParticle/ParticleSystemUpdater>
#include <osgParticle/ModularProgram>
#include <osgParticle/RandomRateCounter>
#include <osgParticle/SectorPlacer>
#include <osgParticle/SegmentPlacer>
#include <osgParticle/MultiSegmentPlacer>
#include <osgParticle/RadialShooter>
#include <osgParticle/AccelOperator>

#include <osgParticle/FluidFrictionOperator>
#include <osgUtil/IntersectVisitor>
#include <osgutil/LineSegmentIntersector>
#include <osgText/Font>
#include <osgText/Text>
#include <osgText/FadeText>

#include <OpenThreads/Barrier>
#include <float.h>
#include <math.h>
#include <sstream>

#include "stdafx.h"

#include <osg/Material>
#include <osgDB/ReadFile>
#include <osgDB/WriteFile>
#include <osgDB/Registry>
#include <osgDB/FileUtils>


#include <iosg/OsgScene.h>
#include <iosg/OsgNodeFinderVisitor.h>
#include <iosg/OsgIconSizeUpdaterVisitor.h>
#include <iosg/OsgIntersectorHeightCalculator.h>
#include <iosg/OsgPSCallback.h>
#include <iosg/OsgLineCallback.h>
#include <iosg/OsgLine2Callback.h>

#include <cpw/common/ApplicationLog.h>
#include <cpw/common/Math.h>

//////PJ
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <iostream>
#include <algorithm>
//////


using namespace cpw::iosg;

#define LOD_VALUE 65.0f
#define LOD_0_VALUE 30000.0f
#define LOD_1_VALUE 999999999.0f
#define LOD_PS 150000.0f
#define MIN_LOD_DISTANCE 7000.0f
#define OCEAN_TEXTURE_REPEAT_X 3500
#define OCEAN_TEXTURE_REPEAT_Y 3500
#define OCEAN_HEIGHT (-153-5)
#define OCEAN_POSX -8000000
#define OCEAN_POSY -8000000
#define SKY_HEIGHT -10000 
#define SKY_POSX 400000.0f
#define SKY_POSY 150000.0f
#define SKY_POSZ -11000.0f-12000.0f
#define OCEAN_POSX2 400000.0f
#define OCEAN_POSY2 150000.0f
#define OCEAN_POSZ 0.0f
#define INF 9999999999999.0f

#define MAX_PS_LODS 4
#define MAX_LINE_LODS 4
#define FIRE_STEP_DISTANCE 75.0f
#define LOD_MULT 14.0f //affects the distance in which the particle systems are going to change in to an icon
//#define LOD_MULT  1.81f //affects the distance in which the particle systems are going to change in to an icon
//#define LOD_MULTF 7.7f //affects the distance in which the particle systems are going to change in to an icon

#define VERY_BAD_FPS 10
#define BAD_FPS 20
#define MEDIUM_FPS 30
#define GOOD_FPS 40
#define EXCELLENT_FPS 50

#define MINIMUN_LOD_SIZE 6000

#define ADAPT_FACTOR 500000.0f

cpw::Point3d<float> adapt_center;
float adapt_scale;

//AJ shaders
//-----------
struct ExampleShaderCallback: public osg::Uniform::Callback
{
   ExampleShaderCallback() {}
 
   virtual void operator() ( osg::Uniform* uniform, osg::NodeVisitor* nv )
   {
      static float currentFloatValue = 0.0f;
      static bool isIncreasing = true;
 
      if (isIncreasing)
      {
         currentFloatValue += 0.01f;
         if (currentFloatValue >= 1.0f )
         {
            currentFloatValue = 1.0f;
            isIncreasing = false;
         }
      }
      else
      {
          currentFloatValue -= 0.01f;
           if (currentFloatValue <= 0.0f)
          {
              currentFloatValue = 0.0f;
              isIncreasing = true;
          }
       }
 
	  uniform->set( currentFloatValue );
   }
};
//-----------

OsgScene::OsgScene(const std::string &url): default_directory(url), temp_node(), osg_petition_manager(), visitor_flag(true), height_calculator(NULL), handlers_node(NULL),
                                            root_node(NULL), terrain_group(NULL), first_time(true), scene_sun(NULL), render_to_texture_cameras(NULL), camera_gizmos(NULL), 
											acumulated_time(0.0f), scene_performance(GOOD), frames_count(0), default_icon(""), redirect_wms_requests_to_scene_builder(false),
											request_url(), preview(false), osgdem_param(),  generate(false),res_x(200.0), res_y(200.0)
{
	osg_image_mixer = new OsgImgMixer(url);
	osg_image_mixer->SetPetitionManager(&osg_petition_manager);
	osg_image_mixer->start();

	last_t = actual_t = osg::Timer::instance()->tick();

	
	

	//osgDB::Registry::instance()->setBuildKdTreesHint(osgDB::ReaderWriter::Options::BUILD_KDTREES);

	
}

OsgScene::~OsgScene()
{
	delete osg_image_mixer;
	
	if (height_calculator != NULL)
	{
		delete height_calculator;
		height_calculator = NULL;
	}

	if(scene_sun != NULL)
	{
		delete scene_sun;
		scene_sun = NULL;
	}	

	
}


osg::Group * OsgScene::GetRenderToTextureCamerasGroup()
{
	if (render_to_texture_cameras == NULL)
		return NULL;
	else
		return render_to_texture_cameras.get();
}

osg::Group * OsgScene::GetCameraGizmosGroup()
{
	if (camera_gizmos == NULL)
		return NULL;
	else
		return camera_gizmos.get();

}


void OsgScene::RepeatOceanUV(osg::Node *node)
{
	if (node == NULL)
		return;

	if (node->className() == std::string("MatrixTransform"))
	{
		
		osg::Geode *geode = (osg::Geode *) ((osg::MatrixTransform *) node)->getChild(0);
		osg::Geometry *geometry = (osg::Geometry *) geode->getDrawable(0);

		osg::ref_ptr<osg::Vec2Array> texcoords = (osg::Vec2Array *) geometry->getTexCoordArray(0);
		osg::ref_ptr<osg::Vec2Array> texcoords2 = (osg::Vec2Array *) geometry->getTexCoordArray(1);
		unsigned int nvertices = geometry->getVertexArray()->getNumElements();

		if (texcoords == NULL)
			return;
		if (texcoords2 == NULL)
			texcoords2 = new osg::Vec2Array(nvertices);

		osg::ref_ptr<osg::StateSet> stateset = geometry->getOrCreateStateSet();
		osg::ref_ptr<osg::Texture2D> texture = (osg::Texture2D *) stateset->getTextureAttribute(0, osg::StateAttribute::TEXTURE);
		texture->setWrap(osg::Texture::WRAP_S, osg::Texture::REPEAT);
		texture->setWrap(osg::Texture::WRAP_T, osg::Texture::REPEAT);

		//second texture
		std::string icon_path = default_directory;
		icon_path += "/water_layer.png";

		osg::ref_ptr<osg::Image> dummy_image = osgDB::readImageFile(icon_path.c_str());
		osg::ref_ptr<osg::Texture2D> dummy_tex = new osg::Texture2D;
		dummy_tex.get()->setDataVariance(osg::Object::DYNAMIC); // protect from being optimized away as static state.
		dummy_tex.get()->setImage(dummy_image.get());
		dummy_tex->setWrap(osg::Texture::WRAP_S, osg::Texture::REPEAT);
		dummy_tex->setWrap(osg::Texture::WRAP_T, osg::Texture::REPEAT);

		for (unsigned int i=0; i < texcoords->size(); i++)
		{
			float x = 0.0f;
			float y = 0.0f;

			x = (*texcoords.get())[i].x();
			y = (*texcoords.get())[i].y();

			(*texcoords.get())[i].set(x * OCEAN_TEXTURE_REPEAT_X, y * OCEAN_TEXTURE_REPEAT_Y);
			//(*texcoords2.get())[i].set(x * OCEAN_TEXTURE_REPEAT_X/21.7f, y * OCEAN_TEXTURE_REPEAT_Y/21.7f);
			//(*texcoords2.get())[i].set(x * OCEAN_TEXTURE_REPEAT_X/37.637f, y * OCEAN_TEXTURE_REPEAT_Y/37.637f);
			(*texcoords2.get())[i].set(x * OCEAN_TEXTURE_REPEAT_X/37.637f, y * OCEAN_TEXTURE_REPEAT_Y/37.637f);
			//(*texcoords2.get())[i].set(x * OCEAN_TEXTURE_REPEAT_X/3.637f, y * OCEAN_TEXTURE_REPEAT_Y/3.637f);
		}

		osg::ref_ptr<osg::Vec3Array> vertex_array = (osg::Vec3Array *) geometry->getVertexArray();

		if (vertex_array == NULL)
			return;

	geometry->setTexCoordArray(1, texcoords2.get());
	osg::ref_ptr<osg::TexEnv> modo_tex = new osg::TexEnv(osg::TexEnv::DECAL);
	stateset.get()->setTextureAttributeAndModes(1,&(*dummy_tex),osg::StateAttribute::ON);
	stateset.get()->setTextureAttribute(1, modo_tex.get(), osg::StateAttribute::ON);
	stateset->setTextureAttribute (0, modo_tex.get());	

	}

}

void OsgScene::InitScene()
{
	if(root_node != NULL) return;

	root_node = new osg::Group;

	render_to_texture_cameras = new osg::Group;
	render_to_texture_cameras->setName(std::string("Render_to_texture_cameras"));
	root_node->addChild( render_to_texture_cameras.get() );

	camera_gizmos = new osg::Group;
	camera_gizmos->setName(std::string("Camera Gizmos"));

	terrain_group = new osg::Group;
	terrain_group->setName("Terrain group");
	root_node->addChild( terrain_group.get() );

	height_calculator = new OsgIntersectorHeightCalculator(terrain_group.get(), this);
	curve_adapter.SetOsgHeightCalculator(height_calculator);	


	//add the temp node which will contain the object which is being
	//manipulated by the user when "add element" option is chosen
	temp_node = new osg::MatrixTransform;
	osg::Matrix trans_matrix;
	trans_matrix.setTrans(osg::Vec3(0.0f, 0.0f, 0.0f));
	temp_node.get()->setMatrix(trans_matrix);
	root_node->addChild(temp_node.get());

	//add the group node which will contain all the layers
	//each child of this node is going to be a Switch node
	//and under the switch node there will be another group node
	//which will contain the objects of the primitives
	layers_node = new osg::Group();
	layers_node.get()->setName(std::string("LayersNode"));

	handlers_node = new osg::Switch();
	handlers_node.get()->setName(std::string("HandlersNode"));
	//root_node->addChild(handlers_node.get());

	cpw::Point3d<float> pos(SKY_POSX,SKY_POSY,SKY_POSZ);
	cpw::Point3d<float> pos_ocean(OCEAN_POSX2,OCEAN_POSY2,OCEAN_POSZ);
	cpw::Point3d<float> orientation(0.0f,0.0f,0.0f);
	cpw::Point3d<float> scale(540000.0f,540000.0f,545000.0f);

	osg::ref_ptr<osg::MatrixTransform> mt_temp = CreateTransformationMatrix(pos_ocean, orientation, scale);//new osg::MatrixTransform();
	//osg::Matrix matrix_temp = mt_temp->getMatrix();
	//matrix_temp.makeTranslate(OCEAN_POSX, OCEAN_POSY, OCEAN_HEIGHT);
	//osg::Matrix m_scale;
	//m_scale.makeScale(1.0, 1.0, 1.0);
	//mt_temp->setMatrix(m_scale * matrix_temp);
	ocean = osgDB::readNodeFile(default_directory + "/mar.osg" );
	
	osg::ref_ptr<osg::MatrixTransform> mt_temp2 = new osg::MatrixTransform();
	osg::Matrix matrix_temp2 = mt_temp2->getMatrix();
	matrix_temp2.makeTranslate(OCEAN_POSX, OCEAN_POSY, OCEAN_HEIGHT-80);
	osg::Matrix m_scale2;
	m_scale2.makeScale(1.0, 1.0, 1.0);
	mt_temp2->setMatrix(m_scale2 * matrix_temp2);
	osg::ref_ptr<osg::Node> ocean2 = osgDB::readNodeFile(default_directory + "/marg.osga" );
	ocean2->setName("--OCEAN-visible--");

	ocean->setNodeMask(10);
	//ocean->setNodeMask(OSG_SCENE_TERRAIN_NODE_MASK);


	osg::StateSet *ocean_ss = ocean->getOrCreateStateSet();
	ocean_ss->setMode(GL_LIGHTING, true);
	ocean_ss->setMode(GL_CULL_FACE,osg::StateAttribute::OFF);
	ocean_ss->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);
	
	ocean->setStateSet(ocean_ss);
	ocean->setName("--OCEAN-collider--");

	
	mt_temp->addChild(ocean.get());
	mt_temp2->addChild(ocean2.get());
	layers_node->addChild(mt_temp.get());
	layers_node->addChild(mt_temp2.get());

	// Codigo para insertar el osg del cielo para colocar la camara es en la linea 80 de OsgNavigator.cpp
	sky = osgDB::readNodeFile(default_directory + "/skydome.osg");

	osg::StateSet *sky_ss = sky->getOrCreateStateSet();
	
	sky_ss->setMode(GL_LIGHTING, true);
	sky_ss->setMode(GL_CULL_FACE,osg::StateAttribute::OFF);
	sky->setStateSet(sky_ss);
	sky->setName("--SKYDOME--");
	
	osg::MatrixTransform *newmatrix = CreateTransformationMatrix(pos, orientation, scale);
	newmatrix->addChild(sky.get());
	layers_node->addChild(newmatrix);

	scene_sun = new iosg::OsgSun(default_directory);
	layers_node->addChild(scene_sun->GetSunMatrixTransform());


	particle_systems_node = new osg::Group();
	particle_systems_node.get()->setName(std::string("ParticleSystemsNode"));
	root_node->addChild(particle_systems_node.get());

	//node that will contain a reference to every 3d object inserted in scene
	//so it does not get out of reference until the application goes down
	referenced_objects = new osg::Switch();
	referenced_objects->setName(std::string("ReferencedObjects"));
	root_node->addChild(referenced_objects.get());


	//node that will contain a reference to every entity inserted in scene
	entities = new osg::Switch();
	entities->setName(std::string("Entities"));
	root_node->addChild(entities.get());

}


bool OsgScene::SetSceneData(void *scene_data)
{
	if(root_node != NULL) 
	{
		if(scene_data != NULL)
		{
			Clear();
			terrain_group = (osg::Group *) scene_data;
			terrain_group->setName("Terrain group");
			root_node->addChild( terrain_group.get() );

			if(height_calculator != NULL)
				delete height_calculator;
			height_calculator = new OsgIntersectorHeightCalculator(terrain_group.get(), this);
			curve_adapter.SetOsgHeightCalculator(height_calculator);	

			root_node->addChild(layers_node.get());
			root_node->addChild(referenced_objects.get());
			root_node->addChild(entities.get());
			root_node->addChild(handlers_node.get());
			root_node->addChild( camera_gizmos.get() );	

			ComputeUtmToSceneFactor();

			//now that the factor is computed transform the scene
			for (unsigned int i=0; i<terrain_group->getNumChildren(); i++)
			{
				osg::Matrix mat3;
				osg::Matrix mat = ((osg::MatrixTransform *)terrain_group->getChild(i))->getMatrix();
				
				osg::Vec3 trans = mat.getTrans();

				cpw::Point3d<float> converted(trans.x(), trans.y(), trans.z());
				converted = UTMToSceneCoords(converted);
				
				mat3.setTrans(converted.x, converted.y, converted.z);
								
				((osg::MatrixTransform *)terrain_group->getChild(i))->setMatrix( mat3 );
			}
			
			if(terrain_group->getNumChildren() == 0)
			{
				Clear();
				return false;
			}
			else
				return true;
		}
		else
			return false;
	}
	else
		return false;
}

void OsgScene::GetDefaultCameraCoords(float &pos_x, float &pos_y, float &pos_z,
		                             float &view_x, float &view_y, float &view_z,
									 float &up_x, float &up_y, float &up_z)
{
	if (terrain_group != NULL)
	{
		osg::BoundingSphere bs = terrain_group->getBound();
		view_x = bs.center().x();
		view_y = bs.center().y();
		view_z = bs.center().z();

		pos_x = bs.center().x();
		pos_y = bs.center().y();
		pos_z = bs.center().z() + (bs.radius()*3.5f);

		up_x = 0.0f;
		up_y = 1.0f;
		up_z = 0.0f;
	}
	else
	{
		view_x = 0.0f;
		view_y = 0.0f;
		view_z = -1.0f;

		pos_x = 0.0f;
		pos_y = 0.0f;
		pos_z = 50.0f;

		up_x = 0.0f;
		up_y = 1.0f;
		up_z = 0.0f;
	}
}


bool OsgScene::CreateGroup(const std::string &group_name, const std::string &parent_group_name)
{
	if(ObjectExistsInScene(group_name))
	{
		return true;
	}
	else
	{
		//create the new group node
		osg::ref_ptr<osg::Switch> new_switch_node = new osg::Switch();
		new_switch_node.get()->setName(group_name.c_str());

		//link it to it´s parent
		entities->addChild(new_switch_node.get());

		return true;
	}	
}


bool OsgScene::SetVisible(const std::string &parent_id, const std::string &object_id, const bool &visible)
{
	if (object_id != "")
	{
		//search the wanted node in the inmediate children
		for (unsigned int i=0; i<entities->getNumChildren(); i++)
		{
			if (entities->getChild(i)->getName() == object_id)
			{
				entities->setChildValue(entities->getChild(i), visible);
				return true;
			}
		}
		//if (visible)
		//	entities->setAllChildrenOn();
		//else
		//	entities->setAllChildrenOff();

		return true;
	}
	return false;
}

std::string OsgScene::NormalizeString(const std::string &str)
{
	std::string str1 = str;
	std::replace(str1.begin(), str1.end(), '\\', '/');

	return str1;
}

osg::Node *OsgScene::GetNode(const std::string &url)
{
    //register the relation
	std::string url1 = NormalizeString(url);
	
	std::map<std::string, osg::Node *>::iterator i = url_node_relation.find(url1);
	
	if (i == url_node_relation.end())
	{
		osg::Node *new_model = osgDB::readNodeFile(url1);
		
		if(new_model != NULL)
		{
		
			url_node_relation[url1] = new_model;
			InsertAsReferencedObject(new_model);

		}

		return new_model;

	}
	else
	{
		return i->second;
	}
	

}



osg::MatrixTransform *  OsgScene::CreateTransformationMatrix(cpw::Point3d<float> pos, 
										  cpw::Point3d<float> orientation,
										  cpw::Point3d<float> scale)
{
	osg::Matrix trans_m, scale_m, orientation_mx, orientation_my, orientation_mz;
		
	//convert from grades to radians
	if (orientation.x != 0.0f)
	{
		orientation.x = (2 * osg::PI * orientation.x) / 360.0f;
		orientation_mx.makeRotate(orientation.x, osg::Vec3(1.0f, 0.0f, 0.0f));
	}
	
	if (orientation.y != 0.0f)
	{
		orientation.y = (2 * osg::PI * orientation.y) / 360.0f;
		orientation_my.makeRotate(orientation.y, osg::Vec3(0.0f, 1.0f, 0.0f));
	}

	if (orientation.z != 0.0f)
	{
		orientation.z = (2 * osg::PI * orientation.z) / 360.0f;
		orientation_mz.makeRotate(orientation.z, osg::Vec3(0.0f, 0.0f, 1.0f));
	}
	
	trans_m.setTrans(osg::Vec3(pos.x, pos.y, pos.z));
	scale_m.identity();
	scale_m.makeScale(osg::Vec3(scale.x, scale.y, scale.z));
	
	osg::MatrixTransform *new_matrix = new osg::MatrixTransform;
	new_matrix->setMatrix(scale_m * orientation_mx * orientation_my * orientation_mz * trans_m);
	return new_matrix;	

}

void OsgScene::CreateTransformationMatrix(cpw::Point3d<float> pos, 
										  cpw::Point3d<float> orientation,
										  cpw::Point3d<float> scale, osg::Matrix &matrix)
{
	osg::Matrix trans_m, scale_m, orientation_mx, orientation_my, orientation_mz;
		
	//convert from grades to radians
	if (orientation.x != 0.0f)
	{
		orientation.x = (2 * osg::PI * orientation.x) / 360.0f;
		orientation_mx.makeRotate(orientation.x, osg::Vec3(1.0f, 0.0f, 0.0f));
	}
	
	if (orientation.y != 0.0f)
	{
		orientation.y = (2 * osg::PI * orientation.y) / 360.0f;
		orientation_my.makeRotate(orientation.y, osg::Vec3(0.0f, 1.0f, 0.0f));
	}

	if (orientation.z != 0.0f)
	{
		orientation.z = (2 * osg::PI * orientation.z) / 360.0f;
		orientation_mz.makeRotate(orientation.z, osg::Vec3(0.0f, 0.0f, 1.0f));
	}
	
	trans_m.setTrans(osg::Vec3(pos.x, pos.y, pos.z));
	scale_m.identity();
	scale_m.makeScale(osg::Vec3(scale.x, scale.y, scale.z));
		
	matrix = scale_m * orientation_mx * orientation_my * orientation_mz * trans_m;

}


bool OsgScene::AddPointSprite(const std::string &id, const cpw::Point3d<float> &pos, float width, float height, const std::string &texture_filename, const float &lod_change, const float &lod_factor)
{
	if (texture_filename == "") 
		return false;

	//osg::Image *new_img = osgDB::readImageFile(texture_filename);
	osg::ref_ptr<osg::Image> new_img = osgDB::readImageFile(texture_filename);
	if (new_img == NULL)
	{
		//try to load the default icon
		new_img = osgDB::readImageFile(default_icon);
		if (new_img == NULL)
			return false;
	}

	OsgNodeFinderVisitor node_finder_visitor(id);
	entities.get()->accept(node_finder_visitor);

	//check if we found the node
	osg::Node *model = node_finder_visitor.GetNode();
	
	if (model == NULL)
	{
		return false;
	}

	std::string debugs = model->className();
	
	//Create the node which will contain the point sprite
	osg::ref_ptr<osg::Geode> geode = new osg::Geode();
    osg::ref_ptr<osg::Geometry> geom = new osg::Geometry();
    osg::ref_ptr<osg::Vec3Array> vertex = new osg::Vec3Array();
    osg::ref_ptr<osg::Vec4Array> colors = new osg::Vec4Array();

	//vertex->push_back(osg::Vec3(pos.x, pos.y, pos.z));
	//vertex->push_back(osg::Vec3(-0.5f * width, 0.5f, (-0.5f * height) - 10000.0f));

	//vertex->push_back(osg::Vec3(0.5f * width, 0.0f, 0.5f * height));
	//vertex->push_back(osg::Vec3(-0.5f * width, 0.0f, -0.5f * height));

	//vertex->push_back(osg::Vec3(0.0f, 0.0f, 1000.0f));
	vertex->push_back(osg::Vec3(0.0f, 0.0f, 0.0f));
	//vertex->push_back(osg::Vec3(0.0f, 0.0f, 1500000.0f));
	vertex->push_back(osg::Vec3(0.0f, 0.0f, 8000000.0f));


	colors->push_back(osg::Vec4(1.0f, 1.0f, 1.0f, 1.0f));
	colors->push_back(osg::Vec4(1.0f, 1.0f, 1.0f, 1.0f));
	
	geom->setVertexArray(vertex.get());
	geom->setColorArray(colors.get());
	geom->setColorBinding(osg::Geometry::BIND_PER_VERTEX);
	geom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::POINTS, 0, 2));
	geode->addDrawable(geom.get());

	//now create the state set associated with the new node
	osg::ref_ptr<osg::StateSet> set = new osg::StateSet();

    /// Setup cool blending
    set->setMode(GL_BLEND, osg::StateAttribute::ON);
	
    osg::BlendFunc *fn = new osg::BlendFunc();
	osg::AlphaFunc *af = new osg::AlphaFunc(osg::AlphaFunc::GREATER, 0.01f);
	fn->setFunction(osg::BlendFunc::SRC_ALPHA, osg::BlendFunc::ONE_MINUS_SRC_ALPHA);
	set->setAttributeAndModes(af, osg::StateAttribute::ON);
    set->setAttributeAndModes(fn, osg::StateAttribute::ON);

    /// Setup the point sprites
	osg::ref_ptr<osg::PointSprite> sprite = new osg::PointSprite();
	set->setTextureAttributeAndModes(0, sprite.get(), osg::StateAttribute::ON);


    /// Give some size to the points to be able to see the sprite
	osg::ref_ptr<osg::Point> point = new osg::Point();
    //point->setSize(width*height);
	point->setSize(30.0f);
	set->setAttribute(point.get());

    /// Disable depth test to avoid sort problems and Lighting
    //set->setMode(GL_DEPTH_TEST, osg::StateAttribute::OFF);
    set->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
	set->setMode(GL_ALPHA_TEST, osg::StateAttribute::ON);
	//glAlphaFunc(GL_GREATER,0.1)


    /// The texture for the sprites
	osg::ref_ptr<osg::Texture2D> tex = new osg::Texture2D();
	//osg::Image *new_img = osgDB::readImageFile(texture_filename);
	new_img->flipVertical();
    tex->setImage(new_img.get());
	set->setTextureAttributeAndModes(0, tex.get(), osg::StateAttribute::ON);


	//Now that the state set is created, link it to the new node
	geode->setStateSet(set.get());

	geode->setName(std::string("-icon-"));
	std::string debugs2 = model->className();

	if(model->className() != std::string("LOD")) return false;
	//link the new node to the scene
	osg::LOD *LOD_node = (osg::LOD *) model/*->getParent(last_parent)->getParent(0)*/;
	//osg::MatrixTransform *source_mt = (osg::MatrixTransform *) model->getParent(last_parent);
	std::string cn, cnch;
	osg::MatrixTransform *source_mt;
	if(LOD_node->getChild(0)->className() == std::string("MatrixTransform"))
	{
		source_mt = (osg::MatrixTransform *) LOD_node->getChild(0);
		cn = source_mt->className();
	}
	else
	{
		cn = LOD_node->getChild(0)->className();
		cnch = ((osg::Group*)LOD_node->getChild(0))->getChild(0)->className();
	}

	bool ps = false;
	if (cn == "MatrixTransform") //adding an icon to a model3d
	{
		osg::ref_ptr<osg::MatrixTransform> mt = new osg::MatrixTransform();
		
		osg::Vec3d trans= source_mt->getMatrix().getTrans();
		
		osg::Matrix new_matrix;
		new_matrix.makeIdentity();
		new_matrix.setTrans(trans);
		mt->setMatrix(new_matrix);

		osg::ref_ptr<iosg::IconUserData> icon_user_data = new IconUserData;
		icon_user_data.get()->SetPosition(cpw::Point3d<float>(trans.x(), trans.y(), trans.z()));
		geode->setUserData(icon_user_data.get());
			
		mt->addChild(geode.get());
		osg::LOD::MinMaxPair mmp = LOD_node->getRangeList()[0];
				
		icon_user_data->SetHeight( 3.5f * (mmp.second / LOD_VALUE));
				
		LOD_node->setCenterMode( osg::LOD::USER_DEFINED_CENTER );
		LOD_node->setCenter(trans);
		osg::LOD::CenterMode cm = LOD_node->getCenterMode();
		LOD_node->addChild(mt.get(), mmp.second, LOD_1_VALUE);
	}
	else if ((cn == "Group")&&(cnch != "Geode")) //adding an icon to a particle system
	{
		cpw::Point3d<float> icon_position(0.0f, 0.0f, 0.0f);
		float radius=0.0f;
		int vertex_count=0;
		bool multi_vertex = false;
		ps = true;
		
		osg::Group *group = (osg::Group *) LOD_node->getChild(0); //the PS is here
	
		for (unsigned int i=0; i<group->getNumChildren(); i++)
		{
			if (group->getChild(i)->className() == std::string("ModularEmitter"))
			{
				//change the vertex coords
				osgParticle::ModularEmitter *me = (osgParticle::ModularEmitter *) group->getChild(i);
				if (me->getPlacer()->className() == std::string("MultiSegmentPlacer"))
				{
					osgParticle::MultiSegmentPlacer *msp = (osgParticle::MultiSegmentPlacer *) me->getPlacer();
					vertex_count = msp->numVertices();
					multi_vertex = true;

					for (int j=0; j<vertex_count; j++)
					{
						icon_position.x += msp->getVertex(j).x();
						icon_position.y += msp->getVertex(j).y();
						icon_position.z += msp->getVertex(j).z();
					}

					if (vertex_count > 0)
					{
						icon_position.x /= vertex_count;
						icon_position.y /= vertex_count;
						icon_position.z /= vertex_count;
					}

					for (int j=0; j<vertex_count; j++)
					{
						cpw::Point3d<float> temp_point(icon_position.x - msp->getVertex(j).x(),
							                           icon_position.y - msp->getVertex(j).y(), 
													   icon_position.z - msp->getVertex(j).z());

						float radius_temp = sqrt(temp_point.x * temp_point.x + temp_point.y * temp_point.y + temp_point.z * temp_point.z);

						if (radius_temp > radius)
							radius = radius_temp;

					}

				}
				else
				{
					icon_position.x = LOD_node->getCenter().x();
					icon_position.y = LOD_node->getCenter().y();
					icon_position.z = LOD_node->getCenter().z();
					radius = 1000.0f;
				}
			}
		}

		osg::ref_ptr<osg::MatrixTransform> mt = new osg::MatrixTransform();

		osg::Matrix matrix = mt->getMatrix();
		matrix.makeIdentity();
		matrix.setTrans(icon_position.x, icon_position.y, icon_position.z);
		mt->setMatrix(matrix);

		osg::ref_ptr<iosg::IconUserData> icon_user_data = new IconUserData;
		icon_user_data.get()->SetPosition(icon_position);
		geode->setUserData(icon_user_data.get());
			
		mt->addChild(geode.get());
		osg::Matrix m;
		m.makeIdentity();
		float distance = ComputeLODDistance(osg::BoundingSphere(osg::Vec3(icon_position.x, icon_position.y, icon_position.z), radius * 0.3f), m);
		
		if (!ps)
		//if (true)
		{
			LOD_node->setRange(0, 0.0f, distance);
			LOD_node->addChild(mt.get(), distance, LOD_1_VALUE);
		}
		else
		{
			//if (lod_change != 0.0f)
			//	LOD_node->addChild(mt.get(), lod_change * LOD_MULT, LOD_1_VALUE);
			if (lod_change != 0.0f /*&& lod_factor != 0.0f*/)
				LOD_node->addChild(mt.get(), lod_change * /*lod_factor **/ LOD_MULT, LOD_1_VALUE);
			else
				LOD_node->addChild(mt.get(), LOD_0_VALUE, LOD_1_VALUE);
		}

		//now specify the range of the smok
	}
	else if ((cn == "Group")&&(cnch == "Geode"))
	{
		cpw::Point3d<float> icon_position(0.0f, 0.0f, 0.0f);
		icon_position.x = LOD_node->getCenter().x();
		icon_position.y = LOD_node->getCenter().y();
		icon_position.z = LOD_node->getCenter().z();

		osg::ref_ptr<osg::MatrixTransform> mt = new osg::MatrixTransform();
		osg::Matrix new_matrix;
		new_matrix.makeIdentity();
		new_matrix.setTrans(icon_position.x, icon_position.y, icon_position.z);
		mt->setMatrix(new_matrix);
		
		osg::ref_ptr<iosg::IconUserData> icon_user_data = new IconUserData;
		icon_user_data.get()->SetPosition(icon_position);
		geode->setUserData(icon_user_data.get());
			
		mt->addChild(geode.get());
		//LOD_node->setRange(0, 0.0f, LOD_0_VALUE);
		LOD_node->addChild(mt.get(), LOD_0_VALUE, LOD_1_VALUE);
	}
//	LOD_node->setAllChildrenOn();

	return true;
}



void OsgScene::UpdateIconsSize(float max_size, float min_size, float max_distance, float min_distance, const cpw::Point3d<float> &camera_pos, float max_height, float min_height)
{
	//create the visitor and launch it
	OsgIconSizeUpdaterVisitor icon_size_updater_visitor(max_size, min_size, max_distance, min_distance,
														camera_pos.x, camera_pos.y, camera_pos.z, max_height, min_height);

	if (entities !=NULL) 
		entities.get()->accept(icon_size_updater_visitor);
}




osgParticle::ModularEmitter* OsgScene::AddParticleSystem(const std::string &img_file,
														const std::string &ps_id,
														const std::string &group_name,														 
													    const cpw::Range<int> &new_particles_sec,
  													    const float particles_life, 
 													    const cpw::Range<float> &alpha ,
													    const bool senoidal_alpha_interpolation, 
													    const bool additive_blend,
													    const cpw::Range<float> &speed ,
													    const cpw::Range<float> &phi ,
													    const cpw::Range<float> &theta ,		   
													    const cpw::Range<float> &size,
														const cpw::Point3d<float> &center,
														const float &lod_frontier)
														//,
														//const int tiles)
 {
	//Search for the switch node
	OsgNodeFinderVisitor node_finder_visitor(group_name);
	entities.get()->accept(node_finder_visitor);

	//check if we found the node
	osg::Node *found_node = node_finder_visitor.GetNode();
	
	if (found_node == NULL)
		return false;

	//check if the node is a switch node
	if (found_node->className() != std::string("Switch"))
		return false;

	

	//el sistema global
	osg::ref_ptr<osgParticle::ParticleSystem> ps = new osgParticle::ParticleSystem;
	ps->setDefaultAttributes(img_file, additive_blend, false);

	//Definimos las propiedades de las partículas en este template
	osgParticle::Particle ptemplate;
	ptemplate.setLifeTime(particles_life);

	if (senoidal_alpha_interpolation)
		ptemplate.setAlphaInterpolator(new OsgSenoidalInterpolator);
	
	ptemplate.setSizeRange(osgParticle::rangef(size.min, size.max));
	ptemplate.setAlphaRange(osgParticle::rangef(alpha.min, alpha.max));
    ptemplate.setColorRange(osgParticle::rangev4(
          //osg::Vec4(1, 0.8f, 0.2f, 1.0f), 
          //osg::Vec4(1, 0.3f, 0.2f, 0.0f)));
		  osg::Vec4(0.5f, 0.5f, 0.5f, 0.0f), 
          osg::Vec4(1.0f, 1.0f, 1.0f, 1.0f)));

	osg::ref_ptr<osg::Image> image = osgDB::readImageFile(img_file);
	if(image != NULL)
	{
		int tile_width = image->s();
		int tile_height = image->t();
		int tiles = tile_width/tile_height;	
	
		ptemplate.setTextureTile(tiles, 1, tiles);
		//ptemplate.setTextureTile(3, 1, 3);
	}
	
    // these are physical properties of the particle
    ptemplate.setRadius(PARTICLERADIO);    // 5 cm wide particles
    ptemplate.setMass(PARTICLEWEIGHT);    // 50 g heavy
	ps->setDefaultParticleTemplate(ptemplate);
	
	//creamos el emisor y luego se lo pasamos al nodo root
	osg::ref_ptr<osgParticle::ModularEmitter> emisor = new osgParticle::ModularEmitter;
	emisor->setParticleSystem(ps.get());

	//ritmo de generacion de las particulas
	//osgParticle::RandomRateCounter *rrc = static_cast<osgParticle::RandomRateCounter *>(emisor->getCounter());
	//rrc->setRateRange(new_particles_sec.min, new_particles_sec.max);    // ritmo de generacion
	//emisor->setCounter(rrc);

	//preparamos el shooter
    osgParticle::RadialShooter *shooter = new osgParticle::RadialShooter;
    //shooter->setInitialSpeedRange(40, 60);
	shooter->setInitialSpeedRange(speed.min, speed.max);
	//shooter->setInitialSpeedRange(0.4f, 0.6f);
	shooter->setThetaRange((theta.min/180.0f) * osg::PI, (theta.max/180.0f) * osg::PI);
	shooter->setPhiRange((phi.min/180.0f) * osg::PI, (phi.max/180.0f) * osg::PI);
    emisor->setShooter(shooter);

	

	osg::ref_ptr<osg::Geode> geode = new osg::Geode;    
    geode->addDrawable(ps.get());

    osg::ref_ptr<osgParticle::ParticleSystemUpdater> psu = new osgParticle::ParticleSystemUpdater;
    psu->addParticleSystem(ps.get());
    
    // add the updater node to the scene graph
	osg::ref_ptr<osg::Group> new_group = new osg::Group;
	new_group->setName(ps_id);
	new_group->addChild(emisor.get());
    new_group->addChild(geode.get());
    new_group->addChild(psu.get());

	/////////
	/*osg::ref_ptr<OsgPSCallback> callback2 = new OsgPSCallback;
	callback2.get()->SetID(10);
	new_group->setCullCallback( callback2.get() );*/
	//////////

	osgParticle::RandomRateCounter *rrc = static_cast<osgParticle::RandomRateCounter *>(emisor->getCounter());
	rrc->setRateRange(new_particles_sec.min, new_particles_sec.max);    // ritmo de generacion
	emisor->setCounter(rrc);

	//Create the groups which will hold the 3 items needed for the PS
	std::vector< osg::ref_ptr<osg::Group> > groups;
	osg::ref_ptr< osg::Group > last_group;
	for (unsigned int i=0; i<MAX_PS_LODS; i++)
	{
		const float init_casd = 100.0f;

		osg::ref_ptr<osg::Group> new_subgroup = new osg::Group;
		osg::ref_ptr<OsgPSCallback> callback = new OsgPSCallback;

		callback.get()->SetID(i);

		new_subgroup->addChild(psu.get());
		new_subgroup->addChild(emisor.get());
		new_subgroup->addChild(geode.get());
		new_subgroup->setCullCallback(callback.get());
		new_subgroup->setName(ps_id);

		callback->SetEmitter(emisor.get());
		callback->SetParticleSystem(ps.get());
		callback->SetRandomRateCounter(rrc);
		callback->SetCurveAdapter(&curve_adapter);
		callback->SetCurveAdapterStepDistance(init_casd * (i+1)); //set the curve adapter step distance

		//link the callbacks
		if (i>0)
		{
			OsgPSCallback *temp = (OsgPSCallback *)(groups[i-1].get()->getCullCallback());
			temp->SetRightBrother(callback.get());
			callback->SetLeftBrother(temp);
		}

		//set the new particles rate for all the groups
		if (i+1 == MAX_PS_LODS)
		{
			callback->SetNewParticlesPerSecond(cpw::Range<int>(0, 0));  //the last one will switch off the ps
			last_group = new_subgroup.get();
		}
		else
		{
			callback->SetNewParticlesPerSecond(cpw::Range<int>((int)(new_particles_sec.min * (1.0f / (i+1))) ,
											  (int)(new_particles_sec.max * (1.0f / (i+1)))));		

			//set the particles size for all the groups
			callback->SetParticlesSize(cpw::Range<float>(size.min * (i+1), size.max * (i+1)));
			groups.push_back( new_subgroup.get() );
		}
		         
	}

	//create the LOD node to choose between the icon and the PS
	osg::ref_ptr<osg::LOD> new_lod = new osg::LOD();

	//new_lod->addChild(new_group.get(), 0.0f, LOD_0_VALUE);

	float lod_frontier_ = lod_frontier;
	if (lod_frontier_ == 0.0f)
		lod_frontier_ = LOD_0_VALUE * LOD_MULT /** LOD_MULTF*/;
	else
		lod_frontier_ *= LOD_MULT /** LOD_MULTF*/;

	//add the new groups
	for (unsigned int i=0; i<groups.size(); i++)
	{
		if (i==0) //first
			new_lod->addChild(groups[i].get(), 0.0, lod_frontier_ / (groups.size()));
		else if (i == (groups.size() - 1)) //last
			new_lod->addChild(groups[i].get(), (lod_frontier_ / (groups.size() - i + 1)), lod_frontier_);
		else
			new_lod->addChild(groups[i].get(), (lod_frontier_ / (groups.size() - i + 1)), lod_frontier_ / (groups.size() - i ) );
	}

	//last callback
	new_lod->addChild(last_group.get(), lod_frontier_, LOD_1_VALUE);
		
	new_lod->setCenter(osg::Vec3(center.x, center.y, center.z));
		
	((osg::Switch*)found_node)->addChild(new_lod.get());
	new_lod->setName(ps_id);
	//((osg::Switch*)found_node)->addChild(new_group.get());

	emisor->setNumParticlesToCreateMovementCompensationRatio(1.5f);

	

	return emisor.get();

}

void OsgScene::AddLineParticleSystem(const cpw::Point3d<float> &v1, const cpw::Point3d<float> &v2,											   
									 const std::string &img_file, const std::string &ps_id,
									 const std::string &group_name,
									 const cpw::Range<int> &new_particles_sec,
									 const float particles_life, 
									 const cpw::Range<float> &alpha ,
	                                 const bool senoidal_alpha_interpolation, const bool additive_blend,
								     const cpw::Range<float> &speed ,
								     const cpw::Range<float> &phi ,
								     const cpw::Range<float> &theta ,		   
								     const cpw::Range<float> &size )
{
	cpw::Point3d<float> converted_point1 = UTMToSceneCoords(v1);
	cpw::Point3d<float> converted_point2 = UTMToSceneCoords(v2);

	osgParticle::ModularEmitter* emisor = AddParticleSystem(img_file, ps_id, group_name,new_particles_sec,particles_life,alpha, 
															senoidal_alpha_interpolation, additive_blend, speed, phi, theta, size, converted_point1);

	osgParticle::SegmentPlacer *placer = new osgParticle::SegmentPlacer;
	placer->setSegment(osg::Vec3(converted_point1.x, converted_point1.y, converted_point1.z),osg::Vec3(converted_point2.x, converted_point2.y, converted_point2.z));
    emisor->setPlacer(placer);
}

void OsgScene::AddMultiLineParticleSystem(const cpw::Point3d<float> &v,											   
									 const std::string &img_file, const std::string &ps_id,
									 const std::string &group_name,
									 const cpw::Range<int> &new_particles_sec,
									 const float particles_life, 
									 const cpw::Range<float> &alpha ,
	                                 const bool senoidal_alpha_interpolation, const bool additive_blend,
								     const cpw::Range<float> &speed ,
								     const cpw::Range<float> &phi ,
								     const cpw::Range<float> &theta ,		   
								     const cpw::Range<float> &size )
{
	cpw::Point3d<float> converted_point = UTMToSceneCoords(v);

	osgParticle::ModularEmitter* emisor = AddParticleSystem(img_file, ps_id, group_name,new_particles_sec,particles_life,alpha, 
															senoidal_alpha_interpolation, additive_blend, speed, phi, theta, size, converted_point);

		
	osgParticle::MultiSegmentPlacer *placer = new osgParticle::MultiSegmentPlacer;
	placer->addVertex(converted_point.x, converted_point.y, converted_point.z);
	placer->addVertex(converted_point.x+0.1f, converted_point.y, converted_point.z);

	emisor->setPlacer(placer);
	//fe->setUpEmitterAndProgram();
}

void OsgScene::AddMultiLineParticleSystem(const std::vector< cpw::Point3d<float> > &v,											   
											   const std::string &img_file, const std::string &ps_id,
											   const std::string &group_name,
											   const cpw::Range<int> &new_particles_sec,
											   const float particles_life, 
											   const cpw::Range<float> &alpha,
											   const bool senoidal_alpha_interpolation, 
											   const bool additive_blend,
											   const cpw::Range<float> &speed,
											   const cpw::Range<float> &phi,
											   const cpw::Range<float> &theta,		   
											   const cpw::Range<float> &size,
											   const float &max_radius)
{
	if (v.size() < 2)
		return;

	std::vector< cpw::Point3d<float> > scene_vec, final_vec;

	for (std::vector< cpw::Point3d<float> >::const_iterator iter = v.begin();
		 iter != v.end();
		 iter++)
	{
		scene_vec.push_back( UTMToSceneCoords(*iter) );
	}

	curve_adapter.Adapt(scene_vec, final_vec);

	osgParticle::ModularEmitter* emisor = AddParticleSystem(img_file, ps_id, group_name,new_particles_sec,particles_life,alpha, 
															senoidal_alpha_interpolation, additive_blend, speed, phi, theta, size, *final_vec.begin(), max_radius);

	osgParticle::MultiSegmentPlacer *placer = new osgParticle::MultiSegmentPlacer;

	for ( std::vector< cpw::Point3d<float> >::iterator iter = final_vec.begin();
		iter != final_vec.end();
		iter++)
	{
		placer->addVertex(iter->x, iter->y, iter->z);
	}
	
	emisor->setPlacer(placer);
}


bool OsgScene::AddVertexToMultiLineParticleSystem(const cpw::Point3d<float> &v, const std::string &ps_id)
{
	//Search for the particle system node
	OsgNodeFinderVisitor node_finder_visitor(ps_id);
	entities.get()->accept(node_finder_visitor);

	//check if we found the node
	osg::Node *found_node = node_finder_visitor.GetNode();
		
	if (found_node == NULL)
		return false;

	//check if it´s a LOD
	if (found_node->className() != std::string("LOD"))
		return false;

	found_node = ((osg::LOD *) found_node)->getChild(0);

	if (found_node->className() != std::string("Group"))
		return false;

	osg::Group *group_node = (osg::Group *) found_node;

	for (unsigned int i=0; i<group_node->getNumChildren(); i++)
	{
		if (group_node->getChild(i)->className() == std::string("ModularEmitter"))
		{
			osgParticle::ModularEmitter *me = (osgParticle::ModularEmitter *) group_node->getChild(i);
			if (me->getPlacer()->className() != std::string("MultiSegmentPlacer"))
				return false;

			cpw::Point3d<float> converted_v = UTMToSceneCoords(v);
			((osgParticle::MultiSegmentPlacer *)me->getPlacer())->addVertex(converted_v.x, converted_v.y, converted_v.z);
			return true;
		}
	}

	return true;
}


void OsgScene::AddPuntualParticleSystem(const cpw::Point3d<float> &v,											   
									    const std::string &img_file, const std::string &ps_id,
									    const std::string &group_name,
									    const cpw::Range<int> &new_particles_sec,
									    const float particles_life, 
									    const cpw::Range<float> &alpha,
									    const bool senoidal_alpha_interpolation, 
									    const bool additive_blend,
									    const cpw::Range<float> &speed,
									    const cpw::Range<float> &phi,
									    const cpw::Range<float> &theta,		   
									    const cpw::Range<float> &size)
{
	cpw::Point3d<float> converted_point = UTMToSceneCoords(v);

	osgParticle::ModularEmitter* emisor = AddParticleSystem(img_file, ps_id, group_name,new_particles_sec,particles_life,alpha, 
															senoidal_alpha_interpolation, additive_blend, speed, phi, theta, size, converted_point);

	//donde van a estar las particulas
    
	if (emisor == NULL)
		return;

	cpw::Point3d<float> converted_v = UTMToSceneCoords(v);
	osgParticle::PointPlacer *placer = new osgParticle::PointPlacer();
	placer->setCenter(converted_v.x, converted_v.y, converted_v.z);
	emisor->setPlacer(placer);	
}

bool OsgScene::UpdateMultiLineParticleSystemVertices(const std::vector< cpw::Point3d<float> > &new_vertices, const std::string &ps_id, const bool &use_last_computed_vertices)
{
	//Search for the particle system node
	OsgNodeFinderVisitor node_finder_visitor(ps_id, false);
	entities.get()->accept(node_finder_visitor);

	//check if we found the node
	osg::Node *found_node = node_finder_visitor.GetNode();
		
	if (found_node == NULL)
		return false;

	//check if the node is a switch node
	if (found_node->className() != std::string("Group"))
		return false;

	osg::Group *group_node = (osg::Group *) found_node;

	for (unsigned int i=0; i<group_node->getNumChildren(); i++)
	{
		if (group_node->getChild(i)->className() == std::string("ModularEmitter"))
		{
			//change the vertex coords
			osgParticle::ModularEmitter *me = (osgParticle::ModularEmitter *) group_node->getChild(i);
			if (me->getPlacer()->className() != std::string("MultiSegmentPlacer"))
				return false;

			//remove all the vertices
			while (((osgParticle::MultiSegmentPlacer *)me->getPlacer())->numVertices() > 0)
			{
				((osgParticle::MultiSegmentPlacer *)me->getPlacer())->removeVertex(0);
			}

			//insert them all again
			if (!use_last_computed_vertices)
			{
				//convert to UTM
				std::vector< cpw::Point3d<float> > scene_coords_vec;
				for (std::vector< cpw::Point3d<float> >::const_iterator iter = new_vertices.begin();
					iter != new_vertices.end();
					iter++)
				{
					scene_coords_vec.push_back( UTMToSceneCoords(*iter) );
				}

				CutDownVerticesIfNeccesary(scene_coords_vec);
				float last_step_distance = curve_adapter.GetStepDistance();
				curve_adapter.SetStepDistance( SuggestParticleSystemStepDistance() );
				last_computed_vertices_lying_on_terrain.clear();
				curve_adapter.Adapt(scene_coords_vec, last_computed_vertices_lying_on_terrain);
				//last_computed_vertices_lying_on_terrain = scene_coords_vec;
				curve_adapter.SetStepDistance(last_step_distance);
				
			}
			

			for (std::vector< cpw::Point3d<float> >::iterator iter = last_computed_vertices_lying_on_terrain.begin();
					iter != last_computed_vertices_lying_on_terrain.end();
					iter++)
			{
					((osgParticle::MultiSegmentPlacer *)me->getPlacer())->addVertex(osg::Vec3(iter->x, iter->y, iter->z));
			}
		
			
			return true;
		}
	}
	return true;

}
bool OsgScene::UpdateMultiLineParticleSystemVertex(const int vertex_index, const cpw::Point3d<float> &v, const std::string &ps_id)
{
	//Search for the particle system node
	OsgNodeFinderVisitor node_finder_visitor(ps_id);
	entities.get()->accept(node_finder_visitor);

	//check if we found the node
	osg::Node *found_node = node_finder_visitor.GetNode();
		
	if (found_node == NULL)
		return false;

	//check if the node is a switch node
	if (found_node->className() != std::string("Group"))
		return false;

	osg::Group *group_node = (osg::Group *) found_node;

	for (unsigned int i=0; i<group_node->getNumChildren(); i++)
	{
		if (group_node->getChild(i)->className() == std::string("ModularEmitter"))
		{
			//change the vertex coords
			osgParticle::ModularEmitter *me = (osgParticle::ModularEmitter *) group_node->getChild(i);
			if (me->getPlacer()->className() != std::string("MultiSegmentPlacer"))
				return false;

			int hola = ((osgParticle::MultiSegmentPlacer *)me->getPlacer())->numVertices();
			cpw::Point3d<float> converted_v = UTMToSceneCoords(v);
			((osgParticle::MultiSegmentPlacer *)me->getPlacer())->setVertex(vertex_index, converted_v.x, converted_v.y, converted_v.z);

			//delete the particles which are not dying soon enough
			osgParticle::ParticleSystem *ps = me->getParticleSystem();
			double age, life_time;
			osgParticle::Particle *particle;

			for (int j=0; j<ps->numParticles(); j++)
			{
				particle = ps->getParticle(j);
				age = particle->getAge();
				life_time = particle->getLifeTime();

				//no method for setting age???
				if ((life_time - age) > 2.0)
					ps->destroyParticle(j);				
				
				//ps->destroyParticle(j);
			}
						
			return true;
		}
	}
	return true;
}

bool OsgScene::UpdateParticleSystemProperties()
{
	return true;

}


void OsgScene::DeleteParticleSystemFromScene(const std::string &ps_id, const std::string &parent_id)
{
	//Search for the particle system node
	OsgNodeFinderVisitor node_finder_visitor(parent_id);
	entities.get()->accept(node_finder_visitor);

	//check if we found the node
	osg::Node *found_node = node_finder_visitor.GetNode();
		
	if (found_node == NULL)
		return;

	//check if the node is a switch node
	if (found_node->className() != std::string("Switch"))
		return;

	osg::Switch *switch_node = (osg::Switch *) found_node;

	for (unsigned int i=0; i<switch_node->getNumChildren(); i++)
	{
		if (switch_node->getChild(i)->getName() == ps_id)
		{
			switch_node->removeChild(i);
			return;
		}
	}
}

void OsgScene::UpdatePuntualParticleSystem(const cpw::Point3d<float> position,
										const std::string &img_file,
										const std::string &ps_id,										
										const cpw::Range<int> &new_particles_sec,
										const float particles_life, 
										const cpw::Range<float> &alpha,
										const bool senoidal_alpha_interpolation, 
										const bool additive_blend,
										const cpw::Range<float> &speed,
										const cpw::Range<float> &phi,
										const cpw::Range<float> &theta,		   
										const cpw::Range<float> &size)
{
	//Search for the particle system node
	OsgNodeFinderVisitor node_finder_visitor(ps_id);
	entities.get()->accept(node_finder_visitor);

	//check if we found the node
	osg::Node *found_node = node_finder_visitor.GetNode();
		
	if (found_node == NULL)
		return;

	//check if the node is a switch node
	if (found_node->className() != std::string("Group"))
		return;

	osg::Group *group = (osg::Group *) found_node;

	for (unsigned int i=0; i<group->getNumChildren(); i++)
	{
		osg::Node *child = group->getChild(i);
		if (child->className() == std::string("ModularEmitter"))
		{
			osgParticle::ModularEmitter *emitter = (osgParticle::ModularEmitter *)child;
			osgParticle::PointPlacer *pplacer = (osgParticle::PointPlacer *) emitter->getPlacer();

			//update position
			cpw::Point3d<float> converted_v = UTMToSceneCoords(position);
			pplacer->setCenter(converted_v.x, converted_v.y, converted_v.z);

			//Definimos las propiedades de las partículas en este template
			osgParticle::Particle ptemplate;
			ptemplate.setLifeTime(particles_life);

			if (senoidal_alpha_interpolation)
				ptemplate.setAlphaInterpolator(new OsgSenoidalInterpolator);
	
			ptemplate.setSizeRange(osgParticle::rangef(size.min, size.max));
			ptemplate.setAlphaRange(osgParticle::rangef(alpha.min, alpha.max));
			ptemplate.setColorRange(osgParticle::rangev4(osg::Vec4(1, 1.0f, 1.0f, 1.0f), 
														 osg::Vec4(1, 1.0f, 1.0f, 1.0f)));
	
			// these are physical properties of the particle
			ptemplate.setRadius(PARTICLERADIO);    // 5 cm wide particles
			ptemplate.setMass(PARTICLEWEIGHT);    // 50 g heavy
			emitter->getParticleSystem()->setDefaultParticleTemplate(ptemplate);
			
			//emitter new particles / sec
			osgParticle::RandomRateCounter *rrc = static_cast<osgParticle::RandomRateCounter *>(emitter->getCounter());
			rrc->setRateRange(new_particles_sec.min, new_particles_sec.max);   
			emitter->setCounter(rrc);

			//update the shooter
			//preparamos el shooter
			osgParticle::RadialShooter *shooter = (osgParticle::RadialShooter *)emitter->getShooter();
			shooter->setInitialSpeedRange(speed.min, speed.max);
			shooter->setThetaRange((theta.min/180.0f) * osg::PI, (theta.max/180.0f) * osg::PI);
			shooter->setPhiRange((phi.min/180.0f) * osg::PI, (phi.max/180.0f) * osg::PI);
			return;
			//emisor->setShooter(shooter);

		}
	}
}


bool OsgScene::UpdateParticleSystemParticleGeneratorRate(const std::string &ps_id, const cpw::Range<int> &new_rate)
{
	
	
	//V2
	//Search for the particle system node
	OsgNodeFinderVisitor node_finder_visitor(ps_id, true);
	entities.get()->accept(node_finder_visitor);

	//check if we found the node
	osg::Node *found_node = node_finder_visitor.GetNode();
		
	if (found_node == NULL)
		return false;

	if (found_node->className() != std::string("LOD"))
		return false;

	cpw::Range<int> _new_rate(new_rate);

	switch (GetPerformance())
	{
		case VERY_BAD :
			_new_rate.min *= 0.2f;
			_new_rate.max *= 0.2f;
			break;

		case BAD : 
			_new_rate.min *= 0.4f;
			_new_rate.max *= 0.4f;
			break;

		case MEDIUM : 
			_new_rate.min *= 0.75f;
			_new_rate.max *= 0.75f;
			break;

		default:
			break;
	}

	if ((_new_rate.min <= 0) && (new_rate.min > 0))
		_new_rate.min = 1;

	if ((_new_rate.max <= 0) && (new_rate.max > 0))
		_new_rate.max = 1;

	osg::LOD *lod = (osg::LOD *) found_node;

	for (unsigned int i=0; i<MAX_PS_LODS; i++)
	{
		//osg::Node *child = (OsgPSCallback *) lod->getChild(i)->getCullCallback();
		OsgPSCallback *ps_callback = (OsgPSCallback *) lod->getChild(i)->getCullCallback();

		if (i+1 == MAX_PS_LODS)
			ps_callback->SetNewParticlesPerSecond(cpw::Range<int>(0, 0));
		else
		{
			ps_callback->SetNewParticlesPerSecond(cpw::Range<int>((int)(_new_rate.min * (1.0f / (i+1))) ,
																				  (int)(_new_rate.max * (1.0f / (i+1)))));
		}

		ps_callback->SetNeedToAdapt(false);
	}

	//the last one


	return false;
}

bool OsgScene::UpdateParticleSystemParticleLife(const std::string &ps_id, float new_life)
{
	OsgNodeFinderVisitor node_finder_visitor(ps_id, false);
	entities.get()->accept(node_finder_visitor);

	//check if we found the node
	osg::Node *found_node = node_finder_visitor.GetNode();
		
	if (found_node == NULL)
		return false;

	//check if the node is a switch node
	if (found_node->className() != std::string("Group"))
		return false;

	osg::Group *group = (osg::Group *) found_node;

	for (unsigned int i=0; i<group->getNumChildren(); i++)
	{
		osg::Node *child = group->getChild(i);
		if (child->className() == std::string("ModularEmitter"))
		{
			osgParticle::ModularEmitter *emitter = (osgParticle::ModularEmitter *)child;
			
			osgParticle::ParticleSystem *ps = emitter->getParticleSystem();
			
			osgParticle::Particle ptemplate = ps->getDefaultParticleTemplate();
			ptemplate.setLifeTime(new_life);
			ps->setDefaultParticleTemplate(ptemplate);			
			
			return true;
		}
	}

	return false;
}
bool OsgScene::UpdateParticleSystemParticleSize(const std::string &ps_id, const cpw::Range<float> &new_size)
{
	OsgNodeFinderVisitor node_finder_visitor(ps_id, false);
	entities.get()->accept(node_finder_visitor);

	//check if we found the node
	osg::Node *found_node = node_finder_visitor.GetNode();
		
	if (found_node == NULL)
		return false;

	//check if the node is a switch node
	if (found_node->className() != std::string("Group"))
		return false;

	osg::Group *group = (osg::Group *) found_node;

	for (unsigned int i=0; i<group->getNumChildren(); i++)
	{
		osg::Node *child = group->getChild(i);
		
		if (child->className() == std::string("ModularEmitter"))
		{
			osgParticle::ModularEmitter *emitter = (osgParticle::ModularEmitter *)child;

			osgParticle::ParticleSystem *ps = emitter->getParticleSystem();
			
			osgParticle::Particle ptemplate = ps->getDefaultParticleTemplate();
			ptemplate.setSizeRange(osgParticle::rangef(new_size.min, new_size.max));
			ps->setDefaultParticleTemplate(ptemplate);
						
			return true;
		}

		
	}

	return false;
}


bool OsgScene::AddObjectToGroup(const std::string &object_id, const std::string &group_name, const std::string &new_object,
											cpw::Point3d<float> &pos,
											cpw::Point3d<float> &rotation,
											cpw::Point3d<float> &scale,
											bool visible, 
											const std::string &name)
{
	if(ObjectExistsInScene(object_id))  
	{
		return true;
	}
	else
	{
		osg::Node *node = GetNode(new_object);
		//node->setName(object_id);

		if (node != NULL)
		{
			osg::Switch *parent_node;
			if(group_name != "")
			{
				////Search for the switch node
				OsgNodeFinderVisitor node_finder_visitor(group_name);
				entities.get()->accept(node_finder_visitor);

				//check if we found the node
				osg::Node *found_node = node_finder_visitor.GetNode();
				
				if (found_node == NULL)
					return false;

				//check if the node is a switch node
				if (found_node->className() != std::string("Switch"))
					return false;

				parent_node = (osg::Switch *) found_node;
			}
			else
				parent_node = entities.get();


			//create a new LOD node which will choose between the icon or the object
			osg::ref_ptr<osg::LOD> new_LOD_node = new osg::LOD();
			new_LOD_node->setNodeMask(OSG_SCENE_ENTITIES_NODE_MASK);
			parent_node->addChild(new_LOD_node.get());

			cpw::Point3d<float> converted_pos = UTMToSceneCoords(pos);
			osg::ref_ptr<osg::MatrixTransform> transformation_matrix = OsgScene::CreateTransformationMatrix(converted_pos, rotation, scale);
			
			float lod1 = ComputeLODDistance(node->getBound(), transformation_matrix->getMatrix());
			new_LOD_node->addChild(transformation_matrix.get(), 0.0f, lod1);

			new_LOD_node->setName(object_id);
			transformation_matrix->addChild(node);

			std::string font_path = default_directory;
			osg::AutoTransform *autoText = new osg::AutoTransform;
			autoText->setPosition(osg::Vec3(0.0,0.0,10.0));
			autoText->setAutoRotateMode(osg::AutoTransform::ROTATE_TO_SCREEN);
			cpw::Point3d<float> scale_inv = cpw::Point3d<float> (1/scale.x, 1/scale.y, 1/scale.z);
			cpw::Point3d<float> rotation_inv = cpw::Point3d<float> (-1*rotation.x, -1*rotation.y, -1*rotation.z);
			osg::ref_ptr<osg::MatrixTransform> transformation_matrix_2 = new osg::MatrixTransform;
			osg::Matrix tm_matrix = transformation_matrix->getInverseMatrix();
			osg::Matrix tm_aux;
			tm_aux.makeIdentity();
			tm_matrix.setTrans(tm_aux.getTrans());
			transformation_matrix_2->setMatrix(tm_matrix);
			transformation_matrix->addChild(transformation_matrix_2.get());
			autoText->setAutoScaleToScreen(true);
			transformation_matrix_2->setNodeMask(OSG_SCENE_LABEL_NODE_MASK);

			osgText::Font* _font = osgText::readFontFile(font_path+"/ARIAL.TTF");
			osgText::Text* _text = new osgText::Text;
			osg::Geode* geode  = new osg::Geode;
			//cpw::Point3d<float> converted_position = UTMToSceneCoords(cpw::Point3d<float> (converted_pos.x, converted_pos.y, converted_pos.z+20);
			_text->setFont(_font);
			_text->setFontResolution(20,20);
			_text->setCharacterSize(16);
			float text_offset = 0;//-1*(_text->getGLObjectSizeHint()/2);
			_text->setText(name);
			_text->setBackdropType(osgText::Text::OUTLINE);
			_text->setBackdropOffset(0.2,0.2);
			//_text->setBackdropColor(
			_text->setAlignment(osgText::Text::CENTER_CENTER);
			_text->setLayout(osgText::Text::LEFT_TO_RIGHT);
			//_text->setAxisAlignment(osgText::Text::SCREEN);
			_text->setPosition(osg::Vec3(text_offset, 0, 12));
			geode->addDrawable(_text);
			////autoText->setPosition(osg::Vec3(0.0,0.0,1.0+geode->getBoundingBox().zMax()));
			//autoText->setPosition(osg::Vec3(0.0,0.0,14.0*scale.z));
			////autoText->setPosition(osg::Vec3(0.0,0.0,1.0+( ((osg::Geode*) node)->getBoundingBox().zMax()) ));
			autoText->setPosition( osg::Vec3( 0.0,0.0,1.0+scale.z*2*( node->getBound().radius() )));

			osg::StateSet *text_ss = geode->getOrCreateStateSet();
			text_ss->setMode(GL_LIGHTING, false);
			geode->setStateSet(text_ss);

			autoText->addChild(geode);
			//transformation_matrix->addChild(autoText);
			transformation_matrix_2->addChild(autoText);
			//////////--

			

			//make the object visible or not
			SetVisible(group_name, object_id, visible);

			return true;
		}
		else 
		{
			return false;
		}
	}
}

bool OsgScene::Update(const std::string &object_id, cpw::Point3d<float> position, cpw::Point3d<float> orientation, cpw::Point3d<float> scale)
{
	//Search for the switch node
	OsgNodeFinderVisitor node_finder_visitor(object_id);
	entities.get()->accept(node_finder_visitor);

	//check if we found the node
	osg::LOD *found_node = (osg::LOD *) node_finder_visitor.GetNode();
	
	if (found_node == NULL)
		return false;

	if (found_node->className() != std::string("LOD"))
		return false;
	
	osg::Matrix trans_m, scale_m, orientation_mx, orientation_my, orientation_mz;
		
	orientation.x = (2 * osg::PI * orientation.x) / 360.0f;
	orientation_mx.makeRotate(orientation.x, osg::Vec3(1.0f, 0.0f, 0.0f));
		
	orientation.y = (2 * osg::PI * orientation.y) / 360.0f;
	orientation_my.makeRotate(orientation.y, osg::Vec3(0.0f, 1.0f, 0.0f));
	
	orientation.z = (2 * osg::PI * orientation.z) / 360.0f;
	orientation_mz.makeRotate(orientation.z, osg::Vec3(0.0f, 0.0f, 1.0f));

	osg::Quat rot_quat;
	rot_quat.makeRotate(orientation.x, osg::Vec3(1.0f, 0.0f, 0.0f),
		                orientation.y, osg::Vec3(0.0f, 1.0f, 0.0f),
						orientation.z, osg::Vec3(0.0f, 0.0f, 1.0f));
		
	cpw::Point3d<float> converted_pos = UTMToSceneCoords(position);
	found_node->setCenter(osg::Vec3(converted_pos.x, converted_pos.y, converted_pos.z));
	trans_m.setTrans(osg::Vec3(converted_pos.x, converted_pos.y, converted_pos.z));
	scale_m.identity();
	scale_m.makeScale(osg::Vec3(scale.x, scale.y, scale.z));
	
	osg::MatrixTransform *mtn = (osg::MatrixTransform *)found_node->getChild(0);
	osg::MatrixTransform *mtn_label = (osg::MatrixTransform *)found_node->getChild(0);
	//mtn->setMatrix(orientation_mx * orientation_my * orientation_mz * scale_m * trans_m);
	mtn->setMatrix( osg::Matrix(rot_quat) * scale_m * trans_m);
	
	if (mtn->getNumChildren() >=2)
	{
		if (mtn->getChild(1)->getNodeMask() == OSG_SCENE_LABEL_NODE_MASK )
		{
			osg::Matrix tm_matrix = mtn->getInverseMatrix();
			osg::Matrix tm_aux;
			tm_aux.makeIdentity();
			tm_matrix.setTrans(tm_aux.getTrans());
			
			((osg::MatrixTransform *) mtn->getChild(1))->setMatrix(tm_matrix);
		}
	}

	if (found_node->getNumChildren() > 1)
	{
		mtn = (osg::MatrixTransform *)found_node->getChild(1);

		float min = found_node->getMinRange(0);
		float max = found_node->getMaxRange(0);

		if (mtn->getNumChildren() == 1)
		{
			osg::Node *child0 = mtn->getChild(0);
			//max = this->ComputeLODDistance(child0->getBound(), mtn->getMatrix());

			//found_node->setRange(0, min, max);

			if (child0->className() == std::string("Geode"))
			{
				osg::Geode *geode = (osg::Geode *)child0;
				iosg::IconUserData *icon_user_data = (iosg::IconUserData *) geode->getUserData();

				if (icon_user_data != NULL)
				{
					icon_user_data->SetPosition(cpw::Point3d<float>(converted_pos.x, converted_pos.y, converted_pos.z /*+ 1000.0f*/));
				}
			}
		}

		//adapt the distance in which the change from model to icon happens
		mtn = (osg::MatrixTransform *) found_node->getChild(0);

		float temp = ComputeLODDistance(mtn->getChild(0)->getBound(), mtn->getMatrix());
		found_node->setRange(0, min, temp);
		found_node->setRange(1, temp, found_node->getMaxRange(1));



	}
	return true;
}


bool OsgScene::Remove(const std::string &object_id)
{
	OsgNodeFinderVisitor node_finder_visitor(object_id);
	entities.get()->accept(node_finder_visitor);

	//check if we found the node
	osg::Node *found_node =  node_finder_visitor.GetNode();
	
	if (found_node == NULL)
		return false;

	for (unsigned int i=0; i<entities->getNumChildren(); i++)
	{
		if (entities->getChild(i)->getName() == object_id)
		{
			entities->removeChildren(i, 1);
			return true;
		}
	}
	return false;
}


bool OsgScene::ObjectExistsInScene(const std::string &object_id)
{
	OsgNodeFinderVisitor node_finder_visitor(object_id);
	entities.get()->accept(node_finder_visitor);

	//check if we found the node
	if(node_finder_visitor.GetNode() == NULL)
		return false;
	else
		return true;
}


bool OsgScene::InsertAsReferencedObject(cpw::IGraphic *object)
{
	referenced_objects->addChild((osg::Node *) object);
	referenced_objects->setAllChildrenOff();
	return true;
}

bool OsgScene::IntersectRayWithScene(const cpw::Point3d<float> &r1, const cpw::Point3d<float> &r2, cpw::Point3d<float> &intersection_point, const bool &utm_ray)
{
	// // OPENGL TEST
	// const unsigned char * version = glGetString (GL_VERSION);
	// printf ("VERSION OPENGL: %s\n", version);
	// GLint u;
	// glGetIntegerv (GL_MAX_TEXTURE_UNITS, &u);
	// printf ("GL_MAX_TEXTURE_UNITS=%d\n", u);


	//test collisions against the scene

	cpw::Point3d<float> v1, v2;

	if (utm_ray)
	{
		v1 = UTMToSceneCoords(r1);
		v2 = UTMToSceneCoords(r2);
	}
	else
	{
		v1 = r1;
		v2 = r2;
	}

	osg::ref_ptr<osgUtil::LineSegmentIntersector> intersector;
	intersector = new osgUtil::LineSegmentIntersector( osg::Vec3(v1.x, v1.y, v1.z),
													   osg::Vec3(v2.x, v2.y, v2.z));

	
	osgUtil::IntersectionVisitor intersect_visitor( intersector.get() );
    root_node->accept(intersect_visitor);

	if (intersector->containsIntersections())
	{
		osg::Vec3 ip = intersector.get()->getFirstIntersection().getWorldIntersectPoint();
		intersection_point.x = ip.x();
		intersection_point.y = ip.y();
		intersection_point.z = ip.z();
		intersection_point = SceneCoordsToUTM(intersection_point);
		return true;
	}
	else
		return false;

}

// OJO: esta función la repito para double, porque sólo estaba hecha en float
// supongo que lo ideal sería hacerla con un template
bool OsgScene::IntersectRayWithTerrain(const cpw::Point3d<double> &r1, const cpw::Point3d<double> &r2, cpw::Point3d<double> &intersection_point, const bool &utm_ray)
{
	//test collisions against the terrain

	cpw::Point3d<double> v1, v2;

	if (utm_ray)
	{
		v1 = UTMToSceneCoords(r1);
		v2 = UTMToSceneCoords(r2);
	}
	else
	{
		v1 = r1;
		v2 = r2;
	}

	osg::ref_ptr<osgUtil::LineSegmentIntersector> intersector;
	intersector = new osgUtil::LineSegmentIntersector( osg::Vec3d(v1.x, v1.y, v1.z),
													   osg::Vec3d(v2.x, v2.y, v2.z));

	
	osgUtil::IntersectionVisitor intersect_visitor( intersector.get() );
    terrain_group->accept(intersect_visitor);

	if (intersector->containsIntersections())
	{
		osg::Vec3d ip = intersector.get()->getFirstIntersection().getWorldIntersectPoint();
		intersection_point.x = ip.x();
		intersection_point.y = ip.y();
		intersection_point.z = ip.z();
		intersection_point = SceneCoordsToUTM(intersection_point);
		return true;
	}
	else
		return false;

}


bool OsgScene::IntersectRayWithTerrain(const cpw::Point3d<float> &r1, const cpw::Point3d<float> &r2, cpw::Point3d<float> &intersection_point, const bool &utm_ray)
{
	//test collisions against the terrain

	cpw::Point3d<float> v1, v2;

	if (utm_ray)
	{
		v1 = UTMToSceneCoords(r1);
		v2 = UTMToSceneCoords(r2);
	}
	else
	{
		v1 = r1;
		v2 = r2;
	}

	osg::ref_ptr<osgUtil::LineSegmentIntersector> intersector;
	intersector = new osgUtil::LineSegmentIntersector( osg::Vec3(v1.x, v1.y, v1.z),
													   osg::Vec3(v2.x, v2.y, v2.z));

	
	osgUtil::IntersectionVisitor intersect_visitor( intersector.get() );
    terrain_group->accept(intersect_visitor);

	if (intersector->containsIntersections())
	{
		osg::Vec3 ip = intersector.get()->getFirstIntersection().getWorldIntersectPoint();
		intersection_point.x = ip.x();
		intersection_point.y = ip.y();
		intersection_point.z = ip.z();
		intersection_point = SceneCoordsToUTM(intersection_point);
		return true;
	}
	else
		return false;

}

bool OsgScene::IntersectRayWithScene(const cpw::Point3d<double> &r1, const cpw::Point3d<double> &r2, cpw::Point3d<double> &intersection_point, cpw::Point3d<double> &intersection_normal, const bool &utm_ray)
{
	cpw::Point3d<float> v1, v2, ip, in;
	v1.x = (float) r1.x;
	v1.y = (float) r1.y;
	v1.z = (float) r1.z;
	v2.x = (float) r2.x;
	v2.y = (float) r2.y;
	v2.z = (float) r2.z;

	if (IntersectRayWithScene(v1, v2, ip, in, utm_ray))
	{
		intersection_point.x = (double) ip.x;
		intersection_point.y = (double) ip.y;
		intersection_point.z = (double) ip.z;

		intersection_normal.x = (double) in.x;
		intersection_normal.y = (double) in.y;
		intersection_normal.z = (double) in.z;

		return true;
	}
	else
		return false;

}

bool OsgScene::IntersectRayWithNode(const float &_x, const float &_y, osg::Node *node_to_intersect, cpw::Point3d<float> &intersection_point, const bool &utm)
{
	cpw::Point3d<float> v1(_x, _y, 15000.0f);
	cpw::Point3d<float> v2(_x, _y, -15000.0f);

	if (utm)
	{
		v1 = UTMToSceneCoords(v1);
		v2 = UTMToSceneCoords(v2);
	}
	

	osg::ref_ptr<osgUtil::LineSegmentIntersector> intersector;
	intersector = new osgUtil::LineSegmentIntersector( osg::Vec3(v1.x, v1.y, v1.z),
													   osg::Vec3(v2.x, v2.y, v2.z));

	
	osgUtil::IntersectionVisitor intersect_visitor( intersector.get() );
    node_to_intersect->accept(intersect_visitor);

	if (intersector->containsIntersections())
	{
		osg::Vec3 ip = intersector.get()->getFirstIntersection().getWorldIntersectPoint();
	
		intersection_point.x = ip.x();
		intersection_point.y = ip.y();
		intersection_point.z = ip.z();
		
		intersection_point = SceneCoordsToUTM(intersection_point);
		
		return true;
	}
	else
		return false;

}
bool OsgScene::IntersectRayWithScene(const cpw::Point3d<float> &r1, const cpw::Point3d<float> &r2, cpw::Point3d<float> &intersection_point, cpw::Point3d<float> &intersection_normal, const bool &utm_ray)
{
	cpw::Point3d<float> v1, v2;

	if (utm_ray)
	{
		v1 = UTMToSceneCoords(r1);
		v2 = UTMToSceneCoords(r2);
	}
	else
	{
		v1 = r1;
		v2 = r2;
	}

	osg::ref_ptr<osgUtil::LineSegmentIntersector> intersector;
	intersector = new osgUtil::LineSegmentIntersector( osg::Vec3(v1.x, v1.y, v1.z),
													   osg::Vec3(v2.x, v2.y, v2.z));

	
	osgUtil::IntersectionVisitor intersect_visitor( intersector.get() );
    root_node->accept(intersect_visitor);

	if (intersector->containsIntersections())
	{
		osg::Vec3 ip = intersector.get()->getFirstIntersection().getWorldIntersectPoint();
		osg::Vec3 in = intersector.get()->getFirstIntersection().getLocalIntersectNormal();
		in.normalize();

		intersection_point.x = ip.x();
		intersection_point.y = ip.y();
		intersection_point.z = ip.z();
		
		intersection_point = SceneCoordsToUTM(intersection_point);

		intersection_normal.x = in.x();
		intersection_normal.y = in.y();
		intersection_normal.z = in.z();

		return true;
	}
	else
		return false;
}
bool OsgScene::IntersectRayWithScene(const cpw::Point3d<double> &r1, const cpw::Point3d<double> &r2, cpw::Point3d<double> &intersection_point, const bool &utm_ray)
{

	cpw::Point3d<float> v1, v2, ip;
	v1.x = (float) r1.x;
	v1.y = (float) r1.y;
	v1.z = (float) r1.z;
	v2.x = (float) r2.x;
	v2.y = (float) r2.y;
	v2.z = (float) r2.z;

	if (IntersectRayWithScene(v1, v2, ip, utm_ray))
	{
		intersection_point.x = (double) ip.x;
		intersection_point.y = (double) ip.y;
		intersection_point.z = (double) ip.z;
		return true;
	}
	else
		return false;
}

bool OsgScene::AddText(const std::string &font, const std::string &text, const float &size, const cpw::Point3d<float> &position)
{
	osgText::Font* _font = osgText::readFontFile(font.c_str());
	osgText::FadeText* _text = new osgText::FadeText;
	osg::Geode* geode  = new osg::Geode;
	cpw::Point3d<float> converted_position = UTMToSceneCoords(position);
    _text->setFont(_font);
    _text->setCharacterSize(size);
    _text->setPosition(osg::Vec3(converted_position.x, converted_position.y, converted_position.z));
	_text->setText(text.c_str());
	_text->setLayout(osgText::Text::LEFT_TO_RIGHT);
	_text->setAxisAlignment(osgText::Text::SCREEN);
	//_text->setBackdropType(osgText::Text::BackdropType::DROP_SHADOW_BOTTOM_RIGHT);
	//_text->setBackdropOffset(0.2,0.2);
	geode->addDrawable(_text);
	layers_node->addChild(geode);

	return true;
}


void OsgScene::UpdateTextures(int cx, int cy, const cpw::Point3d<float> &eye, const cpw::Point3d<float> &center, 
							  const cpw::Point3d<float> &up, const cpw::Point3d<float> &punto_mira, const unsigned char &texture_factor, const unsigned char &fire_mode)
{
	static bool first = false;

	//std::vector<osg::ref_ptr<osg::Node>>::iterator i = islands.begin();
	//std::vector<osg::Node *>::iterator i = islands.begin();
	//std::vector<osg::Node *>::iterator i = islands.end()-1; //solo para la palma

	
	if(terrain_group == NULL) 
		return;

	if (first_time)
	{
		first_time = false;
		visitor_text.Init(terrain_group.get(), texture_factor, show_wms_layers, cx, cy, 
					eye, center, up, punto_mira, &osg_petition_manager, fire_mode /*,visitor_flag*/ );
		visitor_text.Launch(terrain_group.get(), texture_factor, show_wms_layers, cx, cy, 
					eye, center, up, punto_mira, &osg_petition_manager, fire_mode /*,visitor_flag*/ );
	}
	else
	{
		//----------LLAMADA NUEVA-------
		visitor_text.Launch(terrain_group.get(), texture_factor, show_wms_layers, cx, cy, 
					eye, center, up, punto_mira, &osg_petition_manager, fire_mode /*,visitor_flag*/ );
	}

	first=true;
}



void OsgScene::ChangeVisitorFlag()
{ 

	visitor_flag = !visitor_flag;

	#ifdef _DEBUG
		//if (visitor_flag)
			//cpw::ApplicationLog::GetInstance()->GetLogger()->NewLogMessage("visitor is ON");
		//else
			//cpw::ApplicationLog::GetInstance()->GetLogger()->NewLogMessage("visitor is OFF");
	#endif
}

void OsgScene::ComputeUtmToSceneFactor()
{
	adapt_scale = adapt_center.x = adapt_center.y = adapt_center.z = 0.0f;
	
	if (terrain_group == NULL)
	{
		//error
		return;
	}
	else
	{
		osg::BoundingSphere bs = terrain_group->getBound();

		osg::Vec3 center = bs.center();

		adapt_center.x = (float) center.x(); 
		adapt_center.y = (float) center.y(); 
		adapt_center.z = 0.0f;

		if ((bs.radius() * 2) > ADAPT_FACTOR) 
			adapt_scale = ADAPT_FACTOR / (bs.radius() * 2);
		else
			adapt_scale = 1;
	}
}
 
cpw::Point3d<float> OsgScene::UTMToSceneCoords(const cpw::Point3d<float> &coords)
{
	cpw::Point3d<float> out;

	/*out.x = coords.x;
	out.y = coords.y - 3000000.0f;
	out.z = coords.z;
*/



	out.x = coords.x - adapt_center.x;
	out.y = coords.y - adapt_center.y;
	out.z = coords.z - adapt_center.z;

	/*out.x *= adapt_scale;
	out.y *= adapt_scale;
	out.z *= adapt_scale;*/

	return out;
}

cpw::Point3d<float> OsgScene::SceneCoordsToUTM(const cpw::Point3d<float> &coords)
{
	cpw::Point3d<float> out;

	/*out.x = coords.x;
	out.y = coords.y + 3000000.0f;
	out.z = coords.z;*/

	

	/*out.x = coords.x / adapt_scale;
	out.y = coords.y / adapt_scale;
	out.z = coords.z / adapt_scale;*/

	out = coords;

	out.x += adapt_center.x;
	out.y += adapt_center.y;
	out.z += adapt_center.z;

	return out;
}
cpw::Point3d<double> OsgScene::SceneCoordsToUTM(const cpw::Point3d<double> &coords)
{
	cpw::Point3d<double> out;

	/*out.x = coords.x;
	out.y = coords.y + 3000000.0;
	out.z = coords.z;

	

	out.x = coords.x / adapt_scale;
	out.y = coords.y / adapt_scale;
	out.z = coords.z / adapt_scale;*/

	out = coords;

	out.x += adapt_center.x;
	out.y += adapt_center.y;
	out.z += adapt_center.z;

	return out;
}

cpw::Point3d<double> OsgScene::UTMToSceneCoords(const cpw::Point3d<double> &coords)
{
	cpw::Point3d<double> out;

	/*out.x = coords.x;
	out.y = coords.y - 3000000.0;
	out.z = coords.z;*/

	

	out.x = coords.x - adapt_center.x;
	out.y = coords.y - adapt_center.y;
	out.z = coords.z - adapt_center.z;

	/*out.x *= adapt_scale;
	out.y *= adapt_scale;
	out.z *= adapt_scale;*/

	return out;
}

cpw::Point3d<double> OsgScene::GetAdaptOffset()
{
	cpw::Point3d<double> out;

	out.x = adapt_center.x;
	out.y = adapt_center.y;
	out.z = adapt_center.z;

	return out;
}
float OsgScene::ComputeLODDistance(const osg::BoundingSphere &bsphere, const osg::Matrix &matrix)
{
	osg::Vec3d scale = matrix.getScale();
	float radius = bsphere.radius();
	float out;

	double max = scale.x();
	
	if (scale.y() > max)
		max = scale.y();

	if (scale.z() > max)
		max = scale.z();

	//max *= 3500.0f;

	out = radius * max * LOD_VALUE;
	
	//if (out < MIN_LOD_DISTANCE)
	//	out = MIN_LOD_DISTANCE;

	if (out < MINIMUN_LOD_SIZE)
		out = MINIMUN_LOD_SIZE;

	// el valor de retorno es la distancia umbral entre pintar el icono o el modelo 3d
	return out;
}


void OsgScene::AddLine(const std::string &object_id, const std::vector<cpw::Point3d<float>> &draw_array, const std::string &color,const int &r, const int &g, const int &b, const int &width, const bool &visible)
{
	//const unsigned char * version = glGetString (GL_VERSION);
	//printf ("VERSION OPENGL: %s\n", version);
	//GLint u;
	//glGetIntegerv (GL_MAX_TEXTURE_UNITS, &u);
	//printf ("GL_MAX_TEXTURE_UNITS=%d\n", u);

	if(draw_array.size()<=1) return;
	

	if(ObjectExistsInScene(object_id))  
	{
		Remove(object_id);
	}
	std::vector<cpw::Point3d<float>>::const_iterator iter;
	std::vector<cpw::Point3d<float>>::const_iterator f_iter;
	std::vector<cpw::Point3d<float>> v_aux, final_vec;

	cpw::Point3d<float> aux;
	for(iter = draw_array.begin(); iter != draw_array.end(); iter++)
	{
		aux = UTMToSceneCoords(*iter);
		v_aux.push_back(cpw::Point3d<float>((float)aux.x, (float)aux.y, (float)aux.z) );
	}
	
	//float step = osg_curve_adapter.GetStepDistance();
	//osg_curve_adapter.SetStepDistance(25);
	//osg_curve_adapter.Adapt(v_aux, final_vec);
	//osg_curve_adapter.SetStepDistance(step);

	final_vec = v_aux;

	osg::ref_ptr<osg::Geometry> geom = new osg::Geometry;
	geom->addPrimitiveSet(new osg::DrawArrays(GL_LINE_STRIP ,0,final_vec.size()));
	//geom->addPrimitiveSet(new osg::DrawArrays(GL_TRIANGLES ,0,final_vec.size()));

	osg::ref_ptr<osg::Vec3Array> v = new osg::Vec3Array;
	geom->setVertexArray(v.get());
	v->resize(final_vec.size());

	int i=0;
	for(f_iter = final_vec.begin(); f_iter != final_vec.end(); f_iter++)
	{
		(*v)[i] = osg::Vec3d(f_iter->x,f_iter->y,f_iter->z);
		i++;
	}


	//////////////////////// COLOR/////////////////////////
	float rf, gf, bf;
	int red(r), green(g), blue(b);
	if(red<0) red=0;
	if(green<0) green=0;
	if(blue<0) blue=0;

	if(red>255) red=255;
	if(green>255) green=255;
	if(blue>255) blue=255;
	
	rf  = red / 255;
	gf  = green / 255;
	bf  = blue / 255;

    //osg::Vec4Array* colors = new osg::Vec4Array;
	osg::ref_ptr<osg::Vec4Array> colors = new osg::Vec4Array;
	colors->push_back(osg::Vec4(rf,gf,bf,1.0f));
    geom->setColorArray(colors.get());

    //geom->setColorBinding(osg::Geometry::BIND_OVERALL); 
	geom->setColorBinding(osg::Geometry::BIND_PER_PRIMITIVE_SET);
    /////////////////////////////////////////////////////////////



	//////////////////////// GROSOR /////////////////////////

	////////////////////////////////////////////////////////



	osg::ref_ptr<osg::Geode> geode = new osg::Geode();
	geode->addDrawable(geom.get());
	geode->setName("- line -");

	//create a new LOD node which will choose between the icon or the object
	osg::ref_ptr<osg::LOD> new_LOD_node = new osg::LOD();
	entities->addChild(new_LOD_node.get());
/**************************************************************************************************************/

	//Create the groups
	std::vector<osg::ref_ptr<osg::Group>> groups;
	osg::ref_ptr<osg::Group> last_group;
	for (unsigned int i=0; i<MAX_LINE_LODS; i++)
	{

		float init_casd = 10.0f;
		if(i>0) 
			init_casd = 25.0f;
		
		

		osg::ref_ptr<osg::Group> new_subgroup = new osg::Group;
		osg::ref_ptr<OsgLineCallback> callback = new OsgLineCallback;

		callback.get()->SetID(i);

		new_subgroup->addChild(geode.get());
		new_subgroup->setCullCallback(callback.get());
		new_subgroup->setName(object_id);

		callback->SetGeometry(geom.get());
		callback->SetCurveAdapter(&curve_adapter);
		callback->SetCurveAdapterStepDistance(init_casd * (i+1)); //set the curve adapter step distance

		//link the callbacks
		if (i>0)
		{
			OsgLineCallback *temp = (OsgLineCallback *)(groups[i-1].get()->getCullCallback());
			temp->SetRightBrother(callback.get());
			callback->SetLeftBrother(temp);
		}

		//set the new particles rate for all the groups
		if (i+1 == MAX_LINE_LODS)
		{
			//callback->SetNewParticlesPerSecond(cpw::Range<int>(0, 0));  //the last one will switch off the ps
			last_group = new_subgroup.get();
			callback->SetRightBrother(NULL);
		}
		else
		{
			//callback->SetNewParticlesPerSecond(cpw::Range<int>((int)(new_particles_sec.min * (1.0f / (i+1))) ,
			//								  (int)(new_particles_sec.max * (1.0f / (i+1)))));		

			////set the particles size for all the groups
			//callback->SetParticlesSize(cpw::Range<float>(size.min * (i+1), size.max * (i+1)));
			groups.push_back( new_subgroup.get() );
		}
		         
	}

	//create the LOD node to choose between the icon and the line
	//osg::ref_ptr<osg::LOD> new_lod = new osg::LOD();

	//add the new groups
	for (unsigned int i=0; i<groups.size(); i++)
	{
		if (i==0) //first
			new_LOD_node->addChild(groups[i].get(), 0.0, LOD_0_VALUE / (groups.size()));
		else if (i == (groups.size() - 1)) //last
			new_LOD_node->addChild(groups[i].get(), (LOD_0_VALUE / (groups.size() - i + 1)), LOD_0_VALUE);
		else
			new_LOD_node->addChild(groups[i].get(), (LOD_0_VALUE / (groups.size() - i + 1)), LOD_0_VALUE / (groups.size() - i ) );
	}

	//last callback
	new_LOD_node->addChild(last_group.get(), LOD_0_VALUE, LOD_0_VALUE-1000);
/**************************************************************************************************************/




	//new_LOD_node->addChild(geode.get(), 0.0f, LOD_0_VALUE);

	std::vector<cpw::Point3d<float>>::iterator iter_center;
	iter_center = v_aux.begin();
	float xmin(iter_center->x), xmax(iter_center->x), ymin(iter_center->y), ymax(iter_center->y),zmax(0.0f);
	for(; iter_center != v_aux.end(); iter_center++)
	{
		if(iter_center->x < xmin) xmin = iter_center->x;
		if(iter_center->x > xmax) xmax = iter_center->x;
		if(iter_center->y < ymin) ymin = iter_center->y;
		if(iter_center->y > ymax) ymax = iter_center->y;
		if(iter_center->z > zmax) zmax = iter_center->z;
	}
	aux.x = (xmin+xmax) / 2;
	aux.y = (ymin+ymax) / 2;
	aux.z = zmax;
	new_LOD_node->setCenter(osg::Vec3(aux.x,aux.y,aux.z));
	new_LOD_node->setName(object_id);

	////make the object visible or not
	SetVisible("", object_id, visible);

	return;
}

void OsgScene::UpdateLine(const std::string &object_id, const std::vector<cpw::Point3d<float>> &draw_array, const std::string &color,const int &r, const int &g, const int &b, const int &width, const bool &visible, const int &update_point)
{
	if(draw_array.size()<=1) return;
		
	if(ObjectExistsInScene(object_id))  
	{
		std::vector<cpw::Point3d<float>> v_aux, final_vec;
		int type = -1;
		if(update_point==-1)
		{
			cpw::Point3d<float> aux1, aux2;
			aux1 = UTMToSceneCoords(draw_array[draw_array.size()-2]);
			aux2 = UTMToSceneCoords(draw_array[draw_array.size()-1]);
			v_aux.push_back(cpw::Point3d<float>(aux1.x, aux1.y, aux1.z) );
			v_aux.push_back(cpw::Point3d<float>(aux2.x, aux2.y, aux2.z) );
		}
		else
		{
			cpw::Point3d<float> aux1, aux2, aux3;
			if(update_point>=1 && update_point<(int)draw_array.size()-1)
			{
				aux1 = UTMToSceneCoords(draw_array[update_point]);
				aux2 = UTMToSceneCoords(draw_array[update_point-1]);
				aux3 = UTMToSceneCoords(draw_array[update_point+1]);
				v_aux.push_back(cpw::Point3d<float>(aux2.x, aux2.y, aux2.z) );
				v_aux.push_back(cpw::Point3d<float>(aux1.x, aux1.y, aux1.z) );
				v_aux.push_back(cpw::Point3d<float>(aux3.x, aux3.y, aux3.z) );
				type = 0;
			}
			else if(update_point == 0)
			{
				aux1 = UTMToSceneCoords(draw_array[update_point]);
				aux3 = UTMToSceneCoords(draw_array[update_point+1]);
				v_aux.push_back(cpw::Point3d<float>(aux1.x, aux1.y, aux1.z) );
				v_aux.push_back(cpw::Point3d<float>(aux3.x, aux3.y, aux3.z) );	
				type = 1;
			}
			else if(update_point == draw_array.size()-1)
			{
				aux1 = UTMToSceneCoords(draw_array[update_point]);
				aux2 = UTMToSceneCoords(draw_array[update_point-1]);
				v_aux.push_back(cpw::Point3d<float>(aux2.x, aux2.y, aux2.z) );
				v_aux.push_back(cpw::Point3d<float>(aux1.x, aux1.y, aux1.z) );
				type = 2;
			}
		}



		//Search for the particle system node
		OsgNodeFinderVisitor node_finder_visitor(object_id);
		entities.get()->accept(node_finder_visitor);

		//check if we found the node
		osg::Node *found_node = node_finder_visitor.GetNode();
		
		if (found_node == NULL)
			return;


		std::vector<cpw::Point3d<float>> aux1;
		for(unsigned int i=0; i<draw_array.size(); i++)
			aux1.push_back(UTMToSceneCoords(draw_array[i]));

		float step=100;
		for (unsigned int i=0; i<MAX_LINE_LODS; i++)
		{
			((OsgLineCallback *) (((osg::LOD*)found_node)->getChild(i)->getCullCallback()))->SetNewInitialVertex(aux1);
			if (((OsgLineCallback *) (((osg::LOD*)found_node)->getChild(i)->getCullCallback()))->IsActive())
				step = ((OsgLineCallback *) (((osg::LOD*)found_node)->getChild(i)->getCullCallback()))->GetCurveAdapterStepDistance();
		}

		float step_aux = curve_adapter.GetStepDistance();
		curve_adapter.SetStepDistance(step);
		curve_adapter.Adapt(v_aux, final_vec);
		curve_adapter.SetStepDistance(step_aux);
		

		osg::Geode* geode = (osg::Geode*)((osg::Group*)(((osg::LOD*)found_node)->getChild(0)))->getChild(0);
		osg::Geometry* geom = (osg::Geometry*)geode->getDrawable(0);

		
		osg::Vec3Array* vertex = (osg::Vec3Array*)geom->getVertexArray();
		osg::ref_ptr<osg::Vec3Array> new_vertex = new osg::Vec3Array();

		int final_vertex_size=0;
		
		int tam_vertex = vertex->getNumElements();
		int tam_final_vec = final_vec.size();
		if(update_point==-1)
		{
			vertex->resize( tam_vertex + tam_final_vec);
			geom->removePrimitiveSet(0,1);
			geom->addPrimitiveSet(new osg::DrawArrays(GL_LINE_STRIP ,0,tam_vertex + tam_final_vec));
			//geom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::TRIANGLES ,0,tam_vertex + tam_final_vec));
			int i=tam_vertex;
			std::vector<cpw::Point3d<float>>::const_iterator f_iter;
			for(f_iter = final_vec.begin(); f_iter != final_vec.end(); f_iter++)
			{
				(*vertex)[i] = osg::Vec3d(f_iter->x,f_iter->y,f_iter->z+1);
				i++;
			}
			final_vertex_size = vertex->getNumElements();
		}
		else
		{
			cpw::Point3d<float> aux1, aux2, aux3;
			aux1 = UTMToSceneCoords(draw_array[update_point]);
			int i, ind_inf=-1, ind_sup=-1;

			if (type==0 || type==1)
				aux3 = UTMToSceneCoords(draw_array[update_point+1]);

			if (type==0 || type == 2)
				aux2 = UTMToSceneCoords(draw_array[update_point-1]);

			for(i=0;i<tam_vertex;i++)
			{
				if((type==0 || type==2)&&(((*vertex)[i].x()==aux2.x) && ((*vertex)[i].y()==aux2.y)))
					ind_inf=i;

				if((type==0 || type==1)&&(((*vertex)[i].x()==aux3.x) && ((*vertex)[i].y()==aux3.y)))
					ind_sup=i-1;
			}
			
			if((type==0)&&((ind_inf==-1)||(ind_sup==-1))) return;
			if(type==1) ind_inf = 0;
			if(type==2) ind_sup = tam_vertex-1;

			int num_del_vertex = (ind_sup - ind_inf)+1;
			int new_size = (tam_vertex - num_del_vertex) + tam_final_vec;
			new_vertex->resize(new_size);
			int j=1;
			for(i=0;i<new_size;i++)
			{
				if(i<ind_inf)
					(*new_vertex)[i]=( (*vertex)[i] );

				//if((i>=ind_inf && i<=ind_sup)&&(f_iter!=final_vec.end()))
				//{
				//	new_vertex->push_back( osg::Vec3d(f_iter->x,f_iter->y,f_iter->z+1));
				//	f_iter++;
				//}

				//if((i>=ind_inf) && (i<=ind_inf+tam_final_vec)) break;

				if((i>=ind_inf+tam_final_vec)&&(ind_sup+j<tam_vertex))
				{
					(*new_vertex)[i]=( (*vertex)[ind_sup+j] );
					j++;
				}
			}

			if(final_vec.empty()) return;
			std::vector<cpw::Point3d<float>>::const_iterator f_iter=final_vec.begin();
			i=ind_inf;
			for(;f_iter!=final_vec.end();f_iter++)
			{
				(*new_vertex)[i] = osg::Vec3d(f_iter->x,f_iter->y,f_iter->z+1);
				i++;
			}

			geom->removePrimitiveSet(0,1);
			geom->addPrimitiveSet(new osg::DrawArrays(GL_LINE_STRIP ,0,new_size));
			//geom->addPrimitiveSet(new osg::DrawArrays(GL_TRIANGLES ,0,new_size));
			geom->setVertexArray(new_vertex.get());
			final_vertex_size = new_vertex->getNumElements();

		}

		//////////////////////// COLOR/////////////////////////
		float rf, gf, bf;
		int red(r), green(g), blue(b);
		if(red<0) red=0;
		if(green<0) green=0;
		if(blue<0) blue=0;

		if(red>255) red=255;
		if(green>255) green=255;
		if(blue>255) blue=255;
		
		rf  = red / 255;
		gf  = green / 255;
		bf  = blue / 255;

		//osg::Vec4Array* colors = new osg::Vec4Array;
		osg::ref_ptr<osg::Vec4Array> colors = new osg::Vec4Array;
		colors->push_back(osg::Vec4(rf,gf,bf,1.0f));
		geom->setColorArray(colors.get());

		geom->setColorBinding(osg::Geometry::BIND_PER_PRIMITIVE_SET);
		/////////////////////////////////////////////////////////////



		//////////////////////// GROSOR /////////////////////////

		////////////////////////////////////////////////////////



		//osg::ref_ptr<osg::Geode> geode = new osg::Geode();
		//geode->addDrawable(geom.get());
		//geode->setName("- line -");

		////create a new LOD node which will choose between the icon or the object
		//osg::ref_ptr<osg::LOD> new_LOD_node = (osg::LOD*)(found_node);

		//new_LOD_node->setChild(0,geode);

		//std::vector<cpw::Point3d<float>>::iterator iter_center;
		//int tam2 = vertex->getNumElements();
		////iter_center = v_aux.begin();

		float xmin, xmax, ymin, ymax, zmax;

		if(update_point==-1)
		{
			xmin=(*vertex)[0].x();
			xmax=(*vertex)[0].x();
			ymin=(*vertex)[0].y();
			ymax=(*vertex)[0].y();
			zmax=(*vertex)[0].z();
			for(int j=0; j< final_vertex_size; j++)
			{
				if((*vertex)[j].x() < xmin) xmin = (*vertex)[j].x();
				if((*vertex)[j].x() > xmax) xmax = (*vertex)[j].x();
				if((*vertex)[j].y() < ymin) ymin = (*vertex)[j].y();
				if((*vertex)[j].y() > ymax) ymax = (*vertex)[j].y();
				if((*vertex)[j].z() > zmax) zmax = (*vertex)[j].z();
			}
		}
		else
		{
			xmin=(*new_vertex)[0].x();
			xmax=(*new_vertex)[0].x();
			ymin=(*new_vertex)[0].y();
			ymax=(*new_vertex)[0].y();
			zmax=(*new_vertex)[0].z();
			for(int j=0; j< final_vertex_size; j++)
			{
				if((*new_vertex)[j].x() < xmin) xmin = (*new_vertex)[j].x();
				if((*new_vertex)[j].x() > xmax) xmax = (*new_vertex)[j].x();
				if((*new_vertex)[j].y() < ymin) ymin = (*new_vertex)[j].y();
				if((*new_vertex)[j].y() > ymax) ymax = (*new_vertex)[j].y();
				if((*new_vertex)[j].z() > zmax) zmax = (*new_vertex)[j].z();
			}
		}

		cpw::Point3d<float> aux;

		aux.x = (xmin+xmax) / 2;
		aux.y = (ymin+ymax) / 2;
		aux.z = zmax;
		((osg::LOD*)found_node)->setCenter(osg::Vec3(aux.x,aux.y,aux.z));
		//new_LOD_node->setCenter(osg::Vec3(aux.x,aux.y,aux.z));
		//new_LOD_node->setName(object_id);

		//////make the object visible or not
		SetVisible("", object_id, visible);
	}
	return;
}

void OsgScene::AddLine2(const std::string &object_id, const std::vector<cpw::Point3d<float>> &draw_array, const std::string &color,const int &r, const int &g, const int &b, const int &width, const bool &visible)
{
	if(ObjectExistsInScene(object_id))  
	{
		Remove(object_id);
	}

	if(draw_array.size()<=1) return;
	

	std::vector<cpw::Point3d<float>>::const_iterator iter;
	std::vector<cpw::Point3d<float>>::const_iterator f_iter;
	std::vector<cpw::Point3d<float>> v_aux, final_vec;

	cpw::Point3d<float> aux;
	for(iter = draw_array.begin(); iter != draw_array.end(); iter++)
	{
		aux = UTMToSceneCoords(*iter);
		v_aux.push_back(cpw::Point3d<float>((float)aux.x, (float)aux.y, (float)aux.z) );
	}
	
	final_vec = v_aux;

	osg::ref_ptr<osg::Geometry> geom = new osg::Geometry;
	geom->addPrimitiveSet(new osg::DrawArrays(GL_TRIANGLES ,0,final_vec.size()));

	osg::ref_ptr<osg::Vec3Array> v = new osg::Vec3Array;
	geom->setVertexArray(v.get());
	v->resize(final_vec.size());

	int i=0;
	for(f_iter = final_vec.begin(); f_iter != final_vec.end(); f_iter++)
	{
		(*v)[i] = osg::Vec3d(f_iter->x,f_iter->y,f_iter->z);
		i++;
	}


	//////////////////////// COLORRRRRRR/////////////////////////
	float rf, gf, bf;
	int red(r), green(g), blue(b);
	if(red<0) red=0;
	if(green<0) green=0;
	if(blue<0) blue=0;

	if(red>255) red=255;
	if(green>255) green=255;
	if(blue>255) blue=255;
	
	rf  = red / 255;
	gf  = green / 255;
	bf  = blue / 255;

	osg::ref_ptr<osg::Vec4Array> colors = new osg::Vec4Array;
	colors->push_back(osg::Vec4(rf,gf,bf,1.0f));
    geom->setColorArray(colors.get());

	geom->setColorBinding(osg::Geometry::BIND_PER_PRIMITIVE_SET);
    /////////////////////////////////////////////////////////////

	osg::ref_ptr<osg::Geode> geode = new osg::Geode();
	geode->addDrawable(geom.get());
	geode->setName("- line -");

	geode->getOrCreateStateSet()->setAttributeAndModes( new osg::CullFace, osg::StateAttribute::ON);
	geode->getOrCreateStateSet()->setMode(GL_LIGHTING, true);


	//create a new LOD node which will choose between the icon or the object
	osg::ref_ptr<osg::LOD> new_LOD_node = new osg::LOD();
	entities->addChild(new_LOD_node.get());
/**************************************************************************************************************/

	//Create the groups
	std::vector<osg::ref_ptr<osg::Group>> groups;
	osg::ref_ptr<osg::Group> last_group;
	for (unsigned int i=0; i<MAX_LINE_LODS; i++)
	{
		float init_casd = 25.0f;

		osg::ref_ptr<osg::Group> new_subgroup = new osg::Group;
		osg::ref_ptr<OsgLine2Callback> callback = new OsgLine2Callback(default_directory);

		callback.get()->SetID(i);

		new_subgroup->addChild(geode.get());
		new_subgroup->setCullCallback(callback.get());
		new_subgroup->setName(object_id);

		callback->SetGeometry(geom.get());
		callback->SetCurveAdapter(&curve_adapter);
		callback->SetCurveAdapterStepDistance(init_casd * (i+1)); //set the curve adapter step distance

		//link the callbacks
		if (i>0)
		{
			OsgLine2Callback *temp = (OsgLine2Callback *)(groups[i-1].get()->getCullCallback());
			temp->SetRightBrother(callback.get());
			callback->SetLeftBrother(temp);
		}

		//set the new particles rate for all the groups
		if (i+1 == MAX_LINE_LODS)
		{
			last_group = new_subgroup.get();
			callback->SetRightBrother(NULL);
		}
		else
		{
			groups.push_back( new_subgroup.get() );
		}
		         
	}

	//add the new groups
	for (unsigned int i=0; i<groups.size(); i++)
	{
		if (i==0) //first
			new_LOD_node->addChild(groups[i].get(), 0.0, LOD_0_VALUE / (groups.size()));
		else if (i == (groups.size() - 1)) //last
			new_LOD_node->addChild(groups[i].get(), (LOD_0_VALUE / (groups.size() - i + 1)), LOD_0_VALUE);
		else
			new_LOD_node->addChild(groups[i].get(), (LOD_0_VALUE / (groups.size() - i + 1)), LOD_0_VALUE / (groups.size() - i ) );
	}

	//last callback
	//new_LOD_node->addChild(last_group.get(), LOD_0_VALUE, LOD_0_VALUE+1000);
	new_LOD_node->addChild(last_group.get(), LOD_0_VALUE, LOD_0_VALUE+1000);
/**************************************************************************************************************/

	std::vector<cpw::Point3d<float>>::iterator iter_center;
	iter_center = v_aux.begin();
	float xmin(iter_center->x), xmax(iter_center->x), ymin(iter_center->y), ymax(iter_center->y),zmax(0.0f);
	for(; iter_center != v_aux.end(); iter_center++)
	{
		if(iter_center->x < xmin) xmin = iter_center->x;
		if(iter_center->x > xmax) xmax = iter_center->x;
		if(iter_center->y < ymin) ymin = iter_center->y;
		if(iter_center->y > ymax) ymax = iter_center->y;
		if(iter_center->z > zmax) zmax = iter_center->z;
	}
	aux.x = (xmin+xmax) / 2;
	aux.y = (ymin+ymax) / 2;
	aux.z = zmax;
	new_LOD_node->setCenter(osg::Vec3(aux.x,aux.y,aux.z));
	new_LOD_node->setName(object_id);

	////make the object visible or not
	SetVisible("", object_id, visible);

	return;
}


void OsgScene::AdaptLineToTerrain(const cpw::Point3d<float> &v0, const cpw::Point3d<float> &v1, std::vector< cpw::Point3d<float> > &new_vertices, const float &distance_between_vertices,  const float &height_tolerance)
{
	cpw::Point3d<float> _v0( UTMToSceneCoords(v0) );
	cpw::Point3d<float> _v1( UTMToSceneCoords(v1) );
	cpw::Point3d<float> difference( _v1 - _v0 );
	std::vector< cpw::Point3d<float> > original_interpolated_vertices;
	float d2;
	float d = cpw::Math::DistanceBetweenTwoPoints(_v0, _v1);

	new_vertices.clear();	

	new_vertices.push_back(v0);

	//compute the new vertices vector
	if (d < distance_between_vertices)
	{
		new_vertices.push_back(v1);
		original_interpolated_vertices.push_back(v1);
	}
	else
	{
		cpw::Point3d<float> tempv0, tempv1, original, final_vertex;
		float step_position;

		for (int aux = 1; (aux * distance_between_vertices) < d; aux++)
		{
			step_position = (aux * distance_between_vertices) / d;

			tempv1.x = tempv0.x = _v0.x + (difference.x * step_position);
			tempv1.y = tempv0.y = _v0.y + (difference.y * step_position);
			tempv1.z = tempv0.z = _v0.z + (difference.z * step_position);

			original = tempv0;

			//original_interpolated_vertices.push_back(tempv1);

			tempv1.z = -100000.0f;
			tempv0.z = 100000.0f;

			if (IntersectRayWithScene(tempv0, tempv1, final_vertex, false))
			{
				original = SceneCoordsToUTM(original);
				d2 = cpw::Math::DistanceBetweenTwoPoints(original, final_vertex); 
				
				if (d2  > height_tolerance)
					new_vertices.push_back( final_vertex );
			}
		}		

		original_interpolated_vertices.push_back(v1);
		new_vertices.push_back(v1);
	}

	//optimize the new vertices vector by removing some vertices which are not important
	//to the final shape
}


void OsgScene::AddTextureLayer(osg::Texture2D *_texture, const cpw::Point2d<float> &utm0, const cpw::Point2d<float> &utm1)
{
	visitor_text.AddTextureRect(_texture, utm0, utm1);

}

void OsgScene::RemoveTextureLayer(osg::Texture2D *_texture)
{
	visitor_text.DelTextureRect(_texture);
}

void OsgScene::AddObject(const std::string &name, const std::string &model, const cpw::Point3d<float> &pos)
{
	if(ObjectExistsInScene(name))  
	{
		Remove(name);
	}

	std::string url1 = NormalizeString(model);
	osg::Node *node = osgDB::readNodeFile(url1);
		
	if (node != NULL)
	{
		cpw::Point3d<float> rotation;
		rotation.x=0.0f;
		rotation.y=0.0f;
		rotation.z=0.0f;
		
		cpw::Point3d<float> scale;
		scale.x = 1.0f;
		scale.y = 1.0f;
		scale.z = 1.0f;

		cpw::Point3d<float> converted_pos = UTMToSceneCoords(pos);
		osg::ref_ptr<osg::MatrixTransform> transformation_matrix = OsgScene::CreateTransformationMatrix(converted_pos, rotation, scale);
		
		transformation_matrix->addChild(node);
		transformation_matrix->setName(name);
		entities->addChild(transformation_matrix.get());
	}
		
}


bool OsgScene::AddGizmo(const std::string &gizmo_id, const std::vector<std::string> &sub_ids, const std::vector<std::string> &filenames)
{
	if (sub_ids.size() != filenames.size())
		return false;

	osg::ref_ptr<osg::MatrixTransform> mt = new osg::MatrixTransform;
	
	mt->setName(gizmo_id);

	unsigned int i=0;
	
	//load all the sub-gizmos
	for (std::vector<std::string>::const_iterator iter = filenames.begin();
		iter != filenames.end();
		iter++)
	{
		osg::ref_ptr<osg::Node> new_node = osgDB::readNodeFile(*iter);
		
		if (new_node == NULL)
			return false;

		new_node->setName( sub_ids[i] );
		new_node->setNodeMask(OSG_SCENE_GIZMOS_NODE_MASK);
		mt->addChild( new_node.get() );
		i++;
	}

	handlers_node->addChild(mt.get());

	return true;
}

bool OsgScene::SetHandlerVisible(const std::string &handler_id, const bool &visible)
{
	if (handlers_node == NULL)
		return false;

	for (unsigned int i=0; i < handlers_node->getNumChildren(); i++)
	{
		if (handlers_node->getChild(i)->getName() == handler_id)
		{
			handlers_node->setValue(i, visible);
			return true;
		}
	}
	
	return false;
}

bool OsgScene::IsHandlerVisible(const std::string &handler_id)
{
	if (handlers_node == NULL)
		return false;

	for (unsigned int i=0; handlers_node->getNumChildren(); i++)
	{
		if (handlers_node->getChild(i)->getName() == handler_id)
		{
			return handlers_node->getValue(i);
		}
	}
	
	return false;
}


bool OsgScene::AdaptHandlerToModel(const std::string &handler_id, const std::string &model_id,  const cpw::Point3d<float> &orientation, const bool &adapt_orientation)
{
	//search for the gizmo
	OsgNodeFinderVisitor node_finder_visitor(handler_id);
	handlers_node->accept(node_finder_visitor);
	osg::Node *handler = node_finder_visitor.GetNode();

	if ((handler == NULL) || (handler->className() != std::string("MatrixTransform")))
		return false;
	
	//search for the object
	OsgNodeFinderVisitor node_finder_visitor2(model_id);
	entities->accept(node_finder_visitor2);
	osg::Node *model = node_finder_visitor2.GetNode();

	if ((model == NULL) || (model->getNodeMask() != OSG_SCENE_ENTITIES_NODE_MASK))
		return false;

	osg::LOD *lod = (osg::LOD *) model;
	
	osg::BoundingSphere bb = ((osg::Group *)lod->getChild(0))->getChild(0)->getBound();
		
	float radius = bb.radius();

	osg::MatrixTransform *mt = (osg::MatrixTransform *) handler;
	osg::Matrix matrix_t, matrix_r, matrix_s, matrix_final;
	
	osg::Vec3 traslation;
	osg::Quat quat;
	osg::Vec3 scale;
	osg::Quat scale_orientation;
	
	((osg::MatrixTransform *) lod->getChild(0))->getMatrix().decompose(traslation, quat, scale, scale_orientation);
	
	matrix_t.setTrans(traslation);
	matrix_s.makeScale(radius * scale.x(), radius * scale.y(), radius * scale.z());

	if (adapt_orientation)
	{
		matrix_r.identity();

		osg::Matrix orientation_mx, orientation_my, orientation_mz;	
			
		cpw::Point3d<float> orientation2;
		
		orientation_mx.makeRotate(orientation.x, osg::Vec3(1.0f, 0.0f, 0.0f));
		orientation_my.makeRotate(orientation.y, osg::Vec3(0.0f, 1.0f, 0.0f));
		orientation_mz.makeRotate(orientation.z, osg::Vec3(0.0f, 0.0f, 1.0f));

		matrix_r = orientation_mx * orientation_my * orientation_mz;

		osg::Quat rot_quat;
		rot_quat.makeRotate(orientation.x, osg::Vec3(1.0f, 0.0f, 0.0f),
							orientation.y, osg::Vec3(0.0f, 1.0f, 0.0f),
							orientation.z, osg::Vec3(0.0f, 0.0f, 1.0f));

		matrix_final = matrix_r * matrix_s * matrix_t;
	}
	else
		matrix_final = matrix_s * matrix_t;

	mt->setMatrix(matrix_final);
	
	return false;
}


bool OsgScene::AdaptHandlerToModelFixedScreenSize(const std::string &handler_id, const std::string &model_id,  const cpw::Point3d<float> &orientation, const bool &adapt_orientation, const int &pixel_size, const cpw::Point3d<double> &camera_position, const float &znear)
{
	
	//search for the gizmo
	OsgNodeFinderVisitor node_finder_visitor(handler_id);
	handlers_node->accept(node_finder_visitor);
	osg::Node *handler = node_finder_visitor.GetNode();

	if ((handler == NULL) || (handler->className() != std::string("MatrixTransform")))
		return false;

	osg::MatrixTransform *mt = (osg::MatrixTransform *) handler;
	
	//search for the object
	OsgNodeFinderVisitor node_finder_visitor2(model_id);
	entities->accept(node_finder_visitor2);
	osg::Node *model = node_finder_visitor2.GetNode();

	if ((model == NULL) || (model->getNodeMask() != OSG_SCENE_ENTITIES_NODE_MASK))
		return false;

	osg::LOD *lod = (osg::LOD *) model;
	
	osg::BoundingSphere bb = ((osg::Group *)lod->getChild(0))->getChild(0)->getBound();
		
	float radius = bb.radius();

	
	osg::Matrix matrix_t, matrix_r, matrix_s, matrix_final;
	
	osg::Vec3 traslation;
	osg::Quat quat;
	osg::Vec3 scale;
	osg::Quat scale_orientation;
	
	((osg::MatrixTransform *) lod->getChild(0))->getMatrix().decompose(traslation, quat, scale, scale_orientation);
	
	matrix_t.setTrans(traslation);
	
	//find the gizmo new size
	cpw::Point3d<double> s_camera_pos = this->UTMToSceneCoords(camera_position);
	cpw::Point3d<double> cpw_traslation(traslation.x(), traslation.y(), traslation.z());

	float distance_to_camera = (float) cpw::Math::DistanceBetweenTwoPoints(s_camera_pos, cpw_traslation);
	float new_size = (pixel_size * distance_to_camera) / sqrt( (4 * znear * znear) + ((float)(pixel_size * pixel_size)));
	new_size *= 2.0f;

	//cpw::ApplicationLog::GetInstance()->GetLogger()->NewLogMessage("visitor is ON");
	new_size = distance_to_camera * 0.065f;

	/*if (new_size < (radius * scale.x()))
		new_size = radius * scale.x();
	
	if (new_size < (radius * scale.y()))
		new_size = radius * scale.y();
	
	if (new_size < (radius * scale.z()))
		new_size = radius * scale.z();*/

	matrix_s.makeScale(new_size, new_size, new_size);


	if (adapt_orientation)
	{
		matrix_r.identity();

		osg::Matrix orientation_mx, orientation_my, orientation_mz;	
			
		cpw::Point3d<float> orientation2;
		
		orientation_mx.makeRotate(orientation.x, osg::Vec3(1.0f, 0.0f, 0.0f));
		orientation_my.makeRotate(orientation.y, osg::Vec3(0.0f, 1.0f, 0.0f));
		orientation_mz.makeRotate(orientation.z, osg::Vec3(0.0f, 0.0f, 1.0f));

		matrix_r = orientation_mx * orientation_my * orientation_mz;

		osg::Quat rot_quat;
		rot_quat.makeRotate(orientation.x, osg::Vec3(1.0f, 0.0f, 0.0f),
							orientation.y, osg::Vec3(0.0f, 1.0f, 0.0f),
							orientation.z, osg::Vec3(0.0f, 0.0f, 1.0f));

		matrix_final = matrix_r * matrix_s * matrix_t;
	}
	else
		matrix_final = matrix_s * matrix_t;

	mt->setMatrix(matrix_final);
	
	return false;

}

bool OsgScene::AddHandler(const std::string &handler_id, const std::string &file_name)
{
	osg::ref_ptr<osg::Node> new_node = osgDB::readNodeFile(file_name);
	
	if (new_node == NULL)
		return false;

	new_node->getOrCreateStateSet()->setAttributeAndModes(new osg::Depth(osg::Depth::ALWAYS), osg::StateAttribute::ON);

	new_node->setNodeMask(OSG_SCENE_GIZMOS_NODE_MASK);
	new_node->setName(handler_id);

	osg::ref_ptr<osg::MatrixTransform> mt = new osg::MatrixTransform;
	
	mt->setName(handler_id);
	mt->addChild(new_node.get());
	if (handlers_node!= NULL)
		handlers_node->addChild(mt.get());

	return true;

}

bool OsgScene::TraslateObjectXAxis(const std::string &object_id, float displacement)
{
	return false;

}

void OsgScene::SetIconVisibility(const std::string &object_id, const bool &vis)
{
	//return;
	if(ObjectExistsInScene(object_id))  
	{
		//Search node
		OsgNodeFinderVisitor node_finder_visitor(object_id);
		entities.get()->accept(node_finder_visitor);

		//check if we found the node
		osg::Node *found_node = node_finder_visitor.GetNode();
		
		if (found_node == NULL)
			return;

		if(vis)
		{
			((osg::LOD*)found_node)->setRange(3, LOD_0_VALUE-1000,LOD_0_VALUE);
			((osg::LOD*)found_node)->setRange(4,LOD_0_VALUE,LOD_1_VALUE);
		}
		else
		{
			((osg::LOD*)found_node)->setRange(3,LOD_0_VALUE,LOD_1_VALUE);
			((osg::LOD*)found_node)->setRange(4,LOD_1_VALUE,INF);
		}

	}
}

void OsgScene::AcumulativeRotation(const int &axis, const float &angle, const std::string &object_id, cpw::Point3d<float> &new_orientation)
{
	OsgNodeFinderVisitor node_finder_visitor(object_id);
	entities.get()->accept(node_finder_visitor);

	//check if we found the node
	osg::Node *found_node = node_finder_visitor.GetNode();

	if ((found_node == NULL) || (found_node->className() != std::string("LOD")))
		return;

	if (((osg::LOD *) found_node)->getChild(0)->className() != std::string("MatrixTransform"))
		return;

	osg::MatrixTransform *mt = (osg::MatrixTransform *) ((osg::LOD *) found_node)->getChild(0);

	osg::Matrix original_matrix = mt->getMatrix();
	osg::Matrix trans_matrix, inv_trans_matrix, rotate_matrix, object_matrix;
	osg::Vec3 trans;
	osg::Vec3 scale;
	osg::Quat rotation, scale_orient;
	
	original_matrix.decompose(trans, rotation, scale, scale_orient);
	
	inv_trans_matrix.makeTranslate(osg::Vec3( -trans.x(), -trans.y(), -trans.z() ));
	trans_matrix.makeTranslate(trans);
			
	if (axis == X_AXIS)
		rotate_matrix.makeRotate(angle, osg::Vec3(1.0f, 0.0f, 0.0f));
	else if (axis == Y_AXIS)
		rotate_matrix.makeRotate(angle, osg::Vec3(0.0f, 1.0f, 0.0f));
	else if (axis == Z_AXIS)
		rotate_matrix.makeRotate(angle, osg::Vec3(0.0f, 0.0f, 1.0f));

	osg::Matrix new_matrix = original_matrix * inv_trans_matrix * rotate_matrix * trans_matrix;
	
	mt->setMatrix(new_matrix);

	object_matrix = new_matrix;
	
	scale.x() = 1.0f / scale.x();
	scale.y() = 1.0f / scale.y();
	scale.z() = 1.0f / scale.z();
	
	osg::Matrix inv_scale_matrix;
	inv_scale_matrix.makeScale(scale);

	new_matrix = new_matrix * inv_trans_matrix * inv_scale_matrix;

	rotation = new_matrix.getRotate();
	
	//normalize the quaternion
	float scalar = sqrt( rotation.w() * rotation.w() + rotation.x() * rotation.x() + rotation.y() * rotation.y() + rotation.z() * rotation.z() );
	scalar = 1.0f / scalar;

	rotation.w() *= scalar;
	rotation.x() *= scalar;
	rotation.y() *= scalar;
	rotation.z() *= scalar;

	//get the euler angles	
	//convert the quaternion to euler angles
	new_orientation.x = atan( (2 * (rotation.w() * rotation.x() + rotation.y() * rotation.z())) / (1 - 2 * (rotation.x() * rotation.x() + rotation.y() * rotation.y())));
	new_orientation.y = asin( 2 * (rotation.w() * rotation.y() - rotation.z() * rotation.x()) );
	new_orientation.z = atan( (2 * (rotation.w() * rotation.z() + rotation.x() * rotation.y())) / (1 - 2 * (rotation.y() * rotation.y() + rotation.z() * rotation.z())));
	
	//rads to grades
	new_orientation.x = (new_orientation.x * 360.0f) / (2*osg::PI);
	new_orientation.y = (new_orientation.y * 360.0f) / (2*osg::PI);
	new_orientation.z = (new_orientation.z * 360.0f) / (2*osg::PI);

	//correction X
	float correction = rotation.w() * rotation.x() + rotation.y() * rotation.z();
	if ( correction > 0.0f)
	{
		if (new_orientation.x < 0.0f)
		{
			new_orientation.x += 180.0f;
			if (new_orientation.x >= 360.0f)
				new_orientation.x -= 360.0f;
		}
	}
	else
	{
		if (new_orientation.x > 0.0f)
		{
			new_orientation.x -= 180.0f;
			if (new_orientation.x <= -360.0f)
				new_orientation.x += 360.0f;
		}
	}

	//correction z
	correction = rotation.w() * rotation.z() + rotation.x() * rotation.y();
	if (correction >= 0.0f)
	{
		if (new_orientation.z < 0.0)
		{
			new_orientation.z += 180.0f;
			if (new_orientation.z >= 360.0f)
				new_orientation.z -= 360.0f;
		}
	}
	else
	{
		if (new_orientation.z >= 0.0)
		{
			new_orientation.z -= 180.0f;
			if (new_orientation.z <= -360.0f)
				new_orientation.z += 360.0f;
		}
	}

	//correction y
	correction = (1 - 2 * (rotation.y() * rotation.y() + rotation.z() * rotation.z()));
	if (correction >= 0.0f)
	{
	}
	else
	{
		if ((new_orientation.x == 0.0f) && (new_orientation.z == 0.0f))
		{
			if (new_orientation.y != 0.0f)
			{
				if (new_orientation.y >= 0.0f)
				{
					new_orientation.y = 90.0f + (90.0f - new_orientation.y);
					if (new_orientation.y >= 360.0f)
						new_orientation.y -= 360.0f;
				}
			}
			else
			{
				new_orientation.y = -(90.0f + (90.0f + new_orientation.y));
				if (new_orientation.y >= 360.0f)
					new_orientation.y -= 360.0f;
			}
		}
	}

	//update the text tag
	std::string debug = ((osg::LOD *) found_node)->getChild(1)->className();
	
	if (((osg::LOD *) found_node)->getChild(1)->className() == std::string("MatrixTransform"))
	{
		osg::MatrixTransform *text_mt = (osg::MatrixTransform *) ((osg::MatrixTransform *) ((osg::LOD *) found_node)->getChild(0))->getChild(1);
		osg::MatrixTransform *obj_mt =  (osg::MatrixTransform *) ((osg::LOD *) found_node)->getChild(0);

		osg::Matrix obj_mi = obj_mt->getInverseMatrix();
		obj_mi.setTrans(0.0f, 0.0f, 0.0f);

		text_mt->setMatrix(obj_mi);
	}



}


void OsgScene::Clear()
{

	OsgNodeFinderVisitor node_finder_visitor("Terrain group");
	if (root_node !=NULL)
		root_node.get()->accept(node_finder_visitor);

	osg::Node *found_node_orig = node_finder_visitor.GetNode();
		
	if (found_node_orig != NULL)
	{
		root_node->removeChild(found_node_orig);
	}



	OsgNodeFinderVisitor node_finder_visitor1("LayersNode");
	if (root_node !=NULL)
		root_node.get()->accept(node_finder_visitor1);

	osg::Node *found_node_orig1 = node_finder_visitor1.GetNode();
		
	if (found_node_orig1 != NULL && root_node !=NULL )
	{
			root_node->removeChild(found_node_orig1);
	}



	OsgNodeFinderVisitor node_finder_visitor2("Entities");
	if (root_node !=NULL)
		root_node.get()->accept(node_finder_visitor2);

	osg::Node *found_node_orig2 = node_finder_visitor2.GetNode();
		
	if (found_node_orig2 != NULL)
	{
		osg::Group *entities_node = ((osg::Group *) found_node_orig2);

		entities_node->removeChild(0, entities_node->getNumChildren());

		if (root_node !=NULL)
			root_node->removeChild(found_node_orig2);
	}



	OsgNodeFinderVisitor node_finder_visitor3("HandlersNode");
	if (root_node !=NULL)
		root_node.get()->accept(node_finder_visitor3);

	osg::Node *found_node_orig3 = node_finder_visitor3.GetNode();
		
	if (found_node_orig3 != NULL && root_node != NULL )
	{
			root_node->removeChild(found_node_orig3);
	}

	


	OsgNodeFinderVisitor node_finder_visitor4("Render_to_texture_cameras");
	if (root_node !=NULL)
		root_node.get()->accept(node_finder_visitor4);

	osg::Node *found_node_orig4 = node_finder_visitor4.GetNode();
		
	if (found_node_orig4 != NULL)
	{
		osg::Group *entities_node = ((osg::Group *) found_node_orig4);

		entities_node->removeChild(0, entities_node->getNumChildren());

		//if (root_node !=NULL)
		//	root_node->removeChild(found_node_orig4);
	}





	OsgNodeFinderVisitor node_finder_visitor5("Camera Gizmos");
	if (root_node !=NULL)
		root_node.get()->accept(node_finder_visitor5);

	osg::Node *found_node_orig5 = node_finder_visitor5.GetNode();
		
	if (found_node_orig5 != NULL && root_node != NULL )
	{
			root_node->removeChild(found_node_orig5);
	}




	OsgNodeFinderVisitor node_finder_visitor6("ReferencedObjects");
	if (root_node !=NULL)
		root_node.get()->accept(node_finder_visitor6);

	osg::Node *found_node_orig6 = node_finder_visitor6.GetNode();
		
	if (found_node_orig6 != NULL)
	{
		osg::Group *entities_node = ((osg::Group *) found_node_orig6);

		entities_node->removeChild(0, entities_node->getNumChildren());

		if (root_node !=NULL)
			root_node->removeChild(found_node_orig6);
	}

	url_node_relation.clear();

	
}


void OsgScene::SetLightning(const bool &_lightning)
{
	if (terrain_group != NULL)
	{
		osg::StateSet *ss = terrain_group->getOrCreateStateSet();
		ss->setMode(GL_LIGHTING, _lightning);
		terrain_group->setStateSet(ss);

		osg::StateSet *ss1 = ocean->getOrCreateStateSet();
		ss1->setMode(GL_LIGHTING, _lightning);
		ocean->setStateSet(ss1);

		osg::StateSet *ss2 = sky->getOrCreateStateSet();
		ss2->setMode(GL_LIGHTING, _lightning);
		sky->setStateSet(ss2);


	}
}


cpw::IScene::SCENE_PERFORMANCE OsgScene::GetPerformance()
{
	return scene_performance;
}

void OsgScene::UpdatePerformance()
{
	actual_t = osg::Timer::instance()->tick();
	acumulated_time += (actual_t - last_t) * osg::Timer::instance()->getSecondsPerTick();
	frames_count++;
	last_t = actual_t;

	if ( acumulated_time >= PERFORMANCE_CHECK_INTERVAL )
	{
		float aux = ((float)frames_count) / PERFORMANCE_CHECK_INTERVAL;

		if (aux < VERY_BAD_FPS)
		{
			scene_performance = VERY_BAD;
		}
		if (aux < BAD_FPS)
		{
			scene_performance = BAD;
			//cpw::ApplicationLog::GetInstance()->GetLogger()->NewLogMessage("BAD_FPS");
		}
		else if (aux < MEDIUM_FPS)
		{
			scene_performance = MEDIUM;
			//cpw::ApplicationLog::GetInstance()->GetLogger()->NewLogMessage("MEDIUM_FPS");
		}
		else if (aux < GOOD_FPS)
		{
			scene_performance = GOOD;
			//cpw::ApplicationLog::GetInstance()->GetLogger()->NewLogMessage("GOOD_FPS");
		}
		else
		{
			scene_performance = EXCELLENT;
			//cpw::ApplicationLog::GetInstance()->GetLogger()->NewLogMessage("EXCELLENT_FPS");
		}

		acumulated_time = 0.0f;
		frames_count = 0;
	}
}


float OsgScene::SuggestParticleSystemStepDistance()
{
	float out = FIRE_STEP_DISTANCE;
	switch (GetPerformance())
	{
		case VERY_BAD:
			out = FIRE_STEP_DISTANCE * 7.0f;
			break;
		case BAD : 
			out = FIRE_STEP_DISTANCE * 4.0f;
			break;
		case MEDIUM : 
			out =  FIRE_STEP_DISTANCE * 2.0f;
			break;
		case GOOD : 
			out =  FIRE_STEP_DISTANCE;
			break;
		case EXCELLENT : 
			out =  FIRE_STEP_DISTANCE * 0.5f;
			break;
	}

	return out;
}

void OsgScene::CutDownVerticesIfNeccesary( std::vector< cpw::Point3d<float> > &scene_coords_vec )
{
	//do not cut vertices down if there are not enough of them
	if (scene_coords_vec.size() < 30) 
		return;

	int aux = 0;
	
	switch (GetPerformance())
	{
		case VERY_BAD:
			aux = 5;
			break;
		case BAD : 
			aux = 1;
			break;
		default:
			return;
			break;
	}
	
	//std::ostringstream msg;
	//msg << "Before " << scene_coords_vec.size();
			

	{
		std::vector< cpw::Point3d<float> > temp;
		
		int aux2 = 0;

		for (std::vector< cpw::Point3d<float> >::iterator iter = scene_coords_vec.begin();
			 iter != scene_coords_vec.end();
			 iter++)
		{			
			if (aux2 >= aux)
			{
				temp.push_back( *iter );
				aux2 = 0;
			}
			else
				aux2++;
			
		}

		scene_coords_vec = temp;
	}

	//msg << " after " << scene_coords_vec.size();
	//cpw::ApplicationLog::GetInstance()->GetLogger()->NewLogMessage(msg.str());
}


void OsgScene::ShowBuildScene()
{
	build_scene_geode = new osg::Geode;
	build_scene_geom = new osg::Geometry;
	build_scene_vertices = new osg::Vec3Array;
}

//shaders
bool OsgScene::loadShaderSource(osg::Shader* obj, const std::string& fileName )
{
   std::string fqFileName = osgDB::findDataFile(fileName);
   if( fqFileName.length() == 0 )
   {
      std::cout << "File \"" << fileName << "\" not found." << std::endl;
      return false;
   }
   bool success = obj->loadShaderSourceFromFile( fqFileName.c_str());
   if ( !success  )
   {
      std::cout << "Couldn't load file: " << fileName << std::endl;
      return false;
   }
   else
   {
      return true;
   }
}

void OsgScene::PreBuild()
{
	// PJ
	root_node->addChild(layers_node.get());
	
}

unsigned long int OsgScene::GetWCS(const double &x_min, const double &y_min, const double &x_max, const double &y_max, const unsigned int &resx, const unsigned int &resy)
{
	redirect_wms_requests_to_scene_builder = true;
	generate=false;
	preview = false;

	res_x = resx;
	res_y = resy;

	 unsigned long int id = scene_builder.NewPetitionWCS(cpw::Point2d<double>(x_min, y_min), cpw::Point2d<double>(x_max, y_max), resx, resy);

	 return id;

}

unsigned long int OsgScene::BuildScene(const double &up_left_corner_x, const double &up_left_corner_y, const double &down_right_corner_x, const double &down_right_corner_y, const int &width, const int &height, const bool force, const bool _preview)
{
	redirect_wms_requests_to_scene_builder = true;
	generate=false;
	preview = _preview; 
	scene_builder.SetCoords(up_left_corner_x, up_left_corner_y, down_right_corner_x, down_right_corner_y);
	terrain_group->removeChild(0, terrain_group->getNumChildren());
	entities->removeChild(0, entities->getNumChildren());

	unsigned long int id = scene_builder.NewPetition(cpw::Point2d<float>(up_left_corner_x, up_left_corner_y), cpw::Point2d<float>(down_right_corner_x, down_right_corner_y), width, height, force);

	return id;
}


void OsgScene::PetitionReady(const std::map<int, std::string> &img_filenames, const unsigned long int &petition_id) 
{ 
	
	osg_image_mixer->MixImages(img_filenames, petition_id); 
}

int OsgScene::ReturnRequest(cpw::Request &request)
{ 
	if (redirect_wms_requests_to_scene_builder)
	{		
		scene_builder.RequestAttended(request.id);

		std::stringstream ss;
		if(preview)
		{
			ss << "Pending Petitions: "<< scene_builder.NumPendingPetitions();
			cpw::ApplicationLog::GetInstance()->GetLogger()->NewLogMessage(ss.str());
			
			osg::Geode* mio = scene_builder.RequestAttended(request);	
			if(root_node != NULL)
			{
				mio->setNodeMask(OSG_SCENE_TERRAIN_NODE_MASK);
				mio->setName("builder");
				terrain_group->addChild(mio);
			}
		}
		else
		{ 
			scene_builder.Generate(request);
		}
	}
	else
	{
		osg_image_mixer->MixImages(request.tiles, request.id); 
	}
	
	return 0;
}

void OsgScene::SetRequestManager(cpw::IRequestReceiver *rm) 
{
	osg_petition_manager.SetRequestManager(rm);
	scene_builder.SetRequestManager(rm);
}

void OsgScene::SetWCSRequestManager(cpw::IRequestReceiver *rm) 
{	
	scene_builder.SetWCSRequestManager(rm);
}

void OsgScene::SetRequestMap(std::map<int, std::vector<unsigned long int>> _rm)
{
	scene_builder.SetRequestMap(_rm);
}

void OsgScene::SetRequestDEMMap(std::map<int, std::vector<unsigned long int>> _rdm)
{
	scene_builder.SetRequestDEMMap(_rdm);
}


void OsgScene::AddLine3(const std::string &object_id, const std::vector<cpw::Point3d<float>> &draw_array, const std::string &color,const int &r, const int &g, const int &b, const int &width, const bool &visible)
{

	if(draw_array.size()<=1) return;
	

	if(ObjectExistsInScene(object_id))  
	{
		Remove(object_id);
	}
	std::vector<cpw::Point3d<float>>::const_iterator iter;
	std::vector<cpw::Point3d<float>>::const_iterator f_iter;
	std::vector<cpw::Point3d<float>> v_aux, final_vec;

	cpw::Point3d<float> aux;
	for(iter = draw_array.begin(); iter != draw_array.end(); iter++)
	{
		aux = UTMToSceneCoords(*iter);
		v_aux.push_back(cpw::Point3d<float>((float)aux.x, (float)aux.y, (float)aux.z) );
	}
	

	osg::ref_ptr<osg::Geometry> geom = new osg::Geometry;
	geom->addPrimitiveSet(new osg::DrawArrays(GL_LINE_STRIP ,0,v_aux.size()));

	osg::ref_ptr<osg::Vec3Array> v = new osg::Vec3Array;
	geom->setVertexArray(v.get());
	v->resize(v_aux.size());

	int i=0;
	for(f_iter = v_aux.begin(); f_iter != v_aux.end(); f_iter++)
	{
		(*v)[i] = osg::Vec3d(f_iter->x,f_iter->y,f_iter->z);
		i++;
	}


	//////////////////////// COLORRRRRRR/////////////////////////
	float rf, gf, bf;
	int red(r), green(g), blue(b);
	if(red<0) red=0;
	if(green<0) green=0;
	if(blue<0) blue=0;

	if(red>255) red=255;
	if(green>255) green=255;
	if(blue>255) blue=255;
	
	rf  = red / 255;
	gf  = green / 255;
	bf  = blue / 255;

	osg::ref_ptr<osg::Vec4Array> colors = new osg::Vec4Array;
	colors->push_back(osg::Vec4(rf,gf,bf,1.0f));
    geom->setColorArray(colors.get());

    //geom->setColorBinding(osg::Geometry::BIND_OVERALL); 
	geom->setColorBinding(osg::Geometry::BIND_PER_PRIMITIVE_SET);
    /////////////////////////////////////////////////////////////



	//////////////////////// GROSOR /////////////////////////

	////////////////////////////////////////////////////////



	osg::ref_ptr<osg::Geode> geode = new osg::Geode();
	geode->addDrawable(geom.get());
	geode->setName(object_id);

	entities->addChild(geode.get());

	////make the object visible or not
	SetVisible("", object_id, visible);

	return;
}






