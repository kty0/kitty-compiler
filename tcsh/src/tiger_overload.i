// -*- C++ -*-

%module tiger_overload

%include "std_pair.i"

%{
  #include <overload/liboverload.hh>
  using namespace overload;
%}

%template() std::pair<overfun_bindings_type, misc::error>;

%include "overload/liboverload.hh"
