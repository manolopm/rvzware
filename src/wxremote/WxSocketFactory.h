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

#ifndef _WXSOCKETFACTORY_
#define _WXSOCKETFACTORY_

#include <wx/event.h>
#include <wx/socket.h>

#include <remote/ISocketFactory.h>
#include <remote/RemoteProtocol.h>
#include <remote/ISocket.h>
#include "WxRemoteExport.h"
#include "WxSocketHandler.h"


/*!
 *  \namespace cpw { namespace wxremote \brief wxWidget implementation of Remote
 *  \defgroup wxremote wxWidget implementation of Remote
 */

/*!
 *  \file WxSocketFactory.h
 */

namespace cpw 
{ 
	namespace wxremote
	{
		/*!
		 *  \class WxSocketFactory WxSocketFactory.h <wxremote/WxSocketFactory.h>
		 *  \ingroup wxremote
		 *  \brief wxWidgets socket factory specialization
		 *
		 *  wxWidgets socket factory specialization.
		 */
		class WxSocketFactory : public cpw::remote::ISocketFactory
		{
		public:
			WxSocketFactory(wxEvtHandler *handler);
			~WxSocketFactory();

			void CreateHelperClasses(cpw::remote::RemoteProtocol *protocol);
			cpw::remote::ISocket *CreateSocket();
			cpw::remote::ISocket *CreateSocket(wxSocketClient *socket);

			bool ListenOn(int port);

		private:
			WxSocketHandler *socket_handler;
			wxEvtHandler *parent_handler;
		};
	}

}

#endif

