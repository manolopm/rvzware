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
#ifndef _REMOTECONTROLLER_
#define _REMOTECONTROLLER_

#ifdef WIN32
 #include <windows.h>
#endif
#include <string>
#include <wx/wx.h>
#include <wx/window.h>
#include <wx/dynlib.h>

#include <cpw/common/IResolver.h>
#include <cpw/entity/LayerTree.h>
#include <remote/Connection.h>
#include <remote/ConnectionManager.h>
#include <remote/GetPublishedResponseData.h>
#include <remote/RemoteCallback.h>
#include <remote/RemoteProtocol.h>
#include <remote/ISocketFactory.h>

/*!
 *  \file RemoteController.h
 */

std::string GetMACaddress(void);

namespace cpw
{

	namespace controllers 
	{  
		class UIRemoteConnection;

		/*!
		 *  \class RemoteController RemoteController.h <controllers/RemoteController.h"
		 *  \ingroup cwcontent
		 *  \brief This is the main controller for the Remote module
		 *
		 *  This is the main controller for the Remote module.
		 */
		class RemoteController
		{
		public:
			
			RemoteController(wxWindow *parent_window = NULL, cpw::LayerTree *layer_tree = NULL, int listen_port = 3000);
			virtual ~RemoteController();

			cpw::remote::ConnectionManager *GetConnectionManager();
			cpw::remote::RemoteProtocol *GetRemoteProtocol();

			void Connect();
			void DisconnectAllEntities(cpw::Entity *entity = NULL);
			void ConnectAllEntities();

			//Events
			void RecvNewConnection(const cpw::remote::Connection *connection,
				const cpw::remote::MessageData *message_data);
			void RecvDisconnection(const cpw::remote::Connection *connection,
				const cpw::remote::MessageData *message_data);
			void RecvGetPublishedResponse(const cpw::remote::Connection *connection,
				const cpw::remote::MessageData *message_data);
			void RecvDisconnectEntity(const cpw::remote::Connection *connection,
				const cpw::remote::MessageData *message_data);

		private:
			wxWindow *parent_window;

			std::string last_ip;
			int last_port;

			cpw::remote::RemoteProtocol *protocol;
			cpw::remote::ISocketFactory *factory;
			cpw::IResolver *resolver;

			cpw::remote::RemoteCallback<RemoteController> *recv_newconnection_cb;
			cpw::remote::RemoteCallback<RemoteController> *recv_disconnection_cb;
			cpw::remote::RemoteCallback<RemoteController> *recv_getpublishedresponse_cb;
			cpw::remote::RemoteCallback<RemoteController> *recv_disconnectentity_cb;
		};

	}
}

#endif
