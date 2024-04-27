/**
 ** \file escapes/escapes-visitor.cc
 ** \brief Implementation for escapes/escapes-visitor.hh.
 */

#include <ast/all.hh>
#include <escapes/escapes-visitor.hh>
#include <misc/contract.hh>

namespace escapes
{
  // FIXED: Some code was deleted here.

  void EscapesVisitor::operator()(ast::FunctionDec& e)
  {
    auto previous_function = current_function_;
    current_function_ = &e;
    super_type::operator()(e);
    current_function_ = previous_function;
  }

  void EscapesVisitor::operator()(ast::VarDec& e)
  {
    e.unescaped_set();
    e.def_site_set(current_function_);
    super_type::operator()(e);
  }

  void EscapesVisitor::operator()(ast::SimpleVar& e)
  {
    ast::VarDec* var = e.def_get();
    if (var->def_site_get() != current_function_)
      {
        var->escaped_set();
      }
  }
} // namespace escapes
