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
#include <osg/PagedLOD>
#include <osg/MatrixTransform>
#include <osg/Geode>
#include <osg/Texture2D>
#include <osg/Geometry>
#include <osg/TexMat>
#include <osg/TexEnvCombine>
#include <osg/TexEnv>
#include <osgDB/ReadFile>


#include "stdafx.h"


#include <cpw/graphic/ApplicationScene.h>
#include <iosg/OsgScene.h>
#include <iosg/OsgDrawableData.h>


// para poder hacer el printf
#include <cpw/common/ApplicationLog.h>
#define printf cpw::ApplicationLog::GetInstance()->GetLogger()->printf

using namespace cpw::iosg;

#define MAX_NFSV_ANTES_PETICION		90

//*****************************************************************************

void OsgDrawableData::SetTextureRect (std::vector <TextureRect> Q)
{
	// si no hay quemadas reseteamos el flag del data y del dcc
	if (Q.empty()) {
		ResetTexRect();
		OsgDrawableCullCallback *dcc = (OsgDrawableCullCallback *) drawable->getCullCallback();
		dcc->SetTexRect (false);
		return;
	}

	int best_n = -1;
	float best_area = -1;
	
	// leemos los límites del drawable
	float minx = centro_UTM.x() - ancho_UTM/2;
	float maxx = centro_UTM.x() + ancho_UTM/2;
	float miny = centro_UTM.y() - alto_UTM/2;
	float maxy = centro_UTM.y() + alto_UTM/2;

	// barremos la lista de texturas quemadas, para ver si caen dentro del drawable
	for (int n=0; n<Q.size(); n++) {
		TextureRect *q = &Q[n];

		// hay que mirar si los puntos caen dentro de la isla
		if (minx > q->texP1.x) continue;
		if (maxx < q->texP0.x) continue;
		if (miny > q->texP1.y) continue;
		if (maxy < q->texP0.y) continue;

		/*
		// calculamos el área dentro del drawable
		float x0 = (minx > q->texP0.x)? minx : q->texP0.x;
		float x1 = (maxx < q->texP1.x)? maxx : q->texP1.x;
		float y0 = (miny > q->texP0.y)? miny : q->texP0.y;
		float y1 = (maxy < q->texP1.y)? maxy : q->texP1.y;
		float area = (x1-x0) * (y1-y0);
		*/

		// calculamos el área de todo el incendio 
		// (es mejor que el área dentro del drawable, para que no de saltos al hacer zoom)
		float area = (q->texP1.x-q->texP0.x) * (q->texP1.y-q->texP0.y);

		// si el área es mayor que la actual, nos quedamos con la nueva
		if (area > best_area) {
			best_area = area;
			best_n = n;
		}
	}

	// si ninguna cayó dentro, retornamos
	if (best_n < 0) return;

	// cargamos la textura 
	TextureRect *q = &Q[best_n]; 
	ss->setTextureAttributeAndModes (2, q->texRect, osg::StateAttribute::ON);

	/*
	// partimos de las mismas coordenadas de textura de la ortofoto
	osg::Geometry *geom = (osg::Geometry *) drawable;
	geom->setTexCoordArray (1, geom->getTexCoordArray(0));
	*/

	// transformamos las coordenadas de textura para que macheen el cuadrante UMT
	float sx = ancho_UTM / (q->texP1.x-q->texP0.x);
	float sy = alto_UTM / (q->texP1.y-q->texP0.y);
	float tx = (q->texP0.x - minx) / (q->texP1.x-q->texP0.x);
	float ty = (q->texP0.y - miny) / (q->texP1.y-q->texP0.y);
	osg::TexMat* texmat = new osg::TexMat;
	texmat->setMatrix (osg::Matrix::scale(sx,sy,1)*osg::Matrix::translate(-tx,-ty,0));
	ss->setTextureAttributeAndModes (2, texmat, osg::StateAttribute::ON);	

	// ponemos el flag del data y el del cullcallback a true
	OsgDrawableCullCallback *dcc = (OsgDrawableCullCallback *) drawable->getCullCallback();
	dcc->SetTexRect (true);
	texRect = 2;
}

//*****************************************************************************

OsgDrawableData *OsgDrawableData::GetData (int lod)
{
	// buscamos el ancestro del data con el lod indicado
	OsgDrawableData *data2 = this;
	while (data2->nivelLOD>lod) {
		osg::Drawable *dr = data2->padre;
		data2 = (OsgDrawableData *) dr->getUserData();
	}
	
	// devolvemos el data
	return (data2);
}

//*****************************************************************************

void OsgDrawableData::TextureReceived (osg::Texture2D *texture)
{
	pet_pend = false;
	texWMS = 2;
	drWMS = drawable;
	//WMS_LOD = nivelLOD;

	// resolvemos el problema de las lineas negras entre tiles
	//texture->setWrap(osg::Texture::WrapParameter::WRAP_S, osg::Texture::WrapMode::CLAMP_TO_EDGE);
	//texture->setWrap(osg::Texture::WrapParameter::WRAP_T, osg::Texture::WrapMode::CLAMP_TO_EDGE);
	texture->setWrap(osg::Texture::WRAP_S, osg::Texture::CLAMP_TO_EDGE);
	texture->setWrap(osg::Texture::WRAP_T, osg::Texture::CLAMP_TO_EDGE);

	// cargamos la textura WMS y sus coordenadas
	osg::Geometry *geom = (osg::Geometry *) drawable;
	ss->setTextureAttributeAndModes (1, texture, osg::StateAttribute::ON);
	geom->setTexCoordArray (1, geom->getTexCoordArray(0));

	// quitamos la matriz de texturas
	ss->setTextureAttributeAndModes (1, new osg::TexMat, osg::StateAttribute::OFF);
}

//*****************************************************************************

void OsgDrawableData::Process (osg::NodeVisitor* nv)
{
	// si no hay stateset es que es un drawable sin textura (como el mar), así que retornamos
	if (!ss) return;

	// obtenemos el objeto CullCallback donde están las coordenadas del ojo
	OsgDrawableCullCallback *dcc = (OsgDrawableCullCallback *) drawable->getCullCallback();

	// obtenemos el userdata del padre
	OsgDrawableData *data2 = (OsgDrawableData *) padre->getUserData();

	// procesando el blending WMS
	if (dcc->estadoWMS>0) {	// el WMS está activado

		// miramos si añadir el drawable a la lista
		dcc->InsertaDrawable (this);

		/*
		// si no hay peticiones pendientes, lo encolamos en la lista
		if (!dcc->PendingPetitions()) dcc->TestPuntoMira (this);
		*/

		// 
		// si estando el WMS full active, y aún no tengo textura fija, hay que hacer par de cosas
		if (dcc->estadoWMS>0 && texWMS<2 && dcc->data_root->texWMS>0) {
			bool pet = pet_pend;

			// buscamos en la jerarquia el primer ancestro con textura propia) texWMS=2
			while (data2->texWMS<2) {
				if (!pet && data2->pet_pend) pet = true;
				osg::Drawable *dr = data2->padre;
				data2 = (OsgDrawableData *) dr->getUserData();
			}

			// si el drawable con texWMS=2 es distinto al que estamos usando, 
			// o bien pertenece a un tree layer anterior (texWMS==0), heredamos la textura
			if (drWMS!=data2->drawable || texWMS==0) {

				// indicamos que estamos heredando la textura del ancestro con texWMS=2
				texWMS = 1;
				drWMS = data2->drawable;
				//WMS_LOD = data2->nivelLOD;
							
				// cargamos la textura WMS y sus coordenadas
				osg::StateSet *ssp = drWMS->getStateSet();
				osg::Texture2D* texture = dynamic_cast<osg::Texture2D*> (ssp->getTextureAttribute (1, osg::StateAttribute::TEXTURE));
				ss->setTextureAttributeAndModes (1, texture, osg::StateAttribute::ON);

				// transformamos las coordenadas de textura para que matcheen el cuadrante correcto
				TransformTexCoords (drWMS, 1);
			}
		}
	} 

	// mirando si hay que poner texRect
	if (dcc->GetTexRect() && !texRect) {
		
		// ponemos el flag siempre a true aunque no caiga dentro la textura, 
		// para no tener que comprobar siempre
		texRect = 1;

		// le jincamos la textura
		/*
		osg::Texture2D *textura;
		cpw::Point2d<float> P0, P1;
		dcc->GetVisitor()->GetTextureRect (&textura, P0, P1);
		*/
		SetTextureRect (dcc->GetVisitor()->quemadas);
	}
	
	// AHORA VIENE EL TEMA DE LOS TEXENVCOMBINE

	// los TexEnvCombine de las unit 0 y 1 son distintos segun el caso (ver WMS.doc)
	if (dcc->estadoWMS < 1) {	

		// no hay textura WMS que mostrar
		// salida 0: tomamos la textura original (unit 0)
		{
			osg::TexEnvCombine *texenv = new osg::TexEnvCombine;
			texenv->setCombine_RGB(osg::TexEnvCombine::REPLACE);
			texenv->setSource0_RGB(osg::TexEnvCombine::TEXTURE0);
			texenv->setOperand0_RGB(osg::TexEnvCombine::SRC_COLOR);
			ss->setTextureAttribute (0, texenv);
		}
		
		// salida 1: seguimos con la misma textura
		{
			osg::TexEnvCombine *texenv = new osg::TexEnvCombine;
			texenv->setCombine_RGB(osg::TexEnvCombine::REPLACE);
			texenv->setSource0_RGB(osg::TexEnvCombine::PREVIOUS);
			texenv->setOperand0_RGB(osg::TexEnvCombine::SRC_COLOR);
			ss->setTextureAttribute (1, texenv);
		}	

	} else {

		// hay textura WMS que mostrar
		// salida 0: multiplicamos el alpha de la unit1 (WMS) por el factor de blending
		{
			osg::TexEnvCombine *texenv = new osg::TexEnvCombine;
			texenv->setCombine_Alpha (osg::TexEnvCombine::MODULATE);
			texenv->setSource0_Alpha (osg::TexEnvCombine::TEXTURE1);
			texenv->setOperand0_Alpha (osg::TexEnvCombine::SRC_ALPHA);
			texenv->setSource1_Alpha (osg::TexEnvCombine::CONSTANT);
			texenv->setOperand1_Alpha (osg::TexEnvCombine::SRC_ALPHA);
			texenv->setConstantColor (osg::Vec4 (1, 1, 1, (float) dcc->GetBlending() / 255));
			ss->setTextureAttribute (0, texenv);
		}	
	
		// salida 1: combinamos unit0 (ortofoto) y unit1 (WMS) según el alpha de la salida anterior 
		{
			osg::TexEnvCombine *texenv = new osg::TexEnvCombine;
			texenv->setCombine_RGB (osg::TexEnvCombine::INTERPOLATE);
			texenv->setSource0_RGB (osg::TexEnvCombine::TEXTURE0);
			texenv->setOperand0_RGB (osg::TexEnvCombine::SRC_COLOR);
			texenv->setSource1_RGB (osg::TexEnvCombine::TEXTURE1);
			texenv->setOperand1_RGB (osg::TexEnvCombine::SRC_COLOR);
			texenv->setSource2_RGB (osg::TexEnvCombine::PREVIOUS);
			texenv->setOperand2_RGB (osg::TexEnvCombine::ONE_MINUS_SRC_ALPHA);
			ss->setTextureAttribute (1, texenv);
		}	
	}

	// el texEnvCombine del 2 depende de si hay textura quemada o no
	if (texRect == 2) {

		// según el modo de mapeo ponemos un envcombine distinto
		if (dcc->GetVisitor()->GetModo()==1) {

			// textura RGBA
			// salida 2: interpolamos la previa con la tex2 (quemada) usando alpha2
			{ 
				osg::TexEnvCombine *texenv = new osg::TexEnvCombine;
				texenv->setCombine_RGB (osg::TexEnvCombine::INTERPOLATE);
				texenv->setSource0_RGB (osg::TexEnvCombine::PREVIOUS);
				texenv->setOperand0_RGB (osg::TexEnvCombine::SRC_COLOR);
				texenv->setSource1_RGB (osg::TexEnvCombine::TEXTURE2);
				texenv->setOperand1_RGB (osg::TexEnvCombine::SRC_COLOR);
				texenv->setSource2_RGB (osg::TexEnvCombine::TEXTURE2);
				texenv->setOperand2_RGB (osg::TexEnvCombine::ONE_MINUS_SRC_ALPHA);
				ss->setTextureAttribute (2, texenv);
			}

		} else {

			// textura solo alpha para quemar la foto
			// salida 2: combinamos la previa con el alpha 2 (quemada)
			{ 
				osg::TexEnvCombine *texenv = new osg::TexEnvCombine;
				texenv->setCombine_RGB (osg::TexEnvCombine::MODULATE);
				texenv->setSource0_RGB (osg::TexEnvCombine::PREVIOUS);
				texenv->setOperand0_RGB (osg::TexEnvCombine::SRC_COLOR);
				texenv->setSource1_RGB (osg::TexEnvCombine::TEXTURE2);
				texenv->setOperand1_RGB (osg::TexEnvCombine::ONE_MINUS_SRC_ALPHA);
				ss->setTextureAttribute (2, texenv);
			}
		}

	} else {

		// salida 2: seguimos con la misma textura
		{
			osg::TexEnvCombine *texenv = new osg::TexEnvCombine;
			texenv->setCombine_RGB(osg::TexEnvCombine::REPLACE);
			texenv->setSource0_RGB(osg::TexEnvCombine::PREVIOUS);
			texenv->setOperand0_RGB(osg::TexEnvCombine::SRC_COLOR);
			ss->setTextureAttribute (2, texenv);
		}	
	}
}

//*****************************************************************************

void OsgDrawableData::TransformTexCoords (osg::Drawable *dr2, int unit)
{		
	// calculamos los centros de amobs drawables
	osg::MatrixTransform *m1 = (osg::MatrixTransform *) drawable->getParent(0)->getParent(0);
	osg::Vec3d cen1 = m1->getMatrix().getTrans();
	osg::MatrixTransform *m2 = (osg::MatrixTransform *) dr2->getParent(0)->getParent(0);
	osg::Vec3d cen2 = m2->getMatrix().getTrans();
	
	// calculamos la relación de escala y posición entre el drawable y su padre
	const osg::BoundingBox bb = drawable->getBound();
	const osg::BoundingBox bb2 = dr2->getBound();
	float dim2x = bb2.xMax() - bb2.xMin();
	float dim2y = bb2.yMax() - bb2.yMin();
	float tx = ((bb.xMin()+cen1.x()) - (bb2.xMin()+cen2.x())) / dim2x;
	float ty = ((bb.yMin()+cen1.y()) - (bb2.yMin()+cen2.y())) / dim2y;
	float sx = (bb.xMax() - bb.xMin()) / dim2x;
	float sy = (bb.yMax() - bb.yMin()) / dim2y;

	// transformamos las coordenadas de textura para que macheen el cuadrante correcto del padre
	osg::TexMat* texmat = new osg::TexMat;
	texmat->setMatrix (osg::Matrix::scale(sx,sy,1)*osg::Matrix::translate(tx,ty,0));
	ss->setTextureAttributeAndModes (unit, texmat, osg::StateAttribute::ON);	
}



//*****************************************************************

void OsgDrawableData::MakePetition (int text_size)
{
	// actualizamos los parámetros
	pet_pend = true;

	// calculamos las esquinas de la peticion
	float dx = ancho_UTM / 2;
	float dy = alto_UTM / 2;
	cpw::Point2d<float> P0 (centro_UTM.y()-dy, centro_UTM.x()-dx); 
	cpw::Point2d<float> P1 (centro_UTM.y()+dy, centro_UTM.x()+dx);

	// lanzamos la petición
	OsgDrawableCullCallback *dcc = (OsgDrawableCullCallback *) drawable->getCullCallback();
	id_wms = dcc->NewPetition (P0, P1, text_size, drawable->getName(), nivelLOD);

	/*
	printf ("pidiendo textura para el dr. '%s': (%.0f, %.0f) -- (%.0f, %.0f)", 
			drawable->getName().c_str(), P0.x, P0.y, P1.x, P1.y);
	*/
	
}

//*****************************************************************

int OsgDrawableData::GetTextureResol()
{
	// devolvemos la resolución de la textura WMS
	osg::Texture2D* texture = dynamic_cast<osg::Texture2D*> (ss->getTextureAttribute (1, osg::StateAttribute::TEXTURE));
	return (texture->getImage()->s());
}

//*****************************************************************

void OsgDrawableData::Init()
{
	// si no hay stateset es que es un drawable sin textura (como el mar), así que retornamos
	if (!ss) return;

	// obtenemos el objeto CullCallback donde están las coordenadas del ojo
	OsgDrawableCullCallback *dcc = (OsgDrawableCullCallback *) drawable->getCullCallback();

	// leemos el plod padre para leer el rango de distancias
	osg::MatrixTransform *mt = (osg::MatrixTransform *) drawable->getParent(0)->getParent(0);
	osg::Group *group = (osg::Group *) mt->getParent(0);
	osg::PagedLOD *plod1 = dynamic_cast<osg::PagedLOD *> (group);

	// ojo: los drawables del último nivel de LOD tienen una jerarquía familiar distinta
	if (plod1) {
		// no estamos en un drawable del último nivel
		group = (osg::Group *) plod1->getParent(0);
		mind = plod1->getMinRange(0);
		tipoNodo = 1;
	} else {
		mind = 0;
		tipoNodo = 2;
	}

	// miramos si es el nodo raiz
	if (plod1 && strcmp (plod1->getName().c_str(), "RAIZ BLENDING")==0) { 
		tipoNodo = 0; 
		maxd = 2*mind;					// ponemos una maxd cualquiera > mind
		cenlod2 = plod1->getCenter();	// el centro del padre es el mismo que él
		padre = drawable;				// hacemos que padre apunte a él mismo
		nivelLOD = 0;					// estamos en el LOD 0
		dcc->data_root = this;			// lo guardamos en el dcc

	} else {

		// leemos el plod abuelo para leer el rango de distancias
		osg::PagedLOD *plod2 = dynamic_cast<osg::PagedLOD *> (group);
		if (!plod2) plod2 = (osg::PagedLOD *) group->getParent(0);	
		maxd = plod2->getMinRange(0);
		cenlod2 = plod2->getCenter();

		// accedemos al drawable padre, y guardamos un puntero a su userdata
		osg::MatrixTransform *mt = (osg::MatrixTransform *) plod2->getChild(0);
		osg::Geode *geode = (osg::Geode *) mt->getChild(0);
		padre = (osg::Drawable *) geode->getDrawable(0);

		// calculamos su nivel de LOD, a partir del de su padre
		OsgDrawableData *datap = (OsgDrawableData *) padre->getUserData();
		nivelLOD = datap->nivelLOD + 1;
	}

	/*
	// leemos la textura del padre (excepto para el drawable raíz)
	osg::StateSet *ssp = padre->getStateSet();
	osg::Texture2D* texture = dynamic_cast<osg::Texture2D*> (ssp->getTextureAttribute (0, osg::StateAttribute::TEXTURE));
	ss->setTextureAttributeAndModes (1, texture, osg::StateAttribute::ON);

	// copiamos las coordenadas de textura del unit texture original
	osg::Geometry *geom = (osg::Geometry *) drawable;
	geom->setTexCoordArray (1, geom->getTexCoordArray(0));

	// transformamos las coordenadas de textura para que matcheen el cuadrante correcto
	TransformTexCoords (padre, 1);
	*/

	// calculamos el centro y las dimensiones UTM
	const osg::BoundingBox bb = drawable->getBound();
	osg::Vec3d centro = mt->getMatrix().getTrans();
	cpw::Point3d<double> out = ((OsgScene *)cpw::ApplicationScene::GetInstance()->GetScene())->SceneCoordsToUTM(
			cpw::Point3d<double>(dcc->traslation.x()+centro.x()+bb.center().x(), 
								dcc->traslation.y()+centro.y()+bb.center().y(), 
								dcc->traslation.z()+centro.z()+bb.center().z()));
	centro_UTM.set (out.x, out.y, out.z);
	ancho_UTM = bb.xMax() - bb.xMin();
	alto_UTM = bb.yMax() - bb.yMin();

	// le ponemos un nombre
	char nombre[100];
	sprintf (nombre, "%s_L%d_X%d_Y%d", dcc->getName().c_str(), nivelLOD, 
				(int) centro_UTM.x(), (int) centro_UTM.y());
	drawable->setName (nombre);

	// le copiamos la textura original en las unit 1, 2 y 3
	osg::StateSet *ssp = drawable->getStateSet();
	osg::ref_ptr <osg::Texture2D> texture = dynamic_cast<osg::Texture2D*> (ssp->getTextureAttribute (0, osg::StateAttribute::TEXTURE));
	ss->setTextureAttributeAndModes (1, texture.get(), osg::StateAttribute::ON);
	ss->setTextureAttributeAndModes (2, texture.get(), osg::StateAttribute::ON);
	ss->setTextureAttributeAndModes (3, texture.get(), osg::StateAttribute::ON);

	// le copiamos también las coordenadas de textura del unit texture original
	osg::Geometry *geom = (osg::Geometry *) drawable;
	geom->setTexCoordArray (1, geom->getTexCoordArray(0));
	geom->setTexCoordArray (2, geom->getTexCoordArray(0));
	geom->setTexCoordArray (3, geom->getTexCoordArray(0));

	
	// salida 0: tomamos la textura original (unit 0)
	{
		osg::TexEnvCombine *texenv = new osg::TexEnvCombine;
		texenv->setCombine_RGB(osg::TexEnvCombine::REPLACE);
		texenv->setSource0_RGB(osg::TexEnvCombine::TEXTURE0);
		texenv->setOperand0_RGB(osg::TexEnvCombine::SRC_COLOR);
		ss->setTextureAttribute (0, texenv);
	}
	
	// salida 1: seguimos con la misma textura
	{
		osg::TexEnvCombine *texenv = new osg::TexEnvCombine;
		texenv->setCombine_RGB(osg::TexEnvCombine::REPLACE);
		texenv->setSource0_RGB(osg::TexEnvCombine::PREVIOUS);
		texenv->setOperand0_RGB(osg::TexEnvCombine::SRC_COLOR);
		ss->setTextureAttribute (1, texenv);
	}	
	
	// salida 2: seguimos con la misma textura
	{
		osg::TexEnvCombine *texenv = new osg::TexEnvCombine;
		texenv->setCombine_RGB(osg::TexEnvCombine::REPLACE);
		texenv->setSource0_RGB(osg::TexEnvCombine::PREVIOUS);
		texenv->setOperand0_RGB(osg::TexEnvCombine::SRC_COLOR);
		ss->setTextureAttribute (2, texenv);
	}	

	// salida 3: le añadimos iluminación
	{
		osg::TexEnvCombine *texenv = new osg::TexEnvCombine;
		texenv->setCombine_RGB (osg::TexEnvCombine::MODULATE);
		texenv->setSource0_RGB (osg::TexEnvCombine::PREVIOUS);
		texenv->setOperand0_RGB (osg::TexEnvCombine::SRC_COLOR);
		texenv->setSource1_RGB (osg::TexEnvCombine::PRIMARY_COLOR);
		texenv->setOperand1_RGB (osg::TexEnvCombine::SRC_COLOR);
		ss->setTextureAttribute (3, texenv);
	}	



}

