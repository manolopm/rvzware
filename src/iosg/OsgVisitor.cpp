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

#include <osg/Texture2D>
#include <osg/Geometry>
#include <osg/MatrixTransform>
#include <osg/TexMat>
#include <osgDB/ReadFile>
#include <osg/Timer>
#include <osg/Vec3>


#include <iosg/stdafx.h>

#include <iosg/OsgVisitor.h>
#include <iosg/OsgDrawableData.h>
#include <iosg/OsgScene.h>




using namespace cpw::iosg;

// para poder hacer el printf
#include <cpw/common/ApplicationLog.h>

#define printf cpw::ApplicationLog::GetInstance()->GetLogger()->printf


OsgVisitor::~OsgVisitor()
{
	
	
  //for ( std::vector<OsgDrawableCullCallback *>::iterator i=dcc.begin(); i!=dcc.end(); i++)
  //{
  //	//delete *i;
  //}

  ////dcc.clear();
}

//************************************************************************

void OsgVisitor::DelTextureRect (osg::Texture2D *textura)
{
  MustSetTextureRect = true;
  for (int n=0; n<quemadas.size(); n++) 
    if (quemadas[n].texRect==textura) quemadas.erase (quemadas.begin()+n);
}

//************************************************************************

void OsgVisitor::AddTextureRect (osg::Texture2D *textura, cpw::Point2d<float> P0, cpw::Point2d<float> P1)
{
  MustSetTextureRect = true;
  quemadas.push_back (TextureRect(textura,P0,P1));
	
  /*
  // si no existe la textura, la añadimos
  int n;
  for (n=0; n<quemadas.size(); n++) if (quemadas[n].texRect==textura) break;
  if (n==quemadas.size()) quemadas.push_back(TextureRect(textura,P0,P1));
  */

  /*
    quemadas[0].texRect = textura;
    quemadas[0].texP0 = P0;
    quemadas[0].texP1 = P1;
  */
}

//************************************************************************

void OsgVisitor::apply (osg::Geode &geode)
{		
  // barremos todos sus drawables
  for (unsigned int i=0; i<geode.getNumDrawables(); i++) {
    osg::Drawable *drawable = geode.getDrawable (i);
			
    // leemos el user data con la información
    osg::ref_ptr<OsgDrawableData> data = dynamic_cast <OsgDrawableData *> (drawable->getUserData());

    // si no tiene user data es que es la primera vez, y hay que ponérselo
    if (!data) {

      // ponemos el callback
      drawable->setCullCallback (dcc_active);

      // ponemos user data
      data = new OsgDrawableData (drawable);
      drawable->setUserData (data.get());	

      // lanzamos el blendLOD la primera vez para que ponga la textura del padre (unit 1)
      data->Init();
    } 
			
    // inicializamos el flag visible a false
    data->TestVisibility();

    // si hay que resetear se resetea la textura
    if (dcc_active->MustReset()) {
      //printf ("--reseteando tile %s (estadoWMDS=%d)", data->GetName().c_str(), dcc_active->estadoWMS);
      data->ResetWMS (dcc_active->estadoWMS);
    }

    // si hay que resetear las peticiones pendientes, se hace
    if (dcc_active->MustResetPetPend()) data->ResetPetition();

    /*
    // si estaba esperando por textura WMS y ya llegó, se la jincamos
    osg::Texture2D *textureWMS = dcc_active->GetTextureWMS();
    if (data->PendingPetition() && textureWMS) {			
    printf ("ya llegó la textura para el dr. %s (después de %.2f seg.)", 
    drawable->getName().c_str(), dcc_active->GetWaitedTimeWMS()/1000);
    data->TextureReceived (textureWMS);
    dcc_active->FinishPetition();
    }
    */

    // si estaba esperando por textura WMS y ya llegó, se la jincamos
    if (data->PendingPetition()) {

      /*
      // si el estadoWMS es distinto de 2 y no somos el nodo raiz, hay que cancelar peticiones
      if (!dcc_active->PendingPetitions() || (dcc_active->estadoWMS!=2 && data->GetNivelLOD()>0))
      data->ResetPetition(); 
      else {
      */

      // buscamos si llegó la textura
      osg::ref_ptr<osg::Texture2D> textureWMS = dcc_active->GetTextureWMS(data.get());
      if (textureWMS.get()) {
	data->TextureReceived (textureWMS.get());
	dcc_active->FinishPetition();
	printf ("ya llegó la textura para el dr. %s size=%d (quedan %d pet.pend)", 
		drawable->getName().c_str(), textureWMS.get()->getImage()->s(),
		/*dcc_active->GetWaitedTimeWMS()/1000,*/ 
		dcc_active->PendingPetitions());
      }
    }

    // si soy raiz y hay que poner texrect y aún no hay, la pongo
    if (MustSetTextureRect) {
      if (data->GetNivelLOD()==0) data->SetTextureRect (quemadas);
      else data->ResetTexRect();
    }
  }
		
  // Keep traversing the rest of the scene graph.
  traverse (geode);
}

//***********************************************************************

void OsgVisitor::Launch (osg::Group *root, const unsigned char &blending_factor, 
			 bool wms_active, int cx, int cy,
			 const cpw::Point3d<float> &_eye, const cpw::Point3d<float> &_center, 
			 const cpw::Point3d<float> &_up, const cpw::Point3d<float> &_punto_mira,
			 OsgPetitionManager *_petman, const unsigned char &fire_mode)
/* en cada frame, David me llama con el nodo padre de las islas */
{
  if(dcc.empty()) 
    return;  //PEDRO PROVISIONAL
  //return;

  //static bool first_time = false;
  static osg::Timer_t last_t = osg::Timer::instance()->tick();

  // guardamos el modo de visualización de incendios
  texModo = (fire_mode==1)? 1 : 2;

  // calculamos el tiempo del anterior frame
  osg::Timer_t actual_t = osg::Timer::instance()->tick(); 
  double msec = osg::Timer::instance()->delta_m (last_t, actual_t);
  //printf ("miliseconds = %f    FPS = %.0f", msec, 1000/msec);
  last_t = actual_t;

  // para cada isla lanzamos el visitor
  for (unsigned int i=0; i<root->getNumChildren(); i++) {
		
    // obtenemos la matriz de traslación
    osg::MatrixTransform * mt = (osg::MatrixTransform *) root->getChild(i);
    osg::Vec3f traslation = mt->getMatrix().getTrans();

    // calculamos los parámetros de la cámara
    osg::Vec3f eye_t(_eye.x - traslation.x(), _eye.y - traslation.y(), _eye.z - traslation.z());
    osg::Vec3f center_t(_center.x - traslation.x(), _center.y - traslation.y(), _center.z - traslation.z());
    osg::Vec3f up_t(_up.x, _up.y, _up.z);
    osg::Vec3f punto_mira_t (_punto_mira.x, _punto_mira.y, _punto_mira.z);

    // guardamos el número de la isla, para que el visitor tenga acceso
    dcc_active = dcc[i];

    // guardamos los valores para el dcc
    dcc_active->SetParams (wms_active, cx, cy, eye_t, center_t, up_t, punto_mira_t, 
			   resetWMS, msec, blending_factor);		

    // obtenemos el nodo raiz de la isla
    osg::Node *isla = mt->getChild(0);

    // lanzamos el visitor	
    isla->accept (*this);

    // reseteamos las variable reset del dcc
    dcc_active->ClearResetWMS();
    dcc_active->ClearResetPetPend();
  }

  // reseteamos la variable resetWMS del visitor
  resetWMS = false; 

  // reseteamos la variable MustSetTextureRect
  MustSetTextureRect = false;
}

void OsgVisitor::Reset()
{
  // borramos la lista
  for (unsigned int k=0; k<dcc.size(); k++)
    {
      if (dcc[k])
	{
	  delete dcc[k];
	  dcc[k] = NULL;
	}
    }
  dcc.clear();
}

void OsgVisitor::Init (osg::Group *root, const unsigned char &blending_factor, 
		       bool wms_active, int cx, int cy,
		       const cpw::Point3d<float> &_eye, const cpw::Point3d<float> &_center, 
		       const cpw::Point3d<float> &_up, const cpw::Point3d<float> &_punto_mira,
		       OsgPetitionManager *_petman, const unsigned char &fire_mode)
{
  dcc.clear();
  // metemos todas las islas
  for (unsigned int i=0; i<root->getNumChildren(); i++) {
		
    // obtenemos la matriz de traslación
    osg::MatrixTransform * mt = (osg::MatrixTransform *) root->getChild(i);
    osg::Vec3f traslation = mt->getMatrix().getTrans();

    // creamos un objeto DrawableCullCallback nuevo y lo añadimos al vector
    OsgDrawableCullCallback *pun = new OsgDrawableCullCallback (_petman, this);
    dcc.push_back (pun);

    // obtenemos el nodo raiz de la isla
    osg::Node *isla = mt->getChild(0);

    // guardamos el nombre del fichero osga, para usarlo en los nombres de los drawables
    char nombre[300];
    strcpy (nombre, isla->getName().c_str());
    char *p1 = strrchr (nombre, '\\');
    if (!p1) p1 = nombre;
    char *p2 = strrchr (nombre, '.');
    p2[0] = '\0';
    pun->setName (p1+1);
	
    // le cambiamos el nombre al nodo para reconocerlo
    isla->setName ("RAIZ BLENDING");
	
    // desactivo la luz 
    //osg::StateSet *ss = isla->getOrCreateStateSet();
    //ss->setMode (GL_LIGHTING, osg::StateAttribute::OFF);

    // pongo la traslacion (solo lo hago la primera vez, con lo cual no se podrá mover)
    dcc[i]->traslation = traslation;
  }
}
