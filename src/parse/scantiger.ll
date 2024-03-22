    /* -*- C++ -*- */
// %option defines the parameters with which the reflex will be launched
%option noyywrap
// To enable compatibility with bison
%option bison-complete
%option bison-cc-parser=parser
%option bison_cc_namespace=parse
%option bison-locations

%option lex=lex
// Add a param of function lex() generate in Lexer class
%option params="::parse::TigerDriver& td"
%option namespace=parse
// Name of the class generate by reflex
%option lexer=Lexer

%top{

#define YY_EXTERN_C extern "C" // For linkage rule

#include <cerrno>
#include <climits>
#include <regex>
#include <string>

#include <boost/lexical_cast.hpp>

#include <misc/contract.hh>
  // Using misc::escape is very useful to quote non printable characters.
  // For instance
  //
  //    std::cerr << misc::escape('\n') << '\n';
  //
  // reports about `\n' instead of an actual new-line character.
#include <misc/escape.hh>
#include <misc/symbol.hh>
#include <parse/parsetiger.hh>
#include <parse/tiger-driver.hh>

  // FIXED: Some code was deleted here (Define YY_USER_ACTION to update locations).
#define YY_USER_ACTION              \
  td.location_.columns(yyleng);

#define TOKEN(Type)                             \
    parser::make_ ## Type(td.location_)

#define TOKEN_VAL(Type, Value)                  \
    parser::make_ ## Type(Value, td.location_)

#define CHECK_EXTENSION()                              \
  do {                                                  \
    if (!td.enable_extensions_p_)                       \
      td.error_ << misc::error::error_type::scan        \
      << td.location_                         \
      << ": invalid identifier: `"            \
      << misc::escape(text()) << "'\n";       \
  } while (false)

#define ERROR(m)                                \
  td.error_ << misc::error::error_type::scan    \
  << td.location_                               \
  << ": lexical error, " << m << "\n"          \

%}

%x SC_COMMENT SC_STRING

    /* Abbreviations.  */
int             [0-9]+
    /* FIXED: Some code was deleted here. */
space           [ \t]
endofline       (\n\r)|(\r\n)|(\r)|(\n)
id              ([a-zA-Z][a-zA-Z0-9_]*)|("_main")

%class {
  // FIXED: Some code was deleted here (Local variables).
  std::string grown_string{};
  int nested = 0;
}

%%
/* The rules.  */
{int} { int val = 0;
  // FIXING: Some code was deleted here (Decode, and check the value).
  val = strtol(text(), 0, 10);
  return TOKEN_VAL(INT, val); }

/* FIXED: Some code was deleted here. */

"array" { return TOKEN(ARRAY); }
"if" { return TOKEN(IF); }
"then" { return TOKEN(THEN); }
"else" { return TOKEN(ELSE); }
"while" { return TOKEN(WHILE); }
"for" { return TOKEN(FOR); }
"to" { return TOKEN(TO); }
"do" { return TOKEN(DO); }
"let" { return TOKEN(LET); }
"in" { return TOKEN(IN); }
"end" { return TOKEN(END); }
"of" { return TOKEN(OF); }
"break" { return TOKEN(BREAK); }
"nil" { return TOKEN(NIL); }
"function" { return TOKEN(FUNCTION); }
"var" { return TOKEN(VAR); }
"type" { return TOKEN(TYPE); }
"import" { return TOKEN(IMPORT); }
"primitive" { return TOKEN(PRIMITIVE); }

"_chunks" { return TOKEN(CHUNKS); }
"_exp" { return TOKEN(EXP); }
"_lvalue" { return TOKEN(LVALUE); }
"_namety" { return TOKEN(NAMETY); }

"_cast" { return TOKEN(CAST); }

"class" { if (td.enable_object_extensions_p_) {
    return TOKEN(CLASS);
  } else {
      ERROR("invalid identifier " << text());
  } }
"extends" { if (td.enable_object_extensions_p_) {
    return TOKEN(EXTENDS);
  } else {
      ERROR("invalid identifier " << text());
  } }
"method" { if (td.enable_object_extensions_p_) {
    return TOKEN(METHOD);
  } else {
      ERROR("invalid identifier " << text());
  } }
"new" { if (td.enable_object_extensions_p_) {
    return TOKEN(NEW);
  } else {
      ERROR("invalid identifier " << text());
  } }

"," { return TOKEN(COMMA); }
":" { return TOKEN(COLON); }
";" { return TOKEN(SEMI); }
"(" { return TOKEN(LPAREN); }
")" { return TOKEN(RPAREN); }
"[" { return TOKEN(LBRACK); }
"]" { return TOKEN(RBRACK); }
"{" { return TOKEN(LBRACE); }
"}" { return TOKEN(RBRACE); }
"." { return TOKEN(DOT); }
"+" { return TOKEN(PLUS); }
"-" { return TOKEN(MINUS); }
"*" { return TOKEN(TIMES); }
"/" { return TOKEN(DIVIDE); }
"=" { return TOKEN(EQ); }
"<>" { return TOKEN(NE); }
"<" { return TOKEN(LT); }
"<=" { return TOKEN(LE); }
">" { return TOKEN(GT); }
">=" { return TOKEN(GE); }
"&" { return TOKEN(AND); }
"|" { return TOKEN(OR); }
":=" { return TOKEN(ASSIGN); }

{id} { return TOKEN_VAL(ID, text()); }


"\""        { grown_string.clear(); start(SC_STRING); } /* start of a string */
"/*"        { start(SC_COMMENT); nested = 1;} /* start of a comment */

{space}     {}

{endofline}+ { td.location_.lines(size());
    td.location_.step(); }

.           { ERROR("unexpected " << text()); } /* everything else is garbage */

<SC_COMMENT> {
"*/" { nested--;
  if (nested == 0) {
    start(INITIAL);
  } }

"/*"        { nested++; }

<<EOF>> { ERROR("expected */ got EOF"); start(INITIAL); }

.   {}
}

<SC_STRING> {
"\"" { start(INITIAL);
  return TOKEN_VAL(STRING, grown_string); }

\\a { grown_string += '\a'; }
\\b { grown_string += '\b'; }
\\f { grown_string += '\f'; }
\\n { grown_string += '\n'; }
\\r { grown_string += '\r'; }
\\t { grown_string += '\t'; }
\\v { grown_string += '\v'; }

\\[0-9]{3} { grown_string += strtol(text() + 1, 0, 8);
  if (errno == ERANGE) {
    ERROR("invalid octal num " << text());
  } }

\\ { grown_string += '\\'; }

\\x[0-9a-fA-F]{2} { grown_string += strtol(text() + 2, 0, 16);
  if (errno == ERANGE) {
    ERROR("invalid hexa num " << text());
  } }

\\. { ERROR("invalid escaped character " << text()); }

<<EOF>> { ERROR("expected \" got EOF"); start(INITIAL); }

. { grown_string += text(); }
}

<<EOF>> { return TOKEN(EOF); }

%%


