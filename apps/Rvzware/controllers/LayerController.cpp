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

#include <string>
#include <sstream>
#include <vector>

#include <stdafx.h>

#include <controllers/LayerController.h>
#include <controllers/PersistentController.h>

#include <gui/UILayer.h>

#include <cpw/entity/Layer.h>
#include <cpw/entity/ContainerLayer.h>
#include <cpw/entity/EntityFactory.h>

  

using namespace cpw::controllers;


LayerController::LayerController(): primitive(NULL)
{

}

LayerController::~LayerController()
{
}


void LayerController::SetPrimitivesUrl(cpw::gui::UILayer *ui_layer_)
{
	cpw::ContainerLayer clp;
	std::map<cpw::TypeId, cpw::Entity *> prototypes = cpw::EntityFactory::GetInstance()->GetPrototypes(clp.GetClassName());
	std::map<std::string, cpw::TypeId> uiv;

	for(std::map<cpw::TypeId, cpw::Entity *>::iterator i = prototypes.begin(); i != prototypes.end(); i++)
		uiv[(i->second->GetName())] = i->first; 

	ui_layer_->SetPrimitivesUrl(uiv);	
}

void LayerController::InstacePrimitiveLayer(cpw::gui::UILayer *ui_layer_)
{
		std::string primitive_url;
		cpw::TypeId primitive_id = ui_layer_->GetId();
		primitive = cpw::EntityFactory::GetInstance()->GetPrototype(primitive_id);

		if(primitive == NULL)
		{
			primitive_url = ui_layer_->GetUrl();
			primitive = cpw::EntityFactory::GetInstance()->GetPrototypeFromUrl(primitive_url);

			if (primitive == NULL && primitive_url != "")
			{
				PersistentController persistent;
				cpw::PersistentError error = persistent.Load((cpw::Persistent**)&primitive, primitive_url);
				if(error == cpw::PERSISTENTOK)
				{
					cpw::EntityFactory::GetInstance()->Register(primitive);
				}
				else
				{
				  wxMessageDialog message1(NULL,wxT("Cannot load the primitive."),
							   wxT("Capaware"),wxICON_WARNING |wxOK);				
					message1.ShowModal();
				}
			}
		}
		
		if (primitive !=NULL)
			ui_layer_->SetAttributes(primitive->GetAttributes());

}


void LayerController::CreateLayer(wxWindow* parent, cpw::LayerTree &layer_tree)
{
	cpw::gui::UILayer ui_layer (this, parent, wxID_ANY, wxString(_T("Layer")), wxDefaultPosition); 
	
	bool stop = false;

	cpw::ContainerLayer clp;

	std::map<cpw::TypeId, cpw::Entity *> prototypes = cpw::EntityFactory::GetInstance()->GetPrototypes(clp.GetClassName());
	std::map<std::string, cpw::TypeId> uiv;
	
	for(std::map<cpw::TypeId, cpw::Entity *>::iterator i = prototypes.begin(); i != prototypes.end(); i++)
		uiv[(i->second->GetName())] = i->first; 

	std::stringstream name;
	name << "Layer" << cpw::EntityFactory::GetInstance()->GetEntityNumber();
	ui_layer.SetLayerName(name.str());
	ui_layer.SetPrimitivesUrl(uiv);	

	if (ui_layer.ShowModal() == wxID_OK)
	{
		cpw::Entity* layer;
		layer = cpw::EntityFactory::GetInstance()->CreateEntity(primitive->GetId());

		if(layer != NULL)
		{
			layer->SetName(ui_layer.GetName());
			layer->SetPrimitiveUrl(primitive->GetUrl());
			layer->SetHtml(ui_layer.GetHTML());
			layer->SetDynamic(ui_layer.IsDynamic());
			layer->SetDescription(ui_layer.GetDescription());
			layer->SetAttributes(ui_layer.GetAttributes());

			Register(layer);

			layer_tree.AddToActiveLayer(layer);

			PersistentController persistent;
			cpw::PersistentError ferror = persistent.Save(parent, layer, ".cla", false);
			
			
			ui_layer.Show(false);

			layer->GraphicInsert();
		}
		else
		{
		  wxMessageDialog message1(NULL,wxT("Cannot allocate memory for the layer.\nClose other applications and try again."),
					   wxT("Warning"),wxICON_EXCLAMATION |wxYES);				
			message1.ShowModal();
		}
	} 
}

void LayerController::ModifyProperties(wxWindow* parent, cpw::Entity *ent)
{
	cpw::ContainerLayer* cl = (cpw::ContainerLayer*) ent;

	cpw::gui::UILayer ui_layer (this, parent, wxID_ANY, wxString(_T("Layer")), wxDefaultPosition);

	ui_layer.Modify(true);
	ui_layer.SetAttributes(cl->GetAttributes());
	ui_layer.SetLayerName(cl->GetName());
	ui_layer.SetHtml(cl->GetHtml());
	ui_layer.SetDescription(cl->GetDescription());
	ui_layer.DisableNonEditable();

	if(ui_layer.ShowModal() == wxID_OK)
	{
		cl->SetName(ui_layer.GetName());
		cl->SetHtml(ui_layer.GetHTML());
		cl->SetDescription(ui_layer.GetDescription());
		cl->SetAttributes(ui_layer.GetAttributes());

		PersistentController persistent;
		cpw::PersistentError ferror = persistent.Save(parent, cl, ".cla", true);		
	}
	ui_layer.Show(false);
}


bool LayerController::IsOpen()
{
	return false;
}
