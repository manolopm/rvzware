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

#include "stdafx.h"

#include "WMSBoundingBoxController.h"

#include <../../include/ogcwcs/WcsLayerController.h>

#define DIM 1000
#define DIM_ERR 200
#define MAXDIST 10000
#define MAXDIST_ERR 2000


BBControllerEventReceiver::BBControllerEventReceiver(void): drag(false), wired(false), button(0)
{
	
}

BBControllerEventReceiver::~BBControllerEventReceiver(void)
{

}

BBControllerEventReceiver::BBControllerEventReceiver(const BBControllerEventReceiver &bber)
{

}

bool BBControllerEventReceiver::MouseLeftButtonDown(const int &x,const int &y)
{
	drag = false;
	button = 1;
	bbc->MouseEvent(x, y, wxMOUSE_BTN_LEFT);

	return false;

}

bool BBControllerEventReceiver::MouseLeftButtonUp(const int &x,const int &y)
{
	button = 0;
	if(drag)
	{
		drag = 0;
		bbc->EndDragEvent(x, y);
	}
	return false;
}

bool BBControllerEventReceiver::MouseRightButtonDown(const int &x,const int &y)
{
	drag = false;
	button = 2;

	return false;

}

bool BBControllerEventReceiver::MouseRightButtonUp(const int &x,const int &y)
{
	drag = false;
	button = 0;
	return false;
}

bool BBControllerEventReceiver::MouseDrag(const int &x0, const int &y0, const int &x1, const int &y1, const cpw::MouseButtonsState &mbs)
{
	drag = true;
	if(button == 1)
	{
		bbc->DragEvent(x1, y1);
	}

	return false;
}

void BBControllerEventReceiver::SetBBController(WMSBoundingBoxController *_WMSBoundingBoxController)
{ 
	bbc = _WMSBoundingBoxController;
}





WMSBoundingBoxController::WMSBoundingBoxController(wxWindow *_parent, cpw::INavigatorManager *nm, cpw::ApplicationScene *_appscene, cpw::ogc::WmsLayer* _wmsl, Application* _app) : 
				ui_wms_bb(NULL), parent(_parent), lux(0.0), luy(0.0), brx(0.0), bry(0.0), coords_ok(false), navigator_manager(nm),
				appscene(_appscene), img_resolution(1.0), wmsl(_wmsl), wmsc(), w(1024), h(1024), resol(1), xmin(0.0), xmax(0.0), ymin(0.0), ymax(0.0), force(false),
				app(_app)
												
{
	px = py = pz = 0.0f;
	ui_wms_bb = new UIWMSBoundingBox(this, parent);
	
	// the mouse event callback
	mouse_event = new BBControllerEventReceiver();
	mouse_event->SetBBController(this);
	mouse_event->SetNavigatorManager(navigator_manager);
	cpw::INavigator *navigator = navigator_manager->GetFocusedOrFirstNavigator();
	int nc = navigator->GetEventReceiversCount();
	if (nc>0)
		navigator->SetEventReceiver(nc-1, mouse_event);

}

WMSBoundingBoxController::~WMSBoundingBoxController(void)
{
	ReleaseMouseCallBack();

	if(mouse_event != NULL)
	{
		delete mouse_event;
		mouse_event = NULL;
	}

	if(ui_wms_bb != NULL)
	{
		delete ui_wms_bb;
		ui_wms_bb = NULL;
	}

}

void WMSBoundingBoxController::Init(double &x_min, double &y_min, double &x_max, double &y_max)
{
	std::stringstream ssxmin, ssymin, ssxmax, ssymax;

	ssxmin.precision(15);
	ssymin.precision(15);
	ssxmax.precision(15);
	ssymax.precision(15);

	ssxmin << x_min;
	ssymin << y_min;
	ssxmax << x_max;
	ssymax << y_max;

	ui_wms_bb->SetLonMin(ssxmin.str());
	ui_wms_bb->SetLatMin(ssymin.str());
	ui_wms_bb->SetLonMax(ssxmax.str());
	ui_wms_bb->SetLatMax(ssymax.str());

	ui_wms_bb->SetURL(wmsc.GetMapURL(*wmsl, 1024, 1024, (float)x_min, (float)y_min,(float)x_max, (float)y_max));

	ui_wms_bb->Show();

}

void WMSBoundingBoxController::GetCoords(double &left_up_x, double &left_up_y, double &bottom_right_x, double &bottom_right_y)
{
	left_up_x = lux;
	left_up_y = luy;
	bottom_right_x = brx;
	bottom_right_y = bry;
}


void WMSBoundingBoxController::SetCoordsOk(const bool _coords_ok) { coords_ok = _coords_ok; }
void WMSBoundingBoxController::SetTopLeftLat(const double &value) { lux = value; }
void WMSBoundingBoxController::SetTopLeftLon(const double &value) { luy = value; }
void WMSBoundingBoxController::SetBotRightLat(const double &value) { brx = value; }
void WMSBoundingBoxController::SetBotRightLon(const double &value) { bry = value; }


void WMSBoundingBoxController::ReleaseMouseCallBack()
{
	if(navigator_manager != NULL)
	{
		cpw::INavigator *navigator = navigator_manager->GetFocusedOrFirstNavigator();
		navigator->RemoveEventReceiver(mouse_event);
	}
}


void WMSBoundingBoxController::MouseEvent(int x, int y, int z)
{
	float ipx, ipy, ipz;
	ipx=ipy=ipz=0.0f;
	
	switch (z)
	{
		case wxMOUSE_BTN_LEFT:
			{	
				appscene->GetScene()->Remove("box");

				std::string entity_id;

				if (navigator_manager->GetFocusedOrFirstNavigator()->IntersectMouseWithScene(x,y, ipx, ipy, ipz))
				{
					px = ipx;
					py = ipy;
					pz = ipz;
					
				}
				return;
			}
			break; //END LEFT BUTTON
		
		case wxMOUSE_BTN_MIDDLE:
			break;

		case wxMOUSE_BTN_RIGHT:
			break;
	}

}


void WMSBoundingBoxController::DragEvent(int x, int y)
{
	float ipx, ipy, ipz;
	ipx=ipy=ipz=0.0f;
	
	if (navigator_manager->GetFocusedOrFirstNavigator()->IntersectMouseWithScene(x,y, ipx, ipy, ipz))
	{
		std::stringstream ss_x, ss_y;
		ss_x.precision(15);
		ss_y.precision(15);
		ss_x << px;
		ss_y << py;
		ui_wms_bb->SetLatMin(ss_x.str());
		ui_wms_bb->SetLonMin(ss_y.str());


		std::stringstream ssx, ssy;
		ssx.precision(15);
		ssy.precision(15);
		ssx << ipx;
		ssy << ipy;
		ui_wms_bb->SetLatMax(ssx.str());
		ui_wms_bb->SetLonMax(ssy.str());


		std::stringstream ssw, ssh;
		ssw << (int)abs(px-ipx);
		ssh << (int)abs(py-ipy);
		ui_wms_bb->SetBBWidth(ssw.str());
		ui_wms_bb->SetBBHeight(ssh.str());

	}

	std::string name("box");
	std::vector<cpw::Point3d<float>> points;

	points.push_back(cpw::Point3d<float>(px, py, 60));
	points.push_back(cpw::Point3d<float>(ipx, py, 60));
	points.push_back(cpw::Point3d<float>(ipx, ipy, 60));
	points.push_back(cpw::Point3d<float>(px, ipy, 60));
	points.push_back(cpw::Point3d<float>(px, py, 60));

	appscene->GetScene()->AddLine3(name, points, "red", 255, 0, 0, 1, true);
	return;

}


void WMSBoundingBoxController::EndDragEvent(int x, int y)
{
	double x0, y0, x1, y1;

	float ipx, ipy, ipz;
	ipx=ipy=ipz=0.0f;
	
	if (navigator_manager->GetFocusedOrFirstNavigator()->IntersectMouseWithScene(x,y, ipx, ipy, ipz))
	{
		lux = px;
		luy = py;
		brx = ipx;
		bry = ipy;

		if((luy < bry)  &&  (lux < brx))
		{
			x0 = luy; y0 = lux; x1 = bry; y1 = brx;
		}
		else if((luy > bry)  &&  (lux < brx))
		{
			x0 = bry; y0 = lux; x1 = luy; y1 = brx;
		}
		else if((luy < bry)  &&  (lux > brx))
		{
			x0 = luy; y0 = brx; x1 = bry; y1 = lux;
		}
		else if((luy > bry)  &&  (lux > brx))
		{
			x0 = bry; y0 = brx; x1 = luy; y1 = lux;
		}

		std::stringstream ss_x, ss_y;
		ss_x.precision(15);
		ss_y.precision(15);
		ss_x << x0;
		ss_y << y0;
		ui_wms_bb->SetLatMin(ss_y.str());
		ui_wms_bb->SetLonMin(ss_x.str());


		std::stringstream ssx, ssy;
		ssx.precision(15);
		ssy.precision(15);
		ssx << x1;
		ssy << y1;
		ui_wms_bb->SetLatMax(ssy.str());
		ui_wms_bb->SetLonMax(ssx.str());

		ui_wms_bb->SetURL(wmsc.GetMapURL(*wmsl, 1024, 1024, (float)x0, (float)y0,(float)x1, (float)y1));	
	}
}



void WMSBoundingBoxController::NewPetition()
{
	appscene->GetScene()->Remove("box");
	//ReleaseMouseCallBack();

	double x0, x1, y0, y1;

	if((luy < bry)  &&  (lux < brx))
	{
		x0 = luy; y0 = lux; x1 = bry; y1 = brx;
	}
	else if((luy > bry)  &&  (lux < brx))
	{
		x0 = bry; y0 = lux; x1 = luy; y1 = brx;
	}
	else if((luy < bry)  &&  (lux > brx))
	{
		x0 = luy; y0 = brx; x1 = bry; y1 = lux;
	}
	else if((luy > bry)  &&  (lux > brx))
	{
		x0 = bry; y0 = brx; x1 = luy; y1 = lux;
	}

	ui_wms_bb->SetURL(wmsc.GetMapURL(*wmsl, 1024, 1024, (float)x0, (float)y0,(float)x1, (float)y1));

	appscene->GetScene()->BuildScene(x0, y0, x1, y1, 1024, 1024, force, true);

	img_resolution = 1.0;

	double dist = std::max(abs(x1-x0), abs(y1-y0))*2;
	navigator_manager->SetAllNavigatorsToLookPos(cpw::Point3d<double>( (y0+y1)/2 , (x0+x1)/2 , dist) );

}

void WMSBoundingBoxController::NewPetition(std::string& _url)
{
	appscene->GetScene()->Remove("box");
	//ReleaseMouseCallBack();

	ProcessURL(_url);

	appscene->GetScene()->BuildScene(ymin,xmin , ymax, xmax, w, h, force, true);

	double dist = std::max( abs(xmax-xmin), abs(ymax-ymin))*2.0;
	navigator_manager->SetAllNavigatorsToLookPos(cpw::Point3d<double>(  (xmin+xmax)/2, (ymin+ymax)/2, dist) );

}

void WMSBoundingBoxController::ProcessURL(std::string _url)
{
	//for find
	_url.append("&");

	// Calculate WIDTH and HEIGHT
	int pos1 = _url.find("WIDTH=");
	int pos2 = _url.find("HEIGHT=");
	int pos3 = _url.find_first_of('&',pos1);
	int pos4 = _url.find_first_of('&',pos2);

	std::stringstream sub1, sub2;
	//int w, h;
	sub1 << _url.substr(pos1+6,pos3-pos1-6);
	sub1 >> w;
	sub2 << _url.substr(pos2+7,pos4-pos2-7);
	sub2 >> h;
	
	if(pos1 == -1 || pos2 == -1 || pos3 == -1 || pos4 == -1)
	{
		wxMessageDialog message1(NULL,wxString("Cannot process WIDTH or HEIGHT parameter."), wxString("Capabuilder"),wxICON_WARNING |wxOK);				
		message1.ShowModal();
		return;
	}

	//Calculate the lat lon
	int pos_bb = _url.find("BBOX=");
	int pos_coma1 = _url.find_first_of(',',pos_bb);
	int pos_coma2 = _url.find_first_of(',',pos_coma1+1);
	int pos_coma3 = _url.find_first_of(',',pos_coma2+1);
	int pos_endbb = _url.find_first_of('&',pos_coma3);

	std::stringstream sub_xmin, sub_xmax, sub_ymin, sub_ymax;


	sub_xmin << _url.substr(pos_bb+5, pos_coma1-pos_bb-5);
	sub_xmin >> xmin;
	
	sub_ymin << _url.substr(pos_coma1+1, pos_coma2-pos_coma1-1);
	sub_ymin >> ymin;
	
	sub_xmax << _url.substr(pos_coma2+1, pos_coma3-pos_coma2-1);
	sub_xmax >> xmax;

	sub_ymax << _url.substr(pos_coma3+1, pos_endbb-pos_coma3-1);
	sub_ymax >> ymax;

	if(pos_bb == -1 || pos_coma1 == -1 || pos_coma2 == -1 || pos_coma3 == -1 || pos_endbb == -1)
	{
		wxMessageDialog message1(NULL,wxString("Cannot process the bounding box."), wxString("Capabuilder"),wxICON_WARNING |wxOK);				
		message1.ShowModal();
		return;
	}

	ui_wms_bb->SetLatMin(sub_xmin.str());
	ui_wms_bb->SetLonMin(sub_ymin.str());
	ui_wms_bb->SetLatMax(sub_xmax.str());
	ui_wms_bb->SetLonMax(sub_ymax.str());


	// Calculate SRS
	int pos_srs = _url.find("SRS=");
	int pos_endsrs = _url.find_first_of('&',pos_srs);

	std::stringstream sub_srs;

	sub_srs << _url.substr(pos_srs+4,pos_endsrs-pos_srs-4);

	if(pos_srs == -1 || pos_endsrs == -1 )
	{
		wxMessageDialog message1(NULL,wxString("Cannot find SRS."), wxString("Capabuilder"),wxICON_WARNING |wxOK);				
		message1.ShowModal();
		return;
	}

	force = !(wmsl->GetSRS() == sub_srs.str());

	wmsl->SetSRS(sub_srs.str());

	//force = true;  //temp


	ui_wms_bb->SetURL(wmsc.GetMapURL(*wmsl, w, h,(float)ymin ,(float)xmin ,(float)ymax,(float)xmax ));

}



void WMSBoundingBoxController::OK(std::string& _url)
{
	appscene->GetScene()->Remove("box");

	ProcessURL(_url);


	
	cpw::Entity* ent = app->GetWCS();  //Select WCS Server and Layer

	if(ent != NULL)
	{
		//appscene->GetScene()->GetWCS(xmin, ymin, xmax, ymax, 10, 10); // Get WCS image
		DownloadWCS();

		cpw::ogcwcs::WcsLayer* wcsl;
		cpw::ogcwcs::WcsLayerController wcsc;

		wcsl = (cpw::ogcwcs::WcsLayer*)ent;

		std::stringstream wcsdir;
		wcsdir << wcsl->GetId();

		std::string wcsurl = wcsc.GetMapURL(*wcsl, 25, 25, ymin, xmin, ymax, xmax, wcsl->GetSRS());

		//SAVE THE REQUEST	
		std::stringstream wmsdir;
		wmsdir << wmsl->GetId();
		std::ofstream fout(std::string(wmsdir.str()+"\\project.cbp").c_str());
		if(fout.is_open()) 
		{
			fout << wmsdir.str() << "\n";
			fout << _url << "\n";

			fout << wcsdir.str() << "\n";
			fout << wcsurl << "\n";
			fout.close();
		}

		Download();
	}
	else
	{
		wxMessageDialog message1(NULL,wxString("Cannot start process. The WCS server failed."), wxString("Capabuilder"),wxICON_WARNING |wxOK);
		message1.ShowModal();		
	}
}

void WMSBoundingBoxController::DownloadWCS()
{
	//appscene->GetScene()->GetWCS(xmin, ymin, xmax, ymax, 10, 10); // Get WCS image

	int resx = 25;
	int resy = 25;
	


	appscene->GetScene()->SetCoordsMinMax(xmin, ymin, xmax, ymax);

	double distx, disty;
	distx = abs(xmax - xmin);
	disty = abs(ymax - ymin);

	int divx, divy;
	divx = divy = 0;

	while(distx>(MAXDIST+MAXDIST_ERR))
	{
		divx++;
		distx = distx/2;
	}

	while(disty>(MAXDIST+MAXDIST_ERR))
	{
		divy++;
		disty = disty/2;
	}

	double temp_xmin, temp_ymin, temp_xmax, temp_ymax;

	
	std::map<int, std::vector<unsigned long int>> request_dem;

	

	int k=0;
	if(divx == 0) divx--;
	if(divy == 0) divy--;

	for(int b=0; b<=divy+1; b++)
	{
		for(int a=0; a<=divx+1 ; a++)
		{
			//
			temp_xmin = xmin+(a*MAXDIST);
			temp_ymin = ymin+(b*MAXDIST);
			if(divx==0 || a == divx+1)
				temp_xmax = xmax;
			else
				temp_xmax = xmin+MAXDIST+(a*MAXDIST);
			if(divy==0 || b == divy+1)
				temp_ymax = ymax;
			else
				temp_ymax = ymin+MAXDIST+(b*MAXDIST);
			

			request_dem[k].push_back( appscene->GetScene()->GetWCS(temp_xmin, temp_ymin, temp_xmax, temp_ymax, resx, resy) );	
			k++;
				
		}
		
	}
	appscene->GetScene()->SetRequestDEMMap(request_dem);

	

}



void WMSBoundingBoxController::Download()
{
	appscene->GetScene()->SetCoordsMinMax(xmin, ymin, xmax, ymax);

	double distx, disty;
	distx = abs(xmax - xmin);
	disty = abs(ymax - ymin);

	int divx, divy;
	divx = divy = 0;

	while(distx>(MAXDIST+MAXDIST_ERR))
	{
		divx++;
		distx = distx/2;
	}

	while(disty>(MAXDIST+MAXDIST_ERR))
	{
		divy++;
		disty = disty/2;
	}

	double temp_xmin, temp_ymin, temp_xmax, temp_ymax;

	std::map<int,std::vector<unsigned long int>> request_map;

	if(divx == 0) divx--;
	if(divy == 0) divy--;
	
	for(int b=0; b<=divy+1; b++)
	{
		for(int a=0; a<=divx+1 ; a++)
		{
			//
			temp_xmin = xmin+(a*MAXDIST);
			temp_ymin = ymin+(b*MAXDIST);
			if(divx==0 || a == divx+1)
				temp_xmax = xmax;
			else
				temp_xmax = xmin+MAXDIST+(a*MAXDIST);
			if(divy==0 || b == divy+1)
				temp_ymax = ymax;
			else
				temp_ymax = ymin+MAXDIST+(b*MAXDIST);
			//
			double end_i, end_j;
			end_i = temp_xmin;
			end_j = temp_ymin;
			double last_dim;

			for(double j=temp_ymin ; (j+DIM+DIM_ERR) < temp_ymax ; j+=DIM)
			{
				end_j = j;
				for(double i=temp_xmin ; (i+DIM+DIM_ERR) < temp_xmax ; i+=DIM)
				{
					end_i = i;
					request_map[a+(b*(divx+2))].push_back( appscene->GetScene()->BuildScene(j, i, j+DIM, i+DIM, DIM, DIM, false, false) );
					
				}
				end_i+=DIM;
				if((end_i<temp_xmax)&&(end_i+DIM+DIM_ERR > temp_xmax))
				{
					last_dim = temp_xmax - end_i;
					request_map[a+(b*(divx+2))].push_back( appscene->GetScene()->BuildScene(end_j, end_i, end_j+DIM, end_i+last_dim, last_dim, DIM, false, false) ); 
				}				
			}

			end_j+=DIM;
			if((end_j<temp_ymax)&&(end_j+DIM+DIM_ERR > temp_ymax))
			{
				double last_dim_j = temp_ymax - end_j;
				for(double i=temp_xmin ; (i+DIM+DIM_ERR) < temp_xmax ; i+=DIM)
				{
					end_i = i;
					request_map[a+(b*(divx+2))].push_back( appscene->GetScene()->BuildScene(end_j, i, end_j+last_dim_j, i+DIM, DIM, last_dim_j, false, false) );			
				}
				end_i+=DIM;
				if((end_i<temp_xmax)&&(end_i+DIM+DIM_ERR > temp_xmax))
				{
					double last_dim_i = temp_xmax - end_i;
					request_map[a+(b*(divx+2))].push_back( appscene->GetScene()->BuildScene(end_j, end_i, end_j+last_dim_j, end_i+last_dim_i, last_dim_i, last_dim_j, false, false) ); 
				}
			}			
		}
	}

	appscene->GetScene()->SetRequestMap(request_map);

	
}



void WMSBoundingBoxController::Cancel()
{
	appscene->GetScene()->Remove("box");
	appscene->GetScene()->Clear();

}
