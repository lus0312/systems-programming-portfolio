#include "whatever.hpp"
#include <iostream>

int main(void)
{
    std::cout << "\033[1;38;2;109;212;136m===subject test===\033[0m\n";
    int a = 2;
    int b = 3;
    ::swap(a, b);
    std::cout << "a = " << a << ", b = " << b << std::endl;
    std::cout << "min(a, b) = " << ::min(a, b) << std::endl;
    std::cout << "max(a, b) = " << ::max(a, b) << std::endl;

    std::string c = "chaine1";
    std::string d = "chaine2";
    ::swap(c, d);
    std::cout << "c = " << c << ", d = " << d << std::endl;
    std::cout << "min(c, d) = " << ::min(c, d) << std::endl;
    std::cout << "max(c, d) = " << ::max(c, d) << std::endl;

    std::cout << "\033[1;38;2;109;212;136m=== equal case ===\033[0m\n";
    int e = 5;
    int f = 5;
    std::cout << "e[address] : "<< &e <<std::endl;
    std::cout << "f[address] : "<< &f <<std::endl;
    std::cout << "min(5, 5) = " << ::min(e, f) << ", address = "<<&::min(e,f)<< std::endl;
    std::cout << "max(5, 5) = " << ::max(e, f) << ", address = "<<&::max(e,f)<< std::endl;

    std::cout << "\033[1;38;2;109;212;136m=== const case test ===\033[0m\n";
    const double p = 3.14;
    const double q = 2.71;
    std::cout << "min(3.14, 2.71) = " << ::min(p, q) << std::endl;
    std::cout << "max(3.14, 2.71) = " << ::max(p, q) << std::endl;

    return 0;
}