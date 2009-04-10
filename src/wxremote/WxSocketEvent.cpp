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
#include <wx/event.h>
#include <wx/socket.h>

#include "stdafx.h"

#include <remote/DataStream.h>
#include <wxremote/WxSocketEvent.h>


using namespace cpw::wxremote;



DEFINE_EVENT_TYPE(cpw::wxremote::WxSocketThreadEvent)


/*!
 *  \param commandType Type of event
 *  \param id Id of event
 *
 *  Parameters to this constructor are evaluated in the base class. See wxWidgets documentation
 *  about wxCommandEvet class.
 */
WxSocketEvent::WxSocketEvent(wxEventType commandType, int id) :
	wxCommandEvent(commandType, id)
{
}


/*!
 *  \brief Copy constructor
 *  \param event The event to be copied
 *
 *  Copy constructor.
 */
WxSocketEvent::WxSocketEvent(const WxSocketEvent &event) :
	wxCommandEvent(event)
{
	this->type = event.type;
	this->data = event.data;
	this->node = event.node;
}


/*!
 *  \brief Assignment operator
 *  \param event The event to get the data from
 *
 *  Assignment operator.
 */
WxSocketEvent &WxSocketEvent::operator=(const WxSocketEvent &event)
{
	this->type = event.type;
	this->data = event.data;
	this->node = event.node;

	return *this;
}


/*!
 *  \brief Returns the type of the event
 *
 *  Returns the type of the event.
 */
const WxSocketEventType &WxSocketEvent::GetType()
{
	return type;
}


/*!
 *  \brief Returns the stream inside the event
 *
 *  Returns the stream inside the event.
 */
const cpw::remote::DataStream &WxSocketEvent::GetData()
{
	return data;
}


/*!
 *  \brief Returns the id of the node that produced the event
 *
 *  Returns the id of the node that produced the event.
 */
const cpw::RemoteNode &WxSocketEvent::GetNode()
{
	return node;
}


/*!
 *  \brief Returns the type of the event
 *
 *  Returns the type of the event.
 */
const WxSocketEventType &WxSocketEvent::GetType() const
{
	return type;
}


/*!
 *  \brief Returns the stream inside the event
 *
 *  Returns the stream inside the event.
 */
const cpw::remote::DataStream &WxSocketEvent::GetData() const
{
	return data;
}


/*!
 *  \brief Returns the id of the node that produced the event
 *
 *  Returns the id of the node that produced the event.
 */
const cpw::RemoteNode &WxSocketEvent::GetNode() const
{
	return node;
}


/*!
 *  \brief Sets the type of the event
 *
 *  Sets the type of the event.
 */
void WxSocketEvent::SetType(const WxSocketEventType _type)
{
	this->type = _type;
}


/*!
 *  \brief Sets the stream to be contained by the event
 *
 *  Sets the stream to be contained by the event.
 */
void WxSocketEvent::SetData(const cpw::remote::DataStream &_data)
{
	this->data = _data;
}


/*!
 *  \brief Sets the id of the node that produced the event
 *
 *  Sets the id of the node that produced the event.
 */
void WxSocketEvent::SetNode(const cpw::RemoteNode &_node)
{
	this->node = _node;
}


/*!
 *  \brief Returns a copy of the object
 *
 *  Returns a copy of the object.
 *
 *  This is an overridden method, needed for this class to be used as a custom event of wxWidgets.
 */
wxEvent* WxSocketEvent::Clone() const
{
	return new WxSocketEvent(*this);
}

