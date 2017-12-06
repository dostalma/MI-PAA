#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string.h>
#include <sstream>
#include <cmath>
#include <bits/stdc++.h>
#include <algorithm> 

using namespace std;

const int ROWS_COUNT = 50;
const int INFINITE = 999999;

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
// support matrix of weight (core of dynamic programming alg.)
int ** g_WMatrix;

/**
 * Initialize method
 */
bool initialize(char* fileArgument) {
	g_Ids = new int[ROWS_COUNT];
	g_Results = new int[ROWS_COUNT];
	g_Weights = new int*[ROWS_COUNT];
	g_Prices = new int*[ROWS_COUNT];

	string filepath = "gen_inst/"; 
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

/**
 * Calculate problem of an instance
 * Dynamic programming - Price method alg.
 * @param Row number of an instance
 * @return Best calculated result of this instance
 */
int calculate(int pos) {
	// calculate sum of all prices
	int maxPriceSum = 0;
	for (int i = 0; i < g_N; i++) maxPriceSum += g_Prices[pos][i];

	// initialize support weight matrix 
	g_WMatrix = new int*[g_N+1];
	for (int i = 0; i <= g_N; i++) g_WMatrix[i] = new int[maxPriceSum+1];
	// set W[0][0] to 0
	g_WMatrix[0][0] = 0;
	// set W[0][p] to INF for p > 0
	for (int i = 1; i <= maxPriceSum; i++) g_WMatrix[0][i] = INFINITE;

	// calculate all other fields in support weight matrix
    for (int i = 0; i < g_N; i++) {
    	for (int p = 0; p <= maxPriceSum; p++) {
    		g_WMatrix[i+1][p] = min(g_WMatrix[i][p], (p - g_Prices[pos][i]) < 0 ? 
    								INFINITE : ( g_WMatrix[i][p - g_Prices[pos][i]] + g_Weights[pos][i] ));
    	}
    }

    // get highest achieved price within knapsack capacity (in last column)
    int bestPrice = 0;
    for (int p = 0; p <= maxPriceSum; p++) {
    	if (g_WMatrix[g_N][p] <= g_M && p > bestPrice) bestPrice = p;
    }

    // deinitialize support weight matrix
	for (int i = 0; i <= g_N; i++) delete g_WMatrix[i];
	delete g_WMatrix;

	return bestPrice;
}
/**
 * Main function of the program
 */
int main(int argc,  char **argv) {
	cout << "dostam13 MI-PAA task 02 - dynamic programming price decomposition alg." << endl;

	if (argc < 2) {
		cout << "Error: Instance package name is required (eg. type knap_4 to load from knap_4.inst.dat file)" << endl;
		return 1;
	}
	if (!initialize(argv[1]))
		return 1;

	// start counting time
    clock_t S, L;
    S = clock();

	double totalAverageDeviation = 0.0;
	for (int currentInstance = 0; currentInstance < ROWS_COUNT; ++currentInstance) {
		L = clock();
		g_Results[currentInstance] = calculate(currentInstance);
		g_Deviations[currentInstance] = ((abs (g_Results[currentInstance] - g_CorrectResults[currentInstance])) 
										/ (double)g_CorrectResults[currentInstance]) * 100;
		totalAverageDeviation += g_Deviations[currentInstance];
		cout << "#" << g_Ids[currentInstance] << ": " << g_Results[currentInstance] << ", t: " << (clock() - L) / (double) CLOCKS_PER_SEC << endl;
	}

	totalAverageDeviation /= (double) ROWS_COUNT;
	printf("Total average deviation: %f\n", totalAverageDeviation);

	// get final time
    double resTime = (clock() - S) / (double) CLOCKS_PER_SEC;
    cout << "Total time: " << resTime << endl;

	deinit();
	cout << endl << "== Run finished ==" << endl;
}
