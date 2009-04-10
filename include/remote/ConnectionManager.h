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
#ifndef _CONNECTIONMANAGER_
#define _CONNECTIONMANAGER_

#include <string>
#include <vector>
#include <map>

#include <cpw/common/Subject.h>
#include <cpw/common/RemoteNode.h>
#include <cpw/common/TypeId.h>
#include "RemoteExport.h"
#include "ISocket.h"
#include "ISocketFactory.h"
#include "Connection.h"


/*!
 *  \file ConnectionManager.h
 */


namespace cpw 
{ 
	namespace remote
	{
		class ISocketFactory;

		/*!
		 *  \class ConnectionManager ConnectionManager.h <remote/ConnectionManager.h>
		 *  \brief Connection register keeper
		 *  \ingroup remote
		 *
		 *  This class keeps the record of Connections active on the system.
		 */
		class REMOTE_EXPORT ConnectionManager: public cpw::Subject
		{
		public:
			ConnectionManager(ISocketFactory *factory);
			~ConnectionManager();

			std::vector<cpw::RemoteNode> GetNodes();
			Connection* GetConnection(const cpw::RemoteNode &node);
			ConnectionState GetConnectionState(const cpw::RemoteNode &node);
			std::vector<Connection *> GetSubscribers(const cpw::TypeId &id);
			std::vector<cpw::TypeId> GetSubscribed(const cpw::RemoteNode &node);

			bool SetListenPort(int port);

			Connection* Connect(const cpw::RemoteNode &node);
			void Disconnect(const cpw::RemoteNode &node);

			Connection *RecvNewConnection(const cpw::RemoteNode &node, ISocket *data);
			void RecvDisconnection(const cpw::RemoteNode &node);

			void Subscribe(const Connection *connection, cpw::TypeId &id);
			void Unsubscribe(const Connection *connection, cpw::TypeId &id);

			//void RecvMessage(const cpw::RemoteNode &node, const DataStream &data);
			//void SendChange(const Connection *connection, SetValueData *data);
			//void SynchronizeOthers(const Connection *connection, RequestSynchroData *data);
			//int GetNumSubscribers(const cpw::TypeId &id);

		private:
			int listenning_on_port;
			std::map<cpw::RemoteNode, Connection*> connections;
			std::map<cpw::RemoteNode, ConnectionState> connection_state;
			std::multimap<cpw::TypeId, const Connection*> shared_entities;

			ISocketFactory *factory;
		};

	}

}
#endif
