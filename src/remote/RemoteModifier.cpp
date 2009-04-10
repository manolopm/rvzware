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
#include <string>

#include "stdafx.h"

#include <cpw/common/IdGenerator.h>
#include <cpw/common/pstdint.h>
#include <cpw/common/Observer.h>
#include <cpw/common/Loggable.h>
#include <cpw/persistent/Persistent.h>
#include <cpw/entity/Entity.h>

#include <remote/RemoteProtocol.h>
#include <remote/RemoteModifier.h>


using namespace cpw::remote;




/*!
 *  \param persistent The persistent object to be observed
 *  \param protocol The RemoteProtocol object that is active on the system
 */
RemoteModifier::RemoteModifier(cpw::Persistent *persistent, RemoteProtocol *protocol)
	: subject(persistent), protocol(protocol), execute_next_update(true)
{
	subject->Attach(this);
}


RemoteModifier::~RemoteModifier()
{
	if (subject != NULL)
		subject->Detach(this);
}


/*!
 *  \brief Enables/Disables successive updates
 *  \param enable Specifies whether the object should be enabled or disabled
 *
 *  Enables/Disables successive updates.
 *
 *  This method sets an internal variable that makes the object to ignore (enable = false) or respond
 *  to Update methods (enable = true). This is useful when processing remote messages that changes
 *  the value of an entity and you don't want to send the changes to other nodes yet.
 *
 *  The object is enabled by default.
 */
void RemoteModifier::SetExecuteNext(bool enable)
{
	execute_next_update = enable;
}


/*!
 *  \brief Method called whenever the entity is changed
 *  \param subject_deleted When true, indicates that the entity is being deleted
 *
 *  Method called whenever the entity is changed.
 *
 *  This is an overloaded method from Observer and is called when the observed entity calls the
 *  Notify method
 */
void RemoteModifier::Update(bool subject_deleted)
{
	if (subject_deleted)
	{
		protocol->EntityRemoved(((cpw::Entity *) subject)->GetId());
		subject = NULL;
	}
	else
	{
		if (execute_next_update)
		{
			cpw::Change change = ((cpw::Loggable*)subject)->GetLastChange();
			if (change.field != "")
				protocol->SendChange(subject->GetId(), change);
		}
	}
}


/*!
 *  \brief Sets the value of a field in the observed entity
 *  \param field Field to be updated
 *  \param value Value to be set to the field
 *  \param ts Timestamp of the change
 *
 *  Sets the value of a field in the observed entity.
 *
 *  This method is meant to be used when a change is received from a remote node.
 */
void RemoteModifier::SetValue(const std::string &field, const std::string &value, const uint64_t ts)
{
	if (subject != NULL)
	{
		execute_next_update = false;
		((cpw::Loggable*)subject)->SetRegisterChanges(false);

		if (((cpw::Loggable*)subject)->AddChange(cpw::Change(field, value, ts), false))
			subject->SetValue(field, value);
		
		//((cpw::Entity *)subject)->GraphicDelete();
		//((cpw::Entity *)subject)->GraphicInsert();
		((cpw::Entity *)subject)->GraphicUpdate();

		((cpw::Loggable*)subject)->SetRegisterChanges(true);
		execute_next_update = true;
	}
}