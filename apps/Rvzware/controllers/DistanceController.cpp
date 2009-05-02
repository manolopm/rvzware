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
#include <vector>
#include <wx/listbox.h>
#include <sstream>

#include <stdafx.h>

#include <controllers/DistanceController.h>


#include <cpw/ApplicationConfiguration.h>

#include <cpw/common/IdGenerator.h>
#include <cpw/common/types.h>
#include <cpw/common/IdGenerator.h>
#include <cpw/entity/EntityRegistry.h>
#include <cpw/entity/EntityFactory.h>
#include <cpw/graphic/ApplicationScene.h>
#include <cpw/common/Math.h>

using namespace cpw::controllers;


/******************************************************************************************************************/
DistanceControllerEventReceiver::DistanceControllerEventReceiver(void): drag(false)
{
	
}

DistanceControllerEventReceiver::~DistanceControllerEventReceiver(void)
{

}

DistanceControllerEventReceiver::DistanceControllerEventReceiver(const DistanceControllerEventReceiver &dcer)
{

}

bool DistanceControllerEventReceiver::MouseLeftButtonDown(const int &x,const int &y)
{
  drag = false;

  return false;

}
bool DistanceControllerEventReceiver::MouseLeftButtonUp(const int &x,const int &y)
{

  if(!drag) 
    Distance_controller->MouseEvent(x, y, wxMOUSE_BTN_LEFT);
		
  return false;
}

bool DistanceControllerEventReceiver::MouseDrag(const int &x0, const int &y0, const int &x1, const int &y1, const cpw::MouseButtonsState &mbs)
{
  drag = true;

  return false;
}

void DistanceControllerEventReceiver::SetDistanceController(DistanceController *_Distance_controller)
{ 
  Distance_controller = _Distance_controller;
}


/******************************************************************************************************************/

DistanceController::DistanceController(wxWindow* _parent, cpw::LayerTree &tr_lay, cpw::IGraphicFactory *graphic_factory, cpw::INavigatorManager *nm, cpw::IGraphicObjectLoader *loader) : 
  parent(_parent), layer_tree(&tr_lay), navigator_manager(nm), ElementVectorial_entity(NULL), Distance_array()
  //graphic_object_loader(loader)
{
  ui_CalculateDistance = new cpw::gui::UICalculateDistance(this,_parent);
	
  // the mouse event callback
  mouse_event = new DistanceControllerEventReceiver();
  mouse_event->SetDistanceController(this);
  mouse_event->SetNavigatorManager(navigator_manager);
  cpw::INavigator *navigator = navigator_manager->GetFocusedOrFirstNavigator();
  int nc = navigator->GetEventReceiversCount();
  if (nc>0)
    navigator->SetEventReceiver(nc-1, mouse_event);



  //graphic_object_controller = new cpw::controllers::GraphicObjectController(graphic_object_loader, navigator_manager);
}


DistanceController::~DistanceController()
{
  if (ui_CalculateDistance != NULL)
    {
      delete ui_CalculateDistance;
      ui_CalculateDistance = NULL;
    }
	
  //if (graphic_object_controller != NULL)
  //{
  //	delete graphic_object_controller;
  //	graphic_object_controller = NULL;
  //}

  //if(graphic_object_loader != NULL)
  //{
  //	delete graphic_object_loader;
  //	graphic_object_loader = NULL;
  //}

  if(ElementVectorial_entity != NULL)
    {
      cpw::ElementVectorial *temp = (cpw::ElementVectorial*)cpw::EntityRegistry::GetInstance()->GetEntity(ElementVectorial_entity->GetId());
      if(temp == NULL)
	{

	  delete ElementVectorial_entity;
	  ElementVectorial_entity = NULL;

	}
    }

  ReleaseMouseCallBack();
  if(mouse_event != NULL)
    {
      delete mouse_event;
      mouse_event = NULL;
    }
	
}

void DistanceController::ReleaseMouseCallBack()
{
  if(navigator_manager != NULL)
    {
      cpw::INavigator *navigator = navigator_manager->GetFocusedOrFirstNavigator();
      navigator->RemoveEventReceiver(mouse_event);
    }
}


void DistanceController::MouseEvent(int x, int y, int z)
{
  //cpw::Point3d<float> intersection_point(0.0f, 0.0f, 0.0f);
  float ipx, ipy, ipz;
  cpw::INavigator *navigator = navigator_manager->GetFocusedOrFirstNavigator();

	
  switch (z)
    {
    case wxMOUSE_BTN_LEFT:
      {	
	std::string entity_id;
	if (navigator->IntersectMouseWithEntities(x, y, entity_id))
	  return;				
				
	if(!navigator->IntersectMouseWithScene(x, y, ipx, ipy, ipz)) 
	  return;
				
	Distance_array.push_back(cpw::Point3d<float>(ipx,ipy,ipz));
			
	ElementVectorial_entity->SetPoints(Distance_array);
	if(ElementVectorial_entity->GetNumPoints() <= 2)
	  ElementVectorial_entity->GraphicInsert();
	else
	  {
	    ElementVectorial_entity->UpdatePoint(-1);
	    ElementVectorial_entity->GraphicUpdate();
	  }
				
	if(Distance_array.size() > 1)
	  {
	    float d = ui_CalculateDistance->GetDistance();
	    float newd = cpw::Math::DistanceBetweenTwoPoints(Distance_array[Distance_array.size()-2],Distance_array[Distance_array.size()-1]);
	    ui_CalculateDistance->SetDistance(d+newd);
	    ui_CalculateDistance->Refresh();
	    ui_CalculateDistance->Update();
	  }


	UpdateUI(cpw::Point3d<float>(ipx,ipy,ipz));
				
	return;
      }
      break; //END LEFT BUTTON
		
    case wxMOUSE_BTN_MIDDLE:
      break;

    case wxMOUSE_BTN_RIGHT:
      break;
    }

}

void DistanceController::CreateDistance()
{
  cpw::ElementVectorial ElementVectorial;
  ElementVectorial_entity = (cpw::ElementVectorial *) cpw::EntityFactory::GetInstance()->CreateEntity(ElementVectorial.GetClassName());
	
  if (ElementVectorial_entity == NULL) return;
	
  std::ostringstream stro_id;

  ElementVectorial_entity->SetIconVisibility(false);	
  ElementVectorial_entity->SetLine(true);
  ui_CalculateDistance->Show(true);
}

void DistanceController::Cancel()
{

  ReleaseMouseCallBack();

  if (ElementVectorial_entity != NULL)
    {
      ElementVectorial_entity->GraphicDelete();

      delete ElementVectorial_entity;
      ElementVectorial_entity = NULL;
    }
}

void DistanceController::UpdateUI(const cpw::Point3d<float> &point)
{
  if(Distance_array.empty()) return;

  cpw::Point3d<float> p = Distance_array.back();

  std::ostringstream aux_str;
  aux_str.precision(15);
	
  aux_str << p.x;
  ui_CalculateDistance->SetPositionX(aux_str.str());
  aux_str.str("");

  aux_str << p.y;
  ui_CalculateDistance->SetPositionY(aux_str.str());
  aux_str.str("");

  aux_str << p.z;
  ui_CalculateDistance->SetPositionZ(aux_str.str());
  aux_str.str("");		
}

void DistanceController::RemoveAll()
{
  if(Distance_array.empty()) return;
  std::vector<cpw::Point3d<float> >::iterator iter=Distance_array.begin();

  Distance_array.clear();
	
  ui_CalculateDistance->SetDistance(0.0f);

  ElementVectorial_entity->SetPoints(Distance_array);
  ElementVectorial_entity->GraphicDelete();
}

void DistanceController::SetDistanceIcon(const std::string &icon, const std::string &icon2)
{
}

bool DistanceController::IsOpen()
{
  if(ui_CalculateDistance != NULL)
    return ui_CalculateDistance->IsShown();
  else
    return false;
}
