// -*- C++ -*-

%module tiger_combine

%include "std_pair.i"

%{
  #include <combine/libcombine.hh>
  using namespace combine;
%}

%template() std::pair<overload::overfun_bindings_type, misc::error>;

%include "combine/libcombine.hh"
