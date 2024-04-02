/**
 ** \file bind/renamer.hh
 ** \brief Implementation of bind::Renamer.
 */

#pragma once

#include <iostream>
#include <map>
#include <ast/default-visitor.hh>
#include <ast/non-object-visitor.hh>
#include <ast/all.hh>

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
    /// \}

    /// \name Visiting usage sites.
    /// \{
    // FIXED: Some code was deleted here.
    void operator()(ast::SimpleVar& e) override;
    void operator()(ast::NameTy& e) override;
    void operator()(ast::CallExp& e) override;
    void operator()(ast::MethodCallExp& e) override;
    /// \}

  private:
    // FIXED: Some code was deleted here.
    int count = 0;
    std::map<ast::Dec*, std::string> map;
  };

} // namespace bind

#include <bind/renamer.hxx>
