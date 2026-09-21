#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>
#include "Array.hpp"

#define MAX_VAL 750

static void print_title(const std::string& title)
{
    std::cout << "\033[1;38;2;109;212;136m=== "
              << title
              << " ===\033[0m\n";
}

int main(void)
{
    print_title("random fill and operator[] write test");
    Array<int> numbers(MAX_VAL);
    int* mirror = new int[MAX_VAL];
    srand(time(NULL));
    for (int i = 0; i < MAX_VAL; i++)
    {
        const int value = rand();
        numbers[i] = value;
        mirror[i] = value;
    }
    for (int i = 0; i < MAX_VAL; i++)
    {
        if (numbers[i] != mirror[i])
        {
            std::cerr << "FAILED: numbers and mirror have different values" << std::endl;
            delete [] mirror;
            return 1;
        }
    }
    std::cout << "OK: numbers and mirror have the same random values" << std::endl;

    print_title("copy constructor scope test");
    {
        Array<int> tmp = numbers;
        Array<int> test(tmp);
    }
    for (int i = 0; i < MAX_VAL; i++)
    {
        if (mirror[i] != numbers[i])
        {
            std::cerr << "FAILED: numbers was changed or invalidated after copied objects were destroyed" << std::endl;
            delete [] mirror;
            return 1;
        }
    }
    std::cout << "OK: numbers is still valid after copied objects were destroyed" << std::endl;

    print_title("negative index exception test");
    try
    {
        numbers[-2] = 0;
        std::cerr << "FAILED: numbers[-2] did not throw an exception" << std::endl;
        delete [] mirror;
        return 1;
    }
    catch (const std::exception& e)
    {
        std::cout << "OK: numbers[-2] threw exception: " << e.what() << std::endl;
    }

    print_title("out of range index exception test");
    try
    {
        numbers[MAX_VAL] = 0;
        std::cerr << "FAILED: numbers[MAX_VAL] did not throw an exception" << std::endl;
        delete [] mirror;
        return 1;
    }
    catch (const std::exception& e)
    {
        std::cout << "OK: numbers[MAX_VAL] threw exception: " << e.what() << std::endl;
    }

    print_title("writable after copy destruction test");
    for (int i = 0; i < MAX_VAL; i++)
    {
        numbers[i] = rand();
    }
    std::cout << "OK: numbers is still writable after copied objects were destroyed" << std::endl;

    print_title("empty array test");
    Array<int> empty;
    if (empty.size() != 0)
    {
        std::cerr << "FAILED: empty.size() is not 0" << std::endl;
        delete [] mirror;
        return 1;
    }
    std::cout << "OK: empty.size() == 0" << std::endl;
    try
    {
        empty[0] = 1;
        std::cerr << "FAILED: empty[0] did not throw an exception" << std::endl;
        delete [] mirror;
        return 1;
    }
    catch (const std::exception& e)
    {
        std::cout << "OK: empty[0] threw exception: " << e.what() << std::endl;
    }

    print_title("string type template test");
    Array<std::string> words(3);
    words[0] = "hello";
    words[1] = "template";
    words[2] = "array";
    if (words[0] != "hello" || words[1] != "template" || words[2] != "array")
    {
        std::cerr << "FAILED: Array<std::string> did not store values correctly" << std::endl;
        delete [] mirror;
        return 1;
    }
    std::cout << "OK: Array<std::string> stores and reads strings correctly" << std::endl;

    print_title("assignment operator deep copy test");
    Array<int> a(2);
    Array<int> b(5);
    a[0] = 1;
    a[1] = 2;
    b = a;
    b[1] = 777;
    if (a[1] != 2 || b[1] != 777 || b.size() != a.size())
    {
        std::cerr << "FAILED: assignment operator does not perform deep copy correctly" << std::endl;
        delete [] mirror;
        return 1;
    }
    std::cout << "OK: assignment operator performs deep copy" << std::endl;
    std::cout << "a[1]: " << a[1] << std::endl;
    std::cout << "b[1]: " << b[1] << std::endl;
    std::cout << "b size: " << b.size() << std::endl;

    print_title("const operator[] test");
    Array<int> original(2);
    original[0] = 10;
    original[1] = 20;
    const Array<int> constCopy(original);
    if (constCopy[0] != original[0] || constCopy[1] != original[1])
    {
        std::cerr << "FAILED: const Array could not read values correctly" << std::endl;
        delete [] mirror;
        return 1;
    }
    std::cout << "OK: const Array can read values through const operator[]" << std::endl;
    // constCopy[0] = 123;

    delete [] mirror;
    return 0;
}