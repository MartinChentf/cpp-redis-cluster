FINAL_INC=-I/usr/local/include/hiredis
FINAL_LIBS=-lhiredis

OPTIMIZATION?=-O1
DEBUG?=-g -ggdb
WARNINGS=-Wall -W -Wwrite-strings
FINAL_CFLAGES= -std=c++11 $(OPTIMIZATION) $(DEBUG) $(WARNINGS)

redis_test : redis_client.o redis_string.o crc16.o
	g++ -o redis_test redis_test.cpp redis_client.o redis_string.o crc16.o $(FINAL_CFLAGES) $(FINAL_LIBS) $(FINAL_INC)

%.o: %.cpp
	g++ -c $< $(FINAL_CFLAGES) $(FINAL_LIBS) $(FINAL_INC)

.PHONY : clean

clean :
	rm -f redis_test *.o *.gch core.*


