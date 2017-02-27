FINAL_INC=-I/usr/local/include/hiredis
FINAL_LIBS=-lhiredis

OPTIMIZATION?=-O1
DEBUG?=-g -ggdb
WARNINGS=-Wall -W -Wwrite-strings
FINAL_CFLAGES= -std=c++11 $(OPTIMIZATION) $(DEBUG) $(WARNINGS)

OBJECT=redis_test.o redis_client.o redis_string.o crc16.o

redis_test : $(OBJECT)
	g++ -o $@ $^ $(FINAL_CFLAGES) $(FINAL_LIBS) $(FINAL_INC)

.cpp.o:
	g++ -c $< $(FINAL_CFLAGES) $(FINAL_LIBS) $(FINAL_INC)

.PHONY : clean

clean :
	rm -f redis_test *.o *.gch core.*


