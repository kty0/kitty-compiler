/**
 ** \file type/builtin-types.hh
 ** \brief The classes Int, String, Void.
 */
#pragma once

#include <misc/singleton.hh>
#include <type/fwd.hh>
#include <type/type.hh>

namespace type
{
  // FIXED: Some code was deleted here (Other types : Int, String, Void).
  class Int
    : public misc::Singleton<Int>
    , public Type
  {
  public:
    void accept(Visitor& v);
    void accept(ConstVisitor& v) const;
  };

  class String
    : public misc::Singleton<String>
    , public Type
  {
  public:
    void accept(Visitor& v);
    void accept(ConstVisitor& v) const;
  };

  class Void
    : public misc::Singleton<Void>
    , public Type
  {
  public:
    void accept(Visitor& v);
    void accept(ConstVisitor& v) const;
  };

} // namespace type
