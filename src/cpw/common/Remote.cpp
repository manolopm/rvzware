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
#include <map>
#include <vector>
#include <string>
#include <ctime>
#include <sstream>

//#include <stdafx.h>

#include <cpw/common/Sorted.h>
#include <cpw/common/RemoteNode.h>
#include <cpw/common/pstdint.h>
#include <cpw/common/Remote.h>


using namespace cpw;


/*!
 *  \fn Remote::Remote(const cpw::TypeId id, const std::string &class_name)
 *  \brief 
 */
Remote::Remote(const cpw::TypeId id, const std::string &class_name)
	: Sorted(id, class_name), synchs()
{

}


/*!
 *  \fn Remote::Remote(const Remote &remote)
 *  \brief 
 */
Remote::Remote(const Remote &remote)
	: Sorted(remote), synchs(remote.synchs)
{

}


/*!
 *  \fn Remote::~Remote(void)
 *  \brief 
 */
Remote::~Remote(void)
{

}


/*!
 *  \fn Remote &Remote::operator= (const Remote &remote)
 *  \brief 
 */
Remote &Remote::operator= (const Remote &remote)
{
	Sorted::operator= (remote);

	synchs = remote.synchs;

	return *this;
}


/*!
 *  \fn time_t Remote::GetSynchronized(const RemoteNode &node)
 *  \brief 
 */
time_t Remote::GetLastSynch(const RemoteNode &node)
{
	std::map<RemoteNode, time_t>::iterator i;
	for (i = synchs.begin(); i != synchs.end(); i++)
	{
		if (i->first.GetIPAddress() == node.GetIPAddress())
			return i->second;
	}

	return 0;
}


/*!
 *  \fn void Remote::SetSynchronized(const RemoteNode &node, time_t last_synch)
 *  \brief 
 */
void Remote::SetLastSynch(const RemoteNode &node, time_t last_synch)
{
	if (synchs.find(node) != synchs.end())
	{
		synchs[node] = last_synch;
		//Persistent::Modified();
	}
	else
	{
		std::map<RemoteNode, time_t>::iterator i = synchs.begin();
		while (i != synchs.end())
		{
			if (i->first.GetIPAddress() == node.GetIPAddress())
			{
				synchs.erase(i);
				synchs[node] = last_synch;
				bool registerc = ((cpw::Loggable*)this)->GetRegisterChanges();
				((cpw::Loggable*)this)->SetRegisterChanges(false);
				Persistent::Modified();
				((cpw::Loggable*)this)->SetRegisterChanges(registerc);
				return;
			}

			i++;
		}

		if (i == synchs.end())
		{
			synchs[node] = last_synch;
			bool registerc = ((cpw::Loggable*)this)->GetRegisterChanges();
			((cpw::Loggable*)this)->SetRegisterChanges(false);
			Persistent::Modified();
			((cpw::Loggable*)this)->SetRegisterChanges(registerc);
		}
	}
}


/*!
 *  \fn std::vector<RemoteNode> Remote::GetNodes()
 *  \brief 
 */
std::vector<RemoteNode> Remote::GetNodes()
{
	std::vector<RemoteNode> nodes;

	std::map<RemoteNode, time_t>::iterator i;
	for (i = synchs.begin(); i != synchs.end(); i++)
		nodes.push_back(i->first);

	return nodes;
}


/*!
 *  \fn int Remote::CreatePersistence()
 *  \brief 
 *
 *  Example of persistence (xml):
 *  ...
 *  <synchs>
 *      <synch>
 *          RemoteNode1.ipaddress time_t1
 *      </synch>
 *      <synch>
 *          RemoteNode2.ipaddress time_t2
 *      </synch>
 *  </synchs>
 *  ...
 */
int Remote::CreatePersistence()
{
	Sorted::CreatePersistence();

	AddPersistenceLevel((std::string)"synchs");

	//Aggregate the class attributes to the persistence
	std::map<RemoteNode, time_t>::iterator i;
	for (i = synchs.begin(); i != synchs.end(); i++) 
	{
		std::ostringstream str;
		str << i->first.GetIPAddress() << " " << i->first.GetPort() << " " << i->second;
		AddPersistence((std::string)"synch", str.str());
	}

	RemovePersistenceLevel();

	return 0;
}


/*!
 *  \fn int Remote::AdaptPersistence(Node *root)
 *  \brief 
 */
int Remote::AdaptPersistence(Node *root)
{
	Sorted::AdaptPersistence(root);

	std::vector<cpw::Node *> components = root->GetChildren();
	std::vector<cpw::Node *>::iterator i = components.begin();
	
	while ((i != components.end()) && ((*i)->GetName() != std::string("synchs"))) i++;

	synchs.clear();
	if (i != components.end())
	{
		std::vector<cpw::Node *> syncs = (*i)->GetChildren();
		std::vector<cpw::Node *>::iterator j = syncs.begin();
		for(j;j!=syncs.end();j++)
		{
			time_t dt;
			std::string node_ip;
			int port;
			std::istringstream str((*j)->GetValue());
			str >> node_ip >> port >> dt;
			RemoteNode node;
			node.SetIPAddress(node_ip);
			node.SetPort(port);
			synchs[node] = dt;
		}
	}

	return 0;
}
