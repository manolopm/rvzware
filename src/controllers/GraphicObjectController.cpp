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
#include "stdafx.h"

#include "controllers/GraphicObjectController.h"

#include <cpw/graphic/ApplicationScene.h>
#include <cpw/graphic/IScene.h>
#include <cpw/entity/Entity.h>
#include <cpw/entity/Element.h>


using namespace cpw::controllers;


GraphicObjectController::GraphicObjectController(cpw::IGraphicObjectLoader *gol, cpw::INavigatorManager *nm):
	x(), y(), z(), graphic_object_loader(gol), navigator_manager(nm)
{

}

GraphicObjectController::~GraphicObjectController()
{
}

cpw::IGraphic *GraphicObjectController::LoadObject(const std::string &filename)
{
	//load the object and return it
	cpw::IGraphic *new_obj = graphic_object_loader->LoadObject(filename);

	return new_obj;
}


bool GraphicObjectController::IntersectCenterCoords(cpw::Point3d<float> &intersection_point)
{
	//calculate the new coordinates of the object
	int centerx, centery;
	cpw::INavigator *nav = navigator_manager->GetFocusedOrFirstNavigator();
	
	if (nav == NULL) //if there is no navigator return
		return false;

	nav->GetCenterScreenCoords(centerx, centery);
	cpw::IScene *app_scene = cpw::ApplicationScene::GetInstance()->GetScene();
	
	//insert the object into the scene
	return nav->IntersectMouseWithScene(centerx, centery,
										intersection_point.x,
										intersection_point.y,
										intersection_point.z);
}

bool GraphicObjectController::InsertTempObject(cpw::IGraphic *new_obj)
{
/*	//calculate the new coordinates of the object
	int centerx, centery;
	INavigator *nav = navigator_manager->GetFocusedOrFirstNavigator();
	
	if (nav == NULL) //if there is no navigator return
		return false;

	nav->GetCenterScreenCoords(centerx, centery);
	cpw::IScene *app_scene = cpw::ApplicationScene::GetInstance()->GetScene();
	
	//insert the object into the scene
	if (nav->IntersectMouseWithScene(centerx, centery, x, y, z))
		app_scene->AddTempObject(new_obj, x, y, z);

	else
		app_scene->AddTempObject(new_obj, 0.0f, 0.0f, 0.0f);
*/	
	return true;
}

void GraphicObjectController::SetPositionOrientationScale(float posx, float posy, float posz,
	                             float orientationx, float orientationy, float orientationz,
								 float scalex, float scaley, float scalez)
{
/*	cpw::IScene *app_scene = cpw::ApplicationScene::GetInstance()->GetScene();
	app_scene->SetTempObjectPositionOrientationScale(posx, posy, posz, 
		                                             orientationx, orientationy, orientationz,
													 scalex, scaley, scalez);
*/
}

void GraphicObjectController::RemoveTempObject()
{
/*	cpw::IScene *app_scene = cpw::ApplicationScene::GetInstance()->GetScene();
	app_scene->RemoveTempObject();*/
}

void GraphicObjectController::MakeTempObjectPermanent(const std::string &group_name, const std::string &element_url, const std::string &icon_filename)
{
/*	cpw::IScene *app_scene = cpw::ApplicationScene::GetInstance()->GetScene();
	app_scene->MakeTempObjectPermanent(group_name, element_url, icon_filename);*/
}




//PARTICLE SYSTEMS
bool GraphicObjectController::InsertMultiLineParticleSystemInScene(const int mouse_x, const int mouse_y,
													  const std::string &img_file, const std::string &ps_id,
													  const std::string &group_name,
													  const cpw::Range<int> &new_particles_sec,
													  const float particles_life, 
													  const cpw::Range<float> &alpha,
													  const bool senoidal_alpha_interpolation, 
													  const bool additive_blend,
													  const cpw::Range<float> &speed,
													  const cpw::Range<float> &phi,
													  const cpw::Range<float> &theta,		   
													  const cpw::Range<float> &size,
													  cpw::Point3d<float> &intersection_point)
{
	cpw::IScene *app_scene = cpw::ApplicationScene::GetInstance()->GetScene();
	cpw::INavigator *active_navigator = navigator_manager->GetFocusedOrFirstNavigator();
	
	if (active_navigator == NULL) //if there is no navigator return
		return false;

	//insert the object into the scene
	if (active_navigator->IntersectMouseWithScene(mouse_x, mouse_y, x, y, z))
	{
		intersection_point.x = x;
		intersection_point.y = y;
		intersection_point.z = z;
	}
	else
	{
		intersection_point.x = 0.0f;
		intersection_point.y = 0.0f;
		intersection_point.z = 0.0f;
	}
	
	app_scene->AddMultiLineParticleSystem(intersection_point, 
										  img_file,
										  ps_id,
										  group_name,
										  new_particles_sec,
										  particles_life, 
										  alpha,
										  senoidal_alpha_interpolation, 
										  additive_blend,
										  speed,
										  phi,
										  theta,		   
										  size);
	

	return true;

}

void GraphicObjectController::AddVertexToMultiLineParticleSystem(const int mouse_x, const int mouse_y, const std::string &ps_id, cpw::Point3d<float> &intersection_point)
{
	cpw::IScene *app_scene = cpw::ApplicationScene::GetInstance()->GetScene();
	cpw::INavigator *active_navigator = navigator_manager->GetFocusedOrFirstNavigator();
	
	if (active_navigator == NULL) //if there is no navigator return
		return;

	//insert the object into the scene
	if (active_navigator->IntersectMouseWithScene(mouse_x, mouse_y, x, y, z))
	{
		app_scene->AddVertexToMultiLineParticleSystem(cpw::Point3d<float>(x, y, z), ps_id);
		intersection_point.x = x;
		intersection_point.y = y;
		intersection_point.z = z;
	}
	else
	{
		intersection_point.x = 0.0f;
		intersection_point.y = 0.0f;
		intersection_point.z = 0.0f;
	}
	

}


bool GraphicObjectController::UpdateMultiLineParticleSystemVertex(const int vertex_index, const cpw::Point3d<float> &vertex, const std::string &ps_id)
{
	return cpw::ApplicationScene::GetInstance()->GetScene()->UpdateMultiLineParticleSystemVertex(vertex_index, vertex, ps_id);
}

void GraphicObjectController::DeleteParticleSystem(const std::string &ps_id, const std::string &parent_id)
{
	cpw::ApplicationScene::GetInstance()->GetScene()->DeleteParticleSystemFromScene(ps_id, parent_id);
}

//Place the new PS directly into position coords
void GraphicObjectController::InsertMultiLineParticleSystemInScene(const cpw::Point3d<float> position,
													  const std::string &img_file, const std::string &ps_id,
													  const std::string &group_name,
													  const cpw::Range<int> &new_particles_sec,
													  const float particles_life, 
													  const cpw::Range<float> &alpha,
													  const bool senoidal_alpha_interpolation, 
													  const bool additive_blend,
													  const cpw::Range<float> &speed,
													  const cpw::Range<float> &phi,
													  const cpw::Range<float> &theta,		   
													  const cpw::Range<float> &size)
{
	cpw::IScene *app_scene = cpw::ApplicationScene::GetInstance()->GetScene();

	app_scene->AddMultiLineParticleSystem(position,
										  img_file,
										  ps_id,
										  group_name,
										  new_particles_sec,
										  particles_life, 
										  alpha,
										  senoidal_alpha_interpolation, 
										  additive_blend,
										  speed,
										  phi,
										  theta,		   
										  size );
}


bool GraphicObjectController::AddVertexToMultiLineParticleSystem(const std::string &ps_id, cpw::Point3d<float> &position)
{
	return cpw::ApplicationScene::GetInstance()->GetScene()->AddVertexToMultiLineParticleSystem(position, ps_id);
}

void GraphicObjectController::InsertPuntualParticleSystemInScene(const cpw::Point3d<float> position,
													  const std::string &img_file, const std::string &ps_id,
													  const std::string &group_name,
													  const cpw::Range<int> &new_particles_sec,
													  const float particles_life, 
													  const cpw::Range<float> &alpha,
													  const bool senoidal_alpha_interpolation, 
													  const bool additive_blend,
													  const cpw::Range<float> &speed,
													  const cpw::Range<float> &phi,
													  const cpw::Range<float> &theta,		   
													  const cpw::Range<float> &size)
{
	cpw::IScene *app_scene = cpw::ApplicationScene::GetInstance()->GetScene();

	app_scene->AddPuntualParticleSystem(position,
										img_file,
										ps_id,
										group_name,
										new_particles_sec,
										particles_life, 
										alpha,
										senoidal_alpha_interpolation, 
										additive_blend,
										speed,
										phi,
										theta,		   
										size );
}

bool GraphicObjectController::InsertPuntualParticleSystemInScene(const int mouse_x, const int mouse_y,
													  const std::string &img_file, const std::string &ps_id,
													  const std::string &group_name,
													  const cpw::Range<int> &new_particles_sec,
													  const float particles_life, 
													  const cpw::Range<float> &alpha,
													  const bool senoidal_alpha_interpolation, 
													  const bool additive_blend,
													  const cpw::Range<float> &speed,
													  const cpw::Range<float> &phi,
													  const cpw::Range<float> &theta,		   
													  const cpw::Range<float> &size,
													  cpw::Point3d<float> &intersection_point)
{
	cpw::IScene *app_scene = cpw::ApplicationScene::GetInstance()->GetScene();
	cpw::INavigator *active_navigator = navigator_manager->GetFocusedOrFirstNavigator();
	
	
	if (active_navigator == NULL) //if there is no navigator return
		return false;


	//insert the object into the scene
	if (active_navigator->IntersectMouseWithScene(mouse_x, mouse_y, x, y, z))
	{
		intersection_point.x = x;
		intersection_point.y = y;
		intersection_point.z = z;
	}
	else
	{
		intersection_point.x = 0.0f;
		intersection_point.y = 0.0f;
		intersection_point.z = 0.0f;
	}
	
	app_scene->AddPuntualParticleSystem(intersection_point, 
									    img_file,
										ps_id,
										group_name,
										new_particles_sec,
										particles_life, 
										alpha,
										senoidal_alpha_interpolation, 
										additive_blend,
										speed,
										phi,
										theta,		   
										size);

	return true;
}


void GraphicObjectController::UpdatePuntualParticleSystem(const cpw::Point3d<float> position,
														const std::string &img_file,
														const std::string &ps_id,														
														const cpw::Range<int> &new_particles_sec,
														const float particles_life, 
														const cpw::Range<float> &alpha,
														const bool senoidal_alpha_interpolation, 
														const bool additive_blend,
														const cpw::Range<float> &speed,
														const cpw::Range<float> &phi,
														const cpw::Range<float> &theta,		   
														const cpw::Range<float> &size)
{
	cpw::IScene *app_scene = cpw::ApplicationScene::GetInstance()->GetScene();

	app_scene->UpdatePuntualParticleSystem(position,
										img_file,
										ps_id,
										new_particles_sec,
										particles_life, 
										alpha,
										senoidal_alpha_interpolation, 
										additive_blend,
										speed,
										phi,
										theta,		   
										size );


}


bool GraphicObjectController::UpdateParticleSystemParticleGenerationRate(const std::string &ps_id, const cpw::Range<int> &new_rate)
{
	cpw::IScene *app_scene = cpw::ApplicationScene::GetInstance()->GetScene();
	return app_scene->UpdateParticleSystemParticleGeneratorRate(ps_id, new_rate);
}

bool GraphicObjectController::UpdateParticleSystemParticleLife(const std::string &ps_id, float new_life)
{
	cpw::IScene *app_scene = cpw::ApplicationScene::GetInstance()->GetScene();
	return app_scene->UpdateParticleSystemParticleLife(ps_id, new_life);
}
bool GraphicObjectController::UpdateParticleSystemParticleSize(const std::string &ps_id, const cpw::Range<float> &new_size)
{
	cpw::IScene *app_scene = cpw::ApplicationScene::GetInstance()->GetScene();
	return app_scene->UpdateParticleSystemParticleSize(ps_id, new_size);
}

bool GraphicObjectController::IntersectMouseWithScene(int mouse_x, int mouse_y, cpw::Point3d<float> &intersection_point)
{
	cpw::INavigator *active_navigator = navigator_manager->GetFocusedOrFirstNavigator();
			
	if (active_navigator == NULL) //if there is no navigator return
		return false;

	//insert the object into the scene
	if (active_navigator->IntersectMouseWithScene(mouse_x, mouse_y, x, y, z))
	{
		intersection_point.x = x;
		intersection_point.y = y;
		intersection_point.z = z;
		return true;
	}

	return false;
}

bool GraphicObjectController::IntersectMouseWithEntities(int mouse_x, int mouse_y, std::string &entity_id)
{
	cpw::INavigator *active_navigator = navigator_manager->GetFocusedOrFirstNavigator();
			
	if (active_navigator == NULL) //if there is no navigator return
		return false;

	//insert the object into the scene
	return (active_navigator->IntersectMouseWithEntities(mouse_x, mouse_y, entity_id));
}
