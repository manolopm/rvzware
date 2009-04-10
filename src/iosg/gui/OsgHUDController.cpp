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

#include <iosg/gui/OsgHUDController.h>
#include <iosg/OsgNavigator.h>



using namespace cpw::iosg;

OsgIHUDController::OsgIHUDController(void) : navigator_manager(NULL), active_widget(NULL), application(NULL), minimap_active(true), osg_navigator(NULL)
{
	TSpecificFunctor<OsgIHUDController> *fire_functor = new TSpecificFunctor<OsgIHUDController>(this, &OsgIHUDController::OpenFireDialog);

	callbacks[std::string("fire_button")] = fire_functor;
	callbacks[std::string("fire_button")]->Call();
}

OsgIHUDController::~OsgIHUDController(void)
{
	std::map<std::string, TFunctor *>::iterator i = callbacks.begin();

	for ( ; i != callbacks.end(); i++)
		delete i->second;
}

bool OsgIHUDController::MouseLeftButtonDown(const int &x,const int &y)
{
	if (navigator_manager == NULL)
		return false;

	OsgNavigator *navigatora = (OsgNavigator *) navigator_manager->GetFocusedOrFirstNavigator();

	if (osg_navigator == NULL)
		return false;

	cpw::Point3d<float> i_point, local_i_point;
	std::string collision_object_id;

	if (navigatora->IntersectMouseWithHUD(x, y, i_point, collision_object_id, local_i_point))
	{
		osg_hud->SetActiveWidget(collision_object_id);

		std::map<std::string, TFunctor *>::iterator it = callbacks.find(collision_object_id);
		
		if (it != callbacks.end())
		{
			
			osg_hud->GetActiveWidget()->StartPickAnimation(0.5);
			it->second->Call();
			
			
			return true;
		}
		else
		{
			osg_hud->GetActiveWidget()->SetLocalIntersectionPoint(local_i_point);
			return osg_hud->GetActiveWidget()->MouseLeftButtonDown(x, y);
		}
	}

	return false;
}

bool OsgIHUDController::MouseLeftDoubleClick(const int &x, const int &y)
{
	if (navigator_manager == NULL)
		return false;

	OsgNavigator *navigator = (OsgNavigator *) navigator_manager->GetFocusedOrFirstNavigator();

	if (navigator == NULL)
		return false;

	cpw::Point3d<float> i_point, local_i_point;
	std::string collision_object_id;

	if (navigator->IntersectMouseWithHUD(x, y, i_point, collision_object_id, local_i_point))
	{
		osg_hud->SetActiveWidget(collision_object_id);

		std::map<std::string, TFunctor *>::iterator it = callbacks.find(collision_object_id);
		
		if (it != callbacks.end())
		{
			osg_hud->GetActiveWidget()->StartPickAnimation(0.5);
			it->second->Call();
			
			return true;
		}
		else
		{
			osg_hud->GetActiveWidget()->SetLocalIntersectionPoint(local_i_point);
			return osg_hud->GetActiveWidget()->MouseLeftDoubleClick(x, y);
		}
	}

	return false;
}
bool OsgIHUDController::MouseRightDoubleClick(const int &x,const int &y)
{
	if (active_widget != NULL)
		return active_widget->MouseRightDoubleClick(x, y);
	else
		return false; 
}
bool OsgIHUDController::MouseMiddleDoubleClick(const int &x,const int &y)
{
	if (active_widget != NULL)
		return active_widget->MouseMiddleDoubleClick(x, y);
	else
		return false;
}
			
bool OsgIHUDController::MouseRightButtonDown(const int &x,const int &y)
{
	if (active_widget != NULL)
		return active_widget->MouseRightButtonDown(x, y);
	else
		return false; 
}

bool OsgIHUDController::MouseMiddleButtonDown(const int &x,const int &y)
{
	if (active_widget != NULL)
		return active_widget->MouseMiddleButtonDown(x, y);
	else
		return false; 
}

bool OsgIHUDController::MouseLeftButtonUp(const int &x,const int &y)
{
	if (active_widget != NULL)
		return active_widget->MouseLeftButtonUp(x, y);
	else
		return false;

}

bool OsgIHUDController::MouseRightButtonUp(const int &x,const int &y)
{
	if (active_widget != NULL)
		return active_widget->MouseRightButtonUp(x, y);
	else
		return false; 
}

bool OsgIHUDController::MouseMiddleButtonUp(const int &x,const int &y)
{
	if (active_widget != NULL)
		return active_widget->MouseMiddleButtonUp(x, y);
	else
		return false; 
}

bool OsgIHUDController::MouseDrag(const int &x0, const int &y0, const int &x1, const int &y1)
{
	if (active_widget != NULL)
		return active_widget->MouseDrag(x0, y0, x1, y1);
	else
		return false; 
}
bool OsgIHUDController::MouseWheel(const int &wheel_rotation, const int &wheel_delta)
{
	if (active_widget != NULL)
		return active_widget->MouseWheel(wheel_rotation, wheel_delta);
	else
		return false; 
}

bool OsgIHUDController::MouseLeaveWindow(const int &x0, const int &y0)
{
	active_widget = false;
	return false; 
}
bool OsgIHUDController::MouseEnterWindow(const int &x0, const int &y0)
{
	active_widget = false;
	return false; 
}
					
bool OsgIHUDController::KeyDown(const int &key)
{
	if (active_widget != NULL)
		return active_widget->KeyDown(key);
	else
		return false; 
}
bool OsgIHUDController::KeyUp(const int &key)
{
	if (active_widget != NULL)
		return active_widget->KeyUp(key);
	else
		return false; 
}



void OsgIHUDController::AddCallBack(const std::string &callback_id, void(OsgIHUDController::*fpt)())
{
	TSpecificFunctor<OsgIHUDController> *new_functor = new TSpecificFunctor<OsgIHUDController>(this, fpt);

	std::map<std::string, TFunctor *>::iterator i = callbacks.find(callback_id);

	if (i != callbacks.end())
		delete i->second;

	callbacks[callback_id] = new_functor;

}


void OsgIHUDController::OpenWMSDialog()
{
	if (application != NULL)
	{
		application->NewWmsLayer();
	}

}
void OsgIHUDController::OpenElement3dDialog()
{
	if (application != NULL)
	{
		application->NewElement();
	}
}

void OsgIHUDController::OpenFireDialog()
{
	if (application != NULL)
	{
		application->NewFire();
	}
}

void OsgIHUDController::ToggleMinimap()
{
	//if (mini_map->IsVisible())
	//	mini_map->SetVisible(false);
	//else
	//	mini_map->SetVisible(true);
	mini_map->StartToggleAnimation();

}

void OsgIHUDController::ToggleCompass()
{
	if (compass->IsVisible())
		compass->SetVisible(false);
	else
		compass->SetVisible(true);
	//compass->StartToggleAnimation();

}

void OsgIHUDController::SetMinimap(OsgIMinimap *_mini_map){ mini_map = _mini_map; }

void OsgIHUDController::SetCompass(OsgICompass *_compass){ compass = _compass; }

void OsgIHUDController::SetNavigationPanel(OsgINavigationPanel *_navigation_bar){ navigation_bar = _navigation_bar; }

void OsgIHUDController::ToggleNavigationPanel()
{
	if (navigation_bar!=NULL)
	{
		navigation_bar->StartToggleAnimation();
	}
}