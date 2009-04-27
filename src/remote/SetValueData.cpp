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

#include <remote/DataStream.h>
#include <remote/SetValueData.h>
#include <cpw/common/TypeId.h>
#include <cpw/common/pstdint.h>

using namespace cpw::remote;


/*!
 *  Sets the corresponding MessageType
 */
SetValueData::SetValueData()
{
	SetMessageType(msgTypeSetValue);
}


/*!
 *  \brief Returns a DataStream with all the information of the stream
 *
 *  Returns a DataStream with all the information of the stream, ready to be sent throught the network.
 *
 *  Returns an empty DataStream in case there is an error.
 */
DataStream SetValueData::Code()
{
	try
	{
		uint32_t msg_size;

		msg_size = 5 + cpw::TypeId::size() + cpw::TypeId::size() + 4 + field.size() + 4 + value.size() + 8;

		DataStream data(msg_size);

		//Introduce the control data
		data <<(uint8_t) message_type;
		data << msg_size;

		data << message_id;


		//Then the ID of the entity
		data << entity_id;

		//Field with its size
		data << field;

		//Value with its size
		data << value;

		data << ts;

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
bool SetValueData::Decode(const DataStream &data_stream)
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

		//Then the ID
		data >> entity_id;
		
		//Field with its size
		data >> field;

		//Value with its size
		data >> value;
		
		data >> ts;

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
bool SetValueData::IsRequest()
{
	return false;
}


/*!
 *  \brief Tells wheter the message is a response or not
 *
 *  Tells wheter the message is a response or not.
 */
bool SetValueData::IsResponse()
{
	return false;
}


/*!
 *  \brief Returns the field name
 *
 *  Returns the field name.
 */
const std::string &SetValueData::GetField()
{
	return field;
}


/*!
 *  \brief Returns the value of the field coded into a string
 *
 *  Returns the value of the field coded into a string.
 */
const std::string &SetValueData::GetValue()
{
	return value;
}


/*!
 *  \brief Returns the timestamp of the change
 *
 *  Returns the timestamp of the change.
 */
uint64_t SetValueData::GetTimeStamp()
{
	return ts;
}


/*!
 *  \brief Returns the entity id
 *
 *  Returns the entity id.
 */
const cpw::TypeId &SetValueData::GetEntityId()
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
const cpw::TypeId &SetValueData::GetMessageId()
{
	return message_id;
}


/*!
 *  \brief Returns the field name
 *
 *  Returns the field name.
 */
const std::string &SetValueData::GetField() const
{
	return field;
}


/*!
 *  \brief Returns the value of the field coded into a string
 *
 *  Returns the value of the field coded into a string.
 */
const std::string &SetValueData::GetValue() const
{
	return value;
}


/*!
 *  \brief Returns the timestamp of the change
 *
 *  Returns the timestamp of the change.
 */
uint64_t SetValueData::GetTimeStamp() const
{
	return ts;
}


/*!
 *  \brief Returns the entity id
 *
 *  Returns the entity id.
 */
const cpw::TypeId &SetValueData::GetEntityId() const
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
const cpw::TypeId &SetValueData::GetMessageId() const
{
	return message_id;
}


/*!
 *  \brief Sets the name of the field
 *  \param field The string containing the name
 *
 *  Sets the name of the field.
 */
void SetValueData::SetField(const std::string &field)
{
	this->field = field;
}


/*!
 *  \brief Sets the value of the field from a string
 *  \param value The string with the value
 *
 *  Sets the value of the field coded into a string.
 */
void SetValueData::SetValue(const std::string &value)
{
	this->value = value;
}


/*!
 *  \brief Sets the timestamp of the change
 *  \param ts The timestamp
 *
 *  Sets the timestamp of the change.
 */
void SetValueData::SetTimeStamp(const uint64_t ts)
{
	this->ts = ts;
}


/*!
 *  \brief Sets the entity id
 *  \param id Entity id
 *
 *  Sets the entity id.
 */
void SetValueData::SetEntityId(const cpw::TypeId &id)
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
void SetValueData::SetMessageId(const cpw::TypeId &id)
{
	message_id = id;
}
