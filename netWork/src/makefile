
WORKSPACE_DIR=../../
INCLUDE= -I./common \
        -I. \
        -I$(WORKSPACE_DIR)share/src \
        -I$(WORKSPACE_DIR)share/src/Header \
        -I$(WORKSPACE_DIR)share/src/Header/tools

OBJECTS=itcpclient.o itcpserver.o tcpclient.o tcpserver.o networkthread.o
CPP_SRCS=itcpclient.cpp itcpserver.cpp tcpclient.cpp tcpserver.cpp networkthread.cpp
TARGET=libnetwork.a
cc=g++-4.4

$(TARGET):$(OBJECTS)
	ar crv $(TARGET) $(OBJECTS) 
	
itcpclient.o:itcpclient.cpp
	$(cc) -c itcpclient.cpp $(INCLUDE)

itcpserver.o:itcpserver.cpp
	$(cc) -c itcpserver.cpp $(INCLUDE)

tcpclient.o:tcpclient.cpp
	$(cc) -c tcpclient.cpp $(INCLUDE)

tcpserver.o:tcpserver.cpp
	$(cc) -c tcpserver.cpp $(INCLUDE)

networkthread.o:networkthread.cpp
	$(cc) -c networkthread.cpp $(INCLUDE)

#	for cppfile in $(CPP_SRCS);do /
#$(subst .cpp,.o,$(cppfile)):$(cppfile)
#		echo $(cppfile) $(cc); /
#		g++-4.4 -c $$cppfile $(INCLUDE); /
#	done;

clean:
	rm $(TARGET) $(OBJECTS)



