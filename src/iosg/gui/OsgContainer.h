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

#ifndef _OSGICONTAINER_
#define _OSGICONTAINER_

#include <iosg/gui/OsgWidget.h>
#include <string>
#include <map>

#include <iosg/Export.h>


namespace cpw 
{ 
	namespace iosg 
	{

		/** 
			\brief Creates a Container Widget, a widget that can contain other widgets
			\ingroup iosg
		*/
		class IOSGEXPORT OsgIContainer : public OsgIWidget
		{
			public:	

				OsgIContainer(const std::string &url, cpw::Point3d<float> _position, cpw::Point3d<float> _size, cpw::Point3d<float> _rotation, const TAnchor &_anchor);
				virtual ~OsgIContainer(void);


				virtual bool HasFocus(){ return false; }
				virtual void SetVisible(const bool &_visible);

				bool AttachChild(OsgIWidget *child);						
				bool DetachChild(const std::string &name);			
				OsgIWidget *GetChild(const std::string &name);
				OsgIWidget *GetChild(const unsigned int &index);
				unsigned int GetNumChildren() { return (unsigned int) children.size(); }
				virtual void ResizeScreen(const int &x, const int &y);

				virtual void Update(osg::Matrix *mp=NULL);

			private:

				std::map<std::string, iosg::OsgIWidget *> children;

			protected:

				std::map<std::string, iosg::OsgIWidget *> & GetMap() { return children; } 
				
		};
	}
}

#endif
