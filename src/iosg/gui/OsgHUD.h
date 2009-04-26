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

#ifndef _OSGIHUD_
#define _OSGIHUD_

#include <vector>
#include <map>
#include <osg/Node>
#include <osg/ref_ptr>
#include <osg/MatrixTransform>
#include <wx/wx.h>

#include <cpw/IApplication.h>

#include <iosg/gui/OsgHUDController.h>
#include <iosg/OsgObjectRegistry.h>


#include <iosg/gui/OsgWidget.h>

#include <iosg/Export.h>


namespace cpw 
{ 
	namespace iosg 
	{
		class OsgIWidget;



		/** 
			\brief Creates a HUD, Head Up Display
			\ingroup iosg
			\todo Quitar Application de aqui
		*/
		class IOSGEXPORT OsgIHUD
		{
			public:
				
				OsgIHUD(void);
				~OsgIHUD(void);

				void AddWidget(OsgIWidget *widget);
				void AddMore(OsgIWidget *widget);

				
				void SetRootNode(osg::MatrixTransform * _root) { root = _root; }

				void UpdateSize(const int &_size_x, const int &_size_y);
				void Update();

				void SetActiveWidget(const std::string &widget_id);
				OsgIWidget * GetActiveWidget() { return active_widget; }
				void ReleaseActiveWidget() { active_widget = NULL; }

				void SetApplication(cpw::IApplication *_application) { application = _application; }
				cpw::IApplication * GetApplication() { return application; }
				const cpw::IApplication * GetApplication() const { return application; }

				void SetParentWindow(wxWindow *_parent_window) { parent_window = _parent_window; }
				wxWindow * GetParentWindow() { return parent_window; }
				const wxWindow * GetParentWindow() const { return parent_window; }

				

			private:

				void MarkTree(const std::string &str, osg::Node *node);
				void SearchForActiveWidget(OsgIWidget *widget, const std::string &widget_id);

				int size_x, size_y;
				osg::ref_ptr<osg::MatrixTransform> root;
				std::vector<OsgIWidget *> widgets;
				OsgIWidget *active_widget;
				unsigned int id_generator;
				
				//neccesary to make calls to the application
				cpw::IApplication *application;
				wxWindow *parent_window;
				

				
		};
	}
}

#endif
