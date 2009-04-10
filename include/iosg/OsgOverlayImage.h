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
#ifndef _OSG__IMAGE_
#define _OSG__IMAGE_

#include <cpw/graphic/IOverlayImage.h>

#include <iosg/OsgNavigatorPostDrawCallback.h>

#include <osg/Node>
#include <osg/ref_ptr>
#include <osg/Image>
#include <osg/Texture2d>
#include <osg/Camera>

#include <iosg/Export.h>


namespace cpw 
{ 
	namespace iosg 
	{

		/** 
			\brief Por comentar ....
			\ingroup iosg
		*/
		class IOSGEXPORT OsgOverlayImage : public cpw::IOverlayImage
		{
			public:
				
				OsgOverlayImage(const cpw::Point2d<float> &_utm0, const cpw::Point2d<float> &_utm1,
						  const int &_resx=512, const int &_resy=512,
						  const cpw::IOverlayImage::CPW_PIXEL_FORMAT &pf=cpw::IOverlayImage::PF_RGBA);

				//OsgIImage(void);
				virtual ~OsgOverlayImage(void);

				virtual void GetSize(int &resx, int &resy);
				
				virtual bool DrawFirstNTriangleArrays(const unsigned int &n);

				std::vector< std::vector<cpw::Triangle> > & GetTriangleArrays() { return triangle_arrays; }

				virtual void Clear();

				virtual void SetPerimeterColors( const std::vector<cpw::Color> &colors) { p_colors = colors; }

				virtual void Visualize(const bool &value);

			private:
				
				int bpp;
				int _resx_, _resy_;

				osg::ref_ptr<osg::Texture2D> texture;
				osg::ref_ptr<osg::Geode> geode;
				osg::ref_ptr<osg::Geometry> geom;
				osg::ref_ptr<osg::Vec3Array> vertices;
				osg::ref_ptr<osg::Camera> camera;
				std::vector< std::vector<cpw::Triangle> > triangle_arrays;
				std::vector< cpw::Color > p_colors;
				bool visible;
				
				cpw::Point2d<float> utm0, utm1;
		};
	}
}

#endif

