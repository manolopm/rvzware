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

#ifndef _RELATIVEDIRECTORY_
#define _RELATIVEDIRECTORY_

#include <boost/filesystem.hpp>

namespace cpw
{

	namespace controllers 
	{  

		/** 
			\brief This class enables the loading of entities through relative and nested directories
			\ingroup controllers
		*/
		class RelativeDirectory
		{
			public:
				
				RelativeDirectory(const std::string &url);

				~RelativeDirectory();
				
				bool IsValid() {return is_valid;}

				const std::string & GetFilename() const { return filename.string(); }

				const std::string & GetDirectory() const { return directory.string(); }

				const std::string & GetActualPath() const { return actual_path.string(); }

				const std::string & GetPreviousPath() const { return previous_path.string(); }

			private:

				boost::filesystem::path directory;
				boost::filesystem::path previous_path;
				boost::filesystem::path actual_path;

				boost::filesystem::path filename;

				bool is_valid;

		};
	}

}

#endif
