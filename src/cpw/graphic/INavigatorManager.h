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

#ifndef _NAVIGATORMANAGER_
#define _NAVIGATORMANAGER_

#include <cpw/graphic/INavigator.h>
#include <cpw/graphic/ICameraController.h>
#include <cpw/common/types.h>
#include <cpw/callback/IEventCallBack.h>
#include <cpw/common/Observer.h>
#include <cpw/IApplication.h>
#include <cpw/Export.h>


namespace cpw
{
  class INavigator;
  class ICameraController;


  /** 
      \brief Por comentar ....
      \ingroup cpw
  */
  class INavigatorManager : public Observer
  {
  public:


    virtual	~INavigatorManager(){};
    virtual IWindow *AddNavigator(IWindow *parent_frame, int id) = 0;
    virtual void SetDefaultPath(const std::string &url) =0;
    virtual void DeleteNavigator(unsigned int navigator_id) = 0;
    virtual void Draw() = 0;
    virtual void SetDrawingModeWireframe(int navigator_id) = 0;
    virtual void SetDrawingModeSolid(int navigator_id) = 0;
    virtual INavigator * GetFocusedOrFirstNavigator() = 0;
    virtual void SetActive(int id) = 0;
    //virtual void SetMouseEventCallBackOnActiveNavigator(void(*_callback_function)(int, int, int)) = 0;
    virtual void ClearMouseEventCallBackOnNavigators() = 0;
    virtual void SetMouseEventCallBackOnNavigators(cpw::IEventCallBack *event_callback) = 0;
    virtual void Update(bool subject_deleted = false){};
    virtual void CameraParametersToNextNewNavigator(const cpw::Point3d<double> &position,
						    const cpw::Point3d<double> &view,
						    const cpw::Point3d<double> &up) {}

    virtual void Pause(bool pause) = 0;
    virtual void EveryNavigatorsToStartPosition(){}
    virtual void SetCameraController(ICameraController *_camera_controller) {}
    virtual ICameraController * GetCameraController() {return NULL;}

    virtual void SetAllNavigatorsToInitialPosition()=0;
    virtual void SetAllNavigatorsToLookDown()=0;
    virtual void SetAllNavigatorsToLookPos(const Point3d<double> &position)=0;

    virtual void SetApplication(IApplication *_application)=0;

    virtual void SetLightningToAllNavigators(const bool &lightning)=0;
    virtual const bool & GetNavigatorsLightning()=0;

		
			

			
  };

}

#endif 







