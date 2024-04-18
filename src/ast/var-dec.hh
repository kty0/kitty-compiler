/**
 ** \file ast/var-dec.hh
 ** \brief Declaration of ast::VarDec.
 */

#pragma once

#include <ast/dec.hh>
#include <ast/escapable.hh>
#include <ast/exp.hh>
#include <ast/name-ty.hh>

namespace ast
{
  /// VarDec.
  class VarDec
    : public Dec
    , public Escapable
  {
  public:
    /** \name Ctor & dtor.
     ** \{ */
    /// Construct a VarDec node.
    VarDec(const Location& location,
           misc::symbol name,
           NameTy* type_name,
           Exp* init);
    VarDec(const VarDec&) = delete;
    VarDec& operator=(const VarDec&) = delete;
    /// Destroy a VarDec node.
    ~VarDec() override;
    /** \} */

    /// \name Visitors entry point.
    /// \{ */
    /// Accept a const visitor \a v.
    void accept(ConstVisitor& v) const override;
    /// Accept a non-const visitor \a v.
    void accept(Visitor& v) override;
    /// \}

    /** \name Accessors.
     ** \{ */
    /// Return optional type of the declared variable.
    const NameTy* type_name_get() const;
    /// Return optional type of the declared variable.
    NameTy* type_name_get();
    /// Return the initial value (expression) assigned to the variable.
    const Exp* init_get() const;
    /// Return the initial value (expression) assigned to the variable.
    Exp* init_get();
    /** \} */

    /// Return whether the variable is read-only or not.
    bool read_only_get() const;
    /// Allows setting whether the variable is read-only or not.
    void read_only_set(bool status);

  protected:
    /// Optional type of the declared variable.
    NameTy* type_name_;
    /// The initial value (expression) assigned to the variable.
    Exp* init_;
    /// Whether the variable is read-only or not (useful for for-loops).
    bool read_only = false;
  };
} // namespace ast
#include <ast/var-dec.hxx>
