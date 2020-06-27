#
# Created by Aaron Kampmeier on 6/25/20.
# Copyright © 2020 Aaron Kampmeier. All rights reserved.
# ASU ID: 1217750807
# For Class: CSE 240 with Prof. Selgrad
#

# A primitive makefile to be used for the graders of Assignment 3 to build my application on general.asu.edu
# Requires that there exist an "out" directory

PROJ_NAME = Assignment_3
BUILD_DIR = out
CPP = /opt/rh/devtoolset-9/root/usr/bin/g++
C = /opt/rh/devtoolset-9/root/usr/bin/gcc
FLAGS = -g -Wall
OBJS = $(BUILD_DIR)/LandGeneration.o $(BUILD_DIR)/kampmeier_aaron_hw3.o
CXX_INCLUDES = -I "termcolor/include"


default_target: $(PROJ_NAME)

$(PROJ_NAME): termcolor/include/termcolor/termcolor.hpp
$(PROJ_NAME): ${OBJS}
	${CPP} ${CXX_INCLUDES} ${FLAGS} ${OBJS} -o "$(BUILD_DIR)/$(PROJ_NAME)"
	echo "Built project into $(BUILD_DIR) folder. Run it with ./$(BUILD_DIR)/$(PROJ_NAME)"


$(BUILD_DIR)/kampmeier_aaron_hw3.o: kampmeier_aaron_hw3.cpp
	${CPP} ${CXX_INCLUDES} ${FLAGS} -c kampmeier_aaron_hw3.cpp -o $(BUILD_DIR)/kampmeier_aaron_hw3.o

$(BUILD_DIR)/LandGeneration.o: termcolor/include/termcolor/termcolor.hpp
$(BUILD_DIR)/LandGeneration.o: kampmeier_aaron_LandGeneration.cpp kampmeier_aaron_LandGeneration.h
	${CPP} ${CXX_INCLUDES} ${FLAGS} -c kampmeier_aaron_LandGeneration.cpp -o $(BUILD_DIR)/LandGeneration.o

clean:
	rm -r $(BUILD_DIR)/*