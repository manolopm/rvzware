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
#ifndef _TYPES_
#define _TYPES_

#include <cpw/Export.h>

#include <string>
#include <iostream>

namespace cpw 
{

	/** 
		\brief Possible error values for a persistent operation 
		\ingroup cpw	
	*/
	enum PersistentError 
	{
		PERSISTENTOK = 0,                  ///< the persistent operation was correct
		PERSISTENTALREADYEXISTS = -1,	   ///< the entity to save already exists on the device
		PERSISTENTNOTFOUND = -2,		   ///< the entity cannot be load from the devide 
		PERSISTENTPERMISSIONDENIED = -3,   ///< permission denied to save on the device	
		PERSISTENTNOTAVAILABLE = -4	       ///< any other error not identified
	};


	typedef void IWindow;
	typedef void IGraphic;
	//typedef void IImage;
	
	/** 
		\brief Template class to represent a 2d point 
		\ingroup cpw	
	*/
	template <typename T> struct Point2d {
		T x,y;   ///< x, y values

		/** Default constructor */
		Point2d():x(), y(){}

		/** Constructor 
			\param xx the x coordinate
			\param yy the y coordinate	
		*/
		Point2d(const T &xx, const T &yy):x(xx), y(yy){}

		/** Copy constructor
			\param point to copy
		*/
		Point2d(const Point2d &point): x(point.x), y(point.y){}

		/** Overload of operator =
			\param point to copy
		*/
		Point2d & operator = (const Point2d &point) { x=point.x; y=point.y; return *this;}

		/** Compares two 2d points. Returns true if they are equal
			\param point to compare
		*/
		bool operator ==(const Point2d &point) { if ((x == point.x) && (y == point.y)) return true; return false; }
		
		/** Compares two 2d points. Returns true if they are not equal
			\param point to compare
		*/
		bool operator !=(const Point2d &point) { if ((x == point.x) && (y == point.y)) return false; return true; }

		/** Subtracts two 2d points
			\param point to subtract
		*/
		Point2d operator -(const Point2d &point) { Point2d out; out.x = x - point.x; out.y = y - point.y; return out;}

		/** Subtracts two 2d points
			\param point to subtract
		*/
		const Point2d operator -(const Point2d &point) const{ Point2d out; out.x = x - point.x; out.y = y - point.y; return out;}

		/** Sums two 2d points
			\param point to sum
		*/
		Point2d operator +(const Point2d &point) { Point2d out; out.x = x + point.x; out.y = y + point.y; return out;}
		
		/** Sums two 2d points
			\param point to sum
		*/
		const Point2d operator +(const Point2d &point) const{ Point2d out; out.x = x + point.x; out.y = y + point.y; return out;}
	};

	/** 
		\brief Template class to represent a 2d point 
		\ingroup cpw
	*/
	template <typename T> struct Point3d {
		T x, y, z; ///< x, y, z values

		/** Default constructor */
		Point3d():x(), y(){}

		/** Constructor */
		Point3d(const T &xx, const T &yy, const T &zz):x(xx), y(yy), z(zz) {}

		/** Copy constructor 
			\param point to copy
		*/
		Point3d(const Point3d &point): x(point.x), y(point.y), z(point.z){}

		/** Overload of operator =
			\param point to copy
		*/
		Point3d & operator = (const Point3d &point) { x=point.x; y=point.y; z=point.z; return *this;}
		
		/** Compares two 3d points. Returns true if they are equal
			\param point to compare
		*/
		bool operator ==(const Point3d &point) { if ((x == point.x) && (y == point.y) && (z == point.z)) return true; return false; }

		/** Compares two 3d points. Returns true if they are not equal
			\param point to compare
		*/
		bool operator !=(const Point3d &point) { if ((x == point.x) && (y == point.y) && (z == point.z)) return false; return true; }
		
		/** Subtracts two 3d points
			\param point to subtract
		*/
		Point3d operator -(const Point3d &point) { Point3d out; out.x = x - point.x; out.y = y - point.y; out.z = z - point.z; return out;}
		
		/** Subtracts two 3d points
			\param point to subtract
		*/
		const Point3d operator -(const Point3d &point) const{ Point3d out; out.x = x - point.x; out.y = y - point.y; out.z = z - point.z; return out;}
		
		/** Adds two 3d points
			\param point to subtract
		*/
		Point3d operator +(const Point3d &point) { Point3d out; out.x = x + point.x; out.y = y + point.y; out.z = z + point.z; return out;}
		
		/** Adds two 3d points
			\param point to subtract
		*/
		const Point3d operator +(const Point3d &point) const{ Point3d out; out.x = x + point.x; out.y = y + point.y; out.z = z + point.z; return out;}

		/** Add a 3d point to the object
			\param point to add
		*/
		void operator +=(const Point3d &point) { x += point.x; y += point.y; z += point.z; }

		/** Subtracts a 3d point to the object
			\param point to subtract
		*/
		void operator -=(const Point3d &point) { x -= point.x; y -= point.y; z -= point.z; }
	};


	/** 
		\brief Template class to represent a range of values 
		\ingroup cpw	
	*/
	template <typename T> struct Range {
		#undef min    //esto es por si hay algún define anterior de min y max (ocurre cuando incluimos encabezados precompilados)
		#undef max
		T min,max;    ///< min and max values of the range

		/** Default constructor */
		Range():min(), max(){}

		/** Constructor
			\param minn min value
			\param maxx max value
		*/
		Range(const T &minn, const T &maxx):min(minn), max(maxx) {}

		/** Copy constructor
			\param range to copy
		*/
		Range(const Range &range): min(range.min), max(range.max){}
		
		/** Overload of operator =
			\param range to copy
		*/
		Range & operator = (const Range &range) { min=range.min; max=range.max; return *this;}
		
		/** Compares two ranges. Returns true if they are equal
			\param range to compare
		*/
		bool operator ==(const Range &range) { if ((min == range.min) && (max == range.max)) return true; return false; }
		
		/** Compares two ranges. Returns true if they are not equal
			\param range to compare
		*/
		bool operator !=(const Range &range) { if ((min == range.min) && (max == range.max)) return false; return true; }
	};

	/** 
		\brief Structure to represent time 
		\ingroup cpw
	*/
	typedef struct 
	{
	    unsigned long int seconds;  ///< number of seconds since Jan 1, 1970
	    unsigned int miliseconds;   ///< miliseconds 
	}cpwTime;

	/** 
		\brief Triangle structure 
		\ingroup cpw
	*/
	struct Triangle
	{
		/** Default constructor */
		Triangle(){}
		
		/** Copy constructor 
			\param triangle to copy
		*/
		Triangle(const Triangle &triangle) : v0(triangle.v0), v1(triangle.v1), v2(triangle.v2)
		{}
		
		/** Compares two triangles. Returns true if they are equal
			\param triangle to compare
		*/
		bool operator ==(const Triangle &triangle) { if ((v0 == triangle.v0) && (v1 == triangle.v1) && (v2 == triangle.v2)) return true; else return false; }

		/** Overload of operator =
			\param triangle to copy
		*/
		Triangle & operator =(const Triangle &triangle)
		{
			v0 = triangle.v0;
			v1 = triangle.v1;
			v2 = triangle.v2;
			return *this;
		}

		cpw::Point3d<float> v0, v1, v2;  ///< the three vertices of the triangle
	};

	/** 
		\brief Structure to represent a pixel color 
		\ingroup cpw
	*/
	struct Color
	{
		/** Default constructor */
		Color() : red(1.0f), green(1.0f), blue(1.0f), alpha(1.0f)
		{}

		/** Constructor 
			 \param red for the red color
			 \param green for the green color
			 \param blue for the blue color
			 \param alpha for the transparency level
		*/
		Color(const float &_red, const float &_green, const float &_blue, const float &_alpha) : red(_red), green(_green), blue(_blue), alpha(_alpha)
		{}

		/** Copy constructor
			\param color to copy
		*/
		Color(const Color &color) : red(color.red), green(color.green), blue(color.blue), alpha(color.alpha)
		{}

		/** Overload of operator =
			\param color to copy
		*/
		Color & operator =(const Color &color)
		{
			red = color.red; blue = color.blue; green = color.green; alpha = color.alpha;
			return *this;
		}


		float red, green, blue, alpha;  ///< the four channels of a pixel color 
	};
}

#endif

