/**
 ** \file inlining/pruner.cc
 ** \brief Implementation of inlining::Pruner.
 */

#include <inlining/pruner.hh>

namespace inlining
{
  using namespace ast;

  ast::FunctionChunk* Pruner::prune(ast::FunctionChunk& e)
  {
    while (true)
      {
        auto [remove_begin, remove_end] =
          std::ranges::remove_if(e, [&](ast::FunctionDec* func_dec) {
            if (!func_dec->body_get() || func_dec->name_get() == "_main")
              return false;
            else
              return called_functions_[func_dec->name_get()] == 0;
          });

        if (remove_begin == remove_end)
          break;
        e.erase(remove_begin, remove_end);
      }

    return new FunctionChunk(e.location_get(), &e.decs_get());
  }

  // FIXME: Some code was deleted here.

} // namespace inlining
