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

#ifndef _UIAPPLICATIONMAINFRAME_
#define _UIAPPLICATIONMAINFRAME_

#include "Application.h"
#include "ApplicationTime.h"

#include <string>
#include <wx/wx.h>
#include <wx/splitter.h>
#include <wx/notebook.h>
#include <wx/menuitem.h>
#include <wx/accel.h>
#include <wx/glcanvas.h>
#include <wx/string.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/colour.h>
#include <wx/region.h>
#include <wx/panel.h>
#include <wx/slider.h>
#include <wx/gauge.h>
#include <wx/aui/aui.h>

#include <gui/UILayerTreePanel.h>
#include <gui/UIConnectionTreePanel.h>
#include <gui/UISplitterCanvas.h>
#include <gui/GUItypes.h>
#include <gui/UIVideoController.h>
#include <gui/UISunConfiguration.h>
#include <gui/UIEntityPropertiesGrid.h>
#include <gui/UILogWindow.h>
#include <gui/UISceneTree.h>
#include <gui/UIMovieScheme.h>
#include <gui/UIProgressionModalDialog.h>
#include <gui/UIHelp.h>
#include <gui/UILoadSplash.h>

#include <cpw/ApplicationConfiguration.h>

#include <cpw/graphic/INavigatorManager.h>
#include <cpw/entity/LayerTree.h>

#include <remote/RemoteProtocol.h>

#define	FILE_NEW 10
#define	FILE_OPEN 11
#define	FILE_CLOSE 12
#define	FILE_SAVE 13 
#define	FILE_SAVE_ALL 14
#define	FILE_EXIT 15

#define	EDIT_CUT 20
#define	EDIT_COPY 21
#define	EDIT_PASTE 22
#define	EDIT_DELETE 23
#define	EDIT_REMOVE 24
#define	EDIT_SELECT_ALL 25
#define	EDIT_FIND 26
#define	EDIT_MODIFY_PROPERTIES 27
#define	EDIT_DUPLICATE 28
	
#define ADD_NAVIGATOR 1000
#define VIEW_WIREFRAME 1001
#define VIEW_SOLID 1002


#define NEW_ELEMPRIMITIVE 1003
#define NEW_ELEMENT 1004
#define NEW_LAYERPRIMITIVE 1005
#define NEW_LAYER 1006
#define ADD_ENTITY 1007
#define ADD_DATABASE 1008
#define ADD_NEW 1109
#define ADD_NEW_LAYER_PRIMITIVE 1110
#define NEW_CONTAINER_LAYER_PRIMITIVE 1009
#define NEW_VECTORIAL_LAYER_PRIMITIVE 1010
#define NEW_GRAPHIC_LAYER_PRIMITIVE 1011
#define NEW_FIRE 1012
#define NEW_WMSLAYER 1013
#define NEW_REMOTE_ENTITY 1017
#define NEW_TABLE 1016
#define NEW_FIRE_PRIMITIVE 1014
#define NEW_FIREWALL 1018
#define ADD_TEMPLATE 1019
#define ADD_IMPORT 1020
#define ADD_REMOTE_ENTITY 1021


#define VIEW_SELECTVIEWMODE 1015

#define VISUALIZE 2000
#define VIEW_HTML 2001
#define ANIMATE 2002
#define PUBLISH 2003
#define SENDBACK 2004
#define SENDFRONT 2005

#define GOTO 2006
#define MOVETO 2007

#define PROPERTIES 3000

#define HELP 4000

#define APPLICATION_DATE 5008
#define APPLICATION_HOUR 5009
#define APPLICATION_DATE 5008
#define	APPLICATION_HOUR 5009
#define	ANIMATION_MULTIPLIER 5010
#define	ANIMATION_SLIDER 5011
#define	ANIMATION_VIEW_ANIMABLE 5012
#define ANIMATION_PLAY 5000
#define ANIMATION_STOP 5001
#define ANIMATION_FORWARD 5002
#define ANIMATION_REWIND 5003
#define ANIMATION_START 5004
#define ANIMATION_END 5005
#define ANIMATION_PAUSE 5006
#define ANIMATION_REPEAT 5007

#define ISLAND_LAPALMA 6001
#define ISLAND_ELHIERRO 6002
#define ISLAND_LAGOMERA 6003
#define ISLAND_TENERIFE 6004
#define ISLAND_GRANCANARIA 6005
#define ISLAND_FUERTEVENTURA 6006
#define ISLAND_LANZAROTE 6007

#define VIEW_LAYERTREE 7000
#define VIEW_PROPERTIES 7001
#define VIEW_FILE 7002
#define VIEW_EDIT 7003
#define VIEW_VIEW 7004
#define VIEW_ANIMATION_CONTROLS 7005
#define VIEW_ANIMATION_SCHEME 7012
#define VIEW_APPLICATION_STATUS 7013
#define VIEW_APPLICATION_STATUS_SCHEME 7014
#define VIEW_ISLANDS 7006
#define VIEW_HELP 7007
#define VIEW_SHOWALL 7008
#define VIEW_HIDEALL 7009
#define VIEW_FULLSCREEN 7010
#define VIEW_NAVIGATION_MODE 7015
#define VIEW_SUN_CONFIG 7016
#define VIEW_CONNECTIONTREE 7017
#define VIEW_PROJECT 7018
#define VIEW_ELEMENT 7019
#define VIEW_TOOLS 7020
#define VIEW_DEFAULT_CONFIG 7021
#define VIEW_LOG 7022

#define PLUGINS_MENUS_ID 8000

#define THREE_DIMENSION_NAVIGATION_MODE 9000
#define TWO_DIMENSION_NAVIGATION_MODE 9001

#define STATUS 9000

#define ALPHA_SLIDER 1001

#define ELEMENT_TRANSFORM 10001
#define ELEMENT_PROPERTIES 10002
#define TRANSFORM_TRANSLATION 11002
#define TRANSFORM_ROTATION 11003
#define TRANSFORM_SCALE 11004

#define TOOLS_CALCULATE 12001
#define CALCULATE_DISTANCE 12002

#define SHOW_ADDITIONAL_WINDOWS 1



class Application;

namespace cpw 
{ 
	
	namespace gui
	{
		class UILayerTreePanel;
		class UIConnectionTreePanel;
		class UISunConfiguration;
		class UILogWindow;

		/** 
			\brief Main frame of the Application
			\ingroup cwcontent
		*/
		
		class UIApplicationMainFrame : public wxFrame
		{
			public:

				wxAuiManager m_mgr;

				UIApplicationMainFrame(Application *app, const wxChar *titulo, int xpos, int ypos, int ancho, int alto, long style = wxDEFAULT_FRAME_STYLE | wxFULL_REPAINT_ON_RESIZE);
				~UIApplicationMainFrame(void);

				void InitGUIContents(cpw::LayerTree &layer_tree, cpw::remote::RemoteProtocol *protocol,
					cpw::INavigatorManager *navigator_manager = NULL);	
				void InitShortCuts();
				void Update();
				void RePaint() {wxSizeEvent q = wxSizeEvent(); OnSize( q);}
				void ShowSplash(){load_splash->Center(); load_splash->Show(true);load_splash->Refresh();load_splash->Update();};
				void HideSplash(){load_splash->Show(false); load_splash->Destroy();};

				void EnableGUI(const bool &value);
				void UpdateAnimableUI();

				void OnFileOpen(wxCommandEvent& WXUNUSED(event));
				void OnFileClose(wxCommandEvent& WXUNUSED(event));
				void EventoQuit(wxCommandEvent& WXUNUSED(event));
				void OnAppOpen(wxCommandEvent& WXUNUSED(event));
				void OnAppClose(wxCommandEvent& WXUNUSED(event));
				void OnClose(wxCloseEvent& event);
				void OnSize(wxSizeEvent& event);
				void OnIdle(wxIdleEvent& event);
				void OnInternalIdle();
				void OnNoteBookPageChanged(wxNotebookEvent& WXUNUSED(event));
				void OnSliderChange(wxScrollEvent& WXUNUSED(event));
				void OnSave(wxCommandEvent& WXUNUSED(event));
				void OnSaveAll(wxCommandEvent& WXUNUSED(event));

				void EditCut(wxCommandEvent& WXUNUSED(event));
				void EditCopy(wxCommandEvent& WXUNUSED(event));
				void EditPaste(wxCommandEvent& WXUNUSED(event));
				void EditDelete(wxCommandEvent& WXUNUSED(event));
				void EditProperties(wxCommandEvent& WXUNUSED(event));

				int  GetWMSALphaValue();
				void SetWMSAlphaValue(const int &value);

				void AnimateEntity(wxCommandEvent& WXUNUSED(event));
				void NotAvailable(wxCommandEvent& WXUNUSED(event));
				void AddNavigator(wxCommandEvent& WXUNUSED(event));
				void DrawModeWireframe(wxCommandEvent& WXUNUSED(event));
				void DrawModeSolid(wxCommandEvent& WXUNUSED(event));
				//void ViewSelection(wxCommandEvent& WXUNUSED(event));
				void ViewFile(wxCommandEvent& WXUNUSED(event));
				void ViewEdit(wxCommandEvent& WXUNUSED(event));
				void ViewView(wxCommandEvent& WXUNUSED(event));
				void ViewAnimationControls(wxCommandEvent& WXUNUSED(event));
				void ViewSunConfig(wxCommandEvent& WXUNUSED(event));
				void ViewConnectionTree(wxCommandEvent& WXUNUSED(event));
				void ViewAnimationScheme(wxCommandEvent& WXUNUSED(event));
				void ViewIslands(wxCommandEvent& WXUNUSED(event));
				void ViewHelp(wxCommandEvent& WXUNUSED(event));
				void ViewLog(wxCommandEvent& WXUNUSED(event));
				void ViewLayerTree(wxCommandEvent& WXUNUSED(event));
				void ViewProperties(wxCommandEvent& WXUNUSED(event));
				void ViewShowAll(wxCommandEvent& WXUNUSED(event));
				void ViewHideAll(wxCommandEvent& WXUNUSED(event));
				void ViewFullScreen(wxCommandEvent& WXUNUSED(event));
				void ViewApplicationClock(wxCommandEvent& WXUNUSED(event));
				void ViewApplicationStatus(wxCommandEvent& WXUNUSED(event));
				void ViewProject(wxCommandEvent& WXUNUSED(event));
				void ViewElement(wxCommandEvent& WXUNUSED(event));
				void ViewTools(wxCommandEvent& WXUNUSED(event));
				void NewElementPrimitive(wxCommandEvent& WXUNUSED(event));
				void NewLayerPrimitive(wxCommandEvent& WXUNUSED(event));
				void NewElement(wxCommandEvent& WXUNUSED(event));
				void NewLayer(wxCommandEvent& WXUNUSED(event));
				void NewTable(wxCommandEvent& WXUNUSED(event));
				void NewFire(wxCommandEvent& WXUNUSED(event));
				void NewFirewall(wxCommandEvent& WXUNUSED(event));
				void NewWmsLayer(wxCommandEvent& WXUNUSED(event));
				void NewRemoteEntity(wxCommandEvent& WXUNUSED(event));
				void NewRasterLayerPrimitive(wxCommandEvent& WXUNUSED(event));
				void NewVectorialLayerPrimitive(wxCommandEvent& WXUNUSED(event));
				void NewContainerLayerPrimitive(wxCommandEvent& WXUNUSED(event));
				void ShowHelp(wxCommandEvent& WXUNUSED(event));
				void AddEntity(wxCommandEvent& WXUNUSED(event));
				void AddRemoteEntity(wxCommandEvent& WXUNUSED(event));
				void AddDataBase(wxCommandEvent& WXUNUSED(event));
				int AddPluginMenu(const std::string &new_plugin);
				void OnPlugin(wxCommandEvent& event);
				void TwoDimesionNavigationMode(wxCommandEvent& WXUNUSED(event));
				void ThreeDimesionNavigationMode(wxCommandEvent& WXUNUSED(event));
				void Translate(wxCommandEvent& WXUNUSED(event));
				void Rotate(wxCommandEvent& WXUNUSED(event));
				void Scale(wxCommandEvent& WXUNUSED(event));
				void CheckVisibility();
				void SetDefaultConfig(wxCommandEvent& WXUNUSED(event));

				void OnPaneClose(wxAuiManagerEvent& event);
				void OnPaneMove(wxAuiManagerEvent& event);

				void OnKeyUp(wxKeyEvent &event);
				void OnKeyDown(wxKeyEvent &event);
				void OnMouse(wxMouseEvent &event);
				void OnLeftDown(wxMouseEvent &event);
				void OnLeftUp(wxMouseEvent &event);
				void OnLeftDoubleClick(wxMouseEvent &event);
				void TogglePaneVisibility(int win_id, const wxString& pane_name);

				static wxMenuItem *GetMenuItem(const int id, wxMenu *parent, const wxString& helpString = wxString(), const wxString& bitmapUrl = wxString(), wxMenu *subMenu = (wxMenu*)0);

				void SetViewportManager(cpw::gui::UISplitterCanvas* viewport_manager_) {viewport_manager =  viewport_manager_;};
				void SetViewportField(wxSplitterWindow *viewport_field_) {viewport_field =  viewport_field_;};

				void Set4ViewsA(), Set4ViewsB(), Set4ViewsC(), Set4ViewsD(), Set4ViewsE();
				void Set3ViewsA(), Set3ViewsB(), Set3ViewsC(), Set3ViewsD();
				void Set2ViewsH(), Set2ViewsV();
				void SetOnlyView();

				void SendFront(const cpw::TypeId &id);
				void SendBack(const cpw::TypeId &id);

				void ModifyProperties(const cpw::TypeId &id);
				void GoTo(const cpw::TypeId &id);

				void Copy(const cpw::TypeId &id);  // not a real copy, its a link
				void Duplicate(const cpw::TypeId &id);
				void Cut(const cpw::TypeId &id);
				void Paste(const cpw::TypeId &id_orig, const cpw::TypeId &id_dest, const cpw::TypeId &parent_id_orig, const cpw::TypeId &parent_id_dest, const bool &cut, const bool &copy, const bool &duplicate);

				void Delete(const cpw::TypeId &ent_id, const cpw::TypeId &parent_id);

				void Visualize(const cpw::TypeId &id, const bool &value);
				void Publish(const cpw::TypeId &id);
				void Animate(const cpw::TypeId &id);

				void SetSunPosition(const float &pos);
				void SetFireViewMode(const int &value);
				int  GetFireViewMode();

				void AddLine();
				void CalculateDistance(wxCommandEvent& WXUNUSED(event));

				std::string GetUIConfiguration();
				//void DeleteLayerTreePanel();				

				Application * GetApplication() { return application; }

				void UpdateAUIManager();

				void StopTimeAndControls();

			private:

				Application *application;
				wxString ui_default_config;

				cpw::gui::UISplitterCanvas *viewport_manager;
				wxSplitterWindow *splitter, *sub_splitter, *viewport_field;
				wxMiniFrame *layer_frame, *properties_frame, *log_frame,
				  *animation_scheme_frame, *scene_tree_frame,
				  *connection_frame;
				UIEntityPropertiesGrid *ui_entity_properties_grid;
				UILogWindow *ui_log_window;
				UISceneTree *ui_scene_tree;
				wxMenu *view_menu;
				wxMenu *file_menu;
				wxMenu *edit_menu;
				wxMenu *navigation_mode_menu;
				wxMenu *project_menu;
				wxMenu *element_menu;
				wxMenu *tools_menu;
				wxMenu *project_submenu_add_template;
				wxMenu *project_submenu_add_remote;
				wxMenu *project_submenu_add_new;
				wxMenu *element_submenu_transform;
				wxMenu *tools_submenu_calculate;
				UIMovieScheme *ui_animation_scheme_window;
				UIHelp *help_window;
				UILoadSplash *load_splash;
				bool busy_status;
				wxStaticText *statusLabel;
				wxGauge *statusGauge;
				wxSlider *alphaSlider;
				cpw::INavigatorManager *navigator_manager_;
				int fireViewMode;
							
				wxNotebook *note_book;
				cpw::gui::UILayerTreePanel *ui_layer_tree;
				wxAuiPaneInfo pane_info;

				cpw::gui::UIConnectionTreePanel *ui_connection_tree;

				//toolbars
				wxToolBar *tool_bar_file;
				wxToolBar *tool_bar_edit;
				wxToolBar *tool_bar_vis;
				UIVideoController *tool_bar_sim;
				UISunConfiguration *tool_bar_sun;
				wxToolBar *tool_bar_islands;
				wxToolBar *tool_bar_help;
				wxToolBar *tool_bar_status;
				wxToolBar *tool_bar_element;
				wxToolBar *tool_bar_project, *tool_bar_tools;
				wxToolBar *ui_layer_tree_tb;
				UIProgressionModalDialog *pm_dialog;

				//plugins
				wxMenu *plugins_menu;
				int plugins_ids;

				static std::map<int, std::string> menus;

				void RegisterMenu(const int id, const std::string &title);
				void InitMenus();
				void InitGUI();
				
				DECLARE_EVENT_TABLE()
		};

	}
}

#endif
