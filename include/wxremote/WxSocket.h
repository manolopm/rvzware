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
#ifndef _WXSOCKET_
#define _WXSOCKET_

#include <wx/event.h>
#include <wx/socket.h>

#include <cpw/common/RemoteNode.h>

#include <remote/DataStream.h>
#include <remote/ISocket.h>

#include "WxRemoteExport.h"
#include "WxSocketHandler.h"
#include "WxClientSocketThread.h"

/*!
 *  \file WxSocket.h
 */

namespace cpw 
{ 
	namespace wxremote
	{
		/*!
		 *  \class WxSocket WxSocket.h <wxremote/WxSocket.h>
		 *  \ingroup wxremote
		 *  \brief This class is an implementation of ISocket to use wxWidgets
		 *
		 *  This class is an implementation of ISocket to use wxWidgets.
		 */
		class WxSocket: public cpw::remote::ISocket
		{           
		public:
			WxSocket(wxSocketClient *_socket, wxEvtHandler *socket_handler);
			~WxSocket();

			void Send(const cpw::remote::DataStream &data);
			void ConnectTo(const cpw::RemoteNode &node);
			void Disconnect();

		private:
			wxSocketClient *socket;
			WxClientSocketThread *socket_thread;
			wxEvtHandler *socket_handler;
		};
	}
}

#endif
