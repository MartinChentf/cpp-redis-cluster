# Redis Client Makefile
# Copyright (C) 2010-2011 chen tengfei <chentf1836@163.com>

# Compile Command
export CXX = g++
export LINK = g++

# Compile Option
OPTIMIZATION?=-O1
DEBUG?=-g -ggdb
WARNINGS=-Wall -W -Wwrite-strings
export CPPFLAGS= $(OPTIMIZATION) $(DEBUG) $(WARNINGS)

# Directory
export HOME = $(PWD)
export BIN_DIR = $(PWD)/bin
export EXAMPLE_DIR = $(PWD)/example
export REDIS_DIR = $(PWD)/redis
export GTEST_DIR = $(PWD)/Gtest

# include and library
export HIREDIS_INCS = -I/usr/local/include/hiredis -I$(REDIS_DIR)
export HIREDIS_LIBS = -lhiredis
export GTEST_LIBS = -L/usr/local/lib/gtest -lgtest -lgtest_main $(HIREDIS_LIBS)

# Binaries
EXAMPLE=redis_example
GTEST_BIN = gt_redis

# Object
TEST_SRC = $(wildcard $(EXAMPLE_DIR)/*.cpp)
TEST_OBJ = $(patsubst %.cpp, %.o, $(TEST_SRC))
REDIS_SRC = $(wildcard $(REDIS_DIR)/*.cpp)
REDIS_OBJ = $(patsubst %.cpp, %.o, $(REDIS_SRC))
GTEST_SRC = $(wildcard $(GTEST_DIR)/*.cpp)
GTEST_OBJ = $(patsubst %.cpp, %.o, $(GTEST_SRC))


all: check_dir $(EXAMPLE_DIR) $(EXAMPLE)

$(REDIS_DIR): COMPLIE
	@make -C $@ 

$(EXAMPLE_DIR): COMPLIE $(REDIS_DIR)
	@make -C $@

$(EXAMPLE): GENERATE
	$(LINK) -o $(BIN_DIR)/$@ $(TEST_OBJ) $(REDIS_OBJ) $(CPPFLAGS) $(HIREDIS_INCS) $(HIREDIS_LIBS)


# gtest
gtest: check_dir $(GTEST_DIR) $(GTEST_BIN)

$(GTEST_DIR): COMPLIE $(REDIS_DIR)
	@make -C $@

$(GTEST_BIN): GENERATE
	$(LINK) -o $(BIN_DIR)/$@ $(GTEST_OBJ) $(REDIS_OBJ) $(CPPFLAGS) $(GTEST_LIBS) $(HIREDIS_INCS)


check_dir:
	@mkdir -p $(BIN_DIR)

COMPLIE:
	@echo ===== compile =====

GENERATE:
	@echo ===== generate binary =====

.PHONY : all gtest clean

clean :
	@make -C redis clean
	@make -C example clean
	@make -C Gtest clean
	@$(RM) -rv $(BIN_DIR)

