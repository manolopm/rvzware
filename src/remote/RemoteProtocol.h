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

#ifndef _REMOTEPROTOCOL_
#define _REMOTEPROTOCOL_

#include <string>
#include <vector>
#include <map>
#include <list>

#include <cpw/common/TypeId.h>
#include <cpw/common/RemoteNode.h>
#include <cpw/entity/LayerTree.h>

#include <remote/RemoteExport.h>
#include <remote/IRemoteCallback.h>
#include <remote/MessageData.h>
#include <remote/ConnectionData.h>
#include <remote/DisconnectionData.h>
#include <remote/GetPublishedRequestData.h>
#include <remote/GetPublishedResponseData.h>
#include <remote/GetEntityRequestData.h>
#include <remote/GetEntityResponseData.h>
#include <remote/RequestSynchroData.h>
#include <remote/SendChangesData.h>
#include <remote/SetValueData.h>
#include <remote/DisconnectEntityData.h>
#include <remote/Connection.h>
#include <remote/ISocketFactory.h>
#include <remote/ConnectionManager.h>
#include <remote/RemoteModifier.h>

/*!
 *	\namespace cpw { namespace remote \brief Remote core package
 *  \defgroup remote Remote core package
 */

/*!
 *  \file RemoteProtocol.h
 */


namespace cpw 
{ 
	namespace remote
	{
		class Connection;
		class ISocketFactory;
		class ConnectionManager;

		/*!
		 *  \class RemoteProtocol RemoteProtocol.h <remote/RemoteProtocol.h>
		 *  \ingroup remote
		 *  \brief Implements the network protocol
		 *
		 *
		 *  This is the main class in the remote package. It implements the protocol defined in remote
		 *  by providing the mecanisms to create and delete all the needed objects, and methods to
		 *  process all the messages received.
		 *
		 *  It also serves as an interface to applications using the remote component in Capaware.
		 *
		 *  Use the methods with the prefix Recv whenever a network event happens.
		 *
		 */
		class REMOTE_EXPORT RemoteProtocol
		{
		public:
			RemoteProtocol(int port, cpw::LayerTree *layer_tree, ISocketFactory *factory);
			~RemoteProtocol();

			remote::ConnectionManager *GetConnectionManager();

			void SetCallback(MessageType type, IRemoteCallback *callback);

			Connection* Connect(std::string hostname, int port);
			void GetPublished(Connection *connection);
			void GetEntities(Connection *connection, std::vector<cpw::TypeId> &ids);
			void SendChange(cpw::TypeId entity_id, struct cpw::Change change);

			void EntityRemoved(cpw::TypeId entity_id);

			void ConnectToAllEntities(cpw::Entity *entity = NULL);
			void DisconnectAllEntities(cpw::Entity *entity = NULL);

			void RecvNewConnection(const cpw::RemoteNode &node, ISocket *data = NULL);
			void RecvDisconnection(const cpw::RemoteNode &node);
			void RecvMessage(const cpw::RemoteNode &node, const DataStream &data);

			void CleanUpModifiers();


		private:

			void RecvMessage(Connection *connection, GetPublishedRequestData *message_data);
			void RecvMessage(Connection *connection, GetPublishedResponseData *message_data);
			void RecvMessage(Connection *connection, GetEntityRequestData *message_data);
			void RecvMessage(Connection *connection, GetEntityResponseData *message_data);
			void RecvMessage(Connection *connection, RequestSynchroData *message_data);
			void RecvMessage(Connection *connection, SendChangesData *message_data);
			void RecvMessage(Connection *connection, SetValueData *message_data);
			void RecvMessage(Connection *connection, DisconnectEntityData *message_data);

			void AddEntity(remote::Connection * connection, const cpw::TypeId &entity_id,
				const cpw::TypeId &child_id);
			void DeleteEntity(remote::Connection * connection, const cpw::TypeId &entity_id,
				const cpw::TypeId &child_id);
			void DeleteEntity_helper(Connection *connection, cpw::Entity *entity);


			cpw::LayerTree *layer_tree;
			ConnectionManager *connection_manager;

			std::map<MessageType, IRemoteCallback *> callbacks;

			std::map<cpw::TypeId, RemoteModifier*> modifiers;
			std::map<std::pair<cpw::TypeId,cpw::RemoteNode>, bool> pending_synchs;
			std::multimap<cpw::TypeId, cpw::TypeId> child_parent; //1:child 2:parent
			std::list<cpw::TypeId> processed_msg;
		};

	}

}
#endif
