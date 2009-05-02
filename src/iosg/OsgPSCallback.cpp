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

#include <sstream>

#include <iosg/stdafx.h>

#include <iosg/OsgPSCallback.h>
#include <cpw/common/ApplicationLog.h>

using namespace cpw::iosg;

OsgPSCallback::OsgPSCallback(void) : active(false), rrc(NULL), need_to_update(false), curve_adapter(NULL), need_to_adapt(true)
{
  brothers[0] = NULL;
  brothers[1] = NULL;
}

OsgPSCallback::~OsgPSCallback(void)
{
}


void OsgPSCallback::operator ()(osg::Node *node, osg::NodeVisitor *nv)
{
	
		
  if (active && !need_to_update)
    {
      //return;
    }
  else
    {
#ifdef _DEBUG
      //std::ostringstream msg;
      //msg << id << ", " << new_particles_sec.min << ", " << new_particles_sec.max;
      //cpw::ApplicationLog::GetInstance()->GetLogger()->NewLogMessage(msg.str());
#endif

      active = true;
      need_to_update = false;
		
      SetBrothersToInactive();

      if ((new_particles_sec.min == 0) && (new_particles_sec.max == 0))
	{
	  //The PS is not visible, so delete all the particles and set the emission rate to 0
	  //while (particle_system->numParticles() > 0)
	  //	particle_system->destroyParticle(0);
	  for (int i=0; i<particle_system->numParticles(); i++)
	    particle_system->destroyParticle(i);

	  if (rrc != NULL)
	    rrc->setRateRange(0, 0);
	}
      else
	{
	  if (rrc != NULL)
	    rrc->setRateRange(new_particles_sec.min, new_particles_sec.max);    // ritmo de generacion

	  ////update the particles size
	  //osgParticle::Particle ptemplate = particle_system->getDefaultParticleTemplate();
	  //ptemplate.setSizeRange(osgParticle::rangef(particles_size.min, particles_size.max));
	  //particle_system->setDefaultParticleTemplate(ptemplate);

	  //Adapt the vertice to terrain surface
	  /*
	    if (need_to_adapt)
	    {
	    AdaptVerticesToTerrain();
	    cpw::ApplicationLog::GetInstance()->GetLogger()->NewLogMessage(msg.str());
	    }
	    else
	    need_to_adapt = true;*/


	}
    }

  osg::NodeCallback::operator ()(node, nv);
}

void OsgPSCallback::SetBrothersToInactive()
{
  //left brothers
  OsgPSCallback *aux = NULL;
	
  if (brothers[0] != NULL)
    {
      aux = GetLeftBrother();
	
      while (aux != NULL)
	{
	  aux->active = false;
	  aux = aux->GetLeftBrother();
	}
    }

  //right brothers
  if (brothers[1] != NULL)
    {
      aux = GetRightBrother();

      while (aux != NULL)
	{
	  aux->active = false;
	  aux = aux->GetRightBrother();
	}
    }
}

void OsgPSCallback::AdaptVerticesToTerrain()
{
  //exit preconditions
  if ((emitter == NULL) || (curve_adapter == NULL))
    return;

  if (emitter->getPlacer()->className() != std::string("MultiSegmentPlacer"))
    return;
	
  osgParticle::MultiSegmentPlacer *msp = (osgParticle::MultiSegmentPlacer *) emitter->getPlacer();

  if (msp->numVertices() == 0)
    return;

  temp_vec0.clear();
  temp_vec1.clear();

  //copy all the vertices to our vector
  for (int i=0; i<msp->numVertices(); i++)
    temp_vec0.push_back( cpw::Point3d<float>(msp->getVertex(i).x(), msp->getVertex(i).y(), msp->getVertex(i).z()) );

  //clean the vertices of osg
  while (msp->numVertices() > 0)
    msp->removeVertex(0);			

  //adapt the new vertices
  float aux = curve_adapter->GetStepDistance();
  curve_adapter->SetStepDistance( curve_adapter_step_distance );
  curve_adapter->Adapt(temp_vec0, temp_vec1);
  curve_adapter->SetStepDistance( aux );

  //fill again the osg vec
  for (std::vector< cpw::Point3d<float> >::iterator iter = temp_vec1.begin();
       iter != temp_vec1.end();
       iter++)
    {
      msp->addVertex( osg::Vec3(iter->x, iter->y, iter->z) );
    }	
}
