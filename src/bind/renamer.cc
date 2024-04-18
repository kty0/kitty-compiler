/**
 ** \file bind/renamer.cc
 ** \brief Implementation of bind::Renamer.
 */

#include <bind/renamer.hh>
#include <misc/symbol.hh>

namespace bind
{
  using namespace ast;

  // FIXED: Some code was deleted here.

  void Renamer::operator()(ast::VarDec& e)
  {
    // Make the new name
    std::string name = e.name_get().get() + "_" + std::to_string(count);
    misc::symbol sym = misc::symbol(name);
    // Increment the count for the next element
    count++;
    // Add the new name to the VarDec
    e.name_set(sym);

    if (e.type_name_get() != nullptr)
      {
        (*this)(e.type_name_get());
      }

    if (e.init_get() != nullptr)
      {
        (*this)(e.init_get());
      }
  }

  void Renamer::operator()(ast::FunctionChunk& e)
  {
    chunk_visit<ast::FunctionDec>(e);
  }

  void Renamer::operator()(ast::FunctionDec&)
  {
    // We must not be here.
    unreachable();
  }

  template <>
  void Renamer::visit_dec_header<ast::FunctionDec>(ast::FunctionDec& e)
  {
    (*this)(e.formals_get());

    if (e.result_get() != nullptr)
      {
        (*this)(e.result_get());
      }

    if (e.body_get() != nullptr && e.name_get().get().compare("_main"))
      {
        // Make the new name
        std::string name = e.name_get().get() + "_" + std::to_string(count);
        misc::symbol sym = misc::symbol(name);
        // Increment the count for the next element
        count++;
        // Add the new name to the FunctionDec
        e.name_set(sym);
      }
  }

  template <>
  void Renamer::visit_dec_body<ast::FunctionDec>(ast::FunctionDec& e)
  {
    if (e.body_get() != nullptr)
      {
        (*this)(e.body_get());
      }
  }

  void Renamer::operator()(ast::TypeChunk& e)
  {
    chunk_visit<ast::TypeDec>(e);
  }

  void Renamer::operator()(ast::TypeDec&)
  {
    // We must not be here.
    unreachable();
  }

  template <>
  void Renamer::visit_dec_header<ast::TypeDec>(ast::TypeDec& e)
  {
    // Make the new name
    std::string name = e.name_get().get() + "_" + std::to_string(count);
    misc::symbol sym = misc::symbol(name);
    // Increment the count for the next element
    count++;
    // Add the new name to the TypeDec
    e.name_set(sym);
  }

  template <>
  void Renamer::visit_dec_body<ast::TypeDec>(ast::TypeDec& e)
  {
    (*this)(e.ty_get());
  }

  template <class D> void Renamer::chunk_visit(ast::Chunk<D>& e)
  {
    for (const auto dec : e)
      {
        visit_dec_header<D>(*dec);
      }
    for (const auto dec : e)
      {
        visit_dec_body<D>(*dec);
      }
  }

  void Renamer::operator()(ast::MethodDec& e)
  {
    // Make the new name
    std::string name = e.name_get().get() + "_" + std::to_string(count);
    misc::symbol sym = misc::symbol(name);
    // Increment the count for the next element
    count++;
    // Add the new name to the MethodDec
    e.name_set(sym);
    (*this)(e.formals_get());
    (*this)(e.result_get());
    (*this)(e.body_get());
  }

  void Renamer::operator()(ast::NameTy& e)
  {
    if (e.def_get() == nullptr)
      {
        return;
      }

    Dec* dec = e.def_get();
    // Make the new name
    misc::symbol sym = misc::symbol(dec->name_get());
    e.name_set(sym);
  }

  void Renamer::operator()(ast::SimpleVar& e)
  {
    VarDec* dec = e.def_get();
    misc::symbol sym = misc::symbol(dec->name_get());
    e.name_set(sym);
  }

  void Renamer::operator()(ast::CallExp& e)
  {
    FunctionDec* dec = e.def_get();
    // Make the new name
    misc::symbol sym = misc::symbol(dec->name_get());
    e.name_set(sym);

    for (Exp* exp : e.args_get())
      {
        (*this)(exp);
      }
  }

  void Renamer::operator()(ast::MethodCallExp& e)
  {
    FunctionDec* dec = e.def_get();
    // Make the new name
    misc::symbol sym = misc::symbol(dec->name_get());
    e.name_set(sym);
    for (Exp* exp : e.args_get())
      {
        (*this)(exp);
      }
    (*this)(e.def_get());
    (*this)(e.object_get());
  }

} // namespace bind
