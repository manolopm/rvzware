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
#ifndef _ELEMENTCONTROLLER_
#define _ELEMENTCONTROLLER_

#include "wx/wx.h"

#include <controllers/EntityController.h>
#include <controllers/GraphicObjectController.h>
#include <controllers/HandlerController.h>

#include <gui/UIElement.h>
#include <cpw/entity/Element3D.h>
#include <cpw/entity/LayerTree.h>
#include <cpw/graphic/IGraphicFactory.h>
#include <cpw/graphic/IGraphicObjectLoader.h>
#include <cpw/graphic/INavigatorManager.h>
#include <cpw/common/Observer.h>


namespace cpw
{

	namespace gui { class UIElement; }

	namespace controllers 
	{  

		/** 
			\brief This controller is responsible for the management of Element3D
			\ingroup cpw::controllers
		*/
		class ElementController: public EntityController, public cpw::Observer
		{

			public:
				ElementController(wxWindow* paren, cpw::LayerTree &tr_lay, 
								  cpw::IGraphicFactory *graphic_factory, cpw::INavigatorManager *nm, HandlerController *_handler_controller, bool modify = false);
				virtual ~ElementController();

				/** \bug Al crear elemento 3D, el Detach del elemento da un error en el destructor*/  
				/**	\bug Cuando se pulsa la tecla tab en el segundo formulario del asistente, cambia el color de algunas palabras a azul 
					\bug El guizmo no aparece sobre la entidad que se esta insertando
					\bug Al insertar un elemento sin haber hecho click sobre la escena, el objeto se mete en una posicion incorrecta

					\todo Poder quitar el guizmo
				*/
				void CreateElement();

				void CreateElement(const cpw::Entity *primitive);
				
				bool CreateElement(const std::string &name, const std::string &text, 
								   const std::string &html, const std::string &description,
								   float *utm, float *orientation, float *scale,
								   bool dynamic, std::vector<cpw::Attribute> &attributes, bool modify=false);


				void RemoveTempObject();
				void Cancel();
				void DeleteElement();
				void CancelModifications();
				void SetPositionOrientationScale(float  pos_x, float  pos_y, float  pos_z, 
												 float  orientation_x, float  orientation_y, float  orientation_z,
												 float  scale_x, float  scale_y, float  scale_z);
				void SetNamePositionOrientationScale(const std::string &name, float  pos_x, float  pos_y, float  pos_z, 
												 float  orientation_x, float  orientation_y, float  orientation_z,
												 float  scale_x, float  scale_y, float  scale_z);
				void SetName(const std::string &name);

				void ModifyProperties(cpw::Entity* ent);

				virtual void Update(bool subject_deleted = false);

				bool ChangePrimitive(const cpw::TypeId &primitive_id);
				bool ChangePrimitive(const std::string &primitive_url);
				bool InstanceElement(const cpw::Entity *primitive);
				void InsertElementInScene();

				void SetPrimitivesUrl();
				std::string &GetPrimitiveName(){ 
					
					return primitive->GetName(); 
				
				};
				
				void UpdateUIPrimitives(const std::string &selected_primitive = "");

				void UpdateHandlerProperties(bool show_default = false);

				bool IsOpen();

			private:

				wxWindow                     *parent;
				cpw::LayerTree               *layer_tree;
				cpw::IGraphicObjectLoader    *graphic_object_loader;
				cpw::gui::UIElement          *ui_element;
				cpw::INavigatorManager       *navigator_manager;
				cpw::controllers::GraphicObjectController *graphic_object_controller;
				cpw::IGraphic                *graphic;
				std::stringstream			 name;

				cpw::Entity *primitive;
				cpw::Element3D* element3d;
				bool new_element, observing, changing;
				HandlerController *handler_controller;

				cpw::Element3D* old_el3d;
		};

	}
}

#endif 
