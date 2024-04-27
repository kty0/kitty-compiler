/**
 ** \file inlining/inliner.hh
 ** \brief Declaration of inlining::Inliner.
 */

#pragma once

#include <map>

#include <ast/function-dec.hh>
#include <astclone/cloner.hh>
#include <misc/scoped-map.hh>
#include <misc/set.hh>

namespace inlining
{
  /// Perform inline expansion of functions.
  class Inliner : public astclone::Cloner
  {
  public:
    using super_type = astclone::Cloner;

    // Import overloaded virtual functions.
    using super_type::operator();

    /// Build an Inliner.
    Inliner(const ast::Ast& tree);

    /// \name Visit methods.
    /// \{
    // FIXED: Some code was deleted here.
    void operator()(const ast::CallExp&) override;
    /// \}

    /// \name Getters.
    /// \{
    const misc::set<const ast::FunctionDec*>& rec_funs_get() const;
    /// \}

  private:
    /// Recursive functions of the program.
    misc::set<const ast::FunctionDec*> rec_funs_;
  };

} // namespace inlining
