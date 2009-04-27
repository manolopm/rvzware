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
#ifndef _FIREANIMATION_
#define _FIREANIMATION_

#include <cpw/common/types.h>
#include <cpw/graphic/IOverlayImage.h>

#include <cpwutils/Exports.h>
#include <cpwutils/Fire.h>





namespace cpw 
{ 
	namespace utils
	{
		struct TimeSlice;
		class Fire;


		/** 
			\brief Por comentar...
			\ingroup utils 
		*/
		class CPWUTILS_EXPORT FireAnimation
		{
			public:
			
				FireAnimation(void);
				~FireAnimation(void);

				FireAnimation(const FireAnimation &_fa);

				inline int & GetCurrentInterval() { return current_interval; }
				inline void SetCurrentInterval(const unsigned int &_current_interval) { current_interval = _current_interval; }

				void UpdateAnimationTargets(TimeSlice *begin_ts, TimeSlice *end_ts, const int &_begin_index, const int &_end_index);
				//void UpdateAnimationTargets(const int &_begin, const int &_end);

				void StepAnimation(const cpwTime &time);

				std::vector<Point3d<float> > & GetCurrentVertices() { return current_vertices; }
				
				float & GetCurrentSmokeParticlesLife() { return current_smoke_pl; }
				float & GetCurrentFireParticlesLife() { return current_fire_pl; }

				void ClearAnimation();

				Range<float> & GetCurrentFireParticlesSize() { return current_fire_size; }
				Range<float> & GetCurrentSmokeParticlesSize() { return current_smoke_size; }

				Range<int> & GetCurrentFireGenerationRate() { return current_fire_generation_rate; }
				Range<int> & GetCurrentSmokeGenerationRate() { return current_smoke_generation_rate; }

				void SetLastInterval(const bool &_last_interval) { last_interval = _last_interval; }
				bool & GetLastInterval() { return last_interval; }

				bool PreComputeAnimationData(Fire *fire);

				void Clear();

				void SetPositionBetweenIntervals(const float &value) { position_between_intervals = value; }

				void SetTextureVisible(const bool &value);

			private:

				/*void MatchVertexCount(std::vector< Point3d<float> > &begin_vec,
									  std::vector< Point3d<float> > &end_vec);*/

				

				void ComputeGenerationRateAccordingToDistance();

				Point3d<float> ComputeCenter( const std::vector<Point3d<float> > &vec);

				void StepAnimationBetweenIntervals(const cpwTime &time);

				void CorrectVertexOrder();
				bool PreComputeAnimatedVertexDataBegin(Fire *fire);
				bool PreComputeAnimatedVertexDataEnd(Fire *fire);
				bool MatchVertexCount(std::vector< Point3d<float> > &begin_vec, std::vector< Point3d<float> > &end_vec);
				bool PreComputeAnimationVertexRelation(Fire *fire);
				bool PreComputeTriangleData(Fire *fire);
				int FindFirstEndVertex();
				void ComputeTextureResolution();
				void PreComputeColors();
				void PreComputeColors(const int &pc, std::vector< cpw::Color > &colors);
				
				bool ComputeCurrentTriangles(const std::vector<Point3d<float> > &old_array, 
											 const std::vector<Point3d<float> > &new_array);

				bool GetFireBoundingBox(float &x_min, float &x_max, float &y_min, float &y_max);

				bool MakeTriangles( std::vector< Point3d<float> > v0, 
									std::vector< Point3d<float> > v1,
									std::vector< int > v_match,
									std::vector< Triangle > &t_vec,
									const bool &close_interval);

				bool DrawTriangles(const int &current_animation_slice);

				void ComputeCurrentColors(const int &current_animation_slice, std::vector< Color > &current_colors);

				


				struct TimeSliceAnimationData
				{
					std::vector< Point3d<float> > animation_vertex_data_begin;
					std::vector< Point3d<float> > animation_vertex_data_end;
					std::vector< int > animation_vertex_match;
					std::vector< Triangle > precomputed_triangles;
					bool close_interval;
					Color color;
				};

				std::vector<TimeSliceAnimationData> time_slice_animation;

				

				int current_interval, begin_index, end_index;
				std::vector< Point3d<float> > current_vertices;//, begin_vertices, end_vertices;
				std::vector< Point3d<float> > current_vertices_old, coords_inc;
				std::vector< Triangle > current_triangles;
				Range<float> current_fire_size, current_smoke_size;
				Range<int> current_fire_generation_rate, current_smoke_generation_rate;
				Point3d<float> begin_center, end_center;
				float current_fire_pl, current_smoke_pl;
				TimeSlice *begin, *end;
				bool last_interval; //to check if we´re in the last interval, no interpolation needed in that case
				float x_max, y_max, x_min, y_min; //the fire bounding box
				unsigned int img_resx, img_resy;
				double animation_total_time, animation_total_time_inv;
				cpwTime last_tick;
				float position_between_intervals;

				//to draw the perimeter
				IOverlayImage *image;

				//default colors
				std::vector< Color > default_colors;

				

				unsigned int vertex_height_index;

		};
	}

}

#endif
