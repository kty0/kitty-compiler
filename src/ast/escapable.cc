/**
 ** \file ast/escapable.cc
 ** \brief Implementation of ast::Escapable.
 */

#include <ast/escapable.hh>
#include <ast/visitor.hh>

namespace ast
{
  // FIXED: Some code was deleted here.
  void Escapable::escaped_set() { this->escaped_ = true; }
  void Escapable::unescaped_set() { this->escaped_ = false; }
  bool Escapable::is_escaped() const { return this->escaped_; }

  ast::FunctionDec* Escapable::def_site_get() const { return this->def_site_; }

  void Escapable::def_site_set(ast::FunctionDec* e) { this->def_site_ = e; }

} // namespace ast
