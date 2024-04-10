/**
 ** \file ast/typable.cc
 ** \brief Implementation of ast::Typable.
 */

#include <ast/typable.hh>
#include <ast/visitor.hh>

namespace ast
{
  // FIXED: Some code was deleted here.

  const type::Type* Typable::type_get() const { return type_; }

  void Typable::type_set(const type::Type* type) { type_ = type; }

} // namespace ast
