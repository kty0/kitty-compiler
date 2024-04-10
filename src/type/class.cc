/**
 ** \file type/class.cc
 ** \brief Implementation for type/class.hh.
 */

#include <ostream>

#include <misc/algorithm.hh>
#include <type/class.hh>
#include <type/visitor.hh>

namespace type
{
  Class::Class(const Class* super)
    : Type()
    , super_(super)
  {}

  void Class::accept(ConstVisitor& v) const { v(*this); }

  void Class::accept(Visitor& v) { v(*this); }

  const Type* Class::attr_type(misc::symbol key) const
  {
    // IGNORED: Some code was deleted here.
  }

  const Type* Class::meth_type(misc::symbol key) const
  {
    // IGNORED: Some code was deleted here.
  }

  // IGNORED: Some code was deleted here (Find common super class).

  // IGNORED: Some code was deleted here (Super class soundness test).

  // IGNORED: Some code was deleted here (Special implementation of "compatible_with" for Class).

  const Class& Class::object_instance()
  {
    // IGNORED: Some code was deleted here.
  }

} // namespace type
