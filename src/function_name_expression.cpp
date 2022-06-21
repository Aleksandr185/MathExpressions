#include "function_name_expression.h"

namespace MathExpressions {

FunctionNameExpression::FunctionNameExpression(const QString& text)
  :SimpleExpression(text)
{

}

// AbstractExpression interface

bool FunctionNameExpression::isArgNeedBrackets() const
{
  return false;
}

} // namespace MathExpressions
