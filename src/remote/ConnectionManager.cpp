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
#include <sstream>
#include <vector>
#include <map>

#include <remote/stdafx.h>

#include <cpw/common/Subject.h>
#include <cpw/common/Remote.h>
#include <cpw/common/TypeId.h>
#include <cpw/entity/EntityRegistry.h>
#include <cpw/common/RemoteNode.h>

#include <remote/ISocket.h>
#include <remote/ISocketFactory.h>
#include <remote/Connection.h>
#include <remote/RequestSynchroData.h>
#include <remote/ConnectionData.h>
#include <remote/DisconnectionData.h>
#include <remote/ConnectionManager.h>



using namespace cpw::remote;


/*!
 *  \param factory The socket factory to be used
 */
ConnectionManager::ConnectionManager(ISocketFactory *factory)
	: factory(factory) {}


ConnectionManager::~ConnectionManager()
{
	std::map<cpw::RemoteNode, Connection* >::iterator it1;
	for(it1 = connections.begin();it1 !=  connections.end(); it1++)
		delete it1->second;
}


/*!
 *  \brief Returns a vector with all the connection identifiers registered
 *
 *  Returns a vector with all the connection identifiers registered.
 */
std::vector<cpw::RemoteNode> ConnectionManager::GetNodes()
{
	std::vector<cpw::RemoteNode> nodes;

	std::map<cpw::RemoteNode, Connection*>::iterator it;
	for (it = connections.begin(); it != connections.end(); it++)
		nodes.push_back(it->first);

	return nodes;
}


/*!
 *  \brief Returns the connection corresponding to the parameter
 *  \param node The identifier of the connection requested
 *
 *  Returns NULL in case there is no connection registered with that identifier
 */
Connection* ConnectionManager::GetConnection(const cpw::RemoteNode &node)
{
	if (connections.find(node) != connections.end())
		return connections[node];
	else
		return NULL;
}


/*!
 *  \brief Returns the state of the connection
 *  \param node The identifier of the connection
 *
 *  Returns cstDisconnected in case the connection is not registered
 */
ConnectionState ConnectionManager::GetConnectionState(const cpw::RemoteNode &node)
{
	if (connection_state.find(node) != connection_state.end())
		return connection_state[node];
	else
		return cstDisconnected;
}


/*!
 *  \brief Returns a vector containing the connections associated with an entity
 *  \param id The id of the Entity
 */
std::vector<Connection *> ConnectionManager::GetSubscribers(const cpw::TypeId &id)
{
	std::vector<Connection *> subscribers;
	std::pair<std::multimap<cpw::TypeId, const Connection*>::iterator,
	std::multimap<cpw::TypeId, const Connection*>::iterator> range;

	range = shared_entities.equal_range(id);
	
	std::multimap<cpw::TypeId, const Connection*>::iterator it;
	for (it = range.first; it!=range.second; it++)
		subscribers.push_back(((Connection*)it->second));

	return subscribers;
}


/*!
 *  \brief Returns a vector with all the entities to which a connection is associated
 *  \param node The identifier of the connection
 */
std::vector<cpw::TypeId> ConnectionManager::GetSubscribed(const cpw::RemoteNode &node)
{
	std::vector<cpw::TypeId> entities;

	std::multimap<cpw::TypeId, const Connection*>::iterator it;

	for (it = shared_entities.begin(); it != shared_entities.end(); it++)
	{
		if (it->second->GetNode() == node)
			entities.push_back(it->first);
	}

	return entities;
}


/*!
 *  \brief Sets the service port on which the system will be expecting new connection
 *  \param port The port that should be used
 *
 *  This method calls the SocketFactory to start listenning.
 *
 *  If the SocketFactory could open the specified port, returns true. It returns false otherwise.
 */
bool ConnectionManager::SetListenPort(int port)
{
	bool isSet = factory->ListenOn(port);
	listenning_on_port = isSet?port:-1;
	return isSet;
}


/*!
 *  \brief Establish a connection with the node specified, returning the Connection object
 *  \param node The identifier of the connection
 *
 *  This method avoids connecting to self, so it is safe to use it without further checking on the user data.
 *  It also checks whether the connection is already registered. In that case, it returns the existing
 *  connection instead of creating a new one.
 *
 *  The method returns NULL when it tries to connect to self.
 *
 *  The connection is not a secuential process, so the link won't be up inmediately, but the Connection object
 *  will be fully operative. The connection state will be set to cstConnecting. There will be a NewConnection
 *  event when the link is finally up, resetting the state for this connection.
 */
Connection* ConnectionManager::Connect(const cpw::RemoteNode &node)
{
	//Checking 127.0.0.1 and localhost should be enough for now, but it would be 
	//better to perform a better check
	if ((node.GetHostname() == "127.0.0.1" || node.GetHostname() == "localhost") 
		&& node.GetPort() == listenning_on_port)
		return NULL;

	Connection *connection;

	std::map<cpw::RemoteNode, Connection*>::iterator iter_connection;
	iter_connection = connections.find(node);
	if (iter_connection != connections.end())
		connection = connections[node];
	else
	{
		//Create the connection
		ISocket *socket = factory->CreateSocket();

		connection = new Connection(socket, node);

		connection->Connect();
		connections[node] = connection;
		connection_state[node] = cstConnecting;

		Notify();
	}


	return connection;
}


/*!
 *  \brief Disconnects from the specified node
 *  \param node The identifier of the connection
 *
 *  The disconnection is not inmediate, so a new Disconnection event will be received when the link is finally down.
 *
 *  This method sets the state of the connection to cstDisconnecting.
 */
void ConnectionManager::Disconnect(const cpw::RemoteNode &node)
{
	connections[node]->Disconnect();
	connection_state[node] = cstDisconnecting;
}


/*!
 *  \brief Method receiving the events of new connections
 *  \param node The identifier of the connection
 *  \param socket The socket object to be used
 *
 *  This method should be called whenever a new connection is detected, or when a connection request
 *  issued on the very same ConnectionManager is really up.
 *
 *  Returns NULL when the connection already existed, or the new Connection object when it wasn't registered.
 */
Connection * ConnectionManager::RecvNewConnection(const cpw::RemoteNode &node, ISocket *socket)
{
	if (connections.find(node) == connections.end())
	{
		//1 - Create Connection
		Connection *connection = new Connection(socket, node);

		//2 - Associate connection with address
		//2.1 - Add parameter cpw::RemoteNode
		connections[node] = connection;
		connection_state[node] = cstConnected;

		Notify();

		return connection;
	}
	else
	{
		connection_state[node] = cstConnected;

		Notify();

		return NULL;
	}
}


/*!
 *  \brief Method receiving the Disconnection event
 *  \param node The identifier of the connection
 *
 *  This method deletes the corresponding Connection object. It also deletes the associations with
 *  entities that where created.
 */
void ConnectionManager::RecvDisconnection(const cpw::RemoteNode &node)
{
	if (connections.find(node) != connections.end())
	{
		Connection *connection = connections[node];

		//Unsubscribe (kind of)
		std::multimap<cpw::TypeId, const Connection*>::iterator it = shared_entities.begin();
		while (it != shared_entities.end())
		{
			std::multimap<cpw::TypeId, const Connection*>::iterator itaux = it;
			itaux++;
			if (it->second == connection)
				shared_entities.erase(it);
			it = itaux;
		}

		delete connection;
		connections.erase(connections.find(node));
		connection_state.erase(connection_state.find(node));

		Notify();
	}
}


/*!
 *  \brief Associates a Connection with an entity
 *  \param connection The connection
 *  \param id The entity's id
 *
 *  This method should be called whenever an entity is downloaded from a remote node. This way
 *  new changes to the entity will be received and sent
 */
void ConnectionManager::Subscribe(const Connection *connection, cpw::TypeId &id)
{
	std::multimap<cpw::TypeId, const Connection*>::iterator it;
	for (it = shared_entities.begin(); it != shared_entities.end(); it++)
	{
		if (it->first == id && it->second == connection)
			break;
	}

	if (it == shared_entities.end())
		shared_entities.insert(std::pair<cpw::TypeId, const Connection*>(id, connection));

	Notify();
}


/*!
 *  \brief Deletes the association between a Connection and an entity
 *  \param connection The connection
 *  \param id The entity's id
 *
 *  This method should be called whenever receiving or sending changes to a remote node is no longer
 *  required.
 *
 *  It is not necessary when disconnecting from a node. Use ConnectionManager::Disconnect instead.
 */
void ConnectionManager::Unsubscribe(const Connection *connection, cpw::TypeId &id)
{
	std::multimap<cpw::TypeId, const Connection*>::iterator it;
	for (it = shared_entities.begin(); it != shared_entities.end(); it++)
	{
		if (it->first == id && it->second == connection)
			break;
	}

	if (it != shared_entities.end())
		shared_entities.erase(it);

	Notify();
}
