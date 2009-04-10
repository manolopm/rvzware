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
#ifndef _OSGICONSIZEUPDATERVISITOR_
#define _OSGICONSIZEUPDATERVISITOR_

#include <osg/NodeVisitor>
#include <osg/ref_ptr>
#include <osg/MatrixTransform>
#include <cpw/common/types.h>

#include <iosg/Export.h>


namespace cpw 
{ 
	namespace iosg 
	{

		/** 
			\brief Por comentar...
			\ingroup iosg
		*/
		class IOSGEXPORT IconUserData : public osg::Referenced
		{
			public:
			
				IconUserData(){ height = 0.0f; }
				~IconUserData(){}

				void SetPosition(const cpw::Point3d<float> &position) { original_position = position; }
				cpw::Point3d<float> & GetPosition() { return original_position; }

				void SetHeight(const float _height) { height = _height; }
				float GetHeight() { return height; }

			private:

				cpw::Point3d<float> original_position;
				float height;

		};

		/** 
			\brief 
			\ingroup iosg
		*/
		class IOSGEXPORT OsgIconSizeUpdaterVisitor : public osg::NodeVisitor
		{
			public:
				OsgIconSizeUpdaterVisitor(const float &_max_size, const float &_min_size,
										  const float &_max_distance, const float &_min_distance,
										  const float &_camera_pos_x, const float &_camera_pos_y,
										  const float &_camera_pos_z, const float &_max_height, const float &_min_height);
				~OsgIconSizeUpdaterVisitor(void);

				virtual void apply( osg::Node& node );

			private:

				float min_size, max_size, min_distance, max_distance, camera_pos_x, camera_pos_y, camera_pos_z, max_height, min_height;
		};
	}
}

#endif