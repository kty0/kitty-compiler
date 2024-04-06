/**
 ** \file ast/escapable.cc
 ** \brief Implementation of ast::Escapable.
 */

#include <ast/escapable.hh>
#include <ast/visitor.hh>

namespace ast
{
  // FIXED: Some code was deleted here.
  void Escapable::set_escaped()
  {
    this->escaped = true;
    this->locked = true;
  }
  void Escapable::set_unescaped() { this->escaped = false; }
  bool Escapable::is_locked() const { return locked; }
  bool Escapable::is_escaped() const { return escaped; }
} // namespace ast
