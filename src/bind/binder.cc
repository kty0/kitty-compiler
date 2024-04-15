/**
 ** \file bind/binder.cc
 ** \brief Implementation for bind/binder.hh.
 */

#include <ast/all.hh>
#include <bind/binder.hh>
#define GETFUN std::get<ast::FunctionDec*>(t)
#define GETVAR std::get<ast::VarDec*>(t)
#define GETTYPE std::get<ast::TypeDec*>(t)

#include <cstring>
#include <misc/contract.hh>

namespace bind
{
  /*-----------------.
  | Error handling.  |
  `-----------------*/

  /// The error handler.
  const misc::error& Binder::error_get() const { return error_; }

  // FIXED: Some code was deleted here.

  void Binder::operator()(ast::FunctionDec& e)
  {
    try
      {
        binding_tuples t = this->sm.get(e.name_get());
        if (GETFUN != nullptr)
          {
            this->error_ << misc::error::error_type::bind << e.location_get()
                         << ": bind error, redefinition of function "
                         << e.name_get() << "\n";
          }
        else
          {
            sm.put(e.name_get(), {&e, GETVAR, GETTYPE});
          }
      }
    catch (std::invalid_argument& er)
      {
        this->sm.put(e.name_get(), {&e, nullptr, nullptr});
      }

    this->sm.scope_begin(); // open the scope

    (*this)(e.formals_get()); // visit

    if (e.result_get() != nullptr)
      {
        (*this)(e.result_get()); // visit
      }

    if (e.body_get() != nullptr)
      {
        (*this)(e.body_get()); // visit
      }

    this->sm.scope_end(); // close the scope
  }

  void Binder::operator()(ast::TypeDec& e)
  {
    try
      {
        binding_tuples t = this->sm.get(e.name_get());
        if (GETTYPE != nullptr)
          {
            this->error_ << misc::error::error_type::bind << e.location_get()
                         << ": bind error, redefinition of type "
                         << e.name_get() << "\n";
          }
        else
          {
            sm.put(e.name_get(), {GETFUN, GETVAR, &e});
          }
      }
    catch (std::invalid_argument& er)
      {
        this->sm.put(e.name_get(), {nullptr, nullptr, &e});
      }
    (*this)(e.ty_get());
  }

  void Binder::operator()(ast::VarDec& e)
  {
    try
      {
        binding_tuples t = this->sm.get(e.name_get());
        if (GETVAR != nullptr)
          {
            this->error_ << misc::error::error_type::bind << e.location_get()
                         << ": bind error, redefinition of variable "
                         << e.name_get() << "\n";
          }
        else
          {
            sm.put(e.name_get(), {GETFUN, &e, GETTYPE});
          }
      }
    catch (std::invalid_argument& er)
      {
        this->sm.put(e.name_get(), {nullptr, &e, nullptr});
      }

    if (e.type_name_get() != nullptr)
      {
        (*this)(e.type_name_get());
      }
    if (e.init_get() != nullptr)
      {
        (*this)(e.init_get());
      }
  }

  void Binder::operator()(ast::ForExp& e)
  {
    this->sm.scope_begin();
    break_stack.push(&e);
    (*this)(e.vardec_get());
    (*this)(e.hi_get());
    (*this)(e.body_get());
    this->sm.scope_end();
  }

  void Binder::operator()(ast::WhileExp& e)
  {
    break_stack.push(&e);
    (*this)(e.test_get());
    (*this)(e.body_get());
  }

  void Binder::operator()(ast::SimpleVar& e)
  {
    try
      {
        auto t = this->sm.get(e.name_get());
        ast::VarDec* def = GETVAR;
        if (def == nullptr)
          {
            this->error_ << misc::error::error_type::bind << e.location_get()
                         << ": bind error, undeclared variable " << e.name_get()
                         << "\n";
          }
        e.def_set(def);
      }
    catch (std::invalid_argument& ex)
      {
        this->error_ << misc::error::error_type::bind << e.location_get()
                     << ": bind error, undeclared variable " << e.name_get()
                     << "\n";
      }
  }

  void Binder::operator()(ast::CallExp& e)
  {
    try
      {
        auto t = this->sm.get(e.name_get());
        ast::FunctionDec* def = GETFUN;
        if (def == nullptr)
          {
            this->error_ << misc::error::error_type::bind << e.location_get()
                         << ": bind error, no fun " << e.name_get() << " T T\n";
          }
        std::vector<ast::Exp*> args = e.args_get();
        for (auto it = args.begin(); it != args.end(); it++)
          {
            (*this)(*it);
          }
        e.def_set(def);
      }
    catch (std::invalid_argument& ex)
      {
        this->error_ << misc::error::error_type::bind << e.location_get()
                     << ": bind error, no fun " << e.name_get() << " T T\n";
      }
  }

  void Binder::operator()(ast::RecordExp& e)
  {
    try
      {
        auto t = this->sm.get(e.type_name_get().name_get());
        ast::TypeDec* def = GETTYPE;
        if (def == nullptr)
          {
            this->error_ << misc::error::error_type::bind << e.location_get()
                         << ": bind error, undeclared type "
                         << e.type_name_get().name_get() << "\n";
          }
        ast::NameTy* nt = &(e.type_name_get());
        nt->def_set(def);
        (*this)(nt);
        std::vector<ast::FieldInit*> fields = e.fields_get();
        for (auto it = fields.begin(); it != fields.end(); it++)
          {
            (*this)(*it);
          }
      }
    catch (std::invalid_argument& er)
      {
        this->error_ << misc::error::error_type::bind << e.location_get()
                     << ": bind error, undeclared type "
                     << e.type_name_get().name_get() << "\n";
      }
  }

  void Binder::operator()(ast::NameTy& e)
  {
    if (e.name_get() == "int" || e.name_get() == "string")
      {
        return;
      }

    try
      {
        auto t = this->sm.get(e.name_get());

        ast::TypeDec* def = GETTYPE;
        if (def == nullptr)
          {
            this->error_ << misc::error::error_type::bind << e.location_get()
                         << ": bind error, undeclared type " << e.name_get()
                         << "\n";
          }

        e.def_set(def);
      }
    catch (std::invalid_argument& er)
      {
        this->error_ << misc::error::error_type::bind << e.location_get()
                     << ": bind error, undeclared type " << e.name_get()
                     << "\n";
      }
  }

  void Binder::operator()(ast::BreakExp& e)
  {
    ast::Exp* brk = break_stack.top();
    if (brk == nullptr)
      {
        this->error_ << misc::error::error_type::bind << e.location_get()
                     << ": bind error, 'break' outside any loop\n";
      }
    e.def_set(brk);
    break_stack.pop();
  }
  void Binder::operator()(ast::LetExp& e)
  {
    sm.scope_begin();
    // visit ChunkList
    for (auto& it : e.chunks_get())
      {
        (*this)(it);
      }
    // visit the body
    (*this)(e.body_get());
    sm.scope_end();
  }

} // namespace bind
