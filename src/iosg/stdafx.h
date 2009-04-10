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
// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#ifndef _WIN32_WINNT		// Allow use of features specific to Windows XP or later.                   
#define _WIN32_WINNT 0x0501	// Change this to the appropriate value to target other versions of Windows.
#endif						

#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers



// TODO: reference additional headers your program requires here
#include <math.h>
#include <stdlib.h>
#include <sstream>
#include <math.h>
#include <float.h>
#include <fstream>

#include <osg/AlphaFunc>
#include <osg/AutoTransform>
#include <osg/BlendFunc>
#include <osg/CullFace>
#include <osg/Depth>
#include <osg/Drawable>
#include <osg/Geode>
#include <osg/Geometry>
#include <osg/Group>
#include <osg/LineSegment>
#include <osg/LineWidth>
#include <osg/LOD>
#include <osg/MatrixTransform>
#include <osg/Node>
#include <osg/PagedLOD>
#include <osg/Point>
#include <osg/PointSprite>
#include <osg/ref_ptr>
#include <osg/Referenced>
#include <osg/StateAttribute>
#include <osg/Switch>
#include <osg/TexEnvCombine>
#include <osg/TexEnv>
#include <osg/TexMat>
#include <osg/Texture>
#include <osg/Texture2D>
#include <osg/Timer>
#include <osg/Transform>
#include <osg/Vec3>
#include <osgDB/ReadFile>
#include <osgDB/WriteFile>
#include <osgParticle/AccelOperator>
#include <osgParticle/FluidFrictionOperator>
#include <osgParticle/MultiSegmentPlacer>
#include <osgParticle/Particle>
#include <osgParticle/ParticleSystem>
#include <osgParticle/ParticleSystemUpdater>
#include <osgParticle/ModularProgram>
#include <osgParticle/RandomRateCounter>
#include <osgParticle/SectorPlacer>
#include <osgParticle/SegmentPlacer>
#include <osgParticle/RadialShooter>
#include <osgText/FadeText>
#include <osgText/Font>
#include <osgUtil/IntersectVisitor>
#include <osgUtil/LineSegmentIntersector>
#include <osgUtil/Optimizer>

#include <OpenThreads/Barrier>

#include <wx/gdicmn.h>

#include <cpw/debugger.h>

