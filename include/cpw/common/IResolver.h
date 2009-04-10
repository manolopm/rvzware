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
#ifndef _IRESOLVER_
#define _IRESOLVER_

#include <string>


/*!
 *  \file IResolver.h
 */

namespace cpw
{
	/*!
	 *  \class IResolver IResolver.h <cpw/IResolver.h>
	 *  \ingroup cpw
	 *  \brief Resolver interface
	 *
	 *  This abstract class represents the interface of a the Resolver.
	 *
	 *  The resolver is an object capable of translating a hostname into an IP address.
	 */
	class IResolver
	{
	public:
		/*!
		 *  \brief Returns an string with the IP address
		 *  \param hostname The string to be resolved
		 *
		 *  Returns an string with the IP address. If the resolution is not possible, 
		 *  it should return the string "0.0.0.0"
		 */
		virtual std::string Resolve(const std::string &hostname) = 0;

		/*!
		 *  \brief Returns an string with the IP address
		 *  \param hostname The string to be resolved
		 *
		 *  Returns an string with the IP address. If the resolution is not possible, 
		 *  it should return the string "0.0.0.0"
		 */
		virtual std::string Resolve(const std::string &hostname) const = 0;
	};
}

#endif
