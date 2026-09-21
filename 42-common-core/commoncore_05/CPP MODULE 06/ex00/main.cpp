#include "ScalarConverter.hpp"
#include <iostream>

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        std::cout << "Usage: ./convert <literal>" << std::endl;
        return 1;
    }
    ScalarConverter::convert(argv[1]);
    return 0;
}
//int main(void)
//{
//    const char* tests[] = {
//        "0", "42", "-42", "'a'", "42.0f", "42.11f",
//        "-4.2f", "42.0", "42.11", "nan", "nanf",
//        "+inf", "-inf", "+inff", "-inff"
//    };
//    int size = sizeof(tests) / sizeof(tests[0]);
//    for (int i = 0; i < size; i++)
//    {
//        std::cout << "--- " << tests[i] << " ---" << std::endl;
//        ScalarConverter::convert(tests[i]);
//        std::cout << std::endl;
//    }
//    return 0;
//}