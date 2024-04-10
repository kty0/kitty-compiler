/**
 ** \file type/array.hh
 ** \brief The class Array.
 */
#pragma once

#include <type/fwd.hh>
#include <type/type.hh>
#include <type/visitor.hh>

namespace type
{
  /// Array types.
  class Array : public Type
  {
    // FIXED: Some code was deleted here.
  public:
    virtual ~Array() = default;
    Array(const Type& type);

    const Type& type_get() const;

  private:
    const Type& type_;
  };

} // namespace type

#include <type/array.hxx>
