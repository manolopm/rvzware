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

#ifndef _OSGDRAWABLEDATA_
#define _OSGDRAWABLEDATA_

#include <osg/Referenced>
#include <osg/Drawable>
#include <osg/Texture2D>

#include <iosg/OsgDrawableCullCallback.h>

#include <iosg/Export.h>


namespace cpw 
{ 
	namespace iosg  
	{

	// habrá un DrawableData por cada drawable de la escena


		/** 
			\brief Por comentar...
			\ingroup iosg
		*/
		class IOSGEXPORT OsgDrawableData : public osg::Referenced
		{
		public:
			OsgDrawableData (osg::Drawable *dr): 
							drawable(dr), ss(dr->getStateSet()), 
							factor(0), texWMS(0),
							visible(false), NFSV(0),
							pet_pend(false), texRect(0) {}


			void Process (osg::NodeVisitor* nv=NULL);
			void TextureReceived (osg::Texture2D *texture);
			//void SetBlendingFactors (float f0, float f1, float f2=-100);
			//void SetBlendingFactors (float f0, float f1);
			void MakePetition (int text_size);
			OsgDrawableData *GetData (int lod);
			int GetTextureResol(); 
			void Init();

			//void SetTextureRect (osg::Texture2D *textura, cpw::Point2d<float> P0, cpw::Point2d<float> P1); 
			void SetTextureRect (std::vector <TextureRect> q); 





			inline osg::StateSet *GetStateSet() { return ss; }
			inline void TestVisibility() { if (!visible) NFSV=0; visible=false; }
			inline bool IsRoot() { return (tipoNodo==0)? true : false; }
			inline int GetTexWMS() { return texWMS; }
			inline bool PendingPetition() { return pet_pend; }
			inline void GetUTMData (osg::Vec3d &centro, float &ancho, float &alto) {
				centro = centro_UTM;
				ancho = ancho_UTM;
				alto = alto_UTM;
			}
			inline osg::Vec3d GetCentroUTM() { return centro_UTM; }
			inline const std::string &GetName() { return drawable->getName(); }
			inline int GetMainLODToAsk() { 
				int lod = (tipoNodo==2 && dist<maxd/2)? nivelLOD-1 : nivelLOD-2;
				if (lod<0) lod = 0;
				return lod;
			}
			//inline unsigned char GetWMS_LOD() { return WMS_LOD; }
			inline int GetNivelLOD() { return nivelLOD; }
			inline int GetTipoNodo() { return tipoNodo; }
			inline void ResetPetition() { pet_pend = false; }

			inline unsigned long int GetId_wms() { return id_wms; }
			
			inline void ResetWMS (int estadoWMS) { 
				texWMS = 0;
				// hay una excepción: nodo raíz con estadoWMS=1
				if (estadoWMS!=1 || nivelLOD>0) pet_pend=false; 
			}

			inline int GetTexRect() { return texRect; }
			inline void ResetTexRect() { texRect = 0; }

			inline void ResetPetPend() { pet_pend = false; }



		private:
			osg::Drawable *drawable;	// puntero al drawable
			osg::Drawable *padre;		// puntero al drawable padre
			bool ocultoPorHijo;			// indica que ahora mismo no se está viendo este drawable sino sus hijos
			osg::Vec3d cenlod2;			// coordenadas del centro del LOD padre
			float factor;				// último factor de blending que se usó
			int situacionAnt;			// -1:vengo del padre; 0:me estoy mostrando; 1:vengo del hijo
			float dist;					// distancia del ojo al centro del LOD padre (para medir el cambio de LOD) 
			float mind, maxd;			// rango de distancias para el LOD


			void TransformTexCoords (osg::Drawable *dr2, int unit);

			int tipoNodo;				// 0:raiz; 1:intermedio; 2:hoja
			osg::StateSet *ss;			// puntero al state set del drawable, donde se encuentran las texturas
			int texWMS;					// 0:aún no he cargado ninguna; 1:uso la que usa mi padre; 2:tengo la mía propia
			osg::Drawable *drWMS;		// cuando texWMS>1 apunta al drawable con la textura WMS original
			//unsigned char WMS_LOD;		// nivel de LOD del drawable cuya textura estoy heredando
			bool visible;				// indica si es visible (se activó en el evento cull del frame actual)
			int NFSV;					// número de frames siendo visible
			bool pet_pend;				// indica si tiene una petición WMS pendiente
			unsigned long int id_wms;		// identificador de la petición WMS que está esperando
			int nivelLOD;				// indica a qué nivel de LOD pertenece el drawable
			osg::Vec3d centro_UTM;		// centro del drawable en coordenadas UTM
			float ancho_UTM, alto_UTM;	// dimensiones del drawable en UTM

			int texRect;				// 0: no existe textura quemada: 1: existe pero no cae en este drawable
										// 2: tengo una textura puesta

		};


	}
}

#endif
