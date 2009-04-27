LIB_FILES = cpwutils.cpp FireAnimation.cpp Fire.cpp GraphicFireCallBack.cpp

INCLUDE = -I/usr/include  -I/usr/include/postgresql/ -I../
LFLAGS = -fPIC -lcurl -lpq -lOpenThreads -lboost_filesystem
CFLAGS = -fPIC
CC = g++
LIBOFILES		= $(LIB_FILES:%.cpp=%.o)

all: libcpwutils.so

clean:
	rm -f $(LIBOFILES) libcpwutils.so

%.o: %.cpp
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

libcpwutils.so: $(LIBOFILES)
	rm -f $@
	g++ -shared -Wl,-soname, $(LIBOFILES) -o $@

install:
	cp libcpwutils.so /usr/lib
	mkdir /usr/include/cpwutils
	cp *.h /usr/include/cpwutils/

uninstall:
	rm  /usr/lib/libcpwutils.so
	rm -rf /usr/include/cpwutils




