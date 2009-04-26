SRC =  main.cpp Application.cpp  ApplicationTime.cpp  DynamicLibrary.cpp stdafx.cpp  UIApplicationMainFrame.cpp controllers/FireController.cpp controllers/PersistentController.cpp controllers/WCSServerConnectionController.cpp controllers/HandlerController.cpp controllers/ElementController.cpp controllers/ElementVectorialController.cpp controllers/DistanceController.cpp controllers/EntityController.cpp controllers/RemoteController.cpp controllers/LayerController.cpp controllers/ProjectController.cpp  controllers/ServerConnectionController.cpp controllers/VideoController.cpp controllers/LayerPrimitiveController.cpp controllers/ElementPrimitiveController.cpp controllers/DataBaseController.cpp controllers/KeyboardControl.cpp controllers/RemoteEntityController.cpp controllers/SunController.cpp controllers/ConfigurationController.cpp controllers/StatusController.cpp controllers/GraphicObjectController.cpp controllers/CwsLoader.cpp controllers/RelativeDirectory.cpp gui/MyTimeCtrl.cpp gui/UICalculateDistance.cpp gui/UIElement.cpp gui/UIFire.cpp gui/UILayerTreePanel.cpp gui/UIProgressionModalDialog.cpp gui/UISceneTree.cpp gui/UISunConfiguration.cpp gui/UIElementPrimitive.cpp gui/UIHelp.cpp gui/UILoadSplash.cpp gui/UIRemoteConnection.cpp gui/UIServerConnection.cpp gui/UIVideoController.cpp gui/UIAnimableList.cpp gui/UIConnectionTreePanel.cpp gui/UIElementVectorial.cpp gui/UILayer.cpp gui/UILogWindow.cpp gui/UIRemoteImportation.cpp gui/UISplitterCanvas.cpp gui/UIWCSServerConnection.cpp gui/UIAttributes.cpp gui/UIEAttributes.cpp gui/UIEntityPropertiesGrid.cpp gui/UILayerPrimitive.cpp gui/UIMovieScheme.cpp gui/UIRemotePublished.cpp gui/UIStatusBar.cpp gui/treelistctrl.cpp 


INCLUDE = -I/usr/include/postgresql -I/usr/lib/wx/include/gtk2-unicode-release-2.8 -I/usr/include/wx-2.8 -I ../../include/ -I . -I ../../src
LFLAGS = -ldl -lOpenThreads -lwx_gtk2u_richtext-2.8 -lwx_gtk2u_aui-2.8 -lwx_gtk2u_xrc-2.8 -lwx_gtk2u_qa-2.8 -lwx_gtk2u_html-2.8 -lwx_gtk2u_adv-2.8 -lwx_gtk2u_core-2.8 -lwx_baseu_xml-2.8 -lcpw  -lwx_baseu_net-2.8 -lwx_baseu-2.8 -lcpwutils -L../../src/cpw -L../../src/cpwutils -lboost_filesystem  -lwms -logc -L../../src/remote -lremote  -lwxremote -liosg -L../../src/ogcwcs -logc_wcs  -lwcs -losg -lcurl -losgGA -losgDB -losgText -losgParticle -losgViewer -lpq -lwx_gtk2u_gl-2.8
OFILES = $(SRC:%.cpp=%.o)
CFLAGS = -rdynamic -pthread -Wl,-Bsymbolic-functions -D_FILE_OFFSET_BITS=64 -D_LARGE_FILES -D__WXGTK__ -pthread -g

CC = g++

all: Rvzware

clean:
	rm -f Rvzware
	rm *.o

mrpropper:
	rm $(OFILES)
	make clean

%.o: %.cpp
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

Rvzware: $(OFILES)
	$(CC) $(INCLUDE) $(LFLAGS) $(OFILES) -o $@


