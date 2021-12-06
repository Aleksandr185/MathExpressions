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
  void setPaintDevice(QPaintDevice *paintDevice);
  void setNext(AbstractExpression *next);
  void setFont(const QFont& font);
  void setColor(const QColor& color);
  void addNext(AbstractExpression* next);

  virtual MultiplicationFlags multiplicationFlags();
  AbstractExpression* cutOff();

protected:
  virtual void fontChanged();
  virtual void colorChanged();
  virtual void paintDeviceChanged();

  //virtual void setPainterFont(QPainter* painter);  // NOTE overrided TExprPlank, TExprVar. maybe replase by 'virtual QFont font() const' ?

  virtual void paint(QPainter* painter, int x, int y) const = 0;
  virtual bool isNeedBrackets() const;
  virtual bool isArgNeedBrackets() const;
  virtual int calcWidth() const;
  virtual int calcHeight() const;
  virtual int calcAscent() const;
  virtual int calcDescent() const;
  virtual int calcSuperscriptX() const;
  virtual int calcSuperscriptY() const;
  virtual int calcSubscriptX() const;
  virtual int calcSubscriptY() const;
  virtual int calcCapDY() const;
  virtual void calcCapDX(int& DLeft, int& DRight) const;

  void convertCoords(int& X, int& Y,
                     HorizontalAlignment hAligment, VerticalAlignment vAligment) const;
  void setFont(const QFont& font,  int line_width_x, int line_width_y,
               double r_line_width_x, double r_line_width_y);
  inline void setFlag(CalculateFlag flag, bool on = true) { m_flags.setFlag(flag, on); }

  static void setParent(AbstractExpression *expression, AbstractExpression *parent);
  static void assignPaintDevice(AbstractExpression *expression, QPaintDevice *paintDevice,
                                int line_width_x, int line_width_y,
                                double r_line_width_x, double r_line_width_y);

  inline int lineWidthX() const { return m_line_width_x; }
  inline int lineWidthY() const { return m_line_width_y; }
  inline double rLineWidthX() const { return m_r_line_width_x; }
  inline double rLineWidthY() const { return m_r_line_width_y; }
  inline const CalculateFlags& flags() { return m_flags; }

private:
  AbstractExpression* m_parent = nullptr;
  AbstractExpression* m_next   = nullptr;
  QPaintDevice*       m_paint_device = nullptr;
  QFont               m_font;
  QColor              m_color;
  
  int m_line_width_x = 0;
  int m_line_width_y = 0;
  double m_r_line_width_x = 0;  // NOTE: what is mean 'r' ? maybe 'cap_factor_*' ?
  double m_r_line_width_y = 0;

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
