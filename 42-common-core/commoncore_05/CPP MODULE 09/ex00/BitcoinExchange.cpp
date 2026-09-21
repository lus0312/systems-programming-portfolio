#include "BitcoinExchange.hpp"
#include <fstream>
#include <iostream>
#include <sstream>
#include <cctype>

BitcoinExchange::BitcoinExchange(){
	loadDatabase();
}
BitcoinExchange::BitcoinExchange(const BitcoinExchange& other): _db(other._db) 
{
}
BitcoinExchange::~BitcoinExchange(){}
BitcoinExchange& BitcoinExchange::operator=(const BitcoinExchange& other){
	if (this != &other)
		_db = other._db;
	return *this;
}

void BitcoinExchange::loadDatabase(){
	std::ifstream file;
	file.open("data.csv");
	if (!file.is_open()){
		std::cerr <<"Error: could not open data.csv"<<std::endl;
		return ;
	}
	std::string line;
	std::getline(file, line);
	while (std::getline(file,line))
	{
		std::string date;
		std::string rateStr;
		double		rate;
		std::stringstream ss(line);
		std::getline(ss,date, ',');
		std::getline(ss,rateStr);
		std::stringstream rateStream(rateStr);
		rateStream >> rate;
		_db[date] = rate; 
	}
}

//윤년(연도가 4로 나누어 떨어지고, 100으로 나누어 떨어지지 않는 해 or 400으로 나누어 떨어지는 해)
static bool	isLeapYear(int year)
{
	if (year % 400 == 0)
		return true;
	if (year % 4 == 0 && year % 100 != 0)
		return true;
	return false;
}

static int	toInt(const std::string& str)
{
	std::stringstream	ss;
	int					n;

	ss.str(str);
	ss >> n;
	return n;
}

static bool	isDateDigitPart(const std::string& date)
{
	for (int i = 0; i < 10; i++)
	{
		if (i != 4 && i != 7 && !std::isdigit(static_cast<int>(date[i])))
			return false;
	}
	return true;
}

static bool	isValidDate(const std::string& date)
{
	if (date.size() != 10)
		return false;
	if (date[4] != '-' || date[7] != '-')
		return false;
	if (!isDateDigitPart(date))
		return false;
	int month = toInt(date.substr(5, 2));
	if (month < 1 || month > 12)
		return false;
	int year = toInt(date.substr(0, 4));
	int	daysInMonth[12] = {31, 28, 31, 30, 31, 30,	31, 31, 30, 31, 30, 31};
	if (isLeapYear(year))
		daysInMonth[1] = 29;
	int day = toInt(date.substr(8, 2));
	if (day < 1 || day > daysInMonth[month - 1])
		return false;
	return true;
}

static bool parseValue(const std::string& valueStr, double& value, const std::string& line)
{
	std::stringstream	ss;
	char				remain;

	ss.str(valueStr);
	ss >> value;
	if (ss.fail()){ 
		std::cerr << "Error: bad input => " << line << std::endl;
		return false;	
	}
	if(ss >>remain){ 
		std::cerr << "Error: bad input => " << line << std::endl;
		return false;
	}
	if (value < 0){
		std::cerr<<"Error: not a positive number."<<std::endl;
		return false;
	}
	if (value > 1000){
		std::cerr<<"Error: too large a number."<<std::endl;
		return false;
	}
	return true;
}

static std::string trim(const std::string& original){
	std::string::size_type start;
	std::string::size_type end;
	start = original.find_first_not_of(" \t\n\r");
	end = original.find_last_not_of(" \t\n\r");
	if (start == std::string::npos)
		return "";
	return original.substr(start, end -start + 1);
}

bool BitcoinExchange::processInputFile(const std::string& filename){
	std::ifstream file;
	std::string line;
	std::string date;
	std::string valueStr;
	double value;

	file.open(filename.c_str());
	if (!file.is_open()){
		std::cerr<<"Error: could not open file."<<std::endl;
		return false;
	}
	std::getline(file,line);
	while (std::getline(file,line)){
		std::stringstream ss(line);
		if(!std::getline(ss,date,'|') || !std::getline(ss,valueStr)){
			std::cerr << "Error: bad input => " << line << std::endl;
			continue;
		}
		date = trim(date);
		valueStr = trim(valueStr);
		if (!isValidDate(date)){
			std::cerr << "Error: bad input => " << line << std::endl;
			continue;
		}
		if (!parseValue(valueStr, value, line))
			continue;
		std::map<std::string, double>::iterator it;
		it = _db.upper_bound(date);
		if (it == _db.begin())
		{
			std::cerr << "Error: bad input => "<< line << std::endl;
			continue;
		}
		--it;
		std::cout <<date<<" => "<<value<<" = "<<it->second * value<<std::endl;
	}
	return true;
}

