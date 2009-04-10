SRC =  main.cpp Application.cpp  ApplicationTime.cpp  DynamicLibrary.cpp stdafx.cpp  UIApplicationMainFrame.cpp ../../src/controllers/FireController.cpp ../../src/controllers/PersistentController.cpp ../../src/controllers/WCSServerConnectionController.cpp ../../src/controllers/HandlerController.cpp ../../src/controllers/ElementController.cpp ../../src/controllers/ElementVectorialController.cpp ../../src/controllers/DistanceController.cpp ../../src/controllers/EntityController.cpp ../../src/controllers/RemoteController.cpp ../../src/controllers/LayerController.cpp ../../src/controllers/ProjectController.cpp  ../../src/controllers/ServerConnectionController.cpp ../../src/controllers/VideoController.cpp ../../src/controllers/LayerPrimitiveController.cpp ../../src/controllers/ElementPrimitiveController.cpp ../../src/controllers/DataBaseController.cpp ../../src/controllers/KeyboardControl.cpp ../../src/controllers/RemoteEntityController.cpp ../../src/controllers/SunController.cpp ../../src/controllers/ConfigurationController.cpp ../../src/controllers/StatusController.cpp ../../src/controllers/GraphicObjectController.cpp ../../src/controllers/CwsLoader.cpp ../../src/controllers/RelativeDirectory.cpp ../../src/gui/MyTimeCtrl.cpp ../../src/gui/UICalculateDistance.cpp ../../src/gui/UIElement.cpp ../../src/gui/UIFire.cpp ../../src/gui/UILayerTreePanel.cpp ../../src/gui/UIProgressionModalDialog.cpp ../../src/gui/UISceneTree.cpp ../../src/gui/UISunConfiguration.cpp ../../src/gui/UIElementPrimitive.cpp ../../src/gui/UIHelp.cpp ../../src/gui/UILoadSplash.cpp ../../src/gui/UIRemoteConnection.cpp ../../src/gui/UIServerConnection.cpp ../../src/gui/UIVideoController.cpp ../../src/gui/UIAnimableList.cpp ../../src/gui/UIConnectionTreePanel.cpp ../../src/gui/UIElementVectorial.cpp ../../src/gui/UILayer.cpp ../../src/gui/UILogWindow.cpp ../../src/gui/UIRemoteImportation.cpp ../../src/gui/UISplitterCanvas.cpp ../../src/gui/UIWCSServerConnection.cpp ../../src/gui/UIAttributes.cpp ../../src/gui/UIEAttributes.cpp ../../src/gui/UIEntityPropertiesGrid.cpp ../../src/gui/UILayerPrimitive.cpp ../../src/gui/UIMovieScheme.cpp ../../src/gui/UIRemotePublished.cpp ../../src/gui/UIStatusBar.cpp ../../src/gui/treelistctrl.cpp 

#../../src/gui/UICompass.cpp 


INCLUDE = -I/usr/include/postgresql -I/usr/lib/wx/include/gtk2-unicode-release-2.8 -I/usr/include/wx-2.8 -I ../../include/ -I .
LFLAGS = -ldl -lOpenThreads -lwx_gtk2u_richtext-2.8 -lwx_gtk2u_aui-2.8 -lwx_gtk2u_xrc-2.8 -lwx_gtk2u_qa-2.8 -lwx_gtk2u_html-2.8 -lwx_gtk2u_adv-2.8 -lwx_gtk2u_core-2.8 -lwx_baseu_xml-2.8 -lcpw  -lwx_baseu_net-2.8 -lwx_baseu-2.8 -lcpwutils -L../../src/cpw -L../../src/cpwutils -lboost_filesystem -L../../src/libwms -lwms -L../../src/ogc -logc -L../../src/remote -lremote -L../../src/wxremote -lwxremote -L../../src/iosg -liosg -L../../src/ogcwcs -logc_wcs -L../../src/libwcs -lwcs -losg -lcurl -losgGA -losgDB -losgText -losgParticle -losgViewer -lpq -lwx_gtk2u_gl-2.8
OFILES = $(SRC:%.cpp=%.o)
CFLAGS = -rdynamic -pthread -Wl,-Bsymbolic-functions -D_FILE_OFFSET_BITS=64 -D_LARGE_FILES -D__WXGTK__ -pthread

CC = g++

all: capaware

clean:
	rm -f capaware

%.o: %.cpp
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

capaware: $(OFILES)
	$(CC) $(INCLUDE) $(LFLAGS) $(OFILES) -o $@


