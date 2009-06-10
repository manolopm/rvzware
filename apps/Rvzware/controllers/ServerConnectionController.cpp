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

#include <controllers/ServerConnectionController.h>
#include <cpw/ApplicationConfiguration.h>

#include <wms/wmsCapabilitiesState.h>
#include <cpw/entity/Entity.h>
#include <cpw/entity/EntityFactory.h>
#include <ogc/wms/WmsLayer.h>

#include <cpw/common/constants.h>
#include <cpw/common/LatLong-UTMconversion.h>


#include <controllers/PersistentController.h>

using namespace cpw::controllers;


ServerConnectionController::ServerConnectionController(wxWindow* _parent, cpw::LayerTree &tr_lay, bool _build): parent(_parent), 
														layer_tree(&tr_lay), build(_build)
{
  controller = new ogc::WmsLayerController;				
}


ServerConnectionController::~ServerConnectionController()
{
  if (controller)
    {
      delete controller;
      controller = NULL;
    }
}

void ServerConnectionController::FillLayerTree()
{
  ogc::Node* root_node = new ogc::Folder;

  controller->GetLayers(root_node);
  ui_ServerConnection->SetLayers(*root_node);
  if (root_node)
    {
      delete root_node;
      root_node = NULL;
    }
}

void ServerConnectionController::ConnectServer(wxWindow *parent, wxWindowID id, const wxString &title, 
					       const wxPoint& pos, const wxSize& size, long style)
{
  ui_ServerConnection = new cpw::gui::UIServerConnection(this, parent, wxID_ANY);
  std::vector<std::string> servers;

  ReadServersFile(servers);

  ui_ServerConnection->SetServerList(servers);
  ui_ServerConnection->ShowSRS(build);

  if (ui_ServerConnection->ShowModal() == wxID_OK)
    {
      std::vector<std::string> vlayers = ui_ServerConnection->GetSelectedLayers();
      PersistentController persistent;
      ogc::WmsLayer wmsaux;
      ogc::WmsLayer *wmsl;
      cpw::Entity* new_wms_layer = cpw::EntityFactory::GetInstance()->CreateEntity(wmsaux.GetClassName());
      wmsl = (ogc::WmsLayer *)new_wms_layer;


      FillWMSL(vlayers.front(), wmsl);

				
      Register(new_wms_layer);

      layer_tree->AddToActiveLayer(new_wms_layer);

      if(!ui_ServerConnection->Builder())
	cpw::PersistentError ferror = persistent.Save(parent, new_wms_layer, ".cla", false);
				
      new_wms_layer->GraphicInsert();	

    }

  servers = ui_ServerConnection->GetServerList();
  WriteServersFile(servers);
}


int ServerConnectionController::Connect(const std::string &server_url, std::string *abstr)
{		
  int state = controller->ConnectToServer(server_url);
  if (state == 0)
    *abstr = controller->GetRoot()->root()->abstract();
  else
    *abstr = "";
  return state;
}

std::vector<std::string> ServerConnectionController::GetImageFormats(const std::string &folder)
{
  return controller->GetRoot()->getNodeGivenName(folder)->formats();
}

std::vector<std::string> ServerConnectionController::GetSRS(const std::string &folder)
{
  return controller->GetRoot()->getNodeGivenName(folder)->SRS();
}

void ServerConnectionController::ModifyProperties(cpw::Entity* ent)
{
  ogc::WmsLayer* wmsl = (ogc::WmsLayer*)ent;
  std::string abstr;
  int c_status = Connect(wmsl->GetWmsUrl().url(), &abstr);
  if( c_status == 0 )
    {
      ui_ServerConnection = new cpw::gui::UIServerConnection(this, parent, wxID_ANY);
      std::vector<std::string> servers;

      ReadServersFile(servers);

      ui_ServerConnection->SetServerList(servers);

      ui_ServerConnection->Modify(true);
      ogc::Node* root_node = new ogc::Folder;
      controller->GetLayers(root_node);
      ui_ServerConnection->SetConnectionMemo(wmsl->GetWmsUrl().url(), abstr, c_status); 
      ui_ServerConnection->SetLayers(*root_node);
      ui_ServerConnection->SetName(wmsl->GetName());

      if( ui_ServerConnection->ShowModal() == wxID_OK )
	{
	  std::vector<std::string> vlayers = ui_ServerConnection->GetSelectedLayers();

	  FillWMSL(vlayers.front(), wmsl);

	  PersistentController persistent;
	  cpw::PersistentError ferror = persistent.Save(parent, wmsl, ".cla", true);
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


void ServerConnectionController::ReadServersFile(std::vector<std::string> &servers)
{
  std::ifstream wms_file(cpw::ApplicationConfiguration::GetInstance()->GetWmsServerList().c_str());	
  if (wms_file.is_open())
    {
      while (!wms_file.eof())
	{
	  std::string server;
	  wms_file >> server;
	  servers.push_back(server);
	}
      wms_file.close();
    }
}

void ServerConnectionController::ReadSRS(std::vector<std::string> &srs_list)
{
  int p=0;
}

void ServerConnectionController::WriteServersFile(std::vector<std::string> &servers)
{
  std::ofstream wms_ofile(cpw::ApplicationConfiguration::GetInstance()->GetWmsServerList().c_str());	
  if (wms_ofile.is_open())
    {
      std::vector<std::string>::iterator i = servers.begin();
      for(;i!=servers.end();i++)
	{
	  wms_ofile << (*i) << std::endl;
	}
      wms_ofile.close();
    }
}


void ServerConnectionController::FillWMSL(std::string &layer, ogc::WmsLayer* wmsl)
{
  controller->CreateLayer(layer, ui_ServerConnection->GetSelectedFormat(), *wmsl);

  double lat1, lat2, lon1, lon2;

  wmsl->GetLatLonBoundingBox(lat1, lat2, lon1, lon2);

  std::string selected_srs = ui_ServerConnection->GetSelectedSRS();
  std::vector<std::string> utm_vector = wmsl->GetWmsUTMvector();
	
  bool utm=false;

  if(!utm_vector.empty())
    {
      wmsl->SetSRS(selected_srs);
      int sel = 0;
      // seleccionar del utm_vector, las coordenadas asociadas al srs seleccionado
      std::vector<std::string> _srs = wmsl->GetWmsSRS();
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
      //parche porque a veces un vector es mayor que el otro
      if(sel >= utm_vector.size() )
	sel = utm_vector.size()-1;
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

  wmsl->SetUTMLatLonBoundingBox(lat1, lat2, lon1, lon2);

  wmsl->SetName(ui_ServerConnection->GetName());
  std::string &icon_path = ApplicationConfiguration::GetInstance()->GetIconDirectory();
  wmsl->SetIcon(icon_path + "wms.png");
}
