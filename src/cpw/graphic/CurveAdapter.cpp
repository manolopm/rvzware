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

//#include <cpw/stdafx.h>

//#include <iosg/OsgCurveAdapter.h>
//#include <iosg/OsgIntersectorHeightCalculator.h>

#include <cpw/common/Math.h>
#include <cpw/common/ApplicationLog.h>
#include <cpw/graphic/CurveAdapter.h>

using namespace cpw;


CurveAdapter::CurveAdapter(void) : step_distance(125.0f), height_tolerance(25.0f), height_calculator(NULL), height_displacement(1.0f)
{
}

CurveAdapter::~CurveAdapter(void)
{
}

void CurveAdapter::Adapt(const std::vector< cpw::Point3d<float> > &original_vec, std::vector< cpw::Point3d<float> > &new_vec)
{
	new_vec.clear();

	collision_count = 0;

	if ( original_vec.empty() || (original_vec.size() < 2) || (height_calculator == NULL) )
		return;

	for (unsigned int i=0; i<original_vec.size()-1; i++)
	{
		AdaptPointsPair(original_vec[i], original_vec[i+1], new_vec);
	}

	float z;
	if (height_calculator->GetHeight(original_vec.back().x, original_vec.back().y, z))
		new_vec.push_back(cpw::Point3d<float>(original_vec.back().x, original_vec.back().y, z + height_displacement));

	/*std::stringstream cc;
	cc << "collision_count " << collision_count;
	if (cpw::ApplicationLog::GetInstance()->GetLogger() != NULL)
		cpw::ApplicationLog::GetInstance()->GetLogger()->NewLogMessage(cc.str());*/


}

void CurveAdapter::AdaptPointsPair(const cpw::Point3d<float> &v0, const cpw::Point3d<float> &v1, std::vector< cpw::Point3d<float> > &new_vertices)
{
	cpw::Point3d<float> difference( v1 - v0 );
	std::vector< cpw::Point3d<float> > original_interpolated_vertices;
	float d = cpw::Math::DistanceBetweenTwoPoints(v0, v1);
	float d_inv = 1.0f / d;
	float step_position;
	cpw::Point3d<float> aux;

	for (int i = 0; (i * step_distance) < d; i++)
	{
		step_position = i * step_distance * d_inv;

		aux.x = v0.x + (difference.x * step_position);
		aux.y = v0.y + (difference.y * step_position);
			
		if (height_calculator->GetHeight(aux.x, aux.y, aux.z))
			new_vertices.push_back(cpw::Point3d<float>(aux.x, aux.y, aux.z + height_displacement));

		collision_count++;
	}
}
