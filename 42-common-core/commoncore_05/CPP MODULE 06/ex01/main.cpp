#include "Serializer.hpp"
#include "Data.hpp"
#include <iostream>

int main(){
Data d;
d.num = 42;
d.name = "Marvin";

Data* ptr = &d;
std::cout << "\033[1;38;2;109;212;136m===Pointer comparison===\033[0m\n";
std::cout<<"original pointer: "<<ptr<<std::endl;
uintptr_t u = Serializer::serialize(ptr);
std::cout<<"serialized(integer): "<<u<<std::endl;
Data* rptr = Serializer::deserialize(u);
std::cout<<"Deserialized pointer: "<<rptr<<std::endl;
if (ptr == rptr)
    std::cout << "same pointer\n";
else
    std::cout << "different pointer\n";
std::cout << "\033[1;38;2;109;212;136m===Data verification===\033[0m\n";
std::cout << "ptr->num:   " << ptr->num << std::endl;
std::cout << "rptr->num:  " << rptr->num << std::endl;
std::cout << "ptr->name:  " << ptr->name << std::endl;
std::cout << "rptr->name: " << rptr->name << std::endl;

return 0;
}
