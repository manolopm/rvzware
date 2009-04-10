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
#ifndef _DISTANCECONTROLLER_
#define _DISTANCECONTROLLER_

#include <wx/wx.h>
#include <sstream>

#include "EntityController.h"
#include "GraphicObjectController.h"

#include "../gui/UICalculateDistance.h"
#include <cpw/entity/LayerTree.h>
#include <cpw/entity/ElementVectorial.h>
#include <cpw/graphic/IGraphicFactory.h>
#include <cpw/graphic/IGraphicObjectLoader.h>
#include <cpw/graphic/INavigatorManager.h>
#include <cpw/callback/IEventReceiver.h>


namespace cpw
{

	namespace gui { class UICalculateDistance; }

	namespace controllers 
	{  
		
		class DistanceController;

		/** 
			\brief This controllers measures the distance between a set of coordinate points
			\ingroup cpw
		*/
		class DistanceControllerEventReceiver : public IEventReceiver
		{
			public:
				/** */
				DistanceControllerEventReceiver(void);
				/** */
				~DistanceControllerEventReceiver(void);
				/** */
				DistanceControllerEventReceiver(const DistanceControllerEventReceiver &dcer);
				/** */
				virtual bool MouseLeftButtonDown(const int &x,const int &y);
				/** */
				virtual bool MouseLeftButtonUp(const int &x,const int &y);
				/** */
				virtual bool MouseDrag(const int &x0, const int &y0, const int &x1, const int &y1, const MouseButtonsState &mbs);
				/** */
				virtual void SetNavigatorManager(INavigatorManager *_navigator_manager) { navigator_manager = _navigator_manager; }
				/** */
				virtual cpw::INavigatorManager * GetNavigatorManager() { return navigator_manager; }

				/** */
				void SetDistanceController(DistanceController *_Distance_controller);

			private:
				/** */
				INavigatorManager *navigator_manager;
				/** */
				DistanceController *Distance_controller;

				bool drag;

		};


		/** 
			\brief This class is responsible for measuring distances on the terrain
			\ingroup cpw
		*/
		class DistanceController: public EntityController
		{
			public:
				
				/** */
				DistanceController(wxWindow *_parent,cpw::LayerTree &tr_lay, cpw::IGraphicFactory *graphic_factory, 
							   cpw::INavigatorManager *nm, cpw::IGraphicObjectLoader *loader);
				/** */
				virtual ~DistanceController();

				/** */
				void ReleaseMouseCallBack();
				/** */
				void CreateDistance();
				/** */
				void UpdateUI(const cpw::Point3d<float> &point);

				/** */
				void Cancel();

				/** */
				void RemoveAll();

				//callback function for mouse handler
				/** */
				void MouseEvent(int x, int y, int z);

				/** */
				void SetDistanceIcon(const std::string &icon, const std::string &icon2);

				/** */
				bool IsOpen();

			private:

				std::vector<cpw::Point3d<float>> Distance_array;

				cpw::ElementVectorial *ElementVectorial_entity;

				//controllers::GraphicObjectController *graphic_object_controller;
				gui::UICalculateDistance *ui_CalculateDistance;

				wxWindow* parent;
				cpw::LayerTree *layer_tree;
				cpw::INavigatorManager *navigator_manager;
				//cpw::IGraphicObjectLoader *graphic_object_loader;		
				DistanceControllerEventReceiver *mouse_event;
		};

	}
}

#endif