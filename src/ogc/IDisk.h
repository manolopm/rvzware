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

#ifndef _IDISK_
#define _IDISK_

#include <vector>
#include <string>

#include <ogc/Export.h>

namespace cpw 
{ 
	namespace ogc
	{

		/** 
			\brief Interface for disk management 
			\ingroup ogc
		*/
		class OGCEXPORT IDisk
		{

		public:
			virtual ~IDisk(){}
			virtual int Dir(const std::string &dir, std::vector<std::string> &files) = 0;
			virtual int Save(const std::string &name, const std::string &data) = 0;
			virtual int Load(const std::string &name, std::string &data) = 0;
			virtual int RemoveFile(const std::string &name) = 0;
			virtual int RemoveDir(const std::string &name) = 0;
			virtual int MakeDir(const std::string &name) = 0;
		};

	}

}

#endif
