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
#ifndef _OSGNAVIGATORPOSTDRAWCALLBACK_
#define _OSGNAVIGATORPOSTDRAWCALLBACK_

#include <osg/Camera>
#include <osg/Texture2D>
#include <osgDB/WriteFile>

#include <iosg/OsgNavigator.h>

#include <iosg/Export.h>

namespace cpw 
{ 
	namespace iosg 
	{

		/** 
			\brief Por comentar ....
			\ingroup iosg
		*/
		class IOSGEXPORT OsgNavigatorPostDrawCallback : public osg::Camera::DrawCallback
		{
			public:
				OsgNavigatorPostDrawCallback(void);
				~OsgNavigatorPostDrawCallback(void);

				virtual void operator () (const osg::Camera &camera) const;
				void SetNavigator(OsgNavigator *_navigator) { navigator = _navigator; }
			
			private:

				OsgNavigator *navigator;
		};

		/** 
			\brief Por comentar ....
			\ingroup iosg
		*/
		class IOSGEXPORT OsgCameraRenderToTexturePostDrawCallback : public osg::Camera::DrawCallback
		{
			public:
				OsgCameraRenderToTexturePostDrawCallback() : text2d(NULL), draw_to_disk(false) {}
				~OsgCameraRenderToTexturePostDrawCallback() {}

				virtual void operator () (const osg::Camera &camera) const
				{}

				void SetTexture(osg::Texture2D *_text2d) { text2d = _text2d; }
				osg::Texture2D * GetTexture() { return text2d; }

				bool draw_to_disk;

			private:
				osg::Texture2D *text2d;

		};
	}
}

#endif
