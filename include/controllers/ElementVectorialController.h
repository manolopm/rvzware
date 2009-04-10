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
#ifndef _ELEMENTVECTORIALCONTROLLER_
#define _ELEMENTVECTORIALCONTROLLER_

#include <wx/wx.h>
#include <sstream>

#include "EntityController.h"
#include "GraphicObjectController.h"

#include "../gui/UIElementVectorial.h"

#include <cpw/entity/LayerTree.h>
#include <cpw/entity/ElementVectorial.h>
#include <cpw/graphic/IGraphicFactory.h>
#include <cpw/graphic/IGraphicObjectLoader.h>
#include <cpw/graphic/INavigatorManager.h>
#include <cpw/callback/IEventReceiver.h>


namespace cpw
{
	namespace gui { class UIElementVectorial; }

	namespace controllers 
	{  
		class ElementVectorialController;


		/** 
			\brief This controller is responsible for the management of ElementVectorial mouse events
			\ingroup cwcontent
		*/
		class EVControllerEventReceiver : public cpw::IEventReceiver
		{
			public:
				/** */
				EVControllerEventReceiver(void);
				/** */
				~EVControllerEventReceiver(void);
				/** */
				EVControllerEventReceiver(const EVControllerEventReceiver &evcer);
				/** */
				virtual bool MouseLeftButtonDown(const int &x,const int &y);
				/** */
				virtual bool MouseLeftButtonUp(const int &x,const int &y);
				/** */
				virtual bool MouseDrag(const int &x0, const int &y0, const int &x1, const int &y1, const cpw::MouseButtonsState &mbs);
				/** */
				virtual void SetNavigatorManager(cpw::INavigatorManager *_navigator_manager) { navigator_manager = _navigator_manager; }
				/** */
				virtual cpw::INavigatorManager * GetNavigatorManager() { return navigator_manager; }

				/** */
				void SetDistanceController(ElementVectorialController *_ElementVectorialController);

			private:
				/** */
				cpw::INavigatorManager *navigator_manager;
				/** */
				ElementVectorialController *evc;

				bool drag;

		};


		/** 
			\brief This controller is responsible for the management of Element Vectorial
			\ingroup cwcontent
		*/
		class ElementVectorialController: public EntityController
		{
			public:
				/** Constructor*/
				ElementVectorialController(wxWindow *_parent,cpw::LayerTree &tr_lay, cpw::IGraphicFactory *graphic_factory, 
							   cpw::INavigatorManager *nm, cpw::IGraphicObjectLoader *loader);
				/** Destructor*/
				virtual ~ElementVectorialController();

				/** Release the mouse callback*/
				void ReleaseMouseCallBack();
				/** Create a new vectorial element*/
				void CreateElementVectorial();
				/** Update the user interface*/
				void UpdateUI(const cpw::Point3d<float> &point);
				/** Update the user interface*/
				void UpdateUIPoint(const int &index, const bool &change_point = true);

				/** Create or modify a vectorial element*/
				void CreatePermanentElementVectorial(const std::string &ElementVectorial_name, const std::string &color, const int &width, const bool &modify);
				/** Cancel the vectorial element creation*/
				void Cancel();
				/** Cancel the vectorial element modifications*/
				void CancelModifications();
				/** Remove a vectorial element*/
				void Remove(const int &index);
				/** Show the modify user interface*/
				void ModifyProperties(cpw::Entity *ent);
				/** Set colour to the vectorial element*/
				void SetRGBColour(const int &r, const int &g, const int &b);

				
				/** Callback function for mouse handler*/
				void MouseEvent(int x, int y, int z);

				/** Set name*/
				void SetVectorialName(const std::string &name);
				/** Set icon*/
				void SetVectorialIcon(const std::string &icon, const std::string &icon2);

				/** */
				bool IsOpen();

			private:

				/** */
				std::vector<cpw::Point3d<float>> ElementVectorial_array;

				/** */
				cpw::ElementVectorial *ElementVectorial_entity;
				/** */
				cpw::ElementVectorial *ElementVectorial_old;

				/** */
				std::string str_id;
				/** */
				std::string parent_str_id;

				/** */
				GraphicObjectController *graphic_object_controller;
				/** */
				cpw::gui::UIElementVectorial *ui_ElementVectorial;

				/** */
				wxWindow* parent;
				/** */
				cpw::LayerTree *layer_tree;
				/** */
				cpw::INavigatorManager *navigator_manager;
				/** */
				cpw::IGraphicObjectLoader *graphic_object_loader;		
				/** */
				EVControllerEventReceiver *mouse_event;

				/** */
				std::string vname;
				/** */
				std::string vicon, vicon_child;
		};

	}
}

#endif