
WORKSPACE_DIR=../../
INCLUDE= -I./common \
-I. \
-I./header \
-I./Map \
-I$(WORKSPACE_DIR)common/src/header \
-I$(WORKSPACE_DIR)netWork/src/common \
-I$(WORKSPACE_DIR)netWork/src \
-I$(WORKSPACE_DIR)share/src \
-I$(WORKSPACE_DIR)share/src/Header \
-I$(WORKSPACE_DIR)share/src/Header/tools

OBJECTS=WGNetProtocolProcess.o WGMapManager.o Core.o WGMapNodeBase.o
CPP_SRCS=WGNetProtocolProcess.cpp WGMapManager.cpp Core.cpp WGMapNodeBase.cpp
TARGET=libservercore.a
cc=g++

LIB_DIR=-L $(WORKSPACE_DIR)share/src \
		-L $(WORKSPACE_DIR)common/src \
		-L $(WORKSPACE_DIR)serverCore/src \
		-L $(WORKSPACE_DIR)netWork/src \

LIBS=-lshare

$(TARGET):$(OBJECTS)
	ar crv $(TARGET) $(OBJECTS) 
	
WGNetProtocolProcess.o:WGNetProtocolProcess.cpp
	$(cc) -c WGNetProtocolProcess.cpp $(INCLUDE)

WGMapManager.o:Map/WGMapManager.cpp
	$(cc) -c Map/WGMapManager.cpp $(INCLUDE)

Core.o:Core.cpp
	$(cc) -c Core.cpp $(INCLUDE)

WGMapNodeBase.o:common/WGMapNodeBase.cpp
	$(cc) -c common/WGMapNodeBase.cpp $(INCLUDE)


#	for cppfile in $(CPP_SRCS);do /
#$(subst .cpp,.o,$(cppfile)):$(cppfile)
#		echo $(cppfile) $(cc); /
#		g++-4.4 -c $$cppfile $(INCLUDE); /
#	done;

clean:
	rm $(TARGET) $(OBJECTS)



