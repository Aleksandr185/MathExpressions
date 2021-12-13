#include "function_expression.h"

#include "variable_expression.h"
#include "function_name_expression.h"

namespace ExprDraw {

FunctionExpression::FunctionExpression(const QString &funcName, AbstractExpression *daughter)
{
  if (funcName.length() == 1) {
    setSon(new VariableExpression(funcName));
  }
  else {
    setSon(new FunctionNameExpression(funcName));
  }

  setDaughter(daughter);
}

} // namespace ExprDraw
