
INCLUDE= -I./Header \
        -I./Header/tools
OBJECTS=md5.o file_log.o md5c.o random.o header_debug.o tools.o utilities.o minilzo.o
SRCS=md5.cpp file_log.cpp md5c.cpp random.cpp header_debug.cpp tools.cpp utilities.cpp minilzo.c
TARGET=libshare.a

cc=g++

$(TARGET):$(OBJECTS)
		ar crv $(TARGET) $(OBJECTS) 

md5.o:md5.cpp 
	$(cc) -c $(INCLUDE) md5.cpp

file_log.o:file_log.cpp 
	$(cc) -c file_log.cpp $(INCLUDE)

md5c.o:md5c.c 
	$(cc) -c -x c md5c.c $(INCLUDE)

random.o:random.cpp 
	$(cc) -c random.cpp $(INCLUDE)

header_debug.o:header_debug.cpp 
	$(cc) -c header_debug.cpp $(INCLUDE)

tools.o:tools.cpp 
	$(cc) -c tools.cpp $(INCLUDE)

utilities.o:utilities.cpp 
	$(cc) -c utilities.cpp $(INCLUDE)
minilzo.o:minilzo.c 
	$(cc) -c minilzo.c $(INCLUDE)


clean:
	rm $(TARGET) $(OBJECTS)



