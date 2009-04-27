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

#ifndef _MESSAGEDATA_
#define _MESSAGEDATA_

#include <remote/RemoteExport.h>
#include <remote/DataStream.h>

/*!
 *  \file MessageData.h
 */

namespace cpw 
{ 
	namespace remote
	{
		/*!
		 *  \ingroup remote
		 *
		 *  Defines all the message types available
		 */
		enum REMOTE_EXPORT MessageType {
			msgTypeConnection = 1, /*!< Connection message. Special message used in callback methods */
			msgTypeDisconnection = 2, /*!< Disconnection message. Special message used in callback methods */
			msgTypeGetPublishedRequest = 3, /*!< GetPublishedRequest message */
			msgTypeGetPublishedResponse = 4, /*!< GetPublishedResponse message */
			msgTypeSetValue = 5, /*!< SetValue message */
			msgTypeGetEntityRequest = 6, /*!< GetEntityRequest message */
			msgTypeGetEntityResponse = 7, /*!< GetEntityRequest message */
			msgTypeSendChanges = 8, /*!< SendChanges message */
			msgTypeRequestSynchro = 9, /*!< RequestSynchro message */
			msgTypeDisconnectEntity = 10 /*!< DisconnectEntity message */
		};


		/*!
		 *  \class MessageData MessageData.h <remote/messages/MessageData.h>
		 *  \brief Message hierarchy base clase
		 *  \ingroup remote
		 *
		 *  Message hierarchy base clase.
		 *
		 *  This is an abstract class so you should instantiate a derived class instead.
		 *
		 *  To create a new message, it is necessary to add a new value to the MessageType enum.
		 */
		class REMOTE_EXPORT MessageData
		{
		public:
			virtual ~MessageData() { }

			const MessageType &GetMessageType();
			const uint8_t &GetRequestID();

			const MessageType &GetMessageType() const;
			const uint8_t &GetRequestID() const;

			void SetMessageType(MessageType id);
			void SetRequestID(uint8_t id);

			/*!
			 *  \brief Tells wheter the message is a request or not
			 *
			 *  Tells wheter the message is a request or not.
			 *
			 *  This method should be overridden in derived classes.
			 */
			virtual bool IsRequest() = 0;

			/*!
			 *  \brief Tells whether the message is a response or not
			 *
			 *  Tells whether the message is a response or not.
			 *
			 *  This method should be overridden in derived classes.
			 */
			virtual bool IsResponse() = 0;



			/*!
			 *  \brief Returns a DataStream with all the information of the stream
			 *
			 *  Returns a DataStream with all the information of the stream, ready to be sent throught the network.
			 *
			 *  This method should be overridden in derived classes.
			 */
			virtual DataStream Code() = 0;

			/*!
			 *  \brief Creates the message from a DataStream
			 *  \param data The data to be used to compose the message
			 *
			 *  Creates the message from a DataStream.
			 *
			 *  Returns a boolean that should be true if the process is successful.
			 *
			 *  This method should be overridden in derived classes.
			 */
			virtual bool Decode(const DataStream &data) = 0;


		protected:
			MessageType message_type;
			uint8_t request_id;
		};

	}

}

#endif

