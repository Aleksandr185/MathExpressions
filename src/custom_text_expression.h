#ifndef CUSTOMTEXTEXPRESSION_H
#define CUSTOMTEXTEXPRESSION_H

#include "simple_expression.h"

namespace ExprDraw {

class EXPRDRAW_EXPORT CustomTextExpression : public SimpleExpression
{
public:
  CustomTextExpression(const QString& text, const QFont& custom_font);
  CustomTextExpression(const QChar& ch, const QFont& custom_font);

  // AbstractExpression interface
  QFont font() const override;

private:
  QFont m_custom_font;
};

} // namespace ExprDraw

#endif // CUSTOMTEXTEXPRESSION_H
