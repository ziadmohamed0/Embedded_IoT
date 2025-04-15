#include "sum.h"

sum::sum(int num1, int num2) : number1(num1), number2(num2) {

}

[[nodiscard]] int sum::summing(void) {
	return (number1+number2);
}
