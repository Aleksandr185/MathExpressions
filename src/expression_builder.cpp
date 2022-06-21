#include "expression_builder.h"

#include <QRegExp>

#include "asterix_expression.h"
#include "argument_expression.h"
#include "at_value_expression.h"
#include "base_expression.h"
#include "case_expression.h"
#include "chain_expression.h"
#include "character_expression.h"
#include "comma_expression.h"
#include "common_function_expression.h"
#include "empty_expression.h"
#include "extended_number_expresssion.h"
#include "fraction_expression.h"
#include "frame_expression.h"
#include "function_expression.h"
#include "function_name_expression.h"
#include "index_expression.h"
#include "matrix_expression.h"
#include "null_expression.h"
#include "number_expresssion.h"
#include "lambda_expression.h"
#include "limit_expression.h"
#include "space_expression.h"
#include "sign_expression.h"
#include "simple_expression.h"
#include "strokes_expression.h"
#include "stand_expression.h"
#include "round_bracketed_expression.h"
#include "root_expression.h"
#include "planck_expression.h"
#include "variable_expression.h"

#include "summa_expression.h"
#include "product_expression.h"
#include "circulation_expression.h"
#include "integral_expression.h"

namespace MESSAGE {
  const QString ERROR_CONVERT_STR_TO_INT      = QStringLiteral("Error convert string to integer!");
  const QString ERROR_CONVERT_STR_TO_DOUBLE   = QStringLiteral("Error convert string to double!");
  const QString ERROR_EXPECTED_NUMBER         = QStringLiteral("Expected number in position %1!");
  const QString ERROR_EXPECTED_COMMA          = QStringLiteral("Expected comma in position %1!");
  const QString ERROR_EXPECTED_SQUARE_BRACKET = QStringLiteral("Expected square bracket in position %1!");
  const QString ERROR_EXPECTED_FIGURE_BRACKET = QStringLiteral("Expected figure bracket in position %1!");
  const QString ERROR_EXPECTED_ROUND_BRACKET  = QStringLiteral("Expected round bracket in position %1!");
  const QString ERROR_EXPECTED_MODULE_BRACKET = QStringLiteral("Expected module bracket in position %1!");

  const QString ERROR_EXPECTED_OPEN_BRACKET   = QStringLiteral("Expected open bracket in position %1!");
  const QString ERROR_EXPECTED_CLOSE_BRACKET  = QStringLiteral("Expected close bracket in position %1!");

  const QString ERROR_INVALID_CHARACTER       = QStringLiteral("Invalid character in position %1!");
  const QString ERROR_UNFINISHED_STRING       = QStringLiteral("Unfinished string!");
}

namespace MathExpressions {

enum PseudoSign : ushort {       // befor after note
  NotEqual           = 1,        // <>    ≠
  GreaterOrEqual     = 2,        // >=    ≥
  LessOrEqual        = 3,        // <=    ≤
  MuchGreater        = 4,        // >>    ≫
  MuchLess           = 5,        // <<    ≪
  Arrow              = 6,        // ->    →
  ApproxEqual        = 7,        // ~~    ≈
  PlusMinus          = 8,        // +-    ±
  MinusPlus          = 9,        // -+    ∓
  Equivalent         = 10,       // ==    ≡
  Divide             = 11,       // /+    ÷
  MultiplyCross      = 12,       // *+    x
  Ellipsis           = 13,       // ...   …
  ApproxGreater      = 14,       // >~    ≳
  ApproxLess         = 15,       // <~    ≲
  DivideSlash        = 16,       // //    /
  NecessarilyBracket = 17,       // !(          required brackets, must be closed with a sign ')'
  MultiplyDot        = 18,       // *.    ·
  Multiply           = 19,       // **          multiplication without permutation of factors
  AlmostEqual        = 20,       // =~    ≃
  Ampersand          = 21,       // ' & ' &     to correctly determine the position of the character with an error
  Plambda            = 22
};


ExpressionBuilder::ExpressionBuilder()
  : m_current_pos(0), m_flag_variable_auto_index(true), m_flag_function_auto_index(true),
    m_flag_post_symbols(true), m_flag_wrap_empty_to_frame(false)
{

}

/*!
    \fn bool variableAutoIndex() const
    If true (the default) expression a2 will become a₂
*/

/*!
    \fn bool functionAutoIndex() const
    If true (the default) expression a2(x+1) will become a₂(x+1)
*/

/*!
    \fn bool postSymbols() const

*/

ExpressionUniquePtr ExpressionBuilder::exec(const QString& text)
{
  clearLastError();

  if (text.isEmpty())
    return ExpressionUniquePtr(makeEmptyExpression());

  m_text = preprocess(text);
  m_text.append(' ');           // the last character must be a space ( for func() )
  m_current_pos = 0;

  return ExpressionUniquePtr(stringExpression(Wrapper::Expression, true));
}

QString ExpressionBuilder::preprocess(const QString &text) const
{
  QString result;

  const int text_size = text.size();
  int i = 0;
  while (i < text_size) {
    const ushort ch = text.at(i).unicode();
    const ushort ch_next = (i + 1) < text_size ? text.at(i + 1).unicode()
                                             : 0;
    switch (ch) {
    case '<':
      switch (ch_next) {
      case '<': {                                // '<<' -> '≪'
          result.append(PseudoSign::MuchLess);
          ++i;
          break;
        }
      case '>': {                                // '<>' -> '≠'
          result.append(PseudoSign::NotEqual);
          ++i;
          break;
        }
      case '=': {                                // '<=' -> '≤'
          result.append(PseudoSign::LessOrEqual);
          ++i;
          break;
        }
      case '~': {                               // '<~' -> '≲'
          result.append(PseudoSign::ApproxLess);
          ++i;
          break;
        }
      default:
        result.append(ch);
        break;
      }  // switch (ch_next)
      break;

    case '>':
      switch (ch_next) {
      case '=': {                                   // '>=' -> '≥'
          result.append(PseudoSign::GreaterOrEqual);
          ++i;
          break;
        }
      case '>': {                                  // '>>' -> '≫'
          result.append(PseudoSign::MuchGreater);
          ++i;
          break;
        }
      case '~': {                                  // '>~' -> '≳'
          result.append(PseudoSign::ApproxGreater);
          ++i;
          break;
        }
      default:
        result.append(ch);
        break;
      }  // switch (ch_next)
      break;

    case '=':
      switch (ch_next) {
      case '=': {                                // '==' -> '≡'
          result.append(PseudoSign::Equivalent);
          ++i;
          break;
        }
      case '~': {                                // '=~' -> '≃'
          result.append(PseudoSign::AlmostEqual);
          ++i;
          break;
        }
      default:
        result.append(ch);
        break;
      }  // switch (ch_next)
      break;

    case '~':
      if (ch_next == '~') {            // '~~' -> '≈'
        result.append(PseudoSign::ApproxEqual);
        ++i;
      }
      else {
        result.append(ch);
      }
      break;

    case '+':
      if (ch_next == '-') {           // '+-' -> '±'
        result.append(PseudoSign::PlusMinus);
        ++i;
      }
      else {
        result.append(ch);
      }
      break;

    case '-':
      switch (ch_next) {
      case '+': {                                        // '-+' -> '∓'
          result.append(PseudoSign::MinusPlus);
          ++i;
          break;
        }
      case '>': {                                       // '->' -> '→'
          result.append(PseudoSign::Arrow);
          ++i;
          break;
        }
      default:
        result.append(ch);
        break;
      }  // switch (ch_next)
      break;

    case '/':
      switch (ch_next) {
      case '/': {
          result.append(PseudoSign::DivideSlash);
          ++i;
          break;
        }
      case '+': {                                // '/+' -> '÷'
          result.append(PseudoSign::Divide);
          ++i;
          break;
        }
      default:
        result.append(ch);
        break;
      } // switch (ch_next)
      break;

    case '*': 
      switch (ch_next) {
      case '+': {                                    // '*+' -> 'x'
          result.append(PseudoSign::MultiplyCross);
          ++i;
          break;
        }
      case '*': {                                   // '**'
          result.append(PseudoSign::Multiply);
          ++i;
          break;
        }
      case '.': {                                  // '*.' -> '·'
          result.append(PseudoSign::MultiplyDot);
          ++i;
          break;
        }
      default:
        result.append(ch);
        break;
      } // switch (ch_next)
      break;

    case '!':
      switch (ch_next) {
      case '(': {                                       // !(
          result.append(PseudoSign::NecessarilyBracket);
          ++i;
          break;
        }
      case '=': {                                       // '!=' -> '≠'
          result.append(PseudoSign::NotEqual);
          ++i;
          break;
        }
      default:
        result.append(ch);
        break;
      }  // switch (ch_next)
      break;

    case ' ': {
      const ushort ch_next_next = (i + 2) < text_size ? text.at(i + 2).unicode()
                                                    : 0;
      if ((ch_next == '&') && (ch_next_next == ' ')) {     // " & " -> "&"
        result.append(PseudoSign::Ampersand);
        i += 2;
      }
      else {
        result.append(ch);
      }
      break;
    }

    case '.': {
        const ushort ch_next_next = (i + 2) < text_size ? text.at(i + 2).unicode()
                                                      : 0;
      if ((ch_next == '.') && (ch_next_next == '.')) {  // ...
        result.append(PseudoSign::Ellipsis);
        i += 2;
      }
      else {
        result.append(ch);
      }
      break;
    }

    default:
      result.append(ch);
      break;
    } // switch (ch)

    ++i;
  } // while

  return result;
}

void ExpressionBuilder::skipSpaces()
{
  const int text_size = m_text.size();
  while ( (m_current_pos < text_size)
          && m_text.at(m_current_pos).unicode() == ' ') {
    ++m_current_pos;
  }

  if (m_current_pos >= text_size)
    m_current_pos = text_size - 1;
}

bool ExpressionBuilder::skipComma()
{
  const bool result = currentChar() == ',';
  if (result) {
    ++m_current_pos;
    skipSpaces();
  }
  return result;
}

bool ExpressionBuilder::skipRequiredComma()
{
  clearLastError();
  const bool result = skipComma();

  if ( !result ) {
    const int original_pos = getOriginalPosition();
    m_last_error = Error(ErrorCode::ExpectedComma,
                         MESSAGE::ERROR_EXPECTED_COMMA.arg(original_pos), original_pos);
  }

  return result;
}

ExpressionPtr ExpressionBuilder::wrap(ExpressionPtr expr, Wrapper wrapper, WrapperFlags flags) const
{
  switch (wrapper) {
  case Wrapper::Expression:
    if (expr->hasNext()) {
      return new ChainExpression(expr);
    }
    break;
  case Wrapper::Bracketed:{
    return new RoundBracketedExpression(expr);
    }
    //break;
  case Wrapper::Argument:
    if (expr->hasNext()) {
      return new ArgumentExpression(expr);
    }
    break;
  case Wrapper::Power:
    if (expr->hasNext()) {
      return new BaseExpression(expr);
    }
    else if (flags.testFlag(WrapperFlag::Powered)) {
      return new BracketedExpression(expr);
    }
    break;
  case Wrapper::Abs:{
      return new BracketedExpression(expr, BracketedExpression::Module);
    }
    //break;
  case Wrapper::Squared: {
      return new BracketedExpression(expr, BracketedExpression::Square);
    }
    //break;
  case Wrapper::Figured: {
      return new BracketedExpression(expr, BracketedExpression::Figure);
    }
    //break;
  default:
    break;
  } // switch

  return expr;
}

ExpressionPtr ExpressionBuilder::stringExpression(Wrapper wrapper, bool allowComma)
{
  WrapperFlags flags;
  ExpressionPtr result = booleanExpression(flags);

  if ( !stopBuild() ) {
    ushort ch = currentChar().unicode();
    while ( ch == '&' || ch == PseudoSign::Ampersand || (ch == ',' && allowComma) ) {
      ushort sep = ch;
      ++m_current_pos;

      if (sep == ',')
        skipSpaces();

      ExpressionPtr expr = booleanExpression(flags);
      if (sep == '&' || sep == PseudoSign::Ampersand) {
        result->addNext(expr);
      }
      else {
        result->addNext(new CommaExpression());
        result->addNext(new SpaceExpression(7));
        result->addNext(expr);
      }

      if (stopBuild())
        break;

      ch = currentChar().unicode();
    }  // while
  }

  return wrap(result, wrapper, flags);
}

ExpressionPtr ExpressionBuilder::booleanExpression(WrapperFlags& flags)
{
  static const QMap<ushort, SignExpression::SignType> SIGNS {
    {PseudoSign::NotEqual, SignExpression::SignType::NotEqualTo},
    {PseudoSign::GreaterOrEqual, SignExpression::SignType::GreaterOrEqual},

    {PseudoSign::LessOrEqual, SignExpression::SignType::LessOrEqual},
    {PseudoSign::MuchGreater, SignExpression::SignType::MuchGreater},
    {PseudoSign::MuchLess, SignExpression::SignType::MuchLess},
    {PseudoSign::Arrow, SignExpression::SignType::Arrow},
    {PseudoSign::ApproxEqual, SignExpression::SignType::ApproxEqual},
    {PseudoSign::Equivalent, SignExpression::SignType::IdenticalTo},
    {PseudoSign::ApproxGreater, SignExpression::SignType::ApproxGreater},
    {PseudoSign::ApproxLess, SignExpression::SignType::ApproxLess},
    {PseudoSign::AlmostEqual, SignExpression::SignType::AlmostEqual},
    {'<', SignExpression::SignType::Less},
    {'=', SignExpression::SignType::Equals},
    {'>', SignExpression::SignType::Greater},
    {'~', SignExpression::SignType::Tilda}
  };

  WrapperFlags _flags;
  ExpressionPtr result = expression(_flags);

  if ( !stopBuild() ) {
    ushort ch = currentChar().unicode();
    while (SIGNS.contains(ch)) {
      ++m_current_pos;

      result->addNext(new SignExpression(SIGNS.value(ch)));
      result->addNext( expression(_flags) );

      if (stopBuild())
        break;

      ch = currentChar().unicode();
    } // while

    flags = result->hasNext() ? WrapperFlags(WrapperFlag::Powered)
                              : _flags;
  }

  return result;
}

ExpressionPtr ExpressionBuilder::expression(WrapperFlags &flags)
{
  static const QMap<ushort, SignExpression::SignType> SIGNS {
    {PseudoSign::PlusMinus, SignExpression::SignType::PlusMinus},
    {PseudoSign::MinusPlus, SignExpression::SignType::MinusPlus},
    {'-', SignExpression::SignType::Minus},
    {'+', SignExpression::SignType::Plus}
  };

  WrapperFlags _flags;
  ExpressionPtr result = trans(_flags);

  if ( !stopBuild() ) {
    ushort ch = currentChar().unicode();
    while (SIGNS.contains(ch)) {
      ++m_current_pos;
      ExpressionPtr expr = trans(_flags);

      if (_flags.testFlag(WrapperFlag::Trans)) {
        BracketedExpression* brackets = new BracketedExpression();
        brackets->addNext(expr);
        expr = brackets;
      }

      result->addNext( new SignExpression(SIGNS.value(ch)) );
      result->addNext(expr);

      if (stopBuild())
        break;

      ch = currentChar().unicode();
    } // while

    flags = result->hasNext() ? WrapperFlags(WrapperFlag::Powered)
                              : _flags;
  }

  return result;
}

ExpressionPtr ExpressionBuilder::trans(WrapperFlags &flags)
{
  static const QMap<ushort, SignExpression::SignType> SIGNS {
    {PseudoSign::Divide, SignExpression::SignType::Divide},
    {PseudoSign::DivideSlash, SignExpression::SignType::Slash},
    {PseudoSign::MultiplyCross, SignExpression::SignType::CrossMultiply},
    {PseudoSign::MultiplyDot, SignExpression::SignType::Multiply},
    {PseudoSign::Multiply,  SignExpression::SignType::Multiply},   // any SignType, value is not used
    {'*', SignExpression::SignType::CrossMultiply},      // any SignType, value is not used
    {'/', SignExpression::SignType::Slash}               // any SignType, value is not used
  };

  WrapperFlags _flags;
  ExpressionPtr d1 = factor(_flags);
  ExpressionPtr d2 = nullptr;

  if ( !stopBuild() ) {
    ushort ch = currentChar().unicode();
    while (SIGNS.contains(ch)) {
      ++m_current_pos;

      switch (ch) {
      case PseudoSign::Multiply: {
          d1->addNext( factor(_flags) );
          break;
        }
      case '*': {
          ExpressionPtr expr = factor(_flags);
          addMult(&d1, &expr);
          break;
        }
      case '/': {
          ExpressionPtr expr = factor(_flags);
          addMult(&d2, &expr);
          break;
        }
      default: {
          ExpressionPtr expr = factor(_flags);
          d1->addNext( new SignExpression(SIGNS.value(ch)) );
          d1->addNext(expr);
          break;
        }
      }  // switch

      if (stopBuild())
        break;

      ch = currentChar().unicode();
    }  // while

    if (d2 && !d1->hasNext())
      flags.setFlag(WrapperFlag::NoFlag);
  }

  if (d2) {
    if (d1->hasNext()) {
      d1 = new ChainExpression(d1);
    }

    if (d2->hasNext()) {
      d2 = new ChainExpression(d2);
    }

    flags.setFlag(WrapperFlag::Powered);

    if (d1->multiplicationFlags().testFlag(MultiplicationFlag::RoundBrackets)) {
      BracketedExpression* brackets = dynamic_cast<BracketedExpression*>(d1);
      if (brackets)
        brackets->removeBrackets();
    }

    if (d2->multiplicationFlags().testFlag(MultiplicationFlag::RoundBrackets)) {
      BracketedExpression* brackets = dynamic_cast<BracketedExpression*>(d2);
      if (brackets)
        brackets->removeBrackets();
    }

    return new FractionExpression(d1, d2);
  }
  else {
    return d1;
  }
}

ExpressionPtr ExpressionBuilder::factor(WrapperFlags &flags)
{
  clearLastError();
  flags.setFlag(WrapperFlag::NoFlag);

  if (stopBuild())
    return makeEmptyExpression();

  ExpressionPtr result = nullptr;
  WrapperFlags dummy_flags;

  ushort ch = currentChar().unicode();
  switch (ch) {
  case '0': case '1': case '2': case '3': case '4':
  case '5': case '6': case '7': case '8': case '9': {
      int next_pos = -1;
      const double number = getDouble(m_text, m_current_pos, next_pos);
      result = new NumberExpression(number, NumberExpression::Style::Regular);
      if ( !stopBuild() )
        m_current_pos = next_pos;

      break;
    }
  case '#': {
      ++m_current_pos;
      if (currentChar().isDigit()) {
        int next_pos = -1;
        const double number = getDouble(m_text, m_current_pos, next_pos);
        result = new NumberExpression(number, NumberExpression::Style::Exponential);
        if ( !stopBuild() )
          m_current_pos = next_pos;
      }
      else {
        result = makeEmptyExpression();
        const int original_pos = getOriginalPosition();
        m_last_error = Error(ErrorCode::ExpectedNumber,
                             MESSAGE::ERROR_EXPECTED_NUMBER.arg(original_pos), original_pos);
      }
      break;
    }
  case '+': {
      ++m_current_pos;
      flags.setFlag(WrapperFlag::Trans);
      result = new SignExpression(SignExpression::SignType::Plus);
      result->addNext(factor(dummy_flags));
      break;
    }
  case '-': {
      ++m_current_pos;
      result = new SignExpression(SignExpression::SignType::Minus);
      result->addNext(factor(dummy_flags));
      break;
    }
  case '[': {
      ++m_current_pos;
      result = stringExpression(Wrapper::Squared, true);
      if (currentChar().unicode() == ']')
        ++m_current_pos;
      else {
        const int original_pos = getOriginalPosition();
        m_last_error = Error(ErrorCode::ExpectedSquareBracket,
                             MESSAGE::ERROR_EXPECTED_SQUARE_BRACKET.arg(original_pos), original_pos);
      }
      break;
    }
  case '{': {
      ++m_current_pos;
      result = stringExpression(Wrapper::Figured, true);
      if (currentChar().unicode() == '}')
        ++m_current_pos;
      else {
        const int original_pos = getOriginalPosition();
        m_last_error = Error(ErrorCode::ExpectedFigureBracket,
                             MESSAGE::ERROR_EXPECTED_FIGURE_BRACKET.arg(original_pos), original_pos);
      }
      break;
    }
  case '(': {
      ++m_current_pos;
      result = stringExpression(Wrapper::Argument, true);
      if (currentChar().unicode() == ')')
        ++m_current_pos;
      else {
        const int original_pos = getOriginalPosition();
        m_last_error = Error(ErrorCode::ExpectedRoundBracket,
                             MESSAGE::ERROR_EXPECTED_ROUND_BRACKET.arg(original_pos), original_pos);
      }
      break;
    }
  case '|': {
      ++m_current_pos;
      result = stringExpression(Wrapper::Abs, true);
      if (currentChar().unicode() == '|')
        ++m_current_pos;
      else {
        const int original_pos = getOriginalPosition();
        m_last_error = Error(ErrorCode::ExpectedModuleBracket,
                             MESSAGE::ERROR_EXPECTED_MODULE_BRACKET.arg(original_pos), original_pos);
      }
      break;
    }
  case '_': {
      ++m_current_pos;
      result = makeCap(factor(dummy_flags), CapExpression::CapStyle::Vector , 0);
      break;
    }
  case PseudoSign::PlusMinus: {
      ++m_current_pos;
      flags.setFlag(WrapperFlag::Trans);
      result = new SignExpression(SignExpression::SignType::PlusMinus);
      result->addNext(factor(dummy_flags));
      break;
    }
  case PseudoSign::MinusPlus: {
      ++m_current_pos;
      flags.setFlag(WrapperFlag::Trans);
      result = new SignExpression(SignExpression::SignType::MinusPlus);
      result->addNext(factor(dummy_flags));
      break;
    }
  case PseudoSign::NecessarilyBracket: {
      ++m_current_pos;
      result = stringExpression(Wrapper::Bracketed, true);
      if (currentChar().unicode() == ')')
        ++m_current_pos;
      else {
        const int original_pos = getOriginalPosition();
        m_last_error = Error(ErrorCode::ExpectedRoundBracket,
                             MESSAGE::ERROR_EXPECTED_ROUND_BRACKET.arg(original_pos), original_pos);
      }
      break;
    }
  case PseudoSign::Ellipsis: {
      ++m_current_pos;
      result = new SignExpression(SignExpression::SignType::Ellipsis);
      break;
    }
  default: {
      static const QRegExp reg_exp = QRegExp(QStringLiteral("[A-Z]|[a-z]|[А-я]|[Ёё]"));
      if (QString(ch).contains(reg_exp))
        result = func(flags);
      else {
        result = makeEmptyExpression();
        const int original_pos = getOriginalPosition();
        m_last_error = Error(ErrorCode::InvalidCharacter,
                             MESSAGE::ERROR_INVALID_CHARACTER.arg(original_pos), original_pos);
      }
      break;
    }
  } // switch

  if ( !stopBuild() && postSymbols()) {
    ch = currentChar().unicode();
    while (ch == '^' || ch == '_' || ch == '!' || ch == '`') {
      switch (ch) {
      case '^': {
          ++m_current_pos;
          ExpressionPtr expr = factor(dummy_flags);
          if ( ArgumentExpression* arg = dynamic_cast<ArgumentExpression*>(expr) ) {
            arg->removeBrackets();
          }
          decorate(&result);
          result = makePower(result, expr);
          flags.setFlag(WrapperFlag::Powered);
          break;
        }
      case '_': {
          ++m_current_pos;
          postSymbols(false);
          ExpressionPtr expr = factor(dummy_flags);
          postSymbols(true);
          if ( ArgumentExpression* arg = dynamic_cast<ArgumentExpression*>(expr) ) {
            arg->removeBrackets();
          }
          decorate(&result);
          result = makeIndex(result, expr);
          break;
        }
      case '!': {
          ++m_current_pos;
          decorate(&result);
          result->addNext(new SimpleExpression('!'));
          break;
        }
      case '`': {
          ++m_current_pos;
          int count_strokes = 1;
          while (currentChar().unicode() == '`') {
            ++m_current_pos;
            ++count_strokes;
          }
          decorate(&result);
          result = makePower(result, new StrokesExpression(count_strokes));
          break;
        }
      default:
        break;
      } // switch

      if (stopBuild())
        break;

      ch = currentChar().unicode();
    }  // while
  }  // if

  return result;
}

ExpressionPtr ExpressionBuilder::func(WrapperFlags &flags)
{
  static const QRegExp reg_exp = QRegExp(QStringLiteral("[A-Z]|[a-z]|[А-я]|[Ёё]|[0-9]"));

  QString function_name;
  QChar ch = currentChar();
  while ( QString(ch).contains(reg_exp) ) {
    function_name.append(ch);
    ++m_current_pos;

//    if (isEnd())
//      break;

    ch = currentChar();
  }

  if (ch.unicode() == '(' || ch.unicode() == PseudoSign::NecessarilyBracket) {
    ++m_current_pos;
    ExpressionPtr result = funcName(function_name, flags, ch.unicode() == PseudoSign::NecessarilyBracket);

    if (lastError().isValid())
      return result;

    if ( !isEnd() && currentChar().unicode() == ')' )
      ++m_current_pos;
    else {
      const int original_pos = getOriginalPosition();
      m_last_error = Error(ErrorCode::ExpectedRoundBracket,
                           MESSAGE::ERROR_EXPECTED_ROUND_BRACKET.arg(original_pos), original_pos);
    }

    return result;
  }
  else {
    bool has_index = false;
    const int index_number = variableAutoIndex() ? cutFuncIndex(function_name, has_index) : 0;

    ExpressionPtr result = token(function_name);
    if (has_index)
      result = new IndexExpression(result, new NumberExpression(index_number), nullptr);

    return result;
  }
}

ExpressionPtr ExpressionBuilder::funcName(const QString &name, WrapperFlags &flags, bool needBrackets)
{
  flags.setFlag(WrapperFlag::NoFlag);
  const QString name_to_upper = name.toUpper();

  if (name_to_upper == QLatin1Literal("SQRT")) {
    ExpressionPtr expr = stringExpression(Wrapper::Expression);
    return new RootExpression(expr);
  }
  else if (name_to_upper == QLatin1Literal("SQR")) {
    flags.setFlag(WrapperFlag::Powered);
    return makePower(stringExpression(Wrapper::Power), new NumberExpression(2.0));
  }
  else if (name_to_upper == QLatin1Literal("LOG")) {
    return functionLOG(name, needBrackets);
  }
  else if (name_to_upper == QLatin1Literal("ABS")) {
    return stringExpression(Wrapper::Abs);
  }
  else if (name_to_upper == QLatin1Literal("POW")) {
    return functionPOW(flags);
  }
  else if (name_to_upper == QLatin1Literal("ROOT")) {
    return functionROOT();
  }
  else if (name_to_upper == QLatin1Literal("IND")) {
    return functionIND();
  }
  else if (name_to_upper == QLatin1Literal("LIM")) {
    return functionLIM(flags, needBrackets);
  }
  else if (name_to_upper == QLatin1Literal("FUNC")) {
    return functionFUNC(needBrackets);
  }
  else if (name_to_upper == QLatin1Literal("FRAME")) {
    return functionFRAME();
  }
  else if (name_to_upper == QLatin1Literal("SPACE")) {
    return functionSPACE();
  }
  else if (name_to_upper == QLatin1Literal("DIFF")) {
    return functionDIFF();
  }
  else if (name_to_upper == QLatin1Literal("PDIFF")) {
    return functionPDIFF();
  }
  else if (name_to_upper == QLatin1Literal("DIFFN")) {
    return functionDIFFN();
  }
  else if (name_to_upper == QLatin1Literal("PDIFFN")) {
    return functionPDIFFN();
  }
  else if (name_to_upper == QLatin1Literal("DIFFR")) {
    return functionDIFFR();
  }
  else if (name_to_upper == QLatin1Literal("PDIFFR")) {
    return functionPDIFFR();
  }
  else if (name_to_upper == QLatin1Literal("DIFFRF")) {
    return functionDIFFRF();
  }
  else if (name_to_upper == QLatin1Literal("PDIFFRF")) {
    return functionPDIFFRF();
  }
  else if (name_to_upper == QLatin1Literal("STRING")) {
    return functionSTRING();
  }
  else if (name_to_upper == QLatin1Literal("STROKES")) {
    return functionSTROKES(flags);
  }
  else if (name_to_upper == QLatin1Literal("FACT")) {
    return functionFACT(flags);
  }
  else if (name_to_upper == QLatin1Literal("AT")) {
    return functionAT();
  }
  else if (name_to_upper == QLatin1Literal("LINE")) {
    return functionCAPTION(CapExpression::CapStyle::Line);
  }
  else if (name_to_upper == QLatin1Literal("VECT")) {
    return functionCAPTION(CapExpression::CapStyle::Vector);
  }
  else if (name_to_upper == QLatin1Literal("CAP")) {
    return functionCAPTION(CapExpression::CapStyle::Cap);
  }
  else if (name_to_upper == QLatin1Literal("TILDA")) {
    return functionCAPTION(CapExpression::CapStyle::Tilde);
  }
  else if (name_to_upper == QLatin1Literal("POINTS")) {
    return functionPOINTS();
  }
  else if (name_to_upper == QLatin1Literal("STANDL")) {
    return functionSTAND(Qt::AlignLeft);
  }
  else if (name_to_upper == QLatin1Literal("STANDC")) {
    return functionSTAND(Qt::AlignHCenter);
  }
  else if (name_to_upper == QLatin1Literal("STANDR")) {
    return functionSTAND(Qt::AlignRight);
  }
  else if (name_to_upper == QLatin1Literal("MATRIX")) {
    return functionMATRIX();
  }
  else if (name_to_upper == QLatin1Literal("SUMMA")) {
    return functionGroupOp(GroupType::Summa, needBrackets);
  }
  else if (name_to_upper == QLatin1Literal("PROD")) {
    return functionGroupOp(GroupType::Product, needBrackets);
  }
  else if (name_to_upper == QLatin1Literal("CIRC")) {
    return functionGroupOp(GroupType::Circulation, needBrackets);
  }
  else if (name_to_upper == QLatin1Literal("INT")) {
    return functionGroupOp(GroupType::Integral, needBrackets);
  }
  else if (name_to_upper == QLatin1Literal("INTM")) {
    return functionGroupOp(GroupType::IntegralM, needBrackets);
  }
  else if (name_to_upper == QLatin1Literal("CASE")) {
    return functionCASE();
  }
  else if (name_to_upper == QLatin1Literal("COMMA")) {
    return functionCOMMA();
  }
  else if (name_to_upper == QLatin1Literal("BRACKETS")) {
    return functionBRACKETS();
  }
  else if (name_to_upper == QLatin1Literal("SYSTEM")) {
    return functionSYSTEM();
  }
  else if (name_to_upper == QLatin1Literal("NUM")) {
    return functionNUM();
  }
  else if (name_to_upper == QLatin1Literal("SYMBOL")) {
    return functionSYMBOL();
  }
  else if (name_to_upper == QLatin1Literal("ANGLE")) {
    return functionANGLE();
  }
//  else if (name_to_upper == QLatin1Literal("COLOR")) {
//    return functionCOLOR();
//  }
  // else

  ExpressionPtr expr = stringExpression(Wrapper::Chain, true);
  ArgumentExpression* arg = new ArgumentExpression(expr);
  arg->setForsedBrackets(needBrackets);

  bool has_index = false;
  QString function_name = name;
  const int index_number = functionAutoIndex() ? cutFuncIndex(function_name, has_index) : 0;

  const ushort greek_letter = greekLetterCode(function_name);
  if (greek_letter == 0) {
    ExpressionPtr result = new FunctionExpression(function_name, arg);
    if (has_index)
      result = makeIndex(result, new NumberExpression(index_number));
    return result;
  }
  else {
    ExpressionPtr greek_expr = greek_letter == PseudoSign::Plambda ? new LambdaExpression()
                                                                   : new CharacterExpression(QChar(greek_letter));
    ExpressionPtr result = new CommonFunctionExpression(greek_expr, arg);
    if (has_index)
      result = makeIndex(result, new NumberExpression(index_number));

    return result;
  }

}

ExpressionPtr ExpressionBuilder::token(const QString &name)
{
  {
    const ushort greek_letter = greekLetterCode(name);
    if (greek_letter != 0) {
      if (greek_letter == PseudoSign::Plambda)
        return new LambdaExpression();
      else
        return new CharacterExpression(QChar(greek_letter));
    }
  }

  const QString name_to_upper = name.toUpper();
  if (name_to_upper == QLatin1Literal("INF") ) {
    return new SignExpression(SignExpression::SignType::Infinity);
  }
  else if (name_to_upper == QLatin1Literal("PLANK")) {
    return new PlanckExpression();
  }
  else if (name_to_upper == QLatin1Literal("NIL")
           || name_to_upper == QLatin1Literal("NULL") ) {
    return new NullExpression();
  }
  else if (name_to_upper == QLatin1Literal("COMMA")) {
    return new CommaExpression();
  }
  else if (name_to_upper == QLatin1Literal("CONST")) {
    ExpressionPtr expr = new SimpleExpression(name);
    expr->addNext(new SpaceExpression(3));
    return expr;
  }
  else if (name_to_upper == QLatin1Literal("ASTERIX")) {
    return new AsterixExpression();
  }
  else if (name_to_upper == QLatin1Literal("MINUS")) {
    return new SignExpression(SignExpression::SignType::Minus);
  }
  else if (name_to_upper == QLatin1Literal("PARALLEL")) {
    return new SignExpression(SignExpression::SignType::Parallel);
  }
  else if (name_to_upper == QLatin1Literal("PERPENDICULAR")) {
    return new SignExpression(SignExpression::SignType::Perpendicular);
  }
  else if (name_to_upper == QLatin1Literal("ANGLE")) {
    return new SignExpression(SignExpression::SignType::Angle);
  }
  else if (name_to_upper == QLatin1Literal("EMPTY")) {
    return new EmptyExpression();
  }
  else {
    static const QRegExp reg_exp = QRegExp("[A-Z]|[a-z]");
    if ( QString(name.at(0)).contains(reg_exp) )
      return new VariableExpression(name);
  }

  return new SimpleExpression(name);
}

ExpressionPtr ExpressionBuilder::makeCap(ExpressionPtr base, CapExpression::CapStyle style, int count) const
{

  if ( CommonFunctionExpression* com_func = dynamic_cast<CommonFunctionExpression*>(base) ) {
    // exemple: _log(β,x+1) - The cap will only be above the "logᵦ" expression.
    ExpressionPtr cap = makeCap(com_func->cutOffSun(), style, count);
    com_func->setSon(cap);
    return base;
  }

  {
    IndexExpression* index = dynamic_cast<IndexExpression*>(base);
    if ( index && index->hasSecondTwin() ) {
      // exemple: _s_2 - The cap will only be above the "s" expression.
      CapExpression* cap = new CapExpression(index->cutOffSun(), style);
      cap->setPointCount(count);
      index->setSon(cap);
      return base;
    }
  }

  CapExpression* cap = new CapExpression(base, style);
  cap->setPointCount(count);
  return cap;
}

ExpressionPtr ExpressionBuilder::makePower(ExpressionPtr base, ExpressionPtr exponent) const
{
  if ( CommonFunctionExpression* com_func = dynamic_cast<CommonFunctionExpression*>(base) ) {
    IndexExpression* son_as_index = dynamic_cast<IndexExpression*>(com_func->son());
    if (son_as_index && !son_as_index->hasSuperscript()) {
      // exemple: log(β,x+1)^2 -> log²ᵦ (x+1)
      son_as_index->setSuperscript(exponent);
      return base;
    }

    if ( !son_as_index ) {
      // exemple: func(f,a)^2 -> f²(a)
      son_as_index = new IndexExpression(com_func->cutOffSun(), nullptr, exponent);
      com_func->setSon(son_as_index);
      return base;
    }
  }

  {
    IndexExpression* index = dynamic_cast<IndexExpression*>(base);
    if (index && !index->hasSuperscript()) {
      // exemple: s_2^3 -> s₂³
      index->setSuperscript(exponent);
      return base;
    }
  }

  return new IndexExpression(base, nullptr, exponent);
}

ExpressionPtr ExpressionBuilder::makeIndex(ExpressionPtr base, ExpressionPtr index) const
{
  if ( CommonFunctionExpression* com_func = dynamic_cast<CommonFunctionExpression*>(base) ) {
    IndexExpression* son_as_index = dynamic_cast<IndexExpression*>(com_func->son());
    if (son_as_index && !son_as_index->hasSubscript()) {
      // exemple: func(f^3,a)_2 -> f₂³(a)
      son_as_index->setSubscript(index);
      return base;
    }

    if ( !son_as_index ) {
      // exemple: func(f,a)_2 -> f₂(a)
      son_as_index = new IndexExpression(com_func->cutOffSun(), index, nullptr);
      com_func->setSon(son_as_index);
      return base;
    }
  }

  return new IndexExpression(base, index, nullptr);
}

ExpressionPtr ExpressionBuilder::makeEmptyExpression() const
{
  if (wrapEmptyExpressionToFrame()) {
     ExpressionPtr empty = new CharacterExpression('_');
     return new FrameExpression(empty);
  }
  else {
    return new EmptyExpression();
  }
}

int ExpressionBuilder::getOriginalPosition() const
{
  static const QSet<ushort> SIGNS {
        PseudoSign::NotEqual,
        PseudoSign::GreaterOrEqual,
        PseudoSign::LessOrEqual,
        PseudoSign::MuchGreater,
        PseudoSign::MuchLess,
        PseudoSign::Arrow,
        PseudoSign::ApproxEqual,
        PseudoSign::PlusMinus,
        PseudoSign::MinusPlus,
        PseudoSign::Equivalent,
        PseudoSign::Divide,
        PseudoSign::MultiplyCross,
        PseudoSign::Ellipsis,
        PseudoSign::ApproxGreater,
        PseudoSign::ApproxLess,
        PseudoSign::DivideSlash,
        PseudoSign::NecessarilyBracket,
        PseudoSign::MultiplyDot,
        PseudoSign::AlmostEqual,
        PseudoSign::Ampersand
  };

  int count = 0;
  int i = 0;
  // m_current_pos can be more then m_text.size()
  while (i <= m_current_pos && i < m_text.size()) {
    const ushort ch = m_text.at(i).unicode();
    if (SIGNS.contains(ch)) {
      count += (ch == PseudoSign::Ellipsis || ch == PseudoSign::Ampersand) ? 2 : 1;
    }

    ++i;
  }  // while

  return m_current_pos + count + 1;
}

void ExpressionBuilder::addMult(ExpressionPtr* _existing, ExpressionPtr* _multiplier) const
{
  ExpressionPtr exist = *_existing;
  ExpressionPtr multi = *_multiplier;

  // Note: 'expr_exist' and 'expr_multi' can be nullptr
  if ( !multi )
    return;

  if ( !exist ) {
    *_existing = *_multiplier;
    return;
  }

  ExpressionPtr last_exist = exist;
  while (last_exist->hasNext()) {
    last_exist = last_exist->next();
  }

  ExpressionPtr last_multi = multi;
  while (last_multi->hasNext()) {
    last_multi = last_multi->next();
  }

  // exepmle: a*b -> ab
  if ( last_exist->multiplicationFlags().testFlag(MultiplicationFlag::Right)
       && multi->multiplicationFlags().testFlag(MultiplicationFlag::Left) ) {
    exist->addNext(multi);
    return;
  }

  // exemple: a*2 -> 2a
  if ( last_multi->multiplicationFlags().testFlag(MultiplicationFlag::Right)
       && exist->multiplicationFlags().testFlag(MultiplicationFlag::Left)) {
    multi->addNext(exist);
    *_existing = *_multiplier;
    return;
  }

  // exemple: 2*4 = 8
  {
    NumberExpression* exist_as_number = dynamic_cast<NumberExpression*>(exist);
    NumberExpression* last_multi_as_number = dynamic_cast<NumberExpression*>(last_multi);
    if ( exist_as_number && last_multi_as_number ) {
      last_multi_as_number->setNumber(last_multi_as_number->number() * exist_as_number->number());
      last_multi->setNext(exist->cutOff());
      delete exist;
      *_existing = multi;
      return;
    }
  }

  // exemple: ??? // TODO: add exemple
  if ( multi->multiplicationFlags().testFlag(MultiplicationFlag::Left)
       && last_multi->multiplicationFlags().testFlag(MultiplicationFlag::Right)) {
    ExpressionPtr tmp_exist = exist;
    while (tmp_exist->hasNext()) {
      if ( tmp_exist->multiplicationFlags().testFlag(MultiplicationFlag::Right)
           && tmp_exist->next()->multiplicationFlags().testFlag(MultiplicationFlag::Left))
        break;
      tmp_exist = tmp_exist->next();
    }

    if (tmp_exist->hasNext()) {
      last_multi->setNext(tmp_exist->cutOff());
      tmp_exist->setNext(multi);
    }

    return;
  }

  // exemple: sin(x)*cos(x) -> sin(x)·cos(x)
  exist->addNext(new SignExpression(SignExpression::SignType::Multiply));
  exist->addNext(multi);
}

void ExpressionBuilder::decorate(ExpressionPtr* _base) const
{
  ChainExpression* chain = dynamic_cast<ChainExpression*>(*_base);
  if ( chain && chain->hasSon() && chain->son()->hasNext() ) {
    ExpressionPtr son = chain->cutOffSun();
    delete *_base;
    *_base = new BracketedExpression(son, BracketedExpression::Round);
  }
}

int ExpressionBuilder::getInt(const QString& str, int from, int& next_pos)
{
  // Delphi Var() function analogue
  if (from < 0)
    from = 0;

  for (next_pos = from; next_pos < str.size(); ++next_pos) {
    if ( !str.at(next_pos).isDigit() )
      break;
  }

  bool ok;
  const int result = str.midRef(from, next_pos - from).toInt(&ok);
  if ( !ok ) {
    next_pos = -1;
    m_last_error = Error(ErrorCode::ConvertToNumber,
                         MESSAGE::ERROR_CONVERT_STR_TO_INT, getOriginalPosition());
  }

  return result;
}

double ExpressionBuilder::getDouble(const QString &str, int from, int &next_pos)
{
  // Delphi Var() function analogue
  if (from < 0)
    from = 0;

  int dot_count = 0;
  for (next_pos = from; next_pos < str.size(); ++next_pos) {
    const QChar ch = str.at(next_pos);

    if ( !ch.isDigit() ) {
      if (dot_count == 0 && ch.unicode() == '.')
        ++dot_count;
      else
        break;
    }
  }

  if (next_pos > 0 && next_pos < str.size()) {
    const QChar ch = str.at(next_pos);

    if (ch.unicode() == 'e' || ch.unicode() == 'E') {
      ++next_pos;

      if ( next_pos < str.size() ) {
        const QChar ch = str.at(next_pos);
        if (ch.unicode() == '-' || ch.unicode() == '+')
          ++next_pos;
      }

      for (; next_pos < str.size(); ++next_pos) {
        if ( !str.at(next_pos).isDigit() )
          break;
      } // for
    }
  }

  bool ok;
  const double result = str.midRef(from, next_pos - from).toDouble(&ok);
  if ( !ok ) {
    next_pos = -1;
    m_last_error = Error(ErrorCode::ConvertToNumber,
                         MESSAGE::ERROR_CONVERT_STR_TO_DOUBLE, getOriginalPosition());
  }

  return result;
}

int ExpressionBuilder::cutFuncIndex(QString &name, bool &ok) const
{
  ok = false;
  int result = 0;

  if (name.isEmpty())
    return result;

  const int name_size = name.size();
  int i = name_size;
  ushort ch = name.at(i - 1).unicode();
  while ( ch >= '0' && ch <= '9' ) {
    --i;
    if (i < 0)
      break;
    ch = name.at(i - 1).unicode();
  }

  if ( i < name_size ) {
    result = name.midRef(i).toInt(&ok);
    if (ok)
      name = name.left(i);
  }

  return result;
}

ushort ExpressionBuilder::greekLetterCode(const QString &name) const
{
  static const QHash<QString, ushort> GREEK_ALPHABET {
    {"ALPHA", 913},   // 'Α'
    {"BETA", 914},    // 'Β'
    {"GAMMA", 915},   // 'Γ'
    {"DELTA", 916},   // 'Δ'
    {"EPSILON", 917}, // 'Ε'
    {"ZETA", 918},    // 'Ζ'
    {"ETA", 919},     // 'Η'
    {"THETA", 920},   // 'Θ'
    {"IOTA", 921},    // 'Ι'
    {"KAPPA", 922},   // 'Κ'
    {"LAMBDA", 923},  // 'Λ'
    {"MU", 924},      // 'Μ'
    {"NU", 925},      // 'Ν'
    {"XI", 926},      // 'Ξ'
    {"OMICRON", 927}, // 'Ο'
    {"PI", 928},      // 'Π'
    {"RHO", 929},     // 'Ρ'
    {"SIGMA", 931},   // 'Σ'
    {"TAU", 932},     // 'Τ'
    {"UPSILON", 933}, // 'Υ'
    {"PHI", 934},     // 'Φ'
    {"CHI", 935},     // 'Χ'
    {"PSI", 936},     // 'Ψ'
    {"OMEGA", 937},   // 'Ω'
    {"NABLA", 8711},  // ∇
    {"PLAMBDA", PseudoSign::Plambda}
  };

  const ushort offset = name.at(0).isLower() ? 32 : 0;
  ushort result = GREEK_ALPHABET.value(name.toUpper(), 0);

  if (result != 0 && !(result == PseudoSign::Plambda))
    result += offset;

  return result;
}

ExpressionPtr ExpressionBuilder::functionLOG(const QString& name, bool needBrackets)
{
  ExpressionPtr index = stringExpression(Wrapper::Expression);
  ExpressionPtr expr = nullptr;

  if ( skipRequiredComma() ) {
    expr = stringExpression(Wrapper::Argument);
    if (needBrackets){
      if (ArgumentExpression* expr_as_arg = dynamic_cast<ArgumentExpression*>(expr))
        expr_as_arg->setForsedBrackets(true);
      else
        expr = new RoundBracketedExpression(expr);
    }
  }

  ExpressionPtr log = new IndexExpression(new FunctionNameExpression(name), index, nullptr);
  return new CommonFunctionExpression(log, expr);
}

ExpressionPtr ExpressionBuilder::functionPOW(WrapperFlags &flags)
{
  flags.setFlag(WrapperFlag::Powered);
  ExpressionPtr expression = stringExpression(Wrapper::Power);
  ExpressionPtr superscript = skipRequiredComma() ? stringExpression(Wrapper::Expression) : nullptr;
  return makePower(expression, superscript);
}

ExpressionPtr ExpressionBuilder::functionROOT()
{
  ExpressionPtr index = stringExpression(Wrapper::Expression);
  ExpressionPtr radical_expression = skipRequiredComma() ? stringExpression(Wrapper::Expression) : nullptr;
  return new RootExpression(radical_expression, index);
}

ExpressionPtr ExpressionBuilder::functionIND()
{
  ExpressionPtr expression = stringExpression(Wrapper::Power);
  ExpressionPtr subscript = skipRequiredComma() ? stringExpression(Wrapper::Expression) : nullptr;
  return makeIndex(expression, subscript);
}

ExpressionPtr ExpressionBuilder::functionLIM(WrapperFlags &flags, bool needBrackets)
{
  flags.setFlag(WrapperFlag::Powered);
  ExpressionPtr condition = stringExpression(Wrapper::Power);
  ExpressionPtr expression = skipRequiredComma() ? stringExpression(Wrapper::Expression) : nullptr;

  if (needBrackets) {
    if ( ArgumentExpression* arg = dynamic_cast<ArgumentExpression*>(condition))
      arg->setForsedBrackets(true);
    else
      condition = new RoundBracketedExpression(condition);
  }

  return new CommonFunctionExpression(new LimitExpression(condition), expression);
}

ExpressionPtr ExpressionBuilder::functionFUNC(bool needBrackets)
{
  ExpressionPtr func_name = stringExpression(Wrapper::Power);
  ExpressionPtr expression =  nullptr;

  if (skipRequiredComma()) {
    expression = stringExpression(Wrapper::Chain);

    while (skipComma()) {
      expression->addNext(new CommaExpression());
      expression->addNext(stringExpression(Wrapper::Chain));
    }
  }

  ArgumentExpression* arg = new ArgumentExpression(expression);
  arg->setForsedBrackets(needBrackets);

  return new CommonFunctionExpression(func_name, arg);
}

ExpressionPtr ExpressionBuilder::functionFRAME()
{
  // Frame(E [,n])
  // 'n' - is frame width (optional)
  ExpressionPtr expr = stringExpression(Wrapper::Expression);

  int frame_width = 1;
  if (skipComma()) {
    int next_pos = -1;
    frame_width = getInt(m_text, m_current_pos, next_pos);
    if ( !stopBuild() )
      m_current_pos = next_pos;
  }

  FrameExpression* frame = new FrameExpression(expr);
  frame->setFrameWidth(frame_width);
  return frame;
}

ExpressionPtr ExpressionBuilder::functionSPACE()
{
  int next_pos = -1;
  const int number = getInt(m_text, m_current_pos, next_pos);
  if ( !stopBuild() )
    m_current_pos = next_pos;

  return new SpaceExpression(number);
}

ExpressionPtr ExpressionBuilder::functionDIFF()
{
  // Diff(x [,n]) - dx^n
  // 'n' - is optional
  ExpressionPtr x = stringExpression(Wrapper::Power);
  ExpressionPtr result = new SpaceExpression(4);
  result->addNext(new VariableExpression('d'));

  if (skipComma()) {
    ExpressionPtr n = stringExpression(Wrapper::Expression);
    result->addNext(new IndexExpression(x, nullptr, n));
  }
  else {
    result->addNext(x);
  }

  return result;
}

ExpressionPtr ExpressionBuilder::functionPDIFF()
{
  // PDiff(x [,n]) - ∂x^n
  // 'n' - is optional
  ExpressionPtr x = stringExpression(Wrapper::Power);
  ExpressionPtr result = new SpaceExpression(4);
  result->addNext(new SignExpression(SignExpression::PartialDifferential));

  if (skipComma()) {
    ExpressionPtr n = stringExpression(Wrapper::Expression);
    result->addNext(new IndexExpression(x, nullptr, n));
  }
  else {
    result->addNext(x);
  }

  return result;
}

ExpressionPtr ExpressionBuilder::functionDIFFN()
{
  // DiffN(x [,n]) - d^n x
  // 'n' - is optional
  ExpressionPtr x = stringExpression(Wrapper::Power);
  ExpressionPtr result = new SpaceExpression(4);
  ExpressionPtr d = new VariableExpression('d');

  if (skipComma()) {
    ExpressionPtr n = stringExpression(Wrapper::Power);
    result->addNext(new IndexExpression(d, nullptr, n));
  }
  else {
    result->addNext(d);
  }

  result->addNext(x);
  return result;
}

ExpressionPtr ExpressionBuilder::functionPDIFFN()
{
  // PDiffN(x [,n]) - ∂^n x
  // 'n' - is optional
  ExpressionPtr x = stringExpression(Wrapper::Power);
  ExpressionPtr result = new SpaceExpression(4);
  ExpressionPtr d = new SignExpression(SignExpression::PartialDifferential);

  if (skipComma()) {
    ExpressionPtr n = stringExpression(Wrapper::Power);
    result->addNext(new IndexExpression(d, nullptr, n));
  }
  else {
    result->addNext(d);
  }

  result->addNext(x);
  return result;
}

ExpressionPtr ExpressionBuilder::functionDIFFR()
{
  // DiffR(x [,n]) - d^n/ dx^n
  // 'n' - is optional
  ExpressionPtr x = stringExpression(Wrapper::Power);
  ExpressionPtr denominator = new VariableExpression('d');
  ExpressionPtr numerator = new VariableExpression('d');

  if (skipComma()) {
    const int old_current_position = m_current_pos;
    ExpressionPtr n = stringExpression(Wrapper::Expression);
    denominator->addNext(new IndexExpression(x, nullptr, n));
    m_current_pos = old_current_position;

    n = stringExpression(Wrapper::Expression);
    numerator = new IndexExpression(numerator, nullptr, n);
  }
  else {
    denominator->addNext(x);
  }

  denominator = new ChainExpression(denominator);
  return new FractionExpression(numerator, denominator);
}

ExpressionPtr ExpressionBuilder::functionPDIFFR()
{
  // PDiffR(x [,n]) - ∂^n/ ∂x^n
  // 'n' - is optional
  ExpressionPtr x = stringExpression(Wrapper::Power);
  ExpressionPtr denominator = new SignExpression(SignExpression::PartialDifferential);
  ExpressionPtr numerator = new SignExpression(SignExpression::PartialDifferential);

  if (skipComma()) {
    const int old_current_position = m_current_pos;
    ExpressionPtr n = stringExpression(Wrapper::Expression);
    denominator->addNext(new IndexExpression(x, nullptr, n));
    m_current_pos = old_current_position;

    n = stringExpression(Wrapper::Expression);
    numerator = new IndexExpression(numerator, nullptr, n);
  }
  else {
    denominator->addNext(x);
  }

  denominator = new ChainExpression(denominator);
  return new FractionExpression(numerator, denominator);
}

ExpressionPtr ExpressionBuilder::functionDIFFRF()
{
  // DiffRF(y, x [, n]) - d^n y/ dx^n
  // 'n' - is optional
  ExpressionPtr y = stringExpression(Wrapper::Power);
  ExpressionPtr denominator = new VariableExpression('d');
  ExpressionPtr numerator = new VariableExpression('d');

  ExpressionPtr x = skipRequiredComma() ? stringExpression(Wrapper::Power)
                                        : makeEmptyExpression();
  if (skipComma()) {
    const int old_current_position = m_current_pos;
    ExpressionPtr n = stringExpression(Wrapper::Expression);
    denominator->addNext(new IndexExpression(x, nullptr, n));
    m_current_pos = old_current_position;

    n = stringExpression(Wrapper::Expression);
    numerator = new IndexExpression(numerator, nullptr, n);
  }
  else {
    denominator->addNext(x);
  }

  numerator->addNext(y);
  numerator = new ChainExpression(numerator);
  denominator = new ChainExpression(denominator);
  return new FractionExpression(numerator, denominator);
}

ExpressionPtr ExpressionBuilder::functionPDIFFRF()
{
  // PDiffRF(y, x [, n]) - ∂^n y/ ∂x^n
  // 'n' - is optional
  ExpressionPtr y = stringExpression(Wrapper::Power);
  ExpressionPtr denominator = new SignExpression(SignExpression::PartialDifferential);
  ExpressionPtr numerator = new SignExpression(SignExpression::PartialDifferential);

  ExpressionPtr x = skipRequiredComma() ? stringExpression(Wrapper::Power)
                                        : makeEmptyExpression();
  if (skipComma()) {
    const int old_current_position = m_current_pos;
    ExpressionPtr n = stringExpression(Wrapper::Expression);
    denominator->addNext(new IndexExpression(x, nullptr, n));
    m_current_pos = old_current_position;

    n = stringExpression(Wrapper::Expression);
    numerator = new IndexExpression(numerator, nullptr, n);
  }
  else {
    denominator->addNext(x);
  }

  numerator->addNext(y);
  numerator = new ChainExpression(numerator);
  denominator = new ChainExpression(denominator);
  return new FractionExpression(numerator, denominator);
}

ExpressionPtr ExpressionBuilder::functionSTRING()
{
  const ushort QUOTE = '"';
// TODO refact ExpressionBuilder::functionSTRING
  clearLastError();
  QString string;

  QChar ch = currentChar();
  if (ch.unicode() == QUOTE) {
    // exemple: String("text with (brackets)")
    ++m_current_pos;
    ch = currentChar();

    do {
      if (ch.unicode() != QUOTE) {
        string.append(ch);
      }
      else if (m_text.at(m_current_pos + 1).unicode() == QUOTE) {
        string.append(ch);
        ++m_current_pos;
      }

      ++m_current_pos;
      if (isEnd()) {
        m_last_error = Error(ErrorCode::UnfinishedString, MESSAGE::ERROR_UNFINISHED_STRING,
                             getOriginalPosition());
        break;
      }

      ch = currentChar();
    } while (ch.unicode() != QUOTE);

    ++m_current_pos;
  }
  else {
    // exemple: String(hellow word)
    while (ch.unicode() != ')') {
      string.append(ch);
      ++m_current_pos;

      if (isEnd()) {
        m_last_error = Error(ErrorCode::UnfinishedString, MESSAGE::ERROR_UNFINISHED_STRING,
                             getOriginalPosition());
        break;
      }

      ch = currentChar();
    }
  }

  return new SimpleExpression(string);
}

ExpressionPtr ExpressionBuilder::functionSTROKES(WrapperFlags &flags)
{
  // Strokes(E [,n])
  // 'n' - is optional
  // exemple: Strokes(f(x)) -> f'(x)
  // exemple: Strokes(f(x), 3) -> f'''(x)

  flags.setFlag(WrapperFlag::Powered);
  ExpressionPtr E = stringExpression(Wrapper::Power);

  int count = 1;
  if (skipComma()) {
    int next_pos = -1;
    count = getInt(m_text, m_current_pos, next_pos);
    if ( !stopBuild() )
      m_current_pos = next_pos;
  }

  return makePower(E, new StrokesExpression(count));
}

ExpressionPtr ExpressionBuilder::functionFACT(WrapperFlags &flags)
{
  // Fuct(E) -> E!
  // exemple: Fuct(x + 1) -> (x + 1)!
  // exemple: Fuct(n) -> n!
  flags.setFlag(WrapperFlag::Powered);
  ExpressionPtr result = stringExpression(Wrapper::Power);
  result->addNext(new SimpleExpression('!'));
  return result;
}

ExpressionPtr ExpressionBuilder::functionAT()
{
  // At(E1, E2)
  // exemple: At(x,1) -> x|₁
  ExpressionPtr E1 = stringExpression(Wrapper::Argument);
  ExpressionPtr E2 = skipRequiredComma() ? stringExpression(Wrapper::Expression)
                                         : makeEmptyExpression();
  return new AtValueExpression(E1, E2);
}

ExpressionPtr ExpressionBuilder::functionCAPTION(CapExpression::CapStyle style)
{
  // exemples:
  //   Line(x)  - horizontal line over x
  //   Vect(x)  - horizontal arrow over x
  //   Cap(x)   - sign '^' over x
  //   Tilda(x) - sign '~' over x
  ExpressionPtr expr = stringExpression(Wrapper::Expression);
  return makeCap(expr, style, 0);
}

ExpressionPtr ExpressionBuilder::functionPOINTS()
{
  // Points(E [,n])
  // 'n' - is optional
  ExpressionPtr expr = stringExpression(Wrapper::Expression);

  int count = 1;
  if (skipComma()) {
    int next_pos = -1;
    count = getInt(m_text, m_current_pos, next_pos);
    if ( !stopBuild() )
      m_current_pos = next_pos;
  }

  return makeCap(expr, CapExpression::CapStyle::Points, count);
}

ExpressionPtr ExpressionBuilder::functionSTAND(Qt::Alignment horizontal_alignment)
{
  // StandC(E1, E2, ...)
  // StandL(E1, E2, ...)
  // StandR(E1, E2, ...)
  ExpressionPtr expr = stringExpression(Wrapper::Expression);

  if (stopBuild())
    return expr;

  while (skipComma()) {
    expr->addNext(stringExpression(Wrapper::Expression));

    if (stopBuild())
      break;
  }

  return new StandExpression(expr, horizontal_alignment);
}

ExpressionPtr ExpressionBuilder::functionMATRIX()
{
  // Matrix(n, m, E1, E2, ...)

  int next_pos = -1;
  const int n = getInt(m_text, m_current_pos, next_pos);
  if ( stopBuild() )
    return makeEmptyExpression();

  m_current_pos = next_pos;

  if ( !skipRequiredComma() ) {
    return makeEmptyExpression();
  }

  next_pos = -1;
  const int m = getInt(m_text, m_current_pos, next_pos);
  if ( stopBuild() )
    return makeEmptyExpression();

  m_current_pos = next_pos;

  if ( !skipRequiredComma() ) {
    return makeEmptyExpression();
  }

  ExpressionPtr expr = stringExpression(Wrapper::Expression);
  while (skipComma()) {
    expr->addNext( stringExpression(Wrapper::Expression) );

    if (stopBuild())
      break;
  }

  return new MatrixExpression(n, m, expr);
}

ExpressionPtr ExpressionBuilder::functionGroupOp(GroupType type, bool needBrackets)
{
  // Summa(E1 [, E2 [,E3]])
  // Prod(E1 [, E2 [,E3]])
  // Circ(E1 [, E2 [,E3]])
  // Int(E1 [, E2 [,E3]])
  // IntM(n, E1 [, E2 [,E3]])
  // E2 - under the sign, E3 - over the sign.  E2, E3 - is optional
  int n = 0;
  if (type == GroupType::IntegralM) {
    int next_pos = -1;
    n = getInt(m_text, m_current_pos, next_pos);
    if ( stopBuild() )
      return makeEmptyExpression();

    m_current_pos = next_pos;

    if ( !skipRequiredComma() ) {
      return makeEmptyExpression();
    }
  }

  ExpressionPtr E1 = stringExpression(Wrapper::Argument);
  if (needBrackets) {
    if ( ArgumentExpression* E1_as_arg = dynamic_cast<ArgumentExpression*>(E1) )
      E1_as_arg->setForsedBrackets(true);
    else
      E1 = new RoundBracketedExpression(E1);
  }

  ExpressionPtr E2 = skipComma() ? stringExpression(Wrapper::Expression) : nullptr;
  ExpressionPtr E3 = skipComma() ? stringExpression(Wrapper::Expression) : nullptr;

  switch (type) {
  case GroupType::Summa:
    return new SummaExpression(E1, E2, E3);
  case GroupType::Product:
    return new ProductExpression(E1, E2, E3);
  case GroupType::Circulation:
    return new CirculationExpression(E1, E2, E3);
  case GroupType::Integral:
    return new IntegralExpression(E1, E2, E3);
  case GroupType::IntegralM:
    return new IntegralExpression(E1, E2, E3, n);
  }

  return E1;  // for remove warning [-Wreturn-type] only
}

ExpressionPtr ExpressionBuilder::functionCASE()
{
  // Case(E1 [, E2, ...])
  ExpressionPtr expr = stringExpression(Wrapper::Expression);

  if ( !stopBuild() ) {
    while (skipComma()) {
      expr->addNext( stringExpression(Wrapper::Expression) );

      if (stopBuild())
        break;
    } // while
  }

  return new CaseExpression(expr);
}

ExpressionPtr ExpressionBuilder::functionCOMMA()
{
  // Comma(n)
  ExpressionPtr result = new CommaExpression();

  int next_pos = -1;
  const int n = getInt(m_text, m_current_pos, next_pos);
  if ( stopBuild() )
    return result;

  m_current_pos = next_pos;

  result->addNext(new SpaceExpression(n));
  return result;
}

ExpressionPtr ExpressionBuilder::functionBRACKETS()
{
  // Brackets(S1S2, E)
  // S1 - left bracket, one of '(', '{', '[', '|' or '0'
  // S2 - right bracket, one of ')', '}', ']', '|' or '0'
  // E  - expression in brackets

  clearLastError();
  BracketedExpression::BracketStyle left_bracket = BracketedExpression::BracketStyle::None;
  ushort ch = currentChar().unicode();
  switch (ch) {
  case '(': {
      left_bracket = BracketedExpression::BracketStyle::Round;
      break;
    }
  case '[': {
      left_bracket = BracketedExpression::BracketStyle::Square;
      break;
    }
  case '{': {
      left_bracket = BracketedExpression::BracketStyle::Figure;
      break;
    }
  case '|': {
      left_bracket = BracketedExpression::BracketStyle::Module;
      break;
    }
  case '0':
    break;
  default: {
      const int original_pos = getOriginalPosition();
      m_last_error = Error(ErrorCode::ExpectedOpenBracket,
                           MESSAGE::ERROR_EXPECTED_OPEN_BRACKET.arg(original_pos), original_pos);
      return makeEmptyExpression();
      //break;
    }
  }  // switch

  ++m_current_pos;
  if (stopBuild()) {
    m_last_error = Error(ErrorCode::UnfinishedString, MESSAGE::ERROR_UNFINISHED_STRING,
                         getOriginalPosition());
    return makeEmptyExpression();
  }

  BracketedExpression::BracketStyle right_bracket = BracketedExpression::BracketStyle::None;
  ch = currentChar().unicode();
  switch (ch) {
  case ')': {
      right_bracket = BracketedExpression::BracketStyle::Round;
      break;
    }
  case ']': {
      right_bracket = BracketedExpression::BracketStyle::Square;
      break;
    }
  case '}': {
      right_bracket = BracketedExpression::BracketStyle::Figure;
      break;
    }
  case '|': {
      right_bracket = BracketedExpression::BracketStyle::Module;
      break;
    }
  case '0':
    break;
  default: {
      const int original_pos = getOriginalPosition();
      m_last_error = Error(ErrorCode::ExpectedCloseBracket,
                           MESSAGE::ERROR_EXPECTED_OPEN_BRACKET.arg(original_pos), original_pos);
      return makeEmptyExpression();
      //break;
    }
  } // switch

  ++m_current_pos;
  ExpressionPtr expr = skipRequiredComma() ? stringExpression(Wrapper::Expression) : nullptr;
  return new BracketedExpression(expr, left_bracket, right_bracket);
}

ExpressionPtr ExpressionBuilder::functionSYSTEM()
{
  // Case(E1 [, E2, ...])
  ExpressionPtr expr = stringExpression(Wrapper::Expression);

  if ( !stopBuild() ) {
    while (skipComma()) {
      expr->addNext(stringExpression(Wrapper::Expression));

      if (stopBuild())
        break;
    } // while
  }

  expr = new StandExpression(expr, Qt::AlignLeft);
  return new BracketedExpression(expr, BracketedExpression::BracketStyle::Figure,
                                 BracketedExpression::BracketStyle::None);
}

ExpressionPtr ExpressionBuilder::functionNUM()
{
  // Num(R [, n1[, n2[, n3]]])
  // R - double or integer number
  // n1 - precision
  // n2 - decimals
  // n3 - maxDegree
  // n1, n2, n3 -  is optional integer number
  // n1 = 4, n2 = 4, n3 = 2 by default
  clearLastError();

  int next_pos = -1;
  const double R = getDouble(m_text, m_current_pos, next_pos);
  if ( !stopBuild() )
    m_current_pos = next_pos;

  int n1 = 4;
  int n2 = 4;
  int n3 = 2;

  if ( !stopBuild() && skipComma()) {
    n1 = getInt(m_text, m_current_pos, next_pos);
    if ( !stopBuild() )
      m_current_pos = next_pos;
  }

  if ( !stopBuild() && skipComma()) {
    n2 = getInt(m_text, m_current_pos, next_pos);
    if ( !stopBuild() )
      m_current_pos = next_pos;
  }

  if ( !stopBuild() && skipComma()) {
    n3 = getInt(m_text, m_current_pos, next_pos);
    if ( !stopBuild() )
      m_current_pos = next_pos;
  }

  return new ExtendedNumberExpresssion(R, n1, n2, n3);
}

ExpressionPtr ExpressionBuilder::functionSYMBOL()
{
  // Symbol(n), n - Unicode code
  // exepmle: Symbol(937) -> Ω
  clearLastError();

  int next_pos = -1;
  const int n = getInt(m_text, m_current_pos, next_pos);
  if ( !stopBuild() )
    m_current_pos = next_pos;

  return new CharacterExpression(QChar(n));
}

ExpressionPtr ExpressionBuilder::functionANGLE()
{
  // Angle(Text)
  // exepmles: Angle(ABS) -> ∠ABS
  //           Angte("Text with (brackets)") -> ∠Text with (brackets)
  ExpressionPtr angle = new SignExpression(SignExpression::SignType::Angle);
  angle->addNext(functionSTRING());
  return angle;
}

//ExpressionPtr ExpressionBuilder::functionCOLOR()
//{
// TODO add function Color()
//  // COLOR(R, G, B [, a])
//  // R - reg
//  // G - green
//  // B - blue
//  // a - the alpha-channel (transparency), is optional

//  int next_pos = -1;
//  const int R = getInt(m_text, m_current_pos, next_pos);
//  if (next_pos < 0) {
//    m_last_error = Error(ErrorCode::ConvertToNumber,
//                         MESSAGE::ERROR_CONVERT_STR_TO_INT, getOriginalPosition());
//    return makeEmptyExpression();
//  }
//  m_current_pos = next_pos;

//  if ( !skipRequiredComma() ) {
//    return makeEmptyExpression();
//  }

//  next_pos = -1;
//  const int G = getInt(m_text, m_current_pos, next_pos);
//  if (next_pos < 0) {
//    m_last_error = Error(ErrorCode::ConvertToNumber,
//                         MESSAGE::ERROR_CONVERT_STR_TO_INT, getOriginalPosition());
//    return makeEmptyExpression();
//  }
//  m_current_pos = next_pos;

//  if ( !skipRequiredComma() ) {
//    return makeEmptyExpression();
//  }

//  next_pos = -1;
//  const int B = getInt(m_text, m_current_pos, next_pos);
//  if (next_pos < 0) {
//    m_last_error = Error(ErrorCode::ConvertToNumber,
//                         MESSAGE::ERROR_CONVERT_STR_TO_INT, getOriginalPosition());
//    return makeEmptyExpression();
//  }
//  m_current_pos = next_pos;

//  int alpha = 255;
//  if (skipComma()) {
//    next_pos = -1;
//    alpha = getInt(m_text, m_current_pos, next_pos);
//    if (next_pos < 0) {
//      alpha = 255;
//      m_last_error = Error(ErrorCode::ConvertToNumber,
//                           MESSAGE::ERROR_CONVERT_STR_TO_INT, getOriginalPosition());
//    }
//    else
//      m_current_pos = next_pos;
//  }

//  ExpressionPtr result = new VariableExpression('X');
//  result->setColor(QColor(R, G, B, alpha));
//  return result;
//}

} // namespace MathExpressions
