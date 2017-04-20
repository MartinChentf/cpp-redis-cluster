# Redis Makefile

SOURCE=$(wildcard *.cpp)
OBJECT=$(patsubst %.cpp, %.o, $(SOURCE))

all: $(OBJECT)

.cpp.o:
	$(CXX) -c $(CPPFLAGS) $(REDIS_INCS) $<

.PHONY: clean

clean:
	@$(RM) -v $(OBJECT)

