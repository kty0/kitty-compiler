// -*- C++ -*-

%module tiger_object

%import "tiger_ast.i"

%{
  #include <object/libobject.hh>
  #include <ast/all.hh>
  #include "swig_real_type.hh"
%}

%import "tiger_misc.i"

%rename(desugar) object_desugar;
%rename(raw_desugar) object_raw_desugar;

%inline {
ast::Ast* object_desugar(const ast::Ast& tree,
                         const object::class_names_type& class_names)
{
  return object::desugar<ast::Ast>(tree, class_names);
}
ast::Ast* object_raw_desugar(const ast::Ast& tree,
                             const object::class_names_type& class_names)
{
  return object::raw_desugar<ast::Ast>(tree, class_names);
}
}

%include "object/fwd.hh"
%include "object/libobject.hh"
