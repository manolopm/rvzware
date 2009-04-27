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
#ifndef _CONNECTION_
#define _CONNECTION_

#include <string>
#include <map>

#include <cpw/common/pstdint.h>
#include <cpw/common/Loggable.h>
#include <cpw/common/RemoteNode.h>

#include <remote/RemoteExport.h>
#include <remote/MessageData.h>
#include <remote/ISocket.h>

/*!
 *  \file Connection.h
 */

namespace cpw 
{ 
	namespace remote
	{
		/*!
		 *  \ingroup remote
		 *
		 *  Defines the possible states of a connection
		 */
		enum REMOTE_EXPORT ConnectionState
		{
			cstConnected = 1, /*!< The connection is established */
			cstDisconnected = 2, /*!< The connection is down */
			cstConnecting = 3, /*!< The connection is trying to be established */
			cstDisconnecting = 4, /*!< The connection is currently trying to disconnect */
			cstUnknown = 5 /*!< Unknow state */
		};


		/*!
		 *  \class Connection Connection.h <remote/Connection.h>
		 *  \brief A connection
		 *  \ingroup remote
		 *
		 *  This class represents a connection to a remote node in a high abstraction level.
		 *  It is library agnostic, meaning that is independent from the concrete socket library or
		 *  communication technology used.
		 */
		class REMOTE_EXPORT Connection
		{
		public:
			Connection(ISocket *socket, cpw::RemoteNode node = cpw::RemoteNode());
			~Connection();

			cpw::RemoteNode GetNode();
			cpw::RemoteNode GetNode() const;

			void Connect();
			void Disconnect();

			MessageData *RecvMessage(const DataStream &data);

			void Send(MessageData *data);


		private:
			MessageData *GetMessageData(MessageType message_type);
			//bool ProcessMessage(MessageData *msg_data);

			cpw::RemoteNode node;
			
			ISocket *socket;
			uint8_t last_request;

			std::map<cpw::TypeId, int> state;
		};

	}

}
#endif

