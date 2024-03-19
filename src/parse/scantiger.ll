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
#define YY_USER_ACTION td.location_.columns(yyleng);

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

#define ERROR() td.error_ << misc::error::error_type::scan        \
        << td.location_                         \
        << ": error D:\n"

%}

%x SC_COMMENT SC_STRING

    /* Abbreviations.  */
int             [0-9]+
    /* FIXING: Some code was deleted here. */
space           [ \n\r]+
id              [a-z][a-z0-9]*

%class{
        // FIXING: Some code was deleted here (Local variables).
        std::string grown_string{};
        int nested = 0;
}

%%
/* The rules.  */
{int}         {
    int val = 0;
    // FIXING: Some code was deleted here (Decode, and check the value).
    val = strtol(text(), 0, 10);
    return TOKEN_VAL(INT, val);
              }

/* FIXING: Some code was deleted here. */

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

.           { std::cerr << "unexpected oh no" << text(); ERROR(); } /* everything else is garbage */

<SC_COMMENT> {
"*/" {
        nested--;
        if (nested == 0)
        {
            start(INITIAL);
        }
    }

"/*"        { nested++; }

<<EOF>> { std::cerr << "expected */ got EOF\n"; ERROR(); start(INITIAL); }

.   {}
}

<SC_STRING> {
"\""    {
        start(INITIAL);
        return TOKEN_VAL(STRING, grown_string);
    }

\\x[0-9a-fA-F]{2}  { grown_string += strtol(text() + 2, 0, 16); }

<<EOF>> { std::cerr << "expected \" got EOF\n"; ERROR(); start(INITIAL); }

. { std::cout << "appending " << text() << "\n"; grown_string += text(); }
}

<<EOF>> { return TOKEN(EOF); }

%%


