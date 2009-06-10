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
#include <fstream>

#include <stdafx.h>

#include <controllers/WCSServerConnectionController.h>
#include <cpw/ApplicationConfiguration.h>

#include <wcs/wcsCapabilitiesState.h>
#include <cpw/entity/Entity.h>
#include <cpw/entity/EntityFactory.h>
#include <ogcwcs/WcsLayer.h>

#include <cpw/common/constants.h>
#include <cpw/common/LatLong-UTMconversion.h>


#include <controllers/PersistentController.h>

using namespace cpw::controllers;


WCSServerConnectionController::WCSServerConnectionController(wxWindow* _parent, cpw::LayerTree &tr_lay, bool _build): parent(_parent), 
														      layer_tree(&tr_lay), build(_build)
{
  controller = new ogcwcs::WcsLayerController;				
}


WCSServerConnectionController::~WCSServerConnectionController()
{
  if (controller)
    {
      delete controller;
      controller = NULL;
    }
}

void WCSServerConnectionController::FillLayerTree()
{
  ogcwcs::Node* root_node = new ogcwcs::Folder;

  controller->GetLayers(root_node);
  ui_ServerConnection->SetLayers(*root_node);
  if (root_node)
    {
      delete root_node;
      root_node = NULL;
    }
}

void WCSServerConnectionController::ConnectServer(wxWindow *parent, wxWindowID id, const wxString &title, 
						  const wxPoint& pos, const wxSize& size, long style)
{
  ui_ServerConnection = new cpw::gui::UIWCSServerConnection(this, parent, wxID_ANY);
  std::vector<std::string> servers;

  ReadServersFile(servers);

  ui_ServerConnection->SetServerList(servers);
  ui_ServerConnection->ShowSRS(build);

  if (ui_ServerConnection->ShowModal() == wxID_OK)
    {
      std::vector<std::string> vlayers = ui_ServerConnection->GetSelectedLayers();
      PersistentController persistent;
      ogcwcs::WcsLayer wcsaux;
      ogcwcs::WcsLayer *wcsl;
      cpw::Entity* new_wcs_layer = cpw::EntityFactory::GetInstance()->CreateEntity(wcsaux.GetClassName());
      wcsl = (ogcwcs::WcsLayer *)new_wcs_layer;


      FillWCSL(vlayers.front(), wcsl);

				
      Register(new_wcs_layer);

      layer_tree->AddToActiveLayer(new_wcs_layer);

      //if(!ui_ServerConnection->Builder())
      //	cpw::PersistentError ferror = persistent.Save(parent, new_wcs_layer, ".cla", false);
				
      new_wcs_layer->GraphicInsert();	

    }

  servers = ui_ServerConnection->GetServerList();
  WriteServersFile(servers);
}


int WCSServerConnectionController::Connect(const std::string &server_url, std::string *abstr)
{		
  int state = controller->ConnectToServer(server_url);
  if (state == 0)
    *abstr = controller->GetRoot()->root()->abstract();
  else
    *abstr = "";
  return state;
}

std::vector<std::string> WCSServerConnectionController::GetImageFormats(const std::string &folder)
{	
  return controller->GetRoot()->getNodeGivenName(folder)->formats();
}

std::vector<std::string> WCSServerConnectionController::GetSRS(const std::string &folder)
{	

  if (controller->DescribeCoverage(folder))

    return controller->WCS_CRS(folder);

  else

    return std::vector<std::string>();

}

void WCSServerConnectionController::ModifyProperties(cpw::Entity* ent)
{
  ogcwcs::WcsLayer* wcsl = (ogcwcs::WcsLayer*)ent;
  std::string abstr;
  int c_status = Connect(wcsl->GetWcsUrl().url(), &abstr);
  if( c_status == 0 )
    {
      ui_ServerConnection = new cpw::gui::UIWCSServerConnection(this, parent, wxID_ANY);
      std::vector<std::string> servers;

      ReadServersFile(servers);

      ui_ServerConnection->SetServerList(servers);

      ui_ServerConnection->Modify(true);
      ogcwcs::Node* root_node = new ogcwcs::Folder;
      controller->GetLayers(root_node);
      ui_ServerConnection->SetConnectionMemo(wcsl->GetWcsUrl().url(), abstr, c_status); 
      ui_ServerConnection->SetLayers(*root_node);
      ui_ServerConnection->SetName(wcsl->GetName());

      if( ui_ServerConnection->ShowModal() == wxID_OK )
	{
	  std::vector<std::string> vlayers = ui_ServerConnection->GetSelectedLayers();

	  FillWCSL(vlayers.front(), wcsl);

	  //PersistentController persistent;
	  //cpw::PersistentError ferror = persistent.Save(parent, wcsl, ".cla", true);
	}

      servers = ui_ServerConnection->GetServerList();
      WriteServersFile(servers);

      if(root_node != NULL)
	{
	  delete root_node;
	  root_node = NULL;
	}
    }
}


void WCSServerConnectionController::ReadServersFile(std::vector<std::string> &servers)
{
  //std::ifstream wcs_file(cpw::ApplicationConfiguration::GetInstance()->GetWcsServerList().c_str());	
  std::ifstream wcs_file("wcsserverlist.dat");
  if (wcs_file.is_open())
    {
      while (!wcs_file.eof())
	{
	  std::string server;
	  wcs_file >> server;
	  servers.push_back(server);
	}
      wcs_file.close();
    }
}

void WCSServerConnectionController::ReadSRS(std::vector<std::string> &srs_list)
{
  int p=0;
}

void WCSServerConnectionController::WriteServersFile(std::vector<std::string> &servers)
{
  //std::ofstream wcs_ofile(cpw::ApplicationConfiguration::GetInstance()->GetWcsServerList().c_str());	
  std::ofstream wcs_ofile("wcsserverlist.dat");
  if (wcs_ofile.is_open())
    {
      std::vector<std::string>::iterator i = servers.begin();
      for(;i!=servers.end();i++)
	{
	  wcs_ofile << (*i) << std::endl;
	}
      wcs_ofile.close();
    }
}


void WCSServerConnectionController::FillWCSL(std::string &layer, ogcwcs::WcsLayer* wcsl)
{
  controller->CreateLayer(layer, ui_ServerConnection->GetSelectedFormat(), *wcsl);

  double lat1, lat2, lon1, lon2;

  wcsl->GetLatLonBoundingBox(lat1, lat2, lon1, lon2);

  std::string selected_srs = ui_ServerConnection->GetSelectedSRS();
  std::vector<std::string> utm_vector = wcsl->GetWcsUTMvector();
	
  bool utm=false;
  wcsl->SetSRS(selected_srs);

  if(!utm_vector.empty())
    {
      int sel = 0;
      // seleccionar del utm_vector, las coordenadas asociadas al srs seleccionado
      std::vector<std::string> _srs = wcsl->GetWcsSRS();
      std::vector<std::string>::iterator it = _srs.begin();
      int i = 0;
      for( ; it != _srs.end() ; it++)
	{
	  if(selected_srs == *it)
	    {
	      sel = i;
	      break;
	    }
	  i++;
	}


      // una vez seleccionado hay que convertirlo a lat1 lon1 lat2 lon2
      std::string srs_utm = utm_vector[sel];
      std::stringstream  ss;
      ss << srs_utm;
      ss >> lat1 >> lat2 >> lon1 >> lon2;
		
      utm=true;
    }

  //utm = false;
  if(!utm)
    {
      //Conversion to UTM
      int RefEllipsoid = 23;//WGS-84. See list with file "LatLong-UTM conversion.cpp" for id numbers
      UTMLL  utm_aux;
      double UTMNorthing=0.f, UTMEasting=0.f;
      char UTMZone[5];
      utm_aux.LLtoUTM(RefEllipsoid, lat1, lon1, lat1, lon1,UTMZone);
      utm_aux.LLtoUTM(RefEllipsoid, lat2, lon2, lat2, lon2,UTMZone);
    }

  wcsl->SetUTMLatLonBoundingBox(lat1, lat2, lon1, lon2);

  wcsl->SetName(ui_ServerConnection->GetName());
  std::string &icon_path = ApplicationConfiguration::GetInstance()->GetIconDirectory();
  wcsl->SetIcon(icon_path + "wcs.png");
}
