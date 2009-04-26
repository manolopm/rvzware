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

#include <fstream>
#include <sstream>
#include <stdlib.h>


#include <iosg/stdafx.h>

#include <iosg/OsgSceneLoader.h>
#include <iosg/OsgScene.h>

#include <cpw/common/types.h>
#include <cpw/graphic/ApplicationScene.h>




using namespace cpw::iosg;

OsgSceneLoader::OsgSceneLoader(void)
{
	error = false;
}

OsgSceneLoader::~OsgSceneLoader(void)
{
}


void * OsgSceneLoader::LoadScene(const std::string &filename)
{
	error = false;
	std::ifstream scene_file(filename.c_str());

	if (!scene_file.is_open())
	{
		error = true;
		return NULL;
	}

	osg::Group *root_node = new osg::Group;
	root_node->setName("root");


	std::string tag;
	scene_file >> tag;
	while (tag != "[OSG]" && (!scene_file.eof()))
	{
		scene_file >> tag;
	}
	if (tag == "[OSG]")
	{std::string model_file;
		do
		{
			int cont = 0;
			
			float utm1 = 0.0f;
			float utm2 = 0.0f;
			float utm3 = 0.0f;

			std::string s_utm1;
			std::string s_utm2;
			std::string s_utm3;

			char *fail_conv;

			do
			{
				scene_file >> model_file >> s_utm1 >> s_utm2 >> s_utm3;
			} while (((model_file.empty()) || (model_file.at(0) == '\n') || (model_file.at(0) == '\t') || (model_file.at(0) == ' ') || (model_file == "")) && (!scene_file.eof()));
			
			utm1 = strtod(s_utm1.c_str(), &fail_conv);
			if(*fail_conv != 0) 
				continue;
			
			utm2 = strtod(s_utm2.c_str(), &fail_conv);
			if(*fail_conv != 0) 
				continue;

			utm3 = strtod(s_utm3.c_str(), &fail_conv);
			if(*fail_conv != 0) 
				continue;


			//Add the object to the root node
			if ((!scene_file.eof()) && (model_file.at(0)!='['))
				this->AddNodeToTree(root_node, model_file, utm1, utm2, utm3);				

		}while((!scene_file.eof()) && (model_file.at(0)!='['));
	}
	

	scene_file.close();

	osg::StateSet *ss = root_node->getOrCreateStateSet();
	ss->setMode(GL_LIGHTING, true);
	//ss->setMode(GL_LIGHTING, true);
	ss->setMode(GL_CULL_FACE,osg::StateAttribute::ON);
	root_node->setStateSet(ss);

	return (void *) root_node;
}


void OsgSceneLoader::AddNodeToTree(osg::Group *root, const std::string &filename, float utm1, float utm2, float utm3)
{
	osg::ref_ptr<osg::Node> new_node = osgDB::readNodeFile(filename);

	//check if the model was loaded
	if (new_node == NULL)
	{
		error = true;
		return;
	}

	new_node->setNodeMask(OSG_SCENE_TERRAIN_NODE_MASK);

	osg::StateSet *ss = new_node->getOrCreateStateSet();
	

	new_node->setName(filename);


	
	osg::ref_ptr<osg::MatrixTransform> transformation_node = new osg::MatrixTransform;

	osg::Matrix transformation_matrix;
		
	cpw::Point3d<float> converted_coords(utm1, utm2, utm3);
	converted_coords = OsgScene::UTMToSceneCoords(converted_coords);

	//transformation_matrix.setTrans(converted_coords.x, converted_coords.y, converted_coords.z);
	transformation_matrix.setTrans(utm1, utm2, utm3);
		
	transformation_node.get()->setMatrix(transformation_matrix);
	transformation_node->setName(filename);



	transformation_node.get()->addChild(new_node.get());
	root->addChild(transformation_node.get());
	
}

