#include <iostream>
#include <iomanip>
#include <random>
#include <vector>

#define _OKAY_   0



int testBinSizing(int argc, char *argv[])
{
	const int numVals = 10000;
	const int numBins = 5;

	double min = 0.0, max = 0.0;

	std::vector<double> vals;

	auto binEndVals = new double[numBins];
	auto newBinEndVals = new double[numBins];
	auto binCount = new int[numBins];


	//
	// Generate random values
	//

	std::cout << "Generating random numbers" << std::endl;

	std::default_random_engine generator;
	std::normal_distribution<double> normalDistribution(0, 1);
	std::uniform_real_distribution<double> uniformDistribution(0.0, 0.4);

	for (auto i = 0; i < numVals; i++)
		vals.push_back(normalDistribution(generator));


	// Sort
	std::sort(vals.begin(), vals.end());


	//
	// Find the min value
	//

	min = vals[0];
	max = vals[vals.size() - 1];
	double binWidth = (max - min) / static_cast<double>(numBins);

	std::cout << "Minimum was " << min << std::endl
		<< "Maximum was " << max << std::endl
		<< "Range is " << max - min << std::endl
		<< "Number of bins " << numBins << std::endl
		<< "Bin width " << binWidth << std::endl;

	//
	// Initialize bin edges
	//

	std::cout << std::endl << "==================" << std::endl << std::endl;

	for (auto i = 0; i < numBins; i++) {
		binEndVals[i] = min + binWidth * static_cast<double>(i + 1);
		newBinEndVals[i] = binEndVals[i];
		std::cout << "Bin [" << std::fixed << std::setprecision(0) << i << "] " << std::setprecision(4) << binEndVals[i] << std::endl;
	}

	binEndVals[numBins - 1] = max;

	//
	// Distribute the data
	//

	int iterations = 1 + numBins / 2;
	int goal = numVals / numBins;
	int minGoal = (97 * numVals) / (100 * numBins);
	int maxGoal = (103 * numVals) / (100 * numBins);

	std::cout << "Goal is between " << minGoal << " and " << maxGoal << std::endl;

	for (auto i = 0; i < numBins; i++)
		binCount[i] = 0;

	for (auto v : vals)
	{
		for (auto b = 0; b < numBins; b++)
			if (v < binEndVals[b])
			{
				binCount[b]++;
				b = numBins + 1;
			}
	}


	int loop = 0;

	while ((binCount[0] < minGoal) || (binCount[0] > maxGoal))
	{		
		// Lower edge
		loop++;

		auto shift = uniformDistribution(generator);

		if (binCount[0] < minGoal)
			newBinEndVals[0] = binEndVals[0] + shift * (binEndVals[1] - binEndVals[0]);

		if (binCount[0] > maxGoal)
			newBinEndVals[0] = binEndVals[0] - shift * (binEndVals[1] - binEndVals[0]);

		binEndVals[0] = newBinEndVals[0];

		for (auto i = 0; i < numBins; i++)
			binCount[i] = 0;

		for (auto i = 0; i < numVals; i++) {
			if (vals[i] < binEndVals[0])
				binCount[0]++;
			else
				if (vals[i] < binEndVals[1])
					binCount[1]++;
				else
					i = numVals;
		}
	}


	std::cout << std::endl << "Finished lower edge in " << loop << " loops" << std::endl;

	for (auto b = 0; b < numBins; b++)
	{
		std::cout << "Bin [" << std::fixed << std::setprecision(0) << b
			<< "] Count " << std::setprecision(0) << binCount[b]
			<< " Goal " << numVals / numBins
			<< " Edge " << std::setprecision(4) << binEndVals[b]
			<< std::endl;
	}

	loop = 0;

	while ((binCount[numBins-1] < minGoal) || (binCount[numBins-1] > maxGoal))
	{
		// Upper edge
		loop++;

		auto shift = uniformDistribution(generator);

		if (binCount[numBins - 1] < minGoal)
			newBinEndVals[numBins - 2] = binEndVals[numBins - 2] + shift * (binEndVals[numBins - 2] - binEndVals[numBins - 1]);

		if (binCount[numBins - 1] > maxGoal)
			newBinEndVals[numBins - 2] = binEndVals[numBins - 2] - shift * (binEndVals[numBins - 2] - binEndVals[numBins - 1]);

		binEndVals[numBins - 2] = newBinEndVals[numBins - 2];

		for (auto i = 0; i < numBins; i++)
			binCount[i] = 0;

		for (auto v : vals)
		{
			for (auto b = 0; b < numBins; b++)
				if (v < binEndVals[b])
				{
					binCount[b]++;
					b = numBins + 1;
				}
		}
	}


	std::cout << std::endl << "Finished upper edge in " << loop << " loops" << std::endl;

	for (auto b = 0; b < numBins; b++)
	{
		std::cout << "Bin [" << std::fixed << std::setprecision(0) << b
			<< "] Count " << std::setprecision(0) << binCount[b]
			<< " Goal " << numVals / numBins
			<< " Edge " << std::setprecision(4) << binEndVals[b]
			<< std::endl;
	}


	loop = 0;

	while ((binCount[1] < minGoal) || (binCount[1] > maxGoal))
	{
		// Next lower edge
		loop++;

		auto shift = uniformDistribution(generator);

		if (binCount[1] < minGoal)
			newBinEndVals[1] = binEndVals[1] + shift * (binEndVals[2] - binEndVals[1]);

		if (binCount[1] > maxGoal)
			newBinEndVals[1] = binEndVals[1] - shift * (binEndVals[2] - binEndVals[1]);

		binEndVals[1] = newBinEndVals[1];

		for (auto i = 0; i < numBins; i++)
			binCount[i] = 0;

		for (auto v : vals)
		{
			for (auto b = 0; b < numBins; b++)
				if (v < binEndVals[b])
				{
					binCount[b]++;
					b = numBins + 1;
				}
		}
	}


	std::cout << std::endl << "Finished next lower edge in " << loop << " loops" << std::endl;

	for (auto b = 0; b < numBins; b++)
	{
		std::cout << "Bin [" << std::fixed << std::setprecision(0) << b
			<< "] Count " << std::setprecision(0) << binCount[b]
			<< " Goal " << numVals / numBins
			<< " Edge " << std::setprecision(4) << binEndVals[b]
			<< std::endl;
	}


	//
	// Wrap up
	//

	delete binEndVals;
	delete binCount;

	return _OKAY_;
}