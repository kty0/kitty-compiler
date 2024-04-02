/**
 ** \file bind/libbind.cc
 ** \brief Define exported bind functions.
 */

// FIXED: Some code was deleted here.
#include <ast/fwd.hh>
#include <bind/binder.hh>
#include <bind/libbind.hh>
#include <bind/renamer.hh>
#include <misc/error.hh>

namespace bind
{
  misc::error bind(ast::Ast& tree)
  {
    Binder binder;
    binder(tree);
    return binder.error_get();
  }

  void rename(ast::Ast& tree)
  {
    Renamer renamer;
    renamer(tree);
  }
} // namespace bind
