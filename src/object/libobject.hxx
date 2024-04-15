#pragma once

/**
 ** \file object/libobject.hxx
 ** \brief Functions exported by the object module.
 */

#include <memory>

#include <desugar/libdesugar.hh>
#include <object/desugar-visitor.hh>
#include <object/fwd.hh>

namespace object
{

  /*------------------.
  | Desugar objects.  |
  `------------------*/

  template <typename A>
  A* raw_desugar(const A& tree, const class_names_type& class_names)
  {
    // Desugar.
    ::object::DesugarVisitor desugar(class_names);
    desugar(tree);
    return dynamic_cast<A*>(desugar.result_get());
  }

  template <typename A>
  A* desugar(const A& tree, const class_names_type& class_names)
  {
    // Desugar.
    A* desugared = raw_desugar(tree, class_names);
    assertion(desugared);
    std::unique_ptr<A> desugared_ptr(desugared);
    // Recompute the bindings and the types.
    ::desugar::bind_and_types_check(*desugared_ptr);
    return desugared_ptr.release();
  }

  /// Explicit instantiations.
  template ast::ChunkList* raw_desugar(const ast::ChunkList&,
                                       const class_names_type&);
  template ast::ChunkList* desugar(const ast::ChunkList&,
                                   const class_names_type&);

} // namespace object
