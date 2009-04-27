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

#ifndef _REQUESTSYNCHRODATA_
#define _REQUESTSYNCHRODATA_

#include <cpw/common/pstdint.h>
#include <cpw/common/TypeId.h>

#include <remote/RemoteExport.h>
#include <remote/DataStream.h>
#include <remote/MessageData.h>

/*!
 *  \file RequestSynchroData.h
 */

namespace cpw 
{ 
	namespace remote
	{

		/*!
		 *  \class RequestSynchroData RequestSynchroData.h <remote/messages/RequestSynchroData.h>
		 *  \ingroup remote
		 *  \brief Message used to let the node know there are new changes in the entity previous to the current time
		 *
		 *  Message used to let the node know there are new changes in the entity previous to the current time.
		 *
		 *  This message is meant to be sent to all the connections after a synchronization.
		 *
		 *  Stream contents:
		 *  -  5 bytes - Control data
		 *  - 16 bytes - Message id
		 *  - 16 bytes - Entity's id
		 *  -  8 bytes - Current Time
		 *  -  8 bytes - Timestamp of last synch
		 */
		class REMOTE_EXPORT RequestSynchroData : public MessageData
		{
		public:
			RequestSynchroData();

			bool IsRequest();
			bool IsResponse();

			DataStream Code();
			bool Decode(const DataStream &_data);

			uint64_t GetLastSynchro();
			uint64_t GetCurrTime();
			const cpw::TypeId &GetEntityId();
			const cpw::TypeId &GetMessageId();

			uint64_t GetLastSynchro() const;
			uint64_t GetCurrTime() const;
			const cpw::TypeId &GetEntityId() const;
			const cpw::TypeId &GetMessageId() const;

			void SetLastSynchro(const uint64_t &ts);
			void SetCurrTime(const uint64_t &ts);
			void SetEntityId(const cpw::TypeId &id);
			void SetMessageId(const cpw::TypeId &id);


		private:
			uint64_t last_synchro;
			cpw::TypeId entity_id;
			cpw::TypeId message_id;
			uint64_t current_time;
		};

	}

}

#endif
