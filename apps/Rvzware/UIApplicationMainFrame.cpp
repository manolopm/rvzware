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

#include "stdafx.h"

#include "UIApplicationMainFrame.h"
#include <cpw/common/ApplicationLog.h>
#include <gui/UIStatusBar.h>
#include <remote/ConnectionManager.h>

using namespace cpw::gui;


#define NOTEBOOK_ID 1011

#define FIRST_PANE 2200 //used as first pane
#define PANE_FILE 2200
#define PANE_EDIT 2201
#define PANE_VIEW 2202
#define PANE_ANIMATION_CONTROLS 2203
#define PANE_HELP 2204
#define PANE_LAYERTREE 2205
#define PANE_PROPERTIES 2206
#define PANE_STATUS 2207
#define PANE_ANIMATION_SCHEME 2208 
#define PANE_SUN_CONFIG 2209 
#define PANE_ELEMENT 2210
#define PANE_PROJECT 2211
#define PANE_TOOLS 2212
#define PANE_CONNECTIONTREE 2213
#define PANE_LOG 2214
#define LAST_PANE 2214 //used as last pane

//Put debug panes here
//#define PANE_LOG 2301

#define NAME_TOOL_BAR_VIS		"00000001"
#define NAME_TOOL_BAR_TOOLS		"00000002"
#define NAME_TOOL_BAR_STATUS	"00000003"
#define NAME_TOOL_BAR_SIM		"00000004"
#define NAME_TOOL_BAR_SUN		"00000005"
#define NAME_TOOL_BAR_HELP		"00000006"
#define NAME_LAYER_FRAME		"00000007"
#define NAME_PROPERTIES_FRAME	"00000008"
#define NAME_SPLITTER			"00000009"
#define NAME_LOG_FRAME			"00000010"
#define NAME_SCENE_TREE_FRAME	"00000011"
#define NAME_ANIMATION_SCHEME_FRAME "00000012"
#define NAME_CONNECTION_FRAME	"00000013"
#define NAME_TOOL_BAR_FILE		"00000014"
#define NAME_TOOL_BAR_EDIT		"00000015"
#define NAME_TOOL_BAR_PROJECT	"00000016"
#define NAME_TOOL_BAR_ELEMENT	"00000017"

BEGIN_EVENT_TABLE(UIApplicationMainFrame, wxFrame)
EVT_MENU	 ( FILE_NEW, UIApplicationMainFrame::NotAvailable ) 
EVT_MENU	 ( FILE_OPEN, UIApplicationMainFrame::OnFileOpen ) 	
EVT_MENU	 ( FILE_CLOSE, UIApplicationMainFrame::OnFileClose )
EVT_MENU	 ( FILE_SAVE, UIApplicationMainFrame::OnSave ) 
//EVT_MENU	 ( FILE_SAVE_ALL, UIApplicationMainFrame::OnSaveAll ) 
EVT_MENU	 ( FILE_EXIT, UIApplicationMainFrame::OnAppClose ) 

EVT_MENU	 ( EDIT_CUT, UIApplicationMainFrame::EditCut ) 
EVT_MENU	 ( EDIT_COPY, UIApplicationMainFrame::EditCopy ) 
EVT_MENU	 ( EDIT_PASTE, UIApplicationMainFrame::EditPaste ) 
EVT_MENU	 ( EDIT_DELETE, UIApplicationMainFrame::EditDelete ) 
EVT_MENU	 ( EDIT_REMOVE, UIApplicationMainFrame::NotAvailable ) 
EVT_MENU	 ( EDIT_SELECT_ALL, UIApplicationMainFrame::NotAvailable ) 
EVT_MENU	 ( EDIT_FIND, UIApplicationMainFrame::NotAvailable ) 
	
EVT_MENU	 ( VIEW_WIREFRAME, UIApplicationMainFrame::DrawModeWireframe )
EVT_MENU	 ( VIEW_SOLID, UIApplicationMainFrame::DrawModeSolid )
//EVT_MENU	 ( VIEW_SELECTVIEWMODE, UIApplicationMainFrame::ViewSelection )
EVT_MENU	 ( VIEW_FILE, UIApplicationMainFrame::ViewFile )
EVT_MENU	 ( VIEW_EDIT, UIApplicationMainFrame::ViewEdit )
EVT_MENU	 ( VIEW_VIEW, UIApplicationMainFrame::ViewView )
EVT_MENU	 ( VIEW_ANIMATION_CONTROLS, UIApplicationMainFrame::ViewAnimationControls )
EVT_MENU	 ( VIEW_SUN_CONFIG, UIApplicationMainFrame::ViewSunConfig )
EVT_MENU	 ( VIEW_CONNECTIONTREE, UIApplicationMainFrame::ViewConnectionTree )
EVT_MENU	 ( VIEW_ANIMATION_SCHEME, UIApplicationMainFrame::ViewAnimationScheme )
EVT_MENU	 ( VIEW_APPLICATION_STATUS, UIApplicationMainFrame::ViewApplicationStatus )
EVT_MENU	 ( VIEW_LOG, UIApplicationMainFrame::ViewLog )
EVT_MENU	 ( VIEW_HELP, UIApplicationMainFrame::ViewHelp )
EVT_MENU	 ( VIEW_LAYERTREE, UIApplicationMainFrame::ViewLayerTree )
EVT_MENU	 ( VIEW_PROPERTIES, UIApplicationMainFrame::ViewProperties )
EVT_MENU	 ( VIEW_SHOWALL, UIApplicationMainFrame::ViewShowAll )
EVT_MENU	 ( VIEW_HIDEALL, UIApplicationMainFrame::ViewHideAll )
EVT_MENU	 ( VIEW_FULLSCREEN, UIApplicationMainFrame::ViewFullScreen )
EVT_MENU	 ( VIEW_PROJECT, UIApplicationMainFrame::ViewProject )
EVT_MENU	 ( VIEW_ELEMENT, UIApplicationMainFrame::ViewElement )
EVT_MENU	 ( VIEW_TOOLS, UIApplicationMainFrame::ViewTools )
EVT_MENU	 ( VIEW_DEFAULT_CONFIG, UIApplicationMainFrame::SetDefaultConfig )
	

EVT_MENU	 ( ADD_NAVIGATOR, UIApplicationMainFrame::AddNavigator	)
EVT_MENU	 ( ADD_DATABASE, UIApplicationMainFrame::AddDataBase	)

EVT_MENU	 ( VISUALIZE, UIApplicationMainFrame::NotAvailable ) 
EVT_MENU	 ( ANIMATE  , UIApplicationMainFrame::AnimateEntity ) 
EVT_MENU	 ( VIEW_HTML, UIApplicationMainFrame::NotAvailable ) 

EVT_MENU	 ( ADD_ENTITY, UIApplicationMainFrame::AddEntity )
EVT_MENU	 ( ADD_REMOTE_ENTITY, UIApplicationMainFrame::AddRemoteEntity )
EVT_MENU	 ( NEW_ELEMPRIMITIVE, UIApplicationMainFrame::NewElementPrimitive )
EVT_MENU	 ( NEW_ELEMENT, UIApplicationMainFrame::NewElement )
EVT_MENU	 ( NEW_LAYER, UIApplicationMainFrame::NewLayer )
EVT_MENU	 ( NEW_FIRE, UIApplicationMainFrame::NewFire )
EVT_MENU	 ( NEW_FIREWALL, UIApplicationMainFrame::NewFirewall )
EVT_MENU	 ( NEW_TABLE, UIApplicationMainFrame::NewTable )
EVT_MENU	 ( NEW_WMSLAYER, UIApplicationMainFrame::NewWmsLayer )
EVT_MENU	 ( NEW_REMOTE_ENTITY, UIApplicationMainFrame::NewRemoteEntity )
EVT_MENU	 ( NEW_GRAPHIC_LAYER_PRIMITIVE, UIApplicationMainFrame::NewRasterLayerPrimitive )
EVT_MENU	 ( NEW_VECTORIAL_LAYER_PRIMITIVE, UIApplicationMainFrame::NewVectorialLayerPrimitive )
EVT_MENU	 ( NEW_CONTAINER_LAYER_PRIMITIVE, UIApplicationMainFrame::NewContainerLayerPrimitive )

EVT_MENU	 ( PROPERTIES, UIApplicationMainFrame::NotAvailable ) 
EVT_MENU	 ( TRANSFORM_TRANSLATION, UIApplicationMainFrame::Translate ) 
EVT_MENU	 ( TRANSFORM_ROTATION,    UIApplicationMainFrame::Rotate ) 
EVT_MENU	 ( TRANSFORM_SCALE,       UIApplicationMainFrame::Scale ) 
EVT_MENU	 ( ELEMENT_PROPERTIES,    UIApplicationMainFrame::EditProperties ) 
EVT_MENU	 ( CALCULATE_DISTANCE,    UIApplicationMainFrame::CalculateDistance ) 
EVT_MENU	 ( HELP, UIApplicationMainFrame::ShowHelp ) 
EVT_MENU	 ( wxID_EXIT, UIApplicationMainFrame::EventoQuit )
EVT_MENU	 ( TWO_DIMENSION_NAVIGATION_MODE, UIApplicationMainFrame::TwoDimesionNavigationMode )
EVT_MENU	 ( THREE_DIMENSION_NAVIGATION_MODE, UIApplicationMainFrame::ThreeDimesionNavigationMode )

EVT_CLOSE	 ( UIApplicationMainFrame::OnClose   )
EVT_IDLE	 ( UIApplicationMainFrame::OnIdle    )
EVT_SIZE     ( UIApplicationMainFrame::OnSize    )
EVT_KEY_UP   ( UIApplicationMainFrame::OnKeyUp   )
EVT_KEY_DOWN ( UIApplicationMainFrame::OnKeyDown )
EVT_MOUSE_EVENTS ( UIApplicationMainFrame::OnMouse )
EVT_NOTEBOOK_PAGE_CHANGED ( NOTEBOOK_ID, UIApplicationMainFrame::OnNoteBookPageChanged )

EVT_AUI_PANE_CLOSE(UIApplicationMainFrame::OnPaneClose) 
EVT_COMMAND_SCROLL (ALPHA_SLIDER, UIApplicationMainFrame::OnSliderChange)

END_EVENT_TABLE()


UIApplicationMainFrame::UIApplicationMainFrame(Application *app, const wxChar *titulo, int xpos, int ypos, int ancho, int alto, long style)
: application(app), wxFrame(NULL, wxID_ANY, titulo, wxPoint(xpos, ypos), wxSize(ancho, alto), style), splitter(), plugins_ids(PLUGINS_MENUS_ID), fireViewMode(0)
				
{
  wxWindow::SetThemeEnabled(true);
  m_mgr.SetManagedWindow(this);


  if (!(cpw::ApplicationConfiguration::GetInstance()->IsThemed()))
    {
      wxAuiDockArt* auiArtProvider = m_mgr.GetArtProvider();
      wxColour backgroundColor = wxString(cpw::ApplicationConfiguration::GetInstance()->
					  GetBackgroundColour().c_str(),
					  wxConvUTF8);
      wxColour inactiveColor   = wxString(cpw::ApplicationConfiguration::GetInstance()->
					  GetTitleBarGradient1Colour().c_str(),
					  wxConvUTF8);
      wxColour gradientColor2  = wxString(cpw::ApplicationConfiguration::GetInstance()->
					  GetTitleBarGradient2Colour().c_str(),
					  wxConvUTF8);
      wxColour fontColor       = wxString(cpw::ApplicationConfiguration::GetInstance()->
					  GetFontLightColour().c_str(),
					  wxConvUTF8);
		
      SetBackgroundColour(backgroundColor);
      SetForegroundColour(fontColor);

      auiArtProvider->SetColour(5,backgroundColor); //do SetBackgroundColor() in children frames/toolbars, etc.
      auiArtProvider->SetColour(14,backgroundColor); //gripper
      auiArtProvider->SetColour(6,backgroundColor);	//sash
      auiArtProvider->SetColour(13,backgroundColor);	//border
      auiArtProvider->SetColour(7,backgroundColor);	//active caption
      auiArtProvider->SetColour(8,gradientColor2);	//active caption gradient
      auiArtProvider->SetColour(9,inactiveColor);		//inactive caption
      auiArtProvider->SetColour(10,gradientColor2);	//inactive caption gradient
      auiArtProvider->SetColour(11,fontColor);	//active caption text
      auiArtProvider->SetColour(12,fontColor);	//inactive caption text
      //auiArtProvider->SetColour(15,inactiveColor);	//caption font
      auiArtProvider->SetMetric(16,1);				//gradient type
    }

	  

  load_splash = new UILoadSplash(this, wxID_ANY,
				 wxString(((std::string)("Rvzware")).c_str(),
					  wxConvUTF8),
				 wxString(((std::string)
					   ("resources/application_splash.png"))
					  .c_str(),wxConvUTF8));
 
  
  //  load_splash->Show(true);

  InitMenus();

  InitGUI();

  m_mgr.Update();
  //this->Show(true);  
  //load_splash->Show(false);
  ////pasar a application
}

void UIApplicationMainFrame::InitShortCuts()
{
  wxAcceleratorEntry entries[16];
  entries[0].Set(wxACCEL_CTRL,  (int) 'N',     FILE_NEW);
  entries[1].Set(wxACCEL_CTRL,  (int) 'O',     FILE_OPEN);
  entries[2].Set(wxACCEL_CTRL | wxACCEL_SHIFT,  (int) 'S',     FILE_SAVE);
  //entries[3].Set(wxACCEL_CTRL | wxACCEL_SHIFT,  (int) 'A',     FILE_SAVE_ALL);
  entries[4].Set(wxACCEL_CTRL,  (int) 'X',     EDIT_CUT);
  entries[5].Set(wxACCEL_CTRL,  (int) 'C',     EDIT_COPY);
  entries[6].Set(wxACCEL_CTRL,  (int) 'V',     EDIT_PASTE);
  entries[7].Set(wxACCEL_CTRL | wxACCEL_SHIFT,  (int) 'A',     EDIT_SELECT_ALL);
  entries[8].Set(wxACCEL_CTRL,  (int) 'F',     EDIT_FIND);
  entries[9].Set(wxACCEL_NORMAL, WXK_F9, VIEW_SHOWALL);
  entries[10].Set(wxACCEL_NORMAL, WXK_F10, VIEW_HIDEALL);
  entries[11].Set(wxACCEL_NORMAL, WXK_F11, VIEW_FULLSCREEN);
  entries[12].Set(wxACCEL_CTRL,  (int) 'T',     TRANSFORM_TRANSLATION);
  entries[13].Set(wxACCEL_CTRL,  (int) 'R',     TRANSFORM_ROTATION);
  entries[14].Set(wxACCEL_CTRL,  (int) 'S',     TRANSFORM_SCALE);
  entries[15].Set(wxACCEL_CTRL,  (int) 'P',     ELEMENT_PROPERTIES);
  wxAcceleratorTable accel(16, entries);
  this->SetAcceleratorTable(accel);
  layer_frame->SetAcceleratorTable(accel);
  properties_frame->SetAcceleratorTable(accel);
  animation_scheme_frame->SetAcceleratorTable(accel);
  connection_frame->SetAcceleratorTable(accel);
}

std::map<int, std::string> UIApplicationMainFrame::menus = std::map<int, std::string>();

void UIApplicationMainFrame::InitMenus()
{
  //RegisterMenu(FILE_NEW, "New\tCtrl+N");
  RegisterMenu(FILE_OPEN, "Open Project\tCtrl+O");
  RegisterMenu(FILE_CLOSE, "Close Project");
  RegisterMenu(FILE_SAVE, "Save\tCtrl+Shift+S");
  //RegisterMenu(FILE_SAVE_ALL, "Save Project\tCtrl+Shift+A");
  RegisterMenu(FILE_EXIT, "Exit");

  RegisterMenu(EDIT_CUT, "Cut\tCtrl+X");
  RegisterMenu(EDIT_COPY, "Copy\tCtrl+C");
  RegisterMenu(EDIT_PASTE, "Paste\tCtrl+V");
  RegisterMenu(EDIT_DELETE, "Delete");
  //RegisterMenu(EDIT_REMOVE, "Remove");
  //RegisterMenu(EDIT_SELECT_ALL, "Select All\tCtrl+A");
  //RegisterMenu(EDIT_FIND, "Find\tCtrl+F");
  RegisterMenu(EDIT_MODIFY_PROPERTIES, "Edit Properties");
	
  RegisterMenu(PROPERTIES, "Properties");

  //RegisterMenu(VIEW_SELECTVIEWMODE, "Select View Mode");
  RegisterMenu(VIEW_WIREFRAME, "Wireframe");
  RegisterMenu(VIEW_SOLID, "Solid");

  RegisterMenu(VIEW_FILE, "File");
  RegisterMenu(VIEW_EDIT, "Edit");
  RegisterMenu(VIEW_VIEW, "View");
  RegisterMenu(VIEW_ANIMATION_CONTROLS, "Animation Controls");
  RegisterMenu(VIEW_SUN_CONFIG, "Sun Configuration");
  RegisterMenu(VIEW_ANIMATION_SCHEME, "Animation Entity Scheme");
  RegisterMenu(VIEW_APPLICATION_STATUS, "Application Status");
  RegisterMenu(VIEW_HELP, "Help");
  RegisterMenu(VIEW_LAYERTREE, "Layer Tree");
  RegisterMenu(VIEW_CONNECTIONTREE, "Connection Tree");
  RegisterMenu(VIEW_PROPERTIES, "Properties");
  RegisterMenu(VIEW_SHOWALL, "Show All\tF9");
  RegisterMenu(VIEW_HIDEALL, "Hide All\tF10");
  RegisterMenu(VIEW_FULLSCREEN, "Full Screen\tF11");
  RegisterMenu(VIEW_DEFAULT_CONFIG, "Default Configuration");
  //RegisterMenu(STATUS_MENU_ID, "Status");
  RegisterMenu(VIEW_NAVIGATION_MODE, "Change Navigation Mode");	
  RegisterMenu(VIEW_LOG, "Log");
  RegisterMenu(THREE_DIMENSION_NAVIGATION_MODE, "3D");
  RegisterMenu(TWO_DIMENSION_NAVIGATION_MODE, "2D");
	
  RegisterMenu(NEW_CONTAINER_LAYER_PRIMITIVE, "Container Layer");
  RegisterMenu(NEW_ELEMPRIMITIVE, "Element");
  RegisterMenu(NEW_LAYER, "Container Layer");
  RegisterMenu(NEW_ELEMENT, "Element");
  RegisterMenu(NEW_TABLE, "Table");
  RegisterMenu(NEW_FIRE, "Fire");
  RegisterMenu(NEW_FIREWALL, "Firewall");
  RegisterMenu(NEW_WMSLAYER, "Wms Layer");
  RegisterMenu(NEW_REMOTE_ENTITY, "Remote Entity");

  RegisterMenu(ADD_TEMPLATE, "Create Template");	
  RegisterMenu(ADD_NEW, "Add New");	
  RegisterMenu(ADD_NEW_LAYER_PRIMITIVE, "Layer Primitive");
  RegisterMenu(ADD_IMPORT, "Import");
  RegisterMenu(ADD_ENTITY, "Import"); //from disk
  RegisterMenu(ADD_REMOTE_ENTITY, "Remote");

  RegisterMenu(ELEMENT_TRANSFORM, "Transform");
  RegisterMenu(TRANSFORM_TRANSLATION, "Translation\tCtrl+T");
  RegisterMenu(TRANSFORM_ROTATION, "Rotation\tCtrl+R");
  RegisterMenu(TRANSFORM_SCALE, "Scale\tCtrl+S");
  RegisterMenu(ELEMENT_PROPERTIES, "Edit Properties\tCtrl+P");
	
  RegisterMenu(TOOLS_CALCULATE, "Calculate");
  RegisterMenu(CALCULATE_DISTANCE, "Distance");

  RegisterMenu(VISUALIZE, "Visualize");
  RegisterMenu(ANIMATE, "Animate");
  RegisterMenu(GOTO, "GoTo");
  RegisterMenu(MOVETO, "MoveTo");
  RegisterMenu(VIEW_HTML, "View Html");
  RegisterMenu(HELP, "Help");

}

void UIApplicationMainFrame::OnPaneMove(wxAuiManagerEvent& event)
{
  Refresh();
}
void UIApplicationMainFrame::OnPaneClose(wxAuiManagerEvent& event)
{
  view_menu->UpdateUI();

  std::string icon_path = ApplicationConfiguration::GetInstance()->GetUIIconDirectory();
  int id = event.pane->window->GetId();
  wxString nombre = FindWindow(id)->GetName();
  int pane_id = view_menu->FindItem(nombre);
  wxMenuItem *menu_item;
  menu_item = view_menu->FindItem(pane_id);

  menu_item->Check(false);
}

void UIApplicationMainFrame::RegisterMenu(const int id, const std::string &title)
{
  menus[id] = title.c_str();
}

wxMenuItem *UIApplicationMainFrame::GetMenuItem(const int id, wxMenu *parent, const wxString& helpString, const wxString& bitmapUrl, wxMenu* subMenu)
{

  std::map<int, std::string>::iterator i = menus.find(id);

  if(i != menus.end())
    {

      wxMenuItem *menu_item = new wxMenuItem(parent, i->first,
					     wxString( i->second.c_str(),wxConvUTF8),
					     helpString, wxITEM_NORMAL, subMenu);
      if (bitmapUrl!=wxT(""))
	{
	  menu_item->SetBitmap(wxBitmap(wxImage(bitmapUrl)));
	}
      return menu_item;
    }
  else 
    return NULL;
	  
}

void UIApplicationMainFrame::Update() 
{
  wxSizeEvent tmp = wxSizeEvent();
  OnSize(tmp); 
  wxFrame::Update(); 
  ui_layer_tree_tb->Update();
  ui_entity_properties_grid->Update(); 
  ui_layer_tree->Update();
  ui_animation_scheme_window->Update();
  connection_frame->Update();
}

void UIApplicationMainFrame::InitGUI () {

  cpw::ApplicationConfiguration *app_config = cpw::ApplicationConfiguration::GetInstance();
  std::string icon_path = app_config->GetUIIconDirectory();
  
  int ic_size = cpw::ApplicationConfiguration::GetInstance()->GetIconSize();
  int bic_size = cpw::ApplicationConfiguration::GetInstance()->GetBigIconSize();


  this->SetIcon(wxString((icon_path + "geviemer3.ico").c_str(),wxConvUTF8));
  this->SetMinSize(wxSize(800,600));
	
  wxMenuBar *menu_bar = new wxMenuBar();


  //file menu
  file_menu = new wxMenu;
  //file_menu->Append(GetMenuItem(FILE_NEW, file_menu,_T("Ctrl+N"), _T(icon_path + "page_white_add.png")));
	  
  file_menu->Append(GetMenuItem(FILE_OPEN, file_menu,
				wxString(((std::string)("Ctrl+O")).c_str(),wxConvUTF8),
				wxString((icon_path + "folder.png").c_str(),wxConvUTF8)));

  file_menu->Append(GetMenuItem(FILE_CLOSE, file_menu,
				wxT(""),
				wxString((icon_path + "close_doc.png").c_str(),wxConvUTF8)));

  //file_menu->Append(GetMenuItem(FILE_CLOSE, file_menu,_T(""), _T(icon_path + "folder_delete.png")));
  //file_menu->AppendSeparator();

  file_menu->Append(GetMenuItem(FILE_SAVE, file_menu,wxT("Ctrl+S"),
				wxString((icon_path+"disk.png").c_str(),wxConvUTF8)));
  //file_menu->Append(GetMenuItem(FILE_SAVE_ALL, file_menu,_T("Ctrl+Mayús+S"), _T(icon_path + "disk_multiple.png")));
  file_menu->AppendSeparator();
  file_menu->Append(GetMenuItem(FILE_EXIT, file_menu,wxT(""),
				wxString((icon_path+"cross.png").c_str(),wxConvUTF8)));

  menu_bar->Append(file_menu, wxT("File"));

  //edit menu
  edit_menu = new wxMenu;
  edit_menu->Append(GetMenuItem(EDIT_CUT, edit_menu,wxT("Ctrl+X"),
				wxString((icon_path+"cut.png").c_str(),wxConvUTF8)));
  edit_menu->Append(GetMenuItem(EDIT_COPY, edit_menu,wxT("Ctrl+C"),
				wxString((icon_path+"page_white_copy.png").c_str(),wxConvUTF8)));
  edit_menu->Append(GetMenuItem(EDIT_PASTE, edit_menu,wxT("Ctrl+V"),
				wxString((icon_path+"page_white_paste.png").c_str(),wxConvUTF8)));

  edit_menu->Append(GetMenuItem(EDIT_DELETE, edit_menu,wxT(""),
				wxString((icon_path+"page_white_delete.png").c_str(),wxConvUTF8)));
  //edit_menu->AppendSeparator();
  //edit_menu->Append(GetMenuItem(EDIT_SELECT_ALL, edit_menu,_T(""), _T(icon_path + "page_white_stack.png")));
  //edit_menu->Append(GetMenuItem(EDIT_FIND, edit_menu,_T("Ctrl+F"), _T(icon_path + "page_white_magnify.png")));

  menu_bar->Append(edit_menu, wxT("Edit"));

  //navigation mode submenu
  navigation_mode_menu = new wxMenu;

  navigation_mode_menu->AppendRadioItem(THREE_DIMENSION_NAVIGATION_MODE,
					wxT("3D"),
					wxT("Sets the navigation mode to 3d"));
  navigation_mode_menu->AppendRadioItem(TWO_DIMENSION_NAVIGATION_MODE,
					wxT("2D"),
					wxT("Sets the navigation mode to 2d"));

  //view menu
  view_menu = new wxMenu;
  view_menu->Append(GetMenuItem(VIEW_NAVIGATION_MODE, view_menu,
				wxT(""),
				wxString((icon_path + "navigate.png").c_str(),wxConvUTF8),
				navigation_mode_menu));
  
  //view_menu->Append(GetMenuItem(VIEW_SELECTVIEWMODE, view_menu,_T(""), _T(icon_path + "image.png")));
view_menu->AppendSeparator();

view_menu->Append(GetMenuItem(VIEW_WIREFRAME, view_menu,
			      wxT(""),
			      wxString((icon_path + "chart_line.png").c_str(),wxConvUTF8)));

  view_menu->Append(GetMenuItem(VIEW_SOLID, view_menu,
				wxT(""),
				wxString((icon_path + "chart_bar.png").c_str(),wxConvUTF8)));

  view_menu->AppendSeparator();
  view_menu->AppendCheckItem(VIEW_FILE, wxT("File"))->Check(); 
  view_menu->AppendCheckItem(VIEW_EDIT, wxT("Edit"))->Check();
  view_menu->AppendCheckItem(VIEW_VIEW, wxT("View"))->Check();
  view_menu->AppendCheckItem(VIEW_PROJECT, wxT("Project"))->Check();
  view_menu->AppendCheckItem(VIEW_ELEMENT, wxT("Element"))->Check();
  view_menu->AppendCheckItem(VIEW_TOOLS, wxT("Tools"))->Check();
  view_menu->AppendCheckItem(VIEW_SUN_CONFIG, wxT("Sun Configuration"))->Check();
  view_menu->AppendCheckItem(VIEW_LAYERTREE, wxT("Layer Tree"))->Check();
	
  if (SHOW_ADDITIONAL_WINDOWS)
    view_menu->AppendCheckItem(VIEW_CONNECTIONTREE, wxT("Connection Tree"))->Check();
  else
    view_menu->AppendCheckItem(VIEW_CONNECTIONTREE, wxT("Connection Tree"))->Check(false);
	
  view_menu->AppendCheckItem(VIEW_PROPERTIES, wxT("Properties"))->Check();
  view_menu->AppendCheckItem(VIEW_ANIMATION_CONTROLS, wxT("Animation Controls"))->Check();
  view_menu->AppendCheckItem(VIEW_ANIMATION_SCHEME, wxT("Animation Entity Scheme"))->Check();
  view_menu->AppendCheckItem(VIEW_APPLICATION_STATUS, wxT("Application Status"))->Check();

  if (SHOW_ADDITIONAL_WINDOWS)
    view_menu->AppendCheckItem(VIEW_LOG, wxT("Log"))->Check();
  else
    view_menu->AppendCheckItem(VIEW_LOG, wxT("Log"))->Check(false);

  view_menu->AppendCheckItem(VIEW_HELP, wxT("Help"))->Check();

  view_menu->Append(GetMenuItem(VIEW_SHOWALL, view_menu,
				wxT(""),
				wxString((icon_path + "eye.png").c_str(),wxConvUTF8)));


  view_menu->Append(GetMenuItem(VIEW_HIDEALL, view_menu,
				wxT(""),
				wxString((icon_path + "eye_crossed.png").c_str(),wxConvUTF8)));

  view_menu->AppendSeparator();

  view_menu->Append(GetMenuItem(VIEW_FULLSCREEN, view_menu,
				wxT("F11"),
				wxString((icon_path + "arrow_out.png").c_str(),wxConvUTF8)));

  view_menu->AppendSeparator();

  view_menu->Append(GetMenuItem(VIEW_DEFAULT_CONFIG, view_menu,
				wxT(""),
				wxString((icon_path + "default_ui_config.png").c_str(),wxConvUTF8)));

  menu_bar->Append(view_menu, wxT("View"));
  view_menu->UpdateUI();

  //Project menu
  project_menu = new wxMenu;
  project_submenu_add_template = new wxMenu(); 
  //project_submenu_add_remote = new wxMenu(); 
  project_submenu_add_new = new wxMenu();

  //project_submenu_add_remote->Append(GetMenuItem(ADD_ENTITY, project_submenu_add_template,_T("Add remote entity"), _T(icon_path + "import_from_disk.png")));
  //project_submenu_add_remote->Append(GetMenuItem(ADD_REMOTE_ENTITY, project_submenu_add_template,_T("Add remote entity"), _T(icon_path + "import_remote.png")));


  project_submenu_add_template->Append(GetMenuItem(NEW_CONTAINER_LAYER_PRIMITIVE, project_submenu_add_template,
						   wxT("Adds anew container layer primitive"),
						   wxString((icon_path + "folder_brick_template.png").c_str(),wxConvUTF8)));


  project_submenu_add_template->Append(GetMenuItem(NEW_ELEMPRIMITIVE, project_submenu_add_template,
						   wxT("Adds a new element primitive"),
						   wxString((icon_path + "brick_p.png").c_str(),wxConvUTF8)));

  project_submenu_add_new->Append(GetMenuItem(NEW_LAYER, project_submenu_add_new,
					      wxT("Adds a new layer"),
					      wxString((icon_path + "folder_brick.png").c_str(),wxConvUTF8)));

  project_submenu_add_new->AppendSeparator();

  project_submenu_add_new->Append(GetMenuItem(NEW_ELEMENT, project_submenu_add_new,
					      wxT("Adds a new element"),
					      wxString((icon_path + "brick.png").c_str(),wxConvUTF8)));

  project_submenu_add_new->Append(GetMenuItem(NEW_WMSLAYER, project_submenu_add_new,
					      wxT("Adds a new WMS Layer"),
					      wxString((icon_path + "wms.png").c_str(),wxConvUTF8)));


  project_submenu_add_new->Append(GetMenuItem(NEW_REMOTE_ENTITY, project_submenu_add_new,
					      wxT("Adds a new Remote Entity"),
					      wxString((icon_path + "entity_remote.png").c_str(),wxConvUTF8)));

  project_submenu_add_new->Append(GetMenuItem(NEW_FIRE, project_submenu_add_new,
					      wxT("Adds a new Fire"),
					      wxString((icon_path + "flame.png").c_str(),wxConvUTF8)));

  project_submenu_add_new->Append(GetMenuItem(NEW_FIREWALL, project_submenu_add_new,
					      wxT("Adds a new Firewall"),
					      wxString((icon_path + "firewall.png").c_str(),wxConvUTF8)));



  project_menu->Append(GetMenuItem(ADD_ENTITY, project_menu,
				   wxT(""),
				   wxString((icon_path + "import.png").c_str(),wxConvUTF8)));
  //project_menu->Append(GetMenuItem(ADD_IMPORT, project_menu,_T(""), _T(icon_path + "import.png"),project_submenu_add_remote));

  project_menu->Append(GetMenuItem(ADD_TEMPLATE, project_menu,
				   wxT(""),
				   wxString((icon_path + "template.png").c_str(),wxConvUTF8),
				   project_submenu_add_template));

  project_menu->Append(GetMenuItem(ADD_NEW, project_menu,
				   wxT(""),
				   wxString((icon_path + "add_new.png").c_str(),wxConvUTF8),
				   project_submenu_add_new));

  menu_bar->Append(project_menu, wxT("Project"));

  //Element menu	
  element_menu = new wxMenu;
  element_submenu_transform = new wxMenu();

  element_submenu_transform->Append(GetMenuItem(TRANSFORM_TRANSLATION, element_submenu_transform,
						wxT("Translate element"),
						wxString((icon_path + "translation.png").c_str(),wxConvUTF8)));

  element_submenu_transform->Append(GetMenuItem(TRANSFORM_ROTATION, element_submenu_transform,
						wxT("Rotate element"),
						wxString((icon_path + "rotation.png").c_str(),wxConvUTF8)));


  element_submenu_transform->Append(GetMenuItem(TRANSFORM_SCALE, element_submenu_transform,
						wxT("Scale element"),
						wxString((icon_path + "scale.png").c_str(),wxConvUTF8)));


  element_menu->Append(GetMenuItem(ELEMENT_TRANSFORM, project_menu,
				   wxT("Transform element"),
				   wxString((icon_path + "transform.png").c_str(),wxConvUTF8),
				   element_submenu_transform));

  element_menu->Append(GetMenuItem(ELEMENT_PROPERTIES, project_menu,
				   wxT("Edit properties"),
				   wxString((icon_path + "properties.png").c_str(),wxConvUTF8)));

  menu_bar->Append(element_menu, wxT("Element"));

  //Tools menu
  tools_menu = new wxMenu;
  tools_submenu_calculate = new wxMenu(); 
  tools_submenu_calculate->Append(GetMenuItem(CALCULATE_DISTANCE, tools_submenu_calculate,
					      wxT("Measure"),
					      wxString((icon_path + "distance.png").c_str(),wxConvUTF8)));

  tools_menu->Append(GetMenuItem(TOOLS_CALCULATE, project_menu,
				 wxT(""),
				 wxString((icon_path + "calculator.png").c_str(),wxConvUTF8),
				 tools_submenu_calculate));
  
  menu_bar->Append(tools_menu, wxT("Tools"));

  //plugins menu
  plugins_menu = new wxMenu;
  menu_bar->Append(plugins_menu, wxT("Plugins"));

  //Help menu
  wxMenu *help_menu = new wxMenu;

  help_menu->Append(GetMenuItem(HELP, help_menu,
				wxT(""),
				wxString((icon_path + "help.png").c_str(),wxConvUTF8)));
  
  menu_bar->Append(help_menu, wxT("Help"));

  //////ToolBars

  tool_bar_file = new wxToolBar(this, PANE_FILE,
				wxDefaultPosition, wxDefaultSize,
				wxTB_HORIZONTAL|wxTB_FLAT|wxTB_NODIVIDER,
				wxT("File"));

  tool_bar_edit = new wxToolBar(this, PANE_EDIT,
				wxDefaultPosition, wxDefaultSize,
				wxTB_HORIZONTAL|wxTB_FLAT|wxTB_NODIVIDER,
				wxT("Edit"));

  tool_bar_vis  = new wxToolBar(this, PANE_VIEW,
				wxDefaultPosition, wxDefaultSize,
				wxTB_HORIZONTAL|wxTB_FLAT|wxTB_NODIVIDER,
				wxT("View"));

  tool_bar_element = new wxToolBar(this, PANE_ELEMENT,
				   wxDefaultPosition, wxDefaultSize,
				   wxTB_HORIZONTAL|wxTB_FLAT|wxTB_NODIVIDER,
				   wxT("Element"));

  tool_bar_help    = new wxToolBar(this, PANE_HELP,
				   wxDefaultPosition, wxDefaultSize,
				   wxTB_HORIZONTAL|wxTB_FLAT|wxTB_NODIVIDER,
				   wxT("Help"));

  tool_bar_status  = new UIStatusBar(application, this, PANE_STATUS,
				     wxDefaultPosition, wxDefaultSize,
				     wxTB_HORIZONTAL|wxTB_FLAT|wxTB_NODIVIDER,
				     wxT("Application Status"));

  tool_bar_sim = new UIVideoController(&m_mgr,
				       this,
				       PANE_ANIMATION_CONTROLS,
				       wxDefaultPosition, wxDefaultSize,
				       wxTB_HORIZONTAL|wxTB_FLAT|wxTB_NODIVIDER,
				       wxT("Animation Controls"));

  tool_bar_sun = new UISunConfiguration(&m_mgr, this, PANE_SUN_CONFIG,
					wxDefaultPosition, wxDefaultSize,
					wxTB_HORIZONTAL|wxTB_FLAT|wxTB_NODIVIDER,
					wxT("Sun Configuration"));

  tool_bar_project = new wxToolBar(this, PANE_PROJECT, wxDefaultPosition,
				   wxDefaultSize,
				   wxTB_HORIZONTAL|wxTB_FLAT|wxTB_NODIVIDER,
				   wxT("Project"));

  tool_bar_tools = new wxToolBar(this, PANE_TOOLS, wxDefaultPosition,
				 wxDefaultSize,
				 wxTB_HORIZONTAL|wxTB_FLAT|wxTB_NODIVIDER,
				 wxT("Tools"));

  tool_bar_file->SetToolBitmapSize(wxSize(bic_size,bic_size));
  tool_bar_edit->SetToolBitmapSize(wxSize(bic_size,bic_size));
  tool_bar_project->SetToolBitmapSize(wxSize(bic_size,bic_size));
  tool_bar_vis->SetToolBitmapSize(wxSize(bic_size,bic_size));
  tool_bar_element->SetToolBitmapSize(wxSize(bic_size,bic_size));
  tool_bar_tools->SetToolBitmapSize(wxSize(bic_size,bic_size));
  tool_bar_help->SetToolBitmapSize(wxSize(bic_size,bic_size));
  tool_bar_status->SetToolBitmapSize(wxSize(bic_size,bic_size));
  tool_bar_sun->SetToolBitmapSize(wxSize(bic_size,bic_size));
  ((cpw::Subject*)application->GetStatusController())->Attach((cpw::Observer*)(cpw::gui::UIStatusBar*)tool_bar_status); 
  pm_dialog = new UIProgressionModalDialog(application->GetStatusController(), this);
  pm_dialog->Show(false);
  ((cpw::Subject*)application->GetStatusController())->Attach(pm_dialog); 

  //toolbars backgroundcolour
  wxAuiDockArt* auiArtProvider = m_mgr.GetArtProvider();
  wxColour backgroundColor= auiArtProvider->GetColour(5);
  tool_bar_file->SetBackgroundColour(backgroundColor);
  tool_bar_edit->SetBackgroundColour(backgroundColor);
  tool_bar_project->SetBackgroundColour(backgroundColor);
  tool_bar_vis->SetBackgroundColour(backgroundColor);
  tool_bar_element->SetBackgroundColour(backgroundColor);
  tool_bar_tools->SetBackgroundColour(backgroundColor);
  tool_bar_help->SetBackgroundColour(backgroundColor);
  tool_bar_status->SetBackgroundColour(backgroundColor);
  tool_bar_sim->SetBackgroundColour(backgroundColor);
  tool_bar_sun->SetBackgroundColour(backgroundColor);


  //tool_bar_file
  //tool_bar->AddSeparator();
  //tool_bar_file->AddTool(FILE_NEW, _T("New File"), wxBitmap(wxImage(wxT(icon_path + "page_white_add.png"))), _T("New File"));
  tool_bar_file->AddTool(FILE_OPEN, wxT("Open"),
			 wxString((icon_path + "folder.png").c_str(),wxConvUTF8),
			 wxT("Open"));

  tool_bar_file->AddTool(FILE_SAVE,wxT("Save"),
			 wxString((icon_path + "disk.png").c_str(),wxConvUTF8),
			 wxT("Save"));
  //tool_bar_file->AddTool(FILE_SAVE_ALL, _T("Save All"), wxBitmap(wxImage(wxT(icon_path + "disk_multiple.png"))), _T("Save All"));

  //tool_bar_edit

  tool_bar_edit->AddTool(EDIT_CUT, wxT("Cut"),
			 wxString((icon_path + "cut.png").c_str(),wxConvUTF8),
			 wxT("Cur"));

  tool_bar_edit->AddTool(EDIT_COPY, wxT("Copy"),
			 wxString((icon_path + "page_white_copy.png").c_str(),wxConvUTF8),
			 wxT("Copy"));

  tool_bar_edit->AddTool(EDIT_PASTE, wxT("Paste"),
			 wxString((icon_path + "page_white_paste.png").c_str(),wxConvUTF8),
			 wxT("Paste"));

  tool_bar_edit->AddTool(EDIT_DELETE, wxT("Delete"),
			 wxString((icon_path + "page_white_delete.png").c_str(),wxConvUTF8),
			 wxT("Delete"));
  //tool_bar_edit->AddTool(EDIT_FIND, _T("Find"),  wxBitmap(wxImage(wxT(icon_path + "page_white_magnify.png"))), _T("Find"));

  //tool_bar_project

  tool_bar_project->AddTool(NEW_LAYER, wxT("New Layer"),
			    wxString((icon_path + "folder_brick.png").c_str(),wxConvUTF8),
			    wxT("Add container layer"));

  tool_bar_project->AddTool(NEW_ELEMENT, wxT("New Element"),
			    wxString((icon_path + "brick.png").c_str(),wxConvUTF8),
			    wxT("Add element"));

  tool_bar_project->AddTool(NEW_WMSLAYER,wxT("New WMS Layer"),
			    wxString((icon_path + "wms.png").c_str(),wxConvUTF8),
			    wxT("Add WMS Layer"));

  tool_bar_project->AddTool(NEW_REMOTE_ENTITY, wxT("New Remote Entity"),
			    wxString((icon_path + "entity_remote.png").c_str(),wxConvUTF8),
			    wxT("Add remote entity"));

  tool_bar_project->AddTool(NEW_FIRE, wxT("New fire"),
			    wxString((icon_path + "flame.png").c_str(),wxConvUTF8),
			    wxT("Add fire"));

  tool_bar_project->AddTool(NEW_FIREWALL, wxT("New Firewall"),
			    wxString((icon_path + "firewall.png").c_str(),wxConvUTF8),
			    wxT("Add Firewall"));


  //tool_bar_vis
  //tool_bar_vis->AddTool(VIEW_SELECTVIEWMODE, _T("Select View Mode"), wxBitmap(wxImage(wxT(icon_path + "image.png"))), _T("Select View Mode"));

  tool_bar_vis->AddTool(VIEW_WIREFRAME,wxT("View Wireframe"),
			wxString((icon_path + "chart_line.png").c_str(),wxConvUTF8),
			wxT("View Wireframe"));
	
  tool_bar_vis->AddTool(VIEW_SOLID, wxT("View solid"),
			wxString((icon_path + "chart_bar.png").c_str(),wxConvUTF8),
			wxT("View solid"));

  //tool_bar_tools
  tool_bar_tools->AddTool(CALCULATE_DISTANCE,wxT("Calculate distance"),
			  wxString((icon_path + "distance.png").c_str(),wxConvUTF8),
			  wxT("Calculate distance"));

  //tool_bar_element
  tool_bar_element->AddTool(TRANSFORM_TRANSLATION, wxT("Translate element"),
			    wxString((icon_path + "translation.png").c_str(),wxConvUTF8),
			    wxT("Translate element"));
	
  tool_bar_element->AddTool(TRANSFORM_ROTATION, wxT("Rotate element"),
			    wxString((icon_path + "rotation.png").c_str(),wxConvUTF8),
			    wxT("Rotate element"));

  tool_bar_element->AddTool(TRANSFORM_SCALE, wxT("Scale Element"),
			    wxString((icon_path + "scale.png").c_str(),wxConvUTF8),
			    wxT("Scale element"));

  tool_bar_element->AddTool(ELEMENT_PROPERTIES,wxT("Edit element properties"),
			    wxString((icon_path + "properties.png").c_str(),wxConvUTF8),
			    wxT("Edit properties"));

  //tool_bar_sim
  tool_bar_sim->SetToolBitmapSize(wxSize(bic_size,bic_size));
  ApplicationTime::GetInstance()->Attach(tool_bar_sim);

  //tool_bar_help

  tool_bar_help->AddTool(HELP,wxT("Help"),
			 wxString((icon_path + "help.png").c_str(),wxConvUTF8),
			 wxT("Help"));

  tool_bar_file->Realize();
  tool_bar_edit->Realize();
  tool_bar_project->Realize();
  tool_bar_vis->Realize();
  tool_bar_element->Realize();
  tool_bar_tools->Realize();
  tool_bar_sim->Realize();
  tool_bar_sun->Realize();
  tool_bar_help->Realize();
  tool_bar_status->Realize();



  m_mgr.AddPane(tool_bar_file,
		wxAuiPaneInfo().
		ToolbarPane().
		Top().Gripper().
		Floatable().
		Caption(wxT("File")).
		PaneBorder(false).
		Name(wxString(((std::string)NAME_TOOL_BAR_FILE).c_str(),wxConvUTF8)));

  m_mgr.AddPane(tool_bar_edit,
		wxAuiPaneInfo().
		ToolbarPane().
		Top().Gripper().
		Floatable().
		Caption(wxT("Edit")).
		PaneBorder(false).
		Name(wxString(((std::string)NAME_TOOL_BAR_EDIT).c_str(),wxConvUTF8)));
	

  m_mgr.AddPane(tool_bar_vis,
		wxAuiPaneInfo().
		ToolbarPane().
		Top().Gripper().
		Floatable().
		Caption(wxT("View")).
		PaneBorder(false).
		Name(wxString(((std::string)NAME_TOOL_BAR_VIS).c_str(),wxConvUTF8)));


  m_mgr.AddPane(tool_bar_project,
		wxAuiPaneInfo().
		ToolbarPane().
		Top().Gripper().
		Floatable().
		Caption(wxT("Project")).
		PaneBorder(false).
		Name(wxString(((std::string)NAME_TOOL_BAR_PROJECT).c_str(),wxConvUTF8)));
	

  m_mgr.AddPane(tool_bar_element,
		wxAuiPaneInfo().
		ToolbarPane().
		Top().Gripper().
		Floatable().
		Caption(wxT("Element")).
		PaneBorder(false).
		Name(wxString(((std::string)NAME_TOOL_BAR_ELEMENT).c_str(),wxConvUTF8)));


  m_mgr.AddPane(tool_bar_tools,
		wxAuiPaneInfo().
		ToolbarPane().
		Top().Gripper().
		Floatable().
		Caption(wxT("Tools")).
		PaneBorder(false).
		Name(wxString(((std::string)NAME_TOOL_BAR_TOOLS).c_str(),wxConvUTF8)));


  int width,height;
  GetClientSize(&width,&height);

  m_mgr.AddPane(tool_bar_status,
		wxAuiPaneInfo().
		ToolbarPane().
		Bottom().Gripper().
		Floatable().
		Caption(wxT("Application Status")).
		PaneBorder(false).
		Name(wxString(((std::string)NAME_TOOL_BAR_STATUS).c_str(),wxConvUTF8)));

  m_mgr.AddPane(tool_bar_sim,
		wxAuiPaneInfo().
		ToolbarPane().
		Bottom().Gripper().
		Floatable().
		Caption(wxT("Animation Controls")).
		PaneBorder(false).
		Name(wxString(((std::string)NAME_TOOL_BAR_SIM).c_str(),wxConvUTF8)));
		   
  m_mgr.AddPane(tool_bar_sun,
		wxAuiPaneInfo().
		ToolbarPane().
		Top().Gripper().
		Floatable().
		Caption(wxT("Sun Configuration")).
		PaneBorder(false).
		Name(wxString(((std::string)NAME_TOOL_BAR_SUN).c_str(),wxConvUTF8)));

  m_mgr.AddPane(tool_bar_help,
		wxAuiPaneInfo().
		ToolbarPane().
		Top().Gripper().
		Floatable().
		Caption(wxT("Help")).
		PaneBorder(false).
		Name(wxString(((std::string)NAME_TOOL_BAR_HELP).c_str(),wxConvUTF8)));
	

  SetMenuBar(menu_bar);

  menu_bar->Refresh();

  RegisterMenu(EDIT_SELECT_ALL, "Select All\tCtrl+A");
  RegisterMenu(EDIT_FIND, "Find\tCtrl+F");

  CreateStatusBar();
  SetStatusText( _T(" ") );

}

void UIApplicationMainFrame::InitGUIContents(cpw::LayerTree &layer_tree,
					     cpw::remote::RemoteProtocol *protocol,
					     cpw::INavigatorManager *navigator_manager)
{
  std::string icon_path = ApplicationConfiguration::GetInstance()->GetUIIconDirectory();
  
  int width, height;
  GetClientSize(&width, &height);

  int ic_size = cpw::ApplicationConfiguration::GetInstance()->GetIconSize();
  int bic_size = cpw::ApplicationConfiguration::GetInstance()->GetBigIconSize();

  tool_bar_sun->Update_SunPos();

  //Help Window
  help_window = new UIHelp(this, wxID_ANY, wxT("Rvzware"), wxT("resources/application_help.png"), wxT("http://www.capaware.org"), wxT("Rvzware 2009"), wxT("Version 1.0.0"));
  help_window->Show(false);

  //Layer Tree
  layer_frame = new wxMiniFrame(this, PANE_LAYERTREE, _T("Layer Tree"),
				wxDefaultPosition, wxDefaultSize,
				wxBORDER_NONE | wxTRANSPARENT_WINDOW,
				_T("Layer Tree"));

    
  wxString tmp2(((std::string)("Layer Tree")).c_str(),wxConvUTF8);
  ui_layer_tree = new cpw::gui::UILayerTreePanel(this,navigator_manager,
						 layer_tree,
						 layer_frame, 0, 0, 200, 512,
						 tmp2);
  


  layer_tree.Attach((cpw::Observer *)ui_layer_tree);
  ui_layer_tree->FillTree();
  layer_frame->SetSize(wxSize(200,200));


  //Layer Tree toolbar
  ui_layer_tree_tb = layer_frame->CreateToolBar(wxNO_BORDER | wxHORIZONTAL | wxTB_FLAT);
  ui_layer_tree_tb->SetToolBitmapSize(wxSize(ic_size,ic_size));

  ui_layer_tree_tb->AddTool(ADD_ENTITY, _("Import"),
			    wxString((icon_path + "import.png").c_str(),wxConvUTF8),
			    _("Import"));	
  ui_layer_tree_tb->AddSeparator();

  ui_layer_tree_tb->AddTool(NEW_CONTAINER_LAYER_PRIMITIVE, _("New container layer template"),
			    wxString((icon_path + "folder_brick_template.png").c_str(),wxConvUTF8),
			    _("Create container layer template"));

  ui_layer_tree_tb->AddTool(NEW_ELEMPRIMITIVE, _("New element template"),
			    wxString((icon_path + "brick_p.png").c_str(),wxConvUTF8),
			    _("Create element template"));
  ui_layer_tree_tb->AddSeparator();

  ui_layer_tree_tb->AddTool(NEW_LAYER, _("New Layer"),
			    wxString((icon_path + "folder_brick.png").c_str(),wxConvUTF8),
			    _("Add container layer"));

  ui_layer_tree_tb->AddTool(NEW_ELEMENT, _("New Element"),
			    wxString((icon_path + "brick.png").c_str(),wxConvUTF8),
			    _("Add element"));
  alphaSlider = new wxSlider(ui_layer_tree_tb, ALPHA_SLIDER, 0 , 0, 255, wxDefaultPosition, wxSize(70,19), wxSL_HORIZONTAL);
  alphaSlider->SetLineSize(2);
  alphaSlider->SetValue(255);
  ui_layer_tree_tb->AddControl(alphaSlider);
  ui_layer_tree_tb->Realize();

  ////Connection Tree
  cpw::remote::ConnectionManager * connection_manager = protocol->GetConnectionManager();
  connection_frame = new wxMiniFrame(this, PANE_CONNECTIONTREE, _("Connection Tree"), wxDefaultPosition,
				     wxDefaultSize, wxBORDER_NONE | wxTRANSPARENT_WINDOW,_("Connection Tree"));
	
  wxString tmp(((std::string)"Connection Tree").c_str(),wxConvUTF8);
  ui_connection_tree = new cpw::gui::UIConnectionTreePanel(protocol,
							   connection_frame,
							   0, 0, 200, 512,
							   tmp);

	
  connection_manager->Attach((cpw::Observer *)ui_connection_tree);
  ui_connection_tree->InitializeTree();
  connection_frame->SetSize(wxSize(200,200));

  //toolbars backgroundcolour
  wxAuiDockArt* auiArtProvider = m_mgr.GetArtProvider();
  wxColour backgroundColor= auiArtProvider->GetColour(5);
  ui_layer_tree_tb->SetBackgroundColour(backgroundColor);

  //Properties
  properties_frame = new wxMiniFrame(this, PANE_PROPERTIES, _T("Properties"),wxDefaultPosition, wxDefaultSize, wxBORDER_NONE | wxTRANSPARENT_WINDOW,_T("Properties"));
  ui_entity_properties_grid = new UIEntityPropertiesGrid (properties_frame, wxID_ANY);
  ui_layer_tree->SetEntityPropertiesGrid(ui_entity_properties_grid);
  ui_entity_properties_grid->SetSize(ui_entity_properties_grid->GetBestSize());
  properties_frame->SetSize(wxSize(200,200));

  //Animation Entity Scheme
  animation_scheme_frame = new wxMiniFrame(this, PANE_ANIMATION_SCHEME,_T("Animation Entity Scheme"), wxDefaultPosition, wxDefaultSize, wxBORDER_NONE | wxTRANSPARENT_WINDOW,_T("Animation Entity Scheme"));
  ui_animation_scheme_window = new UIMovieScheme(animation_scheme_frame, wxID_ANY);
  ApplicationTime::GetInstance()->Attach((cpw::Observer*)ui_animation_scheme_window);
	
  //log_frame
  log_frame = new wxMiniFrame(this, PANE_LOG, _T("Log"), wxDefaultPosition, wxDefaultSize, wxBORDER_NONE | wxTRANSPARENT_WINDOW,_T("Log"));
  ui_log_window = new UILogWindow(log_frame);
  log_frame->SetSize(wxSize(200, 200));
  cpw::ApplicationLog::GetInstance()->SetLogger((cpw::Logger *) ui_log_window);

  //scene tree frame
  scene_tree_frame = new wxMiniFrame(this, PANE_LOG, _T("Scene tree"), wxDefaultPosition, wxDefaultSize, wxBORDER_NONE | wxTRANSPARENT_WINDOW,_T("Properties"));;
  ui_scene_tree = new UISceneTree(scene_tree_frame);
  scene_tree_frame->SetSize(wxSize(200, 200));

  //splitter canvas
  splitter = new wxSplitterWindow(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSP_3D | wxSP_LIVE_UPDATE);
  viewport_manager = new cpw::gui::UISplitterCanvas(splitter, navigator_manager);
  navigator_manager_ = navigator_manager;

  splitter->SetFocus();
  splitter->SetSize(wxSize(width, height));
  splitter->SetSashGravity(0.1);
  splitter->SetMinimumPaneSize(240);

  viewport_field = viewport_manager->PresetOnlyView(wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSP_3D );
  //viewport_field = viewport_manager->Preset4ViewsB(wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSP_3D );
	
  splitter->Initialize(viewport_field);
	

  //wxAUI 
  m_mgr.AddPane(layer_frame, wxLEFT, wxT("Layer Tree"));
  m_mgr.AddPane(properties_frame, wxLEFT, wxT("Properties"));
  m_mgr.AddPane(splitter, wxCENTRE, wxT("Viewport"));
  m_mgr.AddPane(log_frame, wxLEFT, wxT("Log")); 
  m_mgr.AddPane(scene_tree_frame, wxRIGHT, wxT("Scene tree"));
  if (SHOW_ADDITIONAL_WINDOWS)
    m_mgr.AddPane(connection_frame, wxRIGHT, wxT("Connection Tree"));
  else
    m_mgr.AddPane(connection_frame, wxLEFT, wxT("Connection Tree"));
	
  m_mgr.AddPane(animation_scheme_frame, wxBOTTOM, wxT("Animation Entity Scheme")); 

  m_mgr.GetPane(layer_frame).MinSize(wxSize(100,100)).
    Name(wxString(((std::string)NAME_LAYER_FRAME).c_str(),wxConvUTF8));
  m_mgr.GetPane(properties_frame).MinSize(wxSize(100,100)).
    Name(wxString(((std::string)NAME_PROPERTIES_FRAME).c_str(),wxConvUTF8));
  m_mgr.GetPane(splitter).MinSize(wxSize(240,240)).
    Name(wxString(((std::string)NAME_SPLITTER).c_str(),wxConvUTF8));
  m_mgr.GetPane(log_frame).MinSize(wxSize(100, 100)).
    Name(wxString(((std::string)NAME_LOG_FRAME).c_str(),wxConvUTF8));
  m_mgr.GetPane(scene_tree_frame).MinSize(wxSize(100, 100)).
    Name(wxString(((std::string)NAME_SCENE_TREE_FRAME).c_str(),wxConvUTF8));
  m_mgr.GetPane(connection_frame).MinSize(wxSize(100,100)).
    Name(wxString(((std::string)NAME_CONNECTION_FRAME).c_str(),wxConvUTF8));
  m_mgr.GetPane(animation_scheme_frame).MinSize(wxSize(100, 50)).
    Name(wxString(((std::string)NAME_ANIMATION_SCHEME_FRAME).c_str(),wxConvUTF8));
	
  m_mgr.Update();


  //trick
  /*m_mgr.GetPane(layer_frame).Float().Hide();
  m_mgr.GetPane(properties_frame).Float().Hide();
  m_mgr.GetPane(log_frame).Float().Hide(); 
  m_mgr.GetPane(scene_tree_frame).Float().Hide();
  m_mgr.GetPane(connection_frame).Float().Hide();
  m_mgr.GetPane(animation_scheme_frame).Float().Hide(); 
  */
  
  //MPM
  m_mgr.GetPane(layer_frame).Hide();
  m_mgr.GetPane(properties_frame).Hide();
  m_mgr.GetPane(log_frame).Hide(); 
  m_mgr.GetPane(scene_tree_frame).Hide();
  m_mgr.GetPane(connection_frame).Hide();
  m_mgr.GetPane(animation_scheme_frame).Hide(); 
  
  m_mgr.Update();

  m_mgr.GetPane(layer_frame).Dock().Show();
  m_mgr.GetPane(properties_frame).Dock().Show();
  m_mgr.GetPane(animation_scheme_frame).Dock().Show();
  //m_mgr.GetPane(animation_scheme_frame).Dock().Hide();
	
  if (SHOW_ADDITIONAL_WINDOWS)
    {
      m_mgr.GetPane(log_frame).Dock().Show(); 
      ui_log_window->NewLogMessage("debug log");	

      m_mgr.GetPane(scene_tree_frame).Dock().Show(); 
      ui_log_window->NewLogMessage("debug log");	

      m_mgr.GetPane(connection_frame).Dock().Show();
    }
  else
    m_mgr.GetPane(connection_frame).Dock().Hide();		
	
	
  m_mgr.Update();
  

  wxSizeEvent event = wxSizeEvent();
  OnSize((wxSizeEvent &)event);

  ui_default_config = m_mgr.SavePerspective();

  wxString gui_config(ApplicationConfiguration::GetInstance()->GetUIConfiguration().c_str(),wxConvUTF8);
  if (!gui_config.empty())
    m_mgr.LoadPerspective(gui_config);
  m_mgr.Update();

  CheckVisibility();

  InitShortCuts();
}

void UIApplicationMainFrame::OnSize(wxSizeEvent& event)
{
  wxFrame::OnSize(event);
  application->Draw();
}

UIApplicationMainFrame::~UIApplicationMainFrame(void)
{
  m_mgr.UnInit();
  delete viewport_manager;
}

void UIApplicationMainFrame::OnNoteBookPageChanged(wxNotebookEvent& WXUNUSED(event))
{
  application->SetActiveNavigator(note_book->GetSelection());
}

void UIApplicationMainFrame::EventoQuit(wxCommandEvent& WXUNUSED(event))
{
	
} 
void UIApplicationMainFrame::OnAppClose(wxCommandEvent& WXUNUSED(event))
{
  if(application->IsControllerOpen()) return;
  application->Close();
}
//void UIApplicationMainFrame::OnAppOpen(wxCommandEvent& WXUNUSED(event))
//{
//	application->Open(this);
//}
void UIApplicationMainFrame::OnSave(wxCommandEvent& WXUNUSED(event))
{
  application->SaveAll();
}
void UIApplicationMainFrame::OnSaveAll(wxCommandEvent& WXUNUSED(event))
{
  application->SaveAll();
}

void UIApplicationMainFrame::OnFileOpen(wxCommandEvent& WXUNUSED(event))
{
  application->OpenProject();
}

void UIApplicationMainFrame::OnFileClose(wxCommandEvent& WXUNUSED(event))
{
  application->CloseProject();
}

void UIApplicationMainFrame::OnClose(wxCloseEvent& event)
{
  application->Close();
}

void UIApplicationMainFrame::NotAvailable(wxCommandEvent& WXUNUSED(event))
{

  application->NotAvailable();

  wxMessageDialog message(NULL,wxT("This option is not available yet."), wxT("Warning"),wxICON_EXCLAMATION |wxOK);
  message.ShowModal();

}

void UIApplicationMainFrame::AnimateEntity(wxCommandEvent& WXUNUSED(event))
{
  application->AnimateEntity();
  wxMessageDialog message(NULL,wxT("This option is not available yet."), wxT("Warning"),wxICON_EXCLAMATION |wxOK);
  message.ShowModal();

}

void UIApplicationMainFrame::AddNavigator(wxCommandEvent& WXUNUSED(event))
{
}

void UIApplicationMainFrame::SetOnlyView()
{
  viewport_manager->Replace(viewport_manager->PresetOnlyView(wxID_ANY));
}
void UIApplicationMainFrame::Set4ViewsA()
{
  viewport_manager->Replace(viewport_manager->Preset4ViewsA(wxID_ANY));
}
void UIApplicationMainFrame::Set4ViewsB()
{
  viewport_manager->Replace(viewport_manager->Preset4ViewsB(wxID_ANY));
}
void UIApplicationMainFrame::Set4ViewsC()
{
  viewport_manager->Replace(viewport_manager->Preset4ViewsC(wxID_ANY));
}
void UIApplicationMainFrame::Set4ViewsD()
{
  viewport_manager->Replace(viewport_manager->Preset4ViewsD(wxID_ANY));
}
void UIApplicationMainFrame::Set4ViewsE()
{
  viewport_manager->Replace(viewport_manager->Preset4ViewsE(wxID_ANY));
}
void UIApplicationMainFrame::Set3ViewsA()
{
  viewport_manager->Replace(viewport_manager->Preset3ViewsA(wxID_ANY));
}
void UIApplicationMainFrame::Set3ViewsB()
{
  viewport_manager->Replace(viewport_manager->Preset3ViewsB(wxID_ANY));
}
void UIApplicationMainFrame::Set3ViewsC()
{
  viewport_manager->Replace(viewport_manager->Preset3ViewsC(wxID_ANY));
}
void UIApplicationMainFrame::Set3ViewsD()
{
  viewport_manager->Replace(viewport_manager->Preset3ViewsD(wxID_ANY));
}
void UIApplicationMainFrame::Set2ViewsH()
{
  viewport_manager->Replace(viewport_manager->Preset2ViewsH(wxID_ANY));
}
void UIApplicationMainFrame::Set2ViewsV()
{
  viewport_manager->Replace(viewport_manager->Preset2ViewsV(wxID_ANY));
}

void UIApplicationMainFrame::OnIdle(wxIdleEvent& event)
{
  application->Update();
  event.RequestMore();
}

void UIApplicationMainFrame::OnInternalIdle()
{
  application->Update();
  //ui_layer_tree->Repaint();	
  //wxFrame::Update(); 
	
  //ui_layer_tree->Update();
}

void UIApplicationMainFrame::DrawModeWireframe(wxCommandEvent& WXUNUSED(event))
{
  application->DrawModeWireframe(0);
}

void UIApplicationMainFrame::DrawModeSolid(wxCommandEvent& WXUNUSED(event))
{
  application->DrawModeSolid(0);
}

//void UIApplicationMainFrame::ViewSelection(wxCommandEvent& WXUNUSED(event))
//{
//	application->ViewSelection(this);
//}

void UIApplicationMainFrame::ViewFile(wxCommandEvent& WXUNUSED(event))
{
  TogglePaneVisibility(PANE_FILE, _T("File"));
}
void UIApplicationMainFrame::ViewEdit(wxCommandEvent& WXUNUSED(event))
{
  TogglePaneVisibility(PANE_EDIT, _T("Edit"));
}
void UIApplicationMainFrame::ViewView(wxCommandEvent& WXUNUSED(event))
{
  TogglePaneVisibility(PANE_VIEW, _T("View"));
}
void UIApplicationMainFrame::ViewAnimationControls(wxCommandEvent& WXUNUSED(event))
{
  TogglePaneVisibility(PANE_ANIMATION_CONTROLS, _T("Animation Controls"));
}

void UIApplicationMainFrame::ViewSunConfig(wxCommandEvent& WXUNUSED(event))
{
  TogglePaneVisibility(PANE_SUN_CONFIG, _T("Sun Configuration"));
}

void UIApplicationMainFrame::ViewAnimationScheme(wxCommandEvent& WXUNUSED(event))
{
  TogglePaneVisibility(PANE_ANIMATION_SCHEME, _T("Animation Entity Scheme"));
}
void UIApplicationMainFrame::ViewApplicationStatus(wxCommandEvent& WXUNUSED(event))
{
  TogglePaneVisibility(PANE_STATUS, _T("Application Status"));
}

void UIApplicationMainFrame::ViewProject(wxCommandEvent& WXUNUSED(event))
{
  TogglePaneVisibility(PANE_PROJECT, _T("Project"));
}
void UIApplicationMainFrame::ViewElement(wxCommandEvent& WXUNUSED(event))
{
  TogglePaneVisibility(PANE_ELEMENT, _T("Element"));
}
void UIApplicationMainFrame::ViewTools(wxCommandEvent& WXUNUSED(event))
{
  TogglePaneVisibility(PANE_TOOLS, _T("Tools"));
}
void UIApplicationMainFrame::ViewHelp(wxCommandEvent& WXUNUSED(event))
{
  TogglePaneVisibility(PANE_HELP, _T("Help"));
}
void UIApplicationMainFrame::ViewLog(wxCommandEvent& WXUNUSED(event))
{
  TogglePaneVisibility(PANE_LOG, _T("Log"));
}
void UIApplicationMainFrame::ViewLayerTree(wxCommandEvent& WXUNUSED(event))
{
  TogglePaneVisibility(PANE_LAYERTREE, _T("Layer Tree"));
}
void UIApplicationMainFrame::ViewConnectionTree(wxCommandEvent& WXUNUSED(event))
{
  TogglePaneVisibility(PANE_CONNECTIONTREE, _T("Connection Tree"));
}
void UIApplicationMainFrame::ViewProperties(wxCommandEvent& WXUNUSED(event))
{
  TogglePaneVisibility(PANE_PROPERTIES, _T("Properties"));
}

void UIApplicationMainFrame::TogglePaneVisibility(int win_id, const wxString& pane_name)
{
  std::string icon_path = ApplicationConfiguration::GetInstance()->GetUIIconDirectory();	
  wxWindow *window = FindWindow(win_id);

  if (m_mgr.GetPane(window).IsShown()) 
    m_mgr.GetPane(window).Hide();
  else
    m_mgr.GetPane(window).Show();

  view_menu->UpdateUI();
  m_mgr.Update();
}

void UIApplicationMainFrame::ViewShowAll(wxCommandEvent& WXUNUSED(event))
{
  std::string icon_path = ApplicationConfiguration::GetInstance()->GetUIIconDirectory();	
  wxWindow *window;
  wxString nombre;
  for (int i = FIRST_PANE ; i<=LAST_PANE; i++)
    {
      window = FindWindow(i);
      if (!m_mgr.GetPane(window).IsShown())
	{
	  nombre = FindWindow(i)->GetName();
	  m_mgr.GetPane(window).Show();
	  view_menu->FindItem(view_menu->FindItem(nombre))->Check();
	}
    }
  view_menu->UpdateUI();
  m_mgr.Update();
}
void UIApplicationMainFrame::ViewHideAll(wxCommandEvent& WXUNUSED(event))
{
  std::string icon_path = ApplicationConfiguration::GetInstance()->GetUIIconDirectory();	
  wxWindow *window;
  wxString nombre;
  for (int i = FIRST_PANE ; i<=LAST_PANE; i++)
    {
      window = FindWindow(i);
      if (m_mgr.GetPane(window).IsShown())
	{
	  nombre = FindWindow(i)->GetName();
	  m_mgr.GetPane(window).Hide();
	  view_menu->FindItem(view_menu->FindItem(nombre))->Check(false);
	}
    }
  m_mgr.Update();
}

void UIApplicationMainFrame::ViewFullScreen(wxCommandEvent& WXUNUSED(event))
{
  application->SwitchFullScreen();
}

void UIApplicationMainFrame::NewElementPrimitive(wxCommandEvent& WXUNUSED(event))
{
  application->NewElementPrimitive(this);
}

void UIApplicationMainFrame::NewElement(wxCommandEvent& WXUNUSED(event))
{
  application->NewElement();
}

void UIApplicationMainFrame::NewLayer(wxCommandEvent& WXUNUSED(event))
{
  application->NewLayer(this);
}

void UIApplicationMainFrame::ShowHelp(wxCommandEvent& WXUNUSED(event))
{
  help_window->Show();
}

void UIApplicationMainFrame::NewTable(wxCommandEvent& WXUNUSED(event))
{
  application->NewTable(this);
}

void UIApplicationMainFrame::NewFire(wxCommandEvent& WXUNUSED(event))
{
  application->NewFire();
}

void UIApplicationMainFrame::NewFirewall(wxCommandEvent& WXUNUSED(event))
{
  application->NewFirewall(this);
}

void UIApplicationMainFrame::NewWmsLayer(wxCommandEvent& WXUNUSED(event))
{
  application->NewWmsLayer();
}

void UIApplicationMainFrame::NewRemoteEntity(wxCommandEvent& WXUNUSED(event))
{
  application->NewRemoteEntity(this);
}

void UIApplicationMainFrame::AddEntity(wxCommandEvent& WXUNUSED(event))
{
  application->AddEntity(this);
}
void UIApplicationMainFrame::AddRemoteEntity(wxCommandEvent& WXUNUSED(event))
{
  application->AddRemoteEntity(this);
}
void UIApplicationMainFrame::AddDataBase(wxCommandEvent& WXUNUSED(event))
{
  application->AddDataBase(this);
}

void UIApplicationMainFrame::NewContainerLayerPrimitive(wxCommandEvent& WXUNUSED(event))
{
  application->NewContainerLayerPrimitive(this);
}

void UIApplicationMainFrame::NewRasterLayerPrimitive(wxCommandEvent& WXUNUSED(event))
{
  application->NewRasterLayerPrimitive(this);
}

void UIApplicationMainFrame::NewVectorialLayerPrimitive(wxCommandEvent& WXUNUSED(event))
{
  application->NewVectorialLayerPrimitive(this);
}

void UIApplicationMainFrame::OnKeyUp(wxKeyEvent &event)
{ 
  application->ReleaseKey(event.GetKeyCode()); 
}

void UIApplicationMainFrame::OnKeyDown(wxKeyEvent &event)
{
  application->PressKey(event.GetKeyCode()); 
}

void UIApplicationMainFrame::OnMouse(wxMouseEvent &event)
{
  int debug;
  debug = 1;
  if (event.ButtonDown())
    {
      int button = event.GetButton();
    }
}

void UIApplicationMainFrame::OnLeftDown(wxMouseEvent &event)
{}
void UIApplicationMainFrame::OnLeftUp(wxMouseEvent &event)
{}
void UIApplicationMainFrame::OnLeftDoubleClick(wxMouseEvent &event)
{}


int UIApplicationMainFrame::AddPluginMenu(const std::string &new_plugin)
{
  int new_id = plugins_ids;
  std::string icon_path = ApplicationConfiguration::GetInstance()->GetUIIconDirectory();

  
  RegisterMenu(new_id, new_plugin);

  wxMenuItem * menu_item = GetMenuItem(new_id, plugins_menu,
				       wxT("Ctrl+P"),
				       wxString((icon_path + "add.png").c_str(),wxConvUTF8));
  plugins_menu->Append(menu_item);
  Connect(new_id, wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction)&UIApplicationMainFrame::OnPlugin);

  plugins_ids++;

  return (plugins_ids-1);
}


void UIApplicationMainFrame::OnPlugin(wxCommandEvent& event)
{
  application->ExecutePlugin(event.GetId());
}

void UIApplicationMainFrame::SendFront(const cpw::TypeId &id)
{
  application->SendFront(id);
}

void UIApplicationMainFrame::SendBack(const cpw::TypeId &id)
{
  application->SendBack(id);
}


void UIApplicationMainFrame::ModifyProperties(const cpw::TypeId &id)
{
  application->ModifyProperties(id, this);
}

void UIApplicationMainFrame::GoTo(const cpw::TypeId &id)
{
  application->GoTo(id);
}
			
void UIApplicationMainFrame::EditCopy(wxCommandEvent& WXUNUSED(event))
{
  if(application->IsControllerOpen()) return;

  ui_layer_tree->CopySelection();  
}

void UIApplicationMainFrame::Copy(const cpw::TypeId &id)
{
  if(application->IsControllerOpen()) return;

  application->Copy(id);
}

void UIApplicationMainFrame::Duplicate(const cpw::TypeId &id)
{
  if(application->IsControllerOpen()) return;

  application->Duplicate(id);
}
			
void UIApplicationMainFrame::EditCut(wxCommandEvent& WXUNUSED(event))
{
  if(application->IsControllerOpen()) return;

  ui_layer_tree->CutSelection();   
}

void UIApplicationMainFrame::Cut(const cpw::TypeId &id)
{
  if(application->IsControllerOpen()) return;

  application->Cut(id);
}
			
void UIApplicationMainFrame::EditPaste(wxCommandEvent& WXUNUSED(event))
{
  if(application->IsControllerOpen()) return;

  ui_layer_tree->PasteSelection();   
}

void UIApplicationMainFrame::Paste(const cpw::TypeId &id_orig,
				   const cpw::TypeId &id_dest,
				   const cpw::TypeId &parent_id_orig,
				   const cpw::TypeId &parent_id_dest,
				   const bool &cut, const bool &copy,
				   const bool &duplicate)
{
  if(application->IsControllerOpen()) return;

  application->Paste(id_orig, id_dest, parent_id_orig,
		     parent_id_dest, cut, copy, duplicate);
}
			
void UIApplicationMainFrame::EditDelete(wxCommandEvent& WXUNUSED(event))
{
  if(application->IsControllerOpen()) return;

  ui_layer_tree->DeleteSelection();   
}

void UIApplicationMainFrame::Delete(const cpw::TypeId &ent_id,
				    const cpw::TypeId &parent_id)
{
  if(application->IsControllerOpen()) return;

  application->Delete(ent_id, parent_id);
}

void UIApplicationMainFrame::EditProperties(wxCommandEvent& WXUNUSED(event))
{
  ui_layer_tree->ModifySelection();   
}

void UIApplicationMainFrame::Visualize(const cpw::TypeId &id, const bool &value)
{
  application->Visualize(id, value);
}

void UIApplicationMainFrame::Publish(const cpw::TypeId &id)
{
  cpw::Entity* ent;
  ent = cpw::EntityRegistry::GetInstance()->GetEntity(id);

  if (ent != NULL) 
    ent->SetPublished(!ent->isPublished());
}

void UIApplicationMainFrame::Animate(const cpw::TypeId &id)
{
  cpw::Entity* ent;
  ent = cpw::EntityRegistry::GetInstance()->GetEntity(id);

  if (ent != NULL) 
    ent->SetAnimate(!ent->isAnimate());
}

void UIApplicationMainFrame::AddLine()
{
  application->AddLine();
}

void UIApplicationMainFrame::OnSliderChange(wxScrollEvent& WXUNUSED(event))
{
}

int  UIApplicationMainFrame::GetWMSALphaValue()
{
  return alphaSlider->GetValue();
}

void UIApplicationMainFrame::SetWMSAlphaValue(const int &value)
{
  alphaSlider->SetValue(value);
}

void UIApplicationMainFrame::SetSunPosition(const float &pos)
{
  application->SetSunPosition(pos);
}

int UIApplicationMainFrame::GetFireViewMode()
{
  return tool_bar_sim->GetFireViewMode();
}

void UIApplicationMainFrame::SetFireViewMode(const int &value)
{
}

void UIApplicationMainFrame::TwoDimesionNavigationMode(wxCommandEvent& WXUNUSED(event))
{
  application->Set2DNavigationMode();
}

void UIApplicationMainFrame::ThreeDimesionNavigationMode(wxCommandEvent& WXUNUSED(event))
{
  application->Set3DNavigationMode();
}

void UIApplicationMainFrame::CalculateDistance(wxCommandEvent& WXUNUSED(event))
{
  application->CalculateDistance(this);
}

std::string UIApplicationMainFrame::GetUIConfiguration()
{
  wxString tmp = m_mgr.SavePerspective();
  return std::string(tmp.mb_str());
}

void UIApplicationMainFrame::Translate(wxCommandEvent& WXUNUSED(event))
{
  application->SetTranslateHandler();
}
void UIApplicationMainFrame::Rotate(wxCommandEvent& WXUNUSED(event))
{
  application->SetRotateHandler();
}
void UIApplicationMainFrame::Scale(wxCommandEvent& WXUNUSED(event))
{
  application->SetScaleHandler();
}

void UIApplicationMainFrame::SetDefaultConfig(wxCommandEvent& WXUNUSED(event))
{
  m_mgr.LoadPerspective(ui_default_config);
  m_mgr.Update();
  CheckVisibility();
}

void UIApplicationMainFrame::CheckVisibility()
{
  std::string icon_path = ApplicationConfiguration::GetInstance()->GetUIIconDirectory();	
  wxWindow *window;
  wxString nombre;
  for (int i = FIRST_PANE ; i<=LAST_PANE; i++)
    {
      window = FindWindow(i);
      if (window!=NULL)
	{
	  nombre = FindWindow(i)->GetName();
	  if (!m_mgr.GetPane(window).IsShown())
	    view_menu->FindItem(view_menu->FindItem(nombre))->Check(false);
	  else
	    view_menu->FindItem(view_menu->FindItem(nombre))->Check(true);
	}
    }
  view_menu->UpdateUI();
  m_mgr.Update();
}

//void UIApplicationMainFrame::DeleteLayerTreePanel()
//{
//	ui_layer_tree->Clear();
//}

void UIApplicationMainFrame::EnableGUI(const bool &value)
{
  tool_bar_vis->Enable(value);
  if (!value) tool_bar_sim->Stop(); 
  tool_bar_sim->Enable(value); 
  tool_bar_sun->Enable(value); 
  tool_bar_tools->Enable(value);
  tool_bar_file->EnableTool(FILE_SAVE,value); 
  //tool_bar_file->EnableTool(FILE_SAVE_ALL,value); 
  tool_bar_edit->Enable(value); 
  tool_bar_element->Enable(value);
  tool_bar_status->Enable(value); 
  tool_bar_project->Enable(value);

  ui_layer_tree_tb->EnableTool(ADD_ENTITY, value);
  ui_layer_tree_tb->EnableTool(NEW_CONTAINER_LAYER_PRIMITIVE, value);
  ui_layer_tree_tb->EnableTool(NEW_ELEMPRIMITIVE, value);
  ui_layer_tree_tb->EnableTool(NEW_LAYER, value);
  ui_layer_tree_tb->EnableTool(NEW_ELEMENT, value);

  view_menu->Enable(VIEW_WIREFRAME,value);
  view_menu->Enable(VIEW_SOLID,value);

  file_menu->Enable(FILE_SAVE,value);
  //file_menu->Enable(FILE_SAVE_ALL,value);
  file_menu->Enable(FILE_CLOSE,value);
  edit_menu->Enable(EDIT_CUT,value);
  edit_menu->Enable(EDIT_COPY,value);
  edit_menu->Enable(EDIT_PASTE,value);
  edit_menu->Enable(EDIT_DELETE,value);
  navigation_mode_menu->Enable(TWO_DIMENSION_NAVIGATION_MODE,value);
  navigation_mode_menu->Enable(THREE_DIMENSION_NAVIGATION_MODE,value);
	
  project_menu->Enable(ADD_ENTITY,value);
  project_submenu_add_template->Enable(NEW_CONTAINER_LAYER_PRIMITIVE,value);
  project_submenu_add_template->Enable(NEW_ELEMPRIMITIVE,value);
  //project_submenu_add_remote->Enable(ADD_ENTITY,value);
  //project_submenu_add_remote->Enable(ADD_REMOTE_ENTITY,value);
  project_submenu_add_new->Enable(NEW_ELEMENT,value);
  project_submenu_add_new->Enable(NEW_LAYER,value);
  project_submenu_add_new->Enable(NEW_WMSLAYER,value);
  project_submenu_add_new->Enable(NEW_REMOTE_ENTITY,value);
  project_submenu_add_new->Enable(NEW_FIRE,value);
  project_submenu_add_new->Enable(NEW_FIREWALL,value);

  element_submenu_transform->Enable(TRANSFORM_TRANSLATION,value);
  element_submenu_transform->Enable(TRANSFORM_ROTATION,value);
  element_submenu_transform->Enable(TRANSFORM_SCALE,value);
  element_menu->Enable(ELEMENT_PROPERTIES,value);
  tools_submenu_calculate->Enable(CALCULATE_DISTANCE,value);

  UpdateAnimableUI();
}

void UIApplicationMainFrame::UpdateAnimableUI()
{
  application->UpdateTimeSettings();
  tool_bar_sim->Update();
  ui_animation_scheme_window->UpdateList();
}

void UIApplicationMainFrame::UpdateAUIManager()
{
  m_mgr.Update();
}

void UIApplicationMainFrame::StopTimeAndControls()
{
  tool_bar_sim->Stop();
}

