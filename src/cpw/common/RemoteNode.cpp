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

#include <stdlib.h>
#include <string>

//#include <stdafx.h>

#include <cpw/common/RemoteNode.h>


using namespace cpw;


IResolver *RemoteNode::resolver = NULL;


/*!
 *  \brief Default constructor.
 *
 *  Sets the Hostname as an empty string ("") and the IP Address as "0.0.0.0".
 */
RemoteNode::RemoteNode()
	: hostname(std::string()), ipaddress(std::string("0.0.0.0")), port(-1) {}


/*!
 *  \brief Constructor.
 *  \param hostname The hostname to be resolved (so it accepts IP o Hostname)
 *  \param port The service port
 *
 *  Special constructor which sets the properties as specified on the parameters.
 */
RemoteNode::RemoteNode(const std::string &hostname, int port)
	: hostname(hostname), port(port)
{
	ipaddress = resolver->Resolve(hostname);
}


/*!
 *  \brief Equal to operator
 *
 *  Equal to operator.
 *
 *  Compares the IP address and the the service port, ignoring the hostname specified on the constructor.
 */
bool RemoteNode::operator==(const RemoteNode &n2) const
{
	if (ipaddress == n2.ipaddress)
	{
		if (port == -1 || n2.port == -1) //-1 means any port, just check the IP
			return true;
		if (port == n2.port)
			return true;
	}

	return false;
}


/*!
 *  \brief Not equal to operator
 *
 *  Not equal to operator.
 *
 *  Compares the IP address and the the service port, ignoring the hostname specified on the constructor.
 */
bool RemoteNode::operator!=(const RemoteNode &n2) const
{
	return !((*this) == n2);
}


/*!
 *  \brief Less than operator
 *
 *  Less than operator.
 *
 *  Sets order according to IP address (alphabetical) and then by service port.
 */
bool RemoteNode::operator<(const RemoteNode &n2) const
{
	if (ipaddress == n2.ipaddress)
	{
		if (port < n2.GetPort())
			return true;
		else
			return false;
	}
	else
	{
		if (ipaddress < n2.ipaddress)
			return true;
		else
			return false;
	}
}


/*!
 *  \brief Returns the service port
 *
 *   Returns the service port.
 */
int RemoteNode::GetPort()
{
	return port;
}


/*!
 *  \brief Returns the Hostname
 *
 *  Returns the Hostname.
 */
const std::string &RemoteNode::GetHostname()
{
	return hostname;
}


/*!
 *  \brief Returns the IP address
 *
 *  Returns the IP address.
 */
const std::string &RemoteNode::GetIPAddress()
{
	return ipaddress;
}


/*!
 *  \brief Returns the service port
 *
 *  Returns the service port.
 */
int RemoteNode::GetPort() const
{
	return port;
}


/*!
 *  \brief Returns the Hostname
 *
 *  Returns the Hostname.
 */
const std::string &RemoteNode::GetHostname() const
{
	return hostname;
}


/*!
 *  \brief Returns the IP address
 *
 *  Returns the IP address.
 */
const std::string &RemoteNode::GetIPAddress() const
{
	return ipaddress;
}


/*!
 *  \brief Sets the service port
 *
 *  \param port Service port to be used
 *
 *  Sets the service port.
 */
void RemoteNode::SetPort(int port)
{
	this->port = port;
}


/*!
 *  \brief Sets the Hostname
 *  \param hostname Hostname to be used. Will be resolved, son it can be IP o DNS name
 *
 *  Sets the Hostname.
 *
 *  Sets the hostname and resolves the name setting the IP address.
 */
void RemoteNode::SetHostname(const std::string &hostname)
{
	this->hostname = hostname;
	ipaddress = resolver->Resolve(hostname);
}


/*!
 *  \brief Sets the IP address
 *  \param ipaddress The IP address to be set.
 *
 *  Sets the IP address.
 *
 *  This method doesn't set the hostname.
 */
void RemoteNode::SetIPAddress(const std::string &ipaddress)
{
	this->ipaddress = ipaddress;
}


/*!
 *  \brief Sets the resolver object
 *  \param inresolver The resolver to be used
 *
 *  Sets the resolver object.
 *
 *  The resolver is a static member, so it should be set at the beginning only.
 */
void RemoteNode::SetResolver(IResolver *inresolver)
{
	resolver = inresolver;
}










/*!
 *  \brief Returns the MAC address
 */
/*const std::string &RemoteNode::GetMACAddress()
{
	return macaddress;
}*/


/*!
 *  \brief Returns the MAC address
 */
/*const std::string &RemoteNode::GetMACAddress() const
{
	return macaddress;
}*/
/*!
 *  \brief Sets the MAC address
 */
/*void RemoteNode::SetMACAddress(const std::string &_macaddress)
{
	macaddress = _macaddress;
}*/

/*bool HostnameIsIP(const std::string &_hostname)
{
	std::string numbers = "0123456789";
	std::string hosttemp = _hostname;
	int octets = 0;
	while ((hosttemp.find(std::string(".")) != -1) && octets < 4)
	{
		std::string octet;
		octet = hosttemp.substr(0,hosttemp.find(std::string(".")));
		hosttemp = hosttemp.substr(octet.size()+1);
		if (octet.size() <= 3) //max integer 255
		{
			std::string::iterator it;
			int value = 0;
			int multiplier = 0;
			for (it = octet.begin(); it != octet.end(); it++)
			{
				if (numbers.find(*it) != -1)
				{

					value += numbers.find(*it) * multiplier;
					multiplier *= 10;
				}
				else
					break;
			}
			if (it == octet.end())
			{
				if (value<=255)
					octets++;
			}
		}
	}

	if (octets == 4)
		return true;
	else
		return false;	
}*/
