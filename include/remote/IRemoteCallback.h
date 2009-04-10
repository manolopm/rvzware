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
#ifndef _IREMOTECALLBACK_
#define _IREMOTECALLBACK_


#include "RemoteExport.h"
#include "Connection.h"
#include "MessageData.h"

/*!
 *  \file IRemoteCallback.h
 */

namespace cpw 
{ 
	namespace remote
	{
		/*!
		 *  \interface IRemoteCallback IRemoteCallback.h <remote/utils/IRemoteCallback.h>
		 *  \brief Remote callback interface
		 *  \ingroup remote
		 *
		 *  This abstract class is used to register functors of type RemoteCallback in the 
		 *  remote package.
		 */
		class REMOTE_EXPORT IRemoteCallback
		{
		public:
			virtual ~IRemoteCallback() { }

			/*!
			 *  \brief Calls the method on the object stored in the object
			 *  \param connection The Connection triggering the event
			 *  \param msg_data The message involved
			 *
			 *  Calls the method on the object stored in the object.
			 */
			virtual void operator()(const Connection *connection, const MessageData *msg_data) = 0;

		};

	}

}


#endif

