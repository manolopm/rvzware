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
#include <osg/MatrixTransform>
#include <osg/Vec3>

#include "stdafx.h"

#include <cpw/graphic/ApplicationScene.h>

#include <iosg/OsgScene.h>
#include <iosg/OsgDrawableCullCallback.h>
#include <iosg/OsgDrawableData.h>



#define MIN_WMS_TEXT_RESOL 256
#define MAX_WMS_TEXT_RESOL 512

#define MAX_TIME_OUT		7

#define BLENDING_DELAY		500		// in milliseconds, never zero

#define WMS_PETICIONES_EN_PARALELO

using namespace cpw::iosg;

#include <cpw/common/ApplicationLog.h>
#define printf cpw::ApplicationLog::GetInstance()->GetLogger()->printf


void OsgDrawableCullCallback::InsertaDrawable (OsgDrawableData *data)
{
	OsgDrawableData  *dr;

	// miramos que lod tiene
	int lod = data->GetNivelLOD();

	// si tiene abuelo y no se ha pedido, lo metemos
	if (lod > 1) {
		dr = data->GetData (lod-2);
		if (dr->GetTexWMS() < 2) {
			if (!dr->PendingPetition()) datas.push_back (dr);
			return;
		}
	}

	// si tiene padre y no se ha pedido, lo metemos
	if (lod > 0) {
		dr = data->GetData (lod-1);
		if (dr->GetTexWMS() < 2) {
			if (!dr->PendingPetition()) datas.push_back (dr);
			return;
		}
	}

	// si ya tiene petición pendiente, retornamos
	if (data->PendingPetition()) return;

	// si es nodo hoja, lo metemos siempre (quizás necesite una textura de mayor resolución)
	if (data->GetTipoNodo() == 2) {
		datas.push_back (data);

	} else {

		// si no es nodo hoja, y aún no tiene textura, lo metemos
		if (data->GetTexWMS() < 2) datas.push_back (data);
	}
}


//*****************************************************************

float OsgDrawableCullCallback::GetDistance (osg::Vec3d punto)
{
	return sqrt (	(eye.x()-punto.x()) * (eye.x()-punto.x())+
					(eye.y()-punto.y()) * (eye.y()-punto.y())+
					(eye.z()-punto.z()) * (eye.z()-punto.z()));
}
	
//*****************************************************************


void OsgDrawableCullCallback::FinishPetition()
{
	// si vengo del estado 1, tengo que resetear todos los drawables
	if (estadoWMS==1) resetWMS = true;

	// pasamos a estado 2
	estadoWMS = 2;

	// medimos el tiempo para el futuro time out
	t0_wms = osg::Timer::instance()->tick(); 
}

//*****************************************************************

osg::ref_ptr<osg::Texture2D> OsgDrawableCullCallback::GetTextureWMS (OsgDrawableData *data)
{
	// leemos el id_wms del data
	unsigned long id_wms = data->GetId_wms();

	// buscamos una petición pendiente con ese id
	unsigned int i;
	for (i=0; i<pets.size(); i++)
		if (pets[i].id_wms == id_wms) break;

	// si se encontró una, retornamos la textura
	if (i<pets.size()) {

		// copiamos la textura a devolver
		osg::ref_ptr<osg::Texture2D> TextureWMS = pets[i].TextureWMS;

		// si ya llegó, borramos esa petición de la lista
		if (TextureWMS.get()) pets.erase (pets.begin()+i);

		// retornamos la textura
		return TextureWMS;

	} else {

		// cancelamos la petición pendiente del data
		data->ResetPetition();

		// retornamos null
		return (osg::ref_ptr<osg::Texture2D>) NULL;
	}
}


void OsgDrawableCullCallback::CancelPetitions()
{
	// si no hay peticiones pendiente retornamos
	//if (!PET_PEND) return;
	if (pets.empty()) return;
	
	// reseteamos la variable
	//PET_PEND = false;
	
	// borramos la lista de peticiones
	pets.clear();

	// cancelamos las peticiones del petman
	petman->Clear();
	printf ("cancelando peticiones...");
}

//*****************************************************************

bool OsgDrawableCullCallback::cull(osg::NodeVisitor* nv, osg::Drawable* drawable, osg::State* /*state*/) const
{	
	// leemos el user data con la información
	osg::ref_ptr <OsgDrawableData> data = dynamic_cast <OsgDrawableData *> (drawable->getUserData());

	// procesamos el drawable
	data->Process (nv);

	return false;
}

//*****************************************************************

OsgDrawableCullCallback::~OsgDrawableCullCallback()
{
	
}

//*****************************************************************


void OsgDrawableCullCallback::SetParams (bool wms_active, int cx, int cy, 
											osg::Vec3f &_eye, osg::Vec3f &_center, osg::Vec3f &_up, 
											osg::Vec3f &_punto_mira, bool _resetWMS,
											double msec, const unsigned char &_blending)
{
	// guardamos los parámetros
	eye = _eye;	// posición de la cámara
	punto_mira = _punto_mira; // punto de corte con la escena
	width2 = cx; // mitad del ancho de la imagen, en pixels
	height2 = cy; // mitad del alto de la imagen, en pixels

	// calculamos los 3 ejes del sistema de referencia de la cámara
	vx = _up.x();
	vy = _up.y();
	vz = _up.z();
	wx = _center.x() - _eye.x();
	wy = _center.y() - _eye.y();
	wz = _center.z() - _eye.z();
	ux = wy*vz - wz*vy;
	uy = wz*vx - wx*vz;
	uz = wx*vy - wy*vx;

	// calculamos la distancia focal (mirar perspectiva.tex)
	// tendría que usar la función UTMToSceneCoords, pero no sé como hacerlo desde aquí
	//cpw::Point3d<float> P1 = cpw::ApplicationScene::GetInstance()->GetScene()->UTMToSceneCoords(cpw::Point3d<float>(punto_mira.x(), punto_mira.y(), punto_mira.z()));

	cpw::Point3d<double> out = ((OsgScene *)cpw::ApplicationScene::GetInstance()->GetScene())->UTMToSceneCoords(
			cpw::Point3d<double>(punto_mira.x() - traslation.x(), 
								punto_mira.y() - traslation.y(), 
								punto_mira.z() - traslation.z()));
	double x1=out.x, y1=out.y, z1=out.z;

	/* ahora convertimos a utm con la llamada de David
	double x1 = punto_mira.x() - traslation.x();
	double y1 = punto_mira.y() - traslation.y() - 3e6;
	double z1 = punto_mira.z() - traslation.z();
	*/

	double num = x1*wx + y1*wy + z1*wz - eye.x()*wx - eye.y()*wy - eye.z()*wz;
	double den = x1*vx + y1*vy + z1*vz - eye.x()*vx - eye.y()*vy - eye.z()*vz; 
	D = - height2 / 3.0 * num / den;
	
	// si alquien picó en el tree layer, _resetWMS está a true, y pasamos al estado 3
	if (_resetWMS) {
		switch (estadoWMS) {
			case 1: CancelPetitions();
					resetWMS = true;
					break;
			case 2: estadoWMS = 3;
					CancelPetitions();
					// según si quedan capas o no, pasamos al estado 3 o seguimos en el estado 2
					if (wms_active) {
						estadoWMS=1;
						//resetWMS = true;
					} else {
						estadoWMS=3;
					}
					break;
			case 3: CancelPetitions();
					break;
		}
	}

	// colocamos el estado WMS actual (ver grafo libreta y algoritmoWMS.doc)
	if (wms_active) {
		if (estadoWMS == 0) estadoWMS = 1;
		//if (pets.empty()) ComputeNextPetition();
		ComputeNextPetition();
	} else {
		switch (estadoWMS) {
			case 1: estadoWMS = 0;
					CancelPetitions();
					resetWMS = true;
					break;
			case 2: estadoWMS = 3;
					CancelPetitions();
					break;
		}
	}

   // si hay peticiones pendientes, miramos si han llegado
	if (!pets.empty() && estadoWMS>0 && estadoWMS<3) {

			// si alguna textura no la reclamo nadie, las descartamos
			for (int i=(int)pets.size()-1; i>=0; i--) {
				if (pets[i].TextureWMS.get()) {
					pets.erase (pets.begin()+i);
					printf ("hubo una textura que no reclamó nadie. Quedan %d pendientes", pets.size());
				}
			}

			// si se quedó vacía reiniciamos el flag
			//if (pets.empty()) PET_PEND = false;

			
			//instead of getting and releasing the mutex in every "IsPetitionAttended" call,
			//better call this function to get it only once 
			petman->GainSafeAccess(); 

			// miramos si alguna petición pendiente ha llegado
			for (unsigned int i=0; i<pets.size(); i++) {
				bool result = petman->IsPetitionAttended (pets[i].id_wms, pets[i].TextureWMS);
				if (!result) printf ("hubo una petición errónea");
			}
			petman->ReleaseSafeAccess(); //release mutex
	}

	// calculamos el nuevo factor de blending
	if (last_blending != _blending) {

		// si han movido el slider, cogemos el nuevo valor
		last_blending = blending = _blending;

	} else {

		// si no, lo hacemos en función del tiempo
		switch (estadoWMS) {
			case 0: blending = 0; break;
			case 2: if (blending < _blending) {
						int inc = msec * 255 / BLENDING_DELAY;
						if (inc==0) inc = 1;
						int bl_i = (int) blending + inc;
						blending = (bl_i > _blending)? _blending : (unsigned char) bl_i;
					}
					break;
			case 3: if (blending > 0) {
						int inc = msec * 255 / BLENDING_DELAY;
						if (inc==0) inc = 1;
						int bl_i = (int) blending - inc;
						blending = (bl_i < 0)? 0 : (unsigned char) bl_i;
					}


					// si llegamos a cero cambiamos de estado y reseteamos texturas
					if (blending == 0) { 	
						resetWMS = true;
						if (wms_active) estadoWMS=1; 
						else {
							estadoWMS=0;
							CancelPetitions();
						}
					}
					break;
		}
	}

	// si no hay peticiones pendientes, actualizamos el cronometro
	if (pets.empty()) {
		t0_wms = osg::Timer::instance()->tick(); 
	
	} else {

		// si hace tiempo que no llegan texturas, reseteamos la cola de peticiones
		if (estadoWMS==2 && GetWaitedTimeWMS()>1000*MAX_TIME_OUT) {
			printf ("¡¡¡¡¡¡¡¡¡¡¡¡¡TIME_OUT!!!!!!!!!!!!!!!!");
			CancelPetitions();
			t0_wms = osg::Timer::instance()->tick(); 
			resetPetPend = true;
		}
	}
}

//*****************************************************************

unsigned long int OsgDrawableCullCallback::NewPetition (cpw::Point2d<float> P0, cpw::Point2d<float> P1, 
				const unsigned int tile_size, const std::string &nombre, int nivelLOD)
{
	// actualizamos los parámetros
	//PET_PEND = true;
	//t0_wms = osg::Timer::instance()->tick(); 
	//NFDP = 0;
	//ultLODped = nivelLOD;
	
	// pedimos la petición
	//id_wms = petman->NewPetition (P0, P1, tile_size, nombre);
	
	unsigned long int id_wms = petman->NewPetition (P0, P1, tile_size, nombre);
	pets.push_back (WMSPetition (NULL, id_wms));

	// retornamos el identificador para que lo guarde el drawable
	return id_wms;
}

//*****************************************************************

osg::Vec2s OsgDrawableCullCallback::UTMToPixel (float _x, float _y, float _z)
{
	// pasamos a coordenadas de escena
	cpw::Point3d<double> out = ((OsgScene *)cpw::ApplicationScene::GetInstance()->GetScene())->UTMToSceneCoords(
			cpw::Point3d<double>(_x - traslation.x(), 
								_y - traslation.y(), 
								_z - traslation.z()));
	double x=out.x, y=out.y, z=out.z;

	/*// pasamos a coordenadas de escena
	double x = _x - traslation.x();
	double y = _y - traslation.y() - 3e6;
	double z = _z - traslation.z();
	*/

	// aplicamos la matriz de proyección
	double den = x*wx + y*wy + z*wz - eye.x()*wx - eye.y()*wy - eye.z()*wz;
	double numx = x*ux + y*uy + z*uz - eye.x()*ux - eye.y()*uy - eye.z()*uz;
	double numy = x*vx + y*vy + z*vz - eye.x()*vx - eye.y()*vy - eye.z()*vz;
	float px = D * numx / den;
	float py = D * numy / den;

	// devolvemos el punto
	osg::Vec2s P ((int)(px+0.5), (int)(py+0.5));
	return P;
}

//*****************************************************************

void OsgDrawableCullCallback::ComputeNextPetition()
{
	float mindist2 = 1e20;
	int ibest = -1;
	float dist2;
	OsgDrawableData *data;
	OsgDrawableData *best_data_to_ask=NULL;
	osg::Vec3d centro;
	float ancho, alto;
	float diag2;
	int minx, miny, maxx, maxy;
	int text_size_to_ask;
	std::vector <OsgDrawableData *> datas_to_ask;

	switch (estadoWMS) {

		case 1:
		case 3: // pedimos para el nodo raiz

			// cogemos el nodo raiz del primer tipo que haya
			if (!datas.empty()) {
				data = datas[0]->GetData(0);
				if (!data->PendingPetition()) {
					best_data_to_ask = data;
					text_size_to_ask = MIN_WMS_TEXT_RESOL;
				}
			}
			break;


		case 2: // buscamos el más idóneo de la lista
			for (unsigned int i=0; i<datas.size(); i++) {
				int text_size = MIN_WMS_TEXT_RESOL;
				data = datas[i];

				// calculamos las variables UTM
				data->GetUTMData (centro, ancho, alto);

				// si es nodo hoja con textura ya puesta, miramos si sigue siendo candidato
				if (data->GetTipoNodo()==2 && data->GetTexWMS()==2) {

					// calculamos la resolución del drawable
					int text_res = data->GetTextureResol();

					// si ya tiene la máxima permitida, fuera
					if (text_res == MAX_WMS_TEXT_RESOL) continue;

					// si está prácticamente fuera de la imagen, fuera
					if (!GetBoundingPixels (centro, ancho, alto, minx, miny, maxx, maxy, diag2)) continue;

					// si su diagonal no es suficiente grande, fuera
					if (diag2 < 400*2*text_res) continue;

					// este tipo es candidato con la nueva resolución
					text_size = text_res * 2;
					if (text_size==0) 
						printf ("OJO: TEXT_RES 0 EN %s", data->GetName().c_str());
				}

				// calculamos la distancia de la cámara al centro del drawable
				dist2 = (punto_mira.x() - centro.x()) * (punto_mira.x() - centro.x()) +  
										(punto_mira.y() - centro.y()) * (punto_mira.y() - centro.y()); 

				// si la distancia es la mínima hasta el momento, nos quedamos con ese
				if (dist2 < mindist2) {
					best_data_to_ask = data;
					mindist2 = dist2;
					text_size_to_ask = text_size;
				}
			}
			break;
	}

	// hacemos la petición
	if (best_data_to_ask) {
		best_data_to_ask->MakePetition (text_size_to_ask);			
		printf ("Nueva pet (%d pend.): '%s'. Text_size=%d", 
			pets.size(), best_data_to_ask->GetName().c_str(), text_size_to_ask); 
	}

	// reseteamos la lista
	datas.clear();
}

//*****************************************************************

bool OsgDrawableCullCallback::GetBoundingPixels (osg::Vec3d &centro, float ancho, float alto,
												 int &minx, int &miny, int &maxx, int &maxy, float &diag)
/* calcula el bounding box de la proyección del drawable en pantalla, y cuanto vale 
   su diagonal mayor. El valor de retorno indica true si el drawable es visible en al menos un 10% */
{
	// proyectamos las cuatro esquinas del drawable
	osg::Vec2s P[4];
	P[0] = UTMToPixel (centro.x()+ancho/2, centro.y()+alto/2, centro.z());
	P[1] = UTMToPixel (centro.x()-ancho/2, centro.y()+alto/2, centro.z());
	P[2] = UTMToPixel (centro.x()-ancho/2, centro.y()-alto/2, centro.z());
	P[3] = UTMToPixel (centro.x()+ancho/2, centro.y()-alto/2, centro.z());

	// calculamos los extremos
	minx = maxx = P[0].x();
	miny = maxy = P[0].y();
	for (int i=1; i<4; i++) {
		if (P[i].x() < minx) minx = P[i].x();
		if (P[i].x() > maxx) maxx = P[i].x();
		if (P[i].y() < miny) miny = P[i].y();
		if (P[i].y() > maxy) maxy = P[i].y();
	}

	// calculamos las diagonales
	float diag1 = (P[0].x()-P[2].x())*(P[0].x()-P[2].x()) + (P[0].y()-P[2].y())*(P[0].y()-P[2].y());
	float diag2 = (P[1].x()-P[3].x())*(P[1].x()-P[3].x()) + (P[1].y()-P[3].y())*(P[1].y()-P[3].y());
	diag = (diag1 > diag2)? diag1 : diag2;

	// devolvemos si aparecemos en pantalla (al menos un porcentaje suficiente)
	if (minx > width2*1.0) return false;
	if (maxx < -width2*1.0) return false;
	if (miny > height2*1.0) return false;
	if (maxy < -height2) return false;
	return true;
}

