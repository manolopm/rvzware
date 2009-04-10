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
#ifndef _MATH_
#define _MATH_

#include <cpw/common/types.h>
#include <cpw/Export.h>

#define CPW_PI 3.14159265358979323846

namespace cpw
{
	/** 
		\brief Contains several common mathematical functions
		\ingroup cpw
	*/
	class CPWEXPORT Math
	{
		public:

			/** Default constructor */
			Math();

			/** Destructor */
			~Math();
			
			/** Computes the length of a 3d vector 
				\param v is the 3d vector
			*/
			static double VectorLength(const cpw::Point3d<double> &v);

			/** Computes the length of a 3d vector 
				\param v is the 3d vector
			*/
			static float VectorLength(const cpw::Point3d<float> &v);
			
			/** Normalize a 3d vector 
				\param v is the 3d vector
			*/
			static void NormalizeVec(cpw::Point3d<double> &v);

			/** Normalize a 3d vector  
				\param v is the 3d vector
			*/
			static void NormalizeVec(cpw::Point3d<float> &v);

			/** Computes the cross product between two vectors
				\param v1 the first vector
				\param v2 the second vector
			*/
			static cpw::Point3d<double> CrossProduct(const cpw::Point3d<double> &v1, const cpw::Point3d<double> &v2);

			/** Computes the cross product between two vectors
				\param v1 the first vector
				\param v2 the second vector
			*/
			static cpw::Point3d<float> CrossProduct(const cpw::Point3d<float> &v1, const cpw::Point3d<float> &v2);

			/** Computes the scalar product between two vectors
				\param v1 the first vector
				\param v2 the second vector
			*/
			static double DotProduct(const cpw::Point3d<double> &v1, const cpw::Point3d<double> &v2) { return (v1.x * v2.x) + (v1.y * v2.y) + (v1.z * v2.z); }

			/** Computes the scalar product between two vectors
				\param v1 the first vector
				\param v2 the second vector
			*/
			static float DotProduct(const cpw::Point3d<float> &v1, const cpw::Point3d<float> &v2) { return (v1.x * v2.x) + (v1.y * v2.y) + (v1.z * v2.z); }

			/** Computes the distance between two 3d points
				\param p1 the first point
				\param p2 the second point
			*/			
			static double DistanceBetweenTwoPoints(const cpw::Point3d<double> &p1, const cpw::Point3d<double> &p2);

			/** Computes the distance between two 3d points
				\param p1 the first point
				\param p2 the second point
			*/			
			static float DistanceBetweenTwoPoints(const cpw::Point3d<float> &p1, const cpw::Point3d<float> &p2);

			/** Computes the angle between two 3d vectors
				\param v1 the first point
				\param v2 the second point
			*/			
			static double AngleBetweenTwoVectorsInRadians(cpw::Point3d<double> &v1, cpw::Point3d<double> &v2);

			/** Computes the angle between two 3d vectors
				\param v1 the first point
				\param v2 the second point
			*/			
			static float AngleBetweenTwoVectorsInRadians(cpw::Point3d<float> &v1, cpw::Point3d<float> &v2);

			/** Computes the angle between two 3d vectors in grades
				\param v1 the first point
				\param v2 the second point
			*/			
			static double AngleBetweenTwoVectorsInGrades(cpw::Point3d<double> &v1, cpw::Point3d<double> &v2);

			/** Computes the angle between two 3d vectors in grades
				\param v1 the first point
				\param v2 the second point
			*/			
			static float AngleBetweenTwoVectorsInGrades(cpw::Point3d<float> &v1, cpw::Point3d<float> &v2);

			/** Rotates a point with respect to a given axis
				\param point to be rotated
				\param axis for the rotation
				\param angle to be rotated
				\param angle_in_grades determines whether the angle is in grades or radians
			*/			
			static void RotatePoint(cpw::Point3d<double> &point,const cpw::Point3d<double> &axis, const double &angle, const bool &angle_in_grades = true);

			/** Rotates a point with respect to a given axis
				\param point to be rotated
				\param axis for the rotation
				\param angle to be rotated
				\param angle_in_grades determines whether the angle is in grades or radians
			*/			
			static void RotatePoint(cpw::Point3d<float> &point,const cpw::Point3d<float> &axis, const float &angle, const bool &angle_in_grades = true);

			/** Returns the value of PI */
			static double PI() { return CPW_PI;}
	};
	
	/** Class to represent a geometrical plane 
	*/
	class CPWEXPORT Plane
	{
		public:
			
			/** Default constructor */
			Plane();

			/** Constructor */
			Plane(const double &x, const double &y, const double &z, const double &w);
			
			/** Copy constructor
				\param plane to copy
			*/
			Plane(const Plane &plane);

			/** Destructor */
			~Plane();

			/** Overload of operator =
				\param plane to copy
			*/
			Plane & operator =(const Plane &plane);

			/** Assigns a new value to the x coordinate
				\param x
			*/
			void SetX(const double &x) { coords[0] = x; }

			/** Assigns a new value to the y coordinate
				\param y
			*/
			void SetY(const double &y) { coords[1] = y; }

			/** Assigns a new value to the z coordinate
				\param z
			*/
			void SetZ(const double &z) { coords[2] = z; }

			/** Assigns a new value to the w coordinate
				\param w
			*/
			void SetW(const double &w) { coords[3] = w; }

			/** Obtains the x coordinate */
			double & GetX() { return coords[0]; }

			/** Obtains the y coordinate */
			double & GetY() { return coords[1]; }

			/** Obtains the z coordinate */
			double & GetZ() { return coords[2]; }

			/** Obtains the w coordinate */
			double & GetW() { return coords[3]; }

			/** Obtains the x coordinate */
			const double & GetX() const { return coords[0]; }

			/** Obtains the y coordinate */
			const double & GetY() const { return coords[1]; }

			/** Obtains the z coordinate */
			const double & GetZ() const { return coords[2]; }

			/** Obtains the w coordinate */
			const double & GetW() const { return coords[3]; }

			/** Returns the angle between to planes 
				\param plane is the second plane
			*/
			double Angle(const Plane &plane);

			/** Computes the distance between the plane and a point
				\param point
			*/
			double Distance(const cpw::Point3d<double> &point);

			/** Computes the distance between the plane and a point
				\param point
			*/
			double Distance(const cpw::Point3d<float> &point);

			/** Computes the distance between the plane and a point
				\param point
			*/
			const double Distance(const cpw::Point3d<double> &point) const;

			/** Computes the distance between the plane and a point
				\param point
			*/
			const double Distance(const cpw::Point3d<float> &point) const;

			/** Computes the signed distance between the plane and a point
				\param point
			*/
			double DistanceSigned(const cpw::Point3d<double> &point);

			/** Computes the signed distance between the plane and a point
				\param point
			*/
			double DistanceSigned(const cpw::Point3d<float> &point);

			/** Computes the signed distance between the plane and a point
				\param point
			*/
			const double DistanceSigned(const cpw::Point3d<double> &point) const;

			/** Computes the signed distance between the plane and a point
				\param point
			*/
			const double DistanceSigned(const cpw::Point3d<float> &point) const;


			/** Creates a plane from 3 points
				\param p1
				\param p2
				\param p3
			*/
			void MakeFrom3Points(const cpw::Point3d<double> &p1, const cpw::Point3d<double> &p2, const cpw::Point3d<double> &p3);

			/** Creates a plane from 3 points
				\param p1
				\param p2
				\param p3
			*/
			void MakeFrom3Points(const cpw::Point3d<float> &p1, const cpw::Point3d<float> &p2, const cpw::Point3d<float> &p3);

			/** Projects a vector on a plane
			    \param v
			*/
			cpw::Point3d<double> ProjectVectorOnPlane(const cpw::Point3d<double> &v);

			/** Projects a vector on a plane
			    \param v
			*/
			cpw::Point3d<float> ProjectVectorOnPlane(const cpw::Point3d<float> &v);


		private:
			
			double coords[4]; ///< plane coordinates
	};

	/** Class used to represent a quaternion 
	*/
	class CPWEXPORT Quaternion
	{
		public:

			/** Default constructor */
			Quaternion();

			/** Copy constructor 
				\param quat quaternion to copy
			*/
			Quaternion(const Quaternion &quat);

			/** Destructor */
			~Quaternion();
			
			/** Overload of operator =
				\param quat quaternion to copy
			*/
			Quaternion & operator =(const Quaternion &quat);
			
			/** Overload of operator [] to obtain the quaternion values
				\param index 
			*/
			double & operator [](int index) { return coords[index]; }
			
			/** Overload of operator * to multiply two quaternions
				\param quat 
			*/
			Quaternion operator *(const Quaternion &quat);
			
			/** Overload of operator * to multiply two quaternions
				\param quat 
			*/
			const Quaternion operator *(const Quaternion &quat) const;

			/** Assigns a new value to the first coordinate
				\param x
			*/
			void SetX(const double &x) { coords[0] = x; }

			/** Assigns a new value to the second coordinate
				\param y
			*/
			void SetY(const double &y) { coords[1] = y; }

			/** Assigns a new value to the third coordinate
				\param z
			*/
			void SetZ(const double &z) { coords[2] = z; }

			/** Assigns a new value to the fourth coordinate
				\param w
			*/
			void SetW(const double &w) { coords[3] = w; }


			/** Returns the first coordinate */
			double & GetX() { return coords[0]; }
			
			/** Returns the second coordinate */
			double & GetY() { return coords[1]; }
			
			/** Returns the third coordinate */
			double & GetZ() { return coords[2]; }
			
			/** Returns the fourth coordinate */
			double & GetW() { return coords[3]; }

			/** Obtains the conjugate quaternion */
			void Conjugate();

		private:

			double coords[4];  ///< coordinates of the quaternion
	};
}

#endif