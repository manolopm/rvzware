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

#ifndef _GRAPHICOBJECTCONTROLLER_
#define _GRAPHICOBJECTCONTROLLER_

#include <string>

#include <cpw/common/types.h>

#include <cpw/graphic/IGraphicObjectLoader.h>
#include <cpw/graphic/INavigatorManager.h>
#include <cpw/entity/LayerTree.h>
#include <cpw/entity/Entity.h>



namespace cpw
{

	namespace controllers 
	{  

		/** 
			\brief This controller inserts different graphical objects in the scene
			\ingroup cpw
		*/
		class GraphicObjectController
		{

			public:
				GraphicObjectController(){}
				GraphicObjectController(cpw::IGraphicObjectLoader *gol, cpw::INavigatorManager *nm);
				virtual ~GraphicObjectController();

				cpw::IGraphic *LoadObject(const std::string &filename); //esto se convertirá en load primitive cuando esté hecho la otra parte
				
				bool InsertTempObject(cpw::IGraphic *new_obj);
				bool IntersectCenterCoords(cpw::Point3d<float> &intersection_point);
				bool IntersectMouseWithEntities(int mouse_x, int mouse_y, std::string &entity_id);

				void SetPositionOrientationScale(float posx, float posy, float posz,
												 float orientationx, float orientationy, float orientationz,
												 float scalex, float scaley, float scalez);
				void RemoveTempObject();
				void MakeTempObjectPermanent(const std::string &group_name, const std::string &element_url, const std::string &icon_filename);
				void GetCoords(float &_x, float &_y, float &_z) {_x = x; _y = y; _z = z;}

				bool IntersectMouseWithScene(int mouse_x, int mouse_y, cpw::Point3d<float> &intersection_point);

				//test ray intersection against the scene where the mouse clicked and place
				//the new PS there
				bool InsertMultiLineParticleSystemInScene(const int mouse_x, const int mouse_y,
														  const std::string &img_file, const std::string &ps_id,
														  const std::string &group_name,
														  const cpw::Range<int> &new_particles_sec,
														  const float particles_life, 
														  const cpw::Range<float> &alpha,
														  const bool senoidal_alpha_interpolation, 
														  const bool additive_blend,
														  const cpw::Range<float> &speed,
														  const cpw::Range<float> &phi,
														  const cpw::Range<float> &theta,		   
														  const cpw::Range<float> &size,
														  cpw::Point3d<float> &intersection_point);

				//Place the new PS directly into position coords
				void InsertMultiLineParticleSystemInScene(const cpw::Point3d<float> position,
														  const std::string &img_file, const std::string &ps_id,
														  const std::string &group_name,
														  const cpw::Range<int> &new_particles_sec,
														  const float particles_life, 
														  const cpw::Range<float> &alpha,
														  const bool senoidal_alpha_interpolation, 
														  const bool additive_blend,
														  const cpw::Range<float> &speed,
														  const cpw::Range<float> &phi,
														  const cpw::Range<float> &theta,		   
														  const cpw::Range<float> &size);

				void InsertPuntualParticleSystemInScene(const cpw::Point3d<float> position,
														  const std::string &img_file, const std::string &ps_id,
														  const std::string &group_name,
														  const cpw::Range<int> &new_particles_sec,
														  const float particles_life, 
														  const cpw::Range<float> &alpha,
														  const bool senoidal_alpha_interpolation, 
														  const bool additive_blend,
														  const cpw::Range<float> &speed,
														  const cpw::Range<float> &phi,
														  const cpw::Range<float> &theta,		   
														  const cpw::Range<float> &size);

				bool InsertPuntualParticleSystemInScene(const int mouse_x, const int mouse_y,
														  const std::string &img_file, const std::string &ps_id,
														  const std::string &group_name,
														  const cpw::Range<int> &new_particles_sec,
														  const float particles_life, 
														  const cpw::Range<float> &alpha,
														  const bool senoidal_alpha_interpolation, 
														  const bool additive_blend,
														  const cpw::Range<float> &speed,
														  const cpw::Range<float> &phi,
														  const cpw::Range<float> &theta,		   
														  const cpw::Range<float> &size,
														  cpw::Point3d<float> &intersection_point);

				void UpdatePuntualParticleSystem(const cpw::Point3d<float> position,
												const std::string &img_file,
												const std::string &ps_id,														
												const cpw::Range<int> &new_particles_sec,
												const float particles_life, 
												const cpw::Range<float> &alpha,
												const bool senoidal_alpha_interpolation, 
												const bool additive_blend,
												const cpw::Range<float> &speed,
												const cpw::Range<float> &phi,
												const cpw::Range<float> &theta,		   
												const cpw::Range<float> &size);

				void AddVertexToMultiLineParticleSystem(const int mouse_x, const int mouse_y, const std::string &ps_id, cpw::Point3d<float> &intersection_point);
				bool AddVertexToMultiLineParticleSystem(const std::string &ps_id, cpw::Point3d<float> &position);
				bool UpdateMultiLineParticleSystemVertex(const int vertex_index, const cpw::Point3d<float> &vertex, const std::string &ps_id);
				void DeleteParticleSystem(const std::string &ps_id, const std::string &parent_id);
				bool UpdateParticleSystemParticleGenerationRate(const std::string &ps_id, const cpw::Range<int> &new_rate);
				bool UpdateParticleSystemParticleLife(const std::string &ps_id, float new_life);
				bool UpdateParticleSystemParticleSize(const std::string &ps_id, const cpw::Range<float> &new_size);

			private:

				cpw::IGraphicObjectLoader *graphic_object_loader;
				cpw::INavigatorManager *navigator_manager;
				float x, y, z; //coords of the temp object
				
		};
	}

}

#endif 
