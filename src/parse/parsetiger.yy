// -*- C++ -*-
%require "3.8"
%language "c++"
// Set the namespace name to `parse', instead of `yy'.
%define api.prefix {parse}
%define api.namespace {parse}
%define api.parser.class {parser}
%define api.value.type variant
%define api.token.constructor

// We use a GLR parser because it is able to handle Shift-Reduce and
// Reduce-Reduce conflicts by forking and testing each way at runtime. GLR
// permits, by allowing few conflicts, more readable and maintainable grammars.
%glr-parser
%skeleton "glr2.cc"

// In TC, we expect the GLR to resolve one Shift-Reduce and zero Reduce-Reduce
// conflict at runtime. Use %expect and %expect-rr to tell Bison about it.
  // FIXED: Some code was deleted here (Other directives).
%expect 1
%expect-rr 0

%define parse.error verbose
%defines
%debug
// Prefix all the tokens with TOK_ to avoid colisions.
%define api.token.prefix {TOK_}

/* We use pointers to store the filename in the locations.  This saves
   space (pointers), time (no deep copy), but leaves the problem of
   deallocation.  This would be a perfect job for a misc::symbol
   object (passed by reference), however Bison locations require the
   filename to be passed as a pointer, thus forcing us to handle the
   allocation and deallocation of this object.

   Nevertheless, all is not lost: we can still use a misc::symbol
   object to allocate a flyweight (constant) string in the pool of
   symbols, extract it from the misc::symbol object, and use it to
   initialize the location.  The allocated data will be freed at the
   end of the program (see the documentation of misc::symbol and
   misc::unique).  */
%define api.filename.type {const std::string}
%locations

/*---------------------.
| Support for tokens.  |
`---------------------*/
%code requires
{
#include <string>
#include <misc/algorithm.hh>
#include <misc/separator.hh>
#include <misc/symbol.hh>
#include <parse/fwd.hh>

  // Pre-declare parse::parse to allow a ``reentrant'' parsing within
  // the parser.
  namespace parse
  {
    ast_type parse(Tweast& input);
  }
}

// The parsing context.
%param { ::parse::TigerDriver& td }
%parse-param { ::parse::Lexer& lexer }

%printer { yyo << $$; } <int> <std::string> <misc::symbol>;

%token <std::string>    STRING "string"
%token <misc::symbol>   ID     "identifier"
%token <int>            INT    "integer"


/*-----------------------------------------.
| Code output in the implementation file.  |
`-----------------------------------------*/

%code
{
# include <parse/tweast.hh>
# include <misc/separator.hh>
# include <misc/symbol.hh>

  namespace
  {

    /// Get the metavar from the specified map.
    template <typename T>
    T*
    metavar(parse::TigerDriver& td, unsigned key)
    {
      parse::Tweast* input = td.input_;
      return input->template take<T>(key);
    }

  }
}

%code
{
  #include <parse/scantiger.hh>  // header file generated with reflex --header-file
  #undef yylex
  #define yylex lexer.lex  // Within bison's parse() we should invoke lexer.lex(), not the global lex()
}

// Definition of the tokens, and their pretty-printing.
%token AND          "&"
       ARRAY        "array"
       ASSIGN       ":="
       BREAK        "break"
       CAST         "_cast"
       CLASS        "class"
       COLON        ":"
       COMMA        ","
       DIVIDE       "/"
       DO           "do"
       DOT          "."
       ELSE         "else"
       END          "end"
       EQ           "="
       EXTENDS      "extends"
       FOR          "for"
       FUNCTION     "function"
       GE           ">="
       GT           ">"
       IF           "if"
       IMPORT       "import"
       IN           "in"
       LBRACE       "{"
       LBRACK       "["
       LE           "<="
       LET          "let"
       LPAREN       "("
       LT           "<"
       MINUS        "-"
       METHOD       "method"
       NE           "<>"
       NEW          "new"
       NIL          "nil"
       OF           "of"
       OR           "|"
       PLUS         "+"
       PRIMITIVE    "primitive"
       RBRACE       "}"
       RBRACK       "]"
       RPAREN       ")"
       SEMI         ";"
       THEN         "then"
       TIMES        "*"
       TO           "to"
       TYPE         "type"
       VAR          "var"
       WHILE        "while"
       EOF 0        "end of file"


  // FIXED: Some code was deleted here (Priorities/associativities).
%precedence "then"
%precedence "else"
%precedence "do" "of"
%precedence ":="
%left "<>" "=" ">"
%left "+" "-"
%left "*" "/"

// Solving conflicts on:
// let type foo = bar
//     type baz = bat
// which can be understood as a list of two TypeChunk containing
// a unique TypeDec each, or a single TypeChunk containing two TypeDec.
// We want the latter.
%precedence CHUNKS
%precedence TYPE
// FIXED: Some code was deleted here (Other declarations).
%precedence UNARY

%start program

%%
program:
  /* Parsing a source program.  */
  exp
   
| /* Parsing an imported file.  */
  chunks
   
;

  // FIXED: Some code was deleted here (More rules).

exps:
    %empty
    | exp sub_exps

sub_exps:
    %empty
    | ";" exp

exp:
  /* Literals */
   NIL
  | INT
  | STRING

  /* Array and record creations */
  | ID "[" exp "]" "of" exp
  | ID "{" empty_type "}"
   
  /* Variables, field, elements of an array */
  | lvalue

  /* Function call */
  | ID "(" extra_exp_1 ")"

  /* Operations*/
  | "-" exp %prec UNARY
  /* Operations with op */
  | exp "+" exp
  | exp "-" exp
  | exp "*" exp
  | exp "/" exp
  | exp "=" exp
  | exp "<>" exp
  | exp ">" exp
  | "(" exps ")"

  /* Assignment */
  | lvalue ":=" exp

  /* Control structures */
  | "if" exp "then" exp
  | "if" exp "then" exp "else" exp
  | "while" exp "do" exp
  | "for" ID ":=" exp "to" exp "do" exp
  | "break"
  | "let" chunks "in" exps "end"
  ;

empty_type:
    %empty
    | ID "=" exp empty_type_2
    ;

empty_type_2:
    %empty
    | "," ID "=" exp empty_type_2
    ;

extra_exp_1:
    %empty
    | exp extra_exp_2
    ;

extra_exp_2:
    %empty
    | "," exp
    ;


lvalue:
    ID
    /* record field access */
    | lvalue "." ID
    /* array sub */
    | lvalue "[" exp "]"
    ;

/*---------------.
| Declarations.  |
`---------------*/

%token CHUNKS "_chunks";
chunks:
  /* Chunks are contiguous series of declarations of the same type
     (TypeDec, FunctionDec...) to which we allow certain specfic behavior like
     self referencing.
     They are held by a ChunkList, that can be empty, like in this case:
        let
        in
            ..
        end
     which is why we end the recursion with a %empty. */
  %empty                  
| tychunk   chunks        
  // FIXED: Some code was deleted here (More rules).
| funchunk chunks
| varchunk
;

/*--------------------.
| Type Declarations.  |
`--------------------*/

tychunk:
  /* Use `%prec CHUNKS' to do context-dependent precedence and resolve a
     shift-reduce conflict. */
  tydec %prec CHUNKS  
| tydec tychunk       
;

funchunk:
  fundec CHUNKS
| fundec funchunk
;

fundec:
    "function" ID "(" tyfields ")" fundecbis "=" exp
| "primitive" ID "(" tyfields ")" fundecbis
;

fundecbis:
    %empty
    | ":" ID
    ;

varchunk:
    "var" ID fundecbis ":=" exp
;

tydec:
  "type" ID "=" ty 
;

%token NAMETY "_namety";

ty:
  ID
| "{" tyfields "}"     
| "array" "of" ID
| "array" "of" NAMETY "(" INT ")"
;

tyfields:
        %empty               
| tyfields.1           
;

tyfields.1:
  tyfields.1 "," tyfield 
| tyfield                
;

tyfield:
       ID ":" ID
| ID ":" NAMETY "(" INT ")"
;

/*
typeid:
      ID
  COMMENT This is a metavariable. It it used internally by TWEASTs to retrieve
     already parsed nodes when given an input to parse.
| NAMETY "(" INT ")"
;
*/

%%

void
parse::parser::error(const location_type& l, const std::string& m)
     {
  // FIXED: Some code was deleted here.
    std::cerr << m << " at " << l;
}
