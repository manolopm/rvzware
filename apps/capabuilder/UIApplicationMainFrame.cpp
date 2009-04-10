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
#define PANE_CONNECTIONTREE 2210
#define PANE_ELEMENT 2211
#define PANE_PROJECT 2212
#define PANE_TOOLS 2213
#define LAST_PANE 2213 //used as last pane

//Put debug panes here
#define PANE_LOG 2301

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

	EVT_MENU     ( SCENE_BUILDER, UIApplicationMainFrame::BuildScene )
	EVT_MENU     ( LOAD_SCENE_BUILDER, UIApplicationMainFrame::LoadBuildScene )

	

	EVT_CLOSE	 ( UIApplicationMainFrame::OnClose   )
	EVT_IDLE	 ( UIApplicationMainFrame::OnIdle    )
	EVT_SIZE     ( UIApplicationMainFrame::OnSize    )
	EVT_KEY_UP   ( UIApplicationMainFrame::OnKeyUp   )
	EVT_KEY_DOWN ( UIApplicationMainFrame::OnKeyDown )
	EVT_MOUSE_EVENTS ( UIApplicationMainFrame::OnMouse )
	EVT_NOTEBOOK_PAGE_CHANGED ( NOTEBOOK_ID, UIApplicationMainFrame::OnNoteBookPageChanged )

	//EVT_AUI_PANE_CLOSE(UIApplicationMainFrame::OnPaneClose) 
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
		wxColour backgroundColor = cpw::ApplicationConfiguration::GetInstance()->GetBackgroundColour();
		wxColour inactiveColor   = cpw::ApplicationConfiguration::GetInstance()->GetTitleBarGradient1Colour();
		wxColour gradientColor2  = cpw::ApplicationConfiguration::GetInstance()->GetTitleBarGradient2Colour();
		wxColour fontColor       = cpw::ApplicationConfiguration::GetInstance()->GetFontLightColour();
		
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

   
	load_splash = new UILoadSplash(this, wxID_ANY, "Capabuilder", "resources/application_splash.png");
	//load_splash->Show(true);
	InitMenus();
	InitGUI();
    m_mgr.Update();
	//this->Show(true);
	//load_splash->Show(false);
	////pasar a application
}

void UIApplicationMainFrame::InitShortCuts()
{
}

std::map<int, std::string> UIApplicationMainFrame::menus = std::map<int, std::string>();

void UIApplicationMainFrame::InitMenus()
{
	
	RegisterMenu(FILE_EXIT, "Exit");
	RegisterMenu(HELP, "Help");

	RegisterMenu(SCENE_BUILDER, "Build Scene");
	RegisterMenu(LOAD_SCENE_BUILDER, "Load Project");

}

void UIApplicationMainFrame::OnPaneMove(wxAuiManagerEvent& event)
{
	Refresh();
}
void UIApplicationMainFrame::OnPaneClose(wxAuiManagerEvent& event)
{
	
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
		wxMenuItem *menu_item = new wxMenuItem(parent, i->first, wxString(_T(i->second.c_str())), helpString, wxITEM_NORMAL, subMenu);
		if (bitmapUrl!="")
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
	OnSize(wxSizeEvent()); 
	wxFrame::Update(); 
	//ui_layer_tree_tb->Update();
	//ui_entity_properties_grid->Update(); 
	//ui_layer_tree->Update();
	//ui_animation_scheme_window->Update();
	//connection_frame->Update();
}

void UIApplicationMainFrame::InitGUI () {

	cpw::ApplicationConfiguration *app_config = cpw::ApplicationConfiguration::GetInstance();
	std::string &icon_path = app_config->GetUIIconDirectory();

	int ic_size = cpw::ApplicationConfiguration::GetInstance()->GetIconSize();
	int bic_size = cpw::ApplicationConfiguration::GetInstance()->GetBigIconSize();

	this->SetIcon(wxIcon(_T(icon_path + "geviemer3.ico"), wxBITMAP_TYPE_ICO));
	this->SetMinSize(wxSize(800,600));
	
	wxMenuBar *menu_bar = new wxMenuBar();

	
	//Scene builder menu
	scene_builder_menu = new wxMenu;
	scene_builder_menu->Append(GetMenuItem(SCENE_BUILDER, scene_builder_menu, _T("Build Scene"), _T(icon_path + "brick.png")));
	scene_builder_menu->Append(GetMenuItem(LOAD_SCENE_BUILDER, scene_builder_menu, _T("Load Project"), _T(icon_path + "brick_import.png")));
	scene_builder_menu->AppendSeparator();
	scene_builder_menu->Append(GetMenuItem(FILE_EXIT, scene_builder_menu,_T(""), _T(icon_path + "cross.png")));
	menu_bar->Append(scene_builder_menu, _T("Build Scene"));


	//plugins menu
	//plugins_menu = new wxMenu;
	//menu_bar->Append(plugins_menu, _T("Plugins"));

	//Help menu
	wxMenu *help_menu = new wxMenu;
	help_menu->Append(GetMenuItem(HELP, help_menu,_T(""), _T(icon_path + "help.png")));
	menu_bar->Append(help_menu, _T("Help"));

	//////ToolBars

	SetMenuBar(menu_bar);
	menu_bar->Refresh();

	//RegisterMenu(EDIT_SELECT_ALL, "Select All\tCtrl+A");
	//RegisterMenu(EDIT_FIND, "Find\tCtrl+F");

	CreateStatusBar();
	SetStatusText( _T(" ") );
}

void UIApplicationMainFrame::InitGUIContents(cpw::LayerTree &layer_tree, cpw::remote::ConnectionManager *connection_manager,
	cpw::INavigatorManager *navigator_manager)
{
	std::string &icon_path = ApplicationConfiguration::GetInstance()->GetUIIconDirectory();	
	int width, height;
	GetClientSize(&width, &height);

	int ic_size = cpw::ApplicationConfiguration::GetInstance()->GetIconSize();
	int bic_size = cpw::ApplicationConfiguration::GetInstance()->GetBigIconSize();

	//tool_bar_sun->Update_SunPos();

	//Help Window
	help_window = new UIHelp(this, wxID_ANY, "Capabuilder", "resources/application_help.png", "http://www.capaware.org", "CapaBuilder 2008", "Version 0.0.8");
	help_window->Show(false);

	
	//log_frame
	log_frame = new wxMiniFrame(this, PANE_LOG, _T("Log"), wxDefaultPosition, wxDefaultSize, wxBORDER_NONE | wxTRANSPARENT_WINDOW ,_T("Log"));
	ui_log_window = new UILogWindow(log_frame);
	log_frame->SetSize(wxSize(200, 200));
	cpw::ApplicationLog::GetInstance()->SetLogger((cpw::Logger *) ui_log_window);

	

	////scene tree frame
	//scene_tree_frame = new wxMiniFrame(this, PANE_LOG, _T("Scene tree"), wxDefaultPosition, wxDefaultSize, wxBORDER_NONE | wxTRANSPARENT_WINDOW,_T("Properties"));;
	//ui_scene_tree = new UISceneTree(scene_tree_frame);
	//scene_tree_frame->SetSize(wxSize(200, 200));

	//splitter canvas
	splitter = new wxSplitterWindow(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSP_3D | wxSP_LIVE_UPDATE);
	viewport_manager = new cpw::gui::UISplitterCanvas(splitter, navigator_manager);
	navigator_manager_ = navigator_manager;
	//navigator_manager->AddNavigator(this, 0);
	splitter->SetFocus();
	splitter->SetSize(wxSize(width, height));
	splitter->SetSashGravity(0.1);
	splitter->SetMinimumPaneSize(240);

	viewport_field = viewport_manager->PresetOnlyView(wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSP_3D );
	
	splitter->Initialize(viewport_field);
	

	//wxAUI 

	m_mgr.AddPane(splitter, wxCENTRE, wxT("Viewport"));
	m_mgr.AddPane(log_frame, wxLEFT, wxT("Log")); 
	m_mgr.GetPane(splitter).MinSize(wxSize(240,240)).Name(NAME_SPLITTER);
	//PJ//m_mgr.GetPane(log_frame).MinSize(wxSize(100, 100)).Name(NAME_LOG_FRAME); 
	m_mgr.GetPane(log_frame).MinSize(wxSize(100, 100)).Name(NAME_LOG_FRAME).Float().CloseButton(false).Dockable(false); 
	
	
	m_mgr.Update();


	//trick
	
	m_mgr.GetPane(splitter).Float().Hide(); 
	m_mgr.Update();
	m_mgr.GetPane(splitter).Dock().Show(); 


	
	
	m_mgr.Update();
	//m_mgr.GetPane(layer_frame).Dock().Show();
	//m_mgr.GetPane(properties_frame).Dock().Show();
	//m_mgr.GetPane(animation_scheme_frame).Dock().Show();
	//m_mgr.GetPane(animation_scheme_frame).Dock().Hide();
	
		//PJ//m_mgr.GetPane(log_frame).Dock().Show(); 
		//ui_log_window->NewLogMessage("Log");	
		//m_mgr.GetPane(splitter).Dock().Show(); 

	//if (SHOW_ADDITIONAL_WINDOWS)
	//{

	//	//m_mgr.GetPane(scene_tree_frame).Dock().Show(); 
	//	//ui_log_window->NewLogMessage("debug log");	

	//	//m_mgr.GetPane(connection_frame).Dock().Show();
	//}
	//else
	//	m_mgr.GetPane(connection_frame).Dock().Hide();		
	//
	//PJ//m_mgr.GetPane(log_frame).Float();
    m_mgr.Update();
	
	OnSize(wxSizeEvent());
	
	//ui_default_config = m_mgr.SavePerspective();
	//wxString gui_config = ApplicationConfiguration::GetInstance()->GetUIConfiguration();
	//if (!gui_config.empty())
	//	m_mgr.LoadPerspective(gui_config);
	m_mgr.Update();

	CheckVisibility();

	//InitShortCuts();
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

	wxMessageDialog message(NULL,wxString("This option is not available yet."), wxString("Warning"),wxICON_EXCLAMATION |wxOK);
	message.ShowModal();

}

void UIApplicationMainFrame::AnimateEntity(wxCommandEvent& WXUNUSED(event))
{
	application->AnimateEntity();
	wxMessageDialog message(NULL,wxString("This option is not available yet."), wxString("Warning"),wxICON_EXCLAMATION |wxOK);
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
	std::string &icon_path = ApplicationConfiguration::GetInstance()->GetUIIconDirectory();	
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
	std::string &icon_path = ApplicationConfiguration::GetInstance()->GetUIIconDirectory();	
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
	std::string &icon_path = ApplicationConfiguration::GetInstance()->GetUIIconDirectory();	
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
	std::string &icon_path = ApplicationConfiguration::GetInstance()->GetUIIconDirectory();

	
	RegisterMenu(new_id, new_plugin);
	wxMenuItem * menu_item = GetMenuItem(new_id, plugins_menu, _T("Ctrl+P"), _T(icon_path + "add.png"));
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
	ui_layer_tree->CopySelection();  
}

void UIApplicationMainFrame::Copy(const cpw::TypeId &id)
{
	application->Copy(id);
}

void UIApplicationMainFrame::Duplicate(const cpw::TypeId &id)
{
	application->Duplicate(id);
}
			
void UIApplicationMainFrame::EditCut(wxCommandEvent& WXUNUSED(event))
{
	ui_layer_tree->CutSelection();   
}

void UIApplicationMainFrame::Cut(const cpw::TypeId &id)
{
	application->Cut(id);
}
			
void UIApplicationMainFrame::EditPaste(wxCommandEvent& WXUNUSED(event))
{
	ui_layer_tree->PasteSelection();   
}

void UIApplicationMainFrame::Paste(const cpw::TypeId &id_orig, const cpw::TypeId &id_dest, const cpw::TypeId &parent_id_orig, const cpw::TypeId &parent_id_dest, const bool &cut, const bool &copy, const bool &duplicate)
{
	application->Paste(id_orig, id_dest, parent_id_orig, parent_id_dest, cut, copy, duplicate);
}
			
void UIApplicationMainFrame::EditDelete(wxCommandEvent& WXUNUSED(event))
{
	ui_layer_tree->DeleteSelection();   
}

void UIApplicationMainFrame::Delete(const cpw::TypeId &ent_id, const cpw::TypeId &parent_id)
{
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
//	return alphaSlider->GetValue();
	return 0;
}

void UIApplicationMainFrame::SetWMSAlphaValue(const int &value)
{
//	alphaSlider->SetValue(value);
}

void UIApplicationMainFrame::SetSunPosition(const float &pos)
{
//	application->SetSunPosition(pos);
}

int UIApplicationMainFrame::GetFireViewMode()
{
//	return tool_bar_sim->GetFireViewMode();
	return 0;
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
	return (std::string) m_mgr.SavePerspective();
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
	/*std::string &icon_path = ApplicationConfiguration::GetInstance()->GetUIIconDirectory();	
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
	m_mgr.Update();*/
}

//void UIApplicationMainFrame::DeleteLayerTreePanel()
//{
//	ui_layer_tree->Clear();
//}

void UIApplicationMainFrame::EnableGUI(const bool &value)
{

	UpdateAnimableUI();
}

void UIApplicationMainFrame::UpdateAnimableUI()
{
	
}

void UIApplicationMainFrame::UpdateAUIManager()
{
	m_mgr.Update();
}


void UIApplicationMainFrame::BuildScene(wxCommandEvent& WXUNUSED(event))
{
	application->BuildScene();

}

void UIApplicationMainFrame::LoadBuildScene(wxCommandEvent& WXUNUSED(event))
{
	application->LoadBuildScene();
}

void UIApplicationMainFrame::GetWCS() 
{
	application->GetWCS();
}

