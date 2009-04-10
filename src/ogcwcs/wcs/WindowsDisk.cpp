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
#include <ogcwcs/WindowsDisk.h>
#ifdef WIN32
#include <windows.h>

#include <direct.h>
#else
#include <sys/types.h>
 
#include <dirent.h>

#endif
//#include <cpw/debugger.h>
#include <boost/filesystem.hpp>

using namespace cpw::ogcwcs;

WindowsDisk::WindowsDisk(){}

WindowsDisk::~WindowsDisk(){}


int WindowsDisk::Dir(const std::string &dir, std::vector<std::string> &files)
{

   const std::string dir1 = dir + "/*.*";
   std::string fich(dir1.begin(), dir1.end());
#ifdef WIN32
   WIN32_FIND_DATA f;
   HANDLE h = FindFirstFile(fich.c_str(), &f);
	if(h != INVALID_HANDLE_VALUE)
	  {
		do
		{
			int filesize = (f.nFileSizeHigh * (MAXDWORD+1)) + f.nFileSizeLow;
			if(filesize > 100)
			{
				std::string file(f.cFileName);
				files.push_back(file);
			}

		} while(FindNextFile(h, &f));
	}
	return 0;
#else
	DIR * direct = opendir (fich.c_str());

	if (direct == NULL){
	  return 0;
	}


	struct dirent *d = readdir (direct);
	while (d != NULL){
	  int filesize = d->d_reclen;
	  if (filesize>100)
	    {
	      std::string file(d->d_name);
	      files.push_back(file);
	    }
	  d = readdir (direct);
	}

	// Now, close the directory.

	closedir (direct);
	return 0;
	
#endif
}


int WindowsDisk::Save(const std::string &name, const std::string &data)
{
	
	return 0;
}

int WindowsDisk::Load(const std::string &name, std::string &data)
{
	return 0;
}

int WindowsDisk::RemoveFile(const std::string &name)
{
	try{
		boost::filesystem::remove(name);
	}
	catch(...)
	{
	}

	return 0;
}


int WindowsDisk::RemoveDir(const std::string &name)
{
	
	try{
		boost::filesystem::remove_all( name );
	}
	catch(...)
	{
	}

	return 0;
}

int WindowsDisk::MakeDir(const std::string &name)
{

	try{
		boost::filesystem::create_directory( name );
	}
	catch(...)
	{
	}
	
	return 0;
}
