FINAL_INC=-I/usr/local/include/hiredis
FINAL_LIBS=-lhiredis

OPTIMIZATION?=-O3
DEBUG?=-g -ggdb
WARNINGS=-Wall -W -Wwrite-strings
#CPP11=-std=c++11
FINAL_CFLAGES= $(CPP11) $(OPTIMIZATION) $(DEBUG) $(WARNINGS)

#OBJECT=redis_test.o redis_client.o redis_string.o redis_log.o redis_command.o crc16.o
#SOURCE= redis_test.cpp \
#        redis_client.cpp \
#        redis_string.cpp \
#        redis_log.cpp \
#        redis_command.cpp \
#        crc16.cpp

SOURCE=$(shell ls ./*.cpp)

OBJECT=$(patsubst %.cpp, %.o, $(SOURCE))

redis_test : $(OBJECT)
	g++ -o $@ $^ $(FINAL_CFLAGES) $(FINAL_LIBS) $(FINAL_INC)

.cpp.o:
	g++ -c $< $(FINAL_CFLAGES) $(FINAL_LIBS) $(FINAL_INC)

.PHONY : clean

clean :
	rm -f redis_test *.o *.gch core.*


