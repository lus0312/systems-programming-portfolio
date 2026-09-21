#ifndef BITCOINEXCHANGE_HPP
# define BITCOINEXCHANGE_HPP
# include <map>
# include <string>

class BitcoinExchange{
private:
	std::map<std::string, double> _db;
	void	loadDatabase();
public:
	BitcoinExchange();
	BitcoinExchange(const BitcoinExchange& other);
	~BitcoinExchange();
	BitcoinExchange& operator=(const BitcoinExchange& other);

	bool processInputFile(const std::string& filename);
};

#endif
