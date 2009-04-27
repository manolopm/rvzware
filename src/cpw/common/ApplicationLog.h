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

#ifndef _APPLICATIONLOG_
#define _APPLICATIONLOG_

#include <cpw/common/Logger.h>
#include <cpw/Export.h>

namespace cpw
{
	/** 
		\brief This class is used to keep a log of the application execution 
		Singleton design pattern
		\ingroup cpw
	*/
	class CPWEXPORT ApplicationLog
	{
		public:
			
			/** Gets the unique instance of the class */
			static ApplicationLog * GetInstance();

			/** Release the instance at the end of the execution */
			static void ReleaseInstance();
			
			/** Returns the Logger of the system */
			Logger * GetLogger();

			/** Sets a new Logger
				\parem logger the new logger
			*/
			void SetLogger(Logger *logger);
				
		private:
			
			Logger *logger;  ///< address of the logger
			
			static ApplicationLog *instance;  ///< the unique instance of the class
			
			/** The default constructor is private to implement the Singleton pattern */
			ApplicationLog();   

			/** Destructor */
			~ApplicationLog();
	};
}

#endif

