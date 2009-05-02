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

//#include <stdafx.h>

#include <cpw/persistent/database/PersistentDBManager.h>

using namespace cpw;


PersistentDBManager::PersistentDBManager(void)
{

}

PersistentDBManager::~PersistentDBManager(void)
{

}



PersistentError PersistentDBManager::Save(const std::string &url, const Node *root, bool overwrite)
{
  cpw::PersistentError ferror = cpw::PERSISTENTNOTFOUND;

  if(root != NULL)
    {
      /*	cpw::XmlCpwParser parser;
		std::string       xml;

		parser.EntityToXml(root, xml);
		
		cpw::FileManager fout;
		ferror = (PersistentError) fout.Save(xml, url, overwrite); 
      */
    }
  return ferror;
}


PersistentError PersistentDBManager::Load(const std::string &url, Node **root)
{
  //load the file with xml parser
  //FileManager file_manager;		
  //std::string xml;
	
  cpw::PersistentError ferror = cpw::PERSISTENTOK;// = (PersistentError) file_manager.Load(url, xml);

  //if (ferror == cpw::PERSISTENTOK)
  //	{
  //    cpw::XmlCpwParser parser;
  //	*root = parser.XmlToEntity(xml);	
  //}
  //else *root = NULL;
  return ferror;
}


PersistentError PersistentDBManager::Delete(const std::string &url)
{
  cpw::PersistentError ferror = cpw::PERSISTENTOK;
  //delete file from file system
  //FileManager file_manager;		
  return ferror;//(PersistentError) file_manager.Delete(url);
}

