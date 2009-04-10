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

#include <iosg/gui/OsgLabel.h>
#include <iosg/OsgObjectRegistrySingleton.h>

using namespace cpw::iosg; 

OsgILabel::OsgILabel(const std::string &url, cpw::Point3d<float> _position, cpw::Point3d<float> _size, cpw::Point3d<float> _rotation, const TAnchor &_anchor, const std::string &label_) : OsgIWidget(url, _position, _size, _rotation, _anchor), label(label_)
{
	SetDefaultPath(url);
	rotation.x = 0.0f;
	rotation.y = 0.0f;
	rotation.z = 0.0f;
	
	OsgIWidget::OsgIWidget(url, _position, _size, _rotation,_anchor);
	if (label != "")
	{
		SetObject(label);
	}

}

OsgILabel::~OsgILabel(void)
{
}


void OsgILabel::SetObject(const std::string &filename)
{

	std::vector<osg::MatrixTransform *> mt_vec;
	object = new osg::Geode;
	debug_text = new  osgText::Text;
	osgText::Font* arial = osgText::readFontFile(this->GetDefaultPath() + "arial.ttf");

	osgText::Font *mifont = OsgObjectRegistrySingleton::GetInstance()->GetObjReg()->GetFontFromFile(this->GetDefaultPath() + "arial.ttf");
		
	debug_text->setFont(arial);
	debug_text->setFontResolution(GetSize().x,GetSize().y);
	debug_text->setPosition(osg::Vec3(GetPosition().x,GetPosition().y,GetPosition().z));
	debug_text->setText(filename);
	debug_text->setCharacterSize(GetSize().x);
	debug_text->setBackdropType(osgText::Text::DROP_SHADOW_BOTTOM_RIGHT);
	debug_text->setBackdropOffset(0.2,0.2);
	(*object).addDrawable( debug_text.get() );

	osg::StateSet *text_ss = (*object).getOrCreateStateSet();
	text_ss->setMode(GL_LIGHTING, false);
	(*object).setStateSet(text_ss);


	if (object == NULL)
	{
		return;
	}


	OsgVisitorFirstGeode vfg;
	vfg.apply(*(object.get()));

	if (vfg.GetFoundGeode() == NULL)
	{	
		return;
	}


	OsgIWidget::SetObject((osg::Node *)object.get());

	sw = new osg::Switch();
	sw->setAllChildrenOn();
	sw->addChild(object.get());
	GetMatrixTransform()->addChild(sw.get());
	SetSwitch(sw);

	MarkTree(GetId(), GetMatrixTransform());
	
	UpdateTransformationMatrix();

}

void OsgILabel::Update(osg::Matrix *mp)
{
	OsgIWidget::Update(mp);
}

void OsgILabel::SetLabel(const std::string &label_)
{
	label = label_; 
	debug_text->setText(label_);
}
