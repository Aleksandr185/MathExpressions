#ifndef CIRCULATIONEXPRESSION_H
#define CIRCULATIONEXPRESSION_H

#include "group_expression.h"

namespace ExprDraw {

class EXPRDRAW_EXPORT CirculationExpression : public GroupExpression
{
public:
    CirculationExpression();

protected:
    // GroupExpression interface
    void drawSymbol(QPainter *painter, int x, int y) const;
};

} // namespace ExprDraw

#endif // CIRCULATIONEXPRESSION_H
