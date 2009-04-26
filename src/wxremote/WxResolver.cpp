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

#include <string>
#include <wx/socket.h>

#include <wxremote/stdafx.h>

#include <cpw/common/IResolver.h>

#include <wxremote/WxResolver.h>


using namespace cpw::wxremote;



/*!
 *  \brief Returns an string with the IP address
 *  \param hostname The string to be resolved
 *
 *  Returns an string with the IP address. If the resolution is not possible, 
 *  it returns the string "0.0.0.0"
 */
std::string WxResolver::Resolve(const std::string &hostname)
{
	wxIPV4address address;
	address.Hostname(wxString::From8BitData(hostname.c_str()));
	return std::string(address.IPAddress().To8BitData());
}


/*!
 *  \brief Returns an string with the IP address
 *  \param hostname The string to be resolved
 *
 *  Returns an string with the IP address. If the resolution is not possible, 
 *  it returns the string "0.0.0.0"
 */
std::string WxResolver::Resolve(const std::string &hostname) const
{
	wxIPV4address address;
	address.Hostname(wxString::From8BitData(hostname.c_str()));
	return std::string(address.IPAddress().To8BitData());
}
