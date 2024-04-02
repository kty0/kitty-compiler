/**
 ** \file object/libobject.cc
 ** \brief Define exported object functions.
 */

// IGNORED: Some code was deleted here.
#include <object/libobject.hh>
#include <object/type-checker.hh>

namespace object
{
  /*-------.
  | Bind.  |
  `-------*/

  // IGNORED: Some code was deleted here.

  /*----------------.
  | Compute types.  |
  `----------------*/

  misc::error types_check(ast::Ast& tree)
  {
    TypeChecker type;
    type(tree);
    return type.error_get();
  }

} // namespace object
