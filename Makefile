# Redis Client Makefile
# Copyright (C) 2017-2018 chen tengfei <chentf1836@163.com>

# Compile Command
export CXX = g++
export LINK = g++

# Compile Option
OPTIMIZATION?=-O1
DEBUG?=-g -ggdb
WARNINGS=-Wall -W -Wwrite-strings -Wshadow
export CPPFLAGS= $(OPTIMIZATION) $(DEBUG) $(WARNINGS)

# Directory
HOME = $(PWD)
BIN_DIR = $(PWD)/bin
EXAMPLE_DIR = $(PWD)/example
REDIS_DIR = $(PWD)/redis
GTEST_DIR = $(PWD)/gtest

# include and library
export REDIS_INCS = -I$(REDIS_DIR)
export GTEST_LIBS = -L/usr/local/lib/gtest -lgtest -lgtest_main

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


all: check_dir $(EXAMPLE_DIR) $(EXAMPLE) ACCOMPLISH

$(REDIS_DIR): COMPLIE
	@make -C $@ 

$(EXAMPLE_DIR): COMPLIE $(REDIS_DIR)
	@make -C $@

$(EXAMPLE): GENERATE
	$(LINK) -o $(BIN_DIR)/$@ $(TEST_OBJ) $(REDIS_OBJ) $(CPPFLAGS) $(REDIS_INCS)


# gtest
gtest: check_dir $(GTEST_DIR) $(GTEST_BIN) ACCOMPLISH

$(GTEST_DIR): COMPLIE $(REDIS_DIR)
	@make -C $@

$(GTEST_BIN): GENERATE
	$(LINK) -o $(BIN_DIR)/$@ $(GTEST_OBJ) $(REDIS_OBJ) $(CPPFLAGS) $(GTEST_LIBS) $(REDIS_INCS)


check_dir:
	@mkdir -p $(BIN_DIR)

COMPLIE:
	@echo ===== compile =====

GENERATE:
	@echo ===== generate binary =====

ACCOMPLISH:
	@echo ===== accomplish =====
	@echo please see ./bin

.PHONY : all gtest clean

clean :
	@make -C $(REDIS_DIR) clean
	@make -C $(EXAMPLE_DIR) clean
	@make -C $(GTEST_DIR) clean
	@$(RM) -rv $(BIN_DIR)

