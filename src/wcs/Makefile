LIB_FILES = iso8601.cpp wcsCapabilitiesRoot.cpp wcsClient.cpp wcsCurlMemoryStream.cpp wcsNotify.cpp wcsStyle.cpp wcsCapabilitiesParser.cpp wcsCapabilitiesState.cpp wcs.cpp wcsMemoryStream.cpp wcsReferenced.cpp wcsUrl.cpp


INCLUDE = -I/usr/include -I../../include -I/usr/include/postgresql/ -I/usr -I../
LFLAGS = -fPIC -lcurl -lpq -lOpenThreads -lboost_filesystem -lexpat
CFLAGS = -fPIC
CC = g++
LIBOFILES		= $(LIB_FILES:%.cpp=%.o)

all: libwcs.so

clean:
	rm -f $(LIBOFILES) libwcs.so

%.o: %.cpp
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

libwcs.so: $(LIBOFILES)
	rm -f $@
	g++ -shared -Wl,-soname, $(LIBOFILES) -o $@

install:
	cp libwcs.so /usr/lib
	mkdir /usr/include/wcs
	cp *.h /usr/include/wcs/

uninstall:
	rm  /usr/lib/libwcs.so
	rm -rf /usr/include/wcs





