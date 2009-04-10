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
#ifndef _ISCENE_
#define _ISCENE_

#include <string>
#include <vector>
#include <map>
#include <cpw/common/types.h>
#include <cpw/thread/IRequestReceiver.h>
#include <cpw/graphic/IHeightCalculator.h>

#define X_AXIS 0
#define Y_AXIS 1
#define Z_AXIS 2

namespace cpw
{

	/** 
		\brief Comentario breve de la clase ....
		\ingroup cpw
	*/
	class IScene: public IRequestReceiver
	{
		public:

			enum SCENE_PERFORMANCE { VERY_BAD, BAD, MEDIUM, GOOD, EXCELLENT };

			virtual	~IScene(){};

			virtual void InitScene() = 0;

			virtual bool SetSceneData(void *scene_data) = 0;

			virtual void *GetSceneData() = 0;
			
			virtual void GetDefaultCameraCoords(float &pos_x, float &pos_y, float &pos_z,
												float &view_x, float &view_y, float &view_z,
												float &up_x, float &up_y, float &up_z) = 0;

			virtual bool IntersectRayWithScene(const cpw::Point3d<float> &r1, const cpw::Point3d<float> &r2, cpw::Point3d<float> &intersection_point, const bool &utm_ray = false)   = 0;
			virtual bool IntersectRayWithTerrain(const cpw::Point3d<float> &r1, const cpw::Point3d<float> &r2, cpw::Point3d<float> &intersection_point, const bool &utm_ray = false) = 0;
			virtual bool IntersectRayWithTerrain(const cpw::Point3d<double> &r1, const cpw::Point3d<double> &r2, cpw::Point3d<double> &intersection_point, const bool &utm_ray = false) = 0;
			virtual bool IntersectRayWithScene(const cpw::Point3d<double> &r1, const cpw::Point3d<double> &r2, cpw::Point3d<double> &intersection_point, const bool &utm_ray = false) = 0;
			virtual bool IntersectRayWithScene(const cpw::Point3d<float> &r1, const cpw::Point3d<float> &r2, cpw::Point3d<float> &intersection_point, cpw::Point3d<float> &intersection_normal, const bool &utm_ray=false) = 0;
			virtual bool IntersectRayWithScene(const cpw::Point3d<double> &r1, const cpw::Point3d<double> &r2, cpw::Point3d<double> &intersection_point, cpw::Point3d<double> &intersection_normal, const bool &utm_ray=false) = 0;
			//virtual bool IntersectRayWithEntities(const cpw::Point3d<float> &r1, const cpw::Point3d<float> &r2, std::string &entity_id, const bool &utm_ray = false)   = 0;
			virtual void AdaptLineToTerrain(const cpw::Point3d<float> &v0, const cpw::Point3d<float> &v1, std::vector< cpw::Point3d<float> > &new_vertices, const float &distance_between_vertices, const float &height_tolerance=20.0f)=0;

			
			virtual bool Update(const std::string &object_id, cpw::Point3d<float> position, cpw::Point3d<float> orientation, cpw::Point3d<float> scale)=0;
			virtual bool Remove(const std::string &object_id)=0;
			virtual bool ObjectExistsInScene(const std::string &object_id)=0;

			virtual bool TraslateObjectXAxis(const std::string &object_id, float displacement) {return false;}

			virtual bool InsertAsReferencedObject(cpw::IGraphic *object)=0;

			virtual bool CreateGroup(const std::string &group_name, const std::string &parent_group_name)=0;
			virtual bool SetVisible(const std::string &parent_id, const std::string &object_id, const bool &visible)=0;

			virtual bool AddObjectToGroup(const std::string &object_id, const std::string &group_name, const std::string &new_object,
											cpw::Point3d<float> &pos,
											cpw::Point3d<float> &rotation,
											cpw::Point3d<float> &scale,
											bool visible = true, 
											const std::string &name = "")=0;

			virtual bool AddPointSprite(const std::string &url_model, 
										const cpw::Point3d<float> &pos, 
										float width, float height, 
										const std::string &texture_filename,
										const float &lod_change = 0.0f, 
										const float &lod_factor = 0.0f 
										) = 0;

			virtual void UpdateIconsSize(float max_size, float min_size, float max_distance, float min_distance, const cpw::Point3d<float> &camera_pos, float max_height, float min_height) = 0;
			virtual void UpdateTextures(int cx, int cy, const cpw::Point3d<float> &eye, const cpw::Point3d<float> &center,
										const cpw::Point3d<float> &up, const cpw::Point3d<float> &punto_mira, const unsigned char &texture_factor=255, const unsigned char &fire_mode=0) = 0;

			virtual void AddLineParticleSystem(const cpw::Point3d<float> &v1, const cpw::Point3d<float> &v2,											   
											   const std::string &img_file, 
											   const std::string &ps_id,
											   const std::string &group_name,
											   const cpw::Range<int> &new_particles_sec,
											   const float particles_life, 
											   const cpw::Range<float> &alpha ,
											   const bool senoidal_alpha_interpolation, 
											   const bool additive_blend,
											   const cpw::Range<float> &speed ,
											   const cpw::Range<float> &phi ,
											   const cpw::Range<float> &theta ,		   
											   const cpw::Range<float> &size ) = 0;

			virtual void AddMultiLineParticleSystem(const cpw::Point3d<float> &v,											   
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
											   const cpw::Range<float> &size) = 0;

			virtual void AddMultiLineParticleSystem(const std::vector< cpw::Point3d<float> > &v,											   
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
											   const float &max_radius = 0.0f  ) = 0;

			virtual bool AddVertexToMultiLineParticleSystem(const cpw::Point3d<float> &v, const std::string &ps_id) = 0;

			

			virtual void AddPuntualParticleSystem(const cpw::Point3d<float> &v,											   
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
												  const cpw::Range<float> &size) = 0;

			virtual void UpdatePuntualParticleSystem(const cpw::Point3d<float> position,
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
														const cpw::Range<float> &size) = 0;

			virtual bool UpdateMultiLineParticleSystemVertex(const int vertex_index, const cpw::Point3d<float> &v, const std::string &ps_id)=0;
			virtual bool UpdateMultiLineParticleSystemVertices(const std::vector< cpw::Point3d<float> > &new_vertices, const std::string &ps_id, const bool &use_last_computed_vertices=false)=0;
			virtual void DeleteParticleSystemFromScene(const std::string &ps_id, const std::string &parent_id)=0;
			virtual bool UpdateParticleSystemParticleGeneratorRate(const std::string &ps_id, const cpw::Range<int> &new_rate) = 0;
			virtual bool UpdateParticleSystemParticleLife(const std::string &ps_id, float new_life)=0;
			virtual bool UpdateParticleSystemParticleSize(const std::string &ps_id, const cpw::Range<float> &new_size)=0;
			virtual bool AddText(const std::string &font, const std::string &text, const float &size, const cpw::Point3d<float> &position)=0;

			virtual void ShowWmsLayers(const bool &active)=0;
			virtual void PetitionReady(const std::map<int, std::string> &img_filenames, const unsigned long int &petition_id)=0;
			virtual void SetPetitionImagesFolder(const std::string &img_folder) = 0;
			virtual void TestPetitions() = 0;
			
			virtual void SetRequestManager(cpw::IRequestReceiver *rm) = 0;
			virtual void SetWCSRequestManager(cpw::IRequestReceiver *rm) = 0;
			virtual void AddLine(const std::string &object_id, const std::vector<cpw::Point3d<float>> &draw_array, const std::string &color, const int &r, const int &g, const int &b, const int &width, const bool &visible) = 0;
			virtual void AddLine2(const std::string &object_id, const std::vector<cpw::Point3d<float>> &draw_array, const std::string &color, const int &r, const int &g, const int &b, const int &width, const bool &visible) = 0;
			virtual void AddLine3(const std::string &object_id, const std::vector<cpw::Point3d<float>> &draw_array, const std::string &color, const int &r, const int &g, const int &b, const int &width, const bool &visible) = 0;
			virtual void UpdateLine(const std::string &object_id, const std::vector<cpw::Point3d<float>> &draw_array, const std::string &color, const int &r, const int &g, const int &b, const int &width, const bool &visible, const int &update_point) = 0;
			virtual void AddObject(const std::string &name, const std::string &model, const cpw::Point3d<float> &pos)=0;

			virtual void SetSunPosition(const float &pos) = 0;

			virtual void AcumulativeRotation(const int &axis, const float &angle, const std::string &object_id, cpw::Point3d<float> &new_orientation){}
			virtual bool AddGizmo(const std::string &gizmo_id, const std::vector<std::string> &sub_ids, const std::vector<std::string> &filenames) { return false; }
			virtual bool AddHandler(const std::string &handler_id, const std::string &file_name)=0;
			virtual bool SetHandlerVisible(const std::string &handler_id, const bool &visible) { return false; }
			virtual void SetAllHandlersInvisible(){};
			virtual bool IsHandlerVisible(const std::string &gizmo_id) { return false; }
			virtual bool AdaptHandlerToModel(const std::string &gizmo_id, const std::string &model_id, const cpw::Point3d<float> &orientation, const bool &adapt_orientation) { return false; }
			virtual bool AdaptHandlerToModelFixedScreenSize(const std::string &handler_id, const std::string &model_id,  const cpw::Point3d<float> &orientation, const bool &adapt_orientation, const int &pixel_size, const cpw::Point3d<double> &camera_position, const float &znear) { return false; }

			virtual void SetIconVisibility(const std::string &object_id, const bool &vis)=0;

			virtual void Clear()=0;

			virtual void InitializeTextures()=0;

			virtual void SetLightning(const bool &_lightning)=0;

			virtual SCENE_PERFORMANCE GetPerformance() = 0;
			virtual void UpdatePerformance() = 0;

			virtual IHeightCalculator * GetHeightCalculator() = 0;

			virtual void SetDefaultIcon(const std::string &_default_icon) = 0;

			virtual void PreBuild()=0;
			virtual unsigned long int BuildScene(const double &up_left_corner_x, const double &up_left_corner_y, const double &down_right_corner_x, const double &down_right_corner_y, const int &width=1024, const int &height=1024, const bool force=false, const bool _preview=true)=0;
			virtual unsigned long int GetWCS(const double &x_min, const double &y_min, const double &x_max, const double &y_max, const unsigned int &resx=1, const unsigned int &resy=1)=0;
			virtual std::string GetRequestURL() = 0;
			virtual void SetCoordsMinMax(double _xmin, double _ymin, double _xmax, double _ymax)=0;
			virtual cpw::Point2d<double> GetCoordsMin()=0;
			virtual bool Generate() = 0;
			//virtual bool GenerateImage(cpw::Request &request) = 0;
			virtual std::vector<std::string> GetOsgdemParam()=0;
			virtual void SetRequestMap(std::map<int, std::vector<unsigned long int>> _rm) = 0;
			virtual void SetRequestDEMMap(std::map<int, std::vector<unsigned long int>> _rdm) = 0;
			
			

	};
}
#endif 
