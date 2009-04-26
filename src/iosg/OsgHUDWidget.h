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

#ifndef _OSGHUDWIDGET_
#define _OSGHUDWIDGET_

#include <osg/Node>
#include <osg/MatrixTransform>

#include <iosg/Export.h>


namespace cpw 
{ 
	namespace iosg 
	{

		/** 
			\brief Por comentar...
			\ingroup iosg
		*/
		class IOSGEXPORT OsgHUDWidget
		{
			public:

				OsgHUDWidget(void);
				~OsgHUDWidget(void);

				void SetName(const std::string &name);
				osg::MatrixTransform * GetNode() { return mt.get(); }

				unsigned int GetNumChildren() { return (unsigned int) children.size(); }
				OsgHUDWidget * GetChildren(const unsigned int &child_index);
				void AddChild(OsgHUDWidget *new_child) { children.push_back(new_child); }
			
			private:

				osg::ref_ptr<osg::MatrixTransform> mt;
				std::vector<OsgHUDWidget *> children;



				


		};
	}
}

#endif
