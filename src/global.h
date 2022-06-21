#ifndef GLOBAL_H
#define GLOBAL_H

#include <QtCore/qglobal.h>

#include <QtCore>

#if defined(MATH_EXPRESSIONS_EXPORTS)
#  define MATH_EXPRESSIONS_EXPORT Q_DECL_EXPORT
#elif defined (MATH_EXPRESSIONS_IMPORTS)
#  define MATH_EXPRESSIONS_EXPORT Q_DECL_IMPORT
#else
#  define MATH_EXPRESSIONS_EXPORT   /**/
#endif

/*! \file global.h
    \brief Сontains global flags and helper classes.
 */

namespace MathExpressions {

/// Used to recalculate cached parameters for width, height, index coordinates, degree coordinates, etc.
enum class CalculateFlag {
  NoFlag       = 0x00000000,  ///< Empty flag
  Width        = 0x00000001,  ///< Recalculate width of expression
  Height       = 0x00000002,  ///< Recalculate height of expression
  SuperscriptX = 0x00000004,  ///< Recalculate superscript X position
  SuperscriptY = 0x00000008,  ///< Recalculate superscript Y position
  SubscriptX   = 0x00000010,  ///< Recalculate subscript X position
  SubscriptY   = 0x00000020,  ///< Recalculate subscript Y position
  CapDX        = 0x00000040,  ///< Recalculate caption dX position
  CapDY        = 0x00000080,  ///< Recalculate caption dY position
  Ascent       = 0x00000100,  ///< Recalculate ascent
  Descent      = 0x00000200,  ///< Recalculate descent
  CellSize     = 0x00000400,  ///< Recalculate cell size (for MatrixExpression only)
  SymbolWidth  = 0x00000800,  ///< Recalculate width of symbol (for GroupExpression only)
  SymbolHeight = 0x00001000,  ///< Recalculate height of symbol (for GroupExpression only)
  All          = 0x7fffffff   ///< Recalculate all
};
Q_DECLARE_FLAGS(CalculateFlags, CalculateFlag)

template <typename T>
bool operator==(const QFlags<T> lhs, const CalculateFlag rhs)
{
    return (QFlags<T>::Int(lhs) == static_cast<int>(rhs));
}

/// These flags indicate whether the expression can be multiplied without the sign on the left, on the right, whether the expression is a number, etc.
enum class MultiplicationFlag {
  NoFlag        = 0x00000000,   ///< Empty flag
  Left          = 0x00000001,   ///< The expression can be multiplied without the sign on the left
  Right         = 0x00000002,   ///< The expression can be multiplied without the sign on the right
  Negative      = 0x00000004,   ///< The expression is negative
  Brackets      = 0x00000010,   ///< The expression has brackets
  RoundBrackets = 0x00000018,   ///< The expression has round brackets
  Number        = 0x00000020    ///< The expression is a number
};
Q_DECLARE_FLAGS(MultiplicationFlags, MultiplicationFlag)

template <typename T>
bool operator==(const QFlags<T> lhs, const MultiplicationFlag rhs)
{
    return (QFlags<T>::Int(lhs) == static_cast<int>(rhs));
}

constexpr inline QIncompatibleFlag operator|(MultiplicationFlag f1, int f2) noexcept \
{
  return QIncompatibleFlag(static_cast<int>(f1) | f2);
}

constexpr inline QFlags<MultiplicationFlag> operator|(MultiplicationFlag f1, MultiplicationFlag f2) noexcept
{
  return QFlags<MultiplicationFlag>(f1) | f2;
}

/*!
   \brief Helper class for storing the line widths in coordinates x ans y.
*/

class LineWidth
{
public:
/*!
  @brief Returns the line width for x coordinate.
  @returns {int}
  \sa y(), setX()
*/
  inline int x() const { return m_x; }

  /*!
  \return Returns the line width for y coordinate.
  \sa x(), setY()
*/
  inline int y() const { return m_y;}


/*!
  Sets the line width for x coordinate.
  \sa x(), setY()
*/
  inline void setX(int x) { m_x = x; }

  /*!
  Sets the line width for y coordinate.
  \sa y(), setX()
*/
  inline void setY(int y) { m_y = y; }

private:
  int m_x;
  int m_y;
};


/*!
   \brief Helper class for storing the multiplier for diacritic in coordinates x ans y.
*/

class CapMultiplier
{
public:
/*!
  \return Returns the multiplier for x coordinate.
  \sa y(), setX()
*/
  inline double x() const { return m_x; }

    /*!
  \return Returns the multiplier for y coordinate.
  \sa x(), setY()
*/
  inline double y() const { return m_y;}

/*!
  Sets the multiplier for x coordinate.
  \sa x(), setY()
*/
  inline void setX(double x) { m_x = x; }

    /*!
  Sets the multiplier for y coordinate.
  \sa y(), setX()
*/
  inline void setY(double y) { m_y = y; }

private:
  double m_x;
  double m_y;
};

} // namespace MathExpressions

#endif // GLOBAL_H
