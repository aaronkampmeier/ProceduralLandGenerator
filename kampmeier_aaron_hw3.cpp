//
// Created by Aaron Kampmeier on 6/23/20.
// Copyright © 2020 Aaron Kampmeier. All rights reserved.
// ASU ID: 1217750807
// For Class: CSE 240 with Prof. Selgrad
//

#define LAND_TYPES 6

#include <iostream>

using namespace std;

/**
 * Represents each type of land possible for generation
 */
enum LandType: char {RURAL = 'R', FOREST = 'F', TOWN = 'T', CITY = 'C', MOUNTAIN = 'M', WATER = 'W'};

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

bool scanIntSafe(int &outputVal, char *inputStr = nullptr);
LandType **generateLand(int width, int height);
LandType pickLandType();
LandType generateNeighbor(LandType item);
void printLand(LandType **land, int width, int height);

LandType newFromRuralLand();
LandType newFromForest();
LandType newFromTown();
LandType newFromWater();
LandType newFromCity();
LandType newFromMountain();
LandType generateProbabilisticLandFromCell(const LandNeighborProbabilities &cellNeighborProbabilities);

/**
 * Procedurally generates a land mass grid with random land types in each cell dependent on the types around it.
 * @return Exit code
 */
int main() {
	cout << "World Generator!\nGenerates a land mass of specified size with random blocks representing different "
		 "land types.\n" << endl;
	
	// Seed the rand
	srand(time(nullptr));
	
	// Ask the user for the land mass size
	cout << "Enter the width of the land mass: " << endl;
	int landWidth;
	while (!scanIntSafe(landWidth) && landWidth > 0) {
		cerr << "Invalid input. Make sure to enter a positive integer." << endl;
	}
	
	cout << "Enter the height of the land area: " << endl;
	int landHeight;
	while (!scanIntSafe(landHeight) && landHeight > 0) {
		cerr << "Invalid input. Make sure to enter a positive integer." << endl;
	}
	
	LandType **land = generateLand(landWidth, landHeight);
	
	return 0;
}

/**
 * Scans an input string or the std cin and converts it to an int if possible, will fail gracefully if any chars
 * other than digits are present.
 * @param outputVal A reference to the int where the scanned value should be stored.
 * @param inputStr Can be NULL. If provided, input will be scanned from this char array instead of cin.
 * @return Successful or not
 */
bool scanIntSafe(int &outputVal, char *inputStr) {
	char* end; // strtol will put the pointer of where it ended scanning on the input string in here
	
	// If it was not called with an input string then allocate one and read in from cin.
	bool readFromCin = false;
	if (inputStr == nullptr) {
		inputStr = (new char[20]);
		cin >> inputStr;
		readFromCin = true;
	}
	
	// Originally I used this sscanf to convert a str to an int, but after doing some reading I decided strtol is the
	// widely accepted choice for safe conversion.
	// Update: Okay I reached the end of the assignment doc where it listed "Important Notes" and said we didn't have
	// to worry about bizarre input like text into an integer field right now which I didn't see until now when I'm
	// all done. I quite like my implementation now, I'm proud of it, so even if it is overkill for right now oh well
	// whatever. Put text into my integer fields all you like, see what it does. Who needs scanf.
	int val = (int) strtol(inputStr, &end, 0);
	
	// Check if it succeeded, if the end ptr points to something other than \0, then the conversion didn't go
	// through the entire arg string meaning some characters other than digits got in the way. Consider this
	// invalid input. Thus if end is not pointing to \0 then the conversion failed.
	bool success = *end == '\0';
	
	//Clean up the memory allocated for the array if we created it
	if (readFromCin) {
		delete[] inputStr;
	}
	
	// Set the read integer into the variable passed in
	outputVal = val;
	
	return success;
}


/**
 * Generates a land mass of specified size with a random seed land cell at (0,0) and then generates more land based on
 * probabilities from there
 * @param width The width in cells of the land mass
 * @param height The height in cells
 * @return A 2D array of size height x width
 */
LandType **generateLand(int width, int height) {
	LandType **land = new LandType*[height];
	
	// Pre-declare variables used in the loop to avoid unnecessary re-declaration each iteration
	// each will be overwritten each iteration
	int neighbor1X;
	int neighbor1Y;
	int neighbor2X;
	int neighbor2Y;
	
	// Go through each cell starting from (0,0) and generate the cell from one of its neighbors
	for (int y=0; y < height; y++) {
		land[y] = new LandType[width];
		
		for (int x=0; x < width; x++) {
			
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
	LandNeighborProbabilities landNeighborProbabilities(1,1,1,1,1,1);
	
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
	}

}

/**
 * Prints out the entire land mass in the console, optionally using text color
 * @param land The land mass
 * @param width Its width
 * @param height Its height
 */
void printLand(LandType **land, int width, int height) {

}

//------------
/**
 * Each of these methods generates a probabilistic neighbor based on the LandType that the function represents.
 * To be clear, I don't like this implementation because the repetitive nature of each function and the code
 * similarity between them screams to be that I should just have one function that lookups the neighbor probs for
 * each LandType in some table or map. But alas, these functions are in the assignment spec so here they are.
 * @return The new neighbor.
 */
LandType newFromRuralLand() {
	const static LandNeighborProbabilities neighborProbabilities(40, 25, 15, 10, 0, 10);
	
	return generateProbabilisticLandFromCell(neighborProbabilities);
}

LandType newFromForest() {
	const static LandNeighborProbabilities neighborProbabilities(20,40,5,20,0,15);
	
	return generateProbabilisticLandFromCell(neighborProbabilities);
}

LandType newFromTown() {
	const static LandNeighborProbabilities neighborProbabilities(20,5,40,10,20,5);
	
	return generateProbabilisticLandFromCell(neighborProbabilities);
}

LandType newFromWater() {
	const static LandNeighborProbabilities neighborProbabilities(15,20,5,40,10,10);
	
	return generateProbabilisticLandFromCell(neighborProbabilities);
}

LandType newFromCity() {
	const static LandNeighborProbabilities neighborProbabilities(0,15,25,10,40,10);
	
	return generateProbabilisticLandFromCell(neighborProbabilities);
}

LandType newFromMountain() {
	const static LandNeighborProbabilities neighborProbabilities(5,15,5,15,10,50);
	
	return generateProbabilisticLandFromCell(neighborProbabilities);
}
//----------

/**
 * Goes through each of the neighbor probabilities for a cell to probabilistically generate a neighbor cell.
 * TODO: Explanation.
 * @param cellNeighborProbabilities
 * @return A probable neighbor
 */
LandType generateProbabilisticLandFromCell(const LandNeighborProbabilities &cellNeighborProbabilities) {
	// Creates an array of LandTypes whose length is the sum of all probabilities in cellNeighborProbabilities
	// Each land type appears in the array exactly the number of its probability
	
	LandType *weightedLandTypes = new LandType[cellNeighborProbabilities.totalProbabilityVolume()];
	
	// Go through each probability and add that number of LandTypes to the array
	int arrayItr = 0;
	for (auto neighborProbability : cellNeighborProbabilities.neighborProbabilities) {
		for (int i=0; i < neighborProbability.probability; i++) {
			weightedLandTypes[arrayItr] = neighborProbability.neighborType;
			arrayItr++;
		}
	}
	
	// Generate a random number between 0 and the totalProbabilityVolume and check what land type is at that index in
	// the array
	int randIndex = rand() % cellNeighborProbabilities.totalProbabilityVolume();
	return weightedLandTypes[randIndex];
	
}
