#ifndef SIMPLEEXPRESSION_H
#define SIMPLEEXPRESSION_H

#include <QString>
#include <QChar>

#include "abstract_expression.h"

namespace MathExpressions {

class MATH_EXPRESSIONS_EXPORT SimpleExpression : public AbstractExpression
{
public:
  explicit SimpleExpression(const QString& text);
  explicit SimpleExpression(const QChar& ch);

  inline const QString& text() const { return m_text; }

protected:
  // AbstractExpression interface
  void paint(QPainter *painter, int x, int y) const override;
  int calcWidth() const override;
  int calcHeight() const override;
  int calcCapDY() const override;

private:
  QString m_text;
};

} // namespace MathExpressions

#endif // SIMPLEEXPRESSION_H
