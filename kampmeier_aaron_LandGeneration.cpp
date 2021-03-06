//
// Created by Aaron Kampmeier on 6/25/20.
// Copyright © 2020 Aaron Kampmeier. All rights reserved.
// ASU ID: 1217750807
// For Class: CSE 240 with Prof. Selgrad
//

#include "kampmeier_aaron_LandGeneration.h"

using namespace std;

/**
 * Holds a singular record used inside of LandNeighborProbabilities that represents the probability for some land
 * cell to have a certain neighbor of neighborType.
 */
struct LandCellNeighborTypeProbability {
	LandType neighborType;
	int probability;
};

/**
 * Holds the probabilities for some land cell to have a neighboring cell of each type. The weights are relative to
 * each other. It is a struct wrapped in a union to allow the named probability variables to be iterated through as
 * an array.
 * neighborProbabilities is assumed and constructed to hold one record of LandCellNeighborTypeProbability for each
 * LandType. However, if it were to hold multiple records of the same LandType (and 0 records of some other LandTypes),
 * this would not practically cause a problem as the probabilities of the same type would just be added together.
 *
 * And yes yes, I know. I really would have rather used a map (or similar) structure here, but I wasn't sure we were
 * allowed to pull in other libraries like that. So instead I just architected it this way with an array of structs
 * containing a "key" (LandType) and a "value" (probability). I guess this is kinda a primitive map implementation
 * anyway just without the enforcement of only one record for a specific key.
 */
struct LandNeighborProbabilities {
	LandCellNeighborTypeProbability neighborProbabilities[LAND_TYPES];

//	int ruralNeighborProb, forestNeighborProb, townNeighborProb, cityNeighborProb, mountainNeighborProb,
//			waterNeighborProb;
	
	LandNeighborProbabilities(int ruralProb, int forestProb, int townProb, int waterProb, int cityProb, int
	mountainProb) {
		neighborProbabilities[0] = {RURAL, ruralProb};
		neighborProbabilities[1] = {FOREST, forestProb};
		neighborProbabilities[2] = {TOWN, townProb};
		neighborProbabilities[3] = {WATER, waterProb};
		neighborProbabilities[4] = {CITY, cityProb};
		neighborProbabilities[5] = {MOUNTAIN, mountainProb};
	}
	
	int totalProbabilityVolume() const {
		int volume = 0;
		for (auto neighborProbability : neighborProbabilities) {
			volume += neighborProbability.probability;
		}
		return volume;
	}
};


/**
 * Generates a land mass of specified size with a random seed land cell at (0,0) and then generates more land based on
 * probabilities from there
 * @param width The width in cells of the land mass
 * @param height The height in cells
 * @return A 2D array of size height x width
 */
LandType **generateLand(int width, int height) {
	LandType **land = new LandType *[height];
	
	// Pre-declare variables used in the loop to avoid unnecessary re-declaration each iteration
	// each will be overwritten each iteration
	int neighbor1X;
	int neighbor1Y;
	int neighbor2X;
	int neighbor2Y;
	
	// Go through each cell starting from (0,0) and generate the cell from one of its neighbors
	for (int y = 0; y < height; y++) {
		land[y] = new LandType[width];
		
		for (int x = 0; x < width; x++) {
			
			// Find the neighbors in the negative direction from this cell to use
			// Neighbor1 is to the left, neighbor2 is up
			neighbor1X = x - 1;
			neighbor1Y = y;
			neighbor2X = x;
			neighbor2Y = y - 1;
			
			if (neighbor1X < 0 && neighbor2Y < 0) { // Don't need to check the 1Y and 2X because those are the same
				// as loop itrs and known to never be negative
				
				//No neighbors, should only happen for (0,0)
				// call pickLandType()
				land[y][x] = pickLandType();
			} else if (neighbor1X < 0) {
				// Neighbor 1 is out of bounds, but neighbor 2 isn't, use neighbor 2
				land[y][x] = generateNeighbor(land[neighbor2Y][neighbor2X]);
			} else if (neighbor2Y < 0) {
				// Neighbor 2 is out of bounds, use neighbor 1
				land[y][x] = generateNeighbor(land[neighbor1Y][neighbor1X]);
			} else {
				// Both neighbors are in bounds, flip a coin
				if (rand() % 2 == 1) {
					// Use neighbor 1
					land[y][x] = generateNeighbor(land[neighbor1Y][neighbor1X]);
				} else {
					// Use neighbor 2
					land[y][x] = generateNeighbor(land[neighbor2Y][neighbor2X]);
				}
			}
			
		}
	}
	
	return land;
}

/**
 * Picks a random land type to start the land generation in the upper left corner.
 * @return The land type
 */
LandType pickLandType() {
	// Set a landNeighborProbabilities with all at equal probability
	LandNeighborProbabilities landNeighborProbabilities(1, 1, 1, 1, 1, 1);
	
	return generateProbabilisticLandFromCell(landNeighborProbabilities);
}

/**
 * Generates a probable neighbor for a land cell based on that cell's neighbor probabilities
 * @param item The land cell to generate a neighbor for
 * @return The probable neighbor
 */
LandType generateNeighbor(LandType item) {
	// Just switch the land type and call appropriate land cell function
	
	switch (item) {
		case RURAL:
			return newFromRuralLand();
		case FOREST:
			return newFromForest();
		case TOWN:
			return newFromTown();
		case WATER:
			return newFromWater();
		case CITY:
			return newFromCity();
		case MOUNTAIN:
			return newFromMountain();
		default:
			cerr << "Unknown LandType" << endl;
			exit(UNKNOWN_LAND_TYPE_EXIT);
	}
	
}

/**
 * Prints out the entire land mass in the console, optionally using text color
 * @param land The land mass
 * @param width Its width
 * @param height Its height
 */
void printLand(LandType **land, int width, int height) {
	cout << "\nLand output: " << endl;
	// First perform sanity size check
	if (width > CONSOLE_MAX_WIDTH_OUTPUT || height > CONSOLE_MAX_HEIGHT_OUTPUT) {
		// Output is kinda large
		// Use File IO to put it into some file
		
		// Gets the current time to in a string such as "2020-06-25 15:02:40"
		time_t nowTime = time(nullptr);
		tm *localTime = localtime(&nowTime);
		char *timeString = new char[20]; //Length is always 20 as can be seen in the example time str above
		timeString[19] = '\0';
		sprintf(timeString, "%04d-%02d-%02d %02d:%02d:%02d", localTime->tm_year + 1900, localTime->tm_mon + 1,
				localTime->tm_mday, localTime->tm_hour, localTime->tm_min, localTime->tm_sec);
		
		char outputFilePrefix[] = "Land Outputs/land_";
		
		// Combine the two strings
		char *outputFilePath = concatStrings(outputFilePrefix, timeString);
		
		// Set up the file
		FILE *outputFile = fopen(outputFilePath, "w");
		
		if (outputFile == nullptr) {
			cout << termcolor::bold << termcolor::red << "Error opening output file: " << outputFilePath << endl;
			
			cout << "Make sure you have a \"Land Outputs\" folder in the current working directory." <<
			termcolor::reset << endl;
			
			exit(FILE_ERROR_EXIT);
		}
		
		// Print every LandType to the file
		for (int y = 0; y < height; y++) {
			for (int x = 0; x < width; x++) {
				fprintf(outputFile, "%c", land[y][x]);
			}
			fprintf(outputFile, "\n");
		}
		
		cout << termcolor::italic << "Output the land structure to a file: " << outputFilePath << termcolor::reset
			 << endl;
		
		//Close file
		fclose(outputFile);
		delete[] outputFilePath;
		delete[] timeString;
		
		// This delete command causes a runtime error (SIGABRT) on general.asu.edu but not on my computer
//		delete [] localTime;
	
	} else {
		// Fits nicely within console, print it out
		for (int y = 0; y < height; y++) {
			for (int x = 0; x < width; x++) {
				cout << land[y][x];
//				printf("%c", land[y][x]);
			}
			cout << endl;
//			printf("\n");
		}
	}
}

/**
 * Handles printing out a LandType as a character and with the correct colors
 * @param os
 * @param landType
 * @return
 */
ostream &operator<<(ostream &os, LandType landType) {
	os << termcolor::white;
	
	switch (landType) {
		case FOREST:
			os << termcolor::on_green;
			break;
		case RURAL:
			os << termcolor::on_yellow;
			break;
		case TOWN:
			os << termcolor::on_magenta;
			break;
		case CITY:
			os << termcolor::on_red;
			break;
		case WATER:
			os << termcolor::on_blue;
			break;
		case MOUNTAIN:
			os << termcolor::on_grey;
			break;
		default:
//			cerr << "Unknown color for land type" << endl;
			os << termcolor::bold;
			break;
	}
	
	os << (char) landType << termcolor::reset;
	return os;
}


//------------
/**
 * Each of these methods generates a probabilistic neighbor based on the LandType that the function represents.
 * To be clear, I don't like this implementation because of the repetitive nature of each function and the code
 * similarity between them screams to me that I should just have one function that lookups the neighbor probs for
 * each LandType in some table or map. But alas, these functions are in the assignment spec so here they are.
 * @return The new neighbor.
 */
LandType newFromRuralLand() {
#if ON_SPEC_PROBS
	const static LandNeighborProbabilities neighborProbabilities(40, 25, 15, 10, 0, 10);
#else
	const static LandNeighborProbabilities neighborProbabilities(90, 15, 10, 5, 1, 5);
#endif
	return generateProbabilisticLandFromCell(neighborProbabilities);
}

LandType newFromForest() {
#if ON_SPEC_PROBS
	const static LandNeighborProbabilities neighborProbabilities(20, 40, 5, 20, 0, 15);
#else
	const static LandNeighborProbabilities neighborProbabilities(20,100,5,20,1,7);
#endif
	return generateProbabilisticLandFromCell(neighborProbabilities);
}

LandType newFromTown() {
#if ON_SPEC_PROBS
	const static LandNeighborProbabilities neighborProbabilities(20, 5, 40, 10, 20, 5);
#else
	const static LandNeighborProbabilities neighborProbabilities(15,5,120,8,17,3);
#endif
	return generateProbabilisticLandFromCell(neighborProbabilities);
}

LandType newFromWater() {
#if ON_SPEC_PROBS
	const static LandNeighborProbabilities neighborProbabilities(15, 20, 5, 40, 10, 10);
#else
	const static LandNeighborProbabilities neighborProbabilities(15,20,5,70,10,5);
#endif
	return generateProbabilisticLandFromCell(neighborProbabilities);
}

LandType newFromCity() {
#if ON_SPEC_PROBS
	const static LandNeighborProbabilities neighborProbabilities(0, 15, 25, 10, 40, 10);
#else
	const static LandNeighborProbabilities neighborProbabilities(1,1,9,5,80,3);
#endif
	return generateProbabilisticLandFromCell(neighborProbabilities);
}

LandType newFromMountain() {
#if ON_SPEC_PROBS
	const static LandNeighborProbabilities neighborProbabilities(5, 15, 5, 15, 10, 50);
#else
	const static LandNeighborProbabilities neighborProbabilities(0,10,5,10,7,200);
#endif
	return generateProbabilisticLandFromCell(neighborProbabilities);
}
//----------

/**
 * Goes through each of the neighbor probabilities for a cell to probabilistically generate a neighbor cell.
 * The way it does this is by creating an array of LandTypes where each LandType appears exactly as many times as the
 * probability for that LandType specified in cellNeighborProbabilities. Then an index is chosen at random from that
 * array and the LandType at that index is returned.
 * This preserves the probabilities of picking each LandType as specified in cellNeighborProbabilities as it mimics a
 * grab bag where a random item is returned and each item has probability of probability/totalProbabilityVolume. This
 * way also naturally allows for future expansion into more LandTypes as it has no hardcoded knowledge of
 * specific LandTypes.
 * @param cellNeighborProbabilities A struct representing the probabilities for this cell to have a neighbor of each
 * LandType
 * @return A probable neighbor
 */
LandType generateProbabilisticLandFromCell(const LandNeighborProbabilities &cellNeighborProbabilities) {
	// Creates an array of LandTypes whose length is the sum of all probabilities in cellNeighborProbabilities
	// Each land type appears in the array exactly the number of its probability
	
	LandType *weightedLandTypes = new LandType[cellNeighborProbabilities.totalProbabilityVolume()];
	
	// Go through each probability and add that number of LandTypes to the array
	int arrayItr = 0;
	for (auto neighborProbability : cellNeighborProbabilities.neighborProbabilities) {
		for (int i = 0; i < neighborProbability.probability; i++) {
			weightedLandTypes[arrayItr] = neighborProbability.neighborType;
			arrayItr++;
		}
	}
	
	// Generate a random number between 0 and the totalProbabilityVolume and check what land type is at that index in
	// the array
	int randIndex = rand() % cellNeighborProbabilities.totalProbabilityVolume();
	LandType selectedType = weightedLandTypes[randIndex];
	delete[] weightedLandTypes;
	return selectedType;
	
}


/**
 * Creates a new char[] and copies the two specified strings to it; concat str1 + str2.
 * @param str1
 * @param str2
 * @return A new char[]
 */
char *concatStrings(char *str1, char *str2) {
	char *newString = new char[strlen(str1) + strlen(str2) + 1];
	strcpy(newString, str1);
	// Second string goes at the end of the first string accomplished using pointer math
	strcpy(newString + strlen(str1), str2);
	return newString;
}

