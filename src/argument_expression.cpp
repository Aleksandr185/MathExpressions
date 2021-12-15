#include "argument_expression.h"

#include "common_function_expression.h"

namespace ExprDraw {

ArgumentExpression::ArgumentExpression()
 : m_forced_brackets(false)
{

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
    AbstractExpression* expr = son();
    while (expr) {
      if (expr->isNeedBrackets()) {
        return true;
      }

      expr = expr->next();
    }
  }

  return false;
}

} // namespace ExprDraw
