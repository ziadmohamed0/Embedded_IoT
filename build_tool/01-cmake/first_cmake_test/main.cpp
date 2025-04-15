#include "calc.hpp"

int main (void) {
    int return_value{0};
    return_value = calc(num1, num2);
    std::cout << "Result: " << return_value << std::endl;
    return 0; 
}