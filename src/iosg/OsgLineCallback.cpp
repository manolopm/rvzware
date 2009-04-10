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
#include <sstream>

#include "stdafx.h"

#include <iosg/OsgLineCallback.h>
#include <cpw/common/ApplicationLog.h>
#include <cpw/common/Math.h>

using namespace cpw::iosg;

OsgLineCallback::OsgLineCallback(void) : active(false), need_to_update(false), curve_adapter(NULL), orig(), geom(NULL)
{
	brothers[0] = NULL;
	brothers[1] = NULL;
}

OsgLineCallback::~OsgLineCallback(void)
{
}


void OsgLineCallback::operator ()(osg::Node *node, osg::NodeVisitor *nv)
{
	
		
	if (active && !need_to_update)
	{
		//return;
	}
	else
	{
		#ifdef _DEBUG
		//std::ostringstream msg;
		//msg <<  << ;
		//cpw::ApplicationLog::GetInstance()->GetLogger()->NewLogMessage(msg.str());
		#endif

		active = true;
		need_to_update = false;
		
		SetBrothersToInactive();

		AdaptVerticesToTerrain();
	}

	osg::NodeCallback::operator ()(node, nv);
}

void OsgLineCallback::SetBrothersToInactive()
{
	//left brothers
	OsgLineCallback *aux = NULL;
	
	if (brothers[0] != NULL)
	{
		aux = GetLeftBrother();
	
		while (aux != NULL)
		{
			aux->active = false;
			aux = aux->GetLeftBrother();
		}
	}

	//right brothers
	if (brothers[1] != NULL)
	{
		aux = GetRightBrother();

		while (aux != NULL)
		{
			aux->active = false;
			aux = aux->GetRightBrother();
		}
	}
}

void OsgLineCallback::AdaptVerticesToTerrain()
{
	//exit preconditions
	if (curve_adapter == NULL)
		return;

	osg::Vec3Array* vertex = (osg::Vec3Array*)geom->getVertexArray();

	if (vertex->empty())
		return;

	temp_vec1.clear();

	//clean the vertices of osg
	vertex->clear();

	//adapt the new vertices
	float aux = curve_adapter->GetStepDistance();
	curve_adapter->SetStepDistance( curve_adapter_step_distance );
	curve_adapter->Adapt(orig, temp_vec1);
	curve_adapter->SetStepDistance( aux );

	geom->removePrimitiveSet(0,1);
	geom->addPrimitiveSet(new osg::DrawArrays(GL_LINE_STRIP ,0,temp_vec1.size()));	
	//fill again the osg vec
	for (std::vector< cpw::Point3d<float> >::iterator iter = temp_vec1.begin();
		 iter != temp_vec1.end();
		 iter++)
	{
		vertex->push_back( osg::Vec3(iter->x, iter->y, iter->z) );
	}	
}

void OsgLineCallback::SetGeometry(osg::Geometry* g) 
{ 
	geom = g; 

	osg::Vec3Array* vertex = (osg::Vec3Array*)geom->getVertexArray();

	if (vertex->empty())
		return;

	for (int i=0; i<(signed)vertex->size(); i++)
		orig.push_back( cpw::Point3d<float>((*vertex)[i].x(), (*vertex)[i].y(), (*vertex)[i].z()) );

}

void OsgLineCallback::SetNewInitialVertex(const std::vector<cpw::Point3d<float> > &draw_array)
{
	if(!orig.empty())
		orig.clear();
	for (int i=0; i<(signed)draw_array.size(); i++)
		orig.push_back(draw_array[i]);//.x(), draw_array[i].y(), draw_array[i].z()) );

	//if(active)
	//	AdaptVerticesToTerrain();
}
