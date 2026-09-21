#ifndef SPAN_HPP
# define SPAN_HPP
# include <vector>
# include <iterator>
# include <exception>

class Span{
private:
	std::vector<int> _numbers;
	unsigned int _maxSize;

public:
	Span();
	Span(unsigned int size);
	Span(const Span& other);
	Span& operator=(const Span& other);
	~Span();

	void addNumber(int num);
	unsigned int shortestSpan() const;
	unsigned int longestSpan() const;
	
	class spanFullException : public std::exception{
	public:
		virtual const char* what() const throw();
	};

	class notEnoughNumbersException : public std::exception{
	public:
		virtual const char* what() const throw();
	};
	
	template <typename Iterator>
	void addNumbers(Iterator begin, Iterator end);
};

template <typename Iterator>
void Span::addNumbers(Iterator begin, Iterator end){
	unsigned int count;

	count = std::distance(begin, end);
	if (_numbers.size() + count > _maxSize)
		throw spanFullException();
	_numbers.insert(_numbers.end(), begin, end);
}

#endif