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

#ifndef _SETVALUEDATA_
#define _SETVALUEDATA_

#include <cpw/common/pstdint.h>
#include <cpw/common/TypeId.h>
#include <remote/RemoteExport.h>
#include <remote/DataStream.h>
#include <remote/MessageData.h>

/*!
 *  \file SetValueData.h
 */

namespace cpw 
{ 
	namespace remote
	{

		/*!
		 *  \class SetValueData SetValueData.h <remote/messages/SetValueData.h>
		 *  \ingroup remote
		 *  \brief Message used send a single change of an entity
		 *
		 *  Message used send a single change of an entity.
		 *
		 *  This message is meant to be sent to all the connections after local change.
		 *
		 *  Stream contents:
		 *  -  5 bytes - Control data
		 *  - 16 bytes -Message ID
		 *  - 16 bytes -Entity ID
		 *  -  4 bytes -Field size
		 *  -  ? bytes -Field
		 *  -  4 bytes -Value size
		 *  -  ? bytes -Value
		 *  -  8 bytes -Timestamp
		 */
		class REMOTE_EXPORT SetValueData : public MessageData
		{
		public:
			SetValueData();

			bool IsRequest();
			bool IsResponse();

			DataStream Code();
			bool Decode(const DataStream &_data);

			const std::string &GetField();
			const std::string &GetValue();
			uint64_t GetTimeStamp();
			const cpw::TypeId &GetEntityId();
			uint8_t GetSequence();
			const cpw::TypeId &GetMessageId();

			const std::string &GetField() const;
			const std::string &GetValue() const;
			uint64_t GetTimeStamp() const;
			const cpw::TypeId &GetEntityId()const;
			uint8_t GetSequence() const;
			const cpw::TypeId &GetMessageId() const;

			void SetField(const std::string &_field);
			void SetValue(const std::string &_value);
			void SetTimeStamp(const uint64_t _ts);
			void SetEntityId(const cpw::TypeId &_id);
			void SetNewSequence();
			void SetMessageId(const cpw::TypeId &_id);

		private:
			std::string field;
			std::string value;
			uint64_t ts;
			cpw::TypeId entity_id;
			static uint8_t global_sequence;
			cpw::TypeId message_id;
		};

	}

}

#endif
