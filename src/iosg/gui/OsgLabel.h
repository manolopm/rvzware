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

#ifndef _OSGILABEL_
#define _OSGILABEL_

#include <iosg/gui/OsgWidget.h>

#include <osgText/FadeText>

#include <iosg/Export.h>


namespace cpw 
{ 
	namespace iosg 
	{

		/** 
			\brief Label type widget
			\ingroup iosg
		*/
		class IOSGEXPORT OsgILabel : public OsgIWidget
		{
                        private:
                                OsgIWidget * widget;
			public:	

				OsgILabel(const std::string &url, cpw::Point3d<float> _position, cpw::Point3d<float> _size, cpw::Point3d<float> _rotation, const TAnchor &_anchor, const std::string &label_ = "");
				~OsgILabel(void);

				virtual void SetValue(const std::string &value){SetLabel(value);}

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

				virtual bool HasFocus(){ return false; }

				void SetLabel(const std::string &label_ = "");
				virtual void SetObject(const std::string &filename);

				virtual void Update(osg::Matrix *mp);

			private:

				std::string label;
				osg::ref_ptr<osgText::Text> debug_text;
				osg::ref_ptr<osg::Geode> object;
				cpw::Point3d<float> rotation;
				osg::ref_ptr<osg::Switch> sw;

				float x_max, x_min, y_max, y_min;
				

		};
	}
}

#endif
