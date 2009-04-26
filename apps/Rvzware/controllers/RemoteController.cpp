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
#include <vector>
#include <string>
#include <wx/wx.h>
#include <wx/window.h>
#include <wx/dynlib.h>

#ifdef WIN32
	#include <iphlpapi.h>
	#include <windows.h>
#endif

#include <stdafx.h>

#include <cpw/entity/LayerTree.h>
#include <remote/Connection.h>
#include <remote/IRemoteCallback.h>
#include <remote/RemoteCallback.h>
#include <remote/PublishedTree.h>
#include <remote/MessageData.h>
#include <remote/GetPublishedResponseData.h>
#include <remote/RemoteProtocol.h>
#include <wxremote/WxSocketFactory.h>
#include <wxremote/WxResolver.h>
#include <gui/UIRemoteConnection.h>
#include <gui/UIRemotePublished.h>
#include <cpw/ApplicationConfiguration.h>
#include <controllers/RemoteController.h>



std::string GetMACaddress(void)
{
	std::string address = "a3af11wf1v233c11";
			
	#ifdef WIN32

		address = "";

		PIP_ADAPTER_INFO pAdapterInfo;

		DWORD dwBufLen = sizeof(IP_ADAPTER_INFO);
		pAdapterInfo = (PIP_ADAPTER_INFO) malloc(dwBufLen);

		DWORD dwStatus = GetAdaptersInfo(pAdapterInfo, &dwBufLen);

		if (dwStatus == ERROR_BUFFER_OVERFLOW) //there are more interfaces
		{
			free(pAdapterInfo);
			pAdapterInfo = (PIP_ADAPTER_INFO) malloc(dwBufLen);

			dwStatus = GetAdaptersInfo(pAdapterInfo, &dwBufLen);
		}

		assert(dwStatus == ERROR_SUCCESS);

		//Only use the first adapter
		for (unsigned int i=0; i<pAdapterInfo->AddressLength; i++)
		{
			uint8_t n = pAdapterInfo->Address[i];

			uint8_t mod = n%16;
			uint8_t res = n/16;
			
			if (res <=9)
				address += res + 48;
			else
				address += res + 65 - 10; //10 = decimal value for A

			if (mod <= 9)
				address += mod + 48;
			else
				address += mod + 65 - 10; //10 = decimal value for A
		}

		free(pAdapterInfo);

	#endif

	return address;
}


using namespace cpw::controllers;

static cpw::LayerTree *slayer_tree = NULL;
static int slisten_port = 3000;

/*!
 *  \param parent_window Pointer to the main panel
 *  \param layer_tree Pointer to the LayerTree object of the application
 *  \param listen_port Service port to listen to
 */
RemoteController::RemoteController(wxWindow *parent_window, cpw::LayerTree *layer_tree, int listen_port)
	: parent_window(parent_window), last_ip(""), last_port(3000)
{
	slayer_tree = layer_tree;
	slisten_port = listen_port;
	wxSocketBase::Initialize();

	cpw::RemoteNode::SetResolver(resolver = new wxremote::WxResolver());

	factory = new wxremote::WxSocketFactory(parent_window);

	std::string root_path = cpw::ApplicationConfiguration::GetInstance()->GetEntityDirectory();

	protocol = new cpw::remote::RemoteProtocol(listen_port, layer_tree, factory);

	recv_newconnection_cb = new cpw::remote::RemoteCallback<RemoteController>(this,
		&RemoteController::RecvNewConnection);
	protocol->SetCallback(cpw::remote::msgTypeConnection, recv_newconnection_cb);

	recv_disconnection_cb = new cpw::remote::RemoteCallback<RemoteController>(this,
		&RemoteController::RecvDisconnection);
	protocol->SetCallback(cpw::remote::msgTypeDisconnection, recv_disconnection_cb);

	recv_getpublishedresponse_cb = new cpw::remote::RemoteCallback<RemoteController>(this,
		&RemoteController::RecvGetPublishedResponse);
	protocol->SetCallback(cpw::remote::msgTypeGetPublishedResponse, recv_getpublishedresponse_cb);

	recv_disconnectentity_cb = new cpw::remote::RemoteCallback<RemoteController>(this,
		&RemoteController::RecvDisconnectEntity);
	protocol->SetCallback(cpw::remote::msgTypeDisconnectEntity, recv_disconnectentity_cb);

	if(layer_tree->GetTopLayer() != NULL)
		protocol->ConnectToAllEntities();
}


RemoteController::~RemoteController()
{
	delete factory;
	delete resolver;
	delete protocol;
	delete recv_newconnection_cb;
	delete recv_disconnection_cb;
	delete recv_getpublishedresponse_cb;
	delete recv_disconnectentity_cb;
}


/*!
 *  \brief Returns the ConnectionManager object
 *
 *  Returns the ConnectionManager object
 */
cpw::remote::ConnectionManager *RemoteController::GetConnectionManager()
{
	return protocol->GetConnectionManager();
}


/*!
 *  \brief Returns the RemoteProtocol object
 *
 *  Returns the RemoteProtocol object
 */
cpw::remote::RemoteProtocol *RemoteController::GetRemoteProtocol()
{
	return protocol;
}


/*!
 *  \brief Lets the user create a new connection with a remote node
 *
 *  Lets the user create a new connection with a remote node.
 *
 *  This shows a new connection window and then sends a GetPublishedRequest through the new connection.
 */
void RemoteController::Connect()
{
	cpw::gui::UIRemoteConnection ui_connect(parent_window);
	ui_connect.SetHostname(last_ip);//wxEmptyString
	ui_connect.SetPort(last_port);

	bool stop = false;
	while (!stop)
	{
		if (ui_connect.ShowModal() != wxID_OK)
			stop = true;
		else
		{
			std::string hostnametemp = last_ip = ui_connect.GetHostname();
			int porttemp = ui_connect.GetPort();

			if (porttemp < 1 || porttemp > 65535)
			{
				wxMessageBox(_("Port is incorrect. Use a number between 1 and 65535. Default is 3000"),
					_("Information"), wxICON_INFORMATION);
				ui_connect.SetPort(3000);
				stop = false;
			}

			cpw::remote::Connection *connection = protocol->Connect(hostnametemp, porttemp);
			if (connection != NULL)
			{
				protocol->GetPublished(connection);
				stop = true;
			}
			else
			{
				wxMessageBox(_("Couldn't connected"), _("Information"), wxICON_INFORMATION);
				stop = false;
			}
		}
	}
}


/*!
 *  \brief Disconnects from all entities connected to other nodes
 *  \param entity The root entity to start disconnecting. Default value (NULL) means disconnect the entire tree
 *
 *  Disconnects from all entities connected to other nodes.
 */
void RemoteController::DisconnectAllEntities(cpw::Entity *entity)
{
	protocol->DisconnectAllEntities(entity);
}


/*!
 *  \brief Connects with all the nodes registered on the entity
 *
 *  Connects with all the nodes registered on the entity.
 */
void RemoteController::ConnectAllEntities()
{
	protocol->ConnectToAllEntities();
}


/*!
 *  \brief Callback method called when a new connection is received
 *  \param connection The new connection
 *  \param message_data ConnectionData object
 *
 *  
 *  Callback method called when a new connection is received.
 */
void RemoteController::RecvNewConnection(const cpw::remote::Connection *connection,
			const cpw::remote::MessageData *message_data)
{
	//wxMessageBox(_("New connection"), _("Information"), wxICON_INFORMATION);
}


/*!
 *  \brief Callback method called when a disconnection is received
 *  \param connection The lost connection
 *  \param message_data DisconnectionData object
 *
 *  Callback method called when a new connection is received.
 */
void RemoteController::RecvDisconnection(const cpw::remote::Connection *connection,
			const cpw::remote::MessageData *message_data)
{
/*	std::string msg("Lost connection with ");
	msg += connection->GetNode().GetHostname();
	msg += "\n\nTry again";
	wxMessageBox(msg, _("Information"), wxICON_INFORMATION);*/
}


/*!
 *  \brief Callback method called when a GetPublishedResponse is received
 *  \param connection The lost connection
 *  \param message_data A GetPublisehdResponseData object with the message
 *
 *  Callback method called when a GetPublishedResponse is received.
 *
 *  Calling this method will open a window with a tree with the remote published entities.
 */
void RemoteController::RecvGetPublishedResponse(const cpw::remote::Connection *connection,
			const cpw::remote::MessageData *message_data)
{
	//Get data from message
	const cpw::remote::PublishedNode *entities = ((cpw::remote::GetPublishedResponseData*)message_data)->GetEntities();

	cpw::gui::UIRemotePublished ui_published(parent_window);

	ui_published.SetEntities(entities);
	if (ui_published.ShowModal() == wxID_OK)
	{
		if (ui_published.IsBack())
		  Connect();
		std::vector<cpw::TypeId, std::allocator<cpw::TypeId> > vct = ui_published.GetSelectedEntities();
		protocol->GetEntities((cpw::remote::Connection *)connection,
				      (std::vector<cpw::TypeId, std::allocator<cpw::TypeId> > &)vct);
	}

}


/*!
 *  \brief Callback method called when a disconnection from an entity is received
 *  \param connection The connection
 *  \param message_data A DisconnectEntityData message
 *
 *  Callback method called when a disconnection from an entity is received.
 */
void RemoteController::RecvDisconnectEntity(const cpw::remote::Connection *connection,
			const cpw::remote::MessageData *message_data)
{
	
}
