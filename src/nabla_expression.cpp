#include "nabla_expression.h"

#include <QFontMetrics>
#include <QDebug>

namespace MathExpressions {

const ushort NABLA_CODE = 0x2207; // ∇

NablaExpression::NablaExpression()
  :CharacterExpression( QChar(NABLA_CODE), true)
{

}

} // namespace MathExpressions

