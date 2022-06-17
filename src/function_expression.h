#ifndef FUNCTIONEXPRESSION_H
#define FUNCTIONEXPRESSION_H

#include "common_function_expression.h"

namespace MathExpressions {

class MATH_EXPRESSIONS_EXPORT FunctionExpression : public CommonFunctionExpression
{
public:
  FunctionExpression(const QString& funcName, ExpressionPtr daughter);
};

} // namespace MathExpressions

#endif // FUNCTIONEXPRESSION_H
