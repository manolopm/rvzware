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

#ifndef _ELEMENTPARTICLESYSTEM_
#define _ELEMENTPARTICLESYSTEM_

#include <cpw/entity/Element.h>
#include <cpw/common/types.h>
#include <cpw/Export.h>

#include <vector>
#include <string>

namespace cpw {

	/** 
		\brief 
		\ingroup cpw
	*/
	class CPWEXPORT ElementParticleSystem: public Element
	{

		public:

			ElementParticleSystem(const cpw::TypeId id = cpw::TypeId(), 
					const std::string &class_name = std::string("ElementParticleSystem"));

			ElementParticleSystem(const ElementParticleSystem &element);

			virtual ~ElementParticleSystem();

			ElementParticleSystem &operator = (const ElementParticleSystem &element);

			virtual Entity* Clone() {return new ElementParticleSystem(*this);}

			void SetAlpha(const cpw::Range<float> &_alpha);
			void SetSpeed(const cpw::Range<float> &_speed);
			void SetPhi(const cpw::Range<float> &_phi);
			void SetTheta(const cpw::Range<float> &_theta);
			void SetSize(const cpw::Range<float> &_size);
			void SetNewParticlesSec(const cpw::Range<int> _particles_sec);
			void SetAdditiveBlend(const bool &_additive_blend);
			void SetSenoidalInterpolator(const bool &_senoidal_interpolator);
			void SetParticlesLife(const float &_particles_life);
			void SetCoords(cpw::Point3d<float> _coordinates);
			void SetMultiPoint(bool mp);
			void SetTextureFileName(const std::string &_texture_filename);
			void SetPolygonal(const bool &_polygonal);
			void SetInsertedInScene(const bool &iis);
			void SetUpdatedInScene(const bool &uis);
			void AddVertex(const cpw::Point3d<float> &new_vertex);

			virtual void SetValue(const std::string &f, const std::string &v);

			cpw::Range<float> &GetAlpha() { return alpha; }
			cpw::Range<float> &GetSpeed() { return speed; }
			cpw::Range<float> &GetPhi() { return phi; }
			cpw::Range<float> &GetTheta() { return theta; }
			cpw::Range<float> &GetSize() { return size; }
			cpw::Range<int> &GetNewParticlesSec() { return particles_sec; }
			bool GetAdditiveBlend() { return additive_blend; }
			bool GetSenoidalInterpolator() { return senoidal_interpolator; }
			cpw::Point3d<float> &GetCoords() { return coordinates; }
			bool GetVertex(int index, cpw::Point3d<float> &vertex);
			int GetNumVertices() { return (int) vertices.size(); }
			void ClearVertices() { vertices.clear(); }
			float GetParticlesLife() { return particles_life; }
			bool GetIsMultiPoint() { return multi_point; }
			int GetVertexCount() { return ((int)vertices.size()); }
			std::vector< cpw::Point3d<float> > & GetVertices() { return vertices; }

			std::string &GetTextureFileName() { return texture_filename; }
			const std::string &GetTextureFileName() const { return texture_filename; }
			bool GetPolygonal(){ return polygonal; }
			bool GetInsertedInScene() { return inserted_in_scene; }
			bool GetUpdatedInScene() { return updated_in_scene; }

			const float & GetMaxPerimeterRadius() { return max_perimeter_radius; }
			void SetMaxPerimeterRadius(const float &_max_perimeter_radius) { max_perimeter_radius = _max_perimeter_radius; }

		protected:

			virtual int CreatePersistence();
			virtual int AdaptPersistence(Node *root);

		private:
			
			cpw::Range<float> alpha;
			cpw::Range<float> speed;
			cpw::Range<float> phi;
			cpw::Range<float> theta;		   
			cpw::Range<float> size;
			cpw::Point3d<float> coordinates;
			cpw::Range<int> particles_sec;
			bool senoidal_interpolator;
			float particles_life;
			float max_perimeter_radius;
			bool additive_blend;
			bool multi_point;
			std::string texture_filename;
			bool polygonal;
			bool inserted_in_scene, updated_in_scene;

			std::vector< cpw::Point3d<float> > vertices;
	};

}


#endif 
