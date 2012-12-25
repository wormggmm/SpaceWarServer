
WORKSPACE_DIR=../../
INCLUDE= -I./common \
-I. \
-I./header \
-I./Map \
-I$(WORKSPACE_DIR)common/src/header \
-I$(WORKSPACE_DIR)netWork/src/common \
-I$(WORKSPACE_DIR)netWork/src \
-I$(WORKSPACE_DIR)serverCore/src \
-I$(WORKSPACE_DIR)serverCore/src/common \
-I$(WORKSPACE_DIR)serverCore/src/header \
-I$(WORKSPACE_DIR)share/src \
-I$(WORKSPACE_DIR)share/src/Header \
-I$(WORKSPACE_DIR)share/src/Header/tools

OBJECTS=main.o
CPP_SRCS=main.cpp
TARGET=sbServer
cc=g++-4.4
LIB_DIR=-L $(WORKSPACE_DIR)share/src \
		-L $(WORKSPACE_DIR)common/src \
		-L $(WORKSPACE_DIR)serverCore/src \
		-L $(WORKSPACE_DIR)netWork/src \

PROJECTS_DIR=$(WORKSPACE_DIR)share/src \
			$(WORKSPACE_DIR)common/src \
			$(WORKSPACE_DIR)serverCore/src \
			$(WORKSPACE_DIR)netWork/src \

LIBS=-lservercore -lnetwork -lshare -lcommon -lpthread

$(TARGET):$(OBJECTS)
	$(cc) -o $(TARGET) $(OBJECTS) $(LIBS) $(INCLUDE) $(LIB_DIR)
	
main.o:main.cpp
	$(cc) -c main.cpp $(INCLUDE)


#	for cppfile in $(CPP_SRCS);do /
#$(subst .cpp,.o,$(cppfile)):$(cppfile)
#		echo $(cppfile) $(cc); /
#		g++-4.4 -c $$cppfile $(INCLUDE); /
#	done;


all:
	@list='$(PROJECTS_DIR)'; \
	for subdir in $$list; do \
		(cd $$subdir && make); \
	done; \
	make;
	

clean:
	@list='$(PROJECTS_DIR)'; \
	for subdir in $$list; do \
		(cd $$subdir && make clean); \
	done; \
	rm $(TARGET) $(OBJECTS);
