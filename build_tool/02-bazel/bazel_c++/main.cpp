#include "Summing/sum.h"

int main(void) {
	int num1{0}, num2{0};
	std::cout << "Enter number 1: ";
	std::cin >> num1;
	std::cout << "Enter number 2: ";
	std::cin >> num2;
	sum obj(num1, num2);
	
	int res = obj.summing();
	std::cout << "Result : " << res << std::endl;
		
	return 0;
}
