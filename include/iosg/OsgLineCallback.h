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
#ifndef _OSGLINECALLBACK_
#define _OSGLINECALLBACK_

#include <osg/NodeCallback>
#include <osg/Group>
#include <osg/Geometry>

#include <cpw/common/types.h>
#include <cpw/graphic/CurveAdapter.h>
//#include <iosg/OsgCurveAdapter.h>

#include <iosg/Export.h>

namespace cpw 
{ 
	namespace iosg 
	{

		/**
			\brief This class defines the behaviour of the line callbacks for differents LODs
			\ingroup iosg
		*/
		class IOSGEXPORT OsgLineCallback : public osg::NodeCallback
		{
			public:
				/** Constructor */
				OsgLineCallback(void);

				/** Destructor */
				~OsgLineCallback(void);

				/** overload of operator () 
					/param *node
					/param *nv
				*/
				virtual void operator ()(osg::Node *node, osg::NodeVisitor *nv);

				/**  */
				void SetGeometry(osg::Geometry* g);
				/**  */
				void SetNewInitialVertex(const std::vector<cpw::Point3d<float> > &draw_array);
				/**  */
				void SetLeftBrother(OsgLineCallback *left_brother) { brothers[0] = left_brother; need_to_update=true; }
				/**  */
				void SetRightBrother(OsgLineCallback *right_brother) { brothers[1] = right_brother; need_to_update=true; }
				/**  */
				void SetCurveAdapterStepDistance(const float &_curve_adapter_step_distance) { curve_adapter_step_distance = _curve_adapter_step_distance;}
				/**  */
				void SetCurveAdapter(cpw::CurveAdapter *_curve_adapter) { curve_adapter = _curve_adapter; }

				/**  */
				void SetID(int _id) { id = _id; }
				
				//gets
				/**  */
				OsgLineCallback * GetLeftBrother() { return brothers[0]; }
				/**  */
				OsgLineCallback * GetRightBrother() { return brothers[1]; }
				/**  */
				const OsgLineCallback * GetLeftBrother() const { return brothers[0]; }
				/**  */
				const OsgLineCallback * GetRightBrother() const { return brothers[1]; }
				/**  */
				float GetCurveAdapterStepDistance() { return curve_adapter_step_distance; }
				/**  */
				const float GetCurveAdapterStepDistance() const { return curve_adapter_step_distance; }
				/**  */
				cpw::CurveAdapter * GetCurveAdapter() { return curve_adapter; }

				/**  */
				bool IsActive() {return active;}


			private:

				void SetBrothersToInactive();
				void AdaptVerticesToTerrain();

				bool active, need_to_update;
				float curve_adapter_step_distance;

				osg::Geometry* geom;
				cpw::CurveAdapter *curve_adapter;

				unsigned int id;

				OsgLineCallback *brothers[2];

				std::vector< cpw::Point3d<float> > temp_vec0, temp_vec1;
				std::vector< cpw::Point3d<float> > orig;
		};
	}
}

#endif
