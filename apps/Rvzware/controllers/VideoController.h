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

#ifndef _VIDEOCONTROLLER_
#define _VIDEOCONTROLLER_

#include <vector>

#include <cpw/common/Observer.h>
#include <cpw/entity/LayerTree.h>
#include <cpw/entity/Entity.h>
#include <ApplicationTime.h>


namespace cpw
{

  namespace controllers
  {

    /** 
	\brief This controllers is responsible for the management of the animations
	\ingroup cpw
    */
    class VideoController : public cpw::Observer
      {
      public:
				
	VideoController(void);
	~VideoController(void);
				
	virtual void Update(bool subject_deleted = false);
	void SetAnimatedEntityList(std::vector<cpw::Entity *> &entities);
	void GetAnimatedEntityListFromLayerTree(cpw::LayerTree *layer_tree);

	void SetAppTime(ApplicationTime *_app_time) { app_time = _app_time; }
	void SetLayerTree(cpw::LayerTree *lt) { _layer_tree = lt; }
	void SetAllowUpdate(const bool &_allow_update) { allow_update = _allow_update; }

	void UpdateList(); 

						
      private:

	cpw::LayerTree *_layer_tree;
	std::vector<cpw::Entity *> animated_entities;
	ApplicationTime *app_time;
	bool init, allow_update;

      };
  }
}

#endif


