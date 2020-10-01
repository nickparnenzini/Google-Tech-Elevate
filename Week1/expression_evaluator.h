#include <iostream>
#include <vector>
#include <queue>

enum class TokenType {
  Nothing,
  Delimiter,
  Number,
  Function
};

enum class OperatorId {
  Plus,
  Minus,
  Multiply,
  Divide,
  UnaryMinus,
  Pow,
  Unknown  
};

enum class ErrorCode {
  E_ok,
  E_invalid
};

class ExpressionEvaluator {
 public:
  ExpressionEvaluator() = default;
  ExpressionEvaluator(std::string& expr_);
  ErrorCode eval(double&);

 private:
  bool isMinus(char c);
  bool isOperator(char c);
  bool isDigitDot(char c);
  bool isAlpha(char c);
  bool isSpace(char c);

  int getToken();

  OperatorId get_operator(const std::string& op_name);
  double eval_operator(const OperatorId op_id, const double& lhs, const double& rhs);
  double eval_function(const std::string& fun_name, const double& value);
  double parse();
  double parse_plus_minus();
  double parse_mul_div();
  double parse_power();
  double parse_unary_minus();
  double parse_function();
  double parse_delimiter();
  double parse_number();

  std::string expression;
  std::string token;
  TokenType token_type; 
  int i{0};
  ErrorCode error;
};