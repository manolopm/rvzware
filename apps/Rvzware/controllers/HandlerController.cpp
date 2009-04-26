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
#include <stdafx.h>
#include <controllers/HandlerController.h>

#include <cpw/graphic/IHandler.h>
#include <cpw/graphic/Gizmo.h>
#include <cpw/graphic/TraslationGizmo.h>
#include <cpw/graphic/RotationGizmo.h>
#include <cpw/graphic/ScaleGizmo.h>
#include <cpw/entity/EntityRegistry.h>
#include <cpw/graphic/ApplicationScene.h>

#include <cpw/ApplicationConfiguration.h>

using namespace cpw::controllers;

HandlerController::HandlerController() : scene(NULL), navigator_manager(NULL), handler_active(false), entity_active(false)
{
	scene = cpw::ApplicationScene::GetInstance()->GetScene();
	navigator_manager = cpw::ApplicationScene::GetInstance()->GetNavigatorManager();
}

HandlerController::~HandlerController()
{
	for (std::map<std::string, cpw::IHandler *>::iterator iter = handlers.begin();
		iter != handlers.end();
		iter++)
	{
		delete iter->second;
	}
}

void HandlerController::MakeNextHandlerGroupActive()
{
	std::map<std::string, cpw::IHandler *>::iterator iter = handlers.find(active_handler);
		
	if ( iter != handlers.end() )
	{
		unsigned int limit = handlers.size();
		unsigned int aux = 1;
		cpw::IHandler *current_handler = (cpw::IHandler *) (iter->second);
		
		iter++;
		
		while (aux < limit)
		{
			if (iter == handlers.end())
				iter = handlers.begin();

			//check if has different brothers
			std::vector< IHandler * > &brothers = iter->second->GetBrothers();
			
			if (current_handler->GetBrothers().empty())
			{
				bool brother_found = false;
				if (brothers.empty())
				{
					if ( current_handler == iter->second )
					{
						brother_found = true;
					}
				}
				else
				{
					for (std::vector< IHandler *>::iterator iterc = brothers.begin();
							iterc != brothers.end();
							iterc++)
					{
						brother_found = false;
						if ( current_handler == (cpw::IHandler *) (*iterc))
						{
							brother_found = true;
						}
					}
				}

				if (!brother_found)
				{
					SetActiveHandler( iter->second->GetId() );
					return;
				}
			}
			else
			{
				for (std::vector< IHandler * >::iterator iterb = current_handler->GetBrothers().begin();
					 iterb != current_handler->GetBrothers().end();
					 iterb++)
				{
					bool brother_found = false;
					cpw::IHandler *handler_to_find = (cpw::IHandler *) (*iterb);
					
					for (std::vector< IHandler *>::iterator iterc = brothers.begin();
						iterc != brothers.end();
						iterc++)
					{
						if ( (handler_to_find == (cpw::IHandler *) (*iterc) ) || ( current_handler == (cpw::IHandler *) (*iterc)))
						{
							brother_found = true;
						}
					}

					if (!brother_found)
					{
						SetActiveHandler( iter->second->GetId() );
						return;
					}
				}
			}


			iter++;
			aux++;
		}
		
	}


}

bool HandlerController::MouseRightButtonDown(const int &x, const int &y)
{
	if ((scene == NULL) || (navigator_manager == NULL))
		return false;

	cpw::INavigator *navigator = navigator_manager->GetFocusedOrFirstNavigator();

	if (handler_active && (!handlers.empty()) && entity_active)
	{
		std::string entity_id;
		if (navigator->IntersectMouseWithEntities(x, y, entity_id))
		{
			if (entity_id == active_entity)
			{
				MakeNextHandlerGroupActive();				
				return true;
			}
		}
	}

	return false;
}

bool HandlerController::MouseLeftDoubleClick(const int &x, const int &y)
{
	if ((scene == NULL) || (navigator_manager == NULL))
		return false;

	cpw::INavigator *navigator = navigator_manager->GetFocusedOrFirstNavigator();

	if (navigator != NULL)
	{
		std::string entity_id;

		entity_id.clear();

		/*if (entity_active && handler_active && (!handlers.empty()))
		{
			std::string aux;
			if (navigator->IntersectMouseWithGizmos(x, y, aux))
			{
				MakeNextHandlerGroupActive();				
				return true;
			}
			
		}
		else */if (navigator->IntersectMouseWithEntities(x, y, entity_id))
		{
			handler_active = entity_active = true;
			active_entity = entity_id;

			std::string handler_id;
			if (navigator->IntersectMouseWithGizmos(x, y, handler_id))
			{
				handler_active = true;
				active_handler = handler_id;
			}
			else
			{
				AdaptActiveHandlerAndBrothersToActiveEntity();
				MakeHandlerAndBrothersVisible(active_handler, true);
			}

			return true;
		} 
	}

	return false;
}


bool HandlerController::MouseLeftButtonDown(const int &x,const int &y)
{
	if ((scene == NULL) || (navigator_manager == NULL))
		return false;

	cpw::INavigator *navigator = navigator_manager->GetFocusedOrFirstNavigator();

	if (navigator != NULL)
	{
		std::string entity_id;

		entity_id.clear();

		if (entity_active)
		{
			std::string handler_id;
			if (navigator->IntersectMouseWithGizmos(x, y, handler_id))
			{
				handler_active = true;
				active_handler = handler_id;
				return true;
			}
			else
			{
				
				handler_active = false;
				MakeHandlerAndBrothersVisible(active_handler, false);
			}
		}
	}

	return false;
}

bool HandlerController::MouseLeftButtonUp(const int &x,const int &y)
{
	if (handler_active)
	{
		std::map<std::string, cpw::IHandler *>::iterator iter = handlers.find( active_handler );

		if (iter != handlers.end())
		{
			iter->second->Reset();
			iter->second->AdaptHandlerAndBrothersToModel();
			iter->second->ShowBrothers(true);
		}
	}

	return false;
}


bool HandlerController::MouseDrag(const int &x0, const int &y0, const int &x1, const int &y1, const cpw::MouseButtonsState &mbs)
{
	
	if (handler_active && mbs.mouse_left_down && !mbs.mouse_right_down && !mbs.mouse_middle_down)
	{
		
		std::map<std::string, cpw::IHandler *>::iterator iter = handlers.find( active_handler );

		if (iter != handlers.end())
		{
			//update the camera position and the handlers target
			UpdateEntityAndBrothersCameraPosition();
			UpdateActiveHandlerAndBrothersTarget();
			iter->second->ShowBrothers(false);

			return iter->second->MouseDrag(x0, y0, x1, y1, mbs);
		}
	}

	return false;
}

bool HandlerController::SetActiveHandler(const std::string &handler_id)
{ 
	std::map<std::string, cpw::IHandler *>::iterator iter = handlers.find(handler_id);

	if (iter != handlers.end())
	{
		MakeHandlerAndBrothersVisible(active_handler, false);

		active_handler = handler_id;

		MakeHandlerAndBrothersVisible(active_handler, true);

		cpw::EntityRegistry *entity_reg = cpw::EntityRegistry::GetInstance();
		
		iter->second->SetEntityTarget(entity_reg->GetEntity(active_entity));
		iter->second->AdaptHandlerAndBrothersToModel();

		return true;
	}
	else
		return false;
}

bool HandlerController::InsertHandler(cpw::IHandler *new_handler)
{
	std::map<std::string, cpw::IHandler *>::iterator iter = handlers.find(new_handler->GetId());

	if (iter == handlers.end())
	{
		handlers[new_handler->GetId()] = new_handler;
		return true;
	}
	else
		return false;
}

bool HandlerController::MakeHandlerAndBrothersVisible(const std::string &handler_id, const bool &visible)
{
	if (scene == NULL)
		return false;

	std::map<std::string, cpw::IHandler *>::iterator iter = handlers.find(handler_id);

	if (iter != handlers.end())
	{
		scene->SetHandlerVisible(handler_id, visible);

		for (std::vector<cpw::IHandler *>::iterator iterb = iter->second->GetBrothers().begin();
		     iterb != iter->second->GetBrothers().end();
		   	 iterb++)
		{
			scene->SetHandlerVisible((*iterb)->GetId(), visible);
		}

		return true;
	}
	else
		return false;
}

bool HandlerController::AdaptActiveHandlerAndBrothersToActiveEntity()
{
	if ((scene == NULL) || (!entity_active) || (!handler_active))
		return false;

	int width, height;
	double fov_y, aspect_ratio, znear, zfar;
	navigator_manager->GetFocusedOrFirstNavigator()->GetPerspective(fov_y, aspect_ratio, znear, zfar);	
	navigator_manager->GetFocusedOrFirstNavigator()->GetScreenSize(width, height);
	cpw::Point3d<double> camera_pos = navigator_manager->GetFocusedOrFirstNavigator()->GetCameraController()->GetCamera()->GetUTMPosition();

	scene->AdaptHandlerToModelFixedScreenSize(active_handler, active_entity, cpw::Point3d<float>(0.0f, 0.0f, 0.0f), false,(const int &)( height * 0.66f), camera_pos, znear);

	////scene->AdaptHandlerToModel(active_handler, active_entity, cpw::Point3d<float>(0.0f, 0.0f, 0.0f), false);

	//adapt the brothers
	std::map<std::string, cpw::IHandler *>::iterator iter = handlers.find(active_handler);

	cpw::EntityRegistry *entity_reg = cpw::EntityRegistry::GetInstance();
	cpw::Entity *entity = entity_reg->GetEntity(cpw::TypeId(active_entity));

	iter->second->SetEntityTarget(entity);

	for (std::vector<cpw::IHandler *>::iterator iterb = iter->second->GetBrothers().begin();
	     iterb != iter->second->GetBrothers().end();
	   	 iterb++)
	{

		//scene->AdaptHandlerToModel((*iterb)->GetId(), active_entity, cpw::Point3d<float>(0.0f, 0.0f, 0.0f), false);
	  scene->AdaptHandlerToModelFixedScreenSize((*iterb)->GetId(), active_entity, cpw::Point3d<float>(0.0f, 0.0f, 0.0f), false, (const int &)(height * 0.66f), camera_pos, znear);
		(*iterb)->SetEntityTarget(entity);
	}

	return true;
}


bool HandlerController::UpdateEntityAndBrothersCameraPosition()
{
	if ((scene == NULL) || (!entity_active) || (!handler_active))
		return false;


	std::map<std::string, cpw::IHandler *>::iterator iter = handlers.find(active_handler);

	if (iter != handlers.end())
	{
		cpw::Point3d<double> camera_pos = navigator_manager->GetFocusedOrFirstNavigator()->GetCameraController()->GetCamera()->GetUTMPosition();
		cpw::Point3d<double> camera_view = navigator_manager->GetFocusedOrFirstNavigator()->GetCameraController()->GetCamera()->GetView();

		iter->second->SetCameraPosition(camera_pos);
		iter->second->SetCameraView(camera_view);

		//update brother´s cameras
		for (std::vector<cpw::IHandler *>::iterator iterb = iter->second->GetBrothers().begin();
			 iterb != iter->second->GetBrothers().end();
	   		iterb++)
		{
			(*iterb)->SetCameraPosition(camera_pos);
			(*iterb)->SetCameraView(camera_view);

		}
	}

	return true;
}

bool HandlerController::UpdateActiveHandlerAndBrothersTarget()
{
	if ((scene == NULL) || (!entity_active) || (!handler_active))
		return false;


	std::map<std::string, cpw::IHandler *>::iterator iter = handlers.find(active_handler);

	if (iter != handlers.end())
	{
		cpw::EntityRegistry *entity_reg = cpw::EntityRegistry::GetInstance();
		iter->second->SetEntityTarget(entity_reg->GetEntity(cpw::TypeId(active_entity)));
		iter->second->SetActive(true);

		//update brother´s targets
		for (std::vector<cpw::IHandler *>::iterator iterb = iter->second->GetBrothers().begin();
			 iterb != iter->second->GetBrothers().end();
	   		iterb++)
		{
			(*iterb)->SetEntityTarget(entity_reg->GetEntity(cpw::TypeId(active_entity)));
			(*iterb)->SetActive(true);
		}
	}

	return true;
}


void HandlerController::ClearActiveEntity()
{
	if (entity_active)
		MakeHandlerAndBrothersVisible(active_entity, false);
	
	entity_active = false;
	active_entity = std::string("");

	//clear all targets
	for (std::map<std::string, cpw::IHandler *>::iterator iter = handlers.begin();
			 iter != handlers.end();
			 iter++)
	{
		iter->second->ClearEntityTarget();			
	}

	entity_active = false;


}

void HandlerController::HideAllHandlers()
{
	if (scene == NULL)
		return;

	//hide all the gizmos
	for (std::map<std::string, cpw::IHandler *>::iterator iter = handlers.begin();
		 iter != handlers.end();
		 iter++)
	{
		scene->SetHandlerVisible(iter->first, false);
		//MakeHandlerAndBrothersVisible( iter->first, false);
	}
}

void HandlerController::ShowDefaultGizmoOnActiveEntity()
{
	MakeHandlerAndBrothersVisible(active_handler, false);
	handler_active = entity_active = true;
	active_handler = default_handler->GetId();
	//ShowActiveHandler(true);
	AdaptActiveHandlerAndBrothersToActiveEntity();
	MakeHandlerAndBrothersVisible(active_handler, true);

}

void HandlerController::SetAllowUnderTerrainToAllHandles(const bool &value, const bool &update_entity)
{
	for (std::map<std::string, cpw::IHandler *>::iterator iter = handlers.begin();
		 iter != handlers.end();
		 iter++)
	{
		iter->second->SetAllowGoingUnderTerrain(value);
	}
}
void HandlerController::SetAdaptPositionToTerrainHeightToAllHandles(const bool &value, const bool &update_entity)
{
	//if (navigator_manager == NULL)
	//	return;

	//cpw::Point3d<double> camera_pos = navigator_manager->GetFocusedOrFirstNavigator()->GetCameraController()->GetCamera()->GetUTMPosition();
	//cpw::Point3d<double> camera_view = navigator_manager->GetFocusedOrFirstNavigator()->GetCameraController()->GetCamera()->GetView();

	for (std::map<std::string, cpw::IHandler *>::iterator iter = handlers.begin();
		 iter != handlers.end();
		 iter++)
	{
		//iter->second->SetCameraPosition(camera_pos);
		//iter->second->SetCameraView(camera_view);

		iter->second->SetAdaptPositionToTerrainHeight(value);
		if ((update_entity) && (value == true))
			iter->second->MouseDrag(0, 0, 0, 0, cpw::MouseButtonsState());
	}
}


void HandlerController::Update()
{
	//if (navigator_manager == NULL)
	//	return;

	//cpw::Point3d<double> camera_pos = navigator_manager->GetFocusedOrFirstNavigator()->GetCameraController()->GetCamera()->GetUTMPosition();
	//cpw::Point3d<double> camera_view = navigator_manager->GetFocusedOrFirstNavigator()->GetCameraController()->GetCamera()->GetView();

	if (handler_active)
	{
		std::map<std::string, cpw::IHandler *>::iterator iter = handlers.find(active_handler);

		if (iter != handlers.end())
		{
			//iter->second->SetCameraPosition(camera_pos);
			//iter->second->SetCameraView(camera_view);
			//iter->second->AdaptHandlerAndBrothersToModel();
			iter->second->Update();

		}

	}
}

void HandlerController::UpdateCamera()
{
	if (navigator_manager == NULL)
		return;

	cpw::Point3d<double> camera_pos = navigator_manager->GetFocusedOrFirstNavigator()->GetCameraController()->GetCamera()->GetUTMPosition();
	cpw::Point3d<double> camera_view = navigator_manager->GetFocusedOrFirstNavigator()->GetCameraController()->GetCamera()->GetView();

	for (std::map<std::string, cpw::IHandler *>::iterator iter = handlers.begin();
		 iter != handlers.end();
		 iter++)
	{
		iter->second->SetCameraPosition(camera_pos);
		iter->second->SetCameraView(camera_view);

	}


}


void HandlerController::ShowActiveHandler(bool value) 
{ 
	MakeHandlerAndBrothersVisible(active_handler, value);
}


void HandlerController::SetActiveEntity(const std::string &_active_entity) 
{ 

	//clear all targets
	for (std::map<std::string, cpw::IHandler *>::iterator iter = handlers.begin();
			 iter != handlers.end();
			 iter++)
	{
		iter->second->SetEntityTarget(NULL);			
	}

	//update targets where neccesary
	cpw::Entity *entity = cpw::EntityRegistry::GetInstance()->GetEntity(cpw::TypeId(_active_entity));

	if (entity != NULL)
	{
		active_entity = _active_entity; 
		entity_active = true;

		UpdateActiveHandlerAndBrothersTarget();
	}
	else
	{
		entity_active = false;
		active_entity = std::string("");
	}
}



void HandlerController::ActivateAllHandlers(const bool active)
{
	//clear all targets
	for (std::map<std::string, cpw::IHandler *>::iterator iter = handlers.begin();
			 iter != handlers.end();
			 iter++)
	{
		iter->second->SetActive(active);
	}
}
