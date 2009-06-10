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

#include <fstream>

#include <stdafx.h>

#include <controllers/CwsLoader.h>

using namespace cpw::controllers;


CwsLoader::CwsLoader(const std::string &filecws)
{
  std::ifstream cws_file(filecws.c_str());
  toplayer = "";
  srs = "";
  
  if (cws_file.is_open())
    {
      while (!cws_file.eof())
	{
	  std::string tag;

	  cws_file >> tag;
	  
	  if(tag == "[TOPLAYER]") 
	    {
	      cws_file >> toplayer;
	    }
	  if(tag == "[SRS]") 
	    {
	      cws_file >> srs;
	    }
	}
      cws_file.close();
    }
}

CwsLoader::~CwsLoader()
{

}

std::string &CwsLoader::GetTopLayer()
{
	return toplayer;
}

const std::string &CwsLoader::GetTopLayer() const 
{
	return toplayer;
}

std::string &CwsLoader::GetSRS()
{
	return srs;
}

const std::string &CwsLoader::GetSRS() const 
{
	return srs;
}
