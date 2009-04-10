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
#ifndef _CURVEADAPTER_
#define _CURVEADAPTER_

#include <cpw/Export.h>
#include <cpw/common/types.h>
#include <cpw/graphic/IHeightCalculator.h>

#include <vector>

namespace cpw 
{ 
	/** 
		\brief Por comentar...
		\ingroup iosg
	*/
	class CPWEXPORT CurveAdapter
	{
		public:

			CurveAdapter(void);
			~CurveAdapter(void);
				
			void Adapt(const std::vector< cpw::Point3d<float> > &original_vec, std::vector< cpw::Point3d<float> > &new_vec);
			void SetStepDistance(const float &_step_distance) { step_distance = _step_distance; }
			void SetHeightTolerance(const float &_height_tolerance) { height_tolerance = _height_tolerance; }
			void SetOsgHeightCalculator(cpw::IHeightCalculator *_height_calculator) { height_calculator = _height_calculator; }

			float & GetHeightTolerance() { return height_tolerance; }
			float & GetStepDistance() { return step_distance; }
			cpw::IHeightCalculator * GetOsgHeightCalculator() { return height_calculator; }
			void SetHeightDisplacement(const float &_height_displacement) { height_displacement = _height_displacement; }
			float & GetHeightDisplacement() { return height_displacement; }

		private:

			void AdaptPointsPair(const cpw::Point3d<float> &v0, const cpw::Point3d<float> &v1, std::vector< cpw::Point3d<float> > &new_vertices);

			cpw::IHeightCalculator *height_calculator;
			float step_distance, height_tolerance, height_displacement;
			int collision_count;




	};

}

#endif