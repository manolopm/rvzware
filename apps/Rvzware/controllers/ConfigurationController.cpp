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

#include <string>
#include <iostream>
#include <sstream>
#include <fstream>


#include <stdafx.h>

#include <controllers/ConfigurationController.h>

using namespace cpw::controllers;


ConfigurationController::ConfigurationController()
{		
}

ConfigurationController::~ConfigurationController()
{
}

void ConfigurationController::Load(cpw::ApplicationConfiguration *appconfig) 
{
	std::string configfilename = appconfig->GetRootDirectory() + "/config.ini";
	
	std::ifstream config_file(configfilename.c_str());
	
	if (config_file.is_open())
	{
		while (!config_file.eof())
		{
			char str[5000];		
			const int size = 5000;

			config_file.getline(str, size);

			const std::string tag(str);

			if(tag == "[APPLICATIONLOGO]") {
				config_file.getline(str, size);
				appconfig->SetApplicationLogo(str);
			}
			if(tag == "[UIICONPATH]") {
				config_file.getline(str, size);
				appconfig->SetUIIconDirectory(str);
			}
			if(tag == "[SCENE]") {
				config_file.getline(str, size);
				appconfig->SetSceneFile(str);
			}

			if(tag == "[PRIMITIVEPATH]") {
				config_file.getline(str, size);
				appconfig->SetPrimitiveDirectory(str);
			}

			if(tag == "[ENTITYPATH]") {
				config_file.getline(str, size);
				appconfig->SetEntityDirectory(str);
			}

			if(tag == "[DEFAULTPATH]") {
				config_file.getline(str, size);
				appconfig->SetDefaultDirectory(str);
			}

			if(tag == "[DATAPATH]") {
				config_file.getline(str, size);
				appconfig->SetDataDirectory(str);
			}
			
			if(tag == "[LANGUAGE]") {
				config_file.getline(str, size);
				appconfig->SetLanguage(str);
			}

			if(tag == "[MODEL]") {
				config_file.getline(str, size);
				appconfig->SetModelDirectory(str);
			}

			if(tag == "[FIRETEXTURE]") {
				config_file.getline(str, size);
				appconfig->SetFireTexture(str);
			}

			if(tag == "[SMOKETEXTURE]") {
				config_file.getline(str, size);
				appconfig->SetSmokeTexture(str);
			}

			if(tag == "[WMSSERVERLIST]") {
				config_file.getline(str, size);
				appconfig->SetWmsServerList(str);
			}

			if(tag == "[PLUGINSFILE]") {
				config_file.getline(str, size);
				appconfig->SetPluginsFile(str);
			}

			//if(tag == "[UICONFIG]") {
			//	config_file.getline(str, size);
			//	appconfig->SetUIConfig(str);
			//}

			if (tag == "[LISTENPORT]") {
				int port;
				config_file >> port;
				appconfig->SetListenPort(port);
			}

			if(tag == "[THEMED]") {
				bool themed;
				config_file >> themed;
				appconfig->SetThemed(themed);
			}
			if(tag == "[ICONPATH]") {
				config_file.getline(str, size);
				appconfig->SetIconDirectory(str);
			}
			if(tag == "[ICON_SIZE]") {
				config_file.getline(str, size);
				appconfig->SetIconSize(str);
			}
			if(tag == "[BIG_ICON_SIZE]") {
				config_file.getline(str, size);
				appconfig->SetBigIconSize(str);
			}
			if(tag == "[BACKGROUND_COLOUR]") {
				config_file.getline(str, size);
				appconfig->SetBackgroundColour(str);
			}
			if(tag == "[FOREGROUND_COLOUR]") {
				config_file.getline(str, size);
				appconfig->SetForegroundColour(str);
			}
			if(tag == "[TITLEBAR_GRADIENT_1_COLOUR]") {
				config_file.getline(str, size);
				appconfig->SetTitleBarGradient1Colour(str);
			}
			if(tag == "[TITLEBAR_GRADIENT_2_COLOUR]") {
				config_file.getline(str, size);
				appconfig->SetTitleBarGradient2Colour(str);
			}
			if(tag == "[BACKGROUND_GRADIENT_1_COLOUR]") {
				config_file.getline(str, size);
				appconfig->SetBackgroundGradient1Colour(str);
			}
			if(tag == "[BACKGROUND_GRADIENT_2_COLOUR]") {
				config_file.getline(str, size);
				appconfig->SetBackgroundGradient2Colour(str);
			}
			if(tag == "[FONT_LIGHT_COLOUR]") {
				config_file.getline(str, size);
				appconfig->SetFontLightColour(str);
			}
			if(tag == "[FONT_DARK_COLOUR]") {
				config_file.getline(str, size);
				appconfig->SetFontDarkColour(str);
			}
			if(tag == "[PAGE_COLOUR]") {
				config_file.getline(str, size);
				appconfig->SetPageColour(str);
			}
			if(tag == "[PAGE_FONT_COLOUR]") {
				config_file.getline(str, size);
				appconfig->SetPageFontColour(str);
			}

			if(tag == "[LOG_WINDOW]") {
				config_file.getline(str,size);
				appconfig->SetUIConfiguration(str);
			}
			
		}
		
		appconfig->Saved(); //this is to remove the modified flag and avoid to overwrite the same data
		
		config_file.close();
	}
	else
	{
		appconfig->SetApplicationLogo("resources/logo.osg");

		appconfig->SetUIIconDirectory("resources/icons/");

		appconfig->SetSceneFile("");

		appconfig->SetPrimitiveDirectory("../../data/primitive/");

		appconfig->SetEntityDirectory("../../data/entity/");

		appconfig->SetDefaultDirectory("../../data/default/");

		appconfig->SetDataDirectory("../../data/");
		
		appconfig->SetLanguage("[SP]");

		appconfig->SetModelDirectory("../../data/models");

		appconfig->SetFireTexture("../../data/default/fire.tif");

		appconfig->SetSmokeTexture("../../data/default/smoke.tif");

		appconfig->SetWmsServerList("../../data/wmsserverlist.dat");

		appconfig->SetPluginsFile("pluginsfile.dat");

		appconfig->SetListenPort(3000);

		appconfig->SetThemed(false);

		appconfig->SetIconDirectory("../../data/icons/");

		appconfig->SetIconSize("16");

		appconfig->SetBigIconSize("18");

		appconfig->SetBackgroundColour("RGB(20,20,20)");

		appconfig->SetForegroundColour("RGB(250,250,255)");

		appconfig->SetTitleBarGradient1Colour("RGB(145,145,160)");

		appconfig->SetTitleBarGradient2Colour("RGB(45,45,55)");

		appconfig->SetBackgroundGradient1Colour("RGB(160,160,175)");

		appconfig->SetBackgroundGradient2Colour("RGB(15,15,15)");

		appconfig->SetFontLightColour("RGB(255,250,250)");

		appconfig->SetFontDarkColour("RGB(25,25,10)");

		appconfig->SetPageColour("RGB(215,215,225)");

		appconfig->SetPageFontColour("RGB(85,85,90)");

		appconfig->SetUIConfiguration("");

		Save(appconfig);
			
	}
}


void ConfigurationController::Save(cpw::ApplicationConfiguration *appconfig)
{

	std::string configfilename = appconfig->GetRootDirectory() + "/config.ini";

	std::ofstream config_file(configfilename.c_str());

	if (config_file.is_open())
	{

		std::string tag;
		tag = "[APPLICATIONLOGO]";
		config_file << tag << std::endl;
		config_file << appconfig->GetApplicationLogo() << std::endl;

		tag = "[UIICONPATH]";
		config_file << tag << std::endl;
		config_file << appconfig->GetUIIconDirectory() << std::endl;

		tag = "[SCENE]";
		config_file << tag << std::endl;
		config_file << appconfig->GetSceneFile() << std::endl;

		tag = "[PRIMITIVEPATH]";
		config_file << tag << std::endl;
		config_file << appconfig->GetPrimitiveDirectory() << std::endl;

		tag = "[ENTITYPATH]";
		config_file << tag << std::endl;
		config_file << appconfig->GetEntityDirectory() << std::endl;

		tag = "[DEFAULTPATH]";
		config_file << tag << std::endl;
		config_file << appconfig->GetDefaultDirectory() << std::endl;

		tag = "[DATAPATH]";
		config_file << tag << std::endl;
		config_file << appconfig->GetDataDirectory() << std::endl;

		tag = "[LANGUAGE]";
		config_file << tag << std::endl;
		config_file << appconfig->GetLanguage() << std::endl;


		tag = "[FIRETEXTURE]";
		config_file << tag << std::endl;
		config_file << appconfig->GetFireTexture() << std::endl;

		tag = "[SMOKETEXTURE]";
		config_file << tag << std::endl;
		config_file << appconfig->GetSmokeTexture() << std::endl;

		tag = "[WMSSERVERLIST]";
		config_file << tag << std::endl;
		config_file << appconfig->GetWmsServerList() << std::endl;

		tag = "[PLUGINSFILE]";
		config_file << tag << std::endl;
		config_file << appconfig->GetPluginsFile() << std::endl;

		tag = "[LISTENPORT]";
		config_file << tag << std::endl;
		config_file << appconfig->GetListenPort() << std::endl;

		tag = "[MODEL]";
		config_file << tag << std::endl;
		config_file << appconfig->GetModelDirectory() << std::endl;
 
		tag = "[THEMED]";
		config_file << tag << std::endl;
		config_file << appconfig->IsThemed() << std::endl;
			
		tag = "[ICONPATH]";
		config_file << tag << std::endl;
		config_file << appconfig->GetIconDirectory() << std::endl;
		
		tag = "[ICON_SIZE]";
		config_file << tag << std::endl;
		config_file << appconfig->GetIconSize() << std::endl;
		
		tag = "[BIG_ICON_SIZE]";
		config_file << tag << std::endl;
		config_file << appconfig->GetBigIconSize() << std::endl;
		
		tag = "[BACKGROUND_COLOUR]";
		config_file << tag << std::endl;
		config_file << appconfig->GetBackgroundColour() << std::endl;
		
		tag = "[FOREGROUND_COLOUR]";
		config_file << tag << std::endl;
		config_file << appconfig->GetForegroundColour() << std::endl;
		
		tag = "[TITLEBAR_GRADIENT_1_COLOUR]";
		config_file << tag << std::endl;
		config_file << appconfig->GetTitleBarGradient1Colour() << std::endl;
		
		tag = "[TITLEBAR_GRADIENT_2_COLOUR]";
		config_file << tag << std::endl;
		config_file << appconfig->GetTitleBarGradient2Colour() << std::endl;
		
		tag = "[BACKGROUND_GRADIENT_1_COLOUR]";
		config_file << tag << std::endl;
		config_file << appconfig->GetBackgroundGradient1Colour() << std::endl;
		
		tag = "[BACKGROUND_GRADIENT_2_COLOUR]";
		config_file << tag << std::endl;
		config_file << appconfig->GetBackgroundGradient2Colour() << std::endl;
		
		tag = "[FONT_LIGHT_COLOUR]";
		config_file << tag << std::endl;
		config_file << appconfig->GetFontLightColour() << std::endl;
		
		tag = "[FONT_DARK_COLOUR]";
		config_file << tag << std::endl;
		config_file << appconfig->GetFontDarkColour() << std::endl;
		
		tag = "[PAGE_COLOUR]";
		config_file << tag << std::endl;
		config_file << appconfig->GetPageColour() << std::endl;
		
		tag = "[PAGE_FONT_COLOUR]";
		config_file << tag << std::endl;
		config_file << appconfig->GetPageFontColour() << std::endl;

		tag = "[LOG_WINDOW]";
		config_file << tag << std::endl;
		config_file << appconfig->GetUIConfiguration() << std::endl;

		appconfig->Saved(); 

		config_file.close();
	}

}
