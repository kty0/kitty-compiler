/**
 ** \file type/named.cc
 ** \brief Implementation for type/named.hh.
 */

#include <memory>
#include <set>

#include <type/named.hh>
#include <type/visitor.hh>

namespace type
{
  Named::Named(misc::symbol name)
    : name_(name)
    , type_(nullptr)
  {}

  Named::Named(misc::symbol name, const Type* type)
    : name_(name)
    , type_(type)
  {}

  // Inherited functions
  void Named::accept(ConstVisitor& v) const
  {
    // FIXED: Some code was deleted here.
    v(*this);
  }

  void Named::accept(Visitor& v)
  {
    // FIXED: Some code was deleted here.
    v(*this);
  }

  bool Named::sound() const
  {
    // FIXED: Some code was deleted here (Sound).
    for (auto elt = this; elt != nullptr;
         elt = dynamic_cast<const Named*>(elt->type_))
      {
        if (elt == this)
          {
            return false;
          }
      }
    return true;
  }

  bool Named::compatible_with(const Type& other) const
  {
    // FIXED: Some code was deleted here (Special implementation of "compatible_with" for Named).
    return type_->actual() == other.actual();
  }

} // namespace type
