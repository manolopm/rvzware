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

#ifndef _PERSISTENTFILEMANAGER_
#define _PERSISTENTFILEMANAGER_

#include <string>

#include <cpw/persistent/IPersistentManager.h>
#include <cpw/Export.h>

namespace cpw {


	/** 
		\brief Por comentar ....
		\ingroup cpw
	*/
	class CPWEXPORT PersistentFileManager : public IPersistentManager
	{


	public:
		PersistentFileManager(void);
		virtual ~PersistentFileManager(void);

		virtual PersistentError Save  (const std::string &url, const Node *root, bool overwrite = false);
		virtual PersistentError Load  (const std::string &url, Node **root);
		virtual PersistentError Delete(const std::string &url);

	};

}

#endif
