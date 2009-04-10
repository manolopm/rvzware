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

#ifndef _WMSBOUNDINGBOXCONTROLLER_
#define _WMSBOUNDINGBOXCONTROLLER_
#include "wx/wx.h"
#include "UIWMSBoundingBox.h"

#include <../../include/cpw/graphic/INavigatorManager.h>
#include <../../include/cpw/callback/IEventReceiver.h>
#include <../../include/cpw/graphic/ApplicationScene.h>

#include <../../include/ogc/WmsLayerController.h>


#include <Application.h>

class Application;
class UIWMSBoundingBox; 
class WMSBoundingBoxController;

		/** 
			\brief This controller is responsible for the management of WMSBoundingBox mouse events
			\ingroup 
		*/
		class BBControllerEventReceiver : public cpw::IEventReceiver
		{
			public:
				/** */
				BBControllerEventReceiver(void);
				/** */
				~BBControllerEventReceiver(void);
				/** */
				BBControllerEventReceiver(const BBControllerEventReceiver &evcer);

				/** */
				virtual bool MouseLeftButtonDown(const int &x,const int &y);
				/** */
				virtual bool MouseLeftButtonUp(const int &x,const int &y);


				/** */
				virtual bool MouseRightButtonDown(const int &x,const int &y);
				/** */
				virtual bool MouseRightButtonUp(const int &x,const int &y);


				/** */
				virtual bool MouseDrag(const int &x0, const int &y0, const int &x1, const int &y1, const cpw::MouseButtonsState &mbs);


				/** */
				virtual void SetNavigatorManager(cpw::INavigatorManager *_navigator_manager) { navigator_manager = _navigator_manager;}
				/** */
				virtual cpw::INavigatorManager * GetNavigatorManager() { return navigator_manager; }

				/** */
				void SetBBController(WMSBoundingBoxController *_WMSBoundingBoxController);

			private:
				/** */
				cpw::INavigatorManager *navigator_manager;
				/** */
				WMSBoundingBoxController *bbc;

				bool drag;
				bool wired;
				int button;



		};


class WMSBoundingBoxController
{
	public:
		
		WMSBoundingBoxController(wxWindow *_parent, cpw::INavigatorManager *nm, cpw::ApplicationScene *_appscene, cpw::ogc::WmsLayer* _wmsl, Application* _app);
		~WMSBoundingBoxController(void);

		void Init(double &x_min, double &y_min, double &x_max, double &y_max);

		void GetCoords(double &left_up_x, double &left_up_y, double &bottom_right_x, double &bottom_right_y);
		bool AreCoordsOk() { return coords_ok; }

		void SetCoordsOk(const bool _coords_ok);

		void SetTopLeftLat(const double &value);
		void SetTopLeftLon(const double &value);
		void SetBotRightLat(const double &value);
		void SetBotRightLon(const double &value);
		void SetResolution(int res){resol = res;}

		
		void NewPetition();
		void NewPetition(std::string& _url);
		void OK(std::string& _url);
		void Download();
		void DownloadWCS();
		void Cancel();

		
		/** Release the mouse callback*/
		void ReleaseMouseCallBack();
		/** Callback function for mouse handler*/
		void MouseEvent(int x, int y, int z);
		/** Callback function for draggin handler*/
		void DragEvent(int x, int y);
		/** Callback function for end draggin handler*/
		void EndDragEvent(int x, int y);


	private:

		void ProcessURL(std::string _url);

		Application* app;
		cpw::ApplicationScene *appscene;
		UIWMSBoundingBox *ui_wms_bb;
		wxWindow* parent;

		float px, py, pz;
		double lux, luy, brx, bry;
		double xmin, xmax, ymin, ymax;
		int w, h;
		int resol;

		bool coords_ok;
		bool force;

		cpw::INavigatorManager *navigator_manager;
		BBControllerEventReceiver *mouse_event;
		cpw::ogc::WmsLayer* wmsl;

		cpw::ogc::WmsLayerController wmsc;

		float img_resolution;
};

#endif