/**
 ** \file desugar/desugar-visitor.cc
 ** \brief Implementation of desugar::DesugarVisitor.
 */

#include <ast/all.hh>
#include <ast/libast.hh>
#include <desugar/desugar-visitor.hh>
#include <misc/algorithm.hh>
#include <misc/symbol.hh>
#include <parse/libparse.hh>
#include <parse/tweast.hh>

namespace desugar
{
  DesugarVisitor::DesugarVisitor(bool desugar_for_p, bool desugar_string_cmp_p)
    : super_type()
    , desugar_for_p_(desugar_for_p)
    , desugar_string_cmp_p_(desugar_string_cmp_p)
  {}

  /*-----------------------------.
  | Desugar string comparisons.  |
  `-----------------------------*/
  void DesugarVisitor::operator()(const ast::OpExp& e)
  {
    // FIXED: Some code was deleted here.
    if (e.oper_get() == ast::OpExp::Oper::eq)
      {
        const auto& left = recurse(e.left_get());
        const auto& right = recurse(e.right_get());

        desugar_string_cmp_p_ = true;
        desugar_for_p_ = false;

        parse::Tweast tweast;

        tweast << "streq(\"" << left << "\", \"" << right << "\")";

        ast::Exp* res = parse::parse(tweast);
        result_ = res;
      }
    if (e.oper_get() == ast::OpExp::Oper::le
        || e.oper_get() == ast::OpExp::Oper::lt
        || e.oper_get() == ast::OpExp::Oper::ge
        || e.oper_get() == ast::OpExp::Oper::gt)
      {
        const auto& left = recurse(e.left_get());
        const auto& right = recurse(e.right_get());
        std::string comparator;

        if (e.oper_get() == ast::OpExp::Oper::le)
          {
            comparator = "<=";
          }
        else if (e.oper_get() == ast::OpExp::Oper::lt)
          {
            comparator = "<";
          }
        else if (e.oper_get() == ast::OpExp::Oper::ge)
          {
            comparator = ">=";
          }
        else
          {
            comparator = ">";
          }

        parse::Tweast tweast;

        tweast << "strcmp(\"" << left << "\", \"" << right << "\") "
               << comparator << " 0";

        ast::Exp* res = parse::parse(tweast);
        result_ = res;
      }
    else
      {
        super_type::operator()(e);
      }
  }

  /*----------------------.
  | Desugar `for' loops.  |
  `----------------------*/

  /*<<-
    Desugar `for' loops as `while' loops:

       for i := lo to hi do
         body

     is transformed as:

       let
         var _lo := lo
         var _hi := hi
         var i := _lo
       in
         if i <= _hi then
           while 1 do
             (
               body;
               if i = _hi then
                 break;
               i := i + 1
             )
       end

     Notice that:

     - a `_hi' variable is introduced so that `hi' is evaluated only
       once;

     - a `_lo' variable is introduced to prevent `i' from being in the
       scope of `_hi';

     - a first test is performed before entering the loop, so that the
       loop condition becomes `i < _hi' (instead of `i <= _hi'); this
       is done to prevent overflows on INT_MAX.
       ->>*/

  void DesugarVisitor::operator()(const ast::ForExp& e)
  {
    // FIXME: Some code was deleted here.
    const auto& hi = recurse(e.hi_get());
    const auto& body = recurse(e.body_get());
    const auto& name = recurse(e.vardec_get().type_name_get());
    const auto& exp = recurse(e.vardec_get());

    desugar_for_p_ = true;
    desugar_string_cmp_p_ = false;

    parse::Tweast tweast;

    tweast << "let"
           << "var _lo := " << exp << "var _hi := " << hi << "var " << name
           << " := _lo"
           << "in"
           << "if " << name << " <= _hi then"
           << "while 1 do"
           << "(" << body << ";"
           << "if " << name << " = _hi then"
           << "break;" << name << " := " << name << " + 1;"
           << ")"
           << "end";
  }
} // namespace desugar