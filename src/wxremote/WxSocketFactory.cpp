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

#include <wxremote/stdafx.h>

#include <remote/ISocketFactory.h>
#include <remote/ISocket.h>
#include <remote/RemoteProtocol.h>

#include <wxremote/WxSocket.h>
#include <wxremote/WxSocketFactory.h>



using namespace cpw::wxremote;



/*!
 *  \brief This constructor requires an object to be used as a link to the main thread
 *  \param handler A pointer to an event handler in the main thread
 *
 *  This constructor requires an object to be used as a link to the main thread.
 */
WxSocketFactory::WxSocketFactory(wxEvtHandler *handler)
  : socket_handler(NULL), parent_handler(handler) { }


WxSocketFactory::~WxSocketFactory()
{
  if (socket_handler != NULL)
    {
      delete socket_handler;
      socket_handler = NULL;
    }
}


/*!
 *  \brief Creates all the objects needed for the sockets to work
 *  \param protocol The RemoteProtocol object that is active on the system
 *
 *  Creates all the objects needed for the sockets to work.
 */
void WxSocketFactory::CreateHelperClasses(cpw::remote::RemoteProtocol *protocol)
{
  socket_handler = new WxSocketHandler(protocol);
}


/*!
 *  \brief Returns a new socket object
 *
 *  Returns a new socket object.
 */
cpw::remote::ISocket *WxSocketFactory::CreateSocket()
{
  WxSocket *wx_socket = new WxSocket(new wxSocketClient(), socket_handler);

  return wx_socket;
}


/*!
 *  \fn cpw::remote::ISocket WxSocketFactory::CreateSocket()
 *  \brief 
 */
cpw::remote::ISocket *WxSocketFactory::CreateSocket(wxSocketClient *socket)
{
  WxSocket *wx_socket = new WxSocket(socket, socket_handler);

  return wx_socket;
}


/*!
 *  \brief Sets the service port to listen for new connections and opens it
 *
 *  Sets the service port to listen for new connections and opens it.
 */
bool WxSocketFactory::ListenOn(int port)
{
  return socket_handler->ListenOn(port);
}

