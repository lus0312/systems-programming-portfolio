#include "MutantStack.hpp"
#include <iostream>
#include <list>

static void print_title(const std::string& title)
{
    std::cout << "\033[1;38;2;109;212;136m=== "
              << title
              << " ===\033[0m\n";
}

void subject_test()
{
	MutantStack<int> mstack;

	mstack.push(5);
	mstack.push(17);

	std::cout << mstack.top() << std::endl;

	mstack.pop();

	std::cout << "size: "<<mstack.size() << std::endl;

	mstack.push(3);
	mstack.push(5);
	mstack.push(737);
	mstack.push(0);

	MutantStack<int>::iterator it = mstack.begin();
	MutantStack<int>::iterator ite = mstack.end();

	++it;
	--it;

	while (it != ite)
	{
		std::cout << *it << std::endl;
		++it;
	}

	std::stack<int> s(mstack);
}

void copy_test()
{
	MutantStack<int> a;

	a.push(1);
	a.push(2);
	a.push(3);

	MutantStack<int> b(a);

	std::cout << "copy top: " << b.top() << std::endl;
	std::cout << "copy size: " << b.size() << std::endl;
}

void assignment_test()
{
	MutantStack<int> a;
	MutantStack<int> b;

	a.push(10);
	a.push(20);

	b.push(1);
	b = a;

	std::cout << "assign top: " << b.top() << std::endl;
	std::cout << "assign size: " << b.size() << std::endl;
}

void const_iterator_test()
{
	MutantStack<int> a;

	a.push(4);
	a.push(5);
	a.push(6);

	const MutantStack<int> b(a);

	MutantStack<int>::const_iterator it = b.begin();
	MutantStack<int>::const_iterator ite = b.end();

	while (it != ite)
	{
		std::cout << *it << std::endl;
		++it;
	}
}

void compare_with_list_test()
{
	MutantStack<int> mstack;
	std::list<int> lst;

	mstack.push(5);
	lst.push_back(5);

	mstack.push(17);
	lst.push_back(17);

	mstack.pop();
	lst.pop_back();

	mstack.push(3);
	lst.push_back(3);

	mstack.push(5);
	lst.push_back(5);

	mstack.push(737);
	lst.push_back(737);

	mstack.push(0);
	lst.push_back(0);

	MutantStack<int>::iterator mit = mstack.begin();
	MutantStack<int>::iterator mite = mstack.end();
	std::list<int>::iterator lit = lst.begin();
	std::list<int>::iterator lite = lst.end();

	while (mit != mite && lit != lite)
	{
		std::cout << "mutant: " << *mit << " / list: " << *lit << std::endl;
		++mit;
		++lit;
	}
}

int main()
{
	print_title("subject test");
	subject_test();

	print_title("copy test");
	copy_test();

	print_title("assignment test");
	assignment_test();

	print_title("const iterator test");
	const_iterator_test();

	print_title("compare with list test ===");
	compare_with_list_test();

	return (0);
}