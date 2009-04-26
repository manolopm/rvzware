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

#ifndef _OSGCAMERA_
#define _OSGCAMERA_

#include <osg/Node>
#include <osg/Timer>
#include <osg/ref_ptr>
#include <osg/MatrixTransform>
#include <osg/Switch>


#include <cpw/graphic/ICamera.h>
#include <cpw/common/types.h>
#include <cpw/common/Math.h>

#include <iosg/OsgScene.h>
#include <iosg/OsgNavigator.h>

#include <iosg/Export.h>

namespace cpw 
{ 
	namespace iosg 
	{
		class OsgNavigator;


		/** 
			\brief Por comentar...
			\ingroup iosg
		*/
		class IOSGEXPORT OsgCamera : public cpw::ICamera
		{
			public:
				OsgCamera(void);
				~OsgCamera(void);

				virtual void SetPosition(const cpw::Point3d<double> &_position) { position = _position; }
				virtual void SetView(const cpw::Point3d<double> &_view) { view = _view; }
				virtual void SetUp(const cpw::Point3d<double> &_up) { up = _up; }

				virtual cpw::Point3d<double> & GetPosition() { return position; }
				virtual cpw::Point3d<double> & GetView() { return view; }
				virtual cpw::Point3d<double> & GetUp() { return up; }

				virtual cpw::Point3d<double> GetUTMPosition();

				virtual void SetRoof(const double &_roof) { roof = _roof; }
				virtual double GetRoof() { return roof; }

				virtual void SetVerticalRotationDeacceleration(const double &vrd){ vertical_rotation_deacceleration = vrd; }
				virtual void SetHorizontalRotationDeacceleration(const double &hrd){ horizontal_rotation_deacceleration = hrd; };
				virtual void SetViewRotationDeacceleration(const double &vrd){ view_rotation_deacceleration = vrd; };

				virtual void SetVerticalRotationSpeed(const double &vrs) { vertical_rotation_speed = vrs; }
				virtual void SetHorizontalRotationSpeed(const double &hrs) { horizontal_rotation_speed = hrs; }
				virtual void SetViewRotationSpeed(const double &vrs) { view_rotation_speed = vrs; }

				virtual double & GetVerticalRotationDeacceleration() { return vertical_rotation_deacceleration; }
				virtual double & GetHorizontalRotationDeacceleration() { return horizontal_rotation_deacceleration; }
				virtual double & GetViewRotationDeacceleration() {return view_rotation_deacceleration; }

				virtual void SetVerticalRotationMaxSpeed(const double &vrp) { vertical_rotation_max_speed = vrp; }
				virtual void SetHorizontalRotationMaxSpeed(const double &hrp) { horizontal_rotation_max_speed = hrp; }
				virtual void SetViewRotationRotationMaxSpeed(const double &vrp) { view_rotation_max_speed = vrp; }

				virtual void RotateVertical(const double &multiplier=1.0, const bool &inertia=false);
				virtual void RotateHorizontal(const double &multiplier=1.0, const bool &inertia=false);
				virtual void RotateView(const double &multiplier=1.0, const bool &inertia=false);
				virtual void GoForward(const double &multiplier=1.0, const bool &inertia=false, const bool &stop_on_collision=false);
				virtual void GoUp(const double &multiplier=0.0, const bool &inertia=false);
				virtual void Strafe(const double &multiplier=0.0, const bool &inertia=false);
				virtual void PlaneXTranslation(const double &multiplier, const bool &inertia=false);
				virtual void PlaneYTranslation(const double &multiplier, const bool &inertia=false);
				virtual void UpdateXYPositionWithMouseCollision(const unsigned int &x0, const unsigned int &y0, const unsigned int &x1, const unsigned int &y1);
				virtual void StartContinousPlaneXTranslation(const double &multiplier, const bool &inertia=false);
				virtual void StartContinousPlaneYTranslation(const double &multiplier, const bool &inertia=false);
				virtual void StopContinousPlaneXTranslation();
				virtual void StopContinousPlaneYTranslation();
				virtual void Zoom(const cpw::Point3d<double> &point, const double &mult=0);
				
				virtual void SetForwardMaxSpeed(const double &max_speed) { forward_max_speed = max_speed; }
				virtual void SetForwardMinSpeed(const double &min_speed){ forward_min_speed = min_speed;}
				virtual void SetForwardDeacceleration(const double &_deacceleration) { forward_deacceleration = _deacceleration; }
				virtual void SetForwardSpeed(const double &speed) { forward_speed = speed; }
				virtual double & GetForwardMaxSpeed() { return forward_max_speed; }
				virtual double & GetForwardDeacceleration() { return forward_deacceleration; }
				virtual double & GetForwardSpeed() { return forward_speed; }

				virtual void SetStrafeMaxSpeed(const double &max_speed) { strafe_max_speed = max_speed; }
				virtual void SetStrafeMinSpeed(const double &min_speed){ strafe_min_speed = min_speed; }
				virtual void SetStrafeDeacceleration(const double &_deacceleration) { strafe_deacceleration = _deacceleration; }
				virtual void SetStrafeSpeed(const double &speed) { strafe_speed = speed; }
				virtual double & GetStrafeMaxSpeed() { return strafe_max_speed; } 
				virtual double & GetStrafeDeacceleration() { return strafe_deacceleration; }
				virtual double & GetStrafeSpeed() { return strafe_speed; }

				virtual void SetUpMaxSpeed(const double &max_speed) { up_max_speed = max_speed; }
				virtual void SetUpDeacceleration(const double &_deacceleration) { up_deacceleration = _deacceleration; }
				virtual void SetUpSpeed(const double &speed) { up_speed = speed; }
				virtual double & GetUpMaxSpeed() { return up_max_speed; }
				virtual double & GetUpDeacceleration() { return up_deacceleration; }
				virtual double & GetUpSpeed() { return up_speed; }
				
				virtual void SetMaxHeight(const double &_max_height) { max_height = _max_height; }
				virtual void SetMinHeight(const double &_min_height) { min_height = _min_height; }
				virtual void SetMaxHeightMult(const double &max_height_mult) { max_height_multiplier = max_height_mult; }
				virtual void SetMinHeightMult(const double &min_height_mult) { min_height_multiplier = min_height_mult; }

				virtual double & GetMovementConstantMultiplier() { return movement_constant_multiplier;}
				void SetMovementConstantMultiplier(const double &new_multiplier);

				virtual void GainHeight(const double &multiplier=1.0);
				virtual void LooseHeight(const double &multiplier=1.0);

				virtual void GoTo(const cpw::Point3d<double> &place, const double &arrive_time);
				virtual void GoToJustInterpolate(const cpw::Point3d<double> &final_position, const cpw::Point3d<double> &final_view, 
								  const cpw::Point3d<double> &final_up, const double &arrive_time);
				virtual void GoToWithNoUpViewChanges(const cpw::Point3d<double> &_place, const double &arrive_time);
				virtual void ZoomToPoint(const cpw::Point3d<double> &_point_to_zoom, const double &zoom_time, const double &percentage_to_advance, const bool &forward);
				virtual void GoToPanoramic(const cpw::Point3d<double> &_place, const double &arrive_time);
				virtual void Update();

				void StartHorizontalRotationWithInertia(const double &mult = 1.0);
				void StartVerticalRotationWithInertia(const double &mult = 1.0);
				void StartForwardMovementWithInertia(const double &mult = 1.0);
				void StartGainHeightMovementWithInertia(const double &mult = 1.0);
				void StartViewSceneCollisionPointHorizontalRotation(const double &mult = 1.0, const bool &inertia=false);
				void StartViewSceneCollisionPointVerticalRotation(const double &mult = 1.0, const bool &inertia=false);

				void StopHorizontalRotationWithInertia(){ horizontal_continuous_rotation = false; }
				void StopVerticalRotationWithInertia(){ vertical_continuous_rotation = false; }
				void StopForwardMovementWithInertia(){forward_continous_movement = false;}
				void StopGainHeightMovementWithInertia() {gain_height_continous_movement = false;}
				void StopViewSceneCollisionPointHorizontalRotation();
				void StopViewSceneCollisionPointVerticalRotation();

				virtual void SetCorrectUp(const bool &_correct_up) { correct_up = _correct_up; }
				virtual bool & GetCorrectUp() { return correct_up; }

				virtual void NewTranslationPoint(const cpw::Point3d<double> &new_cp){};
				void SetOsgScene(OsgScene *_osg_scene) { osg_scene = _osg_scene; }

				virtual void StartXYTraslation(const int &x, const int &y);
				virtual void StopXYTraslation();
				virtual bool TraslatingXY() { return traslating_xy; }

				void SetDefaultPath(const std::string &url);

				virtual void NewRotationPoint(const cpw::Point3d<double> &_new_cp);
				virtual void UpdateTimer();
				void SetNavigator(OsgNavigator *_navigator) { osg_navigator = _navigator; }

				float GetPitch();

				void SetDisp(const double &_disp) { disp = _disp; }
				const double & GetDisp() { return disp; }
				
			private:
				
				void SlowDown(double &speed, double deacceleration, const double &time);
				void UpdateSpeed(const double &new_speed, double &current_speed);
				void UpdateHeightMultiplier();
				void UpdateGoingTo(double elapsed_time);
				void UpdateZoomToPoint(const double &time);
				void SetAllCurrentSpeedsToCero();
				double GetTerrainHeightUnderCamera();
				void UpdateZoomPivot(const cpw::Point3d<double> &i_point, const bool &zoom_in = true);

				double GetRotationOrientation(const cpw::Plane &p, const cpw::Point3d<double> &axis, const double &angle, const cpw::Point3d<double> &point_to_rotate);
				
				bool UpdatePositionAccordingToCollisions(const cpw::Point3d<double> &start_position, const cpw::Point3d<double> &end_position);
				bool RotationPossible(const cpw::Point3d<double> &start_position, const cpw::Point3d<double> &end_position);
				double GetAproachingFactor(const cpw::Point3d<double> &place_to_go);
				bool MovementCollides(const cpw::Point3d<double> &start_position, const cpw::Point3d<double> &end_position);
				
				cpw::Plane CreatePlaneFromCameraVectors(  );
				void CorrectUp();
				bool GetIntersectionViewWithScene();
				void UpdatePivotSize(const double &time_between_frames);
				void UpdateGoingToNoUAVChanges(const double &time);
				void UpdateGoingToPanoramic(const double &time);
				void UpdateGoingToJustInterpolate(const double &time);

				void UpdateTraslatePivot(const cpw::Point3d<float> &i_point, const bool &update_position);
				
				cpw::Point3d<double> position, view, up;
				cpw::Point3d<float> traslate_pivot_position;
				
				double vertical_rotation_deacceleration, horizontal_rotation_deacceleration, view_rotation_deacceleration, view_collision_horizontal_rotation_deacceleration, view_collision_vertical_rotation_deacceleration, following_line_deacceleration;
				double vertical_rotation_speed, horizontal_rotation_speed, view_rotation_speed, view_collision_horizontal_rotation_speed, view_collision_vertical_rotation_speed, following_line_speed;
				double vertical_rotation_current_speed, horizontal_rotation_current_speed, view_rotation_current_speed, view_collision_horizontal_rotation_current_speed, view_collision_vertical_rotation_current_speed, following_line_current_speed;
				double vertical_rotation_max_speed, horizontal_rotation_max_speed, view_rotation_max_speed, view_collision_horizontal_rotation_max_speed, view_collision_vertical_rotation_max_speed, following_line_max_speed;
				double forward_current_speed, forward_deacceleration, forward_max_speed, forward_speed, forward_min_speed, following_line_min_speed;
				double movement_constant_multiplier, max_movement_constant_multiplier;
				double strafe_current_speed, strafe_deacceleration, strafe_max_speed, strafe_speed, strafe_min_speed;
				double up_current_speed, up_deacceleration, up_max_speed, up_speed;
				double planex_current_speed, planex_deacceleration, planex_max_speed, planex_speed;
				double planey_current_speed, planey_deacceleration, planey_max_speed, planey_speed;
				double current_height_multiplier, max_height_multiplier, min_height_multiplier, max_height, min_height;
				double height_speed, current_height_speed, height_max_speed, height_deacceleration;

				double disp;
				
				//going to info
				bool going_to, rotating;
				cpw::Point3d<double> place_to_go, place_to_look, go_to_view_vec, place_to_start;
				double current_increment_mult, acceleration_time, deacceleration_time, current_going_to_time;
				bool rotation_orientation, rotation_orientationv;
				double time_to_arrive, rotating_time;
				double tlast_vrotation_update, tlast_hrotation_update;
				osg::Timer_t last_t, actual_t;
				double time_between_frames;
				bool correct_up, forward_inertia, up_inertia, rotate_vertical_inertia, rotate_horizontal_inertia, rotate_view_inertia, strafe_inertia, gain_height_inertia, planex_inertia, planey_inertia, view_collision_horizontal_inertia, view_collision_vertical_inertia, following_line_inertia;

				//continuous movement info
				bool view_collision_horizontal_continous_rotation, view_collision_vertical_continous_rotation, horizontal_continuous_rotation, vertical_continuous_rotation, forward_continous_movement, gain_height_continous_movement;
				bool plane_x_continous_movement, plane_y_continous_movement;
				double horizontal_rotation_continous_mult, vertical_rotation_continous_mult, forward_continous_movement_mult, gain_height_continous_mult;
				double view_collision_horizontal_rotation_continous_mult, view_collision_vertical_rotation_continous_mult;

				//XY PLANE
				
				cpw::Plane xy_plane;

				//intersection point to rotate around
				cpw::Point3d<double> view_intersection_point;
				bool ip_valid, rotation_interrupted;

				OsgScene *osg_scene;
				osg::ref_ptr<osg::MatrixTransform> mtn;
				osg::ref_ptr<osg::Switch> sn;
				bool osg_init;
				double rotation_icon_size_factor;

				//going to with no up and view changes
				bool going_to_no_uav_changes;
				
				double distance_to_advance, distance_to_v, distance_to_h;
				cpw::Plane planev, planeh, planew;
				cpw::Point3d<double> initial_position, initial_up, initial_view;

				//panoramic go to
				bool going_to_panoramic, going_up, need_intermediate_position;
				double panoramic_goto_final_height, panoramic_max_height, middle_time;
				cpw::Point3d<double> panoramic_final_position, panoramic_final_view, panoramic_final_up, position_increment, view_increment, up_increment, middle_position;
				bool forward_stop_on_collision;

				//just interpolate going to
				cpw::Point3d<double> final_up_ji, final_view_ji, final_position_ji, initial_up_ji, initial_view_ji, initial_position_ji;
				bool going_to_just_interpolate;

				//zoom to point
				bool zoom_forward, zooming_to_point, osg_zooming_init;
				cpw::Point3d<double> zoom_final_position;
				double distance_percentage;
				bool zoom_to_point_initial_acceleration;
				osg::ref_ptr<osg::MatrixTransform> mt_zm;
				osg::ref_ptr<osg::Switch> snz;


				//line to follow
				cpw::Point3d<double> line_to_follow;


				//collisions with plane
				OsgNavigator *osg_navigator;
				cpw::Point3d<double> traslation_last_collision_point;
				bool traslating_xy, osg_traslate_init;
				osg::ref_ptr<osg::MatrixTransform> mt_tm;
				osg::ref_ptr<osg::Switch> snt;

				double roof;
				cpw::Range<double> x_limits, y_limits;

				std::string default_directory;
		};
	}
}

#endif
