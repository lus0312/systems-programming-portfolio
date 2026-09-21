#include "iter.hpp"

template <typename T>
void printElement(const T& value)
{
	std::cout << value << std::endl;
}

void increment(int& value)
{
	value += 1;
}

void appendExclamation(std::string& value)
{
	value += "!";
}

int main()
{
	int numbers[5] = {1, 2, 3, 4, 5};
	const int constNumbers[5] = {9, 8, 7, 6, 5};
	std::string words[3] = {"abc", "def", "ghi"};
	const std::string constWords[3] = {"hello", "template", "world"};

    std::cout << "\033[1;38;2;109;212;136m===print non-const int array===\033[0m\n";
	iter(numbers, 5, printElement<int>);

    std::cout << "\033[1;38;2;109;212;136m===modify non-const int array===\033[0m\n";

	iter(numbers, 5, increment);
	iter(numbers, 5, printElement<int>);

    std::cout << "\033[1;38;2;109;212;136m===print const int array===\033[0m\n";
	iter(constNumbers, 5, printElement<int>);
	//iter(constNumbers, 5, increment);

    std::cout << "\033[1;38;2;109;212;136m===print non-const string array===\033[0m\n";
	
	iter(words, 3, printElement<std::string>);

    std::cout << "\033[1;38;2;109;212;136m===modify non-const string array===\033[0m\n";
	
	iter(words, 3, appendExclamation);
	iter(words, 3, printElement<std::string>);

    std::cout << "\033[1;38;2;109;212;136m===print const string array===\033[0m\n";
	
	iter(constWords, 3, printElement<std::string>);
	//iter(constWords, 3, appendExclamation);

	return (0);
}
