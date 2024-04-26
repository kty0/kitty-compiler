/**
 ** \file callgraph/parent-graph-visitor.hh
 ** \brief Definition of callgraph::ParentGraphVisitor.
 **/
#pragma once

#include <ast/default-visitor.hh>
#include <ast/non-object-visitor.hh>
#include <callgraph/fundec-graph.hh>

namespace callgraph
{
  /// Compute the ParentGraph.
  class ParentGraphVisitor
    : public ast::DefaultVisitor
    , public ast::NonObjectVisitor
  {
    using super_type = ast::DefaultVisitor;
    using super_type::operator();

  public:
    ParentGraph* create(ast::Ast& tree);

    void operator()(ast::FunctionChunk& e) override;
    void operator()(ast::FunctionDec& e) override;

  protected:
    /// Current function.
    ast::FunctionDec* parent = nullptr;
    /// Parent graph.
    ParentGraph* parentgraph = nullptr;
  };

} // namespace callgraph
