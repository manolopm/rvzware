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
#include <math.h>

//#include <stdafx.h>

#include <cpw/common/Math.h>

using namespace cpw;



Math::Math(void)
{
}

Math::~Math(void)
{
}

double Math::VectorLength(const cpw::Point3d<double> &v)
{
	return sqrt((v.x * v.x) + (v.y * v.y) + (v.z * v.z));
}

float Math::VectorLength(const cpw::Point3d<float> &v)
{
	return sqrt((v.x * v.x) + (v.y * v.y) + (v.z * v.z));
}

void Math::NormalizeVec(cpw::Point3d<double> &v)
{
	double lenght_inv = 1.0 / (sqrt((v.x * v.x) + (v.y * v.y) + (v.z * v.z)));

	v.x *= lenght_inv;
	v.y *= lenght_inv;
	v.z *= lenght_inv;
}

void Math::NormalizeVec(cpw::Point3d<float> &v)
{
	float lenght_inv = 1.0 / (sqrt((v.x * v.x) + (v.y * v.y) + (v.z * v.z)));

	v.x *= lenght_inv;
	v.y *= lenght_inv;
	v.z *= lenght_inv;
}

cpw::Point3d<double> Math::CrossProduct(const cpw::Point3d<double> &p1, const cpw::Point3d<double> &p2)
{
	cpw::Point3d<double> out;

	out.x = (p1.y * p2.z) - (p1.z * p2.y);
	out.y = (p1.z * p2.x) - (p1.x * p2.z);
	out.z = (p1.x * p2.y) - (p1.y * p2.x);

	return out;
}

cpw::Point3d<float> Math::CrossProduct(const cpw::Point3d<float> &p1, const cpw::Point3d<float> &p2)
{
	cpw::Point3d<float> out;

	out.x = (p1.y * p2.z) - (p1.z * p2.y);
	out.y = (p1.z * p2.x) - (p1.x * p2.z);
	out.z = (p1.x * p2.y) - (p1.y * p2.x);

	return out;
}

double Math::DistanceBetweenTwoPoints(const cpw::Point3d<double> &p1, const cpw::Point3d<double> &p2)
{
	double a = p1.x - p2.x;
	double b = p1.y - p2.y;
	double c = p1.z - p2.z;

	return sqrt((a*a) + (b*b) + (c*c));
}
float  Math::DistanceBetweenTwoPoints(const cpw::Point3d<float> &p1, const cpw::Point3d<float> &p2)
{
	float a = p1.x - p2.x;
	float b = p1.y - p2.y;
	float c = p1.z - p2.z;

	return sqrt((a*a) + (b*b) + (c*c));
}


double Math::AngleBetweenTwoVectorsInRadians(cpw::Point3d<double> &v1, cpw::Point3d<double> &v2)
{
	return acos(cpw::Math::DotProduct(v1, v2) / ( cpw::Math::VectorLength(v1) * cpw::Math::VectorLength(v2)));
}
float Math::AngleBetweenTwoVectorsInRadians(cpw::Point3d<float> &v1, cpw::Point3d<float> &v2)
{
	float cost = cpw::Math::DotProduct(v1, v2) / ( cpw::Math::VectorLength(v1) * cpw::Math::VectorLength(v2));
	
	if (cost < -1) cost = -1;
	else if (cost >  1)	cost = 1;
	
	return acos(cost);
}

double Math::AngleBetweenTwoVectorsInGrades(cpw::Point3d<double> &v1, cpw::Point3d<double> &v2)
{
	double dot_product = cpw::Math::DotProduct(v1, v2);
	double angle_in_radians = acos(dot_product / ( cpw::Math::VectorLength(v1) * cpw::Math::VectorLength(v2)));
	
	return ((angle_in_radians * 360) / (2*CPW_PI));
}
float Math::AngleBetweenTwoVectorsInGrades(cpw::Point3d<float> &v1, cpw::Point3d<float> &v2)
{
	float cost = cpw::Math::DotProduct(v1, v2) / ( cpw::Math::VectorLength(v1) * cpw::Math::VectorLength(v2));

	if (cost < -1) cost = -1;
	else if (cost >  1)	cost = 1;

	float angle_in_radians = acos(cost);

	return ((angle_in_radians * 360) / (2*CPW_PI));
}

void Math::RotatePoint(cpw::Point3d<double> &point,const cpw::Point3d<double> &axis, const double &angle, const bool &angle_in_grades)
{
	double angle_in_radians;
	
	if (angle_in_grades)
	{ 
		angle_in_radians = angle * ((2.0 * CPW_PI) / 360.0);
		angle_in_radians *= 0.5;
	}
	else
		angle_in_radians = angle;
	
	double sinus = sin(angle_in_radians);
	double cosinus = cos(angle_in_radians);
	
	cpw::Quaternion rotation_q, rotation_q_conjugate, point_q, result1, result2;

	rotation_q[0] = axis.x * sinus; rotation_q[1] = axis.y * sinus;
	rotation_q[2] = axis.z * sinus; rotation_q[3] = cosinus;

	rotation_q_conjugate[0] = -rotation_q[0]; rotation_q_conjugate[1] = -rotation_q[1];
	rotation_q_conjugate[2] = -rotation_q[2]; rotation_q_conjugate[3] = cosinus;

	point_q[0] = point.x; point_q[1] = point.y;
	point_q[2] = point.z; point_q[3] = 0.0f;

	result1 = rotation_q * point_q;
	result2 = result1 * rotation_q_conjugate;

	point.x = result2[0];
	point.y = result2[1];
	point.z = result2[2];
}

void Math::RotatePoint(cpw::Point3d<float> &point,const cpw::Point3d<float> &axis, const float &angle, const bool &angle_in_grades)
{
	float angle_in_radians;
	
	if (angle_in_grades)
	{ 
		angle_in_radians = angle * ((2.0f * CPW_PI) / 360.0f);
		angle_in_radians *= 0.5f;
	}
	else
		angle_in_radians = angle;
	
	double sinus = sin(angle_in_radians);
	double cosinus = cos(angle_in_radians);
	
	cpw::Quaternion rotation_q, rotation_q_conjugate, point_q, result1, result2;

	rotation_q[0] = axis.x * sinus; rotation_q[1] = axis.y * sinus;
	rotation_q[2] = axis.z * sinus; rotation_q[3] = cosinus;

	rotation_q_conjugate[0] = -rotation_q[0]; rotation_q_conjugate[1] = -rotation_q[1];
	rotation_q_conjugate[2] = -rotation_q[2]; rotation_q_conjugate[3] = cosinus;

	point_q[0] = point.x; point_q[1] = point.y;
	point_q[2] = point.z; point_q[3] = 0.0f;

	result1 = rotation_q * point_q;
	result2 = result1 * rotation_q_conjugate;

	point.x = result2[0];
	point.y = result2[1];
	point.z = result2[2];
}



Plane::Plane(void)
{
}
Plane::Plane(const Plane &plane)
{
	coords[0] = plane.coords[0];
	coords[1] = plane.coords[1];
	coords[2] = plane.coords[2];
	coords[3] = plane.coords[3];
}

Plane::Plane(const double &x, const double &y, const double &z, const double &w)
{
	coords[0] = x;
	coords[1] = y;
	coords[2] = z;
	coords[3] = w;

}

Plane::~Plane(void)
{
}

Plane & Plane::operator =(const Plane &plane)
{
	coords[0] = plane.coords[0];
	coords[1] = plane.coords[1];
	coords[2] = plane.coords[2];
	coords[3] = plane.coords[3];
	return *this;
}

double Plane::Angle(const Plane &plane)
{
	cpw::Point3d<double> n1(coords[0], coords[1], coords[2]);
	cpw::Point3d<double> n2(plane.coords[0], plane.coords[1], plane.coords[2]);

	return Math::AngleBetweenTwoVectorsInGrades(n1, n2);
}

double Plane::Distance(const cpw::Point3d<double> &point)
{
	double a = abs((coords[0] * point.x) + (coords[1] * point.y) + (coords[2] * point.z) + coords[3]);
	double b = sqrt((coords[0] * coords[0]) + (coords[1] * coords[1]) + (coords[2] * coords[2]));
	return a / b;
}

double Plane::Distance(const cpw::Point3d<float> &point)
{
	double a = abs((coords[0] * point.x) + (coords[1] * point.y) + (coords[2] * point.z) + coords[3]);
	double b = sqrt((coords[0] * coords[0]) + (coords[1] * coords[1]) + (coords[2] * coords[2]));
	return a / b;
}

const double Plane::Distance(const cpw::Point3d<double> &point)const 
{
	double a = abs((coords[0] * point.x) + (coords[1] * point.y) + (coords[2] * point.z) + coords[3]);
	double b = sqrt((coords[0] * coords[0]) + (coords[1] * coords[1]) + (coords[2] * coords[2]));
	return a / b;
}

const double Plane::Distance(const cpw::Point3d<float> &point)const 
{
	double a = abs((coords[0] * point.x) + (coords[1] * point.y) + (coords[2] * point.z) + coords[3]);
	double b = sqrt((coords[0] * coords[0]) + (coords[1] * coords[1]) + (coords[2] * coords[2]));
	return a / b;
}
double Plane::DistanceSigned(const cpw::Point3d<double> &point)
{
	double a = (coords[0] * point.x) + (coords[1] * point.y) + (coords[2] * point.z) + coords[3];
	double b = sqrt((coords[0] * coords[0]) + (coords[1] * coords[1]) + (coords[2] * coords[2]));
	return a / b;
}
double Plane::DistanceSigned(const cpw::Point3d<float> &point)
{
	double a = (coords[0] * point.x) + (coords[1] * point.y) + (coords[2] * point.z) + coords[3];
	double b = sqrt((coords[0] * coords[0]) + (coords[1] * coords[1]) + (coords[2] * coords[2]));
	return a / b;
}
const double Plane::DistanceSigned(const cpw::Point3d<double> &point) const 
{
	double a = (coords[0] * point.x) + (coords[1] * point.y) + (coords[2] * point.z) + coords[3];
	double b = sqrt((coords[0] * coords[0]) + (coords[1] * coords[1]) + (coords[2] * coords[2]));
	return a / b;
}
const double Plane::DistanceSigned(const cpw::Point3d<float> &point) const
{
	double a = (coords[0] * point.x) + (coords[1] * point.y) + (coords[2] * point.z) + coords[3];
	double b = sqrt((coords[0] * coords[0]) + (coords[1] * coords[1]) + (coords[2] * coords[2]));
	return a / b;
}

void Plane::MakeFrom3Points(const cpw::Point3d<double> &p1, const cpw::Point3d<double> &p2, const cpw::Point3d<double> &p3)
{
	cpw::Point3d<double> v1, v2, normal;
	
	v1.x = p2.x - p1.x; 
	v1.y = p2.y - p1.y; 
	v1.z = p2.z - p1.z; 

	v2.x = p3.x - p1.x;
	v2.y = p3.y - p1.y; 
	v2.z = p3.z - p1.z;

	Math::NormalizeVec(v1);
	Math::NormalizeVec(v2);

	normal = Math::CrossProduct(v1, v2);

	Math::NormalizeVec(normal);

	coords[0] = normal.x;
	coords[1] = normal.y;
	coords[2] = normal.z;
	coords[3] = 0.0;
	coords[3] -= normal.x * p1.x;
	coords[3] -= normal.y * p1.y;
	coords[3] -= normal.z * p1.z;

}
void Plane::MakeFrom3Points(const cpw::Point3d<float> &p1, const cpw::Point3d<float> &p2, const cpw::Point3d<float> &p3)
{
	cpw::Point3d<double> v1, v2, normal;
	
	v1.x = p2.x - p1.x; 
	v1.y = p2.y - p1.y; 
	v1.z = p2.z - p1.z; 

	v2.x = p3.x - p1.x;
	v2.y = p3.y - p1.y; 
	v2.z = p3.z - p1.z;

	Math::NormalizeVec(v1);
	Math::NormalizeVec(v2);

	normal = Math::CrossProduct(v1, v2);

	Math::NormalizeVec(normal);

	coords[0] = normal.x;
	coords[1] = normal.y;
	coords[2] = normal.z;
	coords[3] = 0.0;
	coords[3] -= normal.x * p1.x;
	coords[3] -= normal.y * p1.y;
	coords[3] -= normal.z * p1.z;
}

cpw::Point3d<double> Plane::ProjectVectorOnPlane(const cpw::Point3d<double> &vec)
{
	cpw::Point3d<double> projected_vector, plane_normal, u;
	
	plane_normal.x = coords[0];
	plane_normal.y = coords[1];
	plane_normal.z = coords[2];

	double dot = cpw::Math::DotProduct(vec, plane_normal);

	u.x = plane_normal.x * dot;
	u.y = plane_normal.y * dot;
	u.z = plane_normal.z * dot;

	projected_vector.x = vec.x - u.x;
	projected_vector.y = vec.y - u.y;
	projected_vector.z = vec.z - u.z;

	return projected_vector;
}

cpw::Point3d<float> Plane::ProjectVectorOnPlane(const cpw::Point3d<float> &vec)
{
	cpw::Point3d<float> projected_vector, plane_normal, u;
	
	plane_normal.x = coords[0];
	plane_normal.y = coords[1];
	plane_normal.z = coords[2];

	double dot = cpw::Math::DotProduct(vec, plane_normal);

	u.x = plane_normal.x * dot;
	u.y = plane_normal.y * dot;
	u.z = plane_normal.z * dot;

	projected_vector.x = vec.x - u.x;
	projected_vector.y = vec.y - u.y;
	projected_vector.z = vec.z - u.z;

	return projected_vector;
}


Quaternion::Quaternion(void)
{
}

Quaternion::Quaternion(const Quaternion &quat)
{
	coords[0] = quat.coords[0];
	coords[1] = quat.coords[1];
	coords[2] = quat.coords[2];
	coords[3] = quat.coords[3];
}

Quaternion::~Quaternion(void)
{
}

Quaternion & Quaternion::operator =(const Quaternion &quat)
{
	coords[0] = quat.coords[0];
	coords[1] = quat.coords[1];
	coords[2] = quat.coords[2];
	coords[3] = quat.coords[3];
	return *this;
}

Quaternion Quaternion::operator *(const Quaternion &quat)
{
	Quaternion out;

	out.coords[0] = (coords[3] * quat.coords[0]) + ( coords[0] * quat.coords[3] ) + ( coords[1] * quat.coords[2] ) - ( coords[2] * quat.coords[1] );
	out.coords[1] = (coords[3] * quat.coords[1]) - ( coords[0] * quat.coords[2] ) + ( coords[1] * quat.coords[3] ) + ( coords[2] * quat.coords[0] );
    out.coords[2] = (coords[3] * quat.coords[2]) + ( coords[0] * quat.coords[1] ) - ( coords[1] * quat.coords[0] ) + ( coords[2] * quat.coords[3] );
    out.coords[3] = (coords[3] * quat.coords[3]) - ( coords[0] * quat.coords[0] ) - ( coords[1] * quat.coords[1] ) - ( coords[2] * quat.coords[2] );

	return out;

}
const Quaternion Quaternion::operator *(const Quaternion &quat) const
{
	Quaternion out;

	out.coords[0] = (coords[3] * quat.coords[0]) + ( coords[0] * quat.coords[3] ) + ( coords[1] * quat.coords[2] ) - ( coords[2] * quat.coords[1] );
	out.coords[1] = (coords[3] * quat.coords[1]) - ( coords[0] * quat.coords[2] ) + ( coords[1] * quat.coords[3] ) + ( coords[2] * quat.coords[0] );
    out.coords[2] = (coords[3] * quat.coords[2]) + ( coords[0] * quat.coords[1] ) - ( coords[1] * quat.coords[0] ) + ( coords[2] * quat.coords[3] );
    out.coords[3] = (coords[3] * quat.coords[3]) - ( coords[0] * quat.coords[0] ) - ( coords[1] * quat.coords[1] ) - ( coords[2] * quat.coords[2] );

	return out;
}


void Quaternion::Conjugate()
{
	coords[0] = -coords[0];
	coords[1] = -coords[1];
	coords[2] = -coords[2];
	coords[3] = coords[3];
}


