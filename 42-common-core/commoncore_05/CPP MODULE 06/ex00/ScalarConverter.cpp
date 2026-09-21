#include "ScalarConverter.hpp"
#include <cstdlib>
#include <cctype>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <climits>

static bool isValidNumber(const std::string& literal){
	char *end;
	std::strtod(literal.c_str(), &end);
	if (*end == '\0')
		return true;
	return false;
}

static ScalarConverter::eType detectType(const std::string& literal){
	if (literal == "nanf" || literal == "-inff" || literal == "+inff")
		return ScalarConverter::PSEUDO_FLOAT;
	else if (literal == "-inf" || literal == "+inf" || literal == "nan")
		return ScalarConverter::PSEUDO_DOUBLE;
	else if ((literal[0] == '\'' && literal.size() == 3 && literal[2] == '\'')\
	|| (literal.length() == 1 && !isdigit(literal[0])))
		return ScalarConverter::CHAR;
	else if (isValidNumber(literal) && literal.find('.') == std::string::npos){
		std::stringstream ss(literal);
		long num;
		ss >> num;
		if (ss.fail() || num < INT_MIN || num > INT_MAX )
			return ScalarConverter::DOUBLE;
		return ScalarConverter::INT;
	}
	else if (isValidNumber(literal))
		return ScalarConverter::DOUBLE;
	else{
		const std::string withoutF = literal.substr(0, literal.size() -1);
		if (literal[literal.size() -1] == 'f' && isValidNumber(withoutF) && withoutF.find('.') != std::string::npos)
			return ScalarConverter::FLOAT;
		return ScalarConverter::UNKNOWN;
	}
}

static void convertFromChar(const std::string& literal){
	char value;
	if (literal[0] == '\'')
		value = literal[1];
	else
		value = literal[0];
	if(isprint(value))
		std::cout<<"char: '"<<value<<"'"<<std::endl;
	else
		std::cout<<"char: Non displayable"<<std::endl;
	std::cout<<"int: "<<static_cast<int>(value)<<std::endl;
	std::cout << std::fixed << std::setprecision(1);
	std::cout<<"float: "<<static_cast<float>(value)<<"f"<<std::endl;
	std::cout<<"double: "<<static_cast<double>(value)<<std::endl;
}

static void convertFromInt(const std::string& literal){
	std::stringstream ss(literal);
	int value;
	ss >> value;
	if (value < 0 || value > 127)
	std::cout<<"char: impossible"<<std::endl;
	else if (isprint(static_cast<char>(value)))
		std::cout<<"char: '"<<static_cast<char>(value)<<"'"<<std::endl;
	else
		std::cout<<"char: Non displayable"<<std::endl;
	std::cout<<"int: "<<value<<std::endl;
	std::cout<<std::fixed<<std::setprecision(1);
	std::cout<<"float: "<<static_cast<float>(value)<<"f"<<std::endl;
	std::cout<<"double: "<<static_cast<double>(value)<<std::endl;
}

static void convertFromFloat(const std::string& literal){
	float value = (std::strtof(literal.substr(0, literal.size()-1).c_str(), NULL));
	if (value < 0 || value > 127 || static_cast<float>(static_cast<int>(value)) != value)
		std::cout<<"char: impossible"<<std::endl;
	else if (isprint(static_cast<char>(value)))
		std::cout<<"char: '"<<static_cast<char>(value)<<"'"<<std::endl;
	else
		std::cout<<"char: Non displayable"<<std::endl;
	if (value < INT_MIN || value > INT_MAX)
		std::cout<<"int: impossible"<<std::endl;
	else
		std::cout<<"int: "<< static_cast<int>(value)<<std::endl;
	size_t dotPos = literal.find('.');
	size_t precision = literal.size() -dotPos -2;
	std::cout<<std::fixed<<std::setprecision(precision);
	std::cout<<"float: "<<value<<"f"<<std::endl;
	std::cout<<"double: "<<static_cast<double>(value)<<std::endl;
}

static void convertFromDouble(const std::string& literal){
	double value = strtod(literal.c_str(), NULL);
	if (value < 0 || value > 127 || static_cast<double>(static_cast<int>(value)) != value)
		std::cout<<"char: impossible"<<std::endl;
	else if (isprint(static_cast<char>(value)))
		std::cout<<"char: '"<<static_cast<char>(value)<<"'"<<std::endl;
	else
		std::cout<<"char: Non displayable"<<std::endl;
	if (value < INT_MIN || value > INT_MAX)
		std::cout<<"int: impossible"<<std::endl;
	else
		std::cout<<"int: "<<static_cast<int>(value)<<std::endl;
	size_t dotPos = literal.find('.');
	size_t precision;
	if (dotPos == std::string::npos || dotPos == literal.size() -1)
    	precision = 1;
	else
   		precision = literal.size() - dotPos - 1;
	std::cout<<std::fixed<<std::setprecision(precision);
	std::cout<<"float: "<<static_cast<float>(value)<<"f"<<std::endl;
	std::cout<<"double: "<<value<<std::endl;
}

static void convertFromPSEUDO_FLOAT(const std::string& literal){
	std::cout<<"char: impossible"<<std::endl;
	std::cout<<"int: impossible"<<std::endl;
	if (literal == "nanf"){
        std::cout << "float: nanf" << std::endl;
	} else if (literal == "+inff"){
        std::cout << "float: +inff" << std::endl;
	} 	else{
        std::cout<<"float: -inff" << std::endl;
	}
	std::cout << "double: " << literal.substr(0, literal.size()-1) << std::endl;
}

static void convertFromPSEUDO_DOUBLE(const std::string& literal){
	std::cout<<"char: impossible"<<std::endl;
	std::cout<<"int: impossible"<<std::endl;
	std::cout<<"float: "<<literal<<"f"<<std::endl;
	std::cout << "double: " << literal<< std::endl;
}

static void convertFromUnkown(const std::string& ){
	std::cout<<"char: impossible"<<std::endl;
	std::cout<<"int: impossible"<<std::endl;
	std::cout<<"float: impossible"<<std::endl;
	std::cout<<"double: impossible"<< std::endl;
}

void ScalarConverter::convert(const std::string& literal){
	switch(detectType(literal)){
        case CHAR:
			convertFromChar(literal);
			break;
		case INT:
			convertFromInt(literal);
			break;
		case DOUBLE:
			convertFromDouble(literal);
			break;
		case FLOAT:
			convertFromFloat(literal);
			break;
		case PSEUDO_DOUBLE:
			convertFromPSEUDO_DOUBLE(literal);
			break;
		case PSEUDO_FLOAT:
			convertFromPSEUDO_FLOAT(literal);
			break;
		case UNKNOWN:
		default:
			convertFromUnkown(literal);
			break;
	}
}
