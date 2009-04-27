/*
 * Rvzware based in CAPAWARE 3D
 *
 * Rvzware is free software; you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 2.1 of the License, or (at your option)
 * any later version.
 *
 * Rvzware is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this application; if not, write to the Free Software Foundation, Inc.,
 * 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 *
 * The Rvzware development team
 */

#include <string>

#include <remote/stdafx.h>

#include <cpw/common/TypeId.h>
#include <cpw/common/pstdint.h>
#include <cpw/common/Loggable.h>

#include <remote/DataStream.h>
#include <remote/SendChangesData.h>
#include <remote/MessageData.h>


using namespace cpw::remote;


/*!
 *  Sets the corresponding MessageType
 */
SendChangesData::SendChangesData()
{
	SetMessageType(msgTypeSendChanges);
}


/*!
 *  \brief Returns a DataStream with all the information of the stream
 *
 *  Returns a DataStream with all the information of the stream, ready to be sent throught the network.
 *
 *  Returns an empty DataStream in case there is an error.
 */
DataStream SendChangesData::Code()
{
	try
	{
		uint32_t msg_size;

		msg_size = 5 + cpw::TypeId::size() + cpw::TypeId::size() + 8 + 4;

		//Calculate size of changes vector
		std::vector<cpw::Change>::iterator it;
		for (it = changes.begin(); it != changes.end(); it++)
			msg_size += 4 + it->field.size() + 4 + it->value.size() + 8;

		DataStream data(msg_size);


		//Introduce the control data
		data <<(uint8_t) message_type;
		data << msg_size;

		//Then the ID of the entity
		data << message_id;

		//Then the ID of the entity
		data << entity_id;

		data << last_synchro;



		//Number of changes
		data << (uint32_t)changes.size();

		for (it = changes.begin(); it != changes.end(); it++)
		{
			data << it->field;
			data << it->value;
			data << (uint64_t) it->time_stamp;
		}

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
bool SendChangesData::Decode(const DataStream &data_stream)
{
	try
	{
		DataStream data(data_stream);

		if (data.GetSize() < 5)
			return true;

		unsigned int i = 0;

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

		data >> last_synchro;

		uint32_t nchanges;

		data >> nchanges;

		while (nchanges > 0)
		{
			std::string field;
			std::string value;
			uint64_t ts;

			data >> field;
			data >> value;
			data >> ts;

			changes.push_back(cpw::Change(field, value, ts));

			nchanges--;
		}

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
bool SendChangesData::IsRequest()
{
	return false;
}


/*!
 *  \brief Tells wheter the message is a response or not
 *
 *  Tells wheter the message is a response or not.
 */
bool SendChangesData::IsResponse()
{
	return false;
}


/*!
 *  \brief Returns the entity id
 *
 *  Returns the entity id.
 */
const cpw::TypeId &SendChangesData::GetEntityId()
{
	return entity_id;
}


/*!
 *  \brief Returns the list of changes
 *
 *  Returns the list of changes.
 */
const std::vector<cpw::Change> &SendChangesData::GetChanges()
{
	return changes;
}


/*!
 *  \brief Returns the message id
 *
 *  Returns the message id.
 *
 *  It is used to avoid processing the same message more than once.
 */
uint64_t SendChangesData::GetLastSynchro()
{
	return last_synchro;
}


/*!
 *  \brief Returns the message id
 *
 *  Returns the message id.
 *
 *  It is used to avoid processing the same message more than once.
 */
const cpw::TypeId &SendChangesData::GetMessageId()
{
	return message_id;
}


/*!
 *  \brief Returns the entity id
 *
 *  Returns the entity id.
 */
const cpw::TypeId &SendChangesData::GetEntityId() const
{
	return entity_id;
}


/*!
 *  \brief Returns the list of changes
 *
 *  Returns the list of changes.
 */
const std::vector<cpw::Change> &SendChangesData::GetChanges() const
{
	return changes;
}


/*!
 *  \brief Returns the timestamp of the last syncrhonization between the nodes
 *
 *  Returns the timestamp of the las syncrhonization between the nodes.
 */
uint64_t SendChangesData::GetLastSynchro() const
{
	return last_synchro;
}


/*!
 *  \brief Returns the message id
 *
 *  Returns the message id.
 *
 *  It is used to avoid processing the same message more than once.
 */
const cpw::TypeId &SendChangesData::GetMessageId() const
{
	return message_id;
}


/*!
 *  \brief Sets the entity id
 *  \param id Entity id
 *
 *  Sets the entity id.
 */
void SendChangesData::SetEntityId(const cpw::TypeId &id)
{
	entity_id = id;
}


/*!
 *  \brief Sets the list of changes to be sent
 *  \param changes The list of changes
 *
 *  Sets the list of changes to be sent.
 */
void SendChangesData::SetChanges(const std::vector<cpw::Change> &changes)
{
	this->changes = changes;
}


/*!
 *  \brief Returns the timestamp of the las syncrhonization between the nodes
 *
 *  Returns the timestamp of the las syncrhonization between the nodes.
 */
void SendChangesData::SetLastSynchro(uint64_t ts)
{
	last_synchro = ts;
}


/*!
 *  \brief Sets the message id
 *  \param id The id of the message
 *
 *  Sets the message id.
 *
 *  It is used to avoid processing the same message more than once.
 */
void SendChangesData::SetMessageId(const cpw::TypeId &id)
{
	message_id = id;
}
