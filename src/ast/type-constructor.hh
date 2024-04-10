/**
 ** \file ast/type-constructor.hh
 ** \brief Declaration of ast::TypeConstructor.
 */

#pragma once

#include <ast/fwd.hh>
#include <type/fwd.hh>

namespace ast
{
  /** \class ast::TypeConstructor
   ** \brief Create a new type.
   */

  class TypeConstructor
  {
    // FIXED: Some code was deleted here.
  public:
    TypeConstructor() = default;
    ~TypeConstructor();

    void created_type_set(const type::Type*);
    const type::Type* created_type_get() const;

  private:
    const type::Type* instance_;
  };
} // namespace ast
#include <ast/type-constructor.hxx>
