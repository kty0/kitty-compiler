/**
 ** \file ast/escapable.hh
 ** \brief Declaration of ast::Escapable.
 */

#pragma once

#include <ast/fwd.hh>

namespace ast
{
  /// Escapable.
  class Escapable
  {
    // FIXED: Some code was deleted here.

  public:
    void set_escaped();
    void set_unescaped();
    bool is_locked() const;
    bool is_escaped() const;

  private:
    // FIXED: Some code was deleted here.
    bool escaped = true; // by default, variables are escaped
    bool locked = false;
  };
} // namespace ast
#include <ast/escapable.hxx>
