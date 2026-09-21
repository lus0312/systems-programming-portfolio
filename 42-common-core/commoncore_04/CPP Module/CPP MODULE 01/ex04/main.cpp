#include <iostream>
#include <fstream>
#include <sstream>                                                             

int main(int argc, char** argv){
	if (argc != 4){
		std::cerr <<"Usage: ./ex04 filename s1 s2" <<std::endl;
		return 1;
	}
	std::ifstream inFile(argv[1]);
	if (!inFile.is_open()){
		std::cerr << "Error: cannot open file"<<std::endl;
		return 1;
	}
	std::string s1 = argv[2];
	std::string s2 = argv[3];
	if (s1.empty()){
		std::cerr <<" Error: s1 cannot be empty!"<<std::endl;
		return 1;
	}
	std::string outFileName = std::string(argv[1]) + ".replace";
	std::ofstream outFile(outFileName.c_str());

	std::stringstream buffer;
	buffer << inFile.rdbuf();
	std::string str = buffer.str();

	std::string result;
	size_t pos = 0;
	size_t found;
	while ((found = str.find(s1, pos)) != std::string::npos){
		result.append(str, pos, found - pos);
		result.append(s2);
		pos = found + s1.length();
	}
	result.append(str, pos, str.length() - pos);

	outFile << result;

	return 0;
}
