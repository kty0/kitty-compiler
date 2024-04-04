/**
 ** \file ast/type-constructor.cc
 ** \brief Implementation of ast::TypeConstructor.
 */

#include <ast/type-constructor.hh>
#include <ast/visitor.hh>

namespace ast
{
  // FIXED: Some code was deleted here.

  TypeConstructor::~TypeConstructor() { delete instance_; }

  void TypeConstructor::created_type_set(type::Type* type) { instance_ = type; }

  type::Type* TypeConstructor::created_type_get() { return instance_; }

} // namespace ast
