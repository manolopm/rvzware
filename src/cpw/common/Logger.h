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

#ifndef _LOGGER_
#define _LOGGER_

#include <string>

namespace cpw
{
	/** 
		\brief This class is used to keep track of the messages during the execution 
		\ingroup cpw	
	*/
	class Logger
	{
		public:

			/** Inserts a new message into the Log
				\param new_message the message to show
			*/
			virtual void NewLogMessage(const std::string &new_message) = 0;
			
			/** Clears the Log */
			virtual void ClearLog() = 0;

			/** Sends a message to the Log using the printf c format */
			virtual void printf (const char *format, ...) = 0;
	};
}

#endif




