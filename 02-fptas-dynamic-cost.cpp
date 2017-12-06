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
// best calculated result of each instance
int * g_BestPriceWeightPair;
// relative error received as an argument
double g_E = 0;
// array of adjusted prices
int * g_AdjustedPrices;
// correct results found in sol directory
int * g_CorrectResults;
// array of deviations in instances
double * g_Deviations;
// support matrix of weight (core of dynamic programming alg.)
int ** g_WMatrix;


/**
 * Initialize method
 */
bool initialize(char* fileArgument, char* relativeErrorArgument) {
	g_Ids = new int[ROWS_COUNT];
	g_Results = new int[ROWS_COUNT];
	g_Weights = new int*[ROWS_COUNT];
	g_Prices = new int*[ROWS_COUNT];
	g_CorrectResults = new int[ROWS_COUNT];
	g_Deviations = new double[ROWS_COUNT];

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

	filepath = "sol/";
	filepath += fileArgument;
	filepath += ".sol.dat";

	ifstream file2 (filepath.c_str());
	if (!file2.is_open()) {
		cout << "File 2 " << filepath << " could not be loaded!" << endl;
		return false;
	}

	i = 0;
	while (getline (file2, line)) {
		istringstream iss(line);
		string _null;
		iss >> _null;
		iss >> _null;
		iss >> g_CorrectResults[i++];
	}

	g_E = atof(relativeErrorArgument);
	printf("Epsilon set to %f\n", g_E);

	return true;
}

/**
 * Free used memory
 */
void deinit() {
	if (g_Ids != NULL) delete g_Ids;
	if (g_Results != NULL) delete g_Results;
	if (g_CorrectResults != NULL) delete g_CorrectResults;
	if (g_Deviations != NULL) delete g_Deviations;

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

void createAdjustedPrices(int pos, double _K) {
	g_AdjustedPrices = new int[g_N];
	for (int i = 0; i < g_N; i++) {
		g_AdjustedPrices[i] = g_Prices[pos][i] / _K;
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

	// find maximum among prices
	int cMax = 0;
	for (int i = 0; i < g_N; i++)
		if (g_Prices[pos][i] > cMax) cMax = g_Prices[pos][i];
	double _K = (g_E * (double) cMax) / (double) g_N;
	createAdjustedPrices(pos, _K);

	// calculate all other fields in support weight matrix
    for (int i = 0; i < g_N; i++) {
    	for (int p = 0; p <= maxPriceSum; p++) {
    		g_WMatrix[i+1][p] = min(g_WMatrix[i][p], (p - g_AdjustedPrices[i]) < 0 ? 
    								INFINITE : ( g_WMatrix[i][p - g_AdjustedPrices[i]] + g_Weights[pos][i] ));
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
	delete g_AdjustedPrices;

	return bestPrice;
}
/**
 * Main function of the program
 */
int main(int argc,  char **argv) {
	cout << "dostam13 MI-PAA task 02 - branch and bound alg." << endl;

	if (argc < 3) {
		cout << "Error: Instance package name and relative error arguments are required (eg. type knap_4 0.1 to load from knap_4.inst.dat file)" << endl;
		return 1;
	}
	if (!initialize(argv[1], argv[2]))
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
		double timeSpent = (clock() - L) / (double) CLOCKS_PER_SEC;
		printf("#%d: %d, t: %f, deviation: %f\n", g_Ids[currentInstance], g_Results[currentInstance], timeSpent, g_Deviations[currentInstance]);
	}
	totalAverageDeviation /= (double) ROWS_COUNT;
	printf("Total average deviation: %f\n", totalAverageDeviation);

	// get final time
    double resTime = (clock() - S) / (double) CLOCKS_PER_SEC;
    cout << "Total time: " << resTime << endl;

	deinit();
	cout << endl << "== Run finished ==" << endl;
}
