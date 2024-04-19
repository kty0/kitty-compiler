/**
 ** \file callgraph/tasks.cc
 ** \brief Callgraph module related tasks' implementation.
 */

#include <ostream>

#include <ast/libast.hh>
#include <ast/tasks.hh>
#define DEFINE_TASKS 1
#include <callgraph/tasks.hh>
#undef DEFINE_TASKS
#include <callgraph/libcallgraph.hh>

namespace callgraph::tasks
{
  /*------------.
  | CallGraph.  |
  `------------*/

  static std::unique_ptr<CallGraph> callgraph;

  void callgraph_compute()
  {
    callgraph.reset(::callgraph::callgraph_compute(*ast::tasks::the_program));
  }

  void callgraph_dump()
  {
    precondition(callgraph.get());
    callgraph->print("call");
  }

  /*--------------.
  | ParentGraph.  |
  `--------------*/

  static std::unique_ptr<ParentGraph> parentgraph;

  void parentgraph_compute()
  {
    parentgraph.reset(
      ::callgraph::parentgraph_compute(*ast::tasks::the_program));
  }

  void parentgraph_dump()
  {
    precondition(parentgraph.get());
    parentgraph->print("parent");
  }

} // namespace callgraph::tasks
