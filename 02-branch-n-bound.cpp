#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string.h>
#include <sstream>
#include <cmath>
#include <bits/stdc++.h>

using namespace std;

const int ROWS_COUNT = 50;

// items#
int g_N = 0;
// total maximum weight
int g_M = 0;
// array of weights
int ** g_Weights; 
// array of prices
int ** g_Prices; 
// array of unique ids
int * g_Ids;
// array of best results
int * g_Results;
// best calculated result of each instance
int g_BestPrice;

/**
 * Initialize method
 */
bool initialize(char* fileArgument) {
	g_Ids = new int[ROWS_COUNT];
	g_Results = new int[ROWS_COUNT];
	g_Weights = new int*[ROWS_COUNT];
	g_Prices = new int*[ROWS_COUNT];

	string filepath = "inst/"; 
	filepath += fileArgument;
	filepath += ".inst.dat";

	ifstream file (filepath.c_str());
	if (!file.is_open()) {
		cout << "File " << filepath << " could not be loaded!" << endl;
		return false;
	}

	string line;
	int i = 0;
	while (getline (file, line)) {
		istringstream iss(line);
		iss >> g_Ids[i];
		iss >> g_N;
		iss >> g_M;

		g_Weights[i] = new int[g_N];
		g_Prices[i] = new int[g_N];
		for (int j = 0; j < g_N; ++j) {
			iss >> g_Weights[i][j];
			iss >> g_Prices[i][j];
		}
		i++;
	}

	return true;
}

/**
 * Free used memory
 */
void deinit() {
	if (g_Ids != NULL) delete g_Ids;
	if (g_Results != NULL) delete g_Results;

	if (g_Weights != NULL) {
		for (int i = 0; i < ROWS_COUNT; i++) 
			if (g_Weights[i] != NULL) 
				delete g_Weights[i];
		delete g_Weights;
	}

	if (g_Prices != NULL) {
		for (int i = 0; i < ROWS_COUNT; i++) 
			if (g_Prices[i] != NULL) 
				delete g_Prices[i];
		delete g_Prices;
	}
}

void calculateRec(int instance, int pos, bool include, int usedWeight, int currentPrice, int remainingPrice) {
	if ((include && (usedWeight + g_Weights[instance][pos]) > g_M) || remainingPrice + currentPrice < g_BestPrice || pos > g_N + 1) {
		return;
	}

	int newCurrentPrice = currentPrice;
	int newUsedWeight = usedWeight;
	if (include) {
		newCurrentPrice += g_Prices[instance][pos];
		newUsedWeight += g_Weights[instance][pos];
		if (newCurrentPrice > g_BestPrice) {
			g_BestPrice = newCurrentPrice;
		}
	}

	calculateRec(instance, pos + 1, true, newUsedWeight, newCurrentPrice, remainingPrice - g_Prices[instance][pos]);
	calculateRec(instance, pos + 1, false, newUsedWeight, newCurrentPrice, remainingPrice - g_Prices[instance][pos]);
}

/**
 * Calculate problem of an instance
 * PriceWeightRatio algorithm
 * @param Row number of an instance
 * @return Best calculated result of this instance
 */
int calculate(int pos) {
	g_BestPrice = 0;
	//long maxOptions = pow(2, g_N); @TODO possibly remove
	int maxPrice = 0;

	for (int i = 0; i < g_N; i++) {
		maxPrice += g_Prices[pos][i];
	}

    calculateRec(pos, 0, true, 0, 0, maxPrice);
    calculateRec(pos, 0, false, 0, 0, maxPrice);

	return g_BestPrice;
}
/**
 * Main function of the program
 */
int main(int argc,  char **argv) {
	cout << "dostam13 MI-PAA task 02 - branch and bound alg." << endl;

	if (argc < 2) {
		cout << "Error: Instance package name is required (eg. type knap_4 to load from knap_4.inst.dat file)" << endl;
		return 1;
	}
	if (!initialize(argv[1]))
		return 1;

	// start counting time
    clock_t S, L;
    S = clock();

	for (int currentInstance = 0; currentInstance < ROWS_COUNT; ++currentInstance) {
		L = clock();
		g_Results[currentInstance] = calculate(currentInstance);
		cout << "#" << g_Ids[currentInstance] << ": " << g_Results[currentInstance] << ", t: " << (clock() - L) / (double) CLOCKS_PER_SEC << endl;
	}

	// get final time
    double resTime = (clock() - S) / (double) CLOCKS_PER_SEC;
    cout << "Total time: " << resTime << endl;

	deinit();
	cout << endl << "== Run finished ==" << endl;
}
