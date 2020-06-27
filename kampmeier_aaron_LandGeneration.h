//
// Created by Aaron Kampmeier on 6/25/20.
// Copyright © 2020 Aaron Kampmeier. All rights reserved.
// ASU ID: 1217750807
// For Class: CSE 240 with Prof. Selgrad
//

#ifndef ASSIGNMENT_3_KAMPMEIER_AARON_LANDGENERATION_H
#define ASSIGNMENT_3_KAMPMEIER_AARON_LANDGENERATION_H

//-- Defines
//Defines the number of different land types in this generator. Kinda wanna get rid of this as it's only used in one
// place in LandNeighborProbabilities but I don't see much use to move it's functionality over to a static constant...
#define LAND_TYPES 6
//I wanted my testing probabilities to be much more drastic than the spec (i.e. a city cell should have 80+% of being
// next to another city instead of 40% in the spec). This flag flips between building my probs for the spec or for me.
//TODO: Flip to 1
#define ON_SPEC_PROBS 1

//Inspired from the "sanity check" in the original assignment spec. If the dimensions of the land output go over
// these specified here, then the land is output to a file instead.
#define CONSOLE_MAX_WIDTH_OUTPUT 70
#define CONSOLE_MAX_HEIGHT_OUTPUT 70

//-- Exit Codes
#define UNKNOWN_LAND_TYPE_EXIT 1
#define FILE_ERROR_EXIT 2


#include <iostream>
#include <cstring>
#include <termcolor/termcolor.hpp>

/**
 * Represents each type of land possible for generation
 */
enum LandType : char {
	RURAL = 'R', FOREST = 'F', TOWN = 'T', CITY = 'C', MOUNTAIN = 'M', WATER = 'W'
};
struct LandCellNeighborTypeProbability;
struct LandNeighborProbabilities;

LandType **generateLand(int width, int height);

LandType pickLandType();

LandType generateNeighbor(LandType item);

void printLand(LandType **land, int width, int height);

std::ostream &operator<<(std::ostream &os, LandType landType);

LandType newFromRuralLand();

LandType newFromForest();

LandType newFromTown();

LandType newFromWater();

LandType newFromCity();

LandType newFromMountain();

LandType generateProbabilisticLandFromCell(const LandNeighborProbabilities &cellNeighborProbabilities);

char *concatStrings(char *str1, char *str2);


#endif //ASSIGNMENT_3_KAMPMEIER_AARON_LANDGENERATION_H
