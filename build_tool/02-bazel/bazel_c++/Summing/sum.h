#ifndef _SUMMING_SUM_H_
#define _SUMMING_SUM_H_

#include <iostream>

class sum{
	private:
		int number1;
		int number2;
	public:
		sum(int num1, int num2);
		[[nodiscard]] int summing(void);
};

#endif
