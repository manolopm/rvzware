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

#ifndef _ELEMENT_
#define _ELEMENT_

#include <cpw/common/Attribute.h>
#include <cpw/entity/Entity.h>
#include <cpw/Export.h>

#include <sstream>
#include <string>
#include <vector>


namespace cpw {

	/** 
		\brief 
		\ingroup cpw
	*/
	class CPWEXPORT Element: public Entity
	{

	public:

		Element(const cpw::TypeId id = cpw::TypeId(), 
			    const std::string &class_name = std::string("Element"));

		Element(const cpw::TypeId id, const std::string &nam, 
				const std::string &prim, 
			    const float *ut, const float *orient, const float *sca,
			    const std::string &tex, const std::string &htm,	
   		        const bool dyn, const bool vis, const bool anim, const std::string &desc, 
				const std::vector<Attribute> &attr,
				const bool &_allow_under_terrain = true,
				const bool &_adjust_height_to_terrain = true,
				const std::string &class_name = std::string("Element"));


		Element(const Element &element);
		virtual ~Element();

		double *GetUtmd();
		//float *GetUtm();
		double GetUtmd(int i) ;
		float GetUtm(int i) ;
		float *GetOrientation();
		float &GetOrientation(int i);
		float *GetScale();
		float &GetScale(int i);
		std::string &GetText();

		const double *GetUtmd() const;
		//const float *GetUtm() const;
		const double GetUtmd(int i) const;
		const float GetUtm(int i) const;
		const float *GetOrientation() const;
		const float &GetOrientation(int i) const;
		const float *GetScale() const;
		const float &GetScale(int i) const;
		const std::string &GetText() const;

		void SetUtm(const float *value);
		void SetUtmd(const double *value);
		void SetUtmd(const double x,const double y,const double z);
		void SetUtm(const float x,const float y,const float z);
		void SetOrientation(const float *value);
		void SetOrientation(const float &x, const float &y, const float &z);
		void SetScale(const float *value);
		void SetScale(const float &x, const float &y, const float &z);
		void SetText(const std::string &str);

		virtual void SetValue(const std::string &f, const std::string &v);

		Element &operator = (const Element &element);

		virtual Entity* Clone();

		bool GetAllowUnderTerrain() { return allow_under_terrain; }
		bool GetAdjustToTerrainHeight() { return adjust_height_to_terrain; }

		void SetAllowUnderTerrain(const bool &_allow_under_terrain); 
		void SetAdjustToTerrainHeight(const bool &_adjust_height_to_terrain); 



	protected:

		virtual int CreatePersistence();
		virtual int CreateProperties();
		virtual int AdaptPersistence(Node *root);


	private:

		double  utm[3];
		float  orientation[3];
		float  scale[3];
		std::string text;

		bool allow_under_terrain, adjust_height_to_terrain;

	};

}


#endif 

