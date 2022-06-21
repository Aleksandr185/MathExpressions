#ifndef EXPRESSIONBUILDER_H
#define EXPRESSIONBUILDER_H

#include <memory>
#include "cap_expression.h"

namespace MathExpressions {

typedef std::unique_ptr<AbstractExpression> ExpressionUniquePtr;

class MATH_EXPRESSIONS_EXPORT Error
{
public:
  Error()
    : m_number(0), m_position(0)
  {}
  Error(int number, const QString& text, int position)
    : m_number(number), m_text(text), m_position(position)
  {}

  inline bool isValid() const { return m_number != 0; }
  inline int number() const { return m_number; }
  inline const QString& text() const { return m_text; }
  inline int position() const { return m_position; }

private:
  int m_number;
  QString m_text;
  int m_position;
};

class MATH_EXPRESSIONS_EXPORT ExpressionBuilder
{
public:
  enum ErrorCode {
    NoError,
    ConvertToNumber,
    ExpectedNumber,
    ExpectedComma,
    ExpectedSquareBracket,
    ExpectedFigureBracket,
    ExpectedRoundBracket,
    ExpectedModuleBracket,
    ExpectedOpenBracket,
    ExpectedCloseBracket,
    InvalidCharacter,
    UnfinishedString
  };

  ExpressionBuilder();

  ExpressionUniquePtr exec(const QString&);

  inline const Error& lastError() const { return m_last_error; }

  inline bool variableAutoIndex() const { return m_flag_variable_auto_index; }
  inline bool functionAutoIndex() const { return m_flag_function_auto_index; }
  inline bool postSymbols() const { return m_flag_post_symbols; }
  inline bool wrapEmptyExpressionToFrame() const { return m_flag_wrap_empty_to_frame; }

  inline void variableAutoIndex(bool on) { m_flag_variable_auto_index = on; }
  inline void functionAutoIndex(bool on) { m_flag_function_auto_index = on; }
  inline void postSymbols(bool on) { m_flag_post_symbols = on; }
  inline void wrapEmptyExpressionToFrame(bool on) { m_flag_wrap_empty_to_frame = on; }

private:

  enum class Wrapper {
    Chain,
    Expression,
    Bracketed,
    Argument,
    Power,
    Abs,
    Squared,
    Figured
  };

  enum class WrapperFlag {
    NoFlag = 0,
    Powered = 1,
    Trans = 3
  };
  Q_DECLARE_FLAGS(WrapperFlags, WrapperFlag)

  enum class GroupType {
    Summa,
    Product,
    Circulation,
    Integral,
    IntegralM
  };


  int     m_current_pos;
  QString m_text;
  Error   m_last_error;

  bool m_flag_variable_auto_index;
  bool m_flag_function_auto_index;
  bool m_flag_post_symbols;
  bool m_flag_wrap_empty_to_frame;

  inline QChar currentChar() const { return m_text.at(m_current_pos); }
  inline bool isEnd() const { return m_current_pos >= m_text.size(); }
  inline bool stopBuild() const { return lastError().isValid() || isEnd(); }

  QString preprocess(const QString& text) const;
  inline void clearLastError() { m_last_error = Error();}
  void skipSpaces();
  bool skipComma();
  bool skipRequiredComma();

  ExpressionPtr wrap(ExpressionPtr base, Wrapper wrapper, WrapperFlags flags) const;
  ExpressionPtr stringExpression(Wrapper wrapper, bool allowComma = false);
  ExpressionPtr booleanExpression(WrapperFlags& flags);
  ExpressionPtr expression(WrapperFlags& flags);
  ExpressionPtr trans(WrapperFlags& flags);
  ExpressionPtr factor(WrapperFlags& flags);
  ExpressionPtr func(WrapperFlags& flags);
  ExpressionPtr funcName(const QString& name, WrapperFlags& flags, bool needBrackets);
  ExpressionPtr token(const QString& name);

  ExpressionPtr makeCap(ExpressionPtr base, CapExpression::CapStyle style, int count) const;
  ExpressionPtr makePower(ExpressionPtr base, ExpressionPtr exponent) const;
  ExpressionPtr makeIndex(ExpressionPtr base, ExpressionPtr index) const;
  ExpressionPtr makeEmptyExpression() const;

  int getOriginalPosition() const;
  void addMult(ExpressionPtr* _existing, ExpressionPtr* _multiplier) const;
  void decorate(ExpressionPtr* _base) const;
  int getInt(const QString& str, int from,  int& next_pos);
  double getDouble(const QString& str, int from, int& next_pos);
  int cutFuncIndex(QString& name, bool& ok) const;
  ushort greekLetterCode(const QString& name) const;

  ExpressionPtr functionLOG(const QString& name, bool needBrackets);
  ExpressionPtr functionPOW(WrapperFlags& flags);
  ExpressionPtr functionROOT();
  ExpressionPtr functionIND();
  ExpressionPtr functionLIM(WrapperFlags& flags, bool needBrackets);
  ExpressionPtr functionFUNC(bool needBrackets);
  ExpressionPtr functionFRAME();
  ExpressionPtr functionSPACE();
  ExpressionPtr functionDIFF();
  ExpressionPtr functionPDIFF();
  ExpressionPtr functionDIFFN();
  ExpressionPtr functionPDIFFN();
  ExpressionPtr functionDIFFR();
  ExpressionPtr functionPDIFFR();
  ExpressionPtr functionDIFFRF();
  ExpressionPtr functionPDIFFRF();
  ExpressionPtr functionSTRING();
  ExpressionPtr functionSTROKES(WrapperFlags& flags);
  ExpressionPtr functionFACT(WrapperFlags& flags);
  ExpressionPtr functionAT();
  ExpressionPtr functionCAPTION(CapExpression::CapStyle style);
  ExpressionPtr functionPOINTS();
  ExpressionPtr functionSTAND(Qt::Alignment horizontal_alignment);
  ExpressionPtr functionMATRIX();
  ExpressionPtr functionGroupOp(GroupType type, bool needBrackets);
  ExpressionPtr functionCASE();
  ExpressionPtr functionCOMMA();
  ExpressionPtr functionBRACKETS();
  ExpressionPtr functionSYSTEM();
  ExpressionPtr functionNUM();
  ExpressionPtr functionSYMBOL();
  ExpressionPtr functionANGLE();
  //ExpressionPtr functionCOLOR();

};

} // namespace MathExpressions

#endif // EXPRESSIONBUILDER_H
