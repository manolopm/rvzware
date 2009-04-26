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

#ifndef _INTERSECTORHEIGHTCALCULATOR_
#define _INTERSECTORHEIGHTCALCULATOR_

#include <osg/Group>

#include <cpw/graphic/IHeightCalculator.h>
#include <iosg/OsgScene.h>

#include <iosg/Export.h>


namespace cpw 
{ 
	namespace iosg 
	{
		class OsgScene; 


		/** 
			\brief Por comentar...
			\ingroup iosg
		*/
		class IOSGEXPORT OsgIntersectorHeightCalculator : public cpw::IHeightCalculator
		{
			public:
				
				OsgIntersectorHeightCalculator(osg::Group *_root_node, OsgScene *_scene);
				~OsgIntersectorHeightCalculator(void);

				virtual bool GetHeight(const float &x, const float &y, float &height);
				virtual bool GetHeightUTM(const float &x, const float &y, float &height);

				void SetRootNode(osg::Group *_root_node) { root_node = _root_node; }
				osg::Group * GetRootNode() { return root_node; }

			private:

				osg::Group *root_node;
				OsgScene *scene;

		};
	}
}

#endif
