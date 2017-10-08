#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string.h>
#include <sstream>
#include <math.h> 

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
double ** g_Ratios;
// array of unique ids
int * g_Ids;
// array of best results
int * g_Results;

int compare (const void * a, const void * b)
{
  return ( *(double*)b - *(double*)a );
}

/**
 * Initialize method
 */
bool initialize(char* fileArgument) {
	g_Ids = new int[ROWS_COUNT];
	g_Results = new int[ROWS_COUNT];
	g_Weights = new int*[ROWS_COUNT];
	g_Prices = new int*[ROWS_COUNT];
	g_Ratios = new double*[ROWS_COUNT];

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
		g_Ratios[i] = new double[g_N];
		for (int j = 0; j < g_N; ++j) {
			iss >> g_Weights[i][j];
			iss >> g_Prices[i][j];
			g_Ratios[i][j] = (double) g_Prices[i][j] / (double) g_Weights[i][j];			
		}
		qsort (g_Ratios[i], 6, sizeof(double), compare);
	    for (int j=0; j<g_N; j++)
	       if (i==0) printf ("%f ",g_Ratios[i][j]);

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

	if (g_Ratios != NULL) {
		for (int i = 0; i < ROWS_COUNT; i++) 
			if (g_Ratios[i] != NULL) 
				delete g_Ratios[i];
		delete g_Ratios;
	}
}

/**
 * Calculate problem of an instance
 * PriceWeightRatio algorithm
 * @param Row number of an instance
 * @return Best calculated result of this instance
 */
int calculate(int pos) {
	int bestPrice = 0;
	int maxOptions = pow(2, g_N);

	for (int option = 0; option < maxOptions; option++) {
		int currentWeight = 0;
		int currentPrice = 0;
		for (int item = 0; item < g_N; item++) {
			if (option & (int)pow(2,item) && (g_Weights[pos][item] <=  g_M - currentWeight)) {
				currentWeight += g_Weights[pos][item];
				currentPrice += g_Prices[pos][item];
			}
		}
		if (currentPrice > bestPrice)
			bestPrice = currentPrice;
	}

	return bestPrice;
}
/**
 * Main function of the program
 */
int main(int argc,  char **argv) {
	cout << "dostam13 Mi-PAA task 01 - price/weight ratio alg." << endl;

	if (argc < 2) {
		cout << "Error: Instance package name is required (eg. type knap_4 to load from knap_4.inst.dat file)" << endl;
		return 1;
	}
	if (!initialize(argv[1]))
		return 1;

	for (int currentInstance = 0; currentInstance < 1; ++currentInstance) {
		g_Results[currentInstance] = calculate(currentInstance);
		cout << "#" << g_Ids[currentInstance] << ": " << g_Results[currentInstance] << endl;
	}

	deinit();
	cout << endl << "== Run finished ==" << endl;
}
