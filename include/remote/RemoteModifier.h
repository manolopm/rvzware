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
#ifndef _REMOTEMODIFIER_
#define _REMOTEMODIFIER_

#include <string>

#include <cpw/common/pstdint.h>
#include <cpw/common/Observer.h>
#include <cpw/persistent/Persistent.h>

#include "RemoteExport.h"
#include "RemoteProtocol.h"


/*!
 *  \file RemoteModifier.h
 */

namespace cpw 
{ 
	namespace remote
	{
		class RemoteProtocol;

		/*!
		 *  \class RemoteModifier RemoteModifier.h <remote/RemoteModifier.h>
		 *  \brief Observer to an entity shared with a remote node
		 *  \ingroup remote
		 *
		 *  Observer to an entity shared with a remote node.
		 *
		 *  This class is used to received modification events on an Entity, so the changes can be sent
		 *  other nodes inmediatly.
		 *
		 *  There should be an object of this class for every Entity shared with at least one connection.
		 */
		class REMOTE_EXPORT RemoteModifier : public cpw::Observer
		{
		public:
			RemoteModifier(cpw::Persistent *_persistent, RemoteProtocol *_protocol);
			virtual ~RemoteModifier();

			void SetExecuteNext(bool e);

			void Update(bool subject_deleted = false);

			void SetValue(const std::string &field, const std::string &value, const uint64_t ts);

		private:
			cpw::Persistent *subject;
			RemoteProtocol *protocol;
			bool execute_next_update;
		};
	}
}

#endif
