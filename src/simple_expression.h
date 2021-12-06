#ifndef SIMPLEEXPRESSION_H
#define SIMPLEEXPRESSION_H

#include <QString>
#include <QChar>

#include "abstract_expression.h"

namespace ExprDraw {

class EXPRDRAW_EXPORT SimpleExpression : public AbstractExpression
{
public:
  explicit SimpleExpression(const QString& text);
  explicit SimpleExpression(const QChar& ch);

protected:
  // AbstractExpression interface
  void paint(QPainter *painter, int x, int y) const override;
  int calcWidth() const override;
  int calcHeight() const override;
  int calcCapDY() const override;

private:
  QString m_text;
};

} // namespace ExprDraw

#endif // SIMPLEEXPRESSION_H
