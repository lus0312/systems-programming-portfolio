#ifndef SCALARCONVERTER_HPP
# define SCALARCONVERTER_HPP

# include <string>

class ScalarConverter {

private:
	ScalarConverter();
	ScalarConverter(const ScalarConverter& other);
	ScalarConverter& operator=(const ScalarConverter& other);
	~ScalarConverter();

public:
enum eType {
    CHAR,
    INT,
    FLOAT,
    DOUBLE,
    PSEUDO_FLOAT,
    PSEUDO_DOUBLE,
    UNKNOWN
};
static void convert(const std::string& literal);
};

#endif
