
INCLUDE= -I./header 
OBJECTS=Protocol.o
SRCS=Protocol.cpp
TARGET=libcommon.a

cc=g++

$(TARGET):$(OBJECTS)
		ar crv $(TARGET) $(OBJECTS) 

Protocol.o:Protocol.cpp 
	$(cc) -c Protocol.cpp $(INCLUDE)


clean:
	rm $(TARGET) $(OBJECTS)



