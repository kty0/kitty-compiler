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

  void TypeConstructor::created_type_set(const type::Type* type)
  {
    instance_ = type;
  }

  const type::Type* TypeConstructor::created_type_get() const
  {
    return instance_;
  }

} // namespace ast
