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
#include <vector>
#include <map>
#include <list>
#include <algorithm>

#include <remote/stdafx.h>

#include <cpw/ApplicationConfiguration.h>
#include <cpw/common/IdGenerator.h>
#include <cpw/common/TypeId.h>
#include <cpw/common/Composite.h>
#include <cpw/persistent/file/XmlCpwParser.h>
#include <cpw/entity/LayerTree.h>
#include <cpw/entity/EntityRegistry.h>
#include <cpw/entity/EntityFactory.h>
#include <cpw/entity/ContainerLayer.h>

#include <remote/Connection.h>
#include <remote/IRemoteCallback.h>
#include <remote/PublishedTree.h>
#include <remote/GetPublishedRequestData.h>
#include <remote/GetPublishedResponseData.h>
#include <remote/GetEntityRequestData.h>
#include <remote/GetEntityResponseData.h>
#include <remote/RequestSynchroData.h>
#include <remote/SendChangesData.h>
#include <remote/SetValueData.h>
#include <remote/RemoteProtocol.h>

using namespace cpw::remote;

//Little trick. It is used to ignore the changes on fields relating to
//binary files, when processing a SetValue message
static std::vector<std::string> ignore_fields;
void SetIgnoreFields()
{
	ignore_fields.push_back("icon");
	ignore_fields.push_back("FireTexture");
	ignore_fields.push_back("SmokeTexture");
	ignore_fields.push_back("IgnitionPointModel");
	ignore_fields.push_back("primitive_url");
	ignore_fields.push_back("model_url");
	ignore_fields.push_back("");
}

/*!
 *  \param port Port to be used as listen port
 *  \param layer_tree Pointer to the main LayerTree of the application
 *  \param factory The socket factory to be used
 */
RemoteProtocol::RemoteProtocol(int port, cpw::LayerTree *layer_tree, ISocketFactory *factory) :
	layer_tree(layer_tree)
{
	connection_manager = new ConnectionManager(factory);

	factory->CreateHelperClasses(this);

	connection_manager->SetListenPort(port);

	SetIgnoreFields();
}


RemoteProtocol::~RemoteProtocol()
{
	delete connection_manager;
	for (std::map<cpw::TypeId, RemoteModifier*>::iterator it=modifiers.begin(); it != modifiers.end(); it++)
		delete it->second;
}


/*!
 *  \brief Returns the Connection Manager
 *
 *  Returns the Connection Manager.
 */
ConnectionManager *RemoteProtocol::GetConnectionManager()
{
	return connection_manager;
}


/*!
 *  \brief Sets a callback function to call when a message of the specified type is received
 *  \param type Type of message
 *  \param callback The callback
 *
 *  Sets a callback function to call when a message of the specified type is received.
 */
void RemoteProtocol::SetCallback(MessageType type, IRemoteCallback *callback)
{
	callbacks[type] = callback;
}


/*!
 *  \brief Creates a new connection to the hostname and port specified
 *  \param hostname Hostname or IP of the node to connect to
 *  \param port TCP port of the node to connect to
 *
 *  Creates a new connection to the hostname and port specified.
 *
 *  If the connection already exists the method returns it.
 */
Connection* RemoteProtocol::Connect(std::string hostname, int port)
{
	return connection_manager->Connect(cpw::RemoteNode(hostname, port));
}


/*!
 *  \brief Sends the GetPublished message to the specified connection
 *
 *   Sends the GetPublished message to the specified connection.
 */
void RemoteProtocol::GetPublished(Connection *connection)
{
	GetPublishedRequestData *message = new GetPublishedRequestData();

	connection->Send(message);

	delete message;
}


/*!
 *  \brief Sends the required messages to get the entities specified on the second parameter
 *  \param connection Connection through which the system will make the request
 *  \param ids A vector with the IDs of the entities being requested
 *
 *  Sends the required messages to get the entities specified on the second parameter.
 */
void RemoteProtocol::GetEntities(Connection *connection, std::vector<cpw::TypeId> &ids)
{
	std::vector<cpw::TypeId>::iterator it;
	for (it = ids.begin(); it != ids.end(); it++)
	{
		GetEntityRequestData *message = new GetEntityRequestData();

		cpw::Entity *eaux = cpw::EntityRegistry::GetInstance()->GetEntity(*it);

		if (eaux != NULL) 
		{
			message->SetEntityId(*it);
			message->SetSendEntity(false);
		}
		else
		{
			message->SetEntityId(*it);
			message->SetSendEntity(true);
		}

		connection->Send(message);

		delete message;
	}
}


/*!
 *  \brief Sends the change specified to all the connections available
 *  \param entity_id ID of the Entity related
 *  \param change Change to send to other nodes
 *
 *  Sends the change specified to all the connections available.
 */
void RemoteProtocol::SendChange(cpw::TypeId entity_id, struct cpw::Change change)
{
	if (change.field == "visible")
		return;
	//if (change.field == "DeleteChild")

	cpw::IdGenerator id_generator;
	cpw::TypeId msg_id = id_generator.GetNewId();

	if (processed_msg.size() >= 100)
		processed_msg.pop_front();

	processed_msg.push_back(msg_id);


	SetValueData msg_data;
	cpw::Remote *entity = cpw::EntityRegistry::GetInstance()->GetEntity(entity_id);

	msg_data.SetEntityId(entity_id);
	msg_data.SetMessageId(msg_id);
	msg_data.SetField(change.field);
	msg_data.SetValue(change.value);
	msg_data.SetTimeStamp(change.time_stamp);

	std::vector<Connection*> subscribers = connection_manager->GetSubscribers(entity_id);
	std::vector<Connection*>::iterator it;
	for (it=subscribers.begin(); it!= subscribers.end();it++)
	{
		(*it)->Send(&msg_data);
		entity->SetLastSynch((*it)->GetNode(), change.time_stamp);
	}

	if (it == subscribers.begin())
	{
		RemoteModifier *rmod = modifiers[entity_id];
		modifiers.erase(modifiers.find(entity_id));
		delete rmod;
	}
}


/*!
 *  \brief Method used when an entity is deleted and the remotemodifier is not longer necesary
 *  \param entity_id ID of the entity that has been removed
 *
 *  Method used when an entity is deleted and the remotemodifier is not longer necesary.
 */
void RemoteProtocol::EntityRemoved(cpw::TypeId entity_id)
{
	std::map<cpw::TypeId, RemoteModifier*>::iterator it_mod = modifiers.find(entity_id);
	if (it_mod != modifiers.end())
	{
		/*RemoteModifier *modifier = it_mod->second;
		modifiers.erase(it_mod);
		delete modifier;*/

		std::vector<Connection *> connections = connection_manager->GetSubscribers(entity_id);

		std::vector<Connection *>::iterator it_con;
		for (it_con = connections.begin(); it_con != connections.end(); it_con++)
		{
			cpw::remote::DisconnectEntityData *message = new cpw::remote::DisconnectEntityData();
			message->SetEntityId(entity_id);

			(*it_con)->Send(message);

			delete message;

			connection_manager->Unsubscribe(*it_con, entity_id);
		}
	}
}


/*!
 *  \brief Connects and synchronize every entity, as specified in cpw::Remote
 *  \param entity The root entity from which start connecting. Default value (NULL) makes it use the Top Layer
 *
 *  Connects and synchronize every entity, as specified in cpw::Remote.
 */
void RemoteProtocol::ConnectToAllEntities(cpw::Entity *entity)
{
	if (entity == NULL)
		entity = layer_tree->GetTopLayer();

	if (entity != NULL)
	{
		std::vector<cpw::RemoteNode> nodes = ((cpw::Remote*) entity)->GetNodes();

		std::vector<cpw::RemoteNode>::iterator it;
		for (it = nodes.begin(); it != nodes.end(); it++)
		{
			cpw::RemoteNode n(it->GetIPAddress(), 3000);
			Connection *connection = connection_manager->Connect(n);

			if (connection != NULL)
			{
				GetEntityRequestData *message = new GetEntityRequestData();

				message->SetEntityId(entity->GetId());
				message->SetSendEntity(false);

				connection->Send(message);

				delete message;
			}
		}

		if (entity->isContainer())
		{
			//create children
			for (int i=0; i<entity->GetNumChildren(); i++)
			{
				cpw::Entity *child=entity->GetChild(i);
				ConnectToAllEntities(child);
			}
		}
	}
}


/*!
 *  \brief Disconnects an entity and all its children, so none of them produces SetValue messages any more
 *  \param entity The root entity from which start disconnecting. Default value (NULL) makes it use the Top Layer
 *
 *  Disconnects an entity and all its children, so none of them produces SetValue messages any more
 */
void RemoteProtocol::DisconnectAllEntities(cpw::Entity *entity)
{
	if (entity == NULL)
		entity = layer_tree->GetTopLayer();

	if (entity != NULL)
	{
		std::vector<Connection *> subscribers;
		subscribers = connection_manager->GetSubscribers(entity->GetId());
		
		std::vector<Connection *>::iterator it;
		for (it = subscribers.begin(); it != subscribers.end(); it++)
		{
			cpw::remote::DisconnectEntityData *message = new cpw::remote::DisconnectEntityData();
			message->SetEntityId(entity->GetId());

			(*it)->Send(message);

			delete message;

			connection_manager->Unsubscribe(*it, entity->GetId());
		}

		if (entity->isContainer())
		{
			//create children
			for (int i=0; i<entity->GetNumChildren(); i++)
			{
				cpw::Entity *child=entity->GetChild(i);
				DisconnectAllEntities(child);
			}
		}
	}

	//Delete the RemoteModifier associated
	std::map<cpw::TypeId, RemoteModifier*>::iterator itaux = modifiers.find(entity->GetId());
	if (itaux != modifiers.end())
	{
		delete itaux->second;
		modifiers.erase(itaux);
	}
}

/*!
 *  \brief Method to call when a new connection is received
 *  \param node Connection address of the remote node
 *  \param data Socket object for the new connection
 *
 *  Method to call when a new connection is received.
 */
void RemoteProtocol::RecvNewConnection(const cpw::RemoteNode &node, ISocket *data)
{
	Connection *connection = connection_manager->RecvNewConnection(node, data);
	if (connection != NULL)
	{
		if (callbacks.find(msgTypeConnection) != callbacks.end())
			(*callbacks[msgTypeConnection])(connection, &ConnectionData()); //to show message
	}
}


/*!
 *  \brief Method to call when a disconnection is detected
 *  \param node Connection address of the remote node
 *
 *  Method to call when a disconnection is detected.
 */
void RemoteProtocol::RecvDisconnection(const cpw::RemoteNode &node)
{
	Connection *connection = connection_manager->GetConnection(node);
	if (callbacks.find(msgTypeDisconnection) != callbacks.end())
		(*callbacks[msgTypeDisconnection])(connection, &DisconnectionData());

	connection_manager->RecvDisconnection(node);

	CleanUpModifiers();
}


/*!
 *  \brief Method to call when a message is received
 *  \param node Connection address of the remote node
 *  \param data A DataStream containing a message
 *
 *  Method to call when a message is received.
 */
void RemoteProtocol::RecvMessage(const cpw::RemoteNode &node, const DataStream &data)
{
	Connection *connection = connection_manager->GetConnection(node);
	if (connection == NULL)
		return;

	MessageData *msg_data = connection->RecvMessage(data);
	
	if (msg_data == NULL)
		return;

	switch(msg_data->GetMessageType())
	{
		case msgTypeGetPublishedRequest:
			RecvMessage(connection, (GetPublishedRequestData*) msg_data);
			break;
		case msgTypeGetPublishedResponse:
			RecvMessage(connection, (GetPublishedResponseData*) msg_data);
			break;
		case msgTypeGetEntityRequest:
			RecvMessage(connection, (GetEntityRequestData*) msg_data);
			break;
		case msgTypeGetEntityResponse:
			RecvMessage(connection, (GetEntityResponseData*) msg_data);
			break;
		case msgTypeSendChanges:
			RecvMessage(connection, (SendChangesData*) msg_data);
			break;
		case msgTypeSetValue:
			RecvMessage(connection, (SetValueData*) msg_data);
			break;
		case msgTypeRequestSynchro:
			RecvMessage(connection, (RequestSynchroData*) msg_data);
			break;
		case msgTypeDisconnectEntity:
			RecvMessage(connection, (DisconnectEntityData*) msg_data);
			break;			
		default:
			break;
	}

	if (callbacks.find(msg_data->GetMessageType()) != callbacks.end())
		(*callbacks[msg_data->GetMessageType()])(connection, msg_data);

	delete msg_data;
}


/*!
 *  \brief Helper method meant to deal with recursion
 *  \param entity In parameter to contain the entity to be transformed into PublishedNode
 *  \param root Out parameter to contain the translation of the entity into the PublishedNode structure
 *
 *  Helper method meant to deal with recursion.
 */
void GetPublishedRequestCallback_helper(cpw::Entity *entity, PublishedFolder *root)
{
	//Create tree
	PublishedNode *node;

	if (!entity->isContainer())
		node = new PublishedLeaf(entity->GetName(), entity->GetId());
	else
	{
		node = new PublishedFolder(entity->GetName(), entity->GetId());

		//create children
		for (int i=0; i<entity->GetNumChildren(); i++)
		{
			cpw::Entity *child=entity->GetChild(i);
			GetPublishedRequestCallback_helper(child, (PublishedFolder*)node);
		}
	}

	root->Add(node);
}


/*!
 *  \brief Method processing the message GetPublishedRequestData
 *  \param connection The connection from which the message was recieved
 *  \param message_data The message
 *
 *  Method processing the message GetPublishedRequestData.
 *
 *  This method is called internally by RecvMessage
 */
void RemoteProtocol::RecvMessage(Connection *connection, GetPublishedRequestData *message_data)
{
	GetPublishedResponseData *data = new GetPublishedResponseData();

	PublishedFolder *root_node = new PublishedFolder("Published Entities", cpw::TypeId());

	cpw::Entity *entity = layer_tree->GetTopLayer();

	std::vector<cpw::Entity *> to_explore;
	if (entity->isPublished())
	{
		GetPublishedRequestCallback_helper(entity, root_node);
	}
	else
	{
		for (int j=0; j<entity->GetNumChildren(); j++)
			to_explore.push_back(entity->GetChild(j));
	}

	std::vector<cpw::Entity *>::iterator i = to_explore.begin();
	while (i!=to_explore.end())
	{
		cpw::Entity *entity = *i;
		to_explore.erase(i);

		if (entity->isPublished())
			GetPublishedRequestCallback_helper(entity, root_node);
		else
		{
			for (int j=0; j<entity->GetNumChildren(); j++)
				to_explore.push_back(entity->GetChild(j));
		}

		i = to_explore.begin();
	}

	data->SetEntities(root_node);
	data->SetRequestID(message_data->GetRequestID());

	connection->Send(data);

	delete data;
}


/*!
 *  \brief Method processing the message GetPublishedResponseData
 *  \param connection The connection from which the message was recieved
 *  \param message_data The message
 *
 *  Method processing the message GetPublishedResponseData.
 *
 *  This method is called internally by RecvMessage
 */
void RemoteProtocol::RecvMessage(Connection *connection, GetPublishedResponseData *message_data)
{
}


/*!
 *  \brief Method processing the message GetEntityRequestData
 *  \param connection The connection from which the message was recieved
 *  \param inc_msg The message
 *
 *  Method processing the message GetEntityRequestData.
 *
 *  This method is called internally by RecvMessage
 */
void RemoteProtocol::RecvMessage(Connection *connection, GetEntityRequestData *inc_msg)
{
	GetEntityResponseData *new_msg = new GetEntityResponseData();

	cpw::TypeId entity_id = inc_msg->GetEntityId();
	cpw::Entity *entity = cpw::EntityRegistry::GetInstance()->GetEntity(entity_id);

	if (entity != NULL)
	{
		if (entity->isPublished())
		{
			if (!inc_msg->GetSendEntity()) //Don't send the entity
			{
				new_msg->SetSendEntity(false);
				new_msg->SetEntityId(entity_id);

				new_msg->SetLastSynchro(((cpw::Remote*)entity)->GetLastSynch(connection->GetNode()));
				new_msg->SetCurrTime(((cpw::Loggable*)entity)->GetCurrTime());

				pending_synchs[std::pair<cpw::TypeId,cpw::RemoteNode>(entity->GetId(),connection->GetNode())] = false;
			}
			else
			{
				if (modifiers.find(entity->GetId()) == modifiers.end())
					modifiers[entity->GetId()] = new RemoteModifier(entity, this);
				connection_manager->Subscribe(connection, entity->GetId());

				new_msg->SetSendEntity(true);
				new_msg->SetEntityId(entity_id);
				new_msg->SetCurrTime(((cpw::Loggable*)entity)->GetCurrTime());
				modifiers[entity->GetId()]->SetExecuteNext(false);
				((cpw::Remote*)entity)->SetLastSynch(connection->GetNode(), ((cpw::Loggable*)entity)->GetCurrTime());
				modifiers[entity->GetId()]->SetExecuteNext(true);

				cpw::Node *node = ((cpw::Persistent*)entity)->GetPersistence();
				cpw::XmlCpwParser parsercpw;
				std::string xml_string;
				parsercpw.EntityToXml(node, xml_string);
				delete node;

				new_msg->SetEntityXml(xml_string);
			}

			new_msg->SetRequestID(inc_msg->GetRequestID());

			connection->Send(new_msg);
		}
	}

	delete new_msg;
}


/*!
 *  \brief Method processing the message GetEntityResponseData
 *  \param connection The connection from which the message was recieved
 *  \param inc_msg The message
 *
 *  Method processing the message GetEntityResponseData.
 *
 *  This method is called internally by RecvMessage
 */
void RemoteProtocol::RecvMessage(Connection *connection, GetEntityResponseData *inc_msg)
{
	cpw::Entity *entity;

	if (inc_msg->GetSendEntity())
	{
		std::string entity_xml = inc_msg->GetEntityXml();
		cpw::XmlCpwParser parsercpw;
		cpw::Node *root = parsercpw.XmlToEntity(entity_xml);

		if(root != NULL)
		{
			entity = cpw::EntityFactory::GetInstance()->CreateEntity(root->GetName());

			if(entity != NULL)
			{
				entity->SetPersistence(root);
				entity->SetUrl(cpw::ApplicationConfiguration::GetInstance()->GetEntityDirectory()
					+ "remote/" + (std::string) entity->GetId() + ".cel");

				bool has_parents = false;
				std::pair<std::multimap<cpw::TypeId, cpw::TypeId>::iterator,
					std::multimap<cpw::TypeId, cpw::TypeId>::iterator> parents_range;
				parents_range = child_parent.equal_range(entity->GetId());
				std::multimap<cpw::TypeId, cpw::TypeId>::iterator it = parents_range.first;
				while (it != parents_range.second)
				{
					has_parents = true;
					std::multimap<cpw::TypeId, cpw::TypeId>::iterator itaux = it;
					itaux++;
					cpw::Entity *parent = cpw::EntityRegistry::GetInstance()->GetEntity(it->second);
					
					if (modifiers.find(parent->GetId()) != modifiers.end())
					{
						RemoteModifier *pmodifier = modifiers[parent->GetId()];
						
						((cpw::Loggable*)parent)->SetRegisterChanges(false);
						pmodifier->SetExecuteNext(false);
						parent->Add(entity);
						pmodifier->SetExecuteNext(true);
						((cpw::Loggable*)parent)->SetRegisterChanges(true);
					}
					else
						parent->Add(entity);

					child_parent.erase(it);
					it = itaux;
				}
				

				if (modifiers.find(entity->GetId()) == modifiers.end())
					modifiers[entity->GetId()] = new RemoteModifier(entity, this);

				RemoteModifier *modifier = modifiers[entity->GetId()];
				
				((cpw::Loggable*)entity)->SetRegisterChanges(false);
				modifier->SetExecuteNext(false);

				((cpw::Loggable*)entity)->SetCurrTime(inc_msg->GetCurrTime());
				((cpw::Remote*)entity)->SetLastSynch(connection->GetNode(), ((cpw::Loggable*)entity)->GetCurrTime());

				cpw::EntityRegistry::GetInstance()->Add(entity);
				connection_manager->Subscribe(connection, entity->GetId());

				//Register children
				std::vector<cpw::TypeId> children = inc_msg->GetChildren();
				std::vector<cpw::TypeId>::const_iterator it_ch;
				for (it_ch = children.begin(); it_ch != children.end(); it_ch++)
					child_parent.insert(std::pair<cpw::TypeId, cpw::TypeId>(*it_ch, entity->GetId()));

				//Request children, using the same method created to request the entities selected
				//by the user
				GetEntities(connection, children);

				if (!has_parents)
				{
					//entity->AddParent(layer_tree->GetTopLayer());
					entity->GraphicInsert();
					//layer_tree->Notify();

					//Add it to the top layer always
					layer_tree->MakeParentActive(layer_tree->GetTopLayer()->GetId());
					layer_tree->AddToActiveLayer(entity);
				}
				else
				{
					entity->GraphicInsert();
					layer_tree->Notify();
				}

				modifier->SetExecuteNext(true);
				((cpw::Loggable*)entity)->SetRegisterChanges(true);
			}

			delete root;
		}
	}
	else //synchronize
	{
		entity = cpw::EntityRegistry::GetInstance()->GetEntity(inc_msg->GetEntityId());
		if (entity != NULL)
		{
			uint64_t send_ts = ((cpw::Remote*)entity)->GetLastSynch(connection->GetNode());

			if (send_ts > inc_msg->GetLastSynchro())
				send_ts = inc_msg->GetLastSynchro();

			pending_synchs[std::pair<cpw::TypeId,cpw::RemoteNode>(entity->GetId(),connection->GetNode())] = true;

			((cpw::Loggable*)entity)->SetCurrTime(inc_msg->GetCurrTime());

			SendChangesData *new_msg = new SendChangesData();
			new_msg->SetEntityId(entity->GetId());
			new_msg->SetLastSynchro(send_ts);
			new_msg->SetChanges(((cpw::Loggable*)entity)->GetChangesSince(send_ts));

			cpw::IdGenerator id_generator;

			new_msg->SetMessageId(id_generator.GetNewId());
			connection->Send(new_msg);
			delete new_msg;

			int nchild = entity->GetNumChildren();
			if (nchild > 0)
			{
				std::vector<cpw::TypeId> children;
				for (int i=0; i < nchild; i++)
					children.push_back(entity->GetChild(i)->GetId());

				GetEntities(connection, children);
			}

			//Add to parents
			std::pair<std::multimap<cpw::TypeId, cpw::TypeId>::iterator,
					std::multimap<cpw::TypeId, cpw::TypeId>::iterator> parents_range;
			parents_range = child_parent.equal_range(entity->GetId());
			std::multimap<cpw::TypeId, cpw::TypeId>::iterator it = parents_range.first;
			while (it != parents_range.second)
			{
				std::multimap<cpw::TypeId, cpw::TypeId>::iterator itaux = it;
				itaux++;
				cpw::Entity *parent = cpw::EntityRegistry::GetInstance()->GetEntity(it->second);

				//Don't add the to the parent twice
				bool addparent = true;
				int iparent = 0;
				while (iparent < entity->GetNumParents())
				{
					if (entity->GetParent(iparent) == parent)
					{
						addparent = false;
						break;
					}
					iparent++;
				}
				
				if (addparent)
				{
					if (modifiers.find(parent->GetId()) != modifiers.end())
					{
						RemoteModifier *pmodifier = modifiers[parent->GetId()];
						
						((cpw::Loggable*)parent)->SetRegisterChanges(false);
						pmodifier->SetExecuteNext(false);
						parent->Add(entity);
						pmodifier->SetExecuteNext(true);
						((cpw::Loggable*)parent)->SetRegisterChanges(true);
					}
					else
						parent->Add(entity);
				}

				child_parent.erase(it);
				it = itaux;
			}
		}
	}
}


/*!
 *  \brief Method processing the message RequestSynchroData
 *  \param connection The connection from which the message was recieved
 *  \param inc_msg The message
 *
 *  Method processing the message RequestSynchroData.
 *
 *  This method is called internally by RecvMessage
 */
void RemoteProtocol::RecvMessage(Connection *connection, RequestSynchroData *inc_msg)
{
	//First of all, check that it's the first time this message gets to this node
	std::list<cpw::TypeId>::iterator it = find(processed_msg.begin(), processed_msg.end(),
		inc_msg->GetMessageId());

	if (it == processed_msg.end())
	{
		if (processed_msg.size() >= 100)
			processed_msg.pop_front();

		processed_msg.push_back(inc_msg->GetMessageId());

		cpw::Entity *entity;

		entity = cpw::EntityRegistry::GetInstance()->GetEntity(inc_msg->GetEntityId());
		if (entity != NULL)
		{
			uint64_t send_ts = ((cpw::Remote*)entity)->GetLastSynch(connection->GetNode());

			if (send_ts > inc_msg->GetLastSynchro())
				send_ts = inc_msg->GetLastSynchro();

			pending_synchs[std::pair<cpw::TypeId,cpw::RemoteNode>(entity->GetId(),connection->GetNode())] = true;

			((cpw::Loggable*)entity)->SetCurrTime(inc_msg->GetCurrTime());

			SendChangesData *new_msg = new SendChangesData();
			new_msg->SetEntityId(entity->GetId());
			new_msg->SetLastSynchro(send_ts);
			new_msg->SetChanges(((cpw::Loggable*)entity)->GetChangesSince(send_ts));
			connection->Send(new_msg);
			delete new_msg;
		}
	}
}


/*!
 *  \brief Method processing the message SendChangesData
 *  \param connection The connection from which the message was recieved
 *  \param inc_msg The message
 *
 *  Method processing the message SendChangesData.
 *
 *  This method is called internally by RecvMessage
 */
void RemoteProtocol::RecvMessage(Connection *connection, SendChangesData *inc_msg)
{
	//First of all, check that it's the first time this message gets to this node
	std::list<cpw::TypeId>::iterator it = find(processed_msg.begin(), processed_msg.end(),
		inc_msg->GetMessageId());
	if (it != processed_msg.end())
		return;
	if (processed_msg.size() >= 100)
		processed_msg.pop_front();
	processed_msg.push_back(inc_msg->GetMessageId());

	//check the entity exists
	cpw::Entity *entity = cpw::EntityRegistry::GetInstance()->GetEntity(inc_msg->GetEntityId());
	if (entity == NULL)
		return;


	std::vector<cpw::Change> changes = inc_msg->GetChanges();

	//prepare the synch by getting the timestamps
	std::pair<cpw::TypeId, cpw::RemoteNode> p_synch(inc_msg->GetEntityId(), connection->GetNode());
	std::map<std::pair<cpw::TypeId, cpw::RemoteNode>, bool>::iterator ps_it = pending_synchs.find(p_synch);
	if (ps_it == pending_synchs.end())
		return; //there MUST be a previous message setting the behaviour

	bool replace = ps_it->second;
	pending_synchs.erase(ps_it);

	if (!replace) //This node is recieving the synchronize request
	{
		SendChangesData *new_msg = new SendChangesData();
		new_msg->SetEntityId(entity->GetId());
		new_msg->SetMessageId(inc_msg->GetMessageId());
		//uint64_t ts = ((cpw::Remote*)entity)->GetLastSynch(connection->GetNode());
		uint64_t ts = inc_msg->GetLastSynchro();
		new_msg->SetChanges(((cpw::Loggable*)entity)->GetChangesSince(ts));
		connection->Send(new_msg);
		delete new_msg;
	}
	//else it is requesting the synchro


	//propagate the synchro
	RequestSynchroData *new_msg2 = new RequestSynchroData();

	new_msg2->SetEntityId(entity->GetId());
	new_msg2->SetCurrTime(((cpw::Loggable*)entity)->GetCurrTime());
	new_msg2->SetMessageId(inc_msg->GetMessageId());

	std::vector<Connection*> more_conn = connection_manager->GetSubscribers(entity->GetId());

	for (std::vector<Connection*>::iterator it = more_conn.begin(); it!=more_conn.end();it++)
	{
		if  (*it != connection)
		{
			uint64_t stored_last_synch;
			stored_last_synch = ((cpw::Remote*)entity)->GetLastSynch(connection->GetNode());
			if (stored_last_synch < inc_msg->GetLastSynchro())
				new_msg2->SetLastSynchro(stored_last_synch);
			else
				new_msg2->SetLastSynchro(inc_msg->GetLastSynchro());

			(*it)->Send(new_msg2);
		}
	}

	delete new_msg2;


	if (modifiers.find(entity->GetId()) == modifiers.end())
		modifiers[entity->GetId()] = new RemoteModifier(entity, this);
	connection_manager->Subscribe(connection, entity->GetId());

	RemoteModifier *modifier = modifiers[entity->GetId()];

	if (changes.size() != 0)
	{
		std::set<std::string> real_changes;
		std::set<cpw::TypeId> children;
		std::vector<cpw::Change> local_changes = 
			((cpw::Loggable*)entity)->GetChangesSince(changes.begin()->time_stamp);
		if (local_changes.empty())
			local_changes.push_back(cpw::Change("","",((cpw::Loggable*)entity)->GetCurrTime()));
			

		std::vector<cpw::Change>::reverse_iterator it_ch = changes.rbegin();
		std::vector<cpw::Change>::reverse_iterator it_loc_ch = local_changes.rbegin();

		bool cislocal; //true: change is local
		
		((cpw::Loggable*)entity)->SetRegisterChanges(false);
		modifier->SetExecuteNext(false);
		while (it_ch != changes.rend())
		{
			cpw::Change thischange;
			//First get the next change to deal with (in reverse order)
			if (it_loc_ch == local_changes.rend())
			{
				cislocal = false;
				thischange = *it_ch;
			}
			else
			{
				if (it_ch->time_stamp < it_loc_ch->time_stamp)
				{
					thischange = *it_loc_ch;
					cislocal = true;
				}
				else
				{
					if (it_ch->time_stamp > it_loc_ch->time_stamp)
					{
						thischange = *it_ch;
						cislocal = false;
					}
					else
					{
						if (replace)
						{
							thischange = *it_ch;
							it_loc_ch++;
							cislocal = false;
						}
						else
						{
							thischange = *it_loc_ch;
							it_ch++;
							cislocal = true;
						}
					}
				}
			}

			bool ignored = false;

			//Then find out if the change is real
			if ((thischange.field == "AddChild") &&
				(children.find(thischange.value) == children.end()))
			{
				if (!cislocal) //Change came from remote node, so needs update
					AddEntity(connection, entity->GetId(), thischange.value);
				children.insert(thischange.value);
			}
			else if ((thischange.field == "DeleteChild") &&
				(children.find(thischange.value) == children.end()))
			{
				if (!cislocal) //Change came from remote node, so needs update
					DeleteEntity(connection, entity->GetId(), thischange.value);
				children.insert(thischange.value);
			}
			else if (real_changes.find(thischange.field) == real_changes.end())
			{
				std::vector<std::string>::iterator ichange =
					find(ignore_fields.begin(), ignore_fields.end(), thischange.field);
				if (ichange == ignore_fields.end()) //not found so apply change
				{
					if (!cislocal) //Change came from remote node, so needs update
						entity->SetValue(thischange.field, thischange.value);
					real_changes.insert(thischange.field);
				}
				else
					ignored = true;
			}

			if (!cislocal && !ignored)
				//Add the change to the list
				((cpw::Loggable*)entity)->AddChange(thischange, replace);

			if (cislocal)
				it_loc_ch++;
			else
				it_ch++;
		}

		((cpw::Remote*)entity)->SetLastSynch(connection->GetNode(), changes.rbegin()->time_stamp);


		//entity->GraphicDelete();
		//entity->GraphicInsert();
		entity->GraphicUpdate();

		modifier->SetExecuteNext(true);
		((cpw::Loggable*)entity)->SetRegisterChanges(true);

	}
}


/*!
 *  \brief Method processing the message SetValueData
 *  \param connection The connection from which the message was recieved
 *  \param inc_msg The message
 *
 *  Method processing the message SetValueData.
 *
 *  This method is called internally by RecvMessage
 */
void RemoteProtocol::RecvMessage(Connection *connection, SetValueData *inc_msg)
{
	std::list<cpw::TypeId>::iterator it = find(processed_msg.begin(), processed_msg.end(),
		inc_msg->GetMessageId());

	if (it == processed_msg.end())
	{
		if (processed_msg.size() >= 100)
			processed_msg.pop_front();

		processed_msg.push_back(inc_msg->GetMessageId());

		cpw::TypeId entity_id = inc_msg->GetEntityId();
		cpw::Entity *entity = cpw::EntityRegistry::GetInstance()->GetEntity(entity_id);
		if (entity != NULL && modifiers.find(entity_id) != modifiers.end())
		{
			RemoteModifier *modifier = modifiers[entity_id];

			if (inc_msg->GetField() == "AddChild")
				AddEntity(connection, entity_id, inc_msg->GetValue());
			else if (inc_msg->GetField() == "DeleteChild")
				DeleteEntity(connection, entity_id, inc_msg->GetValue());

			modifier->SetValue(inc_msg->GetField(),	inc_msg->GetValue(), inc_msg->GetTimeStamp());
			((cpw::Remote*)entity)->SetLastSynch(connection->GetNode(), inc_msg->GetTimeStamp());

			std::vector<Connection*> subscribers = connection_manager->GetSubscribers(inc_msg->GetEntityId());
			for (std::vector<Connection*>::iterator it=subscribers.begin(); it!= subscribers.end();it++)
			{
				if (*it != connection)
					(*it)->Send(inc_msg);
			}
		}
	}
}


/*!
 *  \brief Method processing the message DisconnectEntityData
 *  \param connection The connection from which the message was recieved
 *  \param message_data The message
 *
 *  Method processing the message DisconnectEntityData.
 *
 *  This method is called internally by RecvMessage
 */
void RemoteProtocol::RecvMessage(Connection *connection, DisconnectEntityData *message_data)
{
	cpw::TypeId entity_id = message_data->GetEntityId();
	connection_manager->Unsubscribe(connection, entity_id);

	CleanUpModifiers();
}


/*!
 *  \brief Auxiliar method used internally to register an entity
 *  \param connection Connection from which the addition was initiated
 *  \param entity_id ID of the entity to which the child will be added
 *  \param child_id ID of the entity being added to the previous one
 *
 *   Auxiliar method used internally to register an entity.
 */
void RemoteProtocol::AddEntity(Connection * connection, const cpw::TypeId &entity_id,
	const cpw::TypeId &child_id)
{
	RemoteModifier *modifier;
	if (modifiers.find(entity_id) != modifiers.end())
		modifier = modifiers[entity_id];
	else
		modifier = NULL;

	cpw::Entity *entity = cpw::EntityRegistry::GetInstance()->GetEntity(entity_id);
	cpw::Entity *child = cpw::EntityRegistry::GetInstance()->GetEntity(child_id);

	if (child == NULL)
	{
		child_parent.insert(std::pair<cpw::TypeId, cpw::TypeId>(child_id, entity_id));

		//Request children, using the same method created to request the entities selected
		//by the user
		std::vector<cpw::TypeId> children;
		children.push_back(child_id);
		GetEntities(connection, children);
	}
	else
	{
		//Don't add the to the parent twice
		bool addparent = true;
		int iparent = 0;
		while (iparent < child->GetNumParents())
		{
			if (child->GetParent(iparent) == entity)
			{
				addparent = false;
				break;
			}
			iparent++;
		}
		
		if (addparent)
		{
			((cpw::Loggable*)entity)->SetRegisterChanges(false);
			if (modifier != NULL)
				modifier->SetExecuteNext(false);

			entity->Add(child);
			entity->GraphicInsert();
			layer_tree->Notify();

			if (modifier != NULL)
				modifier->SetExecuteNext(true);
			((cpw::Loggable*)entity)->SetRegisterChanges(true);
		}
	}

}


/*!
 *  \brief Auxiliar method used internally to delete an entity
 *  \param connection Connection from which the addition was initiated
 *  \param entity_id ID of the entity to which the child will be added
 *  \param child_id ID of the entity being added to the previous one
 *
 *  Auxiliar method used internally to delete an entity.
 */
void RemoteProtocol::DeleteEntity(Connection * connection, const cpw::TypeId &entity_id,
	const cpw::TypeId &child_id)
{
	cpw::Entity *entity = cpw::EntityRegistry::GetInstance()->GetEntity(entity_id);
	cpw::Entity *child = cpw::EntityRegistry::GetInstance()->GetEntity(child_id);

	if (entity != NULL && child != NULL)
	{
		((cpw::Loggable*)entity)->SetRegisterChanges(false);

		entity->GraphicDelete();
		if (modifiers.find(entity->GetId()) != modifiers.end())
		{
			RemoteModifier *modifier = modifiers[entity->GetId()];
			modifier->SetExecuteNext(false);

			((cpw::ContainerLayer*)entity)->DeleteChild(child);

			modifier->SetExecuteNext(true);
		}
		else
			((cpw::ContainerLayer*)entity)->DeleteChild(child);

		if(child->GetNumParents()==0)
			DeleteEntity_helper(connection, child);

		entity->GraphicDelete();
		entity->GraphicInsert();
		//entity->GraphicUpdate();
		layer_tree->Notify();

		
		((cpw::Loggable*)entity)->SetRegisterChanges(true);
	}
}


/*!
 *  \brief Helper method used to deal with recursion when deleting an entity
 *  \param connection Connection from which the addition was initiated
 *  \param entity Entity being deleted
 *
 *  Helper method used to deal with recursion when deleting an entity.
 */
void RemoteProtocol::DeleteEntity_helper(Connection *connection, cpw::Entity *entity)
{
	if (entity->GetNumParents()==0)
	{
		if(entity->isContainer())
		{
			((cpw::Loggable*)entity)->SetRegisterChanges(false);

			while (((cpw::ContainerLayer*)entity)->GetNumChildren() != 0)
			{
				cpw::Entity *child = ((cpw::ContainerLayer*)entity)->GetChild(0);

				if (modifiers.find(entity->GetId()) != modifiers.end())
				{
					RemoteModifier *modifier = modifiers[entity->GetId()];

					modifier->SetExecuteNext(false);
					((cpw::ContainerLayer*)entity)->DeleteChild(child);
					modifier->SetExecuteNext(true);

				}
				else
					((cpw::ContainerLayer*)entity)->DeleteChild(child);

				DeleteEntity_helper(connection, child);

			}

			((cpw::ContainerLayer*)entity)->DeleteReferenceToMe();

			((cpw::Loggable*)entity)->SetRegisterChanges(true);
		}
		cpw::EntityRegistry::GetInstance()->DeleteEntity(entity->GetId());

		connection_manager->Unsubscribe(connection, entity->GetId());
	}
}


/*!
 *  \brief Method that tries to erase the modifiers if it's possible
 *
 *  Method that tries to erase the modifiers if it's possible, leaving the modifiers
 *  list in a clean state.
 */
void RemoteProtocol::CleanUpModifiers()
{
	std::map<cpw::TypeId, RemoteModifier*>::iterator it = modifiers.begin();
	while (it != modifiers.end())
	{
		if (connection_manager->GetSubscribers(it->first).empty())
		{
			std::map<cpw::TypeId, RemoteModifier*>::iterator itaux = it;
			itaux++;
			delete it->second;
			modifiers.erase(it);
			it = itaux;
		}
		else
			it++;
	}
}
