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

//#include <stdafx.h>

#include <math.h>
#include <algorithm>

#include <cpwutils/FireAnimation.h>
#include <cpw/entity/ElementParticleSystem.h>
#include <cpw/common/Math.h>

#include <cpw/graphic/ApplicationScene.h>
#include <cpw/graphic/IScene.h>
#include <cpw/graphic/IHeightCalculator.h>

using namespace cpw::utils;

#undef small  //!!!!!!!--> Esto lo metio Jaiver (parece que hay algún define del estilo #define small char ...)

#define PSC_MULT 0.02f
// alpha para medir el negro de la textura quemada
#define FIRE_ALPHA	0.50f	
#define VERTICES_TO_ADAPT_PER_STEP 15
#define ALTERNATIVE_COLOR_METHOD 1


FireAnimation::FireAnimation() : current_interval(-1), last_interval(false), img_resx(512), img_resy(512), image(NULL), position_between_intervals(0.0f), vertex_height_index(0)
{
  default_colors.clear();

  default_colors.push_back( cpw::Color( 1.00f, 0.00f, 0.00f, FIRE_ALPHA) );
  default_colors.push_back( cpw::Color( 0.85f, 0.10f, 0.00f, FIRE_ALPHA) );	
  default_colors.push_back( cpw::Color( 0.70f, 0.30f, 0.05f, FIRE_ALPHA) );
  default_colors.push_back( cpw::Color( 0.60f, 0.60f, 0.15f, FIRE_ALPHA) );
  default_colors.push_back( cpw::Color( 0.50f, 0.70f, 0.25f, FIRE_ALPHA) );
  default_colors.push_back( cpw::Color( 0.40f, 0.80f, 0.30f, FIRE_ALPHA) );
  default_colors.push_back( cpw::Color( 0.30f, 0.90f, 0.35f, FIRE_ALPHA) );
  default_colors.push_back( cpw::Color( 0.20f, 0.95f, 0.40f, FIRE_ALPHA) );
  default_colors.push_back( cpw::Color( 0.10f, 1.00f, 0.45f, FIRE_ALPHA) );
}

FireAnimation::FireAnimation(const FireAnimation &_fa) :  current_interval(-1), last_interval(false), img_resx(512), img_resy(512), image(NULL), position_between_intervals(0.0f), vertex_height_index(0)
{

}

FireAnimation::~FireAnimation()
{
  if (image != NULL)
    {
      delete image;
      image = NULL;
    }
		
  time_slice_animation.clear();
}

void FireAnimation::SetTextureVisible(const bool &value) 
{ 
  if (image != NULL) 
    image->Visualize(value); 
}

void FireAnimation::UpdateAnimationTargets(TimeSlice *begin_ts, TimeSlice *end_ts, const int &_begin_index, const int &_end_index)
{
  begin = begin_ts;
  end = end_ts;

  begin_index = _begin_index;
  end_index = _end_index;

  current_vertices.clear();
  current_vertices.resize( time_slice_animation[begin_index].animation_vertex_data_begin.size() + 1);	
	
  //moved this section here to avoid doing it in every animation frame
  current_vertices_old = time_slice_animation[begin_index].animation_vertex_data_begin;

  if (time_slice_animation[begin_index].close_interval)
    current_vertices_old.push_back( *current_vertices_old.begin() );
  else
    current_vertices_old.push_back( current_vertices_old[current_vertices_old.size()-2] );
	
  coords_inc.clear();

  if (!last_interval)
    {
      for ( unsigned int i=0; i < time_slice_animation[begin_index].animation_vertex_data_begin.size(); i++)
	{
	  int end_ind = time_slice_animation[begin_index].animation_vertex_match[i];
	  cpw::Point3d<float> inc;

	  inc.x = time_slice_animation[begin_index].animation_vertex_data_end[end_ind].x - time_slice_animation[begin_index].animation_vertex_data_begin[i].x;
	  inc.y = time_slice_animation[begin_index].animation_vertex_data_end[end_ind].y - time_slice_animation[begin_index].animation_vertex_data_begin[i].y;
	  inc.z = time_slice_animation[begin_index].animation_vertex_data_end[end_ind].z - time_slice_animation[begin_index].animation_vertex_data_begin[i].z;
	  coords_inc.push_back(inc);
	}

      animation_total_time = ((double)end->time_stamp.seconds) - ((double)begin->time_stamp.seconds);	
      animation_total_time += (((double)end->time_stamp.miliseconds) - ((double)begin->time_stamp.miliseconds)) * 0.001;
      animation_total_time_inv = 1.0 / animation_total_time;
    }
	
  //do not compute the current vertices, just compute them once
  if (last_interval)
    {
      current_vertices.clear();

      for ( unsigned int i=0; i < begin_ts->flames.size(); i++)
	current_vertices.push_back( begin_ts->flames[i].fire.GetCoords() );

      current_fire_size = begin_ts->flames[0].fire.GetSize();
      current_smoke_size = begin_ts->flames[0].smoke.GetSize();

      current_fire_pl = begin_ts->flames[0].fire.GetParticlesLife();
      current_smoke_pl = begin_ts->flames[0].smoke.GetParticlesLife();

      current_fire_generation_rate = begin_ts->flames[0].fire.GetNewParticlesSec();
      current_smoke_generation_rate = begin_ts->flames[0].smoke.GetNewParticlesSec();

      ComputeGenerationRateAccordingToDistance();
    }
}

void FireAnimation::StepAnimation(const cpw::cpwTime &time)
{
  if (!last_interval)
    StepAnimationBetweenIntervals(time);
}

void FireAnimation::StepAnimationBetweenIntervals(const cpw::cpwTime &time)
{
  double current_animation_time;
	
  last_tick = time;
	
  current_animation_time = time.seconds - begin->time_stamp.seconds;
  current_animation_time += (((double)time.miliseconds) - ((double)begin->time_stamp.miliseconds)) * 0.001;

  //update vertex positions
  float cat_atti_mult = current_animation_time * animation_total_time_inv;
	
  //make sure that precision problems don´t let the cat_atti_mult go higher than 1.0f.
  //In rare cases i was getting 1.0002f here which ended in small graphic artifacts
  if (cat_atti_mult > 1.0f) 
    cat_atti_mult = 1.0f;

  for ( unsigned int i=0; i < time_slice_animation[begin_index].animation_vertex_data_begin.size(); i++)
    {
      current_vertices[i].x = time_slice_animation[begin_index].animation_vertex_data_begin[i].x + (cat_atti_mult * coords_inc[i].x);
      current_vertices[i].y = time_slice_animation[begin_index].animation_vertex_data_begin[i].y + (cat_atti_mult * coords_inc[i].y);
      //current_vertices[i].z = time_slice_animation[begin_index].animation_vertex_data_begin[i].z + (cat_atti_mult * coords_inc[i].z);
      //current_vertices[i].z = 0.0f;
    }

  //close or not the shape
  if (time_slice_animation[begin_index].close_interval)
    current_vertices[current_vertices.size()-1] = *current_vertices.begin();		
  else
    current_vertices[current_vertices.size()-1] = current_vertices[current_vertices.size()-2];
		
  ComputeCurrentTriangles(current_vertices_old, current_vertices);
  DrawTriangles(begin_index);

  //update fire size
  cpw::Range<float> begin_size = begin->flames[0].fire.GetSize();
  cpw::Range<float> end_size = end->flames[0].fire.GetSize();

  float max_inc = end_size.max - begin_size.max;
  float min_inc = end_size.min - begin_size.min;

  current_fire_size.max = begin_size.max + (cat_atti_mult * max_inc);
  current_fire_size.min = begin_size.min + (cat_atti_mult * min_inc);

  //update smoke size
  begin_size = begin->flames[0].smoke.GetSize();
  end_size = end->flames[0].smoke.GetSize();

  max_inc = end_size.max - begin_size.max;
  min_inc = end_size.min - begin_size.min;

  current_smoke_size.max = begin_size.max + (cat_atti_mult * max_inc);
  current_smoke_size.min = begin_size.min + (cat_atti_mult * min_inc);

  //update fire particles life
  float begin_particles_life = begin->flames[0].fire.GetParticlesLife();
  float end_particles_life = end->flames[0].fire.GetParticlesLife();

  max_inc = end_particles_life - begin_particles_life;
  current_fire_pl = begin_particles_life + (cat_atti_mult * max_inc);

  //update smoke particles life
  begin_particles_life = begin->flames[0].smoke.GetParticlesLife();
  end_particles_life = end->flames[0].smoke.GetParticlesLife();

  max_inc = end_particles_life - begin_particles_life;
  current_smoke_pl = begin_particles_life + (cat_atti_mult * max_inc);

  //update fire generation rate
  cpw::Range<float> begin_gr(begin->flames[0].fire.GetNewParticlesSec().min, begin->flames[0].fire.GetNewParticlesSec().max);
  cpw::Range<float> end_gr(end->flames[0].fire.GetNewParticlesSec().min, end->flames[0].fire.GetNewParticlesSec().max);

  float max_inc_gr = end_gr.max - begin_gr.max;
  float min_inc_gr = end_gr.min - begin_gr.min;

  current_fire_generation_rate.max = (int) begin_gr.max + (cat_atti_mult * max_inc_gr);
  current_fire_generation_rate.min = (int) begin_gr.min + (cat_atti_mult * min_inc_gr);

  //update smoke generation rate
  begin_gr.min = begin->flames[0].smoke.GetNewParticlesSec().min;
  begin_gr.max = begin->flames[0].smoke.GetNewParticlesSec().max;
  end_gr.min = end->flames[0].smoke.GetNewParticlesSec().min;
  end_gr.max = end->flames[0].smoke.GetNewParticlesSec().max;

  max_inc_gr = end_gr.max - begin_gr.max;
  min_inc_gr = end_gr.min - begin_gr.min;

  current_smoke_generation_rate.max = (int) begin_gr.max + (cat_atti_mult * max_inc_gr);
  current_smoke_generation_rate.min = (int) begin_gr.min + (cat_atti_mult * min_inc_gr);

  ComputeGenerationRateAccordingToDistance();

  /*IScene *scene = ApplicationScene::GetInstance()->GetScene();

    if (scene != NULL)
    {
    cpw::IHeightCalculator *height_calculator = scene->GetHeightCalculator();

    if (height_calculator != NULL)
    {
    unsigned int adapted_vertices = 0;

    while ( (adapted_vertices < VERTICES_TO_ADAPT_PER_STEP) && ( adapted_vertices < current_vertices.size()) )
    {
    if (vertex_height_index >= current_vertices.size())
    vertex_height_index = 0;

    height_calculator->GetHeightUTM( current_vertices[vertex_height_index].x, current_vertices[vertex_height_index].y, current_vertices[vertex_height_index].z);

    vertex_height_index++;
    adapted_vertices++;
    }
    }
    }*/
}

bool FireAnimation::ComputeCurrentTriangles(const std::vector< cpw::Point3d<float> > &old_array, const std::vector< cpw::Point3d<float> > &new_array)
{
  cpw::Triangle t0, t1;
  float x = ((float)img_resx-2) / (x_max - x_min);
  float y = ((float)img_resy-2) / (y_max - y_min);

  current_triangles.clear(); 

  //fill the triangle vector adapting the coordinates
  //xmin will be zero and xmax will be img_resx
  if (old_array.size() == new_array.size())
    {
      for (unsigned int i=0; i < new_array.size()-1; i++)
	{
	  t0.v0 = old_array[i];
	  t0.v1 = old_array[i+1];
	  t0.v2 = new_array[i];

	  t1.v0 = new_array[i];
	  t1.v1 = new_array[i+1];
	  t1.v2 = old_array[i+1];

	  //adapt the coordinates to the image coordinates system
			
	  //the image is an ortographic projection, z is ignored
	  t0.v0.z = t0.v1.z = t0.v2.z = -100.0f;
	  t1.v0.z = t1.v1.z = t1.v2.z = -100.0f;

	  //X
	  //t0.v0.x = ((t0.v0.x - xmin) * img_resx) / (x_max-x_min) //unoptimized code
	  t0.v0.x = ((t0.v0.x - x_min) * x) + 1;
	  t0.v1.x = ((t0.v1.x - x_min) * x) + 1;
	  t0.v2.x = ((t0.v2.x - x_min) * x) + 1;

	  t1.v0.x = ((t1.v0.x - x_min) * x) + 1;
	  t1.v1.x = ((t1.v1.x - x_min) * x) + 1;
	  t1.v2.x = ((t1.v2.x - x_min) * x) + 1;

	  //Y
	  t0.v0.y = ((t0.v0.y - y_min) * y) + 1;
	  t0.v1.y = ((t0.v1.y - y_min) * y) + 1;
	  t0.v2.y = ((t0.v2.y - y_min) * y) + 1;

	  t1.v0.y = ((t1.v0.y - y_min) * y) + 1;
	  t1.v1.y = ((t1.v1.y - y_min) * y) + 1;
	  t1.v2.y = ((t1.v2.y - y_min) * y) + 1;

	  current_triangles.push_back(t0);
	  current_triangles.push_back(t1);
	}
    }
  else
    return false;

  return true;
}

bool FireAnimation::DrawTriangles(const int &current_animation_slice)
{
  if (image == NULL)
    return false;

  //update the triangle data
  std::vector< std::vector<cpw::Triangle> > &triangle_arrays = image->GetTriangleArrays();

  if (triangle_arrays.size() < ((unsigned int)current_animation_slice+1))
    triangle_arrays.resize(current_animation_slice+1);

  for (int i=0; i<current_animation_slice; i++)
    triangle_arrays[i] = time_slice_animation[i].precomputed_triangles;

  triangle_arrays[current_animation_slice] = current_triangles;

  std::vector< cpw::Color > colors;

  /*if (ALTERNATIVE_COLOR_METHOD)
    ComputeCurrentColorsAlternative(current_animation_slice, c_colors);
    else*/
  ComputeCurrentColors(current_animation_slice, colors);
	
  image->SetPerimeterColors(colors);
  image->DrawFirstNTriangleArrays(current_animation_slice+1);

  return true;
}

void FireAnimation::ComputeCurrentColors(const int &current_animation_slice, std::vector< cpw::Color > &current_colors)
{
  //set the colors
	
  std::vector< cpw::Color > new_colors, last_colors;
  std::vector< cpw::Color > new_colorsr, last_colorsr;
  std::vector< cpw::Color > final_colors;
	
  //PreComputeColors(current_animation_slice+1, target_colors);

  //get the origin and target colors
  unsigned int ci = 0;
  int cont = 0;
  while (cont <= current_animation_slice)
    {
      if (ci < default_colors.size())
	{
	  new_colors.push_back( default_colors[ci] );
	  if (ci > 0)
	    last_colors.push_back( default_colors[ci-1] );
	  else
	    last_colors.push_back( default_colors[0] );

	  ci++;
	}
      else
	{
	  new_colors.push_back( default_colors[ default_colors.size() - 1] );

	  if ((default_colors.size() - 2) > 0)
	    last_colors.push_back( default_colors[ default_colors.size() - 2] );
	  else
	    last_colors.push_back( default_colors[0] );
	}
		
      cont++;
    }

  //reverse them
  current_colors.clear();
  for (std::vector< cpw::Color >::reverse_iterator r_iter = new_colors.rbegin();
       new_colors.rend() != r_iter;
       r_iter++)
    {
      new_colorsr.push_back( *r_iter );
    }

  for (std::vector< cpw::Color >::reverse_iterator r_iter = last_colors.rbegin();
       last_colors.rend() != r_iter;
       r_iter++)
    {
      last_colorsr.push_back( *r_iter );
    }

  //now interpolate the colors
  //time_slice_animation[current_animation_slice]

  if (position_between_intervals >= 0.0f)
    {
      cpw::Color new_color;

      current_colors.clear();

      for (unsigned int i=0; i < new_colors.size(); i++)
	{
	  new_color.red = last_colorsr[i].red + ((new_colorsr[i].red - last_colorsr[i].red) * position_between_intervals);
	  new_color.green = last_colorsr[i].green + ((new_colorsr[i].green - last_colorsr[i].green) * position_between_intervals); 
	  new_color.blue = last_colorsr[i].blue +  ((new_colorsr[i].blue - last_colorsr[i].blue) * position_between_intervals);
	  new_color.alpha = last_colorsr[i].alpha + ((new_colorsr[i].alpha - last_colorsr[i].alpha) * position_between_intervals) ;
			
	  current_colors.push_back( new_color );
	}
    }
  else
    current_colors = new_colorsr;
}

void FireAnimation::ComputeGenerationRateAccordingToDistance()
{

  cpw::Point3d<float> temp;
  float total_length=0.0f;

  for (unsigned int i=0; i<(current_vertices.size()-1); i++)
    {
      temp.x = current_vertices[i+1].x - current_vertices[i].x;
      temp.y = current_vertices[i+1].y - current_vertices[i].y;
      temp.z = current_vertices[i+1].z - current_vertices[i].z;

      total_length += sqrt((temp.x * temp.x) + (temp.y * temp.y) + (temp.z * temp.z));
    }

  cpw::Range<int> new_fire_rate, new_smoke_rate;
	
  if (current_vertices.size() > 1)
    {
      current_fire_generation_rate.min = current_fire_generation_rate.min * total_length * PSC_MULT;
      current_fire_generation_rate.max = current_fire_generation_rate.max * total_length * PSC_MULT;

      current_smoke_generation_rate.min = current_smoke_generation_rate.min * total_length * PSC_MULT;
      current_smoke_generation_rate.max = current_smoke_generation_rate.max * total_length * PSC_MULT;
    }
}

cpw::Point3d<float> FireAnimation::ComputeCenter( const std::vector< cpw::Point3d<float> > &vec)
{
  cpw::Point3d<float> out(0.0f, 0.0f, 0.0f);

  std::vector< cpw::Point3d<float> >::const_iterator i = vec.begin();

  for ( ; i != vec.end(); i++)
    {
      out.x += i->x;
      out.y += i->y;
      out.z += i->z;
    }

  float vec_size_inv = 1.0f / ((float)vec.size());
  out.x = out.x * vec_size_inv;
  out.y = out.y * vec_size_inv;
  out.z = out.z * vec_size_inv;

  return out;
}


//find the correct direction
void FireAnimation::CorrectVertexOrder()
{
	
}


void FireAnimation::ClearAnimation()
{

}

bool FireAnimation::PreComputeAnimationData(Fire *fire)
{
  //clear the time slice animation vector
  std::vector<TimeSliceAnimationData>::iterator iter = time_slice_animation.begin();
	
  time_slice_animation.clear();
	
  if (!PreComputeAnimatedVertexDataBegin(fire))
    return false;

  if (!PreComputeAnimatedVertexDataEnd(fire))
    return false;

  for (unsigned int i=0; i<fire->GetFlamesProgression().size()-1; i++)
    MatchVertexCount(time_slice_animation[i].animation_vertex_data_begin, time_slice_animation[i].animation_vertex_data_end);
		
  if (!PreComputeAnimationVertexRelation(fire))
    return false;

  if (!GetFireBoundingBox(x_min, x_max, y_min, y_max))
    return false;

  //set the burnt area resolution
  ComputeTextureResolution();

  //precompute triangle data
  if (!PreComputeTriangleData(fire))
    return false;	

  if (image != NULL)
    {
      delete image;
      image = NULL;
    }

  PreComputeColors();	

  image = fire->GetGraphicFactory()->CreateOverlayImage(cpw::Point2d<float>(x_min, y_min), cpw::Point2d<float>(x_max, y_max),
							img_resx, img_resy, cpw::IOverlayImage::PF_RGBA);

  //set the colors	
  std::vector< cpw::Color > colors;
  std::vector< cpw::Color > perimeter_colors;
  //perimeter_colors.push_back( cpw::Color( 1.0f, 0.0f, 0.0f, 0.75f ) );
  //perimeter_colors.push_back( cpw::Color( 0.0f, 1.0f, 0.0f, 0.75f ) );
  //perimeter_colors.push_back( cpw::Color( 0.0f, 0.0f, 1.0f, 0.75f ) );
  perimeter_colors.push_back( cpw::Color( 1.0f, 0.0f, 0.0f, FIRE_ALPHA ) );
  perimeter_colors.push_back( cpw::Color( 0.0f, 1.0f, 0.0f, FIRE_ALPHA ) );
  perimeter_colors.push_back( cpw::Color( 0.0f, 0.0f, 1.0f, FIRE_ALPHA ) );
	
  int color_index = 0;
  for (unsigned int i=0; i<time_slice_animation.size(); i++)
    {
      if (color_index > 2)
	color_index = 0;
		
      colors.push_back( perimeter_colors[color_index] );
      color_index++;
    }

  image->SetPerimeterColors(colors);

  fire->SetAnimationPreComputed(true);

  return true;
}

void FireAnimation::ComputeTextureResolution()
{
  float x_area = (x_max - x_min);
  float y_area = (y_max - y_min);

  if (x_area < 1500.0f)
    img_resx = 256;
  else if ((x_area >= 1500.0f) && (x_area < 3000.0f))
    img_resx = 512;
  else
    img_resx = 1024;

  if (y_area < 1500.0f)
    img_resy = 256;
  else if ((y_area >= 1500.0f) && (y_area < 3000.0f))
    img_resy = 512;
  else
    img_resy = 1024;

	
	

}
bool FireAnimation::PreComputeTriangleData(Fire *fire)
{
  //pre-generate the triangles for every interval
  for (unsigned int i=0; i<time_slice_animation.size()-1; i++)
    {
      if (!MakeTriangles(time_slice_animation[i].animation_vertex_data_begin,
			 time_slice_animation[i].animation_vertex_data_end,
			 time_slice_animation[i].animation_vertex_match,
			 time_slice_animation[i].precomputed_triangles,
			 time_slice_animation[i].close_interval))
	{
	  return false;
	}
    }
	
  return true;
}

bool FireAnimation::PreComputeAnimatedVertexDataBegin(Fire *fire)
{
  std::vector<float> distances_between_vertices;
  cpw::Point3d<float> vec;
  float distance_between_vertices, d, mult;
  int aux, aux2;
  bool finish = false;

  for (unsigned int i=0; i< fire->GetFlamesProgression().size(); i++)
    {
      time_slice_animation.push_back( TimeSliceAnimationData() );
      finish = false;
      distances_between_vertices.clear();

      for (unsigned int j=0; j<fire->GetFlamesProgression()[i].flames.size()-1; j++)
	distances_between_vertices.push_back( cpw::Math::DistanceBetweenTwoPoints(fire->GetFlamesProgression()[i].flames[j].fire.GetCoords(), 
										  fire->GetFlamesProgression()[i].flames[j+1].fire.GetCoords()) );

		
      sort(distances_between_vertices.begin(), distances_between_vertices.end());

      distance_between_vertices = distances_between_vertices[ distances_between_vertices.size() / 2 ];
      //distance_between_vertices = distances_between_vertices[ distances_between_vertices.size() / 3 ];
      //distance_between_vertices = distances_between_vertices[ 0 ];

      //construct the animation vertex data
      time_slice_animation[i].animation_vertex_data_begin.clear();
      time_slice_animation[i].animation_vertex_data_begin.push_back(fire->GetFlamesProgression()[i].flames[0].fire.GetCoords() ); //first vertex

      //fire->GetFlamesProgression()[i].animation_vertex_data_begin.clear();
      //fire->GetFlamesProgression()[i].animation_vertex_data_begin.push_back( fire->GetFlamesProgression()[i].flames[0].fire.GetCoords() ); //first vertex

      aux = 0;
      aux2 = 1;

      while (!finish)
	{
	  //float temp = cpw::Math::DistanceBetweenTwoPoints( fire->GetFlamesProgression()[i].animation_vertex_data_begin[aux],	fire->GetFlamesProgression()[i].flames[aux2].fire.GetCoords() );
	  float temp = cpw::Math::DistanceBetweenTwoPoints( time_slice_animation[i].animation_vertex_data_begin[aux],	fire->GetFlamesProgression()[i].flames[aux2].fire.GetCoords() );
	  bool aux2_out_of_range = false;
	  while ( (temp < distance_between_vertices) && !aux2_out_of_range )
	    {
	      if (temp > 0.0f)
		{
		  time_slice_animation[i].animation_vertex_data_begin.push_back( fire->GetFlamesProgression()[i].flames[aux2].fire.GetCoords() );
		  aux++;
		}
				
	      aux2++;
	      if (aux2 >= (int)fire->GetFlamesProgression()[i].flames.size())
		{
		  aux2_out_of_range = true;
		  finish = true;

		  if ( cpw::Math::DistanceBetweenTwoPoints( fire->GetFlamesProgression()[i].flames.back().fire.GetCoords(), time_slice_animation[i].animation_vertex_data_begin.back() ) > 1.0f )
		    time_slice_animation[i].animation_vertex_data_begin.push_back( fire->GetFlamesProgression()[i].flames.back().fire.GetCoords() );
					
		}
	      else
		//temp = cpw::Math::DistanceBetweenTwoPoints( fire->GetFlamesProgression()[i].animation_vertex_data_begin[aux],	fire->GetFlamesProgression()[i].flames[aux2].fire.GetCoords() );
		temp = cpw::Math::DistanceBetweenTwoPoints( time_slice_animation[i].animation_vertex_data_begin[aux],	
							    fire->GetFlamesProgression()[i].flames[aux2].fire.GetCoords() );
				
	    }


	  if (!aux2_out_of_range)
	    {
	      //d = cpw::Math::DistanceBetweenTwoPoints( fire->GetFlamesProgression()[i].animation_vertex_data_begin[aux], fire->GetFlamesProgression()[i].flames[aux2].fire.GetCoords() );

	      /*vec.x = fire->GetFlamesProgression()[i].flames[aux2].fire.GetCoords().x - fire->GetFlamesProgression()[i].animation_vertex_data_begin[aux].x;
		vec.y =	fire->GetFlamesProgression()[i].flames[aux2].fire.GetCoords().y - fire->GetFlamesProgression()[i].animation_vertex_data_begin[aux].y;
		vec.z =	fire->GetFlamesProgression()[i].flames[aux2].fire.GetCoords().z - fire->GetFlamesProgression()[i].animation_vertex_data_begin[aux].z;*/

	      d = cpw::Math::DistanceBetweenTwoPoints( time_slice_animation[i].animation_vertex_data_begin[aux], fire->GetFlamesProgression()[i].flames[aux2].fire.GetCoords() );

	      vec.x = fire->GetFlamesProgression()[i].flames[aux2].fire.GetCoords().x - time_slice_animation[i].animation_vertex_data_begin[aux].x;
	      vec.y =	fire->GetFlamesProgression()[i].flames[aux2].fire.GetCoords().y - time_slice_animation[i].animation_vertex_data_begin[aux].y;
	      vec.z =	fire->GetFlamesProgression()[i].flames[aux2].fire.GetCoords().z - time_slice_animation[i].animation_vertex_data_begin[aux].z;

	      mult = distance_between_vertices / d;

	      vec.x *= mult;
	      vec.y *= mult;
	      vec.z *= mult;

	      //final point
	      //vec.x += fire->GetFlamesProgression()[i].animation_vertex_data_begin[aux].x;
	      //vec.y += fire->GetFlamesProgression()[i].animation_vertex_data_begin[aux].y;
	      //vec.z += fire->GetFlamesProgression()[i].animation_vertex_data_begin[aux].z;

	      //fire->GetFlamesProgression()[i].animation_vertex_data_begin.push_back(vec);

	      vec.x += time_slice_animation[i].animation_vertex_data_begin[aux].x;
	      vec.y += time_slice_animation[i].animation_vertex_data_begin[aux].y;
	      vec.z += time_slice_animation[i].animation_vertex_data_begin[aux].z;

	      time_slice_animation[i].animation_vertex_data_begin.push_back(vec);

	      aux++;
	      //aux2++;

	      if (aux2 >= (int)fire->GetFlamesProgression()[i].flames.size())
		{
		  finish = true;
		  //fire->GetFlamesProgression()[i].animation_vertex_data_begin.push_back(fire->GetFlamesProgression()[i].flames.back().fire.GetCoords());
		  time_slice_animation[i].animation_vertex_data_begin.push_back(fire->GetFlamesProgression()[i].flames.back().fire.GetCoords());
		}
	    }
	}

      //to check that every distances between vertices are equal
      //for (unsigned int k=0; k<flames_progression[i].animation_vertex_data.size()-1; k++)
      //{
      //	float hola = cpw::Math::DistanceBetweenTwoPoints(flames_progression[i].animation_vertex_data[k], flames_progression[i].animation_vertex_data[k+1]);
      //	int adios = 1;
      //}
    }

  return true;
}

bool FireAnimation::PreComputeAnimatedVertexDataEnd(Fire *fire)
{
  for (unsigned int i=0; i<fire->GetFlamesProgression().size(); i++)
    time_slice_animation[i].animation_vertex_data_end.clear();
  //fire->GetFlamesProgression()[i].animation_vertex_data_end.clear();

  //the end animation vertices of each time slice, will be the begin animation vertices of the next one
  for (unsigned int i=0; i<fire->GetFlamesProgression().size()-1; i++)
    //fire->GetFlamesProgression()[i].animation_vertex_data_end = fire->GetFlamesProgression()[i+1].animation_vertex_data_begin;
    time_slice_animation[i].animation_vertex_data_end = time_slice_animation[i+1].animation_vertex_data_begin;

  return true;
}


bool FireAnimation::MatchVertexCount(std::vector< cpw::Point3d<float> > &begin_vec,
				     std::vector< cpw::Point3d<float> > &end_vec)
{
  float total_length = 0.0f;
  std::vector< cpw::Point3d<float> > new_vec;
  std::vector< cpw::Point3d<float> > *big, *small;
  int vertex_count_difference, small_segments, new_vertices_per_segment;
  float new_vertices_per_segment_inv;

  if (begin_vec.size() == end_vec.size())
    return true;

  if (begin_vec.size() > end_vec.size())
    {
      big = &begin_vec;
      small = &end_vec;
    }
  else
    {
      big = &end_vec;
      small = &begin_vec;
    }

  //vertex count we have to add to the small one
  vertex_count_difference = big->size() - small->size();
	
  small_segments = small->size() - 1;

  //distribute the new vertices through the segments
  new_vertices_per_segment = ceil( ((float)vertex_count_difference) / ((float)small_segments) );
  new_vertices_per_segment_inv = 1.0f / ((float) (new_vertices_per_segment+1));

  std::vector< cpw::Point3d<float> >::iterator iter = small->begin();

  for (int i=0; i < small_segments; i++)
    {
      cpw::Point3d<float> segment_first_vertex(*iter);
      new_vec.push_back(*iter);
      iter++;

      float dist = cpw::Math::DistanceBetweenTwoPoints((*small)[i], (*small)[i+1]);
      float vertex_distance =	dist * new_vertices_per_segment_inv;

      cpw::Point3d<float> dir_vec((*iter) - new_vec.back());
      cpw::Math::NormalizeVec(dir_vec);

      int j = 1;

      while ((vertex_count_difference > 0) && (j <= new_vertices_per_segment))
	{
	  new_vec.push_back(cpw::Point3d<float>((vertex_distance * j * dir_vec.x) + segment_first_vertex.x,
						(vertex_distance * j * dir_vec.y) + segment_first_vertex.y,
						(vertex_distance * j * dir_vec.z) + segment_first_vertex.z));
	  j++;
	  vertex_count_difference--;
	}
    }

  new_vec.push_back(*iter);

  *small = new_vec;

  return true;
}


bool FireAnimation::PreComputeAnimationVertexRelation(Fire *fire)
{
  for (unsigned int i=0; i<fire->GetFlamesProgression().size()-1; i++)
    {
	
      bool min_displacement_valid = false;
      float min_displacement, aux;

      for (unsigned int j=0; j<time_slice_animation[i].animation_vertex_data_begin.size(); j++)
	{
	  std::vector<int> temp_indices;
	  temp_indices.resize( time_slice_animation[i].animation_vertex_data_begin.size() );
	  aux = 0.0f;

	  for (unsigned int k=0; k<time_slice_animation[i].animation_vertex_data_end.size(); k++)
	    {
	      int begin_index = j+k;
				
	      while (begin_index >= (int)time_slice_animation[i].animation_vertex_data_begin.size())
		begin_index -= (int)time_slice_animation[i].animation_vertex_data_begin.size();
				
	      aux += cpw::Math::DistanceBetweenTwoPoints(time_slice_animation[i].animation_vertex_data_begin[begin_index],
							 time_slice_animation[i].animation_vertex_data_end[k]);				

	      temp_indices[begin_index] = k;
	    }

	  if ((!min_displacement_valid) || ((min_displacement_valid) && (aux < min_displacement)))
	    {
	      min_displacement_valid = true;
	      min_displacement = aux;
	      time_slice_animation[i].animation_vertex_match = temp_indices;
	    }
	}

      for (unsigned int j=0; j<time_slice_animation[i].animation_vertex_data_begin.size(); j++)
	{
	  std::vector<int> temp_indices;
	  temp_indices.resize( time_slice_animation[i].animation_vertex_data_begin.size() );
	  aux = 0.0f;

	  for (unsigned int k=0; k<time_slice_animation[i].animation_vertex_data_end.size(); k++)
	    {
	      int begin_index = j-k;
				
	      if (begin_index < 0)
		begin_index += time_slice_animation[i].animation_vertex_data_end.size();
								
	      aux += cpw::Math::DistanceBetweenTwoPoints(time_slice_animation[i].animation_vertex_data_begin[begin_index],
							 time_slice_animation[i].animation_vertex_data_end[k]);				

	      temp_indices[begin_index] = k;
	    }

	  if ((!min_displacement_valid) || ((min_displacement_valid) && (aux < min_displacement)))
	    {
	      min_displacement_valid = true;
	      min_displacement = aux;
	      time_slice_animation[i].animation_vertex_match = temp_indices;
	    }
	}

      //test if we must close the shape
      if ( (*time_slice_animation[i].animation_vertex_match.begin()) == 0 ||
	   (*time_slice_animation[i].animation_vertex_match.begin()) == (time_slice_animation[i].animation_vertex_match.size()-1))			 
	time_slice_animation[i].close_interval = false;
      else
	time_slice_animation[i].close_interval = true;

    }

  return true;
}

bool FireAnimation::GetFireBoundingBox(float &_x_min, float &_x_max, float &_y_min, float &_y_max)
{
  bool initialized = false;

  for (unsigned int i=0; i<time_slice_animation.size(); i++)
    {
      for (std::vector< cpw::Point3d<float> >::iterator iter = time_slice_animation[i].animation_vertex_data_begin.begin();
	   iter != time_slice_animation[i].animation_vertex_data_begin.end();
	   iter++)
	{
	  if (initialized)
	    {
	      if (iter->x > _x_max)
		_x_max = iter->x;

	      if (iter->x < _x_min)
		_x_min = iter->x;

	      if (iter->y > _y_max)
		_y_max = iter->y;

	      if (iter->y < _y_min)
		_y_min = iter->y;
	    }
	  else
	    {
	      _x_max = _x_min = iter->x;
	      _y_max = _y_min = iter->y;
	      initialized = true;
	    }		
	}
    }

  return initialized;
}


bool FireAnimation::MakeTriangles(std::vector< cpw::Point3d<float> > v0, 
				  std::vector< cpw::Point3d<float> > v1,
				  std::vector< int > v_match,
				  std::vector< cpw::Triangle > &t_vec,
				  const bool &close_interval)
{
  std::vector< cpw::Triangle > triangles;
  cpw::Triangle t0, t1;
  float x = ((float)img_resx-2) / (x_max - x_min);
  float y = ((float)img_resy-2) / (y_max - y_min);

  t_vec.clear();

  if (close_interval)
    {
      v1.push_back(*v1.begin());
      v0.push_back(*v0.begin());
      v_match.push_back( *v_match.begin() );
    }
  else
    {
      v1.push_back(v1.back());
      v0.push_back(v0.back());
      v_match.push_back( v_match.back() ) ;//[v0.size() - 1] );
    }

  //fill the triangle vector adapting the coordinates
  //xmin will be zero and xmax will be img_resx
  if (v0.size() == v1.size())
    {
      for (unsigned int i=0; i < v1.size()-1; i++)
	{
	  t0.v0 = v0[i];
	  t0.v1 = v0[i+1];
	  t0.v2 = v1[ v_match[i] ];

	  t1.v0 = v1[ v_match[i]   ];
	  t1.v1 = v1[ v_match[i+1] ];
	  t1.v2 = v0[i+1];

	  //adapt the coordinates to the image coordinates system
			
	  //the image is an ortographic projection, z is ignored
	  t0.v0.z = t0.v1.z = t0.v2.z = -100.0f;
	  t1.v0.z = t1.v1.z = t1.v2.z = -100.0f;

	  //X
	  //t0.v0.x = ((t0.v0.x - xmin) * img_resx) / (x_max-x_min) //unoptimized code
	  t0.v0.x = ((t0.v0.x - x_min) * x) + 1;
	  t0.v1.x = ((t0.v1.x - x_min) * x) + 1;
	  t0.v2.x = ((t0.v2.x - x_min) * x) + 1;

	  t1.v0.x = ((t1.v0.x - x_min) * x) + 1;
	  t1.v1.x = ((t1.v1.x - x_min) * x) + 1;
	  t1.v2.x = ((t1.v2.x - x_min) * x) + 1;

	  //Y
	  t0.v0.y = ((t0.v0.y - y_min) * y) + 1;
	  t0.v1.y = ((t0.v1.y - y_min) * y) + 1;
	  t0.v2.y = ((t0.v2.y - y_min) * y) + 1;

	  t1.v0.y = ((t1.v0.y - y_min) * y) + 1;
	  t1.v1.y = ((t1.v1.y - y_min) * y) + 1;
	  t1.v2.y = ((t1.v2.y - y_min) * y) + 1;

	  t_vec.push_back(t0);
	  t_vec.push_back(t1);
	}
    }
  else
    return false;

  return true;
}

void FireAnimation::Clear()
{
  if (image != NULL)
    image->Clear();
}

void FireAnimation::PreComputeColors(const int &pc, std::vector< cpw::Color > &colors)
{
  int perimeter_count = time_slice_animation.size();

  //three target colors
  cpw::Color red(1.0f, 0.0f, 0.0f, FIRE_ALPHA);
  cpw::Color green(0.0f, 1.0f, 0.0f, FIRE_ALPHA);
  cpw::Color blue(0.0f, 0.0f, 1.0f, FIRE_ALPHA);

  //fill the color vec
  colors.clear();

  colors.reserve( perimeter_count );

  switch (pc)
    {
    case 0:
      return;
      break;

    case 1:
      colors.push_back( red );
      break;

    case 2:
      colors.push_back( red );
      colors.push_back( green );
      break;

    case 3:
      colors.push_back( red );
      colors.push_back( green );
      colors.push_back( blue );
      break;

    default:
      {
	int middle  = pc / 2;
				
	float inc_a = green.red - red.red;
	float inc_b = green.green - red.green;
	float inc_c = green.blue - red.blue;
	float aux;

	for (int i=0; i<middle; i++)
	  {
	    aux = ((float)i) / ((float)middle);
	    colors.push_back( cpw::Color( red.red + aux * inc_a, red.green + aux * inc_b, red.blue + aux * inc_c, FIRE_ALPHA  ));
	  }

	inc_a = blue.red - green.red;
	inc_b = blue.green - green.green;
	inc_c = blue.blue - green.blue;

	for (int i=0; i<middle; i++)
	  {
	    aux = ((float)i) / ((float)middle);
	    colors.push_back( cpw::Color( green.red + aux * inc_a, green.green + aux * inc_b, green.blue + aux * inc_c, FIRE_ALPHA  ));
	  }
      }
      break;
    }

}


void FireAnimation::PreComputeColors()
{
  int perimeter_count = time_slice_animation.size();

  //three target colors
  cpw::Color red(1.0f, 0.0f, 0.0f, FIRE_ALPHA);
  cpw::Color green(0.0f, 1.0f, 0.0f, FIRE_ALPHA);
  cpw::Color blue(0.0f, 0.0f, 1.0f, FIRE_ALPHA);

  //fill the color vec
  default_colors.clear();

  default_colors.reserve( perimeter_count );

  switch (perimeter_count)
    {
    case 0:
      return;
      break;

    case 1:
      default_colors.push_back( red );
      break;

    case 2:
      default_colors.push_back( red );
      default_colors.push_back( green );
      break;

    case 3:
      default_colors.push_back( red );
      default_colors.push_back( green );
      default_colors.push_back( blue );
      break;

    default:
      {
	int middle  = perimeter_count / 2;
				
	float inc_a = green.red - red.red;
	float inc_b = green.green - red.green;
	float inc_c = green.blue - red.blue;
	float aux;

	for (int i=0; i<middle; i++)
	  {
	    aux = ((float)i) / ((float)middle);
	    default_colors.push_back( cpw::Color( red.red + aux * inc_a, red.green + aux * inc_b, red.blue + aux * inc_c, FIRE_ALPHA  ));
	  }

	inc_a = blue.red - green.red;
	inc_b = blue.green - green.green;
	inc_c = blue.blue - green.blue;

	for (int i=0; i<middle; i++)
	  {
	    aux = ((float)i) / ((float)middle);
	    default_colors.push_back( cpw::Color( green.red + aux * inc_a, green.green + aux * inc_b, green.blue + aux * inc_c, FIRE_ALPHA  ));
	  }
      }
      break;
    }
}
