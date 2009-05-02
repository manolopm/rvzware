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

//#include <stdafx.h>

#include <cpw/graphic/TwoAxisTraslationGizmo.h>

#include <cpw/graphic/ApplicationScene.h>
#include <cpw/entity/EntityRegistry.h>
#include <cpw/entity/Element.h>
#include <cpw/common/TypeId.h>
#include <cpw/common/Math.h>
#include <cpw/graphic/IScene.h>
#include <cpw/common/ApplicationLog.h>

using namespace cpw;

TwoAxisTraslationGizmo::TwoAxisTraslationGizmo(void) 
{
}

TwoAxisTraslationGizmo::~TwoAxisTraslationGizmo(void)
{
}

bool TwoAxisTraslationGizmo::MouseDrag(const int &x0, const int &y0, const int &x1, const int &y1, const cpw::MouseButtonsState &mbs)
{
  IScene *scene = cpw::ApplicationScene::GetInstance()->GetScene();
  EntityRegistry *entity_reg =EntityRegistry::GetInstance();
  const float dist_mult = 0.0025f;
  float increment = 0.0f;
  float increment_x, increment_y;
  int traslation=0;
	

  increment_y = increment_x = 0.0f;

  if ((GetEntityTarget() == NULL) || (scene == NULL))
    return false;

  Element *entity = (Element *) GetEntityTarget();

  float new_x = entity->GetUtm(0); 
  float new_y = entity->GetUtm(1); 
  float new_z = entity->GetUtm(2);

  cpw::Point3d<double> camera_view = GetCameraView();

  int traslation_x = x1 - x0;
  int traslation_y = y1 - y0;

  if ((traslation_x == 0) && (traslation_y == 0))
    return false;
	
  float distance = cpw::Math::DistanceBetweenTwoPoints(cpw::Point3d<double>(new_x, new_y, new_z), GetCameraPos());

  cpw::Point3d<float> ip;

  GetNavigatorManager()->GetFocusedOrFirstNavigator()->SetKeepMouseInsideCanvas(false);

  //if (!GetNavigatorManager()->GetFocusedOrFirstNavigator()->IntersectMouseWithScene(x1, y1, ix, iy, iz))
  if (entity->GetAdjustToTerrainHeight())
    {
      if (!GetNavigatorManager()->GetFocusedOrFirstNavigator()->IntersectMouseWithScene2(x1, y1, ip))
	{
	  /*std::stringstream cc;
	    cc << "collision failed " << x1 << ", " << y1;
	    if (cpw::ApplicationLog::GetInstance()->GetLogger() != NULL)
	    cpw::ApplicationLog::GetInstance()->GetLogger()->NewLogMessage(cc.str());*/
	
	  return false;
	}
    }
  else
    {
      if (!GetNavigatorManager()->GetFocusedOrFirstNavigator()->IntersectMouseWithHorizontalPlane(x1, y1, ip, entity->GetUtm(2)))
	return false;
    }

  new_x = ip.x;
  new_y = ip.y;
  //new_x = ix;
  //new_y = iy;
	
  //check if we should intersect against the terrain
  if (entity->GetAdjustToTerrainHeight() || entity->GetAllowUnderTerrain())
    {
      cpw::Point3d<float> i_point;
      if (scene->IntersectRayWithTerrain(cpw::Point3d<float>(new_x, new_y, 10000.0f),
					 cpw::Point3d<float>(new_x, new_y, -10000.0f),
					 i_point, true))
	{
	  if (entity->GetAllowUnderTerrain())
	    {
	      if (new_z < i_point.z)
		new_z = i_point.z;
	    }

	  if (entity->GetAdjustToTerrainHeight())
	    new_z = i_point.z;
	}
    }
	
  entity->SetUtm(new_x, new_y, new_z);
	
  entity->GraphicUpdate();
	
  IHandler::AdaptHandlerAndBrothersToModel();	
	
  return true;
}

void TwoAxisTraslationGizmo::Update(bool subject_deleted)
{
  if (subject_deleted)
    {
      if (GetEntityTarget() != NULL)
	{
	  //GetEntityTarget()->Detach(this);
	  ClearEntityTarget();
	}
    }
  else
    IHandler::AdaptHandlerAndBrothersToModel();
}
