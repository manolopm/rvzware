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
#ifndef _WXSOCKETEVENT_
#define _WXSOCKETEVENT_

#include <wx/event.h>
#include <wx/socket.h>

#include <cpw/common/RemoteNode.h>
#include <remote/DataStream.h>

/*!
 *  \file WxSocketEvent.h
 */

namespace cpw 
{ 
	namespace wxremote
	{
		/*!
		 *  \ingroup wxremote
		 *
		 *  Defines the types of events from the thread
		 */
		enum WxSocketEventType
		{
			sockTypeNewConnection = 1, /*!< The event represents a new connection */
			sockTypeDisconnection = 2, /*!< The event represents a disconnection */
			sockTypeRead = 3, /*!< The event contains a new message that has been read from the socket */
			sockTypeWrite = 4 /*!< The event contains data to be written on the socket */
		};

		DECLARE_EVENT_TYPE(WxSocketThreadEvent, -1)

		/*!
		 *  \class WxSocketEvent WxSocketEvent.h <wxremote/WxSocketEvent.h>
		 *  \ingroup wxremote
		 *  \brief This is the class used to communicate information between threads
		 *
		 *  This is the class used to communicate information between threads.
		 */
		class WxSocketEvent : public wxCommandEvent
		{
		public:
			WxSocketEvent(wxEventType commandType = WxSocketThreadEvent, int id = 0);
			WxSocketEvent(const WxSocketEvent &_event);


			WxSocketEvent &operator =(const WxSocketEvent &_event);
			
			// required for sending with wxPostEvent()
			wxEvent* Clone() const;

			// accessors
			const WxSocketEventType &GetType();
			const cpw::remote::DataStream &GetData();
			const cpw::RemoteNode &GetNode();
			
			const WxSocketEventType &GetType() const;
			const cpw::remote::DataStream &GetData() const;
			const cpw::RemoteNode &GetNode() const;

			void SetType(const WxSocketEventType _type);
			void SetData(const cpw::remote::DataStream &_data);
			void SetNode(const cpw::RemoteNode &_node);

		private:
			WxSocketEventType type;
			cpw::remote::DataStream data;
			cpw::RemoteNode node;
		};

		typedef void (wxEvtHandler::*WxSocketEventFunction)(WxSocketEvent&);

		#define EVT_SOCKET_THREAD(id, fn) \
			DECLARE_EVENT_TABLE_ENTRY(WxSocketThreadEvent, id, -1, \
			(wxObjectEventFunction) (wxEventFunction) (wxCommandEventFunction) \
			wxStaticCastEvent( WxSocketEventFunction, & fn ), (wxObject *) NULL ),

	}
}

#endif
