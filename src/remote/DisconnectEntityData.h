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
#ifndef _DISCONNECTENTITYDATA_
#define _DISCONNECTENTITYDATA_

#include <cpw/common/TypeId.h>
#include <remote/RemoteExport.h>
#include <remote/DataStream.h>
#include <remote/MessageData.h>

/*!
 *  \file DisconnectEntityData.h
 */

namespace cpw 
{ 
	namespace remote
	{
		/*!
		 *  \class DisconnectEntityData DisconnectEntityData.h <remote/messages/DisconnectEntityData.h>
		 *  \ingroup remote
		 *  \brief Message used to disconnect from an entity
		 *
		 *  Message used to disconnect from an entity.
		 *
 		 *  Stream contents:
		 *  -  5 bytes - Control data
		 *  - 16 bytes - Entity's id
		 */
		class REMOTE_EXPORT DisconnectEntityData : public MessageData
		{
		public:
			DisconnectEntityData();

			bool IsRequest();
			bool IsResponse();

			DataStream Code();
			bool Decode(const DataStream &_data);

			const cpw::TypeId &GetEntityId();
			
			const cpw::TypeId &GetEntityId() const;

			void SetEntityId(const cpw::TypeId &id);

		private:
			cpw::TypeId entity_id;
		};

	}

}

#endif
