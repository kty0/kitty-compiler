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
    void escaped_set();
    void unescaped_set();
    bool is_escaped() const;
    ast::FunctionDec* def_site_get() const;
    void def_site_set(ast::FunctionDec* e);

  private:
    // FIXED: Some code was deleted here.
    bool escaped_ = true; // by default, variables are escaped
    ast::FunctionDec* def_site_ = nullptr;
  };
} // namespace ast
#include <ast/escapable.hxx>
