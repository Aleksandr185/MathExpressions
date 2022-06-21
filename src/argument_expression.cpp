#include "argument_expression.h"

#include "common_function_expression.h"

namespace MathExpressions {

ArgumentExpression::ArgumentExpression(ExpressionPtr son)
 : m_forced_brackets(false)
{
  setSon(son);
}

void ArgumentExpression::setForsedBrackets(bool value)
{
  if (m_forced_brackets != value) {
    m_forced_brackets = true;
    setFlag(CalculateFlag::All);
  }
}

// BracketedExpression interface

bool ArgumentExpression::isBracketed() const
{
  if (parent() && dynamic_cast<CommonFunctionExpression*>(parent())) {
    if (m_forced_brackets || parent()->isArgNeedBrackets()) {
      return true;
    }
  }
  else {
    ExpressionPtr expr = son();
    while (expr) {
      if (expr->isNeedBrackets()) {
        return true;
      }

      expr = expr->next();
    }
  }

  return false;
}

} // namespace MathExpressions
