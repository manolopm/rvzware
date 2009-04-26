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


#ifndef _OSGSUN_
#define _OSGSUN_

#include <osg/Node>
#include <osg/MatrixTransform>
#include <osg/ref_ptr>
#include <osg/Light>
#include <osg/LightSource>

#include <cpw/graphic/IScene.h>
#include <string>
#include <map>

#include <iosg/Export.h>


namespace cpw 
{ 
	namespace iosg 
	{

		/** 
			\brief Por comentar ....
			\ingroup iosg
		*/
		class IOSGEXPORT OsgSun
		{
			public:

				OsgSun(const std::string &url);
				~OsgSun();

				void SetSunDistance(const float &sun_d) {sun_distance = sun_d;};
				void SetRotation(const cpw::Point3d<float> &_rotation);
				void SetRelativeRotation(const cpw::Point3d<float> &_rotation);
				void SetTraslation(const cpw::Point3d<float> &_position);
				void SetScale(const cpw::Point3d<float> &_scale);

				float & GetSunDistance()	{ return sun_distance; };
				cpw::Point3d<float> & GetTraslation()	{ return traslation; };
				cpw::Point3d<float> & GetSize()		{ return scale; }
				cpw::Point3d<float> & GetRotation()	{ return rotation; }			

				void Update();

				osg::Group * GetSunMatrixTransform() {return osg_matrix.get();}

			private:

				void CalculateAmbientLight();

				std::string default_directory;

				osg::ref_ptr<osg::Group> light_group;			
				osg::ref_ptr<osg::Light> sunLight;
				cpw::Point3d<float> traslation, rotation, scale;
				float sun_distance, sun_relative_pos;
				osg::ref_ptr<osg::MatrixTransform> osg_matrix;
				osg::Matrix t_matrix;
				osg::Node *new_node;

		};
	}
}

#endif
