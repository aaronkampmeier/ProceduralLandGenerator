//
// Created by Aaron Kampmeier on 6/23/20.
// Copyright © 2020 Aaron Kampmeier. All rights reserved.
// ASU ID: 1217750807
// For Class: CSE 240 with Prof. Selgrad
//

#include <iostream>

using namespace std;

/**
 * Represents each type of land possible for generation
 */
enum LandType: char {RURAL = 'R', FOREST = 'F', TOWN = 'T', CITY = 'T', MOUNTAIN = 'M', WATER = 'W'};

/**
 * Holds the probabilities for some land cell to have a neighboring cell of each type. The weights are relative to
 * each other. It is a struct wrapped in a union to allow the named probability variables to be iterated through as
 * an array.
 */
union LandNeighborProbabilities {
	int probabilities[6];
	struct {
		int ruralNeighborProb, forestNeighborProb, townNeighborProb, cityNeighborProb, mountainNeighborProb,
				waterNeighborProb;
	} landNeighborProbabilities{};

	LandNeighborProbabilities(int ruralProb, int forestProb, int townProb, int waterProb, int cityProb, int
	mountainProb) {

		landNeighborProbabilities = {ruralProb, forestProb, townProb, waterProb, cityProb, mountainProb};
		
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
LandType generateProbabilisticLandFromCell(LandNeighborProbabilities cellNeighborProbabilities);

/**
 * Procedurally generates a land mass grid with random land types in each cell dependent on the types around it.
 * @return
 */
int main() {
	cout << "World Generator!\nGenerates a land mass of specified size with random blocks representing different "
		 "land types.\n" << endl;
	
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


LandType **generateLand(int width, int height) {
	return nullptr;
}

/**
 * Picks a random land type to start the land generation in the upper left corner.
 * @return The land type
 */
LandType pickLandType() {
	return 0;
}

LandType generateNeighbor(LandType item) {
	return 0;
}

void printLand(LandType **land, int width, int height) {

}

LandType newFromRuralLand() {
	const static LandNeighborProbabilities neighborProbabilities(40, 25, 15, 10, 0, 10);
	
	
	
	return 0;
}

LandType newFromForest() {
	return 0;
}

LandType newFromTown() {
	return 0;
}

LandType newFromWater() {
	return 0;
}

LandType newFromCity() {
	return 0;
}

LandType newFromMountain() {
	return 0;
}

/**
 * Goes through each of the neighbor probabilities for a cell to probabilistically generate a neighbor cell
 * @param cellNeighborProbabilities
 * @return
 */
LandType generateProbabilisticLandFromCell(LandNeighborProbabilities cellNeighborProbabilities) {
	// Creates an array of LandTypes whose length is the sum of all probabilities in cellNeighborProbabilities
	// Each land type appears in the array exactly the number of its probability
	
	for
	LandType *weightedLandTypes = new LandType[];
}
