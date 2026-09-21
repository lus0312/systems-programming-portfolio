#include "Span.hpp"
#include <iostream>
#include <vector>
#include <climits>

static void print_title(const std::string& title)
{
    std::cout << "\033[1;38;2;109;212;136m=== "
              << title
              << " ===\033[0m\n";
}

static void testSubjectExample()
{
	Span sp(5);

	print_title("Subject example");

	sp.addNumber(6);
	sp.addNumber(3);
	sp.addNumber(17);
	sp.addNumber(9);
	sp.addNumber(11);

	std::cout << "shortest span: " << sp.shortestSpan() << std::endl;
	std::cout << "longest span : " << sp.longestSpan() << std::endl;
}

static void testNotEnoughNumbers()
{
	Span sp(5);
	print_title("Not enough numbers");

	try
	{
		std::cout << sp.shortestSpan() << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}

	sp.addNumber(1);

	try
	{
		std::cout << sp.longestSpan() << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}
}

static void testFullSpan()
{
	Span sp(2);
	print_title("Full span");

	try
	{
		sp.addNumber(10);
		sp.addNumber(20);
		sp.addNumber(30);
	}
	catch (const std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}
}

static void testRangeAdd()
{
	Span sp(10);
	std::vector<int> v;
	print_title("Range add");

	v.push_back(1);
	v.push_back(4);
	v.push_back(5);
	v.push_back(10);

	sp.addNumbers(v.begin(), v.end());

	std::cout << "shortest span: " << sp.shortestSpan() << std::endl;
	std::cout << "longest span : " << sp.longestSpan() << std::endl;
}

static void testRangeAddOverflow()
{
	Span sp(3);
	std::vector<int> v;
	print_title("Range add overflow");

	v.push_back(1);
	v.push_back(2);
	v.push_back(3);
	v.push_back(4);

	try
	{
		sp.addNumbers(v.begin(), v.end());
	}
	catch (const std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}
}

static void testDuplicateValues()
{
	Span sp(5);
	print_title("Duplicate values");

	sp.addNumber(5);
	sp.addNumber(5);
	sp.addNumber(10);

	std::cout << "shortest span: " << sp.shortestSpan() << std::endl;
	std::cout << "longest span : " << sp.longestSpan() << std::endl;
}

static void testNegativeValues()
{
	Span sp(5);
	print_title("Negative values");

	sp.addNumber(-10);
	sp.addNumber(-3);
	sp.addNumber(-7);

	std::cout << "shortest span: " << sp.shortestSpan() << std::endl;
	std::cout << "longest span : " << sp.longestSpan() << std::endl;
}

static void testIntLimits()
{
	Span sp(5);
	print_title("INT_MIN / INT_MAX");

	sp.addNumber(INT_MIN);
	sp.addNumber(INT_MAX);
	sp.addNumber(0);

	std::cout << "shortest span: " << sp.shortestSpan() << std::endl;
	std::cout << "longest span : " << sp.longestSpan() << std::endl;
}

static void testManyNumbers()
{
	Span sp(10000);
	std::vector<int> v;
	int i;
	print_title("10000 numbers");

	i = 0;
	while (i < 10000)
	{
		v.push_back(i * 2);
		i++;
	}

	sp.addNumbers(v.begin(), v.end());

	std::cout << "shortest span: " << sp.shortestSpan() << std::endl;
	std::cout << "longest span : " << sp.longestSpan() << std::endl;
}

int main()
{
	testSubjectExample();
	testNotEnoughNumbers();
	testFullSpan();
	testRangeAdd();
	testRangeAddOverflow();
	testDuplicateValues();
	testNegativeValues();
	testIntLimits();
	testManyNumbers();

	return 0;
}