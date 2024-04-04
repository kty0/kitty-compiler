/**
 ** \file ast/typable.cc
 ** \brief Implementation of ast::Typable.
 */

#include <ast/typable.hh>
#include <ast/visitor.hh>

namespace ast
{
  // FIXED: Some code was deleted here.

  type::Type* Typable::get_type() { return type_; }

  void Typable::set_type(type::Type* type) { type_ = type; }

} // namespace ast
