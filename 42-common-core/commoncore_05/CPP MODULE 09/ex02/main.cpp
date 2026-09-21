#include "PmergeMe.hpp"
#include <iostream>
#include <climits>
#include <cstdlib>
#include <stdexcept>
#include <algorithm>
static bool isValid(const char* str, long& num){
	int i = 0;
	if (str == NULL || str[0] == '\0')
		return false;
	while (str[i] != '\0'){
		if (str[i] < '0' || str[i] > '9')
			return false;
		i++;
	}
	num = std::strtol(str, NULL, 10);
	if (num <= 0 || num > INT_MAX)
		return false;
	return true;
}

static void parseInput(int argc, char** argv, std::vector<int>& input){
	int i = 1;
	long num;
	while (i < argc){
		if (!isValid(argv[i], num))
			throw std::runtime_error("Error");
		if (std::find(input.begin(), input.end(), static_cast<int>(num)) != input.end())
			throw std::runtime_error("Error");
		input.push_back(static_cast<int>(num));
		i++;
	}
}

int main(int argc, char** argv){
	try{
		if (argc < 2)
			throw std::runtime_error("Error");
		std::vector<int> input;
		parseInput(argc, argv, input);
		PmergeMe sorter(input);
		sorter.process();
		sorter.printResults();
	}
	catch(std::exception &e){
		std::cerr<<e.what()<<std::endl;
		return 1;
	}
	return 0;
}
