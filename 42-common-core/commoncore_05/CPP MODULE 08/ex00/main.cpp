#include "easyfind.hpp"
#include <iostream>
#include <vector>
#include <list>
#include <deque>
#include <string>
#include <exception>
#include <iterator>

template <typename T>
void	testFind(const std::string& testName, T& container, int target)
{
	typename T::iterator it;

	std::cout << "[" << testName << "] find " << target << " -> ";
	try
	{
		it = easyfind(container, target);
		std::cout << "found: " << *it << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cout << "exception: " << e.what() << std::endl;
	}
}

template <typename T>
void	testConstFind(const std::string& testName, const T& container, int target)
{
	typename T::const_iterator it;

	std::cout << "[" << testName << "] find " << target << " -> ";
	try
	{
		it = easyfind(container, target);
		std::cout << "found: " << *it << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cout << "exception: " << e.what() << std::endl;
	}
}

void	fillVector(std::vector<int>& v)
{
	v.push_back(1);
	v.push_back(2);
	v.push_back(3);
}

void	fillList(std::list<int>& l)
{
	l.push_back(10);
	l.push_back(20);
	l.push_back(30);
}

void	fillDeque(std::deque<int>& d)
{
	d.push_back(100);
	d.push_back(200);
	d.push_back(300);
}

void	testFirstOccurrence()
{
	std::vector<int> v;
	std::vector<int>::iterator it;

	v.push_back(4);
	v.push_back(7);
	v.push_back(7);
	v.push_back(9);

	std::cout << "[duplicate vector] find 7 -> ";
	try
	{
		it = easyfind(v, 7);
		std::cout << "found: " << *it;
		std::cout << ", index: " << std::distance(v.begin(), it) << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cout << "exception: " << e.what() << std::endl;
	}
}

int	main()
{
	std::vector<int> v_con;
	std::list<int> l_con;
	std::deque<int> d_con;
	std::vector<int> empty_v;

	fillVector(v_con);
	fillList(l_con);
	fillDeque(d_con);

	const std::vector<int> const_v(v_con);

	testFind("vector success", v_con, 2);
	testFind("vector fail", v_con, 99);
	testConstFind("const vector success", const_v, 2);
	testConstFind("const vector fail", const_v, 99);

	testFind("list success", l_con, 30);
	testFind("list fail", l_con, 999);
	
	testFind("deque success", d_con, 200);
	testFind("deque fail", d_con, 500);
	
	testFind("empty vector", empty_v, 1);
	
	testFirstOccurrence();

	return 0;
}