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
#ifndef _FILEMANAGER_
#define _FILEMANAGER_

#include <string>

#include <cpw/persistent/file/IFileManager.h>
#include <cpw/Export.h>


namespace cpw {


	/** 
		\brief Por comentar ....
		\ingroup cpw
	*/
	class CPWEXPORT FileManager : public IFileManager
	{

		struct GetMapWorkingData
		{
			bool inMemory; // TRUE Read everything in a stringstream . FALSE Write the map image into a file
			std::string *membuf; // The memory buffer in case we use it
			FILE *fd; // The file descriptor in case we are writing to file
		};

	public:
		FileManager(void);
		virtual ~FileManager(void);
		virtual FileError Save  (const std::string &data, const std::string &url, bool overwrite = true);
		virtual FileError Load  (const std::string &url, std::string &data);
		virtual FileError Delete(const std::string &file);

	private:

		bool IsLocal(const std::string &url);

		static size_t writeCallback(void *buffer, size_t size, size_t nmemb, void *getMapWorkingData);
		bool GetMap(const std::string &url, bool ssl, std::string &memBuf, const std::string &proxy);
		static size_t read_callback(void *ptr, size_t size, size_t nmemb, void *stream);
		int PutFile(const std::string &localFilename, const std::string &urlDest);

	};

}

#endif
