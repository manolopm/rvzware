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
#ifndef _ICAMERA_
#define _ICAMERA_

#include <cpw/Export.h>
#include <cpw/common/types.h>

namespace cpw
{

	/** 
		\brief Por comentar ....
		\ingroup cpw
	*/
	class ICamera
	{
		public:
			virtual ~ICamera(void){};

			virtual void SetPosition(const Point3d<double> &_position)=0;
			virtual void SetView(const Point3d<double> &_view)=0;
			virtual void SetUp(const Point3d<double> &_up)=0;

			virtual Point3d<double> & GetPosition()=0;
			virtual Point3d<double> & GetView()=0;
			virtual Point3d<double> & GetUp()=0;

			virtual Point3d<double> GetUTMPosition()=0;

			virtual void SetRoof(const double &_roof) {}
			virtual double GetRoof() { return 0.0; }

			virtual void SetVerticalRotationDeacceleration(const double &vrd)=0;
			virtual void SetHorizontalRotationDeacceleration(const double &hrd)=0;
			virtual void SetViewRotationDeacceleration(const double &vrd)=0;

			virtual double & GetVerticalRotationDeacceleration()=0;
			virtual double & GetHorizontalRotationDeacceleration()=0;
			virtual double & GetViewRotationDeacceleration()=0;

			virtual void SetVerticalRotationSpeed(const double &vrs)=0;
			virtual void SetHorizontalRotationSpeed(const double &hrs)=0;
			virtual void SetViewRotationSpeed(const double &vrs)=0;
			
			virtual void SetVerticalRotationMaxSpeed(const double &vrp)=0;
			virtual void SetHorizontalRotationMaxSpeed(const double &hrp)=0;
			virtual void SetViewRotationRotationMaxSpeed(const double &vrp)=0;

			virtual void RotateVertical(const double &multiplier=0.0, const bool &inertia=false)=0;
			virtual void RotateHorizontal(const double &multiplier=0.0, const bool &inertia=false)=0;
			virtual void RotateView(const double &multiplier=0.0, const bool &inertia=false)=0;
			virtual void GoForward(const double &multiplier=0.0, const bool &inertia=false, const bool &stop_on_collision=false)=0;
			virtual void GoUp(const double &multiplier=0.0, const bool &inertia=false)=0;
			virtual void Strafe(const double &multiplier=0.0, const bool &inertia=false)=0;
			virtual void GoTo(const cpw::Point3d<double> &place, const double &arrive_time)=0;
			virtual void GoToJustInterpolate(const cpw::Point3d<double> &final_position, const cpw::Point3d<double> &final_view, 
				              const cpw::Point3d<double> &final_up, const double &arrive_time)=0;
			virtual void GoToWithNoUpViewChanges(const cpw::Point3d<double> &place, const double &arrive_time)=0;
			virtual void GoToPanoramic(const cpw::Point3d<double> &place, const double &arrive_time)=0;
			virtual void GainHeight(const double &multiplier=0.0)=0;
			virtual void LooseHeight(const double &multiplier=0.0)=0;
			virtual void PlaneXTranslation(const double &multiplier, const bool &inertia=false)=0;
			virtual void PlaneYTranslation(const double &multiplier, const bool &inertia=false)=0;
			virtual void UpdateXYPositionWithMouseCollision(const unsigned int &x0, const unsigned int &y0, const unsigned int &x1, const unsigned int &y1)=0;
			virtual void StartContinousPlaneXTranslation(const double &multiplier, const bool &inertia=false)=0;
			virtual void StartContinousPlaneYTranslation(const double &multiplier, const bool &inertia=false)=0;
			virtual void StopContinousPlaneXTranslation()=0;
			virtual void StopContinousPlaneYTranslation()=0;
			virtual void StartViewSceneCollisionPointHorizontalRotation(const double &mult = 1.0, const bool &inertia=false)=0;
			virtual void StartViewSceneCollisionPointVerticalRotation(const double &mult = 1.0, const bool &inertia=false)=0;
			virtual void ZoomToPoint(const cpw::Point3d<double> &point_to_zoom, const double &zoom_time, const double &percentage_to_advance, const bool &forward)=0;
			
			virtual void StartHorizontalRotationWithInertia(const double &mult = 1.0)=0;
			virtual void StartVerticalRotationWithInertia(const double &mult = 1.0)=0;
			virtual void StartForwardMovementWithInertia(const double &mult = 1.0)=0;
			virtual void StartGainHeightMovementWithInertia(const double &mult = 1.0)=0;

			virtual void StopHorizontalRotationWithInertia()=0;
			virtual void StopVerticalRotationWithInertia()=0;
			virtual void StopForwardMovementWithInertia()=0;
			virtual void StopGainHeightMovementWithInertia()=0;
			virtual void StopViewSceneCollisionPointHorizontalRotation()=0;
			virtual void StopViewSceneCollisionPointVerticalRotation()=0;

			virtual void SetForwardMaxSpeed(const double &max_speed) = 0;
			virtual void SetForwardMinSpeed(const double &min_speed) = 0;
			virtual void SetForwardDeacceleration(const double &_deacceleration) = 0;
			virtual void SetForwardSpeed(const double &speed) = 0;
			virtual double & GetForwardMaxSpeed() = 0; 
			virtual double & GetForwardDeacceleration() = 0;
			virtual double & GetForwardSpeed() = 0;

			virtual void SetStrafeMaxSpeed(const double &max_speed) = 0;
			virtual void SetStrafeMinSpeed(const double &min_speed) = 0;
			virtual void SetStrafeDeacceleration(const double &_deacceleration) = 0;
			virtual void SetStrafeSpeed(const double &speed) = 0;
			virtual double & GetStrafeMaxSpeed() = 0; 
			virtual double & GetStrafeDeacceleration() = 0;
			virtual double & GetStrafeSpeed() = 0;

			virtual void SetUpMaxSpeed(const double &max_speed) = 0;
			virtual void SetUpDeacceleration(const double &_deacceleration) = 0;
			virtual void SetUpSpeed(const double &speed) = 0;
			virtual double & GetUpMaxSpeed() = 0; 
			virtual double & GetUpDeacceleration() = 0;
			virtual double & GetUpSpeed() = 0;

			virtual void Zoom(const cpw::Point3d<double> &point, const double &mult=0) {}

			//height multipliers
			virtual void SetMaxHeight(const double &_max_height) = 0;
			virtual void SetMinHeight(const double &_min_height) = 0;
			virtual void SetMaxHeightMult(const double &max_height_mult) = 0;
			virtual void SetMinHeightMult(const double &min_height_mult) = 0;

			virtual void SetCorrectUp(const bool &_correct_up) = 0;
			virtual bool & GetCorrectUp() = 0;

			virtual void NewRotationPoint(const cpw::Point3d<double> &new_cp)=0;
			//virtual void NewTranslationPoint(const cpw::Point3d<double> &new_cp)=0;

			virtual void StartXYTraslation(const int &x, const int &y)=0;
			virtual void StopXYTraslation()=0;
			virtual bool TraslatingXY()=0;


			virtual void UpdateTimer()=0;

			
			
			virtual void Update()=0;

	};
}

#endif