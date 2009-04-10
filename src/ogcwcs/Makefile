LIB_FILES =  ogc_wcs.cpp wcs/WcsDiskManager.cpp  wcs/WcsLayerController.cpp wcs/WcsLayer.cpp  wcs/WcsLayerManager.cpp  wcs/WcsNetManager.cpp  wcs/WindowsDisk.cpp


INCLUDE = -I/usr/include -I../../include -I/usr/include/postgresql/ -I/usr
LFLAGS = -fPIC -lcurl -lpq -lOpenThreads -lboost_filesystem -lexpat
CFLAGS = -fPIC
CC = g++
LIBOFILES		= $(LIB_FILES:%.cpp=%.o)

all: libogc_wcs.so

clean:
	rm -f $(LIBOFILES)

%.o: %.cpp
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

libogc_wcs.so: $(LIBOFILES)
	rm -f $@
	g++ -shared -Wl,-soname, $(LIBOFILES) -o $@






