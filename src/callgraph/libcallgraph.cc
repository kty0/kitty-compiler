/**
 ** \file callgraph/libcallgraph.cc
 ** \brief Define exported callgraph functions.
 */

#include <callgraph/call-graph-visitor.hh>
#include <callgraph/libcallgraph.hh>
#include <callgraph/parent-graph-visitor.hh>

#include <misc/contract.hh>
#include <misc/set.hh>

namespace callgraph
{

  // Build the callgraph.
  const CallGraph* callgraph_compute(const ast::Ast& tree)
  {
    CallGraphVisitor callgraph_visitor;
    return callgraph_visitor.create(tree);
  }

  CallGraph* callgraph_compute(ast::Ast& tree)
  {
    CallGraphVisitor callgraph_visitor;
    return callgraph_visitor.create(tree);
  }

  // Build the parentgraph.
  ParentGraph* parentgraph_compute(ast::Ast& tree)
  {
    ParentGraphVisitor parentgraph_visitor;
    return parentgraph_visitor.create(tree);
  }

} // namespace callgraph
