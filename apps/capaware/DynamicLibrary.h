#ifndef _DYNAMICLYBRARY_
#define _DYNAMICLYBRARY_

#include <string>
#include <wx/wx.h>
#include "PluginInterface.h"


	/** 
		\brief This class is used to load dynamic libraries (dll) into the application during the execution
	*/
	class DynamicLibrary
	{
	public:
		DynamicLibrary(){};
		~DynamicLibrary();
		void Load(const std::string &library);
#ifdef WIN32
		HINSTANCE GetH() {return h;}
#else
		void *GetH() {return h;}
#endif
		void EntitiesToRegister(std::vector<cpw::Entity*> &ventityregister);
		void RegisterPluginMenu(std::string &nmenu);
		void ExecutePlugin(std::vector<cpw::Entity*> &ventity, cpw::ApplicationScene *appscene, const std::string &entity_path, const std::string &icon_path, const std::string &data_path, const std::string &language, wxWindow *parent, cpw::INavigatorManager *navigator_manager, cpw::IStatusController *status_controller = NULL);
	private:
		std::string library_url;
#ifdef WIN32
		HINSTANCE h;
#else
		void *h; 
#endif
		
		
	};


#endif
