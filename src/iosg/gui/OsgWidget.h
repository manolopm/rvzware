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

#ifndef _OSGIWIDGET_
#define _OSGIWIDGET_

#include <osgDB/ReadFile>
#include <osg/Matrix>
#include <osg/Node>
#include <osg/MatrixTransform>
#include <osg/Switch>
#include <wx/wx.h>

#include <cpw/callback/IEventReceiver.h>
#include <cpw/common/Math.h>
#include <cpw/common/types.h>
#include <cpw/IApplication.h>

#include <iosg/OsgVisitorFirstGeode.h>
#include <iosg/Export.h>

namespace cpw{
  namespace iosg
	{	  class OsgNavigator;

	  struct TAnchor
		{
		  float h, w;
		  int x, y;
	  };

	  


		/**
			\brief
			\ingroup iosg
			\todo Intentar quita la referencia a Application desde el iosg
		*/
	  class IOSGEXPORT OsgIWidget : public cpw::IEventReceiver
		{
			public:

				OsgIWidget(const std::string &url, const cpw::Point3d<float> &_position, const cpw::Point3d<float> &_size, const cpw::Point3d<float> &_rotation, const TAnchor &_anchor);
				virtual ~OsgIWidget(void);

				virtual bool MouseLeftDoubleClick(const int &x, const int &y){return false;}
				virtual bool MouseRightDoubleClick(const int &x,const int &y){return false;}
				virtual bool MouseMiddleDoubleClick(const int &x,const int &y){return false;}
				
				virtual bool MouseLeftButtonDown(const int &x,const int &y){return false;}
				virtual bool MouseRightButtonDown(const int &x,const int &y){return false;}
				virtual bool MouseMiddleButtonDown(const int &x,const int &y){return false;}

				virtual bool MouseLeftButtonUp(const int &x,const int &y){return false;}
				virtual bool MouseRightButtonUp(const int &x,const int &y){return false;}
				virtual bool MouseMiddleButtonUp(const int &x,const int &y){return false;}

				virtual bool MouseDrag(const int &x0, const int &y0, const int &x1, const int &y1){return false;}
				virtual bool MouseWheel(const int &wheel_rotation, const int &wheel_delta){return false;}

				virtual bool MouseLeaveWindow(const int &x0, const int &y0){return false;}
				virtual bool MouseEnterWindow(const int &x0, const int &y0){return false;}
						
				virtual bool KeyDown(const int &key){return false;}
				virtual bool KeyUp(const int &key){return false;}

				virtual bool HasFocus(){ return focus; }

				virtual void SetValue(const std::string &value){}
				virtual void SetValue(const int &value){}

				virtual cpw::Point3d<float> & GetPosition()	{ return position; };
				virtual cpw::Point3d<float> & GetSize()		{ return size; }
				virtual cpw::Point3d<float> & GetRotation()	{ return rot; }
				virtual iosg::TAnchor		& GetAnchor()	{ return anchor; }
				virtual float GetRadius()		{ return radius; }
				virtual cpw::Point3d<float> & GetScale()		{ return scale; }
				virtual void SetPosition(const cpw::Point3d<float> &_position);
				virtual void SetSize(const cpw::Point3d<float> &_size);
				virtual void SetRotation(const cpw::Point3d<float> &_rot);
				virtual void SetRadius(const float &_radius) { radius = _radius; }
				virtual void SetScale(const cpw::Point3d<float> &_scale) { scale = _scale; }

				virtual void SetObject(const std::string &filename);
				void SetDefaultPath(const std::string &url) {default_directory = url;};
				std::string & GetDefaultPath() {return default_directory;};
				//virtual osg::ref_ptr<osg::Node> GetObject();
				virtual void SetObject(osg::ref_ptr<osg::Node> object_) {object.swap(object_);}

				virtual void RotateX(const float &angle);
				virtual void RotateY(const float &angle);
				virtual void RotateZ(const float &angle);

				virtual void Move(const int &x = 0, const int &y = 0, const int &z = 0);
				virtual void Rotate(const int &x = 0, const int &y = 0, const int &z = 0);
				virtual void Scale(const int &x = 0, const int &y = 0, const int &z = 0);

				virtual osg::Node * GetCompassNode() { return mt.get(); }
				virtual osg::MatrixTransform * GetMatrixTransform() { return mt.get(); }
				virtual void SetMatrixTransform(const osg::ref_ptr<osg::MatrixTransform> &_mt) {mt =_mt;}
				virtual void SetSwitch(const osg::ref_ptr<osg::Switch> &_sw) {sw =_sw;}

				virtual void ResizeScreen(const int &x, const int &y);
				virtual void Update(osg::Matrix *mp=NULL);

				void SetAnchor(const float &w, const float &h, const int &offset_x, const int &offset_y);

				static unsigned int RequestId() { ++id; return id; }

				std::string & GetId() { return id_string; }
				void SetId(const std::string &new_id) { id_string = new_id; }

				//void SetNavigator(OsgNavigator *_navigator) { navigator = _navigator; }
				//OsgNavigator * GetNavigator() { return navigator; }
				//const OsgNavigator * GetNavigator() const { return navigator; }

				void SetLocalIntersectionPoint(const cpw::Point3d<float> &_local_i_point) { local_i_point = _local_i_point; }
				cpw::Point3d<float> & GetLocalIntersectionPoint() { return local_i_point; }

				void SetName(const std::string _name) { name = _name; }
				std::string & GetName() { return name; }
				const std::string & GetName() const { return name; }

				void SetApplication(cpw::IApplication *_application) { application = _application; }
				cpw::IApplication * GetApplication() { return application; }
				const cpw::IApplication * GetApplication() const { return application; }

				void SetParentWindow(wxWindow *_parent_window) { parent_window = _parent_window; }
				wxWindow * GetParentWindow() { return parent_window; }
				const wxWindow * GetParentWindow() const { return parent_window; }

				virtual void StartPickAnimation(const double &_animation_time) {}

				virtual void SetVisible(const bool &_visible);
				bool & IsVisible() { return visible; }

				//void SetObjRegistry(OsgObjectRegistry *_obj_reg) { obj_reg = _obj_reg;}

				
				

			private:

				std::string default_directory;
				osg::ref_ptr<osg::MatrixTransform> mt;
				osg::ref_ptr<osg::Switch> sw;

				cpw::Point3d<float> position, rot, absolute_position, size, scale;
				float radius;
				bool focus;
				int screen_x_size, screen_y_size;
				TAnchor anchor;
				std::string id_string, name;

				//neccesary to make calls to the application
				cpw::IApplication *application;
				wxWindow *parent_window;

				float x_max, x_min, y_max, y_min; //bounding box surrounding the object

				static unsigned int id;

				osg::ref_ptr<osg::Node> object;
				cpw::Point3d<float> local_i_point;

				bool visible;
				
			//OsgNavigator *navigator;


			protected:

				void MarkTree(const std::string &str, osg::Node *node);
				void UpdateTransformationMatrix(osg::Matrix *mp=NULL);
				
				
				cpw::Point3d<float> & GetAbsolutePosition() { return absolute_position; }
				const cpw::Point3d<float> & GetAbsolutePosition() const { return absolute_position; }

				void GetBoundingBox(float &xmax, float &xmin, float &ymax, float &ymin);
				

				

		};
	}
}

#endif
