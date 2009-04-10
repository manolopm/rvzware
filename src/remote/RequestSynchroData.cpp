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

#include <remote/RequestSynchroData.h>
#include <remote/DataStream.h>

using namespace cpw::remote;


/*!
 *  Sets the corresponding MessageType
 */
RequestSynchroData::RequestSynchroData()
{
	SetMessageType(msgTypeRequestSynchro);
}


/*!
 *  \brief Returns a DataStream with all the information of the stream
 *
 *  Returns a DataStream with all the information of the stream, ready to be sent throught the network.
 *
 *  Returns an empty DataStream in case there is an error.
 */
DataStream RequestSynchroData::Code()
{
	try
	{
		uint32_t msg_size = 5 + cpw::TypeId::size() + cpw::TypeId::size() + 8 + 8;

		//Get the content into the stream
		DataStream data(msg_size);

		//control data
		data <<(uint8_t) message_type;
		data << msg_size;

		//Message id
		data << message_id;

		//Entity's id
		data << entity_id;
				
		//timestamp of current time
		data << current_time;

		//timestamp of last synchro
		data << last_synchro;

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
bool RequestSynchroData::Decode(const DataStream &data_stream)
{
	try
	{
		DataStream data(data_stream);

		//Get the control data
		uint8_t msgt;
		data >> msgt;
		message_type = (MessageType) msgt;

		uint32_t msg_size;
		data >> msg_size;

		if (data.GetSize() != msg_size)
			return false;

		data >> message_id;

		data >> entity_id;

		//Timestamp of current time
		data >> current_time;

		//Timestamp of last synch
		data >> last_synchro;

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
bool RequestSynchroData::IsRequest()
{
	return false;
}


/*!
 *  \brief Tells wheter the message is a response or not
 *
 *  Tells wheter the message is a response or not.
 */
bool RequestSynchroData::IsResponse()
{
	return false;
}


/*!
 *  \brief Returns the current entity time set on the message
 *
 *  Returns the current entity time set on the message.
 */
uint64_t RequestSynchroData::GetCurrTime()
{
	return current_time;
}


/*!
 *  \brief Returns the timestamp of the last syncrhonization between the nodes
 *
 *  Returns the timestamp of the las syncrhonization between the nodes.
 */
uint64_t RequestSynchroData::GetLastSynchro()
{
	return last_synchro;
}


/*!
 *  \brief Returns the entity id
 *
 *  Returns the entity id.
 */
const cpw::TypeId &RequestSynchroData::GetEntityId()
{
	return entity_id;
}


/*!
 *  \brief Returns the message id
 *
 *  Returns the message id.
 *
 *  It is used to avoid processing the same message more than once.
 */
const cpw::TypeId &RequestSynchroData::GetMessageId()
{
	return message_id;
}



/*!
 *  \brief Returns the timestamp of the last syncrhonization between the nodes
 *
 *  Returns the timestamp of the las syncrhonization between the nodes.
 */
uint64_t RequestSynchroData::GetLastSynchro() const
{
	return last_synchro;
}


/*!
 *  \brief Returns the current entity time set on the message
 *
 *  Returns the current entity time set on the message.
 */
uint64_t RequestSynchroData::GetCurrTime() const
{
	return current_time;
}


/*!
 *  \brief Returns the entity id
 *
 *  Returns the entity id.
 */
const cpw::TypeId &RequestSynchroData::GetEntityId() const
{
	return entity_id;
}


/*!
 *  \brief Returns the message id
 *
 *  Returns the message id.
 *
 *  It is used to avoid processing the same message more than once.
 */
const cpw::TypeId &RequestSynchroData::GetMessageId() const
{
	return message_id;
}


/*!
 *  \brief Returns the timestamp of the las syncrhonization between the nodes
 *
 *  Returns the timestamp of the las syncrhonization between the nodes.
 */
void RequestSynchroData::SetLastSynchro(const uint64_t &ts)
{
	last_synchro = ts;
}


/*!
 *  \brief Sets the current entity time to be sent
 *
 *  Sets the current entity time to be sent.
 */
void RequestSynchroData::SetCurrTime(const uint64_t &ts)
{
	current_time = ts;
}


/*!
 *  \brief Sets the entity id
 *  \param id Entity id
 *
 *  Sets the entity id.
 */
void RequestSynchroData::SetEntityId(const cpw::TypeId &id)
{
	entity_id = id;
}


/*!
 *  \brief Sets the message id
 *  \param id The id of the message
 *
 *  Sets the message id.
 *
 *  It is used to avoid processing the same message more than once.
 */
void RequestSynchroData::SetMessageId(const cpw::TypeId &id)
{
	message_id = id;
}

