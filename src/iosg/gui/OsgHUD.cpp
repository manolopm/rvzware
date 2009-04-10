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

#include "stdafx.h"

#include <sstream>

#include <iosg/gui/OsgHUD.h>
#include <iosg/gui/OsgWidget.h>
#include <iosg/gui/OsgContainer.h>

using namespace cpw::iosg;

OsgIHUD::OsgIHUD(void) : id_generator(0), active_widget(NULL)
{
}

OsgIHUD::~OsgIHUD(void)
{
	std::vector<OsgIWidget *>::iterator i = widgets.begin();

	for ( ; i != widgets.end(); i++)
	{
		delete *i;
	}
}

void OsgIHUD::Update()
{
	std::vector<OsgIWidget *>::iterator i = widgets.begin();

	for ( ; i != widgets.end(); i++)
	{
		(*i)->Update();
	}
}

void OsgIHUD::UpdateSize(const int &_size_x, const int &_size_y)
{
	std::vector<OsgIWidget *>::iterator i = widgets.begin();

	for ( ; i != widgets.end(); i++)
	{
		(*i)->ResizeScreen(_size_x, _size_y);
	}


}

void OsgIHUD::AddWidget(OsgIWidget *widget/*, const cpw::Point3d<float> &pos,
			                              const cpw::Point3d<float> &size,
										  const cpw::Point3d<float> &rotation*/)
{
	std::ostringstream os;

	os << id_generator;
	id_generator++;

	widgets.push_back(widget);
	widget->SetApplication(application);
	widget->SetParentWindow(parent_window);
	
	//widget->SetSize(size);
	//widget->SetPosition(pos);
	//widget->SetSize(size);
	//widget->SetRotation(rotation);

	root->addChild(widget->GetMatrixTransform());

	OsgIContainer *container = dynamic_cast<OsgIContainer *>(widget);
	if (container != NULL)
	{
		for (unsigned int i = 0; i<container->GetNumChildren(); i++)
		{
			AddMore(container->GetChild(i));
			//root->addChild(container->GetChild(i)->GetMatrixTransform());
			//OsgIContainer *container2 = dynamic_cast<OsgIContainer *>(container->GetChild(i));
		}
	}
}

void OsgIHUD::AddMore(OsgIWidget *widget/*, const cpw::Point3d<float> &pos,
			                              const cpw::Point3d<float> &size,
										  const cpw::Point3d<float> &rotation*/)
{
	//widgets.push_back(widget);

	widget->SetApplication(application);
	widget->SetParentWindow(parent_window);
	

	root->addChild(widget->GetMatrixTransform());
	
	OsgIContainer *container = dynamic_cast<OsgIContainer *>(widget);
	
	if (container != NULL)
	{
		for (unsigned int i = 0; i<container->GetNumChildren(); i++)
			AddMore(container->GetChild(i));
	}

}

void OsgIHUD::SetActiveWidget(const std::string &widget_id)
{
	std::vector<OsgIWidget *>::iterator i = widgets.begin();

	active_widget = NULL;

	for ( ; i != widgets.end(); i++)
	{
		OsgIContainer *container = dynamic_cast<OsgIContainer *>(*i);

		if ((*i)->GetId() == widget_id)
			active_widget = *i;
		else if (container != NULL)
		{
			for (unsigned int i = 0; i<container->GetNumChildren(); i++)
				SearchForActiveWidget(container->GetChild(i), widget_id);
		}
	}

	//active_widget->SetSize(cpw::Point3d<float>(2000, 2000, 200));
}

void OsgIHUD::SearchForActiveWidget(OsgIWidget *widget, const std::string &widget_id)
{
	OsgIContainer *container = dynamic_cast<OsgIContainer *>(widget);

	if (widget->GetId() == widget_id)
		active_widget = widget;
	else if (container != NULL)
	{
		for (unsigned int i = 0; i<container->GetNumChildren(); i++)
			SearchForActiveWidget(container->GetChild(i), widget_id);
	}
}
