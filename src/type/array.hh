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
    ~Array() = default;
    Array(const Type& type);

    const Type& type_get() const;

    /// Accept a const visitor \a v.
    void accept(ConstVisitor& v) const override;
    /// Accept a non-const visitor \a v.
    void accept(Visitor& v) override;

  private:
    const Type& type_;
  };

} // namespace type

#include <type/array.hxx>
