#include "Serializer.hpp"

int main() {
    Data original;
    original.data = "test";

    uintptr_t seria = Serializer::serialize(&original);
    Data* deseria = Serializer::deserialize(seria);

    std::cout << "Original pointer: " << &original << std::endl;
    std::cout << "Recovered pointer: " << deseria << std::endl;

    if (deseria == &original)
        std::cout << "Pointers match" << std::endl;
    else
        std::cout << "Pointers do not match" << std::endl;

    std::cout << "Recovered Data -> " << deseria->data << std::endl;

    return 0;
}
