#include "function_expression.h"

#include "variable_expression.h"
#include "function_name_expression.h"

namespace MathExpressions {

FunctionExpression::FunctionExpression(const QString &funcName, ExpressionPtr daughter)
{
  if (funcName.length() == 1) {
    setSon(new VariableExpression(funcName));
  }
  else {
    setSon(new FunctionNameExpression(funcName));
  }

  setDaughter(daughter);
}

} // namespace MathExpressions
