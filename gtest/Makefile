# gtest Makefile

SOURCE=$(wildcard *.cpp)
OBJECT=$(patsubst %.cpp, %.o, $(SOURCE))

all: $(OBJECT)

.cpp.o:
	$(CXX) -c $(CPPFLAGS) $(GTEST_LIBS) $(HIREDIS_INCS) $<

.PHONY: all clean

clean:
	@$(RM) -v $(OBJECT)

