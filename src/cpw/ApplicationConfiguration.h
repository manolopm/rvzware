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

#ifndef _APPLICATIONCONFIGURATION_
#define _APPLICATIONCONFIGURATION_

#include <OpenThreads/Mutex>
#include <string>
#include <cpw/Export.h>


namespace cpw
{

	/** 
		\brief Contains the configuration of the application. 
		The information for this class is read from the config.ini file and other configuration files.
		
		\ingroup cpw
	*/
	class CPWEXPORT ApplicationConfiguration
	{
	public:
		static ApplicationConfiguration * GetInstance();
		static void ReleaseInstance();	

		virtual void SetRootDirectory(const std::string &primitivepath) { root_path = primitivepath;}
		virtual void SetPrimitiveDirectory(const std::string &primitivepath) { primitive_path = primitivepath; Modified(); }
		virtual void SetEntityDirectory(const std::string &entitypath) { entity_path = entitypath; Modified(); }
		virtual void SetDefaultDirectory(const std::string &defaultpath) { default_path = defaultpath; Modified(); }
		virtual void SetIconDirectory(const std::string &iconpath) { icon_path = iconpath; Modified(); }
		virtual void SetUIIconDirectory(const std::string &iconpath) { ui_icon_path = iconpath; Modified(); }
		virtual void SetDataDirectory(const std::string &datapath) { data_path = datapath; Modified(); }
		virtual void SetLanguage(const std::string &lang) { language = lang; Modified(); }
		virtual void SetModelDirectory(const std::string &modelpath) { model_path = modelpath; Modified(); }
		virtual void SetTopLayer(const std::string &toplayer) { top_layer = toplayer; Modified(); }
		virtual void SetSceneFile(const std::string &sceneFile) { scene_file = sceneFile; Modified(); }
		virtual void SetFireTexture(const std::string &firetexture) { fire_texture = firetexture; Modified(); }
		virtual void SetSmokeTexture(const std::string &smoketexture) { smoke_texture = smoketexture; Modified(); }
		virtual void SetWmsServerList(const std::string &wmsfile) { wms_server_list = wmsfile; Modified(); }
		virtual void SetPluginsFile(const std::string &pluginsfile) { plugins_file = pluginsfile; Modified(); }
		virtual void SetUIConfig(const std::string &file) { uiconfig_file = file; Modified(); }
		virtual void SetListenPort(int port) { listen_port = port; Modified(); }
		virtual void SetApplicationLogo(const std::string &logo) { application_logo = logo; Modified(); }

		virtual std::string &GetRootDirectory() { return root_path; }
		virtual std::string &GetPrimitiveDirectory() { return primitive_path; }
		virtual std::string &GetEntityDirectory() { return entity_path; }
		virtual std::string &GetDefaultDirectory() { return default_path; }
		virtual std::string &GetIconDirectory() { return icon_path; }
		virtual std::string &GetUIIconDirectory() { return ui_icon_path; }
		virtual std::string &GetDataDirectory() { return data_path; }
		virtual std::string &GetLanguage() { return language; }
		virtual std::string &GetModelDirectory() { return model_path; }
		virtual std::string &GetTopLayer()  { return top_layer; }
		virtual std::string &GetSceneFile() { return scene_file; }
		virtual std::string &GetFireTexture() { return fire_texture; }
		virtual std::string &GetSmokeTexture() { return smoke_texture; }
		virtual std::string &GetWmsServerList() { return wms_server_list; }
		virtual std::string &GetWcsServerList() { return wms_server_list; }
		virtual std::string &GetPluginsFile() { return plugins_file; }
		virtual std::string &GetUIConfig() { return uiconfig_file; }
		virtual int GetListenPort() { return listen_port; }
		virtual std::string &GetApplicationLogo() { return application_logo; }

		virtual const std::string &GetRootDirectory() const { return root_path; }
		virtual const std::string &GetPrimitiveDirectory() const { return primitive_path; }
		virtual const std::string &GetEntityDirectory() const { return entity_path; }
		virtual const std::string &GetDefaultDirectory() const { return default_path; }
		virtual const std::string &GetIconDirectory() const { return icon_path; }
		virtual const std::string &GetUIIconDirectory() const { return ui_icon_path; }
		virtual const std::string &GetDataDirectory() const { return data_path; }
		virtual const std::string &GetLanguage() const { return language; }
		virtual const std::string &GetModelDirectory() const { return model_path; }
		virtual const std::string &GetTopLayer()  const { return top_layer; }
		virtual const std::string &GetSceneFile() const { return scene_file; }
		virtual const std::string &GetFireTexture() const { return fire_texture; }
		virtual const std::string &GetSmokeTexture() const { return smoke_texture; }
		virtual const std::string &GetWmsServerList() const { return wms_server_list; }
		virtual const std::string &GetWcsServerList() const { return wms_server_list; }
		virtual const std::string &GetPluginsFile() const { return plugins_file; }
		virtual const std::string &GetUIConfig() const { return uiconfig_file; }
		virtual const std::string &GetApplicationLogo() const { return application_logo; }


		// colours config
		void SetBackgroundColour(const std::string &rgb);
		void SetForegroundColour(const std::string &rgb);
		void SetTitleBarGradient1Colour(const std::string &rgb);
		void SetTitleBarGradient2Colour(const std::string &rgb);
		void SetBackgroundGradient1Colour(const std::string &rgb);
		void SetBackgroundGradient2Colour(const std::string &rgb);
		void SetFontLightColour(const std::string &rgb);
		void SetFontDarkColour(const std::string &rgb);
		void SetPageColour(const std::string &rgb);
		void SetPageFontColour(const std::string &rgb);
		void SetThemed(const bool &value) { is_themed = value; }
		void SetIconSize(const std::string &is);
		void SetBigIconSize(const std::string &is);

		std::string &GetBackgroundColour()			 { return backgroundColour; }
		std::string &GetForegroundColour()	   		 { return foregroundColour; }
		std::string &GetTitleBarGradient1Colour()    { return titleBarGradient1Colour; }
		std::string &GetTitleBarGradient2Colour()    { return titleBarGradient2Colour; }
		std::string &GetBackgroundGradient1Colour()  { return backgroundGradient1Colour; }
		std::string &GetBackgroundGradient2Colour()  { return backgroundGradient2Colour; }
		std::string &GetFontLightColour()			 { return fontLightColour; }
		std::string &GetFontDarkColour()			 { return fontDarkColour; }
		std::string &GetPageFontColour()			 { return pageFontColour; }
		std::string &GetPageColour()				 { return pageColour; }
		
		bool &IsThemed()	   { return is_themed; }
		int  &GetIconSize()	   { return icon_size; }
		int  &GetBigIconSize() { return big_icon_size; }

		const std::string &GetBackgroundColour()		  const { return backgroundColour; }
		const std::string &GetForegroundColour()		  const { return foregroundColour; }
		const std::string &GetTitleBarGradient1Colour()   const { return titleBarGradient1Colour; }
		const std::string &GetTitleBarGradient2Colour()   const { return titleBarGradient2Colour; }
		const std::string &GetBackgroundGradient1Colour() const { return backgroundGradient1Colour; }
		const std::string &GetBackgroundGradient2Colour() const { return backgroundGradient2Colour; }
		const std::string &GetFontLightColour()		      const { return fontLightColour; }
		const std::string &GetFontDarkColour()			  const { return fontDarkColour; }
		const std::string &GetPageColour()				  const { return pageColour; }
		const std::string &GetPageFontColour()			  const { return pageFontColour; }

		const bool &IsThemed()		 const { return is_themed; }
		const int  &GetIconSize()	 const { return icon_size; }
		const int  &GetBigIconSize() const { return big_icon_size; }

		// windows config
		virtual void SetUIConfiguration(const std::string &p);

		virtual std::string &GetUIConfiguration() {return ui_config;}

		virtual const std::string &GetUIConfiguration() const {return ui_config;}


		virtual void Saved()      { modified = false; }
		virtual bool isModified() { return modified; }


	protected:

		virtual void Modified() { modified = true; }

	private:
		ApplicationConfiguration();
		~ApplicationConfiguration();

		static ApplicationConfiguration *instance;
		static OpenThreads::Mutex appmutex;

		std::string root_path;
		std::string primitive_path;
		std::string entity_path;
		std::string default_path;
		std::string icon_path;
		std::string ui_icon_path;
		std::string data_path;
		std::string language;
		std::string model_path;
		std::string top_layer;
		std::string scene_file;
		std::string application_logo;

		std::string fire_texture;
		std::string smoke_texture;
		std::string wms_server_list;
		std::string plugins_file;
		std::string uiconfig_file;

		int listen_port;

		bool modified;

		int icon_size;
		int big_icon_size;

		bool is_themed;

		std::string backgroundColour;
		std::string foregroundColour;
		std::string titleBarGradient1Colour;
		std::string titleBarGradient2Colour;
		std::string backgroundGradient1Colour;
		std::string backgroundGradient2Colour;
		std::string fontLightColour;
		std::string fontDarkColour;
		std::string pageColour;
		std::string pageFontColour;

		std::string ui_config;

	};
}

#endif
