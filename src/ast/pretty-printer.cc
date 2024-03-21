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

namespace ast
{
  // Anonymous namespace: these functions are private to this file.
  namespace
  {

    /// \brief Output \a e on \a ostr.
    ///
    /// Used to factor the output of the name declared,
    /// and its possible additional attributes.
    inline std::ostream& operator<<(std::ostream& ostr, const Dec& e)
    {
      ostr << e.name_get();
      return ostr;
    }
  } // namespace

  PrettyPrinter::PrettyPrinter(std::ostream& ostr)
    : ostr_(ostr)
  {}

  /* foo */
  void PrettyPrinter::operator()(const SimpleVar& e)
  {
    ostr_ << e.name_get();
  }

  /* foo.bar */
  void PrettyPrinter::operator()(const FieldVar& e)
  {
    // FIXED: Some code was deleted here.
    ostr_ << e.var_get() << '.' << e.name_get().get();
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

  // FIXME: Some code was deleted here.

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
  }

  /* myfunc(a, b, c) */
  void PrettyPrinter::operator()(const CallExp& e)
  {
    ostr_ << e.name_get() << "(";

    const exps_type& args = e.args_get();
    if (args.size() > 0)
    {
      ostr_ << args[0];
      for (auto it = args.begin() + 1; it != args.end(); it++)
      {
        ostr_ << ", " << (*it);
      }
    }

    ostr_ << ")";
  }

  /* myobj.myfunc(a, b, c) */
  void PrettyPrinter::operator()(const MethodCallExp& e)
  {
    ostr_ << e.object_get() << '.' << e.name_get() << "(";

    const exps_type& args = e.args_get();
    if (args.size() > 0)
    {
      ostr_ << args[0];
      for (auto it = args.begin() + 1; it != args.end(); it++)
      {
        ostr_ << ", " << (*it);
      }
    }

    ostr_ << ")";
  }

  /* for i := 0 to 42 do exp */
  void PrettyPrinter::operator()(const ForExp& e)
  {
    const VarDec& var = e.vardec_get();
    ostr_ << "for " << var.name_get() << " := " << var.init_get() << " to " \
      << e.hi_get() << " do" << misc::incendl << e.body_get() << misc::decendl;
  }

  /* while false do print("oh no") */
  void PrettyPrinter::operator()(const WhileExp& e)
  {
    ostr_ << "while " << e.test_get() << " do" << misc::incendl \
      << e.body_get() << misc::decendl;
  }

  /* if (1 + 2) * 3 / 4
       then 5 * 6
       else 7 + 8) - 9 / 10 */
  void PrettyPrinter::operator()(const IfExp& e)
  {
    ostr_ << "if " << e.test_get() << misc::incendl \
      << "then " << e.thenclause_get() \
      << "else" << e.elseclause_get() \
      << misc::decendl;
  }

  void PrettyPrinter::operator()(const ArrayTy& e)
  {
    ostr_ << e.base_type_get();
  }

  void PrettyPrinter::operator()(const ClassTy& e)
  {
    ostr_ << e.super_get();
  }

  void PrettyPrinter::operator()(const NameTy& e)
  {
    ostr_ << e.name_get();
  }

  /* I'm unsure of what to do
  void PrettyPrinter::operator()(const RecordTy& e)
  {
  }
  */

  /* myfunc(a : int, b : string) : int =
       exp */
  void PrettyPrinter::operator()(const FunctionDec& e)
  {
    ostr_ << e.name_get() << '(';

    const VarChunk& formals = e.formals_get();
    if (!formals.empty())
    {
      for (auto it = formals.begin() + 1; it != formals.end(); it++)
      {
        if (it == formals.begin())
        {
          ostr_ << (*it)->name_get() << " : " << (*it)->type_name_get();
        }
        else
        {
          ostr_ << ", " << (*it)->name_get() << " : " << (*it)->type_name_get();
        }
      }
    }

    ostr_ << ')';

    const NameTy* result = e.result_get();
    if (result != nullptr)
    {
      ostr_ << " : " << result->name_get();
    }

    ostr_ << " =" << misc::incendl \
      << e.body_get() << misc::decendl;
  }

  void PrettyPrinter::operator()(const TypeDec& e)
  {
    ostr_ << "type " << e.name_get() << " = " << e.ty_get();
  }

  void PrettyPrinter::operator()(const VarDec& e)
  {
    ostr_ << "var " << e.name_get();

    const NameTy* type_name = e.type_name_get();
    if (type_name != nullptr)
    {
      ostr_ << ": " << type_name->name_get();
    }

    ostr_ << " := " << e.init_get() ;
  }

  void PrettyPrinter::operator()(const ast::IntExp& e)
  {
    ostr_ << e.value_get();
  }

  void PrettyPrinter::operator()(const ast::LetExp& e)
  {
    ostr_ << "let" << misc::incindent;
    auto chunks = e.chunks_get();
    for (auto it = chunks.begin(); it != chunks.end(); it++)
    {
      ostr_ << (*it);
    }
    ostr_ << misc::decindent;
    ostr_ << "in" << misc::incindent;
    ostr_ << e.body_get() << misc::decindent();
    ostr_ << "end";
  }

  void PrettyPrinter::operator()(const ast::NilExp& e)
  {
    ostr_ << "nil";
  }

  void PrettyPrinter::operator()(const ast::ObjectExp& e)
  {
    ostr_ << "new " << e.type_name_get();
  }

  void PrettyPrinter::operator()(const ast::OpExp& e)
  {
    ostr_ << e.left_get() << " " << e.oper_get() << " " << e.right_get();
  }

  void PrettyPrinter::operator()(const ast::RecordExp& e)
  {
    ostr_ << e.type_name_get() << " { ";
    auto fields = e.fields_get();
    if (fields.size() != 0)
    {
      ostr_ << fields[0]->name_get() << " = " << fields[0]->init_get();
      for (auto it = fields.begin() + 1; it != fields.end(); it++)
      {
        ostr_ << " , " << (*it)->name_get() << " = " << (*it)->init_get();
      }
    }
    ostr_ << " }";
  }

  void PrettyPrinter::operator()(const ast::SeqExp& e)
  {
    auto exps = e.exps_get();
    ostr_ << "(";
    if (exps.size() != 0)
    {
      ostr_ << exps[0];
      for (auto it = exps.begin() + 1; it != exps.end(); it++)
      {
        ostr_ << " ; " << (*it);
      }
    }
    ostr_ << ")";
  }

  void PrettyPrinter::operator()(const ast::StringExp& e)
  {
    ostr_ << e.value_get();
  }
} // namespace ast
