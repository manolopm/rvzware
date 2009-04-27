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

#ifndef _SENDCHANGESDATA_
#define _SENDCHANGESDATA_

#include <cpw/common/pstdint.h>
#include <cpw/common/TypeId.h>
#include <cpw/common/Loggable.h>
#include <remote/RemoteExport.h>
#include <remote/DataStream.h>
#include <remote/MessageData.h>

/*!
 *  \file SendChangesData.h
 */

namespace cpw 
{ 
	namespace remote
	{
		/*!
		 *  \class SendChangesData SendChangesData.h <remote/messages/SendChangesData.h>
		 *  \ingroup remote
		 *  \brief Message used to synchronize an entity
		 *
		 *  Message used to synchronize an entity.
		 *
		 *  Stream contents:
		 *  -  5 bytes - Control data
		 *  - 16 bytes - Message Id
		 *  - 16 bytes - Entity's id
		 *  -  8 bytes - Timestamp of last change
		 *  -  4 bytes - Number of changes
		 *  -  4 bytes - Size of field string
		 *  -  ? bytes - Field string
		 *  -  4 bytes - Size of value string
		 *  -  ? bytes - Value string
		 *  -  8 bytes - Timestamp of change
		 */
		class REMOTE_EXPORT SendChangesData : public MessageData
		{
		public:
			SendChangesData();

			bool IsRequest();
			bool IsResponse();

			DataStream Code();
			bool Decode(const DataStream &_data);
			
			const cpw::TypeId &GetEntityId();
			const std::vector<cpw::Change> &GetChanges();
			uint64_t GetLastSynchro();
			const cpw::TypeId &GetMessageId();
			
			const cpw::TypeId &GetEntityId() const;
			const std::vector<cpw::Change> &GetChanges() const;
			uint64_t GetLastSynchro() const;
			const cpw::TypeId &GetMessageId() const;

			void SetEntityId(const cpw::TypeId &_id);
			void SetChanges(const std::vector<cpw::Change> &_changes);
			void SetLastSynchro(uint64_t ts);
			void SetMessageId(const cpw::TypeId &_id);

		private:
			cpw::TypeId message_id;
			cpw::TypeId entity_id;
			std::vector<cpw::Change> changes;
			uint64_t last_synchro;
		};

	}

}

#endif
