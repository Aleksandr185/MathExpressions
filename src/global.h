#ifndef GLOBAL_H
#define GLOBAL_H

#include <QtCore/qglobal.h>

#include <QtCore>

#if defined(EXPRDRAW_EXPORTS)
#  define EXPRDRAW_EXPORT Q_DECL_EXPORT
#elif defined (EXPRDRAW_IMPORTS)
#  define EXPRDRAW_EXPORT Q_DECL_IMPORT
#else
#  define EXPRDRAW_EXPORT   /**/
#endif

namespace ExprDraw {

//enum class Origin {
//  Top,        // Ascent
//  Bottom      // Descent
//};

// Вертикальное выравнивание выражения.
// Указывает где находится координата X по отношению к выражению.
// Пример:
// координата          X
// ehLeft               "выражение"
// ehCentr        "выражение"
// ehRight  "выражение"
enum class HorizontalAlignment {  // NOTE maybe use Qt::AlignmentFlag ?
  Left,
  Center,
  Right
};

// Горизонтальное выравнивание выражения.
// Указывает где находится координата Y по отношению к выражению.
// Пример:
//               evBottom
// координата Y  evCenter
//               evTop
enum class VerticalAlignment {
  Top,
  Center,
  Bottom
};

enum class CalculateFlag {
  NoFlag       = 0x00000000,
  Width        = 0x00000001,
  Height       = 0x00000002,
  SuperscriptX = 0x00000004,
  SuperscriptY = 0x00000008,
  SubscriptX   = 0x00000010,
  SubscriptY   = 0x00000020,
  CapDX        = 0x00000040,
  CapDY        = 0x00000080,
  Ascent       = 0x00000100,
  Descent      = 0x00000200,
  CellSize     = 0x00000400,
  SymbolWidth  = 0x00000800,
  SymbolHeight = 0x00001000,
  All          = 0x7fffffff
};
Q_DECLARE_FLAGS(CalculateFlags, CalculateFlag)

template <typename T>
bool operator==(const QFlags<T> lhs, const CalculateFlag rhs)
{
    return (QFlags<T>::Int(lhs) == static_cast<int>(rhs));
}

// Данные флаги показывают, может ли выражение быть умножено без
// знака слева, справа, является ли оно числом и т.д.
enum class MultiplicationFlag {
  Left          = 0x00000001,   // 0000 0001
  Right         = 0x00000002,   // 0000 0010
  Negative      = 0x00000004,   // 0000 0100
  Brackets      = 0x00000010,   // 0001 0000
  RoundBrackets = 0x00000018,   // 0001 1000
  Number        = 0x00000020    // 0010 0000
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

class LineWidth
{
public:
  inline int x() const { return m_x; }
  inline int y() const { return m_y;}

  inline void setX(int x) { m_x = x; }
  inline void setY(int y) { m_y = y; }

private:
  int m_x;
  int m_y;
};

class CapMultiplier
{
public:
  inline double x() const { return m_x; }
  inline double y() const { return m_y;}

  inline void setX(double x) { m_x = x; }
  inline void setY(double y) { m_y = y; }

private:
  double m_x;
  double m_y;
};

} // namespace ExprDraw

#endif // GLOBAL_H
