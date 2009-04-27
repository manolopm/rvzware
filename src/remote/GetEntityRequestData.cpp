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
#include <vector>

#include <remote/stdafx.h>

#include <cpw/common/pstdint.h>
#include <cpw/common/TypeId.h>

#include <remote/DataStream.h>
#include <remote/MessageData.h>
#include <remote/GetEntityRequestData.h>


using namespace cpw::remote;


/*!
 *  Sets the corresponding MessageType
 */
GetEntityRequestData::GetEntityRequestData()
{
	SetMessageType(msgTypeGetEntityRequest);
}


/*!
 *  \brief Returns a DataStream with all the information of the stream
 *
 *  Returns a DataStream with all the information of the stream, ready to be sent throught the network.
 *
 *  Returns an empty DataStream in case there is an error.
 */
DataStream GetEntityRequestData::Code()
{
	try
	{
		uint32_t msg_size = 6 + cpw::TypeId::size() + 1;
		DataStream data(msg_size);

		data << (uint8_t) message_type;
		data << msg_size;
		data << request_id;

		data << entity_id;

		data << send;

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
bool GetEntityRequestData::Decode(const DataStream &data_stream)
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

		data >> tmp8;
		request_id = tmp8;

		data >> entity_id;

		data >> send;

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
bool GetEntityRequestData::IsRequest()
{
	return true;
}


/*!
 *  \brief Tells wheter the message is a response or not
 *
 *  Tells wheter the message is a response or not.
 */
bool GetEntityRequestData::IsResponse()
{
	return false;
}


/*!
 *  \brief Returns the entity id
 *
 *  Returns the entity id.
 */
const cpw::TypeId &GetEntityRequestData::GetEntityId()
{
	return entity_id;
}


/*!
 *  \brief Returns true if the message requests the entity data
 *
 *  Returns true if the message requests the entity data. Returns false in case the purpose of the
 *  request is a synchronization of the entity.
 */
bool GetEntityRequestData::GetSendEntity()
{
	return send;
}


/*!
 *  \brief Returns the entity id
 *
 *  Returns the entity id.
 */
const cpw::TypeId &GetEntityRequestData::GetEntityId() const
{
	return entity_id;
}


/*!
 *  \brief Returns true if the message requests the entity data
 *
 *  Returns true if the message requests the entity data. Returns false in case the purpose of the
 *  request is a synchronization of the entity.
 */
bool GetEntityRequestData::GetSendEntity() const
{
	return send;
}


/*!
 *  \brief Sets the entity id
 *  \param id Entity id
 *
 *  Sets the entity id.
 */
void GetEntityRequestData::SetEntityId(const cpw::TypeId &id)
{
	entity_id = id;
}


/*!
 *  \brief Sets the message to be a download or synchronize request
 *  \param send A boolean meaning the purpose of the request
 *
 *  Sets the message to be a download or synchronize request.
 *
 *  SendEntity = true means the corresponding response message should include all the data of the message.
 *  SendEntity = false means the corresponding response should be a synchronization.
 */
void GetEntityRequestData::SetSendEntity(bool send)
{
	this->send = send;
}
