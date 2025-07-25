#include <iostream>
#include <cstdlib>
#include <ctime>
#include "Base.hpp"

Base* generate() {
    std::srand(std::time(0));
    int r = std::rand() % 3;
    if (r == 0)
        return new A;
    else if (r == 1)
        return new B;
    return new C;
}

void identify(Base* p) {
    if (dynamic_cast<A*>(p))
        std::cout << "A" << std::endl;
    else if (dynamic_cast<B*>(p))
        std::cout << "B" << std::endl;
    else if (dynamic_cast<C*>(p))
        std::cout << "C" << std::endl;
}

void identify(Base& p) {
    try {
        (void)dynamic_cast<A&>(p);
        std::cout << "A" << std::endl;
    } catch (...) {
        try {
            (void)dynamic_cast<B&>(p);
            std::cout << "B" << std::endl;
        } catch (...) {
            std::cout << "C" << std::endl;
        }
    }
}
