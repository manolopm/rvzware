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
#include <remote/stdafx.h>

#include <cpw/common/pstdint.h>
#include <cpw/common/TypeId.h>

#include <remote/DataStream.h>
#include <remote/MessageData.h>
#include <remote/DisconnectEntityData.h>


using namespace cpw::remote;


/*!
 *  Sets the corresponding MessageType
 */
DisconnectEntityData::DisconnectEntityData()
{
	SetMessageType(msgTypeDisconnectEntity);
}


/*!
 *  \brief Returns a DataStream with all the information of the stream
 *
 *  Returns a DataStream with all the information of the stream, ready to be sent throught the network.
 *
 *  Returns an empty DataStream in case there is an error.
 */
DataStream DisconnectEntityData::Code()
{
	try
	{
		uint32_t msg_size = 5 + cpw::TypeId::size();
		DataStream data(msg_size);

		data << (uint8_t) message_type;
		data << msg_size;

		data << entity_id;

		return data;
	}
	catch (DataStreamException)
	{
		return DataStream();
	}
}


/*!
 *  \brief Creates the message from a DataStream
 *  \param data_stream The data to be used to compose the message
 *
 *  Creates the message from a DataStream.
 *
 *  Returns a true on success. Returns false otherwise.
 */
bool DisconnectEntityData::Decode(const DataStream &data_stream)
{
	try
	{
		DataStream data(data_stream);

		uint8_t tmp8;
		data >> tmp8;
		message_type = (MessageType) tmp8;

		uint32_t msg_size;
		data >> msg_size;

		if (data.GetSize() != msg_size)
			return false;

		data >> entity_id;

		return true;
	}
	catch (DataStreamException)
	{
		return false;
	}
}


/*!
 *  \brief Tells wheter the message is a request or not
 *
 *  Tells wheter the message is a request or not.
 */
bool DisconnectEntityData::IsRequest()
{
	return false;
}


/*!
 *  \brief Tells wheter the message is a response or not
 *
 *  Tells wheter the message is a response or not.
 */
bool DisconnectEntityData::IsResponse()
{
	return false;
}


/*!
 *  \brief Returns the entity id
 *
 *  Returns the entity id.
 */
const cpw::TypeId &DisconnectEntityData::GetEntityId()
{
	return entity_id;
}


/*!
 *  \brief Returns the entity id
 *
 *  Returns the entity id.
 */
const cpw::TypeId &DisconnectEntityData::GetEntityId() const
{
	return entity_id;
}


/*!
 *  \brief Returns the entity id
 *
 *  Returns the entity id.
 */
void DisconnectEntityData::SetEntityId(const cpw::TypeId &id)
{
	entity_id = id;
}
