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
#include <wx/socket.h>
#include <wx/wx.h>

#include <wxremote/stdafx.h>

#include <cpw/common/RemoteNode.h>
#include <remote/RemoteProtocol.h>
#include <remote/DataStream.h>

#include <wxremote/WxSocket.h>
#include <wxremote/WxSocketEvent.h>
#include <wxremote/WxSocketHandler.h>


using namespace cpw::wxremote;



BEGIN_EVENT_TABLE(WxSocketHandler, wxEvtHandler)
EVT_SOCKET(SERVER_ID,  WxSocketHandler::OnServerEvent)
EVT_SOCKET_THREAD(wxID_ANY,  WxSocketHandler::OnClientEvent)
END_EVENT_TABLE()


/*!
 *  \param protocol The RemoteProtocol object to use when an event is received
 */
WxSocketHandler::WxSocketHandler(cpw::remote::RemoteProtocol *protocol)
:wxEvtHandler(), listen_socket(NULL), protocol(protocol)
{
  //wxSocketBase::Initialize();
  //wxInitialize();
}


WxSocketHandler::~WxSocketHandler()
{
  if (listen_socket != NULL)
    listen_socket->Destroy();
}


/*!
 *  \brief Opens the specified port and starts listening
 *  \param port port number on which it is listening
 *
 *  Opens the specified port and starts listening.
 */
bool WxSocketHandler::ListenOn(int port)
{
  if (listen_socket != NULL)
    {
      listen_socket->Destroy();
    }

  //Create an address object
  wxIPV4address addr;
  addr.Service(port);

  listen_socket = new wxSocketServer(addr);

  //Check the socket is listening
  if (! listen_socket->Ok())
    return false;

  //Activate event notification so the corresponding method is called
  //when a new incoming connection is established
  listen_socket->SetEventHandler(*(wxEvtHandler *)this, SERVER_ID);
  listen_socket->SetNotify(wxSOCKET_CONNECTION_FLAG);
  listen_socket->Notify(true);

  return true;
}




//---------------------------------------------------------------------------
//---------------------------    EVENTS      --------------------------------
//---------------------------------------------------------------------------


/*!
 *  \brief New incoming connection event
 *  \param event The event
 *
 *  New incoming connection event.
 */
void WxSocketHandler::OnServerEvent(wxSocketEvent& event)
{
  if (event.GetSocketEvent() != wxSOCKET_CONNECTION)
    {
      //Nos aseguramos de que el evento sea de conexión
      return;
    }

  //Aceptamos la conexión, con lo que obtenemos el socket para la comunicación
  wxSocketBase *sock = listen_socket->Accept(false);

  if (sock)
    {
      //Si al aceptar hay algún error, salimos
      delete sock;
      sock = NULL;
      return;
    }

  wxIPV4address addr;
  std::string hostname;
  int port;

  //Obtenemos la dirección del host remoto, para actualizar la interfaz
  if	(sock->GetPeer(addr)) {
    hostname = addr.IPAddress().To8BitData();
    port = addr.Service();
  }

  WxSocket *socket = new WxSocket(((wxSocketClient *)sock), this); 
  protocol->RecvNewConnection(cpw::RemoteNode(hostname,port), socket);
}



/*!
 *  \brief Thread events handler method
 *  \param event The event
 *
 *  Thread events handler method.
 */
void WxSocketHandler::OnClientEvent(WxSocketEvent &event)
{
  switch(event.GetType())
    {
    case sockTypeRead:
      {
	protocol->RecvMessage(event.GetNode(), event.GetData());
	break;
      }
    case sockTypeDisconnection:
      {
	protocol->RecvDisconnection(event.GetNode());
	break;
      }
    case sockTypeNewConnection:
      {
	protocol->RecvNewConnection(event.GetNode());
	break;
      }
    default:
      return;
    }
}

