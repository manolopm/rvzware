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




class GraphicE3DCallBack : public Model3DCallBack
{
	public:
		GraphicE3DCallBack(cpw::IScene *appscene): scene(appscene){}
		GraphicE3DCallBack(const GraphicE3DCallBack &gfcb) {}
		~GraphicE3DCallBack(void) {}
		
		virtual void Insert(const Callable* callable) {}
		virtual void Update(Callable* callable) {}
		virtual void Delete(Callable* callable) {}
		virtual void Visualize(Callable* callable, const bool &value) {}

		//GraphicFireCallBack &operator = (const GraphicFireCallBack &gfcb){};
		virtual ICallBack* Clone() {return new GraphicE3DCallBack(*this);}


		virtual void Animate(Callable* callable, cpwTime &time_stamp){
			
			// obtenemos la posición XYZ del elemento
			cpw::Element3D *e3d = (cpw::Element3D *) callable;
			double *pos = e3d->GetUtmd();

			// incrementamos su posición en X
			pos[0] += 1;

			// actualizamos la coordenada z para que no se hunda en el terreno
			cpw::Point3d<double> i_point;
			if (scene->IntersectRayWithTerrain(cpw::Point3d<double>(pos[0], pos[1], 10000.0f),
			                             cpw::Point3d<double>(pos[0], pos[1], -10000.0f),
										 i_point, true))
		    {

					pos[2] = i_point.z;
		    }

			// actualizamos su posición
			e3d->SetUtmd(pos);
			e3d->GraphicUpdate();
			
		}
		
	private:		
		cpw::IScene *scene;
};







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
	menu = "Ejemplo 2: Insertar Elemento 3D animado";
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

	// vamos a animarla
	el->SetAnimateCallBack(new GraphicE3DCallBack(appscene->GetScene()));
	
	// insertamos el elemento en la lista de entidades de salida
	ventity.push_back(el);
}

