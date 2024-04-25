/**
 ** \file bind/binder.cc
 ** \brief Implementation for bind/binder.hh.
 */

#include <ast/all.hh>
#include <bind/binder.hh>
#define GETFUN std::get<ast::FunctionDec*>(t)
#define GETVAR std::get<ast::VarDec*>(t)
#define GETTYPE std::get<ast::TypeDec*>(t)
#define GET_LAST_CHUNK_FUN std::get<0>(std::get<chunk_tracker>(t))
#define GET_LAST_CHUNK_VAR std::get<1>(std::get<chunk_tracker>(t))
#define GET_LAST_CHUNK_TYPE std::get<2>(std::get<chunk_tracker>(t))

#include <cstring>
#include <misc/contract.hh>

namespace bind
{
  /*-----------------.
  | Error handling.  |
  `-----------------*/

  // to show the number of scope
  int nb_chunks = 0;

  /// The error handler.
  const misc::error& Binder::error_get() const { return error_; }

  // FIXED: Some code was deleted here.

  /*
   *     ________________________
   *    |                        |
   *    |  Visiting dec headers  |
   *    |________________________|
   */

  template <class D> void Binder::visit_dec_header(D& e) {}

  template <> void Binder::visit_dec_header<ast::TypeDec>(ast::TypeDec& e)
  {
    try
      {
        binding_tuples t = this->sm.get(e.name_get());
        if (GETTYPE != nullptr && GET_LAST_CHUNK_TYPE == nb_chunks)
          {
            this->error_ << misc::error::error_type::bind << e.location_get()
                         << ": bind error, redefinition of type "
                         << e.name_get() << "\n"
                         << GETTYPE->location_get() << ": first definition"
                         << "\n";
          }
        else
          {
            sm.put(e.name_get(),
                   {GETFUN,
                    GETVAR,
                    &e,
                    {GET_LAST_CHUNK_FUN, GET_LAST_CHUNK_VAR, nb_chunks}});
          }
      }
    catch (std::invalid_argument& er)
      {
        this->sm.put(e.name_get(), {nullptr, nullptr, &e, {0, 0, nb_chunks}});
      }
  }

  template <>
  void Binder::visit_dec_header<ast::FunctionDec>(ast::FunctionDec& e)
  {
    try
      {
        binding_tuples t = this->sm.get(e.name_get());
        if (GETFUN != nullptr && GET_LAST_CHUNK_FUN == nb_chunks)
          {
            this->error_ << misc::error::error_type::bind << e.location_get()
                         << ": bind error, redefinition of function "
                         << e.name_get() << "\n"
                         << GETFUN->location_get() << ": first definition\n";
          }
        else
          {
            sm.put(e.name_get(),
                   {&e,
                    GETVAR,
                    GETTYPE,
                    {nb_chunks, GET_LAST_CHUNK_VAR, GET_LAST_CHUNK_TYPE}});
          }
      }
    catch (std::invalid_argument& er)
      {
        this->sm.put(e.name_get(), {&e, nullptr, nullptr, {nb_chunks, 0, 0}});
      }
  }

  /*
   *     _____________________
   *    |                     |
   *    |  Visiting dec body  |
   *    |_____________________|
   */

  template <class D> void Binder::visit_dec_body(D& e) {}

  template <> void Binder::visit_dec_body<ast::TypeDec>(ast::TypeDec& e)
  {
    (*this)(e.ty_get());
  }

  template <> void Binder::visit_dec_body<ast::FunctionDec>(ast::FunctionDec& e)
  {
    this->sm.scope_begin(); // open the scope
    (*this)(e.formals_get()); // visit

    if (e.result_get() != nullptr)
      {
        (*this)(*e.result_get()); // visit
      }

    if (e.body_get() != nullptr)
      {
        (*this)(*e.body_get()); // visit
      }
    this->sm.scope_end(); // close the scope
  }

  /*
   *     _____________________
   *    |                     |
   *    |   Visiting Chunks   |
   *    |_____________________|
   */

  template <class D> void Binder::chunk_visit(ast::Chunk<D>& e)
  {
    for (const auto dec : e)
      {
        visit_dec_header<D>(*dec);
      }
    for (const auto dec : e)
      {
        visit_dec_body<D>(*dec);
      }
  }

  void Binder::operator()(ast::FunctionChunk& e)
  {
    chunk_visit<ast::FunctionDec>(e);
    nb_chunks++;
  }

  void Binder::operator()(ast::TypeChunk& e)
  {
    chunk_visit<ast::TypeDec>(e);
    nb_chunks++;
  }

  void Binder::operator()(ast::VarChunk& e)
  {
    super_type::chunk_visit<ast::VarChunk>(e);
    nb_chunks++;
  }

  /*
   *     _____________________
   *    |                     |
   *    |     Visiting Dec    |
   *    |_____________________|
   */

  void Binder::operator()(ast::FunctionDec& e) { unreachable(); }

  void Binder::operator()(ast::TypeDec& e) { unreachable(); }

  void Binder::operator()(ast::VarDec& e)
  {
    try
      {
        binding_tuples t = this->sm.get(e.name_get());
        if (GETVAR != nullptr && GET_LAST_CHUNK_VAR == nb_chunks)
          {
            this->error_ << misc::error::error_type::bind << e.location_get()
                         << ": bind error, redefinition of variable "
                         << e.name_get() << "\n"
                         << GETVAR->location_get() << ": first definition"
                         << "\n";
          }
        else
          {
            sm.put(e.name_get(),
                   {GETFUN,
                    &e,
                    GETTYPE,
                    {GET_LAST_CHUNK_FUN, nb_chunks, GET_LAST_CHUNK_TYPE}});
          }
      }
    catch (std::invalid_argument& er)
      {
        this->sm.put(e.name_get(), {nullptr, &e, nullptr, {0, nb_chunks, 0}});
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

  /*
   *     _____________________
   *    |                     |
   *    |    Visiting Exps    |
   *    |_____________________|
   */

  void Binder::operator()(ast::ForExp& e)
  {
    this->sm.scope_begin();
    (*this)(e.vardec_get());
    (*this)(e.hi_get());
    break_stack.push(&e);
    (*this)(e.body_get());
    break_stack.pop();
    this->sm.scope_end();
  }

  void Binder::operator()(ast::WhileExp& e)
  {
    (*this)(e.test_get());
    break_stack.push(&e);
    (*this)(e.body_get());
    break_stack.pop();
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
    if (break_stack.size() == 0)
      {
        this->error_ << misc::error::error_type::bind << e.location_get()
                     << ": bind error, 'break' outside any loop\n";
        return;
      }
    ast::Exp* brk = break_stack.top();
    e.def_set(brk);
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
