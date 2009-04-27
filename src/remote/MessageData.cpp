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

#include <remote/stdafx.h>

#include <remote/MessageData.h>


using namespace cpw::remote;



/*!
 *  \brief Returns the message's type
 *
 *  Returns the message's type.
 */
const MessageType &MessageData::GetMessageType()
{
	return message_type;
}


/*!
 *  \brief Returns the request id of the message
 *
 *  Returns the request id of the message.
 */
const uint8_t &MessageData::GetRequestID()
{
	return request_id;
}


/*!
 *  \brief Returns the message's type
 *
 *  Returns the message's type.
 */
const MessageType &MessageData::GetMessageType() const
{
	return message_type;
}


/*!
 *  \brief Returns the request id of the message
 *
 *  Returns the request id of the message.
 */
const uint8_t &MessageData::GetRequestID() const
{
	return request_id;
}


/*!
 *  \brief Sets the message's type
 *  \param id The type of the message
 *
 *  Sets the message's type.
 */
void MessageData::SetMessageType(MessageType id)
{
	message_type = id;
}


/*!
 *  \brief Sets the request id of the message
 *  \param id The request id
 *
 *  Sets the request id of the message.
 */
void MessageData::SetRequestID(uint8_t id)
{
	request_id = id;
}
