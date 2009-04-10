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
#ifndef _NAVIGATOR_
#define _NAVIGATOR_

#include <cpw/graphic/INavigatorManager.h>
#include <cpw/graphic/ICameraController.h>
#include <cpw/callback/IEventCallBack.h>
#include <cpw/callback/IEventReceiver.h>
#include <cpw/Export.h>

namespace cpw
{
	class INavigatorManager;
	class ICameraController;

	/** 
		\brief Por comentar ....
		\ingroup cpw
	*/
	class INavigator
	{
		public:

			virtual	~INavigator(){};
			virtual void SetDefaultPath(const std::string &url)=0;
			virtual void SetNavigatorManager(cpw::INavigatorManager *nm) = 0;
			virtual void SetID(int id) = 0;
			virtual void *GetViewer() = 0;
			virtual int  GetId() = 0;
			virtual bool IntersectMouseWithScene(int mousex, int mousey, float &x, float &y, float &z) = 0;
			virtual bool IntersectMouseWithScene2(int mousex, int mousey, cpw::Point3d<float> &i_point) = 0;
			virtual bool IntersectMouseWithHorizontalPlane(int mousex, int mousey, cpw::Point3d<float> &i_point, const double &plane_height) = 0;

			virtual bool IntersectMouseWithEntities(int mousex, int mousey, std::string &entity_id)=0;
			virtual bool IntersectMouseWithGizmos(int mousex, int mousey, std::string &gizmo_id)=0;
			virtual bool IntersectMouseWithXYPlane(int mousex, int mousey, cpw::Point3d<float> &i_point){return false;}
			
			virtual void GetCenterScreenCoords(int &coordx, int &coordy) = 0;
			virtual void SetMouseEventCallBack(cpw::IEventCallBack *event_callback) = 0;
			virtual bool SetEventReceiver(const unsigned int &position, cpw::IEventReceiver *event_receiver) = 0;
			virtual unsigned int GetEventReceiversCount() = 0;
			virtual bool RemoveEventReceiver(cpw::IEventReceiver *event_receiver) = 0;
			virtual void ClearMouseEventCallBack() = 0;
			virtual void SetCameraController(ICameraController *_camera_controller) = 0;
			virtual ICameraController * GetCameraController() = 0;
			virtual void SetFocus()=0;
			virtual void GoBackToInitialPosition()=0;
			virtual bool GetPerspective(double &fovy, double &aspect_ratio, double &_near, double &_far) = 0;
			virtual void GetScreenSize(int &x, int &y) = 0;
			virtual void LookDown()=0;
			virtual void ShowHUD(bool s)=0;
			virtual void SetIllumination(const bool &illumination)=0;

			virtual void SetKeepMouseInsideCanvas(const bool &keep_mouse_inside_canvas)=0;

			virtual void Clear() = 0;
			
	};
}
#endif 