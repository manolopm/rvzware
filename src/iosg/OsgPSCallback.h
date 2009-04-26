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
#ifndef _OSGPSCALLBACK_
#define _OSGPSCALLBACK_

#include <osg/NodeCallback>
#include <osgParticle/ModularEmitter>
#include <osg/Group>

#include <osgParticle/Particle>
#include <osgParticle/ParticleSystem>
#include <osgParticle/ParticleSystemUpdater>
#include <osgParticle/ModularProgram>
#include <osgParticle/RandomRateCounter>
#include <osgParticle/SectorPlacer>
#include <osgParticle/SegmentPlacer>
#include <osgParticle/MultiSegmentPlacer>
#include <osgParticle/RadialShooter>
#include <osgParticle/AccelOperator>

#include <cpw/common/types.h>
#include <cpw/graphic/CurveAdapter.h>
//#include <iosg/OsgCurveAdapter.h>

#include <iosg/Export.h>

namespace cpw 
{ 
	namespace iosg 
	{


		/** 
			\brief Por comentar ....
			\ingroup iosg
		*/
		class IOSGEXPORT OsgPSCallback : public osg::NodeCallback
		{
			public:
				OsgPSCallback(void);
				~OsgPSCallback(void);

				virtual void operator ()(osg::Node *node, osg::NodeVisitor *nv);

				//sets
				void SetNewParticlesPerSecond(const cpw::Range<int> &_new_particles_sec) { new_particles_sec = _new_particles_sec; need_to_update = true;}
				void SetParticlesSize(const cpw::Range<float> &_particles_size) { particles_size = _particles_size; need_to_update = true; }
				void SetEmitter(osgParticle::ModularEmitter *_emitter) { emitter = _emitter; need_to_update=true; }
				void SetParticleSystem(osgParticle::ParticleSystem *_particle_system) { particle_system = _particle_system; need_to_update=true; }
				void SetMultiSegmentPlacer(osgParticle::MultiSegmentPlacer *_ms_placer) { ms_placer = _ms_placer; need_to_update=true; }
				void SetRandomRateCounter(osgParticle::RandomRateCounter *_rrc) { rrc = _rrc; need_to_update=true; }
				void SetLeftBrother(OsgPSCallback *left_brother) { brothers[0] = left_brother; need_to_update=true; }
				void SetRightBrother(OsgPSCallback *right_brother) { brothers[1] = right_brother; need_to_update=true; }
				void SetCurveAdapterStepDistance(const float &_curve_adapter_step_distance) { curve_adapter_step_distance = _curve_adapter_step_distance;}
				void SetCurveAdapter(cpw::CurveAdapter *_curve_adapter) { curve_adapter = _curve_adapter; }
				void SetNeedToAdapt(const bool &value) { need_to_adapt = value; }

				void SetID(int _id) { id = _id; }
				
				//gets
				const cpw::Range<int> & GetNewParticlesPerSecond() { return new_particles_sec; }
				const cpw::Range<int> & GetNewParticlesPerSecond() const { return new_particles_sec; }
				const cpw::Range<float> & GetParticlesSize() { return particles_size; }
				const cpw::Range<float> & GetParticlesSize() const { return particles_size; }
				OsgPSCallback * GetLeftBrother() { return brothers[0]; }
				OsgPSCallback * GetRightBrother() { return brothers[1]; }
				const OsgPSCallback * GetLeftBrother() const { return brothers[0]; }
				const OsgPSCallback * GetRightBrother() const { return brothers[1]; }
				float GetCurveAdapterStepDistance() { return curve_adapter_step_distance; }
				const float GetCurveAdapterStepDistance() const { return curve_adapter_step_distance; }
				cpw::CurveAdapter * GetCurveAdapter() { return curve_adapter; }
				bool GetNeedToAdapt() { return need_to_adapt; }


			private:

				void SetBrothersToInactive();
				void AdaptVerticesToTerrain();

				osgParticle::ParticleSystem *particle_system;
				osgParticle::ModularEmitter *emitter;
				osgParticle::MultiSegmentPlacer *ms_placer;
				osgParticle::RandomRateCounter *rrc;

				cpw::Range<int> new_particles_sec;
				cpw::Range<float> particles_size;
				
				bool active, need_to_update, need_to_adapt;
				float curve_adapter_step_distance;

				cpw::CurveAdapter *curve_adapter;

				unsigned int id;

				OsgPSCallback *brothers[2];

				std::vector< cpw::Point3d<float> > temp_vec0, temp_vec1;
		};
	}
}

#endif