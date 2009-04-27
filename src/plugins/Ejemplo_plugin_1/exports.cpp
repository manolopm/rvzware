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

#include "../../../apps/capaware/PluginInterface.h"

#include <wx/wx.h>
#include <sstream>
#include <cpw/debugger.h>

#include <cpw/graphic/INavigator.h>
#include <cpw/graphic/INavigatorManager.h>
#include <cpw/persistent/file/PersistentFileCallBack.h>
#include <cpw/entity/Element3D.h>
#include <cpw/callback/Model3DCallBack.h>
#include <cpw/persistent/file/PersistentFileCallBack.h>
#include <cpw/common/types.h>

#include "FireSimulation.h"
#include "FireSimulationController.h"


using namespace cpw;





//--------------------------------------------------------
class CMyApp : public wxApp
{
public:
    bool OnInit();

private:
};

IMPLEMENT_APP_NO_MAIN(CMyApp)

BOOL APIENTRY DllMain(HMODULE Module,DWORD Reason,LPVOID)
{
        if(Reason==DLL_PROCESS_DETACH)
        {			
			if( wxTheApp )
				wxTheApp->OnExit();
			wxEntryCleanup();
        }
        else if(Reason==DLL_PROCESS_ATTACH)
        {
                int argc = 0;
                char** argv = NULL;
                if(!wxEntryStart(argc, argv) ||
                        !wxTheApp ||
                        !wxTheApp->CallOnInit())
                {
					wxEntryCleanup();
					if( wxTheApp )
						wxTheApp->OnExit();

                    return FALSE;
                }
        }
       
        return TRUE;
}

bool CMyApp::OnInit()
{       
    return true;
};

void EntitiesToRegister(std::vector<cpw::Entity*> &v)
{
}

//--------------------------------------------------------


void RegisterMenu(std::string &menu)
{
	menu = "Ejemplo 1: Insertar Elemento 3D estático";
}

void ExecPlugin(std::vector<cpw::Entity*> &ventity, cpw::ApplicationScene *appscene, const std::string &entity_path, const std::string &icon_path, const std::string &data_path, const std::string &language, wxWindow *parent, cpw::INavigatorManager *navigator_manager, cpw::IStatusController *istatus_controller)
{	
	float x, y, z;
	int cx, cy;

	// obtenemos la intersección del centro de pantalla con el terreno
	cpw::INavigator *navigator = navigator_manager->GetFocusedOrFirstNavigator();
	navigator->GetCenterScreenCoords (cx, cy);
	navigator->IntersectMouseWithScene (cx, cy, x, y, z);

	// creamos una nueva excavadora en ese punto
	cpw::Element3D *el = new cpw::Element3D;
	el->AddCallBack(new cpw::Model3DCallBack);
	el->SetPersistentCallBack(new cpw::PersistentFileCallBack);
	el->SetName("Elemento3D_plugin");
	el->SetUtm(x, y, z);
	el->SetOrientation(0, 0, 180);
	el->SetScale(1,1,1);
	el->SetIcon("../../data/icons/excavadora.png");
	el->SetModelUrl("../../data/models/excavator/excavator.osg");

	// insertamos el elemento en la lista de entidades de salida
	ventity.push_back(el);
}

