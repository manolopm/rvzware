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

#ifndef _IFILEMANAGER_
#define _IFILEMANAGER_

#include <cpw/Export.h>

namespace cpw {


	/** 
		\brief Por comentar ....
		\ingroup cpw
	*/
	enum FileError 
	{
		FILEOK = 0,
		FILEALREADYEXISTS = -1,
		FILENOTFOUND = -2,
		FILEPERMISSIONDENIED = -3,
		FILECANNOTDELETE = -4
	};


	/** 
		\brief
		\ingroup cpw
	*/
	class CPWEXPORT IFileManager
	{
	public:
		virtual ~IFileManager(void) {};
		virtual FileError Save(const std::string &data, const std::string &file, bool overwrite = false) = 0;
		virtual FileError Load(const std::string &file, std::string &data) = 0;
		virtual FileError Delete(const std::string &file) = 0;

	};

}

#endif
