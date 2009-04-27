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
#ifndef _GETENTITYREQUESTDATA_
#define _GETENTITYREQUESTDATA_

#include <vector>
#include <ctime>

#include <cpw/common/pstdint.h>
#include <cpw/common/TypeId.h>
#include <remote/RemoteExport.h>
#include <remote/DataStream.h>
#include <remote/MessageData.h>

/*!
 *  \file GetEntityRequestData.h
 */

namespace cpw 
{ 
	namespace remote
	{
		/*!
		 *  \class GetEntityRequestData GetEntityRequestData.h <remote/messages/GetEntityRequestData.h>
		 *  \ingroup remote
		 *  \brief Message used to request an entity
		 *
		 *  Message used to request an entity.
		 *
		 *  Stream contents:
		 *   -  6 bytes - Control data
		 *   - 16 bytes - Entity's id
		 *   -  1 bytes - Send entity
		 */
		class REMOTE_EXPORT GetEntityRequestData : public MessageData
		{
		public:
			GetEntityRequestData();

			bool IsRequest();
			bool IsResponse();

			DataStream Code();
			bool Decode(const DataStream &_data);

			const cpw::TypeId &GetEntityId();
			bool GetSendEntity();
			
			const cpw::TypeId &GetEntityId() const;
			bool GetSendEntity() const;

			void SetEntityId(const cpw::TypeId &id);
			void SetSendEntity(bool send);

		private:
			cpw::TypeId entity_id;
			bool send;
		};

	}

}
#endif
