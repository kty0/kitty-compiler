/**
 ** \file desugar/bounds-checking-visitor.cc
 ** \brief Implementation of desugar::BoundsCheckingVisitor.
 */

#include <ast/all.hh>
#include <ast/libast.hh>
#include <desugar/bounds-checking-visitor.hh>
#include <misc/symbol.hh>
#include <parse/libparse.hh>

namespace desugar
{
  namespace
  {
    /// Return the name of the boxed type for \a s.
    std::string box(misc::symbol s) { return "_box_" + s.get(); }

  } // namespace

  BoundsCheckingVisitor::BoundsCheckingVisitor()
    : super_type()
  {}

  /*-----------------------.
  | Array bounds checking.  |
  `-----------------------*/

  // FIXME: Some code was deleted here.
  void DesugarVisitor::operator()(const ast::ArrayTy& e)
  {
    if (!desugar_for_p_)
      {
        super_type::operator()(e);
      }
    else
      {
        parse::Tweast tweast;

        tweast << "type _box_string_array_17 = {
            arr : string_array_17,
            size : int
        }"

        ast::Exp *res = parse::parse(tweast);
        result_ = res;
      }
  }

} // namespace desugar
