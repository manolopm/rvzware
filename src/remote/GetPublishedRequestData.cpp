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

#include <remote/GetPublishedRequestData.h>
#include <remote/DataStream.h>

using namespace cpw::remote;


/*!
 *  Sets the corresponding MessageType
 */
GetPublishedRequestData::GetPublishedRequestData()
{
	SetMessageType(msgTypeGetPublishedRequest);
}


/*!
 *  \brief Returns a DataStream with all the information of the stream
 *
 *  Returns a DataStream with all the information of the stream, ready to be sent throught the network.
 *
 *  Returns an empty DataStream in case there is an error.
 */
DataStream GetPublishedRequestData::Code()
{
	try
	{
		DataStream data(6);

		data << (uint8_t) message_type;
		data << (uint32_t) 6;
		data << request_id;

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
bool GetPublishedRequestData::Decode(const DataStream &data_stream)
{
	try
	{
		DataStream data(data_stream);

		//Get the control data
		uint8_t tmp8;
		data >> tmp8;
		message_type = (MessageType) tmp8;

		uint32_t msg_size;
		data >> msg_size;

		if (data.GetSize() != msg_size)
			return false;

		data >> tmp8;
		request_id = tmp8;

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
bool GetPublishedRequestData::IsRequest()
{
	return true;
}


/*!
 *  \brief Tells wheter the message is a response or not
 *
 *  Tells wheter the message is a response or not.
 */
bool GetPublishedRequestData::IsResponse()
{
	return false;
}

