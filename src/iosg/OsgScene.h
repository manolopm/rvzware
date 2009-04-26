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

#ifndef _OSGSCENE_
#define _OSGSCENE_

#include <osg/Node>
#include <osg/ref_ptr>
#include <osg/MatrixTransform>
#include <osg/Timer>

#include <osgParticle/ModularEmitter>

#include <OpenThreads/Mutex>

#include <osg/Light>
#include <osg/LightSource>
#include <osg/Switch>

#include <string>
#include <map>

#include <cpw/graphic/IScene.h>
#include <cpw/graphic/IHeightCalculator.h>

#include <iosg/OsgSenoidalInterpolator.h>
#include <iosg/OsgPetitionManager.h>
#include <iosg/OsgImgMixer.h>

#include <iosg/OsgVisitor.h>
#include <iosg/OsgSceneBuilder.h>
//#include <iosg/OsgHeightCalculator.h>
//#include <iosg/OsgCurveAdapter.h>
#include <iosg/OsgSun.h>

#include <osgParticle/FireEffect>

#include <cpw/debugger.h>
#include <cpw/graphic/CurveAdapter.h>

#include <iosg/Export.h>


#define PARTICLERADIO  5
#define PARTICLEWEIGHT  50

#define OSG_SCENE_TERRAIN_NODE_MASK 10
#define OSG_SCENE_ENTITIES_NODE_MASK 1
#define OSG_SCENE_GIZMOS_NODE_MASK 2
#define OSG_SCENE_LABEL_NODE_MASK 3
#define PERFORMANCE_CHECK_INTERVAL 1.0f


namespace cpw 
{ 
	namespace iosg 
	{		
		/** 
			\brief Por comentar ....
			\ingroup iosg
		*/
		class IOSGEXPORT OsgScene : public cpw::IScene
		{
			public:
				OsgScene(const std::string &url);
				virtual ~OsgScene();
				virtual void InitScene();
				virtual bool SetSceneData(void *scene_data);

				void TestPetitions()
				{
					cpw::Point2d<float> x0_y0, x1_y1;
					osg_petition_manager.NewPetition(x0_y0, x1_y1);
				}

				virtual void *GetSceneData() { return (void *) root_node.get();  }

				virtual void GetDefaultCameraCoords(float &pos_x, float &pos_y, float &pos_z,
													float &view_x, float &view_y, float &view_z,
													float &up_x, float &up_y, float &up_z);

				//Create a group which will contain objects from one layer
				//virtual bool CreateGroup(const std::string &group_name);

				//create a sub-group of a group
				virtual bool CreateGroup(const std::string &group_name, const std::string &parent_group_name);

				//Set an object visible or not visible
				virtual bool SetVisible(const std::string &parent_id, const std::string &object_id, const bool &visible);


				virtual bool AddObjectToGroup(const std::string &object_id, const std::string &group_name, const std::string &new_object,
												cpw::Point3d<float> &pos,
												cpw::Point3d<float> &rotation,
												cpw::Point3d<float> &scale,
												bool visible = true, 
												const std::string &name = "");

				virtual bool AddPointSprite(const std::string &url_model, const cpw::Point3d<float> &pos, float width, float height, const std::string &texture_filename, const float &lod_change = 0.0f, const float &lod_factor = 0.0f);

				virtual void UpdateIconsSize(float max_size, float min_size, float max_distance, float min_distance, const cpw::Point3d<float> &camera_pos, float max_height, float min_height);

				
				

				virtual void AddLineParticleSystem(const cpw::Point3d<float> &v1, const cpw::Point3d<float> &v2,											   
												   const std::string &img_file, const std::string &ps_id,
												   const std::string &group_name=std::string(""),
												   const cpw::Range<int> &new_particles_sec= cpw::Range<int>(10,20),
												   const float particles_life=10, 
												   const cpw::Range<float> &alpha = cpw::Range<float>(0.0f,1.0f),
												   const bool senoidal_alpha_interpolation=false, 
												   const bool additive_blend=true,
												   const cpw::Range<float> &speed = cpw::Range<float>(5.0f,10.0f),
												   const cpw::Range<float> &phi = cpw::Range<float>(0.0f,360.0f),
												   const cpw::Range<float> &theta = cpw::Range<float>(0.0f,12.0f),		   
												   const cpw::Range<float> &size = cpw::Range<float>(1.0f,2.0f));
				
				virtual void AddMultiLineParticleSystem(const cpw::Point3d<float> &v,											   
												   const std::string &img_file, const std::string &ps_id,
												   const std::string &group_name=std::string(""),
												   const cpw::Range<int> &new_particles_sec= cpw::Range<int>(10,20),
												   const float particles_life=10, 
												   const cpw::Range<float> &alpha = cpw::Range<float>(0.0f,1.0f),
												   const bool senoidal_alpha_interpolation=false, 
												   const bool additive_blend=true,
												   const cpw::Range<float> &speed = cpw::Range<float>(5.0f,10.0f),
												   const cpw::Range<float> &phi = cpw::Range<float>(0.0f,360.0f),
												   const cpw::Range<float> &theta = cpw::Range<float>(0.0f,12.0f),		   
												   const cpw::Range<float> &size = cpw::Range<float>(1.0f,2.0f));

				virtual void AddMultiLineParticleSystem(const std::vector< cpw::Point3d<float> > &v,											   
												   const std::string &img_file, const std::string &ps_id,
												   const std::string &group_name=std::string(""),
												   const cpw::Range<int> &new_particles_sec= cpw::Range<int>(10,20),
												   const float particles_life=10, 
												   const cpw::Range<float> &alpha = cpw::Range<float>(0.0f,1.0f),
												   const bool senoidal_alpha_interpolation=false, 
												   const bool additive_blend=true,
												   const cpw::Range<float> &speed = cpw::Range<float>(5.0f,10.0f),
												   const cpw::Range<float> &phi = cpw::Range<float>(0.0f,360.0f),
												   const cpw::Range<float> &theta = cpw::Range<float>(0.0f,12.0f),		   
												   const cpw::Range<float> &size = cpw::Range<float>(1.0f,2.0f),
												   const float &max_radius = 0.0f);

				virtual bool AddVertexToMultiLineParticleSystem(const cpw::Point3d<float> &v, const std::string &ps_id);

				virtual bool UpdateMultiLineParticleSystemVertex(const int vertex_index, const cpw::Point3d<float> &v, const std::string &ps_id);
				virtual bool UpdateMultiLineParticleSystemVertices(const std::vector< cpw::Point3d<float> > &new_vertices, const std::string &ps_id, const bool &use_last_computed_vertices=false);
				virtual bool UpdateParticleSystemProperties();

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
													  const cpw::Range<float> &size);

				virtual void DeleteParticleSystemFromScene(const std::string &ps_id, const std::string &parent_id);

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
															const cpw::Range<float> &size);

				virtual bool UpdateParticleSystemParticleGeneratorRate(const std::string &ps_id, const cpw::Range<int> &new_rate);

				virtual bool UpdateParticleSystemParticleLife(const std::string &ps_id, float new_life);
				virtual bool UpdateParticleSystemParticleSize(const std::string &ps_id, const cpw::Range<float> &new_size);

				virtual bool Update(const std::string &object_id, cpw::Point3d<float> position, cpw::Point3d<float> orientation, cpw::Point3d<float> scale);
				virtual bool Remove(const std::string &object_id);
				virtual bool ObjectExistsInScene(const std::string &object_id);
				virtual bool InsertAsReferencedObject(cpw::IGraphic *object);
				virtual bool TraslateObjectXAxis(const std::string &object_id, float displacement);

				virtual bool IntersectRayWithScene(const cpw::Point3d<float> &r1, const cpw::Point3d<float> &r2, cpw::Point3d<float> &intersection_point, const bool &utm_ray = false);
				virtual bool IntersectRayWithTerrain(const cpw::Point3d<float> &r1, const cpw::Point3d<float> &r2, cpw::Point3d<float> &intersection_point, const bool &utm_ray = false);
				virtual bool IntersectRayWithTerrain(const cpw::Point3d<double> &r1, const cpw::Point3d<double> &r2, cpw::Point3d<double> &intersection_point, const bool &utm_ray = false);

				virtual bool IntersectRayWithScene(const cpw::Point3d<double> &r1, const cpw::Point3d<double> &r2, cpw::Point3d<double> &intersection_point, const bool &utm_ray = false);
				virtual bool IntersectRayWithScene(const cpw::Point3d<float> &r1, const cpw::Point3d<float> &r2, cpw::Point3d<float> &intersection_point, cpw::Point3d<float> &intersection_normal, const bool &utm_ray=false);
				virtual bool IntersectRayWithScene(const cpw::Point3d<double> &r1, const cpw::Point3d<double> &r2, cpw::Point3d<double> &intersection_point, cpw::Point3d<double> &intersection_normal, const bool &utm_ray=false);

				virtual bool AddText(const std::string &font, const std::string &text, const float &size, const cpw::Point3d<float> &position);

				

				virtual void PetitionReady(const std::map<int, std::string> &img_filenames, const unsigned long int &petition_id);

				virtual void SetPetitionImagesFolder(const std::string &img_folder){ osg_image_mixer->SetImgFolder(img_folder); }

				virtual int  ReturnRequest(cpw::Request &request);

				virtual void UpdateTextures(int cx, int cy, const cpw::Point3d<float> &eye, const cpw::Point3d<float> &center,
											const cpw::Point3d<float> &up, const cpw::Point3d<float> &punto_mira, const unsigned char &texture_factor=255,const unsigned char &fire_mode=0);
				virtual void ShowWmsLayers(const bool &active) { show_wms_layers = active; }
				
				virtual void SetRequestManager(cpw::IRequestReceiver *rm);
				virtual void SetWCSRequestManager(cpw::IRequestReceiver *rm);
				virtual void SetSunPosition(const float &pos) {if (scene_sun !=NULL) scene_sun->SetRelativeRotation(cpw::Point3d<float>(0,pos,0));}

				void ClearInvoker() {
					cpw::Point3d<float> eye(2,1,0); 
					visitor_text.ResetTextures();
				}

				void ChangeVisitorFlag();

				void AddIconToParticleSystem(const std::string &file_name, const cpw::Point3d<float> &pos);

				virtual void AdaptLineToTerrain(const cpw::Point3d<float> &v0, const cpw::Point3d<float> &v1, std::vector< cpw::Point3d<float> > &new_vertices, const float &distance_between_vertices, const float &height_tolerance=20.0f);

				static cpw::Point3d<float> UTMToSceneCoords(const cpw::Point3d<float> &coords);
				static cpw::Point3d<float> SceneCoordsToUTM(const cpw::Point3d<float> &coords);
				static cpw::Point3d<double> UTMToSceneCoords(const cpw::Point3d<double> &coords);
				static cpw::Point3d<double> SceneCoordsToUTM(const cpw::Point3d<double> &coords);
				static cpw::Point3d<double> GetAdaptOffset();

				osg::Node * GetRootNode() { return root_node.get(); }

				void AddLine(const std::string &object_id, const std::vector<cpw::Point3d<float> > &draw_array, const std::string &color, const int &r, const int &g, const int &b, const int &width, const bool &visible);
				void UpdateLine(const std::string &object_id, const std::vector<cpw::Point3d<float> > &draw_array, const std::string &color, const int &r, const int &g, const int &b, const int &width, const bool &visible, const int &update_point);
				void AddLine2(const std::string &object_id, const std::vector<cpw::Point3d<float> > &draw_array, const std::string &color, const int &r, const int &g, const int &b, const int &width, const bool &visible);
				void AddLine3(const std::string &object_id, const std::vector<cpw::Point3d<float> > &draw_array, const std::string &color, const int &r, const int &g, const int &b, const int &width, const bool &visible);
				void AddObject(const std::string &name, const std::string &model, const cpw::Point3d<float> &pos);


				//this is the nodepaths which correspond to each collidable object
				std::vector< osg::NodePath > & GetCollisionNodePaths() { return collision_nodepaths; }


				static bool IntersectRayWithNode(const float &_x, const float &_y, osg::Node *node_to_intersect, cpw::Point3d<float> &intersection_point, const bool &utm=false);

				void AddTextureLayer(osg::Texture2D *_texture, const cpw::Point2d<float> &utm0, const cpw::Point2d<float> &utm1); 
				void RemoveTextureLayer(osg::Texture2D *_texture);

				virtual bool AddGizmo(const std::string &handler_id, const std::vector<std::string> &sub_ids, const std::vector<std::string> &filenames);
				virtual bool AddHandler(const std::string &handler_id, const std::string &file_name);
				virtual bool SetHandlerVisible(const std::string &handler_id, const bool &visible);
				virtual void SetAllHandlersInvisible() { if (handlers_node != NULL) handlers_node->setAllChildrenOff(); }
				virtual bool IsHandlerVisible(const std::string &handler_id);
				virtual bool AdaptHandlerToModel(const std::string &handler_id, const std::string &model_id,  const cpw::Point3d<float> &orientation, const bool &adapt_orientation=false);
				virtual bool AdaptHandlerToModelFixedScreenSize(const std::string &handler_id, const std::string &model_id,  const cpw::Point3d<float> &orientation, const bool &adapt_orientation, const int &pixel_size, const cpw::Point3d<double> &camera_position, const float &znear=0.0f);

				virtual void SetIconVisibility(const std::string &object_id, const bool &vis);

				virtual void AcumulativeRotation(const int &axis, const float &angle, const std::string &object_id, cpw::Point3d<float> &new_orientation);

				void RepeatOceanUV(osg::Node *node);

				virtual void Clear();

				virtual void InitializeTextures(){first_time = true;}

				virtual void SetLightning(const bool &_lightning);

				osg::Group * GetRenderToTextureCamerasGroup();
				osg::Group * GetCameraGizmosGroup();

				virtual cpw::IScene::SCENE_PERFORMANCE GetPerformance();
				virtual void UpdatePerformance();
				bool loadShaderSource(osg::Shader* obj, const std::string& fileName );

				cpw::IHeightCalculator * GetHeightCalculator() { return height_calculator; }

				void ShowBuildScene();

				virtual void SetDefaultIcon(const std::string &_default_icon) { default_icon = _default_icon; }

				virtual unsigned long int BuildScene(const double &up_left_corner_x, const double &up_left_corner_y, const double &down_right_corner_x, const double &down_right_corner_y, const int &width=1024, const int &height=1024, const bool force=false, const bool _preview=true);
				virtual unsigned long int GetWCS(const double &x_min, const double &y_min, const double &x_max, const double &y_max, const unsigned int &resx=1, const unsigned int &resy=1);
				virtual void PreBuild();
				virtual void SetCoordsMinMax(double _xmin, double _ymin, double _xmax, double _ymax) {scene_builder.SetCoordsMinMax(_xmin,_ymin,_xmax,_ymax);/*xmin = _xmin; ymin = _ymin; xmax = _xmax; ymax = _ymax;*/}
				virtual cpw::Point2d<double> GetCoordsMin() {return scene_builder.GetCoordsMin();/*return cpw::Point2d<double>(xmin, ymin);*/}
				virtual bool Generate() {return scene_builder.Generate();}
				
				virtual std::string GetRequestURL() {return request_url;}
				virtual std::vector<std::string> GetOsgdemParam() {return scene_builder.GetOsgdemParam();}
				virtual void SetRequestMap(std::map<int, std::vector<unsigned long int> > _rm);
				virtual void SetRequestDEMMap(std::map<int, std::vector<unsigned long int> > _rdm);



			private:

				float SuggestParticleSystemStepDistance();
				void CutDownVerticesIfNeccesary( std::vector< cpw::Point3d<float> > &scene_coords_vec );

				osgParticle::ModularEmitter* AddParticleSystem(const std::string &img_file, 
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
															const cpw::Range<float> &size,
															const cpw::Point3d<float> &center,
															const float &lod_frontier=0.0f);
															//,
															//const int tiles = 1);

				osg::MatrixTransform * CreateTransformationMatrix(cpw::Point3d<float> pos, 
											  cpw::Point3d<float> orientation,
											  cpw::Point3d<float> scale);

				void CreateTransformationMatrix(cpw::Point3d<float> pos, 
											  cpw::Point3d<float> orientation,
											  cpw::Point3d<float> scale, osg::Matrix &matrix);


				float ComputeLODDistance(const osg::BoundingSphere &bsphere, const osg::Matrix &matrix);

				void ComputeUtmToSceneFactor();

				//osg::Geode * CreatePointSprite();

				OsgSenoidalInterpolator sin_interpolator;
				osg::ref_ptr<osg::Group> root_node;
				osg::ref_ptr<osg::Group> layers_node;
				osg::ref_ptr<osg::Switch> handlers_node;
				osg::ref_ptr<osg::Switch> referenced_objects;
				osg::ref_ptr<osg::Switch> entities;
				osg::ref_ptr<osg::Group> particle_systems_node;
				osg::ref_ptr<osg::Group> light_group;
				osg::ref_ptr<osg::Group> terrain_group;
				osg::ref_ptr<osg::Group> render_to_texture_cameras;
				osg::ref_ptr<osg::Group> camera_gizmos;
				osg::ref_ptr<osg::Node> ocean;
				osg::ref_ptr<osg::Node> sky;
				iosg::OsgSun *scene_sun;
				std::vector<osg::Node *> islands;
				OpenThreads::Mutex mutex;
				osg::ref_ptr<osg::MatrixTransform> temp_node; //this node will contain the object that the user is currently positioning into the scene
				std::map<std::string, osg::Node *> url_node_relation; //reference to every node
				OsgVisitor visitor_text;
				
				bool show_wms_layers;
				cpw::IHeightCalculator *height_calculator;
				cpw::CurveAdapter curve_adapter;
				std::vector< cpw::Point3d<float> > last_computed_vertices_lying_on_terrain;

				//for texture mixing
				OsgPetitionManager osg_petition_manager;
				OsgImgMixer *osg_image_mixer;

	//			bool AddObjectToGroup(const std::string &url, const std::string &group_name, void *new_object, osg::Matrix &transform_matrix);


				std::string NormalizeString(const std::string &str);
				

				osg::Node *GetNode(const std::string &url);

				bool visitor_flag;

				std::vector< osg::NodePath > collision_nodepaths;

				std::string default_directory, default_icon;

				bool first_time;

				osg::Timer_t last_t, actual_t;

				SCENE_PERFORMANCE scene_performance;
				float acumulated_time;
				int frames_count;

				//Shaders
				//----------
				osg::Uniform *alpha;
				osg::Uniform *diffuseTex;
				osg::Image *exampleImage;
				osg::Texture2D *exampleTexture;


				//scene builder
				osg::ref_ptr<osg::Geode> build_scene_geode;
				osg::ref_ptr<osg::Geometry> build_scene_geom;
				osg::ref_ptr<osg::Vec3Array> build_scene_vertices;

				//scene builder
				OsgSceneBuilder scene_builder;
				bool redirect_wms_requests_to_scene_builder;
				std::string request_url;
				bool preview;

				double xmin, ymin;
				double xmax, ymax;
				double res_x, res_y;


				bool generate;
				std::vector<std::string> osgdem_param;
				//adapt factor

		};
	}
}

#endif
