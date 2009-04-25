LIB_FILES = ApplicationConfiguration.cpp cpw.cpp ./persistent/Persistent.cpp ./persistent/file/FileManager.cpp ./persistent/file/PersistentFileCallBack.cpp ./persistent/file/PersistentFileManager.cpp ./persistent/file/XmlCpwParser.cpp ./persistent/file/xmlParser.cpp ./persistent/database/DataBase.cpp ./persistent/database/PersistentDBCallBack.cpp ./persistent/database/PersistentDBManager.cpp ./persistent/database/Table.cpp ./thread/RequestManager.cpp	./thread/RequestThread.cpp ./thread/ResourceManager.cpp	./graphic/AerialCameraController.cpp ./graphic/ApplicationScene.cpp ./graphic/CurveAdapter.cpp ./graphic/Gizmo.cpp ./graphic/IHandler.cpp ./graphic/RotationGizmo.cpp ./graphic/ScaleGizmo.cpp ./graphic/TraslationGizmo.cpp ./graphic/TwoAxisTraslationGizmo.cpp ./graphic/TwoDimensionCameraController.cpp ./callback/Callable.cpp ./callback/ContainerLayerCallBack.cpp ./callback/Model3DCallBack.cpp ./callback/ParticleSystemCallBack.cpp ./callback/RasterCallBack.cpp ./callback/VectorialCallBack.cpp ./common/ApplicationLog.cpp ./common/IdGenerator.cpp ./common/LatLong-UTMconversion.cpp ./common/Loggable.cpp ./common/Math.cpp ./common/Remote.cpp ./common/RemoteNode.cpp ./common/Sha1.cpp ./common/Sorted.cpp ./common/Subject.cpp ./common/TypeId.cpp ./entity/ContainerLayer.cpp ./entity/Element.cpp ./entity/Element3D.cpp ./entity/ElementParticleSystem.cpp ./entity/ElementRaster.cpp ./entity/ElementVectorial.cpp ./entity/Entity.cpp ./entity/EntityFactory.cpp ./entity/EntityRegistry.cpp ./entity/Layer.cpp ./entity/LayerTree.cpp ./entity/RasterLayer.cpp ./entity/VectorialLayer.cpp

INCLUDE = -I/usr/include -I../../include -I/usr/include/postgresql/
LFLAGS = -fPIC -lcurl -lpq -lOpenThreads -lboost_filesystem
CFLAGS = -fPIC
CC = g++
LIBOFILES		= $(LIB_FILES:%.cpp=%.o)

all: libcpw.so

clean:
	rm -f $(LIBOFILES) libcpw.so

%.o: %.cpp
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

libcpw.so: $(LIBOFILES)
	rm -f $@
	g++ -shared -Wl,-soname, $(LIBOFILES) -o $@






