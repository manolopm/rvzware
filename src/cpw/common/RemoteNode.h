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

#ifndef _REMOTENODE_
#define _REMOTENODE_

#include <string>

#include <cpw/Export.h>
#include <cpw/common/IResolver.h>

/*!
 *  \file RemoteNode.h
 */

namespace cpw 
{
	/*!
	 *  \class RemoteNode RemoteNode.h <cpw/RemoteNode.h>
	 *  \ingroup cpw
	 *  \brief Class representing a remote node identifier in TCP/IP terminology
	 *
	 *  The properties are: Hostname and IP address to identify the node
	 *  and port to identify the service.
	 *
	 *  Hostname should be used to set the info provided by the user (previous to name resolution) or
	 *  the IP Address if the RemoteNode is created by the system (after address resolution).
	 *
	 *  IP address should be used together with Port as identifier during a live connection.
	 *
	 */
	class CPWEXPORT RemoteNode
	{
	public:
		RemoteNode();
		RemoteNode(const std::string &_hostname, int port);

		bool operator==(const RemoteNode &n2) const;
		bool operator!=(const RemoteNode &n2) const;
		bool operator<(const RemoteNode &n2) const;

		int GetPort();
		const std::string &GetHostname();
		const std::string &GetIPAddress();

		int GetPort() const;
		const std::string &GetHostname() const;
		const std::string &GetIPAddress() const;

		void SetPort(int _port);
		void SetHostname(const std::string &_hostname);
		void SetIPAddress(const std::string &_ipaddress);

		static void SetResolver(IResolver *_resolver);

	private:
		std::string hostname;
		std::string ipaddress;
		int port;

		static IResolver *resolver;
	};
}


#endif


		//void SetMACAddress(const std::string &_macaddress);
		//const std::string &GetMACAddress() const;
		//const std::string &GetMACAddress();






