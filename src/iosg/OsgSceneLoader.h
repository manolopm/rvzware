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


#ifndef _OSGSCENELOADER_
#define _OSGSCENELOADER_

#include <cpw/graphic/ISceneLoader.h>

#include <string>
#include <stdio.h>

#include <osgDB/ReadFile>
#include <osg/Node>
#include <osg/MatrixTransform>

#include <iosg/Export.h>


namespace cpw 
{ 
	namespace iosg 
	{

		/** 
			\brief Por comentar ....
			\ingroup iosg
		*/
		class IOSGEXPORT OsgSceneLoader : public cpw::ISceneLoader
		{
			public:
				OsgSceneLoader(void);
				virtual ~OsgSceneLoader(void);

				void * LoadScene(const std::string &filename);
				bool CheckError() { return error; }
			
			private:
				void AddNodeToTree(osg::Group *root, const std::string &filename, float utm1=0.0f, float utm2=0.0f, float utm3=0.0f);
				bool error;
				
		};
	}
}

#endif
