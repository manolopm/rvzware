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
#ifndef _ISOCKETFACTORY_
#define _ISOCKETFACTORY_

#include <remote/RemoteExport.h>
#include <remote/RemoteProtocol.h>
#include <remote/ISocket.h>

/*!
 *  \file ISocketFactory.h
 */

namespace cpw 
{ 
	namespace remote
	{
		class RemoteProtocol;

		/*!
		 *  \interface ISocketFactory <remote/ISocketFactory.h>
		 *  \brief Factory interface
		 *  \ingroup remote
		 *
		 *  This abstract class represents the interface of any SocketFactory implementation.
		 *
		 *  There should be a class implementing this interface for every special socket implementation
		 *  (i.e. a SocketFactory for every different socket library use)
		 */
		class REMOTE_EXPORT ISocketFactory
		{
		public:
			virtual ~ISocketFactory() { };

			/*!
			 *  \brief Creates all the objects needed for the sockets to work
			 *  \param protocol The RemoteProtocol object that is active on the system
			 *
			 *  Creates all the objects needed for the sockets to work.
			 */
			virtual void CreateHelperClasses(RemoteProtocol *protocol) = 0;

			/*!
			 *  \brief Returns a new socket object
			 *
			 *  Returns a new socket object.
			 */
			virtual ISocket *CreateSocket() = 0;

			/*!
			 *  \brief Sets the service port to listen for new connections and opens it
			 *
			 *  Sets the service port to listen for new connections and opens it.
			 */
			virtual bool ListenOn(int port) = 0;
		};

	}

}

#endif

