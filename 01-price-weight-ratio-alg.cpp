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
// array of unique ids
int * g_Ids;
// array of best results
int * g_Results;
// correct results found in sol directory
int * g_CorrectResults;
// array of deviations in instances
double * g_Deviations;
// maximum deviation found in instances
double g_maxDeviation = 0.0;

struct TItem {
	int m_Price;
	int m_Weight;
	double m_Ratio;
};

TItem ** g_Items;

void printItems (int limit) {
	for (int i = 0, range = limit != 0 ? limit : ROWS_COUNT; i < range; i++) {
		cout << g_Ids[i] << ": ";
		for (int j = 0; j < g_N; j++) {
			cout << g_Items[i][j].m_Ratio << " ";
		}
		cout << endl;
	}
}

void printItem (int pos) {
	cout << g_Ids[pos] << ": " << endl;
	for (int j = 0; j < g_N; j++) {
		printf ("r: %f, w: %d, p: %d\n", g_Items[pos][j].m_Ratio, g_Items[pos][j].m_Weight, g_Items[pos][j].m_Price);
	}
	cout << endl;
}

/**
 * Initialize method
 */
bool initialize(char* fileArgument) {
	g_Ids = new int[ROWS_COUNT];
	g_Results = new int[ROWS_COUNT];
	g_Items = new TItem*[ROWS_COUNT];
	g_Deviations = new double[ROWS_COUNT];
	g_CorrectResults = new int[ROWS_COUNT];
	
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

		g_Items[i] = new TItem[g_N];

		for (int j = 0; j < g_N; ++j) {
			iss >> g_Items[i][j].m_Weight;
			iss >> g_Items[i][j].m_Price;
			g_Items[i][j].m_Ratio = ((double)g_Items[i][j].m_Price) / (double) g_Items[i][j].m_Weight;
		}

	    i++;
	}

	filepath = "corr_res.txt";

	ifstream file2 (filepath.c_str());
	if (!file2.is_open()) {
		cout << "File 2 " << filepath << " could not be loaded!" << endl;
		return false;
	}

	i = 0;
	while (getline (file2, line)) {
		istringstream iss(line);
		iss >> g_CorrectResults[i++];
	}

	return true;
}

/**
 * Free used memory
 */
void deinit() {
	if (g_Ids != NULL) delete [] g_Ids;
	if (g_Results != NULL) delete [] g_Results;
	if (g_CorrectResults != NULL) delete g_CorrectResults;
	if (g_Deviations != NULL) delete g_Deviations;

	if (g_Items != NULL) {
		for (int i = 0; i < ROWS_COUNT; i++) 
			if (g_Items[i] != NULL) 
				delete [] g_Items[i];
		delete [] g_Items;
	}
}

int compare (const void * a, const void * b)
{
	TItem item1 = *(TItem*) a;
	TItem item2 = *(TItem*) b;

	double scale = 0.00001, fixscale = 10000;
	double val1 = (int)((double)item2.m_Ratio / scale) * scale;
	double val2 = (int)((double)item1.m_Ratio / scale) * scale;
  	return val1*fixscale - val2*fixscale;
}

/**
 * Calculate problem of an instance
 * PriceWeightRatio algorithm
 * @param Row number of an instance
 * @return Best calculated result of this instance
 */
int calculate(int pos) {
	int bestPrice = 0;
	int remainingWeight = g_M;

	//printItem(pos); 
	for (int i = 0; i < g_N; i++) {
		if (g_Items[pos][i].m_Weight <= remainingWeight) {
			bestPrice += g_Items[pos][i].m_Price;
			remainingWeight -= g_Items[pos][i].m_Weight;
		}
	}

	return bestPrice;
}
/**
 * Main function of the program
 */
int main(int argc,  char **argv) {
	cout << "dostam13 MI-PAA task 01 - price/weight ratio alg. v2" << endl;

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
		//printItems(4);
		//for (int j=0; j<g_N; j++)
	    //   printf ("%f ",g_Items[currentInstance][j].m_Ratio);
	   //cout << endl;
		qsort (g_Items[currentInstance], g_N, sizeof(TItem), compare);
		g_Results[currentInstance] = calculate(currentInstance);
		g_Deviations[currentInstance] = ((abs (g_Results[currentInstance] - g_CorrectResults[currentInstance])) 
										/ (double)g_CorrectResults[currentInstance]) * 100;
		if (g_maxDeviation < g_Deviations[currentInstance])
			g_maxDeviation = g_Deviations[currentInstance];

		totalAverageDeviation += g_Deviations[currentInstance];
		printf("#%d: %d, deviation: %f\n", g_Ids[currentInstance], g_Results[currentInstance], g_Deviations[currentInstance]);
	}

	totalAverageDeviation /= (double) ROWS_COUNT;
	printf("Total average deviation: %f\n", totalAverageDeviation);
	printf("Max deviation: %f\n", g_maxDeviation);

	// get final time
    double resTime = (clock() - S) / (double) CLOCKS_PER_SEC;
    cout << "Total time: " << resTime << endl;

	deinit();
	cout << endl << "== Run finished ==" << endl;
}
