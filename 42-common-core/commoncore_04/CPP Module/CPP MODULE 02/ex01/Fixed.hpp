#ifndef FIXED_HPP
# define FIXED_HPP

# include <iostream>

class Fixed{
private:
	int		_value;
	static const int	_fractionalBits = 8;
public:
	Fixed();
	Fixed(const Fixed& other);
	Fixed(const int n);
	Fixed(const float f);
	~Fixed();

	Fixed& operator=(const Fixed& other);
	int toInt(void) const;
	float toFloat(void) const;
	int getRawBits(void) const;
	void setRawBits(int const raw);
};

std::ostream& operator<<(std::ostream& out, const Fixed& fixed);

#endif