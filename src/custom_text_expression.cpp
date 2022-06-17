#include "custom_text_expression.h"

namespace MathExpressions {

CustomTextExpression::CustomTextExpression(const QString& text, const QFont& custom_font)
  :SimpleExpression(text), m_custom_font(custom_font)
{

}

CustomTextExpression::CustomTextExpression(const QChar& ch, const QFont& custom_font)
  : SimpleExpression(ch), m_custom_font(custom_font)
{

}

// AbstractExpression interface

QFont CustomTextExpression::font() const
{
  return m_custom_font;
}

} // namespace MathExpressions
