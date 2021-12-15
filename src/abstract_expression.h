#ifndef ABSTRACTEXPRESSION_H
#define ABSTRACTEXPRESSION_H

#include <QFont>
#include <QColor>
#include <QPen>
#include <QBrush>

#include "global.h"

class QFont;
class QColor;
class QPainter;
class QPaintDevice;

namespace ExprDraw {

class  EXPRDRAW_EXPORT AbstractExpression
{
public:
  AbstractExpression();
  virtual ~AbstractExpression();

  inline QPaintDevice* paintDevice() const { return m_paint_device; }
  inline AbstractExpression* next() const { return m_next; }
  inline AbstractExpression* parent() const { return m_parent; }
  virtual QFont font() const { return m_font; }
  virtual QPen pen() const;
  virtual QBrush brush() const;
  QColor color() const;

  inline bool hasNext() const { return m_next; }
  inline bool hasParent() const { return m_parent; }

  int width() const;
  int height() const;
  int ascent() const;
  int descent() const;
  int superscriptX() const;
  int superscriptY() const;
  //NOTE add QPoint powerPos() const;
  int subscriptX() const;
  int subscriptY() const;
  //NOTE add QPoint indexPos() const;
  int capDXLeft() const;
  int capDXRight() const;
  int capDY() const;

  void draw(int x, int y, HorizontalAlignment hAligment, VerticalAlignment vAligment) const;
  void draw(QPainter* painter, int x, int y,
            HorizontalAlignment hAligment, VerticalAlignment vAligment) const;
  void setPaintDevice(QPaintDevice *paintDevice);
  void setNext(AbstractExpression *next);
  void setFont(const QFont& font);
  void setColor(const QColor& color);
  void addNext(AbstractExpression* next);

  virtual MultiplicationFlags multiplicationFlags() const;
  virtual bool isArgNeedBrackets() const;
  virtual bool isNeedBrackets() const;
  AbstractExpression* cutOff();

protected:
  virtual void fontChanged();
  virtual void colorChanged();
  virtual void paintDeviceChanged();

  //virtual void setPainterFont(QPainter* painter);  // NOTE overrided TExprPlank, TExprVar. maybe replase by 'virtual QFont font() const' ?

  virtual void paint(QPainter* painter, int x, int y) const = 0;
  virtual int calcWidth() const;
  virtual int calcHeight() const;
  virtual int calcAscent() const;
  virtual int calcDescent() const;
  virtual int calcSuperscriptX() const;
  virtual int calcSuperscriptY() const;
  virtual int calcSubscriptX() const;
  virtual int calcSubscriptY() const;
  virtual int calcCapDY() const;
  virtual void calcCapDX(int& dxLeft, int& dxRight) const;

  void convertCoords(int& X, int& Y,
                     HorizontalAlignment hAligment, VerticalAlignment vAligment) const;
  //inline void setFlag(CalculateFlag flag, bool on = true) { m_flags.setFlag(flag, on); }
  void setFlag(CalculateFlag flag, bool on = true);

  static void assignFont(AbstractExpression *expression, const QFont& font,
                         const LineWidth& line_width, const CapMultiplier& cap_multiplier);
  static void assignParent(AbstractExpression *expression, AbstractExpression *parent);
  static void assignPaintDevice(AbstractExpression *expression, QPaintDevice *paintDevice,
                                const LineWidth& line_width, const CapMultiplier& cap_multiplier);


  inline const LineWidth& lineWidth() const { return m_line_width; }
  inline const CapMultiplier& capMultiplier() const { return m_cap_multiplier; }
  inline const CalculateFlags& flags() { return m_flags; }

private:
  AbstractExpression* m_parent = nullptr;
  AbstractExpression* m_next   = nullptr;
  QPaintDevice*       m_paint_device = nullptr;
  QFont               m_font;
  QColor              m_color;

  LineWidth     m_line_width;
  CapMultiplier m_cap_multiplier;

  mutable int m_width = 0;
  mutable int m_height = 0;
  mutable int m_ascent = 0;
  mutable int m_descent = 0;
  mutable int m_superscript_x = 0;
  mutable int m_superscript_y = 0;
  mutable int m_subscript_x = 0;
  mutable int m_subscript_y = 0;
  mutable int m_cap_dy = 0;
  mutable int m_cap_dx_left = 0;
  mutable int m_cap_dx_right = 0;
  mutable CalculateFlags m_flags;

  void setLineWidth();
};

} // namespace ExprDraw

#endif // ABSTRACTEXPRESSION_H
