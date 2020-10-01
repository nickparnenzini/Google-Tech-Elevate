#include <iostream>
#include <sstream>
#include <string>
#include <math.h>
#include <iomanip>

#include "expression_evaluator.h"

int main() {
  std::string expression;
  std::getline(std::cin, expression);
  std::cout.precision(2);
  std::cout << std::fixed;
  
  ExpressionEvaluator e(expression);
  double result = 0.0;
  ErrorCode error = e.eval(result);
  if (error != ErrorCode::E_ok) {
    std::cout << "Invalid mathematical expression." << std::endl;
  }
  else {
    std::cout << std::fixed;
    std::cout << result << std::endl;
  }
  
}