/**
 ** \file type/record.cc
 ** \brief Implementation for type/record.hh.
 */

#include <ostream>

#include <type/builtin-types.hh>
#include <type/nil.hh>
#include <type/record.hh>
#include <type/visitor.hh>

namespace type
{
  void Record::accept(ConstVisitor& v) const { v(*this); }

  void Record::accept(Visitor& v) { v(*this); }

  // FIXED: Some code was deleted here (Field manipulators).
  const Type* Record::field_type(misc::symbol key) const
  {
    int i = field_index(key);
    if (i == -1)
      {
        return nullptr;
      }
    return &fields_[i].type_get();
  }

  int Record::field_index(misc::symbol key) const
  {
    for (auto i = 0; i != fields_.size(); i++)
      {
        if (fields_[i].name_get() == key)
          {
            return i;
          }
      }
    return -1;
  }

  // FIXED: Some code was deleted here (Special implementation of "compatible_with" for Record).

  bool Record::compatible_with(const Type& other) const
  {
    if (dynamic_cast<const Nil*>(&other.actual()) || *this == other)
      {
        return true;
      }
    return false;
  }
} // namespace type
