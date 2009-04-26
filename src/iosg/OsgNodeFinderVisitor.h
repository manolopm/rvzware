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

#ifndef _NODEFINDERVISITOR_
#define _NODEFINDERVISITOR_

#include <osg/NodeVisitor>

#include <iosg/Export.h>


namespace cpw 
{ 
	namespace iosg 
	{

		/** 
			\brief Por comentar ....
			\ingroup iosg
		*/
		class IOSGEXPORT OsgNodeFinderVisitor : public osg::NodeVisitor
		{
			public:
				
				OsgNodeFinderVisitor(const std::string &node_to_search, const bool &_get_first=true);
				~OsgNodeFinderVisitor(void);
				osg::Node * GetNode() { return found_node; }
				virtual void apply( osg::Node& node );
				
			private:
				bool gc, get_first;
				std::string node_name;
				osg::Node *found_node;
		};
	}
}
#endif
