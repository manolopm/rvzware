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

#include <remote/DisconnectionData.h>


using namespace cpw::remote;



DisconnectionData::DisconnectionData()
{
	SetMessageType(msgTypeDisconnection);
}


/*!
 *  \brief Returns a DataStream with all the information of the stream
 *
 *  Returns a DataStream with all the information of the stream, ready to be sent throught the network.
 *
 *  In this class there is nothing to do in this method as the message is not to be sent through the network.
 *  It is just a convenience class. It allways returns an empty DataStream.
 */
DataStream DisconnectionData::Code()
{
	return DataStream();
}


/*!
 *  \brief Creates the message from a DataStream
 *  \param data_stream The data to be used to compose the message
 *
 *  Creates the message from a DataStream.
 *
 *  In this class there is nothing to do in this method as the message is not to be sent through the network.
 *  It is just a convenience class. It allways returns true.
 */
bool DisconnectionData::Decode(const DataStream &data_stream)
{
	return true;
}


/*!
 *  \brief Tells wheter the message is a request or not
 *
 *  Tells wheter the message is a request or not.
 */
bool DisconnectionData::IsRequest()
{
	return false;
}


/*!
 *  \brief Tells wheter the message is a response or not
 *
 *  Tells wheter the message is a response or not.
 */
bool DisconnectionData::IsResponse()
{
	return false;
}

