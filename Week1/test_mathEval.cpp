#include "expression_evaluator.cpp"
#include <gtest/gtest.h>

//ASSERT_EQ(<Function_To_Call>, <Expected_Result>);
//ASSERT_TRUE(<Function_To_Call>);

// Better to use EXPECT_EQ, EXPECT_TRUE
// rather than ASSERT

TEST(Test_Parenthesis, Test_AdditionBrackets) {
  std::string expression("(  3  +  2)  *  4");
  ExpressionEvaluator e(expression);
  EXPECT_EQ(e.eval(), 20.00);
}

TEST(Test_PlusMinusDivide, Test_FirstMul) {
  std::string expression("3+2*4");
  ExpressionEvaluator e(expression);
  EXPECT_EQ(e.eval(), 11.00);
}

TEST(Test_MathEvalAbs, Test_NestedSqrtAbs_1) {
  std::string expression("sqrt(abs(4)) + abs(abs(5))");
  ExpressionEvaluator e(expression); 
  EXPECT_EQ(e.eval(), 7.00);
}

TEST(Test_MathEvalAbs, Test_NestedSqrt_1) {
  std::string expression("sqrt(8+4*7)");
  ExpressionEvaluator e(expression);
  EXPECT_EQ(e.eval(), 6.00);
}


TEST(Test_SquareRoot, Test_NonRealNumber) {
  std::string expression("sqrt(-1)");
  ExpressionEvaluator e(expression);
  EXPECT_TRUE(std::isnan(e.eval()));
}

TEST(Test_Division, Test_DivisionByZero) {
  std::string expression("1/0");
  ExpressionEvaluator e(expression);
  EXPECT_TRUE(std::isinf(e.eval()));
}
 
TEST(Test_Numbers, TestDoubleDots) {
  std::string expression("1..23+4");
  ExpressionEvaluator e(expression);
  std::cout << e.eval() << std::endl;
}

TEST(Test_Pow, Test_MassiveNegativePow) {
  std::string expression("1.6*10^(-35)");
  ExpressionEvaluator e(expression);
  EXPECT_EQ(round(e.eval()), 0.00);
}

TEST(Test_Google, Test_Google_Example1) {
  std::string expression("abs(2.2-(3*2))");
  ExpressionEvaluator e(expression);
  EXPECT_EQ(e.eval(), 3.80);
}

TEST(Test_Google, Test_Google_Example2) {
  std::string expression("-3^4");
  ExpressionEvaluator e(expression);
  EXPECT_EQ(e.eval(), -81.00);
}

TEST(Test_Google, Test_Google_Example3) {
  std::string expression("1+2*3^4");
  ExpressionEvaluator e(expression);
  EXPECT_EQ(e.eval(), 163.00);
}

TEST(Test_Google, Test_Google_Example4) {
  std::string expression("(-3)^4");
  ExpressionEvaluator e(expression);
  std::cout << e.eval() << std::endl;  // Expected: "Invalid mathematical expression."
}

TEST(Test_Google, Test_Google_Example5) {
  std::string expression("12345677*-(-(-(((98765431.)))))");
  ExpressionEvaluator e(expression);
  EXPECT_EQ(e.eval(), -1219326109891787.00);
} 

TEST(Test_Pow, Test_MinusOnePow) {
  std::string expression("1.6*10^(-1)");
  ExpressionEvaluator e(expression);
  //EXPECT_EQ(trunc(e.eval()), 0.16);
  std::cout << "Test: " << e.eval() << std::endl;
}

TEST(Test_Pow, TestRecurringPow) {
  std::string expression("3^2^2^2");
  ExpressionEvaluator e(expression);
  EXPECT_EQ(e.eval(), 43046721);
}

TEST(Test_Double, TestDotPrefix) {
  std::string expression(".5+5.");
  ExpressionEvaluator e(expression);
  EXPECT_EQ(e.eval(), 5.50);
} 

TEST(Test_Zeros, TestLeadingZeros) {
  std::string expression("05+07");
  ExpressionEvaluator e(expression);
  EXPECT_EQ(e.eval(), 12);
}

TEST(Test_UnaryMinus, TestMultipleMinus) {
  std::string expression("---42-1");         
  ExpressionEvaluator e(expression);
  EXPECT_EQ(e.eval(), -43);
}

TEST(Test_UnaryMinus, TestMinusBrackets) {
  std::string expression("-(-(-(42)))-1");
  ExpressionEvaluator e(expression);
  EXPECT_EQ(e.eval(), -43);
}

TEST(Test_UnaryMinus, TestMinusAfter) {
  std::string expression("-1---42");
  ExpressionEvaluator e(expression);
  EXPECT_EQ(e.eval(), -43);
}

TEST(Test_UnaryMinus, TestMinusEnd) {
  std::string expression("2 + 3 * ---4");
  ExpressionEvaluator e(expression);
  EXPECT_EQ(e.eval(), -10);
}

TEST(Test_UnaryMinus, TestMinusMulAdd) {
  std::string expression("---42-1+6*7");
  ExpressionEvaluator e(expression);
  EXPECT_EQ(e.eval(), -1);
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}