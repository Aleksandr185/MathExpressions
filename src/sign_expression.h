#ifndef SIGNEXPRESSION_H
#define SIGNEXPRESSION_H

#include "character_expression.h"

namespace MathExpressions {

class MATH_EXPRESSIONS_EXPORT SignExpression : public CharacterExpression
{
public:
  enum SignType {
    Plus,                // +
    Minus,               // -
    PlusMinus,           // ±
    MinusPlus,           // ∓
    Multiply,            // ·
    CrossMultiply,       // ×
    Divide,              // ÷
    Slash,               // /
    Tilda,               // ~
    Arrow,               // →
    Ellipsis,            // …
    Infinity,            // ∞
    PartialDifferential, // ∂
    Parallel,            // ‖
    Perpendicular,       // ⊥
    Angle,               // ∠
    Equals,              // =
    IdenticalTo,         // ≡
    NotEqualTo,          // ≠
    ApproxEqual,         // ≈
    AlmostEqual,         // ≃
    Less,                // <
    LessOrEqual,         // ≤
    ApproxLess,          // ≲
    MuchLess,            // ≪
    Greater,             // >
    GreaterOrEqual,      // ≥
    ApproxGreater,       // ≳
    MuchGreater          // ≫
  };

  explicit SignExpression(SignType sign);

  // AbstractExpression interface
  MultiplicationFlags multiplicationFlags() const override;
  bool isNeedBrackets() const override;

protected:
  void paint(QPainter *painter, int x, int y) const override;
  int calcWidth() const override;
  int calcCapDY() const override;

private:
  static ushort codeFromSign(SignType sign);
};

} // namespace MathExpressions

#endif // SIGNEXPRESSION_H
