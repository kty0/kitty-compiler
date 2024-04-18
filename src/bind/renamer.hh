/**
 ** \file bind/renamer.hh
 ** \brief Implementation of bind::Renamer.
 */

#pragma once

#include <iostream>
#include <map>
#include <ast/all.hh>
#include <ast/default-visitor.hh>
#include <ast/non-object-visitor.hh>

namespace bind
{
  /// Perform identifier renaming within an AST (in place),
  /// without support for objects.
  class Renamer
    : public ast::DefaultVisitor
    , public ast::NonObjectVisitor
  {
  public:
    using super_type = ast::DefaultVisitor;

    // Import overloaded virtual functions.
    using super_type::operator();

    // FIXED: Some code was deleted here.

    // Visit methods.
    /// \brief Process a declaration body or a usage site.
    ///
    /// \a def is the definition site of \e (must be equal to
    /// \a e if it is a Dec node).
    template <class E, class Def> void visit(E& e, const Def* def);

    /// \name Visiting definition sites.
    /// \{
    // FIXED: Some code was deleted here.
    void operator()(ast::TypeDec& e) override;
    void operator()(ast::FunctionDec& e) override;
    void operator()(ast::VarDec& e) override;
    void operator()(ast::MethodDec& e) override;
    void operator()(ast::FunctionChunk& e) override;
    void operator()(ast::TypeChunk& e) override;
    /// \}

    /// \name Visiting usage sites.
    /// \{
    // FIXED: Some code was deleted here.
    void operator()(ast::SimpleVar& e) override;
    void operator()(ast::NameTy& e) override;
    void operator()(ast::CallExp& e) override;
    void operator()(ast::MethodCallExp& e) override;
    /// \}

    template <class D> void chunk_visit(ast::Chunk<D>& e);

    template <class D> void visit_dec_header(D& e);
    template <class D> void visit_dec_body(D& e);

  private:
    // FIXED: Some code was deleted here.
    int count = 0;
  };

  template <>
  void Renamer::visit_dec_header<ast::FunctionDec>(ast::FunctionDec& e);
  template <>
  void Renamer::visit_dec_body<ast::FunctionDec>(ast::FunctionDec& e);

  template <> void Renamer::visit_dec_header<ast::TypeDec>(ast::TypeDec& e);
  template <> void Renamer::visit_dec_body<ast::TypeDec>(ast::TypeDec& e);
} // namespace bind

#include <bind/renamer.hxx>
