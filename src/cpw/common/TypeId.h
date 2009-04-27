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

#ifndef _TYPEID_
#define _TYPEID_

#include <string>
#include <cpw/common/pstdint.h>
#include <cpw/Export.h>

#define TYPEID_SIZE 16

namespace cpw
{
	/** 
		\brief This class is used to represent the type of the Entity id. 
		\ingroup cpw
	*/
	class CPWEXPORT TypeId
	{
	public:

		/** Default constructor */
		TypeId();

		/** Constructor
			\param id is a string
		*/
		TypeId(const std::string &id);

		/** Constructor
			\param id is a string
		*/
		TypeId(const char *id);
		
		/** Copy constructor
			\param id 
		*/
		TypeId(const TypeId& id);

		/** Returns the size of the identifier in bytes */
		static int size();

		/** Overload of operator =	
			\param id to copy
		*/
		TypeId &operator=(const TypeId& id);

		/** Compares two identifiers. Returns true if they are equal
			\param id to compare
		*/
		const bool operator== (const TypeId &id) const;

		/** Compares two identifiers. Returns true if they are not equal
			\param id to compare
		*/
		const bool operator!= (const TypeId &id) const;

		/** Compares two identifiers. Returns true if the first is smaller than the second
			\param id to compare
		*/
		const bool operator< (const TypeId &id) const;

		/** Overload of the cast operator (string) to convert the identifier to a string */ 
		operator std::string();

		/** Returns the value of the identifier */
		std::string GetValue();

		/** Returns the value of the identifier */
		std::string GetValue() const;

		/** Sets the value of the identifier */
		void SetValue(const std::string &str);

		/** Returns the ith position of the identifier 
			\param i the position to obtain
		*/
		uint8_t &operator[](const int i);

	private:

		/** Converts a string value to a TypeId format 
			\param value
		*/
		void Import(std::string value);

		/** Converts a TypeId format to a string */
		std::string Export();

		/** Converts a TypeId format to a string */
		const std::string Export() const;

		
		uint8_t value[TYPEID_SIZE]; ///< vector representing the identifier (lsb is 0)
	};

	/** Overload of operator << to allow working with streams
		\param s the output stream
		\param id the TypeId to send to the stream
	*/
	std::ostream CPWEXPORT & operator<<(std::ostream &s, const TypeId &id);

	/** Overload of operator >> to allow working with streams
		\param s the input stream
		\param id the TypeId to receive from the stream
	*/
	std::istream CPWEXPORT & operator>>(std::istream &s, TypeId &id);

}

#endif
