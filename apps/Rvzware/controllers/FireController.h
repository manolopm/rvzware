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

#ifndef _FIRECONTROLLER_
#define _FIRECONTROLLER_

#include <wx/wx.h>
#include <sstream>
#include <vector>

#include <controllers/EntityController.h>
#include <controllers/GraphicObjectController.h>

#include <cpwutils/Fire.h>
#include <gui/UIFire.h>

#include <cpw/entity/LayerTree.h>
#include <cpw/graphic/IGraphicFactory.h>
#include <cpw/graphic/IGraphicObjectLoader.h>
#include <cpw/graphic/INavigatorManager.h>
#include <cpw/callback/IEventReceiver.h>



namespace cpw
{
	namespace gui {class UIFire; }

	namespace controllers 
	{  
		class FireController;


		/** 
			\brief This controller is responsible for the management of Fire
			\ingroup cwcontent
		*/
		class FireControllerEventReceiver : public cpw::IEventReceiver
		{
			public:
				/** */
				FireControllerEventReceiver(void);
				/** */
				~FireControllerEventReceiver(void);
				/** */
				FireControllerEventReceiver(const FireControllerEventReceiver &fcer);
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
				void SetFireController(FireController *_fc);

			private:
				/** */
				cpw::INavigatorManager *navigator_manager;
				/** */
				FireController *fc;

				bool drag;

				bool changes;

		};

		/** 
			\brief 
			\ingroup cwcontent
		*/
		class FireController: public EntityController
		{
			public:
				
				FireController(wxWindow *_parent,cpw::LayerTree &tr_lay, cpw::IGraphicFactory *_graphic_factory, 
							   cpw::INavigatorManager *nm, cpw::IGraphicObjectLoader *loader);
				
				virtual ~FireController();

				void ReleaseMouseCallBack();
				void UpdateUI(int active_ignition_point = -1, int active_perimeter = 1, int active_flame = -1);

				void CreateFire();
				bool CreatePermanentFire(const std::string &fire_name, const std::string &fire_texture, const std::string &smoke_texture, const bool &modify);
				void Cancel();
				void UpdateFlame(int active_perimeter, int active_flame);
				void UpdateParticleSystemsTypes();
				void Remove();
				void AddTimeSlice(const unsigned long int &_time);
				void UpdateRender(int active_ignition_point = -1, int active_perimeter = 1, bool force_update=false);
				void ChangeDate(const int &active_perimeter, const wxDateTime &new_time);
				void ChangeTextures(const std::string &fire_tex, const std::string &smoke_tex);
				int  GetFirstKeyFlameIdInTimePeriod(wxDateTime timeMin,wxDateTime timeMax);
				wxDateTime GetKeyFlameTimeStamp(int id);
				void SetKeyFlameTimeStamp(int id, wxDateTime time);
				void RestoreBackup();
				bool GetOverwrite() {return cancel_overwrite;}
				
				void ModifyProperties(cpw::Entity *ent);

				//callback function for mouse handler
				void MouseEvent(int x, int y, int z);

				bool IsOpen();

			private:

				cpw::utils::Fire *fire_entity, *fire_entity_backup;
				std::string str_id;
				std::string parent_str_id;
				bool fire_exists_in_scene;
				GraphicObjectController *graphic_object_controller;
				cpw::gui::UIFire *ui_fire;
				bool update_selected_flame;
				int current_perimeter;
				bool new_fire;
				bool cancel_overwrite;

				wxWindow* parent;
				cpw::LayerTree *layer_tree;
				cpw::INavigatorManager *navigator_manager;
				cpw::IGraphicObjectLoader *graphic_object_loader;		
				cpw::IGraphicFactory *graphic_factory;
				FireControllerEventReceiver *mouse_event;
				
				void UpdateUIIgnitionPoints(int &active_index);
				void UpdateUIFlames(int &active_perimeter, int &active_flame);
				bool NewFlame(const cpw::Point3d<float> &flame_position, const int &time_slice_index);
				void FillUIWithFlameData(cpw::utils::Flame &flame);
				void RemoveIgnitionPoint();
				void RemoveFlame();
				void GraphicFireUpdate(const int &active_perimeter);
				
				int render_ts, render_fl;
		};

	}
}

#endif
