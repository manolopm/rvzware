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
#ifndef _ISOCKET_
#define _ISOCKET_

#include <string>

#include <cpw/common/RemoteNode.h>
#include <remote/RemoteExport.h>
#include <remote/DataStream.h>

/*!
 *  \file ISocket.h
 */

namespace cpw 
{ 
	namespace remote
	{
		/*!
		 *  \interface ISocket ISocket.h <remote/ISocket.h>
		 *  \ingroup remote
		 *  \brief Socket interface
		 *
		 *  Socket interface.
		 *
		 *  This abstract class represents the interface of any Socket implementation
		 */
		class REMOTE_EXPORT ISocket
		{
		public:
			virtual ~ISocket() {}

			/*!
			 *  \brief Sends a stream to the remote node
			 *  \param data The data to be sent
			 *
			 *  Sends a stream to the remote node.
			 */
			virtual void Send(const DataStream &data) = 0;

			/*!
			 *  \brief Connects to the specified remote node
			 *  \param node The identifier of the remote node
			 *
			 *  Connects to the specified remote node.
			 */
			virtual void ConnectTo(const cpw::RemoteNode &node) = 0;

			/*!
			 *  \brief Disconnects from the remote node
			 *
			 *  Disconnects from the remote node.
			 */
			virtual void Disconnect() = 0;
		};

	}

}
#endif

