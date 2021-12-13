#ifndef FUNCTIONEXPRESSION_H
#define FUNCTIONEXPRESSION_H

#include "common_function_expression.h"

namespace ExprDraw {

class EXPRDRAW_EXPORT FunctionExpression : public CommonFunctionExpression
{
public:
  FunctionExpression(const QString& funcName, AbstractExpression* daughter);
};

} // namespace ExprDraw

#endif // FUNCTIONEXPRESSION_H
