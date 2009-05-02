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

//#include <stdafx.h>

#include <cpwutils/GraphicFireCallBack.h>
#include <cpwutils/Fire.h>

#include <cpw/graphic/ApplicationScene.h>
#include <cpw/graphic/IScene.h>
#include <cpw/entity/ElementParticleSystem.h>
#include <cpw/common/ApplicationLog.h>
#include <cpw/common/Math.h>

using namespace cpw::utils;

#define ANIMATION_INTERVAL 10

GraphicFireCallBack::GraphicFireCallBack(void): ignition_point_deleted(false), fire_inserted(false), animate_this_frame(true),
						reset_animation(true), update_animation_every_n_frames(10), frames_count(0)
{
	
}

GraphicFireCallBack::GraphicFireCallBack(const GraphicFireCallBack &gfcb) : update_animation_every_n_frames(gfcb.update_animation_every_n_frames),
									    frames_count(gfcb.frames_count), ignition_point_deleted(gfcb.ignition_point_deleted),
									    fire_animation(gfcb.fire_animation), fire_inserted(gfcb.fire_inserted), animate_this_frame(gfcb.animate_this_frame), reset_animation(gfcb.reset_animation)

{
  int peo = 1;
	
				
}

GraphicFireCallBack::~GraphicFireCallBack(void)
{
  fire_animation.Clear();
}

void GraphicFireCallBack::Insert(const cpw::Callable* callable)
{
  Fire *entity = (Fire *) callable;
  IScene *app_scene = ApplicationScene::GetInstance()->GetScene();

  std::stringstream seid;
  seid << entity->GetId();

  //if(app_scene->ObjectExistsInScene(seid.str())) 
  //	return;

  Flame *flame;
  std::ostringstream parent_id_ss;
  std::string parent_id;

  Entity *parent = entity->GetParent();

  float max_fire_size = 0.0f;
		
  if(parent != NULL)
    {
		
      std::stringstream sse;
      sse << entity->GetId();
      parent_id_ss << parent->GetId();
      parent_id = parent_id_ss.str();

      ApplicationScene::GetInstance()->GetScene()->CreateGroup(sse.str(), parent_id);
      ApplicationScene::GetInstance()->GetScene()->SetVisible(parent_id, sse.str(),entity->isVisible());

      if(entity->GetPolygonal()) //polygonal fire
	{

			
	  flame = entity->GetFlame(0);

	  if (flame != NULL)
	    {
	      float max_fire_radius = 0.0f;

	      //compute max radius of the fire
	      for (int i=0; i < entity->GetNumTimeSlice(); i++)
		{
		  cpw::Point3d<float> center(0.0f, 0.0f, 0.0f);

		  //compute the center of this perimeter
		  for ( std::vector< cpw::utils::Flame >::iterator flame_iter = entity->GetTimeSlice(i)->flames.begin();
			flame_iter != entity->GetTimeSlice(i)->flames.end();
			flame_iter++)
		    {
		      center += flame_iter->fire.GetCoords();
		    }

		  center.x /= entity->GetTimeSlice(i)->flames.size();
		  center.y /= entity->GetTimeSlice(i)->flames.size();
		  center.z /= entity->GetTimeSlice(i)->flames.size();

		  float max_perimeter_radius = 0.0f;

		  //compute the radius of this perimeter
		  for ( std::vector< cpw::utils::Flame >::iterator flame_iter = entity->GetTimeSlice(i)->flames.begin();
			flame_iter != entity->GetTimeSlice(i)->flames.end();
			flame_iter++)
		    {
		      float temp  = cpw::Math::DistanceBetweenTwoPoints(center, flame_iter->fire.GetCoords());
		      float temp2 = (flame_iter->fire.GetParticlesLife()) * (flame_iter->fire.GetSpeed().max) * (flame_iter->fire.GetSize().max);
		      if (temp > max_perimeter_radius)
			max_perimeter_radius = temp;
		      if (temp2 > max_fire_size)
			max_fire_size = temp2;
		    }

		  //check if the perimeter radius is the largest one
		  if (max_perimeter_radius > max_fire_radius)
		    max_fire_radius = max_perimeter_radius;
		}
	      max_fire_size*=0.12f;



	      if (flame->fire.GetNumParents() == 0)
		flame->fire.AddParent(entity);		

	      if (flame->smoke.GetNumParents() == 0)
		flame->smoke.AddParent(entity);		
				
				
	      flame->fire.SetTextureFileName(entity->GetFireTexture());
	      flame->smoke.SetTextureFileName(entity->GetSmokeTexture());

	      flame->fire.SetIcon(entity->GetIcon());
	      flame->smoke.SetIcon(entity->GetIcon());

	      flame->fire.SetPolygonal(true);
	      flame->smoke.SetPolygonal(true);

	      flame->fire.ClearVertices();
	      flame->smoke.ClearVertices();
				
	      for (int i=0; i<entity->GetNumFlames(); i++)
		{
		  if (entity->GetFlame(i)->fire.GetLastParent() == NULL) entity->GetFlame(i)->fire.AddParent(entity);
		  flame->fire.AddVertex(entity->GetFlame(i)->fire.GetCoords());
		  if (entity->GetFlame(i)->smoke.GetLastParent() == NULL) entity->GetFlame(i)->smoke.AddParent(entity);
		  flame->smoke.AddVertex(entity->GetFlame(i)->fire.GetCoords());
		}
				
	      flame->fire.SetMaxPerimeterRadius(max_fire_radius*max_fire_size);
	      flame->smoke.SetMaxPerimeterRadius(max_fire_radius*max_fire_size);

	      ParticleSystemCallBack::Insert(&flame->fire);
	      ParticleSystemCallBack::Insert(&flame->smoke);

	      entity->SetLastInsertedFire(flame->fire.GetId());
	      entity->SetLastInsertedSmoke(flame->smoke.GetId());
	      reset_animation = true;



	      Point3d<float> icon_despl(0.0f, 0.0f, 1000.0f);
	      std::stringstream fid;
	      fid << flame->fire.GetId();
	      app_scene->AddPointSprite( fid.str(), icon_despl, 10.0f, 10.0f, entity->GetIcon(), max_fire_radius*max_fire_size, max_fire_size);

	      flame->fire.SetInsertedInScene(true);
	      flame->smoke.SetInsertedInScene(true);
	      flame->fire.SetUpdatedInScene(true);
	      flame->smoke.SetUpdatedInScene(true);
	    }
	  //return;
	}
      else //puntual fire
	{
	  for (int i=0; i<entity->GetNumFlames(); i++)
	    {
	      flame = entity->GetFlame(i);

	      if (flame != NULL)
		{
		  if (flame->fire.GetNumParents() == 0)
		    flame->fire.AddParent(entity);		

		  if (flame->smoke.GetNumParents() == 0)
		    flame->smoke.AddParent(entity);		
					
		  flame->fire.SetTextureFileName(entity->GetFireTexture());
		  flame->smoke.SetTextureFileName(entity->GetSmokeTexture());
					
		  flame->fire.SetPolygonal(false);
		  flame->smoke.SetPolygonal(false);	

		  flame->fire.SetIcon(entity->GetIcon());
		  flame->smoke.SetIcon(entity->GetIcon());

		  ParticleSystemCallBack::Insert(&flame->fire);
		  ParticleSystemCallBack::Insert(&flame->smoke);	



		  Point3d<float> icon_despl(0.0f, 0.0f, 1000.0f);
		  std::stringstream fid;
		  fid << flame->fire.GetId();
		  app_scene->AddPointSprite( fid.str(), icon_despl, 10.0f, 10.0f, entity->GetIcon());

		  flame->fire.SetInsertedInScene(true);
		  flame->smoke.SetInsertedInScene(true);
		  flame->fire.SetUpdatedInScene(true);
		  flame->smoke.SetUpdatedInScene(true);
		  reset_animation = true;
		}
	    }
	}
      //}
      //the ignition point models
      for (int i=0; i<entity->GetNumIgnitionPoints(); i++)
	{
	  std::ostringstream strpid, strid;
	  std::string id, group_name;
			
	  //strpid << entity->GetLastParent()->GetId();
	  strpid << entity->GetId();
	  group_name = strpid.str();

	  strid << entity->GetId() << "-ignition_point-" << i;
	  id = strid.str();

	  IScene *scene = ApplicationScene::GetInstance()->GetScene();
			
	  Point3d<float> pA(0.0f, 0.0f, 0.0f);
	  Point3d<float> pB(1.0f, 1.0f, 1.0f);
	  scene->AddObjectToGroup(id, group_name, entity->GetIgnitionPointUrl(),
				  *entity->GetIgnitionPoint(i),
				  (Point3d<float> &)pA,
				  (Point3d<float> &)pB);

			
	}
      ignition_point_deleted = false;

		
    }

	
}
void GraphicFireCallBack::Update(cpw::Callable* callable)
{
  Flame *flame;
  Fire *fire_entity = (Fire *) callable;
  Entity *parent = fire_entity->GetParent();

  if (fire_entity->GetPolygonal())
    {
      for (int i=0; i<fire_entity->GetNumFlames(); i++)
	{
	  flame = fire_entity->GetFlame(i);
	  if (flame != NULL)
	    {
	      if (flame->fire.GetNumParents() == 0)
		flame->fire.AddParent(parent);		

	      if (flame->smoke.GetNumParents() == 0)
		flame->smoke.AddParent(parent);	
	    }
	}

      //Delete(callable);
      //Insert(callable);
    }
  else
    {
      for (int i=0; i<fire_entity->GetNumFlames(); i++)
	{
	  flame = fire_entity->GetFlame(i);

	  if (flame != NULL)
	    {
			
	      if (!flame->fire.GetInsertedInScene())
		{
		  if (flame->fire.GetNumParents() == 0)
		    flame->fire.AddParent(parent);		

		  if (flame->smoke.GetNumParents() == 0)
		    flame->smoke.AddParent(parent);		
				
		  flame->fire.SetTextureFileName(fire_entity->GetFireTexture());
		  flame->smoke.SetTextureFileName(fire_entity->GetSmokeTexture());
				
		  flame->fire.SetPolygonal(false);
		  flame->smoke.SetPolygonal(false);	

		  ParticleSystemCallBack::Insert(&flame->fire);
		  ParticleSystemCallBack::Insert(&flame->smoke);			

		  flame->fire.SetInsertedInScene(true);
		  flame->smoke.SetInsertedInScene(true);

		  flame->fire.SetUpdatedInScene(true);
		  flame->smoke.SetUpdatedInScene(true);
		}
	      else
		{
		  if (!flame->fire.GetUpdatedInScene())
		    {
		      flame->fire.SetPolygonal(false);
		      flame->smoke.SetPolygonal(false);	

		      ParticleSystemCallBack::Update(&flame->fire);
		      ParticleSystemCallBack::Update(&flame->smoke);		

		      flame->fire.SetUpdatedInScene(true);
		      flame->smoke.SetUpdatedInScene(true);
		    }
		}
	    }
	} //for each flame
    }

  //the ignition point models
  //Delete(callable);
  //Insert(callable);
	
  for (int i=0; i<fire_entity->GetNumIgnitionPoints(); i++)
    {
      std::ostringstream strpid, strid;
      std::string id, group_name;
		
      strpid << fire_entity->GetLastParent()->GetId();
      group_name = strpid.str();

      strid << fire_entity->GetId() << "-ignition_point-" << i;
      id = strid.str();

      IScene *scene = ApplicationScene::GetInstance()->GetScene();

      Point3d<float> *point = fire_entity->GetIgnitionPoint(i);
      if (point != NULL)
	{
	  if (scene->ObjectExistsInScene(id))
	    {
	      scene->Update(id, 
			    *point,
			    Point3d<float>(0.0f, 0.0f, 0.0f),
			    Point3d<float>(1.0f, 1.0f, 1.0f));
	    }
	  else
	    {
	      Point3d<float> pA(0.0f, 0.0f, 0.0f);
	      Point3d<float> pB(1.0f, 1.0f, 1.0f);
	      scene->AddObjectToGroup(id, group_name, fire_entity->GetIgnitionPointUrl(),
				      *point,
				      (Point3d<float> &)pA,
				      (Point3d<float> &)pB,fire_entity->isVisible());
	    }
	}
    }
}

void GraphicFireCallBack::Delete(cpw::Callable* callable)
{
  //	Flame *flame;
  Fire *fire_entity = (Fire *) callable;
  std::stringstream sse, ssp;
  IScene *scene = ApplicationScene::GetInstance()->GetScene();

  //if(fire_entity->GetParent() == NULL) return;

  sse << fire_entity->GetId();
  //ssp << fire_entity->GetParent()->GetId();

  //ApplicationScene::GetInstance()->GetScene()->Remove(ssp.str(), sse.str());	

  ApplicationScene::GetInstance()->GetScene()->Remove(fire_entity->GetLastInsertedFire());
  ApplicationScene::GetInstance()->GetScene()->Remove(fire_entity->GetLastInsertedSmoke());
	
  ApplicationScene::GetInstance()->GetScene()->Remove(sse.str());	

  fire_animation.Clear();

  //for (int i=0; i<fire_entity->GetNumFlames(); i++)
  //{
  //		flame = fire_entity->GetFlame(i);
  //		ParticleSystemCallBack::Delete(&flame->fire);
  //		ParticleSystemCallBack::Delete(&flame->smoke);
  //}

  //the ignition point models
  //for (int i=0; i<fire_entity->GetNumIgnitionPoints(); i++)
  //{
  //	std::ostringstream strpid, strid;
  //	std::string id, group_name;
  //	
  //	//strpid << fire_entity->GetLastParent()->GetId();
  //	strpid << fire_entity->GetId();
  //	group_name = strpid.str();

  //	strid << fire_entity->GetId() << "-ignition_point-" << i;
  //	id = strid.str();

  //	IScene *scene = ApplicationScene::GetInstance()->GetScene();

  //	Point3d<float> *point = fire_entity->GetIgnitionPoint(i);
  //	if (point != NULL)
  //		scene->Remove(group_name, id);
  //}
  ignition_point_deleted = true;
};

void GraphicFireCallBack::Animate(cpw::Callable* callable, cpw::cpwTime &time_stamp)
{
  Fire *entity = (Fire *) callable;

  if (entity->IsBeingEdited())
    return;

  if (frames_count < update_animation_every_n_frames)
    {
      frames_count++;
      return;
    }
  else
    frames_count = 0;

	

  //check if the animation data is precomputed
  if (!entity->IsAnimationPreComputed())
    fire_animation.PreComputeAnimationData(entity);
	
  //	entity->PreComputeAnimationData();

  reset_animation = entity->GetResetAnimation();
  entity->SetResetAnimation(false);

  if(!entity->isVisible()) return;

  double pos_between_intervals;
  int active_interval = entity->GetActiveInterval(time_stamp, pos_between_intervals);

  fire_animation.SetPositionBetweenIntervals(pos_between_intervals);

  std::stringstream entity_id;
  entity_id << entity->GetId();
	
  //need to update the animation targets or just step forward in the animation
  if (active_interval == -1)
    {
      if ((time_stamp.seconds > entity->GetEndTime().seconds) || (time_stamp.seconds < entity->GetStartTime().seconds))
	{
	  if (ApplicationScene::GetInstance()->GetScene()->ObjectExistsInScene(entity_id.str()))
	    {
	      fire_animation.Clear();
	      entity->GraphicDelete();
	    }
	}
    }
  else
    {
		
      if (!ApplicationScene::GetInstance()->GetScene()->ObjectExistsInScene(entity_id.str()))
	entity->GraphicInsert();
		
		
      if ((fire_animation.GetCurrentInterval() != active_interval) || (reset_animation))
	{
	  //check if we´re in the last interval
	  if (active_interval == (entity->GetNumTimeSlice()-1))
	    {
	      fire_animation.SetLastInterval(true);
	      fire_animation.UpdateAnimationTargets(entity->GetTimeSlice(active_interval), NULL, active_interval, -1);
	    }
	  else
	    {
	      fire_animation.SetLastInterval(false);
	      fire_animation.UpdateAnimationTargets(entity->GetTimeSlice(active_interval), entity->GetTimeSlice(active_interval+1), active_interval, active_interval+1);
	    }

	  fire_animation.SetCurrentInterval(active_interval);
	  reset_animation = false;
	}

      //update the animation
      fire_animation.StepAnimation(time_stamp);

      //std::stringstream fid, sid ;
      std::string fids, sids;

      fids = entity->GetLastInsertedFire();
      sids = entity->GetLastInsertedSmoke();

      //update vertex positions
      IScene *app_scene = ApplicationScene::GetInstance()->GetScene();

      //If the active interval is not the one inserted
      //fire
      if (!app_scene->UpdateMultiLineParticleSystemVertices(fire_animation.GetCurrentVertices(), fids))
	{
	  Delete(entity);
	  entity->MakeTimeSliceCurrent(active_interval);
	  Insert(entity);
	  app_scene->UpdateMultiLineParticleSystemVertices(fire_animation.GetCurrentVertices(), fids);
	}

      //update particles size
      app_scene->UpdateParticleSystemParticleSize(fids, fire_animation.GetCurrentFireParticlesSize());
		
      //update particles size
      app_scene->UpdateParticleSystemParticleLife(fids, fire_animation.GetCurrentFireParticlesLife());
		
      //update particles generation rate
      app_scene->UpdateParticleSystemParticleGeneratorRate(fids, fire_animation.GetCurrentFireGenerationRate());
			
      //take the smoke out if the performance is not good
      if (!((app_scene->GetPerformance() == IScene::BAD) || (app_scene->GetPerformance() == IScene::VERY_BAD)))
	{
	  //smoke
	  if (!app_scene->UpdateMultiLineParticleSystemVertices(fire_animation.GetCurrentVertices(), sids, true))
	    {
	      Delete(entity);
	      entity->MakeTimeSliceCurrent(active_interval);
	      Insert(entity);
	      app_scene->UpdateMultiLineParticleSystemVertices(fire_animation.GetCurrentVertices(), sids);			
	    }
		
	  //update particles size
	  app_scene->UpdateParticleSystemParticleSize(sids, fire_animation.GetCurrentSmokeParticlesSize());

	  //update particles size
	  app_scene->UpdateParticleSystemParticleLife(sids, fire_animation.GetCurrentSmokeParticlesLife());

	  //update particles generation rate
	  app_scene->UpdateParticleSystemParticleGeneratorRate(sids, fire_animation.GetCurrentSmokeGenerationRate());
	}
      else
	{
	  app_scene->UpdateParticleSystemParticleGeneratorRate(sids, cpw::Range<int>(1, 2));
	  //cpw::ApplicationLog::GetInstance()->GetLogger()->NewLogMessage(std::string("Hiding Smoke"));
	}

		
		

		
    }
}


void GraphicFireCallBack::DeleteCurrentFlames(cpw::Callable* callable)
{
  Flame *flame;
  Fire *fire_entity = (Fire *) callable;
	
  for (int i=0; i<fire_entity->GetNumFlames(); i++)
    {
      flame = fire_entity->GetFlame(i);
      ParticleSystemCallBack::Delete(&flame->fire);
      ParticleSystemCallBack::Delete(&flame->smoke);
    }

  fire_animation.Clear();
	
}

void GraphicFireCallBack::Visualize(cpw::Callable* callable, const bool &value)
{
  Fire *ent = (Fire *) callable;
  if (ent!=NULL)
    {
      std::ostringstream uint_to_string1;
      uint_to_string1 << ent->GetId();  //child

      //Entity* parent;
      //for(int i=0; i<ent->GetNumParents(); i++)
      //{
      //	std::ostringstream uint_to_string2;
      //	parent = ent->GetParent(i);
      //	if (parent != NULL)
      //	{
      //		uint_to_string2 << parent->GetId(); //parent		
      //		ApplicationScene::GetInstance()->GetScene()->SetVisible(uint_to_string2.str(), uint_to_string1.str(), value);
      //	}
      //}
      fire_animation.SetTextureVisible(value);
      ApplicationScene::GetInstance()->GetScene()->SetVisible("", uint_to_string1.str(), value);
		
    }

}

//void GraphicFireCallBack::ComputeParticleGenerationRate()
//{
//
//}
