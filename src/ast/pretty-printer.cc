/**
 ** \file ast/pretty-printer.cc
 ** \brief Implementation of ast::PrettyPrinter.
 */

#include <ast/all.hh>
#include <ast/libast.hh>
#include <ast/pretty-printer.hh>
#include <misc/escape.hh>
#include <misc/indent.hh>
#include <misc/separator.hh>
#include <type/class.hh>

namespace ast
{
  // Anonymous namespace: these functions are private to this file.
  namespace
  {
    /*
    /// Output \a e on \a ostr.
    inline std::ostream& operator<<(std::ostream& ostr, const Escapable& e)
    {
      if (escapes_display(ostr))
        // FIXED: Some code was deleted here.
        ostr << "\/* escaping *\/ ";

      return ostr;
    }

    /// \brief Output \a e on \a ostr.
    ///
    /// Used to factor the output of the name declared,
    /// and its possible additional attributes.
    /// REMOVED THIS BECAUSE WE FOUND IT USELESS
    inline std::ostream& operator<<(std::ostream& ostr, const Dec& e)
    {
      ostr << e.name_get();
      if (bindings_display(ostr))
        ostr << " \/* " << &e << " *\/";
      return ostr;
    }
    */
  } // namespace

  PrettyPrinter::PrettyPrinter(std::ostream& ostr)
    : ostr_(ostr)
  {}

  /* foo */
  void PrettyPrinter::operator()(const SimpleVar& e)
  {
    ostr_ << e.name_get();
    if (bindings_display(ostr_))
      ostr_ << " /* " << e.def_get() << " */";
  }

  /* foo.bar */
  void PrettyPrinter::operator()(const FieldVar& e)
  {
    // FIXED: Some code was deleted here.
    ostr_ << e.var_get() << '.' << e.name_get();
  }

  /* foo[10] */
  void PrettyPrinter::operator()(const SubscriptVar& e)
  {
    ostr_ << e.var_get() << '[' << misc::incindent << e.index_get()
          << misc::decindent << ']';
  }

  void PrettyPrinter::operator()(const CastExp& e)
  {
    ostr_ << "_cast(" << e.exp_get() << ", " << e.ty_get() << ')';
  }

  // FIXED: Some code was deleted here.

  /* int[42] of 0 */
  void PrettyPrinter::operator()(const ArrayExp& e)
  {
    ostr_ << e.type_name_get() << '[' << misc::incindent << e.size_get()
          << misc::decindent << ']' << " of " << e.init_get();
  }

  /* a := 42 */
  void PrettyPrinter::operator()(const AssignExp& e)
  {
    ostr_ << e.var_get() << " := " << e.exp_get();
  }

  /* break */
  void PrettyPrinter::operator()(const BreakExp& e)
  {
    ostr_ << "break";
    if (bindings_display(ostr_))
      ostr_ << " /* " << e.def_get() << " */";
  }

  /* myfunc(a, b, c) */
  void PrettyPrinter::operator()(const CallExp& e)
  {
    ostr_ << e.name_get();
    if (bindings_display(ostr_))
      ostr_ << " /* " << e.def_get() << " */";
    ostr_ << "(";
    const exps_type& args = e.args_get();
    ostr_ << misc::separate(args, ", ");
    ostr_ << ")";
  }

  /* myobj.myfunc(a, b, c) */
  void PrettyPrinter::operator()(const MethodCallExp& e)
  {
    ostr_ << e.object_get() << '.' << e.name_get();
    if (bindings_display(ostr_))
      ostr_ << " /* " << e.def_get() << " */";
    ostr_ << "(";
    const exps_type& args = e.args_get();
    ostr_ << misc::separate(args, ", ");
    ostr_ << ")";
  }

  /* for i := 0 to 42 do
   *   exp
   */
  void PrettyPrinter::operator()(const ForExp& e)
  {
    const VarDec& var = e.vardec_get();
    ostr_ << "for ";
    if (bindings_display(ostr_))
      {
        ostr_ << " /* " << &e << " */ ";
      }
    if (escapes_display(ostr_) && var.is_escaped())
      {
        ostr_ << "/* escaping */ ";
      }
    ostr_ << var.name_get();
    if (bindings_display(ostr_))
      {
        ostr_ << " /* " << &var << " */";
      }
    ostr_ << " := " << *var.init_get() << " to " << e.hi_get() << " do"
          << misc::incendl << e.body_get() << misc::decindent;
  }

  /* while false do
   *   print("oh no")
   */
  void PrettyPrinter::operator()(const WhileExp& e)
  {
    ostr_ << "while ";
    if (bindings_display(ostr_))
      {
        ostr_ << "/* " << &e << " */ ";
      }
    ostr_ << e.test_get() << " do" << misc::incendl << e.body_get()
          << misc::decindent;
  }

  /* (if (1 + 2) * 3 / 4
       then 5 * 6
       else 7 + 8) - 9 / 10
   */
  void PrettyPrinter::operator()(const IfExp& e)
  {
    ostr_ << "if " << misc::incindent << e.test_get() << misc::iendl << "then "
          << e.thenclause_get() << misc::iendl << "else " << e.elseclause_get()
          << misc::decindent;
  }

  void PrettyPrinter::operator()(const ArrayTy& e)
  {
    ostr_ << e.base_type_get();
  }

  void PrettyPrinter::operator()(const ClassTy& e) { ostr_ << e.super_get(); }

  void PrettyPrinter::operator()(const NameTy& e)
  {
    ostr_ << e.name_get();
    if (bindings_display(ostr_))
      {
        ostr_ << " /* " << e.def_get() << " */";
      }
  }

  void PrettyPrinter::operator()(const RecordTy& e)
  {
    ostr_ << "{ " << misc::separate(e.fields_get(), ", ") << " }";
  }

  /* a : int */
  void PrettyPrinter::operator()(const Field& e)
  {
    ostr_ << e.name_get() << " : " << e.type_name_get();
  }

  /*
   * var a := 0
   * var b := 0
   *
   * type t = {}
   *
   * function f() = 42
   */
  void PrettyPrinter::operator()(const ChunkList& e)
  {
    ostr_ << misc::separate(e.chunks_get(), misc::iendl);
  }

  /*
   * type a = {}
   * type b = {}
   */
  void PrettyPrinter::operator()(const ast::TypeChunk& e)
  {
    if (e.empty())
      {
        return;
      }

    ostr_ << **e.begin();
    for (auto it = e.begin() + 1; it != e.end(); it++)
      {
        ostr_ << misc::iendl << **it;
      }
  }

  /*
   * function f() = 42
   * function g() : int = 2508
   */
  void PrettyPrinter::operator()(const ast::FunctionChunk& e)
  {
    if (e.empty())
      {
        return;
      }

    ostr_ << **e.begin();
    for (auto it = e.begin() + 1; it != e.end(); it++)
      {
        ostr_ << misc::iendl << **it;
      }
  }

  /* x = 5 */
  void PrettyPrinter::operator()(const FieldInit& e)
  {
    ostr_ << e.name_get() << " = " << e.init_get();
  }

  /* function myfunc(a : int, b : string) : int =
       exp */
  void PrettyPrinter::operator()(const FunctionDec& e)
  {
    ostr_ << ((e.body_get() == nullptr) ? "primitive " : "function ")
          << e.name_get();
    if (bindings_display(ostr_))
      {
        ostr_ << " /* " << &e << " */";
      }
    ostr_ << '(';
    const VarChunk& formals = e.formals_get();
    if (!formals.empty())
      {
        if (bindings_display(ostr_))
          {
            if (escapes_display(ostr_) && (*formals.begin())->is_escaped())
              {
                ostr_ << "/* escaping */ ";
              }
            ostr_ << (*formals.begin())->name_get() << " /* "
                  << (*formals.begin()) << " */"
                  << " : " << *(*formals.begin())->type_name_get();
            for (auto it = formals.begin() + 1; it != formals.end(); it++)
              {
                ostr_ << ", ";
                if (escapes_display(ostr_) && (*it)->is_escaped())
                  {
                    ostr_ << "/* escaping */ ";
                  }
                ostr_ << (*it)->name_get() << " /* " << (*it) << " */"
                      << " : " << *(*it)->type_name_get();
              }
          }
        else
          {
            if (escapes_display(ostr_) && (*formals.begin())->is_escaped())
              {
                ostr_ << "/* escaping */ ";
              }
            ostr_ << (*formals.begin())->name_get() << " : "
                  << *(*formals.begin())->type_name_get();
            for (auto it = formals.begin() + 1; it != formals.end(); it++)
              {
                ostr_ << ", ";
                if (escapes_display(ostr_) && (*it)->is_escaped())
                  {
                    ostr_ << "/* escaping */ ";
                  }
                ostr_ << (*it)->name_get() << " : " << *(*it)->type_name_get();
              }
          }
      }

    ostr_ << ')';

    const NameTy* result = e.result_get();

    if (result != nullptr)
      {
        if (bindings_display(ostr_))
          {
            ostr_ << " : " << result->name_get() << " /* " << result->def_get()
                  << " */";
          }
        else
          {
            ostr_ << " : " << result->name_get();
          }
      }

    if (e.body_get() == nullptr)
      {
        return;
      }

    ostr_ << " =" << misc::incendl << *e.body_get() << misc::decindent;
  }

  void PrettyPrinter::operator()(const TypeDec& e)
  {
    ostr_ << "type " << e.name_get();
    if (bindings_display(ostr_))
      {
        ostr_ << " /* " << &e << " */";
      }
    ostr_ << " = " << e.ty_get();
  }

  /* var x := 42 */
  void PrettyPrinter::operator()(const VarDec& e)
  {
    ostr_ << "var ";
    if (escapes_display(ostr_) && e.is_escaped())
      {
        ostr_ << "/* escaping */ ";
      }
    ostr_ << e.name_get();
    if (bindings_display(ostr_))
      {
        ostr_ << " /* " << &e << " */";
      }
    const NameTy* type_name = e.type_name_get();
    if (type_name != nullptr)
      {
        ostr_ << ": " << *type_name;
      }

    ostr_ << " := " << *e.init_get();
  }

  /* 42 */
  void PrettyPrinter::operator()(const ast::IntExp& e)
  {
    ostr_ << e.value_get();
  }

  /* let
   *   var x := 42
   * in
   *   print(x)
   * end
   */
  void PrettyPrinter::operator()(const ast::LetExp& e)
  {
    ostr_ << "let" << misc::incendl << e.chunks_get() << misc::decendl << "in"
          << misc::incendl << e.body_get() << misc::decendl << "end";
  }

  /* nil */
  void PrettyPrinter::operator()(const ast::NilExp& e) { ostr_ << "nil"; }

  /* new myObj */
  void PrettyPrinter::operator()(const ast::ObjectExp& e)
  {
    ostr_ << "new " << e.type_name_get();
  }

  /* a + b */
  void PrettyPrinter::operator()(const ast::OpExp& e)
  {
    ostr_ << "(" << e.left_get() << " " << str(e.oper_get()) << " "
          << e.right_get() << ")";
  }

  /* node { a = 42, b = "oui" } */
  void PrettyPrinter::operator()(const ast::RecordExp& e)
  {
    ostr_ << e.type_name_get() << " { " << misc::separate(e.fields_get(), ", ")
          << " }";
  }

  /* (
   *   exp;
   *   exp;
   *   ()
   * )
   */
  void PrettyPrinter::operator()(const ast::SeqExp& e)
  {
    if (e.exps_get().size() == 0)
      {
        ostr_ << "()";
        return;
      }
    else if (e.exps_get().size() == 1)
      {
        std::vector<Exp*> exp = e.exps_get();
        ostr_ << **exp.begin();
        return;
      }

    std::vector<Exp*> exps = e.exps_get();

    ostr_ << '(' << misc::incendl << **exps.begin();
    for (auto it = exps.begin() + 1; it != exps.end(); it++)
      {
        ostr_ << ';' << misc::iendl << *(*it);
      }
    ostr_ << misc::decendl << ')';
  }

  /* "Never gonna give you up
   *  Never gonna let you down
   *  Never gonna run around and desert you
   *  Never gonna make you cry
   *  Never gonna say goodbye
   *  Never gonna tell a lie and hurt you"
   */
  void PrettyPrinter::operator()(const ast::StringExp& e)
  {
    ostr_ << '"' << misc::escape(e.value_get()) << '"';
  }

} // namespace ast
