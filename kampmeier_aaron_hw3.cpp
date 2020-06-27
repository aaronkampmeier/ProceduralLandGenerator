//
// Created by Aaron Kampmeier on 6/23/20.
// Copyright © 2020 Aaron Kampmeier. All rights reserved.
// ASU ID: 1217750807
// For Class: CSE 240 with Prof. Selgrad
//


#include <iostream>
#include "kampmeier_aaron_LandGeneration.h"

using namespace std;


// Forward declarations
bool scanIntSafe(int &outputVal, char *inputStr = nullptr);

/**
 * Procedurally generates a land mass grid with random land types in each cell dependent on the types around it.
 * Executable takes in arguments in the form of: assign3 [width [height]]
 * @return Exit code
 */
int main(int argc, char **argv) {
	cout << "World Generator!\nGenerates a land mass of specified size with random blocks representing different "
			"land types.\n" << endl;
	
	if (ON_SPEC_PROBS == 0) {
		cout << termcolor::bold << termcolor::red << "Warning! Off spec!!" << termcolor::reset << endl;
	}
	
	// Seed the rand
	srand(time(nullptr));
	
	int landHeight = 0;
	int landWidth = 0;
	// Pull in these inputs from command line args
	if (argc > 1) {
		// Pull height from command line arg
		if (!scanIntSafe(landHeight, argv[1]) || landHeight <= 0) {
			cout << termcolor::bold << termcolor::red << "Height argument is invalid. Make sure it is a positive "
														 "integer." << termcolor::reset << endl;
			landHeight = 0;
		}
	}
	if (argc > 2) {
		// Pull in width as well
		if (!scanIntSafe(landWidth, argv[2]) || landWidth <= 0) {
			cout << termcolor::bold << termcolor::red << "Width argument is invalid. Make sure it is a positive "
														 "integer." << termcolor::reset << endl;
			landWidth = 0;
		}
	}
	
	
	// Ask the user for the land mass size if the values were not passed in successfully
	if (landHeight == 0) {
		cout << "Enter the height of the land area: " << endl;
		while (!scanIntSafe(landHeight) || landHeight <= 0) {
			cout << termcolor::bold << termcolor::red << "Invalid input. Make sure to enter a positive integer. Try "
														 "again." << termcolor::reset << endl;
		}
	}
	
	if (landWidth == 0) {
		cout << "Enter the width of the land mass: " << endl;
		while (!scanIntSafe(landWidth) || landWidth <= 0) {
			cout << termcolor::bold << termcolor::red << "Invalid input. Make sure to enter a positive integer. Try "
														 "again." << termcolor::reset << endl;
		}
	}
	
	
	LandType **land = generateLand(landWidth, landHeight);
	
	printLand(land, landWidth, landHeight);
	
	// Clean up
	for (int y = 0; y < landHeight; y++) {
		delete[] land[y];
	}
	delete [] land;
	
	return 0;
}

/**
 * Scans an input string or the std cin and converts it to an int if possible, will fail gracefully if any chars
 * other than digits are present.
 * I ported this function over from my assignment 2 code with just the change of making outputVal a pass by reference
 * parameter, so look there for some more explanation.
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
	int val = (int) strtol(inputStr, &end, 0);
	
	// Check if it succeeded, if the end ptr points to something other than \0, then the conversion didn't go
	// through the entire arg string meaning some characters other than digits got in the way. Consider this
	// invalid input. Thus if end is not pointing to \0 then the conversion failed.
	bool success = *end == '\0';
	
	//Clean up the memory allocated for the array if we created it
	if (readFromCin) {
		delete[] inputStr;
	}
	
	// Set the read integer into the specified output location
	if (success) {
		outputVal = val;
	}
	
	return success;
}
