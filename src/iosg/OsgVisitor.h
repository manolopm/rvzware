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

#ifndef _OSGVISITOR_
#define _OSGVISITOR_

#include <osg/NodeVisitor>
#include <osg/Geode>


#include <iosg/OsgPetitionManager.h>

#include <cpw/common/types.h>

#include <iosg/Export.h>



namespace cpw { 
	
	namespace iosg  {

		class OsgDrawableCullCallback;

		// Habrá un único visitor en la aplicación, siempre accesible


		/** 
			\brief Por comentar ....
			\ingroup iosg
		*/

		// clase para la textura quemada
		class TextureRect
		{
		public:
			TextureRect (osg::Texture2D *_texRect, cpw::Point2d<float> _texP0, cpw::Point2d<float> _texP1):
			  texRect(_texRect), texP0(_texP0), texP1(_texP1) {};
			osg::Texture2D *texRect;					// textura a insertar
			cpw::Point2d<float> texP0, texP1;			// esquinas UTM donde insertar
		};


		class IOSGEXPORT OsgVisitor : public osg::NodeVisitor
		{
		public:
			OsgVisitor(): osg::NodeVisitor(osg::NodeVisitor::TRAVERSE_ALL_CHILDREN), 
				resetWMS(false), MustSetTextureRect(false) {}
			
			~OsgVisitor();

			void Launch (osg::Group *root, const unsigned char &blending_factor, 
						bool wms_active, int cx, int cy,
						const cpw::Point3d<float> &_eye, const cpw::Point3d<float> &_center, 
						const cpw::Point3d<float> &_up, const cpw::Point3d<float> &_punto_mira,
						OsgPetitionManager *_petman, const unsigned char &fire_mode);
			void Init (osg::Group *root, const unsigned char &blending_factor, 
						bool wms_active, int cx, int cy,
						const cpw::Point3d<float> &_eye, const cpw::Point3d<float> &_center, 
						const cpw::Point3d<float> &_up, const cpw::Point3d<float> &_punto_mira,
						OsgPetitionManager *_petman, const unsigned char &fire_mode);
			void Reset();

			void AddTextureRect (osg::Texture2D *textura, cpw::Point2d<float> P0, cpw::Point2d<float> P1);
			void DelTextureRect (osg::Texture2D *textura);
			inline int GetModo () { return texModo; }
			inline void GetTextureRect (osg::Texture2D **textura, cpw::Point2d<float> &P0, cpw::Point2d<float> &P1) {
				// OJO: ESTO ES UNA PRUEBA SOLO CON LA NUMERO 0!!!!!
				*textura = quemadas[0].texRect;
				P0 = quemadas[0].texP0;
				P1 = quemadas[0].texP1;
			}
			
			
			inline void ResetTextures() { resetWMS = true; }

			std::vector<TextureRect> quemadas;


		private:
			std::vector<OsgDrawableCullCallback *> dcc;	// punteros a los 7 cull callback (uno por isla)
			OsgDrawableCullCallback  *dcc_active;		// indica el callback de la isla activa actual

			virtual void apply (osg::Geode &geode);
			bool resetWMS;


			bool MustSetTextureRect;					// indica que en el siguiente launch hay que insertar una textura
			unsigned char texModo;						// 1:textura RGBA; 2:sólo alpha para quemar la ortofoto
		};

	}
}

#endif
