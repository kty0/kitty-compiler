// -*- C++ -*-

%module tiger_callgraph

%include "std_string.i"

%{
  #include <callgraph/fundec-graph.hh>
  #include <callgraph/libcallgraph.hh>
%}

namespace callgraph
{
  class FundecGraph;
  using CallGraph = FundecGraph;
}

%include "callgraph/libcallgraph.hh"
