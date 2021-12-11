#ifndef LAMBDAEXPRESSION_H
#define LAMBDAEXPRESSION_H

#include "character_expression.h"

namespace ExprDraw {

class EXPRDRAW_EXPORT LambdaExpression : public CharacterExpression
{
public:
  LambdaExpression();

protected:
  // AbstractExpression interface
  void paint(QPainter* painter, int x, int y) const override;

  // CharacterExpression interface
  bool canChangeCharacter(const QChar& newCharacter) const override;
};

} // namespace ExprDraw

#endif // LAMBDAEXPRESSION_H
