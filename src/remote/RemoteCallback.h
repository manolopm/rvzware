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
#ifndef _REMOTECALLBACK_
#define _REMOTECALLBACK_

#include <remote/RemoteExport.h>
#include <remote/IRemoteCallback.h>
#include <remote/Connection.h>
#include <remote/MessageData.h>

/*!
 *  \file RemoteCallback.h
 */

namespace cpw 
{ 
	namespace remote
	{
		/*!
		 *  \class RemoteCallback RemoteCallback.h <remote/utils/RemoteCallback.h>
		 *  \brief This class is a functor implementing a callback
		 *  \ingroup remote
		 *
		 *  This class is a functor implementing a callback.
		 *
		 *  It is a template class so it can be used to call any object method, provided the method has 
		 *  the same signature of the operator(), as defined in IRemoteCallback.
		 *
		 *  Usage:
		 *	 - Create - RemoteCallback<Class> object_name(&reciever_object, &RecieverClass::CallbackMethod);
		 *	 - Call   - (*object_name)(connection, data);
		 */
		template <class Class>
		class RemoteCallback : public IRemoteCallback
		{
		public:
			/*!
			 *  \param pt2Object Pointer to an object of type Class
			 *  \param fpt Pointer to a member of class "Class"
			 */
			RemoteCallback(Class* pt2Object, void(Class::*fpt)(const Connection*, const MessageData*))
			  :pt2Object(pt2Object), fpt(fpt) {}

			/*!
			 *  \brief Calls the method on the object stored in the object
			 *  \param connection The Connection triggering the event
			 *  \param data The message involved
			 *
			 *  Calls the method on the object stored in the object.
			 */
			virtual void operator()(const Connection* connection, const MessageData *data)
			{
				(*pt2Object.*fpt)(connection, data);  // execute member function
			}

		protected:
			void (Class::*fpt)(const Connection*, const MessageData*);
			Class* pt2Object;
		};
	}
}

#endif

