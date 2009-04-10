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
#ifndef _WXSOCKETHANDLER_
#define _WXSOCKETHANDLER_

#include <wx/event.h>
#include <wx/socket.h>

#include <remote/ISocket.h>
#include <remote/RemoteProtocol.h>
#include <remote/DataStream.h>

#include "WxRemoteExport.h"
#include "WxSocketEvent.h"

/*!
 *  \file WxSocketHandler.h
 */

namespace cpw 
{ 
	namespace wxremote
	{
		/*!
		 *  \class WxSocketHandler WxSocketHandler.h <wxremote/WxSocketHandler.h>
		 *  \ingroup wxremote
		 *  \brief This is the class recieving socket events from wxWidgets sockets
		 *
		 *  This is the class recieving socket events from wxWidgets sockets.
		 */
		class WxSocketHandler: public wxEvtHandler
		{
		public:
			WxSocketHandler(cpw::remote::RemoteProtocol *protocol);
			~WxSocketHandler();
			
			bool ListenOn(int port);
				
		private:			
			void OnServerEvent(wxSocketEvent& event); //New incoming connection event
			void OnClientEvent(WxSocketEvent& event); //Events on a socket already created

			cpw::remote::RemoteProtocol *protocol;  //Link to handle events
			wxSocketServer *listen_socket; //listening socket. Waits for new incoming connections

			DECLARE_EVENT_TABLE()
		};


		//Enum for object IDs. Used on the wxWidgets event table
		enum
		{
			// id for socket events
			SERVER_ID = 100
		};

	}
}

#endif

