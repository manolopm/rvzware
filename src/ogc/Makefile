LIB_FILES = ogc.cpp wms/WindowsDisk.cpp  wms/WmsDiskManager.cpp  wms/WmsLayerController.cpp  wms/WmsLayer.cpp  wms/WmsLayerManager.cpp  wms/WmsNetManager.cpp

INCLUDE = -I/usr/include -I../../include -I/usr/include/postgresql/ -I/usr
LFLAGS = -fPIC -lcurl -lpq -lOpenThreads -lboost_filesystem -lexpat
CFLAGS = -fPIC
CC = g++
LIBOFILES		= $(LIB_FILES:%.cpp=%.o)

all: libogc.so

clean:
	rm -f $(LIBOFILES) libogc.so

%.o: %.cpp
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

libogc.so: $(LIBOFILES)
	rm -f $@
	g++ -shared -Wl,-soname, $(LIBOFILES) -o $@





