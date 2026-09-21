#ifndef RPN_HPP
# define RPN_HPP
# include <stack>
# include <string>

class RPN{
private:
	std::stack<int> _stack;
	bool	operate(int left, int right, char token);
public:
	RPN();
	RPN(const RPN& other);
	~RPN();
	RPN& operator=(const RPN& other);

	bool	process(const std::string& input);
private:
};

#endif