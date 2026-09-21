#include "Animal.hpp"
#include "Cat.hpp"
#include "Dog.hpp"
#include "Brain.hpp"

int main()
{
    std::cout<<"---Array test---"<<std::endl;
    Animal* animals[4];
    for (int i = 0; i < 2; i++)
        animals[i] = new Dog();
    for (int i = 2; i < 4; i++)
        animals[i] = new Cat();
    for (int i = 0; i < 4; i++)
        animals[i]->makeSound();
    for (int i = 0; i < 4; i++)
        delete animals[i];

    std::cout<<"---Deep copy test: Dog---"<<std::endl;

    Dog dog1;
    dog1.getBrain().setIdea(0,"I am the first dog");
    std::cout<<dog1.getType()<<": "<<dog1.getBrain().getIdea(0)<<std::endl;
    Dog dog2(dog1);
    dog2.getBrain().setIdea(0,"I am the second dog");
    std::cout<<dog1.getType()<<": "<<dog1.getBrain().getIdea(0)<<std::endl;
    std::cout<<dog2.getType()<<": "<<dog2.getBrain().getIdea(0)<<std::endl;

    std::cout<<"---Deep copy test: Cat---"<<std::endl;

    Cat cat1;
    cat1.getBrain().setIdea(0,"I am the first cat");
    std::cout<<cat1.getType()<<": "<<cat1.getBrain().getIdea(0)<<std::endl;
    Cat cat2(cat1);
    cat2.getBrain().setIdea(0,"I am the second cat");
    std::cout<<cat1.getType()<<": "<<cat1.getBrain().getIdea(0)<<std::endl;
    std::cout<<cat2.getType()<<": "<<cat2.getBrain().getIdea(0)<<std::endl;

    std::cout<<"---Assignment operator test---"<<std::endl;

    Dog dog3;
    dog3 = dog1;
    dog3.getBrain().setIdea(0,"I am the third dog");
    std::cout<<dog1.getType()<<": "<<dog1.getBrain().getIdea(0)<<std::endl;
    std::cout<<dog3.getType()<<": "<<dog3.getBrain().getIdea(0)<<std::endl;   

    std::cout<<"---Destructor test---"<<std::endl;

    const Animal* j = new Dog();
    const Animal* i = new Cat();
    delete j;
    delete i;
    return 0;
}