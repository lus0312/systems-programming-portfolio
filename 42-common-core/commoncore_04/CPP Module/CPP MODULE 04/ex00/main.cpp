#include "Animal.hpp"
#include "Cat.hpp"
#include "Dog.hpp"
#include "WrongAnimal.hpp"
#include "WrongCat.hpp"

int main()
{
    std::cout << "=== Basic polymorphism test ===" << std::endl;
    const Animal* meta = new Animal();
    const Animal* i = new Dog();
    const Animal* j = new Cat();
    std::cout<<i->getType()<<": ";
    i->makeSound();
    std::cout<<j->getType()<<": ";
    j->makeSound();
    std::cout<<meta->getType()<<": ";
    meta->makeSound();
    delete meta;
    delete i;
    delete j;

    std::cout << "\n=== Reference polymorphism test ===" << std::endl;
    const Dog tmpDog;
    const Animal& ref = tmpDog;
    std::cout<<ref.getType()<<": ";
    ref.makeSound();

    std::cout << "\n=== WrongAnimal: no virtual ===" << std::endl;
    const WrongAnimal* w = new WrongAnimal();
    const WrongAnimal* c = new WrongCat();
    std::cout << w->getType()<<": ";
    w->makeSound();
    std::cout << c->getType() <<": ";
    c->makeSound();
    delete w;
    delete c;

    return 0;
}