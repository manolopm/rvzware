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

#ifndef _LAYERCONTROLLER_
#define _LAYERCONTROLLER_

#include <wx/wx.h>

#include <controllers/EntityController.h>

#include <gui/UILayer.h>


namespace cpw
{

	namespace gui { class UILayer; }
	namespace controllers 
	{  


		/** 
			\brief This controller is responsible for the management of Layer
			\ingroup cwcontent
		*/
		class LayerController: public EntityController
		{
			public:
				
				LayerController();
				
				virtual ~LayerController();

				void CreateLayer(wxWindow* parent, cpw::LayerTree &layer_tree);
				void InstacePrimitiveLayer(cpw::gui::UILayer *ui_layer_);
				void SetPrimitivesUrl(cpw::gui::UILayer *ui_layer_);
				std::string &GetPrimitiveName(){ return primitive->GetName(); };

				void ModifyProperties(wxWindow* parent, cpw::Entity* ent);

				bool IsOpen();

			private:
				
				cpw::Entity *primitive;
				
		};

	}

}

#endif
