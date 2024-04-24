/**
 ** \file inlining/inliner.cc
 ** \brief Implementation of inlining::Inliner.
 */

#include <boost/graph/transitive_closure.hpp>

#include <ranges>
#include <callgraph/libcallgraph.hh>
#include <inlining/inliner.hh>
#include <parse/libparse.hh>
#include <parse/tweast.hh>

namespace inlining
{
  using namespace ast;

  Inliner::Inliner(const ast::Ast& tree)
    : super_type()
    , rec_funs_()
  {
    // Compute the transitive closure of the call graph to compute the
    // set of recursive functions.
    const callgraph::CallGraph* graph = callgraph::callgraph_compute(tree);
    callgraph::CallGraph closure;
    boost::transitive_closure(*graph, closure);

    // Compute the parent graph to get nested functions.
    const callgraph::ParentGraph* parentGraph =
      callgraph::parentgraph_compute(const_cast<ast::Ast&>(tree));

    // Re-attach properties to the vertices.
    for (auto [i, i_end] = boost::vertices(closure); i != i_end; ++i)
      closure[*i] = (*graph)[*i];
    // Detect recursive functions.
    for (auto [i, i_end] = boost::vertices(closure); i != i_end; ++i)
      {
        for (auto [j, j_end] = boost::adjacent_vertices(*i, closure);
             j != j_end; ++j)
          {
            // Check all parent functions to detect cases like these :
            // function a() =
            // (
            //     let
            //         function b() = a()
            //     in
            //     end
            // )
            if (parentGraph->hfundec_deep_get(closure[*i], closure[*j]))
              rec_funs_.insert(closure[*j]);
          }
      }
    delete graph;
    delete parentGraph;
  }

  const misc::set<const ast::FunctionDec*>& Inliner::rec_funs_get() const
  {
    return rec_funs_;
  }

  // FIXED: Some code was deleted here.

  void Inliner::operator()(const ast::CallExp& e)
  {
      parse::Tweast tweast;

      tweast << "let" << '\n';

      VarChunk* dec = recurse(e.def_get()->formals_get());
      for (size_t i = 0; i < dec->decs_get().size(); i++)
      {
          misc::symbol name = dec->decs_get()[i]->name_get();
          misc::symbol type_name = dec->decs_get()[i]->type_name_get()->name_get();
          tweast << "var " << name << " : " << type_name << " := " << exp << '\n';
      }

      tweast << "in" << '\n';

      Exp* body = recurse(e.def_get()->body_get());

      tweast << body << '\n'
            << "end\n";

      ast::Exp *res = parse::parse(tweast);
      result_ = res;
  }

} // namespace inlining
