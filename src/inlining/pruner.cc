/**
 ** \file inlining/pruner.cc
 ** \brief Implementation of inlining::Pruner.
 */

#include <inlining/pruner.hh>
#include <parse/tweast.hh>
#include <parse/libparse.hh>

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
/*
  void Pruner::operator()(const ast::FunctionDec& e)
  {
      if (called_functions_.find() == called_functions_.end())
      {

      }
      current_functions_.push_back(e);
  }

  void Pruner::operator()(const ast::CallExp& e)
  {
      if (called_functions_)
  }

  void Pruner::operator()(const ast::FunctionChunk& e)
  {
      parse::Tweast tweast;
      tweast << *prune(const_cast<ast::FunctionChunk&>(e));
      std::cout << *prune(const_cast<ast::FunctionChunk&>(e));
      ast::Exp *res = parse::parse(tweast);
      result_ = res;
  }
  */

} // namespace inlining
