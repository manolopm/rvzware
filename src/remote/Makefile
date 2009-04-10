LIB_FILES = Connection.cpp  ConnectionManager.cpp  DisconnectEntityData.cpp  GetEntityRequestData.cpp   GetPublishedRequestData.cpp   MessageData.cpp     RemoteProtocol.cpp      SendChangesData.cpp  stdafx.cpp ConnectionData.cpp  DataStream.cpp         DisconnectionData.cpp     GetEntityResponseData.cpp  GetPublishedResponseData.cpp  RemoteModifier.cpp  RequestSynchroData.cpp  SetValueData.cpp     xmlParser.cpp


INCLUDE = -I/usr/include -I../../include -I/usr/include/postgresql/ -I/usr
LFLAGS = -fPIC -lcurl -lpq -lOpenThreads -lboost_filesystem -lexpat
CFLAGS = -fPIC
CC = g++
LIBOFILES		= $(LIB_FILES:%.cpp=%.o)

all: libremote.so

clean:
	rm -f $(LIBOFILES)

%.o: %.cpp
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

libremote.so: $(LIBOFILES)
	rm -f $@
	g++ -shared -Wl,-soname, $(LIBOFILES) -o $@






