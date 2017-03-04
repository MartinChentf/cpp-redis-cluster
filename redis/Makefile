# Redis Makefile

SOURCE=$(wildcard *.cpp)
OBJECT=$(patsubst %.cpp, %.o, $(SOURCE))

all: $(OBJECT)

.cpp.o:
	$(CXX) -c $< $(CPPFLAGS) $(HIREDIS_INCS) $(HIREDIS_LIBS)

.PHONY: clean

clean:
	@$(RM) -v $(OBJECT)

