#include "expression_evaluator.h"
#include <cmath>
#include <cstdlib> 
#include <algorithm>
#include <stdexcept>

ExpressionEvaluator::ExpressionEvaluator(std::string& expr_) {
  expression = expr_;
  token_type = TokenType::Nothing;
  error = ErrorCode::E_ok;
  std::string::iterator end_pos = std::remove(expression.begin(), expression.end(), ' ');
  expression.erase(end_pos, expression.end());
}

ErrorCode ExpressionEvaluator::eval(double& result) {
  result = parse();
  return error;
}

bool ExpressionEvaluator::isMinus(char c) {
  return c == '-';
}

bool ExpressionEvaluator::isOperator(char c) {
  return c == '+' || c == '*' || c == '/' || c == '*' || c == '^';
}

bool ExpressionEvaluator::isDigitDot(char c) {
  return (c >= '0' && c <= '9') || (c == '.');
}

bool ExpressionEvaluator::isAlpha(char c) {
  return c >= 'a' && c <= 'z';
}

bool ExpressionEvaluator::isSpace(char c) {
  return c == ' ';
}

int ExpressionEvaluator::getToken() {
  token_type = TokenType::Nothing;
  token.clear();

  if (i >= expression.length()) {
    token_type = TokenType::Nothing;
    token.clear();
    return 0;
  }

  if (isMinus(expression[i])) {
    token_type = TokenType::Delimiter;
    token = "-";
    i++;
  }
  else if (expression[i] == '(' || expression[i] == ')') {
    token_type = TokenType::Delimiter;
    token.push_back(expression[i]);
    i++;
  }
  else if (isOperator(expression[i])) {
    token_type = TokenType::Delimiter;
    while (i < expression.length() && isOperator(expression[i])) {
      token.push_back(expression[i]);
      i++;
    }
  }  
  else if (isDigitDot(expression[i])) {
    token_type = TokenType::Number;
    int dots = 0;
    while (i < expression.length() && isDigitDot(expression[i])) {
      if (expression[i] == '.') {
        dots++;
      }
      if (dots > 1) {
        error = ErrorCode::E_invalid;
        return -1;
      }
      token.push_back(expression[i]);
      i++;
    }
    return 0;
  }
  else if(isAlpha(expression[i])) {
    token_type = TokenType::Function;
    while (i < expression.length() && isAlpha(expression[i])) {
      token.push_back(expression[i]);
      i++;
    }
    if (expression[i] != '(') {
      error = ErrorCode::E_invalid;
      return -1;
    }
  }
  else {
    error = ErrorCode::E_invalid;
    return -1;
  }

  return 0;
}

OperatorId ExpressionEvaluator::get_operator(const std::string& op_name) {
  if (op_name == "+") {return OperatorId::Plus;}
  if (op_name == "-") {
      return OperatorId::Minus;
  }
  if (op_name == "*") {return OperatorId::Multiply;}
  if (op_name == "/") {return OperatorId::Divide;}
  if (op_name == "^") {return OperatorId::Pow;}
  return OperatorId::Unknown;
}

double ExpressionEvaluator::eval_operator(const OperatorId op_id, const double& lhs, const double& rhs) {
  double result = 0.0;

  switch(op_id) {
    case OperatorId::Plus: 
      result = lhs + rhs; 
      break;
    case OperatorId::Minus: 
      result = lhs - rhs; 
      break;
    case OperatorId::Multiply:
      result = lhs * rhs; 
      break;
    case OperatorId::Divide: 
      result = lhs / rhs; 
      if (std::isnan(result) || std::isinf(result)) {
        error = ErrorCode::E_invalid;
      }
      break;  
    case OperatorId::Pow: 
        if (lhs < 0.0) {
          error = ErrorCode::E_invalid;
        }
        result = pow(lhs,rhs); 
      break;
    default: 
      error = ErrorCode::E_invalid;
      break;
  }
  return result;
}

double ExpressionEvaluator::eval_function(const std::string& fun_name, const double& value) {
  if (fun_name == "abs") { 
    return fabs(value); 
  }
  if (fun_name == "sqrt") {
    double result = sqrt(value);
    if (std::isnan(result)) {
      error = ErrorCode::E_invalid;
    }
    return sqrt(value); 
  } 
  // If we reach this point, the function in the input is not valid
  error = ErrorCode::E_invalid;
  return 0.0;
}

double ExpressionEvaluator::parse() {
  double ans = 0.0;
  getToken();
  ans = parse_plus_minus();
  return ans;
}

double ExpressionEvaluator::parse_plus_minus() {
  double ans = parse_mul_div();
  OperatorId op_id = get_operator(token);
  while (op_id == OperatorId::Plus || op_id == OperatorId::Minus) {
    getToken();
    ans = eval_operator(op_id, ans, parse_mul_div());
    op_id = get_operator(token);
  }
  return ans;
}

double ExpressionEvaluator::parse_mul_div() {
  double ans = parse_unary_minus();
  OperatorId op_id = get_operator(token);
  while (op_id == OperatorId::Multiply || op_id == OperatorId::Divide) {
    getToken();
    ans = eval_operator(op_id, ans, parse_unary_minus());
    op_id = get_operator(token);
  }
  return ans;
}

double ExpressionEvaluator::parse_unary_minus() {
  double ans = 0.0; 
  OperatorId op_id = get_operator(token);
  if (op_id == OperatorId::Minus) {
    getToken();
    ans = parse_unary_minus();
    ans = -ans;
  }
  else {
    ans = parse_power();
  }
  return ans;
}

double ExpressionEvaluator::parse_power() {
  double ans = parse_function();
  OperatorId op_id = get_operator(token);
  while (op_id == OperatorId::Pow) {
    getToken();
    double res = parse_unary_minus();
    ans = eval_operator(op_id, ans, res);
    op_id = get_operator(token);
  }
  return ans;
}

double ExpressionEvaluator::parse_function() {
  std::string fun_name;
  double ans = 0.0;
  if (token_type == TokenType::Function) {
    fun_name = token;
    getToken();
    ans = eval_function(fun_name, parse_delimiter());
  }
  else {
    ans = parse_delimiter();
  }
  return ans;
}

double ExpressionEvaluator::parse_delimiter()
{
  if (token_type == TokenType::Delimiter) {
    if (token[0] == '(') {
      getToken();
      double ans = parse_plus_minus();
      if (token_type != TokenType::Delimiter || token[0] != ')') {
        error = ErrorCode::E_invalid;

        return 0.0;
      }
      getToken();
      return ans;
    }
  }

  return parse_number();
}

double ExpressionEvaluator::parse_number()
{
  double result = 0.0;
  switch(token_type)
  {
    case TokenType::Number:
      result = strtod(token.c_str(), nullptr);
      getToken();
      break;
  }
  return result;
}