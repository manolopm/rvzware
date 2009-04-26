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

#include <wx/event.h>
#include <wx/socket.h>

#include "stdafx.h"

#include <cpw/common/RemoteNode.h>
#include <remote/DataStream.h>
#include <remote/ISocket.h>

#include "WxSocket.h"
#include "WxSocketHandler.h"
#include "WxClientSocketThread.h"
#include "WxSocketEvent.h"




using namespace cpw::wxremote;


/*!
 *  \param socket The wxWidgets socket object
 *  \param socket_handler The handler used to receive events from the socket
 *
 *  The second parameter should be of typ WxSocketHandler
 */
WxSocket::WxSocket(wxSocketClient *socket, wxEvtHandler *socket_handler) :
	socket(socket), socket_handler(socket_handler)
{
	socket_thread = new WxClientSocketThread(socket, socket_handler);
	if(wxTHREAD_NO_ERROR == socket_thread->Create()) // handle creation error here!
		socket_thread->Run(); // run the thread
	else
	{
		delete socket_thread;
		socket_thread = NULL;
	}
}


WxSocket::~WxSocket()
{
	if (socket_thread != NULL)
		socket_thread->Delete();
	else
		delete socket;
}


/*!
 *  \brief Sends a stream to the remote node
 *  \param data The data to be sent
 *
 *  Sends a stream to the remote node.
 */
void WxSocket::Send(const cpw::remote::DataStream &data)
{
	if (socket_thread != NULL)
	{
		WxSocketEvent evt;
		evt.SetType(sockTypeWrite);
		evt.SetData(data);
		socket_thread->Post(evt);
	}
}


/*!
 *  \brief Connects to the specified remote node
 *  \param node The identifier of the remote node
 *
 *  Connects to the specified remote node.
 */
void WxSocket::ConnectTo(const cpw::RemoteNode &node)
{
	if (socket_thread != NULL)
	{
		WxSocketEvent evt;
		evt.SetType(sockTypeNewConnection);
		evt.SetNode(node);
		socket_thread->Post(evt);
	}
}


/*!
 *  \brief Disconnects from the remote node
 *
 *  Disconnects from the remote node.
 */
void WxSocket::Disconnect()
{
	if (socket_thread != NULL)
	{
		WxSocketEvent evt;
		evt.SetType(sockTypeDisconnection);
		socket_thread->Post(evt);
	}
}


