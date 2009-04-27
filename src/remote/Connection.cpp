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

#include <remote/stdafx.h>

#include <cpw/common/pstdint.h>
#include <cpw/common/RemoteNode.h>

#include <remote/DataStream.h>
#include <remote/MessageData.h>
#include <remote/ConnectionData.h>
#include <remote/DisconnectionData.h>
#include <remote/GetPublishedRequestData.h>
#include <remote/GetPublishedResponseData.h>
#include <remote/GetEntityRequestData.h>
#include <remote/GetEntityResponseData.h>
#include <remote/SendChangesData.h>
#include <remote/SetValueData.h>
#include <remote/RequestSynchroData.h>
#include <remote/DisconnectEntityData.h>
#include <remote/Connection.h>

#ifndef UINT8_MAX
#define UINT8_MAX 255
#endif
using namespace cpw::remote;


/*!
 *  \param socket The socket associated with this connection
 *  \param node Identifier of the connection
 */
Connection::Connection(ISocket *socket, cpw::RemoteNode node)
	: socket(socket), last_request(0), node(node) {}


Connection::~Connection()
{
	delete socket;
}


/*!
 *  \brief Returns the identifier of the connection
 *
 *  Returns the RemoteNode object which is the identifier of the connection.
 */
cpw::RemoteNode Connection::GetNode()
{
	return node;
}


/*!
 *  \brief Returns the identifier of the connection
 *
 *  Returns the RemoteNode object which is the identifier of the connection.
 *
 *  This version of the method allows its use with const objects.
 */
cpw::RemoteNode Connection::GetNode() const
{
	return node;
}


/*!
 *  \brief Connects to the corresponding address (specified on the constructor
 *
 *  Connects to the corresponding address (specified on the constructor.
 *
 *  This method uses the socket and node that where registered with the constructor.
 */
void Connection::Connect()
{
	socket->ConnectTo(node);
}


/*!
 *  \brief Disconnects from the remote node
 *
 *  Disconnects from the remote node.
 */
void Connection::Disconnect()
{
	socket->Disconnect();
}


/*!
 *  \brief Returns the corresponding MessageData object to the parameter
 *  \param data The data stream containing the message
 *
 *  Returns the corresponding MessageData object to the parameter.
 *
 *  Should be used whenever a new message is received.
 */
MessageData *Connection::RecvMessage(const DataStream &data)
{
	int message_type = (int)data[0];
	MessageData *message = GetMessageData((MessageType)message_type);
	if (message != NULL)
	{
		if (message->Decode(data))
			return message;
			/*if (ProcessMessage(message))
				return message;
			else
			{
				delete message;
				return NULL;
			}*/
	}

	return NULL;
}


/*!
 *  \brief Sends a message through the socket
 *
 *  Sends a message through the socket.
 *
 *  This method assigns the appropriate request_id, in case it is necessary.
 */
void Connection::Send(MessageData *message)
{
	//Do not repeat GetPublishedRequest
	/*if (message->GetMessageType() == msgTypeGetPublishedRequest)
	{
		if (requesting_published)
			return;
		else
			requesting_published = true;
	}*/

	if (message->IsRequest())
	{
		message->SetRequestID(last_request);
		last_request = (last_request + 1) % UINT8_MAX; //on the stream, we dedicate just 1 byte
	}
	socket->Send(message->Code());
}


/*!
 *  \brief Returns a new MessageData, according to the type specified
 *  \param message_type The kind of message to be created
 *
 *  Returns a new MessageData, according to the type specified.
 */
MessageData* Connection::GetMessageData(MessageType message_type)
{
	switch(message_type)
	{
		case msgTypeConnection:
			return new ConnectionData();
			break;
		case msgTypeDisconnection:
			return new DisconnectionData();
			break;
		case msgTypeGetPublishedRequest:
			return new GetPublishedRequestData();
			break;
		case msgTypeGetPublishedResponse:
			return new GetPublishedResponseData();
			break;
		case msgTypeGetEntityRequest:
			return new GetEntityRequestData();
			break;
		case msgTypeGetEntityResponse:
			return new GetEntityResponseData();
			break;
		case msgTypeSendChanges:
			return new SendChangesData();
			break;
		case msgTypeSetValue:
			return new SetValueData();
			break;
		case msgTypeRequestSynchro:
			return new RequestSynchroData();
			break;
		case msgTypeDisconnectEntity:
			return new DisconnectEntityData();
			break;
			
		default:
			return NULL;
	}
}


/*bool Connection::ProcessMessage(MessageData *msg_data)
{
	switch(msg_data->GetMessageType())
	{
		case msgTypeConnection:
			return true;
			break;
		case msgTypeDisconnection:
			return true;
			break;
		case msgTypeGetPublishedRequest:
			return true;
			break;
		case msgTypeGetPublishedResponse:
			if (requesting_published)
			{
				requesting_published = false;
				return true;
			}
			else
				return false;
			break;
		case msgTypeGetEntityRequest:
		{
			//cpw::TypeId entity_id = inc_msg->GetEntity();
			//cpw::Entity *entity = cpw::EntityRegistry::GetInstance()->GetEntity(entity_id);
			//if (modifiers.find(entity_id) == modifiers.end())
			//	modifiers[entity_id] = new RemoteModifier(entity, this);
			return true;
			break;
		}
		case msgTypeGetEntityResponse:
		{
			return true;
			break;
		}
		case msgTypeSendChanges:

			break;
		case msgTypeSetValue:
		{
			return true;
			//SetValueData *inc_msg = (SetValueData*) msg_data;
			//if (modifiers.find(inc_msg->GetEntityId()) == modifiers.end())
			//	return false;
			//else
			//{
			//	return true;
			//}
		}
			break;
		case msgTypeRequestSynchro:
			return true;
			break;
		case msgTypeDisconnectEntity:
			return true;
			break;
		default:
			return false;
	}
	return true;
}*/
