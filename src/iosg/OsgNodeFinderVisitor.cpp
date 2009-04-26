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

#include <iosg/stdafx.h>

#include <iosg/OsgNodeFinderVisitor.h>

using namespace cpw::iosg;

OsgNodeFinderVisitor::OsgNodeFinderVisitor(const std::string &node_to_search, const bool &_get_first) : osg::NodeVisitor( osg::NodeVisitor::TRAVERSE_ALL_CHILDREN ), get_first(_get_first)
{
	found_node = NULL;
	node_name = node_to_search;
}

OsgNodeFinderVisitor::~OsgNodeFinderVisitor(void)
{

}

void OsgNodeFinderVisitor::apply( osg::Node& node )
{
	//if matches the name, keep a reference to the node and stop traversing
	std::string n = node.getName();
	
	if (n == node_name)
	{
		found_node = &node;

		if (get_first)
			return; //stop traversing
	}
	
	//keep searching
	traverse(node);
}
