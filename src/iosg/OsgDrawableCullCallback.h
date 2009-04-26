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

#ifndef _OSGDRAWABLECULLCALLBACK_
#define _OSGDRAWABLECULLCALLBACK_

#include <osg/Drawable>
#include <osg/TexEnvCombine>
#include <osg/Timer>
#include <osg/Vec3>

#include <iosg/Export.h>

#include <cpw/common/types.h>

#include <iosg/OsgVisitor.h>

/** 
	\brief Por comentar...
	\ingroup iosg
*/
class IOSGEXPORT WMSPetition {

public:
	WMSPetition (osg::ref_ptr<osg::Texture2D> _TextureWMS, unsigned long int _id_wms): 
			TextureWMS(_TextureWMS), id_wms(_id_wms) {}

	osg::ref_ptr<osg::Texture2D> TextureWMS;	// textura recién traida
	unsigned long int id_wms;					// identificador de la petición WMS
};


namespace cpw 
{ 
	namespace iosg  
	{

		class OsgDrawableData;

		/** 
			\brief Comentario breve
			\ingroup iosg
		*/
		class IOSGEXPORT OsgDrawableCullCallback : public osg::Drawable::CullCallback
		{
		public:

			OsgDrawableCullCallback (OsgPetitionManager *_petman, OsgVisitor *_visitor): 
				blending(0), last_blending(255), petman(_petman), visitor(_visitor),
				//PET_PEND(false), //TextureWMS(NULL), //NFDP(0), 
				// ult_gen(0), maxLODvis(0), ultLODped(0), 
				data_central(NULL), estadoWMS(0), texRect(false) {}

			~OsgDrawableCullCallback();


			void FinishPetition();
			void SetParams (bool wms_active, int cx, int cy, 
							osg::Vec3f &_eye, osg::Vec3f &_center, osg::Vec3f &_up, 
							osg::Vec3f &_punto_mira, bool resetWMS,
							double msec, const unsigned char &blending_factor);
			float GetDistance (osg::Vec3d punto);
			unsigned long int NewPetition (cpw::Point2d<float> P0, cpw::Point2d<float> P1, 
						const unsigned int tile_size, const std::string &nombre,
						int nivelLOD);
			// indica si el punto de mira está incluido dentro del drawable
			void InsertaDrawable (OsgDrawableData *data);
			osg::Vec2s UTMToPixel (float x, float y, float z);
			bool GetBoundingPixels (osg::Vec3d &centro, float ancho, float alto,
									int &minx, int &miny, int &maxx, int &maxy, float &diag);
			void CancelPetitions();
			//osg::ref_ptr<osg::Texture2D> GetTextureWMS (unsigned long id_wms);
			osg::ref_ptr<osg::Texture2D> GetTextureWMS (OsgDrawableData *data);


			
			inline int PendingPetitions() { return pets.size(); }
			inline OsgPetitionManager * GetPetitionManager() { return petman; }
			//inline int GetNFDP() { return NFDP; }
			inline bool MustReset() { return resetWMS; }
			inline OsgVisitor *GetVisitor() { return visitor; }
			inline void ClearResetWMS() { resetWMS = false; }
			inline double GetWaitedTimeWMS() { return osg::Timer::instance()->delta_m (t0_wms, osg::Timer::instance()->tick()); }
			inline void SetTexRect (bool value) { texRect = value; }
			inline bool GetTexRect() { return texRect; }
			inline void ClearResetPetPend() { resetPetPend = false; }
			inline bool MustResetPetPend() { return resetPetPend; }
			inline unsigned char GetBlending() { return blending; }


			osg::Vec3f traslation;			///< traslación total de la isla entera

			int estadoWMS;					// 0:apagado; 1:esperando textura raiz; 2:activo; 3:apagándose

			OsgDrawableData *data_root;		// puntero al data dle drawable raiz

		private:
			void ComputeNextPetition();		// algoritmo de decisión de qué drawable pedir a continuacion

			unsigned char blending, last_blending;	// factores de blending para el WMS

			// aquí pongo lo que antes eran variables globales
			//int WMS_BLENDING;				// contador para el blending de inicio/fin
			osg::Vec3f eye;					// posición actual del ojo
			float ux,uy,uz,vx,vy,vz,wx,wy,wz;	// ejes del sistema de referencia de la cámara
			float D;						// distancia focal
			int width2, height2;			// los límites del frame buffer son -width2<x<width2, -height2<y<height2
			osg::Vec3f punto_mira;			// punto del terreno 3D al que se está mirando
			OsgPetitionManager *petman;		// puntero al Petition Manager de David

			virtual bool cull(osg::NodeVisitor* nv, osg::Drawable* drawable, osg::State* /*state*/) const;
			inline osg::Vec3f & GetEye() { return eye; }
			
			//bool PET_PEND;					// indica si hay una petición pendiente
			//osg::Texture2D *TextureWMS;	// textura recién traida
			//unsigned long int id_wms;		// identificador de la petición WMS
			osg::Timer_t t0_wms;			// instante en el que llegó la última petición
			std::vector <WMSPetition> pets;	// peticiones realizadas pendientes

			bool resetWMS;					// indica si hay que resetear todo el scene graph
			OsgDrawableData *data_central;	// puntero al data del drawable que está en el punto de mira
			std::vector <OsgDrawableData *> datas;	// punteros a los datas de los drawables cercanos al punto de mira

			OsgVisitor *visitor;			// puntero al visitor, con los campos globales a la escena

			bool texRect;					// indica si hay que poner texturas quemadas en los drawables

			bool resetPetPend;				// si es true hay que resetear el pet_pend de cada drawable
			

		};

	}
}

#endif


