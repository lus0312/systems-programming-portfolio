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

	bool operator>(const Fixed& other) const;
	bool operator<(const Fixed& other) const;
	bool  operator>=(const Fixed& other) const;
	bool operator<=(const Fixed& other) const;
	bool operator==(const Fixed& other) const;
	bool operator!=(const Fixed& other) const;
	Fixed operator+(const Fixed& other) const;
	Fixed operator-(const Fixed& other) const;
	Fixed operator*(const Fixed& other) const;
	Fixed operator/(const Fixed& other) const;
	Fixed& operator++();
	Fixed operator++(int);
	Fixed& operator--();
	Fixed operator--(int);
	static Fixed& min(Fixed& a, Fixed& b);
	static const Fixed& min(const Fixed& a, const Fixed& b);
	static Fixed& max(Fixed& a, Fixed& b);
	static const Fixed& max(const Fixed& a, const Fixed& b);
};

std::ostream& operator<<(std::ostream& out, const Fixed& fixed);

#endif