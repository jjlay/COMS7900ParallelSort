#include "importFiles.h"

#include <vector>

#include <string>


int main(int argc, char *argv[]) {

	std::vector<std::string> test;

	test.push_back("datafile00302.txt");
	test.push_back("datafile00401.txt");
	test.push_back("datafile00501.txt");

	double *d;

	int rows = 0, cols = 0;

	importFiles(test, 0, d, &rows, &cols);


	return 0;
}
