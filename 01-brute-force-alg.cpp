#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string.h>

using namespace std;

int g_N = 0;
// array of weights
int * g_Weight; 
// array of prices
int * g_Price; 
// array of unique ids
int * g_Ids;

void initialize(string filepath) {

}

/**
 * Main function of the program
 */
int main(int argc,  char **argv) {
	cout << "dostam13 Mi-PAA task 01 - brute force alg." << endl;

	if (argc < 2) {
		cout << "Error: Instance name is required (eg. knap_4 to read knap_4.inst.dat)" << endl;
		return 1;
	}
	string filepath = "inst/"; 
	filepath += argv[1];
	filepath += ".inst.dat";
	initialize(filepath);

	cout << filepath << endl;

	cout << "Run finished" << endl;
}
