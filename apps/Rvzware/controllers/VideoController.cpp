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

#include <stdafx.h>

#include <controllers/VideoController.h>

#include <cpw/common/types.h>
#include <cpw/entity/EntityRegistry.h>


using namespace cpw::controllers;

VideoController::VideoController(void)
{
  app_time = ApplicationTime::GetInstance();
  init = false;
  allow_update = true;
}

VideoController::~VideoController(void)
{
  _layer_tree->Detach(this);
}

void VideoController::UpdateList()
{
}

void VideoController::Update(bool subject_deleted)
{
  if (subject_deleted)
    return;
	
  cpw::cpwTime time;
  bool time_initialized = false;
  std::vector<cpw::Entity *>::iterator i;

  cpw::cpwTime time0, time1;
  animated_entities.clear();

  if (!allow_update)
    int peo = 1;

  if (cpw::EntityRegistry::GetInstance() == NULL)
    {
      int peo = 1;
      return;
    }
	
  animated_entities = cpw::EntityRegistry::GetInstance()->GetAnimatedEntities();

  if(!animated_entities.empty())
    {
      for ( i = animated_entities.begin() ; i != animated_entities.end(); i++)
	{
	  if((*i)->isAnimate()) 
	    {
	      if (!time_initialized)
		{
		  if (   (wxDateTime((time_t)(*i)->GetStartTime().seconds).GetTicks() > 1)
			 && (wxDateTime((time_t)(*i)->GetEndTime().seconds).GetTicks() > 1))
		    {
		      time0 = (*i)->GetStartTime();
		      time1 = (*i)->GetEndTime();
		      time_initialized = true;
		    }
		}
	      else
		{
		  if ((wxDateTime((time_t)(*i)->GetStartTime().seconds).GetTicks() > 1) && (time0.seconds > (*i)->GetStartTime().seconds))
		    time0 = (*i)->GetStartTime();
						
		  if ((wxDateTime((time_t)(*i)->GetEndTime().seconds).GetTicks() > 1) && time1.seconds < (*i)->GetEndTime().seconds)
		    time1 = (*i)->GetEndTime();
		}
	    }
	}
		
      if (time_initialized)
	{
	  app_time->SetPeriod(wxDateTime((time_t)time0.seconds), wxDateTime((time_t)time1.seconds));		
	  if (!init)
	    {
	      init = true;
	      app_time->SetTransformedTime(app_time->GetInitDate());
	    }
	}

      i = animated_entities.begin();

      //if (app_time->IsRunning())
      //{
      time.seconds = app_time->GetTransformedTime().GetTicks();
      time.miliseconds = 	app_time->GetTransformedTime().GetMillisecond();

      for ( ; i != animated_entities.end(); i++)
	{
	  if((*i)->isAnimate()) 
	    (*i)->Animate(time);
	}
      //}		
    }
  else
    app_time->UnsetPeriod();
  UpdateList();
}

void VideoController::SetAnimatedEntityList(std::vector<cpw::Entity *> &entities)
{
  animated_entities.clear();
  animated_entities = entities;
  UpdateList();
}

void VideoController::GetAnimatedEntityListFromLayerTree(cpw::LayerTree *layer_tree)
{
  animated_entities.clear();
  layer_tree->GetAnimatedEntitiesFromTopLayer(animated_entities);
  UpdateList();
}
