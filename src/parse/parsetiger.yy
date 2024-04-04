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


/*--------------------------------.
| Support for the non-terminals.  |
`--------------------------------*/

%code requires
{
# include <ast/fwd.hh>
// Provide the declarations of the following classes for the
// %destructor clauses below to work properly.
# include <ast/exp.hh>
# include <ast/var.hh>
# include <ast/ty.hh>
# include <ast/name-ty.hh>
# include <ast/field.hh>
# include <ast/field-init.hh>
# include <ast/function-dec.hh>
# include <ast/type-dec.hh>
# include <ast/var-dec.hh>
# include <ast/chunk.hh>
# include <ast/chunk-list.hh>
}

  // FIXED: Some code was deleted here (Printers and destructors).

/*-----------------------------------------.
| Code output in the implementation file.  |
`-----------------------------------------*/

%code
{
# include <parse/tweast.hh>
# include <misc/separator.hh>
# include <misc/symbol.hh>
# include <ast/all.hh>
# include <ast/libast.hh>
# include <parse/tiger-factory.hh>

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
%left "|"
%left "&"
%nonassoc ">=" "<=" "=" "<>" "<" ">"
%left "+" "-"
%left "*" "/"
%type <ast::Exp*>             exp
%type <ast::exps_type*>       exps sub_exps extra_exp_1 extra_exp_2
%type <ast::ChunkList*>       chunks
%type <ast::Var*>             lvalue

%type <ast::fieldinits_type*> empty_type empty_type_2


%type <ast::TypeChunk*>       tychunk
%type <ast::TypeDec*>         tydec
%type <ast::FunctionChunk*>   funchunk
%type <ast::FunctionDec*>     fundec
%type <ast::VarChunk*>        varchunk tyfields_fun.1 tyfields_fun
%type <ast::VarDec*>          vardec tyfield_fun
%type <ast::NameTy*>          typeid fundecbis
%type <ast::Ty*>              ty

%type <ast::Field*>           tyfield
%type <ast::fields_type*>     tyfields tyfields.1

  // FIXED: Some code was deleted here (More %types).

  // FIXED: Some code was deleted here (Priorities/associativities).

// Solving conflicts on:
// let type foo = bar
//     type baz = bat
// which can be understood as a list of two TypeChunk containing
// a unique TypeDec each, or a single TypeChunk containing two TypeDec.
// We want the latter.
%precedence CHUNKS
%precedence "primitive" "function" "class"
%precedence TYPE
// FIXED: Some code was deleted here (Other declarations).
%precedence UNARY
  // FIXED: Some code was deleted here (Other declarations).

%start program

%%
%token LVALUE  "_lvalue";
%token EXP  "_exp";
program:
  /* Parsing a source program.  */
  exp { td.ast_ = $1; }
| /* Parsing an imported file.  */
  chunks { td.ast_ = $1; }
;

  // FIXED: Some code was deleted here (More rules).

exps:
    %empty { $$ = make_exps_type();}
    | exp sub_exps {$$ = $2, $$->insert($$->begin(), $1);}
    ;

sub_exps:
    %empty {$$ = make_exps_type();}
    | ";" exp sub_exps {$$ = $3, $$->insert($$->begin(), $2);}
    ;

exp:
  /* Literals */
   NIL { $$ = make_NilExp(@$); }
  | INT { $$ = make_IntExp(@$, $1); }
  // FIXED: Some code was deleted here (More rules).
  | STRING { $$ = make_StringExp(@$, $1); }

  /* Array and record creations */
  | ID "[" exp "]" "of" exp  { $$ = make_ArrayExp(@$,make_NameTy(@1,$1),$3,$6);}
  // add NAMETY "(" INT ")" exp.... if need
  | typeid "{" empty_type "}" { $$ = make_RecordExp(@$,$1,$3);}

  /* Variables, field, elements of an array */
  | lvalue { $$ = $1;}

  /* Function call */
  | ID "(" extra_exp_1 ")" { $$ = make_CallExp(@$, $1, $3); }

  /* Operations*/
  | "-" exp %prec UNARY { $$ = parse::parse(parse::Tweast() << "0 -" << $2); }
  | exp "&" exp   { $$ = parse::parse(parse::Tweast() << "if " << $1 << " then " << $3 << " <> 0 else 0"); }
  | exp "|" exp   { $$ = parse::parse(parse::Tweast() << "if " << $1 << " then " << " 1 else " << $3 << " <> 0"); }

  /* Operations with op */
  | exp "+" exp { $$ = make_OpExp(@$, $1, ast::OpExp::Oper::add, $3); }
  | exp "-" exp { $$ = make_OpExp(@$, $1, ast::OpExp::Oper::sub, $3); }
  | exp "*" exp { $$ = make_OpExp(@$, $1, ast::OpExp::Oper::mul, $3); }
  | exp "/" exp { $$ = make_OpExp(@$, $1, ast::OpExp::Oper::div, $3); }
  | exp "=" exp { $$ = make_OpExp(@$, $1, ast::OpExp::Oper::eq, $3); }
  | exp "<>" exp { $$ = make_OpExp(@$, $1, ast::OpExp::Oper::ne, $3); }
  | exp ">" exp { $$ = make_OpExp(@$, $1, ast::OpExp::Oper::gt, $3); }
  | exp "<" exp { $$ = make_OpExp(@$, $1, ast::OpExp::Oper::lt, $3); }
  | exp ">=" exp { $$ = make_OpExp(@$, $1, ast::OpExp::Oper::ge, $3); }
  | exp "<=" exp { $$ = make_OpExp(@$, $1, ast::OpExp::Oper::le, $3); }
  | "(" exps ")" { $$ = make_SeqExp(@$, $2);}

  /* Assignment */
  | lvalue ":=" exp { $$ = make_AssignExp(@$,$1,$3); }

  /* Control structures */
  | "if" exp "then" exp { $$ = make_IfExp(@$, $2, $4); }
  | "if" exp "then" exp "else" exp { $$ = make_IfExp(@$, $2, $4, $6); }
  | "while" exp "do" exp { $$ = make_WhileExp(@$, $2, $4); }
  | "for" ID ":=" exp "to" exp "do" exp  { $$ = make_ForExp(@$, make_VarDec(@$, $2, nullptr,$4), $6, $8); }
  | "break" { $$ = make_BreakExp(@$); }
  | "let" chunks "in" exps "end" { $$ =  make_LetExp(@$, $2, make_SeqExp(@4,$4)); }

  /* cast of an expression to a given type*/
  | CAST "(" exp "," ty ")" { $$ = make_CastExp(@$, $3, $5); }
  /* an expression metavariable */
  | EXP "(" INT ")" { $$ = metavar<ast::Exp>(td, $3); }

  /* Object creation */
  |"new" typeid

  /* Method call */
  | lvalue "." ID "(" method_rule ")"
  ;

empty_type:
    %empty {$$ = make_fieldinits_type();}
    | ID "=" exp empty_type_2 { $$ = $4; $$->insert($$->begin(), make_FieldInit(@$,$1,$3));}
    ;

empty_type_2:
    %empty {$$ = make_fieldinits_type();}
    | "," ID "=" exp empty_type_2 { $$ = $5; $$->insert($$->begin(), make_FieldInit(@$,$2,$4));}
    ;

extra_exp_1:
    %empty { $$ = make_exps_type(); }
    | exp extra_exp_2 { $$ = $2; $$->insert($$->begin(), $1); }
    ;

extra_exp_2:
    %empty { $$ = make_exps_type(); }
    | "," exp extra_exp_2 { $$ = $3; $$->insert($$->begin(), $2); }
    ;

lvalue:
    ID { $$ = make_SimpleVar(@$,$1); }
    /* record field access */
    | lvalue "." ID { $$ = make_FieldVar(@$, $1, $3); }
    /* array sub */
    | lvalue "[" exp "]" { $$ = make_SubscriptVar(@$, $1, $3); }
    /* a l-value metavariable */
    | LVALUE "(" INT ")" { $$ = metavar<ast::Var>(td, $3); }
    ;

method_rule:
    %empty
    | exp method_rule_2
    ;

method_rule_2:
    %empty
    | "," exp method_rule_2

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
  %empty                  { $$ = make_ChunkList(@$); }
| tychunk   chunks        { $$ = $2; $$->push_front($1); }
  // FIXED: Some code was deleted here (More rules).
| funchunk chunks         { $$ = $2; $$->push_front($1); }
| varchunk chunks         { $$ = $2; $$->push_front($1); }
| "import" STRING         { $$ = td.parse_import($2,@$); }
| CHUNKS "(" INT ")" chunks  { $$ = metavar<ast::ChunkList>(td, $3); $$->splice_back(*$5) ; }
;
/*--------------------.
| Type Declarations.  |
`--------------------*/

tychunk:
  /* Use `%prec CHUNKS' to do context-dependent precedence and resolve a
     shift-reduce conflict. */
  tydec %prec CHUNKS  { $$ = make_TypeChunk(@1); $$->push_front(*$1); }
| tydec tychunk       { $$ = $2; $$->push_front(*$1); }
;

funchunk:
  fundec %prec CHUNKS { $$ = make_FunctionChunk(@1); $$->push_front(*$1); }
| fundec funchunk     { $$ = $2; $$->push_front(*$1); }
;

fundec:
  "function" ID "(" tyfields_fun ")" "=" exp              { $$ = make_FunctionDec(@$, $2, $4, nullptr, $7);}
| "function" ID "(" tyfields_fun ")" ":" typeid "=" exp   { $$ = make_FunctionDec(@$, $2, $4, $7, $9);}
| "primitive" ID "(" tyfields_fun ")"                     { $$ = make_FunctionDec(@$, $2, $4, nullptr, nullptr);}
| "primitive" ID "(" tyfields_fun ")" ":" typeid          { $$ = make_FunctionDec(@$, $2, $4, $7, nullptr);}
;

tyfields_fun:
  %empty               { $$ = make_VarChunk(@$); }
| tyfields_fun.1       { $$ = $1; }
;

tyfields_fun.1:
  tyfields_fun.1 "," tyfield_fun { $$ = $1; $$->emplace_back(*$3); }
| tyfield_fun                { $$ = make_VarChunk(@1); $$->emplace_back(*$1); }
;

tyfield_fun:
  ID ":" typeid     { $$ = make_VarDec(@$, $1, $3, nullptr); }
;

varchunk:
    vardec %prec CHUNKS { $$ = make_VarChunk(@1); $$->push_front(*$1); };
;

vardec:
    "var" ID fundecbis ":=" exp { $$ = make_VarDec(@$, $2, $3, $5); }
    ;

fundecbis:
    %empty {$$ = nullptr;}
    | ":" typeid { $$ = $2;}
    ;

tydec:
  "type" ID "=" ty { $$ = make_TypeDec(@$, $2, $4); }
  /* Class definition (alternative form) */
  | "class" ID extends_rule "{" classfields "}"
  ;

ty:
    ID                     { $$ = make_NameTy(@$, $1); }
    | "{" tyfields "}"     { $$ = make_RecordTy(@$, $2); }
    | "array" "of" typeid  { $$ = make_ArrayTy(@$, $3); }
    /* Class definition (canonical form) */
    | "class" extends_rule "{" classfields "}"
    ;

extends_rule:
    %empty
    | "extends" typeid
    ;

/* Class field */
classfields:
    %empty
    /* Attribute declaration (varchunk) */
    |varchunk classfields
    /* Method declaration (methchunk) */
    | "method" ID "(" tyfields ")" "=" exp
    | "method" ID "(" tyfields ")" ":" typeid"=" exp
    ;

tyfields:
  %empty               { $$ = make_fields_type(); }
| tyfields.1           { $$ = $1; }
;

tyfields.1:
  tyfields.1 "," tyfield { $$ = $1; $$->emplace_back($3); }
| tyfield                { $$ = make_fields_type($1); }
;

tyfield:
  ID ":" typeid     { $$ = make_Field(@$, $1, $3); }
;

%token NAMETY "_namety";
typeid:
  ID                    { $$ = make_NameTy(@$, $1); }
  /* This is a metavariable. It it used internally by TWEASTs to retrieve
     already parsed nodes when given an input to parse. */
| NAMETY "(" INT ")"    { $$ = metavar<ast::NameTy>(td, $3); }
;

%%

void parse::parser::error(const location_type& l, const std::string& m)
{
  // FIXED: Some code was deleted here.
  /*
   * This function is called automatically when an error occurs.
   * - l is the location object of tiger driver which contains the location of
   *    the error in the file
   * - m is the automatically generated error message
   *
   * error_ from tiger driver is used as a stream to handle errors
   */
  td.error_ << misc::error::error_type::parse << l << ": " << m << "\n";
}
