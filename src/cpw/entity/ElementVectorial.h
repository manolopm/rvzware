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
#ifndef _ELEMENTVECTORIAL_
#define _ELEMENTVECTORIAL_

#include <cpw/entity/Element.h>
#include <cpw/Export.h>

namespace cpw {

	/** 
		\brief Por comentar ....
		\ingroup cpw
	*/
	class CPWEXPORT ElementVectorial: public Element
	{

	public:

		ElementVectorial(const cpw::TypeId id = cpw::TypeId(), 
			    const std::string &class_name = std::string("ElementVectorial"));
		ElementVectorial(const ElementVectorial &element);
		virtual ~ElementVectorial();

		ElementVectorial &operator = (const ElementVectorial &element);

		virtual Entity* Clone() {return new ElementVectorial(*this);}

		std::string &GetColor();
		int GetWidth();
		std::vector<cpw::Point3d<float> > &GetPoints();
		int GetNumPoints() {return points.size();}
		cpw::Point3d<float>* GetPoint(const int &index) {return index<(int)points.size()? new cpw::Point3d<float>(points[index]):NULL;}
		bool IsAdapted() {return terrain_adapt;}

		const std::string &GetColor() const ;
		const int GetWidth() const;
		const std::vector<cpw::Point3d<float> > &GetPoints() const;
		const int GetNumPoints() const {return points.size();}
		const cpw::Point3d<float>* GetPoint(const int &index) const {return index<(int)points.size()? new cpw::Point3d<float>(points[index]):NULL;}
		const bool IsAdapted() const {return terrain_adapt;}

		void SetColor(const std::string &color);
		void SetWidth(const int width);
		void SetPoints(const std::vector<cpw::Point3d<float> > &points);
		void SetTerrainAdapt(const bool &adapt) {terrain_adapt = adapt;}

		void SetRGBColour(const int &r, const int &g, const int &b);
		void GetRGBColour(int &r, int &g, int &b);

		void SetVectorialType(const std::string &vname) {type = vname;}
		void SetvectorialIcons(const std::string &parent_icon, const std::string &point_icon){parenticon = parent_icon; pointicon = point_icon;}

		std::string GetVectorialType(){return type;}
		void GetVectorialIcons(std::string &parent, std::string &point){parent = parenticon; point=pointicon;}

		void UpdatePoint(const int &index) {update_point = index;}
		int GetUpdatePoint() {return update_point;}
		const int GetUpdatePoint() const {return update_point;}
		virtual void SetValue(const std::string &f, const std::string &v);

		void SetLine(const bool &isLine) {isline = isLine;}
		bool &GetLine() {return isline;}

	protected:

		virtual int CreatePersistence();
		virtual int AdaptPersistence(Node *root);
		virtual int CreateProperties();

	private:
		std::string color;
		int width;
		bool terrain_adapt;
		std::vector<cpw::Point3d<float> > points;
		bool isline;

		int red, green, blue; //0-255 values

		int update_point;
		std::string type;
		std::string parenticon, pointicon;

	};

}


#endif 
