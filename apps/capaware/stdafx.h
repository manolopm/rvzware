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
// stdafx.h: archivo de inclusión de los archivos de inclusión estándar del sistema
// o archivos de inclusión específicos de un proyecto utilizados frecuentemente,
// pero rara vez modificados
//

#pragma once


#include <wx/wxprec.h>
#include <wx/bitmap.h>
#include <wx/button.h>
#include <wx/calctrl.h>
#include <wx/colour.h>
#include <wx/datectrl.h>
#include <wx/datetime.h>
#include <wx/dcclient.h>
#include <wx/dcscreen.h>
#include <wx/dynlib.h>
#include <wx/filedlg.h>
#include <wx/gdicmn.h>
#include <wx/image.h>
#include <wx/imaglist.h>
#include <wx/listbox.h>
#include <wx/minifram.h>
#include <wx/menu.h>
#include <wx/odcombo.h>
#include <wx/region.h>
#include <wx/renderer.h>
#include <wx/scrolwin.h>
#include <wx/settings.h>
#include <wx/spinbutt.h>
#include <wx/spinctrl.h>
#include <wx/statbox.h>
#include <wx/textctrl.h>
#include <wx/timer.h>
#include <wx/treebase.h>
#include <wx/treectrl.h>
#include <wx/window.h>



#if defined(__WXMSW__) &&  defined(_NATIVE_WIN32_) 
#include <wx/dcmemory.h>
#endif


#ifdef __WXMAC__
    #include "wx/mac/private.h"
#endif

#include <OpenThreads/Mutex>
#include <OpenThreads/Barrier>


#include <osg/AlphaFunc>
#include <osg/BlendFunc>
#include <osg/CullFace>
#include <osg/Depth>
#include <osg/Drawable>
#include <osg/Geode>
#include <osg/Geometry>
#include <osg/Group>
#include <osg/LineSegment>
#include <osg/LOD>
#include <osg/Matrix>
#include <osg/MatrixTransform>
#include <osg/AutoTransform>
#include <osg/Transform>
#include <osg/Node>
#include <osg/PagedLOD>
#include <osg/Point>
#include <osg/PointSprite>
#include <osg/ref_ptr>
#include <osg/Referenced>
#include <osg/StateAttribute>
#include <osg/Switch>
#include <osg/TexEnvCombine>
#include <osg/TexMat>
#include <osg/Texture2D>
#include <osg/Timer>
#include <osgDB/ReadFile>
#include <osgDB/WriteFile>
#include <osgParticle/AccelOperator>
#include <osgParticle/FluidFrictionOperator>
#include <osgParticle/ModularProgram>
#include <osgParticle/MultiSegmentPlacer>
#include <osgParticle/Particle>
#include <osgParticle/ParticleSystem>
#include <osgParticle/ParticleSystemUpdater>
#include <osgParticle/RadialShooter>
#include <osgParticle/RandomRateCounter>
#include <osgParticle/SectorPlacer>
#include <osgParticle/SegmentPlacer>
#include <osgUtil/IntersectVisitor>
#include <osgUtil/LineSegmentIntersector>
#include <osgUtil/Optimizer>
#include <osgText/FadeText>
#include <osgText/Font>
#include <osgText/Text>

#ifdef WIN32
  #include <windows.h>
  #include <Iphlpapi.h>
#endif 


#include <cstdlib>

#include <math.h>
#include <stdlib.h>

//#pragma warning (disable:4786)
#include <string>
#include <vector>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <map>

#include <boost/filesystem.hpp>


#include <cpw/debugger.h>

