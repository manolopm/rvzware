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
#ifndef _GETENTITYRESPONSEDATA_
#define _GETENTITYRESPONSEDATA_

#include <string>
#include <vector>
#include <map>

#include <cpw/common/pstdint.h>
#include <cpw/common/TypeId.h>
#include "RemoteExport.h"
#include "DataStream.h"
#include "MessageData.h"

/*!
 *  \file GetEntityResponseData.h
 */

namespace cpw 
{ 
	namespace remote
	{
		/*!
		 *  \class GetEntitiesResponseData GetEntitiesResponseData.h <remote/messages/GetEntitiesResponseData.h>
		 *  \ingroup remote
		 *  \brief Message used as a response to an entity request
		 *
		 *  Message used as a response to an entity request.
		 *
		 *  Stream contents:
		 *  -    6 bytes - Control data
		 *  -   16 bytes - Entity's id
		 *  -    8 bytes - Timestamp of last synch
		 *  -    8 bytes - Current time
		 *  -    1 bytes - Wether this message contains Entity's data
		 *  -    4 bytes - Size of entity's xml
		 *  -    ? bytes - Entity's xml
		 *  -    4 bytes - Number of children
		 *  - n*16 bytes - Children ids
		 *  -    4 bytes - Size of primitive's xml
		 *  -    ? bytes - Primitive's xml
		 *  -    4 bytes - Number of binaries
		 *  -    4 bytes - Binary id
		 *  -    4 bytes - Size of binary's name
		 *  -    ? bytes - Binary's name
		 *  -    4 bytes - Size of binary
		 *  -    ? bytes - Binary data
		 */
		class REMOTE_EXPORT GetEntityResponseData : public MessageData
		{
		public:
			GetEntityResponseData();
			~GetEntityResponseData();

			bool IsRequest();
			bool IsResponse();

			DataStream Code();
			bool Decode(const DataStream &_data);

			const std::string &GetEntityXml();
			const std::vector<cpw::TypeId> &GetChildren();
			bool GetSendEntity();
			uint64_t GetLastSynchro();
			uint64_t GetCurrTime();
			const cpw::TypeId &GetEntityId();

			const std::string &GetEntityXml() const;
			const std::vector<cpw::TypeId> &GetChildren() const;
			bool GetSendEntity() const;
			uint64_t GetLastSynchro() const;
			uint64_t GetCurrTime() const;
			const cpw::TypeId &GetEntityId() const;

			void SetEntityXml(const std::string &entity);
			//void SetChildren(const std::vector<cpw::TypeId> &ids);
			void SetSendEntity(bool send);
			void SetLastSynchro(const uint64_t &ts);
			void SetCurrTime(const uint64_t &ts);
			void SetEntityId(const cpw::TypeId &id);

		private:
			void GetDataFromEntity(bool primitive,
				std::string &primitive_xml,
				std::map<std::string, uint32_t> &binaries_url,
				std::map<uint32_t, DataStream> &binaries_data,
				uint32_t &binary_last_id);

			void ChangeIdsWithUrl(std::string &xml_string,
				std::map<uint32_t, std::string> &binaries_url);

			cpw::TypeId entity_id;
			std::string entity_xml;
			std::vector<cpw::TypeId> children_ids;
			uint64_t last_synchro;
			uint64_t current_time;
			//std::string root_folder;
			bool send;
			static std::vector<std::string> tags;
		};

	}

}


#endif
