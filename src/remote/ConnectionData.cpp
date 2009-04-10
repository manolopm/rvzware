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

#include <remote/ConnectionData.h>


using namespace cpw::remote;


ConnectionData::ConnectionData()
{
	SetMessageType(msgTypeConnection);
}


/*!
 *  \brief Returns a DataStream with all the information of the stream
 *
 *  Returns a DataStream with all the information of the stream, ready to be sent throught the network.
 *
 *  In this class there is nothing to do in this method as the message is not to be sent through the network.
 *  It is just a convenience class. It allways returns an empty DataStream.
 */
DataStream ConnectionData::Code()
{
	return DataStream();
}


/*!
 *  \brief Creates the message from a DataStream
 *  \param data The data to be used to compose the message
 *
 *  Creates the message from a DataStream.
 *
 *  In this class there is nothing to do in this method as the message is not to be sent through the network.
 *  It is just a convenience class. It allways returns true.
 */
bool ConnectionData::Decode(const DataStream &data)
{
	return true;
}


/*!
 *  \brief Tells wheter the message is a request or not
 *
 *  Tells wheter the message is a request or not.
 */
bool ConnectionData::IsRequest()
{
	return false;
}


/*!
 *  \brief Tells wheter the message is a response or not
 *
 *  Tells wheter the message is a response or not.
 */
bool ConnectionData::IsResponse()
{
	return false;
}

