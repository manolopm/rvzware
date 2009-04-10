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

#include "../stdafx.h"

#include <iosg/gui/OsgContainer.h>

using namespace cpw::iosg; 

OsgIContainer::OsgIContainer(const std::string &url, cpw::Point3d<float> _position, cpw::Point3d<float> _size, cpw::Point3d<float> _rotation, const TAnchor &_anchor) : OsgIWidget(url, _position, _size, _rotation, _anchor)
{
}

OsgIContainer::~OsgIContainer(void)
{
	std::map<std::string, iosg::OsgIWidget*>::iterator i = children.begin();
	
	for ( ; i != children.end(); i++)
		delete i->second;

	children.clear();
}


bool OsgIContainer::AttachChild(OsgIWidget *child)
{
	children[child->GetId()] = child;
	return false;
}

bool OsgIContainer::DetachChild(const std::string &name)
{

	std::map<std::string, iosg::OsgIWidget*>::iterator iter = children.find(name);
	children.erase(iter);
	return false;
}

OsgIWidget * OsgIContainer::GetChild(const std::string &name)
{
	std::map<std::string, iosg::OsgIWidget*>::iterator iter = children.find(name);

	if (iter == children.end())
		return NULL;
	else
		return iter->second;
}

OsgIWidget * OsgIContainer::GetChild(const unsigned int &index)
{
	unsigned int i=0;
	
	std::map<std::string, iosg::OsgIWidget*>::iterator iter = children.begin();

	for ( ; iter != children.end(); iter++)
	{
		if (i == index)
			return iter->second;

		i++;
	}

	return NULL;
}

//void OsgIContainer::SetAnchor(const float &w, const float &h, const int &offset_x, const int &offset_y)
//{
//	std::map<std::string, iosg::OsgIWidget*>::iterator iter = children.begin();
//
//	for ( ; iter != children.end(); iter++)
//		iter->second->SetAnchor(w, h, offset_x, offset_y);
//	
//}

void OsgIContainer::ResizeScreen(const int &x, const int &y)
{
	OsgIWidget::ResizeScreen(x, y);

	std::map<std::string, iosg::OsgIWidget*>::iterator iter = children.begin();

	for ( ; iter != children.end(); iter++)
		iter->second->ResizeScreen(x, y);
}

void OsgIContainer::Update(osg::Matrix *mp)
{
 //   osg::Matrix m_to_propagate;

	//if (mp != NULL)
	//	m_to_propagate = *mp;
	//else
	//	m_to_propagate.makeIdentity();

	//m_to_propagate = GetMatrixTransform()->getMatrix() * m_to_propagate;


	//std::map<std::string, iosg::OsgIWidget*>::iterator iter = children.begin();

	//for ( ; iter != children.end(); iter++)
	//{
	//	//iter->second->GetMatrixTransform()->setMatrix(iter->second->GetMatrixTransform()->getMatrix() * m_to_propagate) ;
	//	iter->second->Update(&m_to_propagate); 
	//}

	UpdateTransformationMatrix();
	osg::Matrix own_matrix = GetMatrixTransform()->getMatrix();

	std::map<std::string, iosg::OsgIWidget*>::iterator iter = children.begin();

	if (mp != NULL)
		own_matrix = own_matrix * (*mp);
		
	for ( ; iter != children.end(); iter++)
		iter->second->Update(&own_matrix); 

	GetMatrixTransform()->setMatrix(own_matrix);
	
	/*else
		for ( ; iter != children.end(); iter++)
			iter->second->Update(NULL); */
}

void OsgIContainer::SetVisible(const bool &_visible)
{
	OsgIWidget::SetVisible(_visible);

	std::map<std::string, iosg::OsgIWidget*>::iterator iter = children.begin();

	for ( ; iter != children.end(); iter++)
		iter->second->SetVisible(_visible);
}
