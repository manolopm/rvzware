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

#include <osgDB/ReadFile>

#include <iosg/stdafx.h>

#include <iosg/OsgObjectRegistry.h>


using namespace cpw::iosg;

OsgObjectRegistry::OsgObjectRegistry(void)
{
}

OsgObjectRegistry::~OsgObjectRegistry(void)
{
	objects.clear();
	fonts.clear();
	
}


osg::Node * OsgObjectRegistry::GetObjectFromFile(const std::string &file_name)
{
	std::map< std::string, osg::ref_ptr<osg::Node>  >::iterator iter = objects.find(file_name);

	if (iter != objects.end())
		return iter->second.get();
	else
	{
		osg::ref_ptr<osg::Node> new_node = osgDB::readNodeFile(file_name);

		if (new_node == NULL)
			return NULL;
		else
		{
			objects[file_name] = new_node;
			return new_node.get();
		}
	}

}
osgText::Font * OsgObjectRegistry::GetFontFromFile(const std::string &file_name)
{
	std::map< std::string, osg::ref_ptr<osgText::Font> >::iterator iter = fonts.find(file_name);

	if (iter != fonts.end())
		return iter->second.get();
	else
	{
		osg::ref_ptr<osgText::Font> new_font = osgText::readFontFile(file_name);

		if (new_font == NULL)
			return NULL;
		else
		{
			fonts[file_name] = new_font;
			return new_font.get();
		}
	}
}
		
		

			
			
