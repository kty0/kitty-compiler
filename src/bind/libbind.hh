/**
 ** \file bind/libbind.hh
 ** \brief Interface of the bind module.
 */

// FIXED: Some code was deleted here.
#pragma once

#include <ast/fwd.hh>
#include <misc/error.hh>

namespace bind
{
  misc::error bind(ast::Ast& tree);

  void rename(ast::Ast& tree);
}
