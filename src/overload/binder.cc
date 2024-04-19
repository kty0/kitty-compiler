/**
 ** \file overload/binder.cc
 ** \brief Implementation of overload::Binder.
 */

#include <ast/all.hh>
#include <overload/binder.hh>

namespace overload
{
  overfun_bindings_type& Binder::overfun_bindings_get()
  {
    return overfun_bindings_;
  }

  /*---------.
  | Scopes.  |
  `---------*/

  // FIXME: Some code was deleted here.

  /*-------------------.
  | Visiting methods.  |
  `-------------------*/

  // Same as Binder's, but do not set the definition site of E.
  void Binder::operator()(ast::CallExp& e)
  {
    // FIXME: Some code was deleted here.
  }

  // Insert the prototype of the function in the environment.
  void Binder::visit_dec_header(ast::FunctionDec& e)
  {
    // FIXME: Some code was deleted here (Checks for the _main case).
    overfuns_.put(e.name_get(), e);
  }

  void Binder::operator()(ast::FunctionChunk& e)
  {
    // Two passes: once on headers, then on bodies.
    // FIXME: Some code was deleted here.
  }

} // namespace overload
