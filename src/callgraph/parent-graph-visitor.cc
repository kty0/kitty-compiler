/**
 ** \file callgraph/parent-graph-visitor.cc
 ** \brief Implementation for callgraph::ParentGraphVisitor.
 **/

#include <ast/function-dec.hh>
#include <callgraph/parent-graph-visitor.hh>

namespace callgraph
{
  ParentGraph* ParentGraphVisitor::create(ast::Ast& tree)
  {
    // Create a new empty parentgraph
    parentgraph = new ParentGraph();

    // Launch visitor.
    tree.accept(*this);

    // Return created parentgraph.
    return parentgraph;
  }

  void ParentGraphVisitor::operator()(ast::FunctionChunk& e)
  {
    for (ast::FunctionDec* f : e)
      {
        parentgraph->fundec_add(f);
        parentgraph->fundec_link(f, parent);
      }
    super_type::operator()(e);
  }

  void ParentGraphVisitor::operator()(ast::FunctionDec& e)
  {
    // Current function becomes temporarily the parent function.
    ast::FunctionDec* tmp = parent;
    parent = &e;
    super_type::operator()(e);
    parent = tmp;
  }

} // namespace callgraph
