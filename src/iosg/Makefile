LIB_FILES =  OsgCamera.cpp OsgIconSizeUpdaterVisitor.cpp OsgNavigator.cpp OsgNavigatorWxGLCanvas.cpp OsgOverlayImage.cpp OsgSceneLoader.cpp OsgVisitorFirstGeode.cpp OsgDrawableCullCallback.cpp OsgImgMixer.cpp OsgNavigatorManager.cpp OsgNodeFinderVisitor.cpp OsgPetitionManager.cpp  OsgSenoidalInterpolator.cpp stdafx.cpp OsgDrawableData.cpp OsgIntersectorHeightCalculator.cpp OsgNavigatorPostDrawCallback.cpp  OsgObjectLoader.cpp OsgPSCallback.cpp OsgSun.cpp OsgFactory.cpp OsgLine2Callback.cpp OsgNavigatorPreDrawCallback.cpp OsgObjectRegistry.cpp OsgSceneBuilder.cpp OsgValidator.cpp OsgHUDWidget.cpp OsgLineCallback.cpp OsgNavigatorWxFrame.cpp OsgObjectRegistrySingleton.cpp  OsgScene.cpp OsgVisitor.cpp gui/OsgArrow.cpp gui/OsgCompass.cpp gui/OsgDummy.cpp gui/OsgHUD.cpp gui/OsgLabel.cpp gui/OsgNavigationPanel.cpp gui/OsgUTMLabel.cpp gui/OsgWidget.cpp gui/OsgButton.cpp gui/OsgContainer.cpp gui/OsgHUDController.cpp  gui/OsgImage.cpp gui/OsgMinimap.cpp  gui/OsgPanel.cpp gui/OsgVerticalButtonPanel.cpp


INCLUDE = -I/usr/include -I../../include -I/usr/include/postgresql/ -I/usr -I/usr/lib/wx/include/gtk2-unicode-release-2.8 -I/usr/include/wx-2.8 -D_FILE_OFFSET_BITS=64 -D_LARGE_FILES -D__WXGTK__ 

LFLAGS = -fPIC -lcurl -lpq -lOpenThreads -lboost_filesystem -lexpat -losg
CFLAGS = -fPIC -g
CC = g++
LIBOFILES		= $(LIB_FILES:%.cpp=%.o)

all: libiosg.so

clean:
	rm -f $(LIBOFILES)

%.o: %.cpp
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

libiosg.so: $(LIBOFILES)
	rm -f $@
	g++ -shared -Wl,-soname, $(LIBOFILES) -o $@






