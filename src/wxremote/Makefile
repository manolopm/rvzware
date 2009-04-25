LIB_FILES = stdafx.cpp  WxClientSocketThread.cpp  WxResolver.cpp  WxSocket.cpp  WxSocketEvent.cpp  WxSocketFactory.cpp  WxSocketHandler.cpp


INCLUDE = -I/usr/include -I../../include -I/usr/include/postgresql/ -I/usr -I/usr/lib/wx/include/gtk2-unicode-release-2.8 -I/usr/include/wx-2.8 -D_FILE_OFFSET_BITS=64 -D_LARGE_FILES -D__WXGTK__

LFLAGS = -fPIC -lcurl -lpq -lOpenThreads -lboost_filesystem -lexpat 
CFLAGS = -fPIC
CC = g++
LIBOFILES		= $(LIB_FILES:%.cpp=%.o)

all: libwxremote.so

clean:
	rm -f $(LIBOFILES) libwxremote.so

%.o: %.cpp
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

libwxremote.so: $(LIBOFILES)
	rm -f $@
	g++ -shared -Wl,-soname, $(LIBOFILES) -o $@






