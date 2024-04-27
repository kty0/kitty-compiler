/**
 ** \file type/type-checker.cc
 ** \brief Implementation for type/type-checker.hh.
 */

#include <memory>
#include <ranges>

#include <ast/all.hh>
#include <type/type-checker.hh>
#include <type/types.hh>

namespace type
{
  TypeChecker::TypeChecker()
    : super_type()
    , error_()
  {}

  const Type* TypeChecker::type(ast::Typable& e)
  {
    // FIXED: Some code was deleted here.
    if (e.type_get() != nullptr)
      {
        return e.type_get();
      }

    e.accept(*this);
    if (e.type_get() == nullptr)
      {
        e.type_set(&Int::instance());
      }
    return e.type_get();
  }

  const Record* TypeChecker::type(const ast::fields_type& e)
  {
    auto res = new Record;
    // FIXED: Some code was deleted here.
    for (const auto& field : e)
      res->field_add(field->name_get(), *type(field->type_name_get()));
    return res;
  }

  const Record* TypeChecker::type(const ast::VarChunk& e)
  {
    auto res = new Record;
    for (const auto& var : e)
      res->field_add(var->name_get(), *type(*var));

    return res;
  }

  const misc::error& TypeChecker::error_get() const { return error_; }

  /*-----------------.
  | Error handling.  |
  `-----------------*/

  void TypeChecker::error(const ast::Ast& ast, const std::string& msg)
  {
    error_ << misc::error::error_type::type << ast.location_get() << ": " << msg
           << std::endl;
  }

  void TypeChecker::type_mismatch(const ast::Ast& ast,
                                  const std::string& exp1,
                                  const Type& type1,
                                  const std::string& exp2,
                                  const Type& type2)
  {
    error_ << misc::error::error_type::type << ast.location_get()
           << ": type mismatch" << misc::incendl << exp1 << " type: " << type1
           << misc::iendl << exp2 << " type: " << type2 << misc::decendl;
  }

  void TypeChecker::check_types(const ast::Ast& ast,
                                const std::string& exp1,
                                const Type& type1,
                                const std::string& exp2,
                                const Type& type2)
  {
    // FIXED: Some code was deleted here (Check for type mismatch).
    if (!type1.actual().compatible_with(type2.actual()))
      {
        type_mismatch(ast, exp1, type1, exp2, type2);
      }

    // If any of the type is Nil, set its `record_type_` to the other type.
    if (!error_)
      {
        // FIXED: Some code was deleted here.
        const Nil* first = dynamic_cast<const Nil*>(&type1);
        const Nil* second = dynamic_cast<const Nil*>(&type2);

        if (first != nullptr && second == nullptr)
          {
            first->record_type_set(type2);
          }
        else if (first == nullptr && second != nullptr)
          {
            second->record_type_set(type1);
          }
      }
  }

  void TypeChecker::check_types(const ast::Ast& ast,
                                const std::string& exp1,
                                ast::Typable& type1,
                                const std::string& exp2,
                                ast::Typable& type2)
  {
    // Ensure evaluation order.
    type(type1);
    type(type2);
    // FIXED: Some code was deleted here (Check types).
    check_types(ast, exp1, *type1.type_get(), exp2, *type2.type_get());
  }

  /*--------------------------.
  | The core of the visitor.  |
  `--------------------------*/

  /*-----------------.
  | Visiting /Var/.  |
  `-----------------*/

  void TypeChecker::operator()(ast::SimpleVar& e)
  {
    // FIXED: Some code was deleted here.
    e.type_set(e.def_get()->type_get());
  }

  // FIXED: Some code was deleted here.

  void TypeChecker::operator()(ast::FieldVar& e)
  {
    const type::Record* record_type =
      dynamic_cast<const type::Record*>(&type(e.var_get())->actual());
    if (record_type == nullptr)
      {
        error(e, "var must be a record");
        type_default(e, &Int::instance());
        return;
      }

    const Type* field_type = record_type->field_type(e.name_get());
    if (field_type == nullptr)
      {
        error(e, "unknown field");
        type_default(e, &Int::instance());
        return;
      }

    type_default(e, field_type);
  }

  void TypeChecker::operator()(ast::SubscriptVar& e)
  {
    check_types(e, "index", *type(e.index_get()), "expected", Int::instance());

    const type::Array* array_type =
      dynamic_cast<const type::Array*>(&type(e.var_get())->actual());
    if (array_type == nullptr)
      {
        error(e, "var must be an array");
        type_default(e, &Int::instance());
        return;
      }

    type_default(e, &array_type->type_get());
  }

  /*-----------------.
  | Visiting /Exp/.  |
  `-----------------*/

  // Literals.
  void TypeChecker::operator()(ast::NilExp& e)
  {
    auto nil_ptr = std::make_unique<Nil>();
    type_default(e, nil_ptr.get());
    created_type_default(e, nil_ptr.release());
  }

  void TypeChecker::operator()(ast::IntExp& e)
  {
    // FIXED: Some code was deleted here.
    type_default(e, &Int::instance());
  }

  void TypeChecker::operator()(ast::StringExp& e)
  {
    // FIXED: Some code was deleted here.
    type_default(e, &String::instance());
  }

  // Complex values.
  void TypeChecker::operator()(ast::RecordExp& e)
  {
    // FIXED: Some code was deleted here.
    const type::Record* record_type =
      dynamic_cast<const type::Record*>(&type(e.type_name_get())->actual());
    if (record_type == nullptr)
      {
        error(e, "type should be a record");
        type_default(e, &Int::instance());
        return;
      }

    auto fields = e.fields_get();

    if (fields.size() != record_type->fields_get().size())
      {
        error(e, "invalid RecordExp");
      }

    for (size_t i = 0; i < fields.size(); i++)
      {
        check_types(e, "field", *type(fields[i]->init_get()), "expected",
                    record_type->fields_get()[i].type_get());
      }

    type_default(e, &e.type_name_get().type_get()->actual());
  }

  void TypeChecker::operator()(ast::OpExp& e)
  {
    // FIXED: Some code was deleted here.
    ast::OpExp::Oper op = e.oper_get();

    if (op == ast::OpExp::Oper::add || op == ast::OpExp::Oper::sub
        || op == ast::OpExp::Oper::mul
        || op == ast::OpExp::Oper::div) // Arithmetic operations
      {
        check_types(e, "left operand", *type(e.left_get()), "expected",
                    Int::instance());
        check_types(e, "right operand", *type(e.right_get()), "expected",
                    Int::instance());
      }
    else // Comparisons
      {
        check_types(e, "left operand", e.left_get(), "right operand",
                    e.right_get());
      }

    type_default(e, &Int::instance());
  }

  // FIXED: Some code was deleted here.

  void TypeChecker::operator()(ast::ArrayExp& e)
  {
    check_types(e, "size clause", *type(e.size_get()), "expected",
                Int::instance());

    const Array* array_type =
      dynamic_cast<const Array*>(&type(e.type_name_get())->actual());
    if (array_type == nullptr)
      {
        error(e, "expected array type");
        type_default(e, &Int::instance());
        return;
      }

    check_types(e, "array", array_type->type_get(), "initializing",
                *type(e.init_get()));

    type_default(e, e.type_name_get().type_get());
  }

  void TypeChecker::operator()(ast::ForExp& e)
  {
    e.vardec_get().read_only_set(true);

    check_types(e, "counter", *type(e.vardec_get()), "expected",
                Int::instance());

    check_types(e, "bound", *type(e.hi_get()), "expected", Int::instance());

    check_types(e, "body", *type(e.body_get()), "expected", Void::instance());

    type_default(e, &Void::instance());
  }

  void TypeChecker::operator()(ast::AssignExp& e)
  {
    ast::SimpleVar* simplevar = dynamic_cast<ast::SimpleVar*>(&e.var_get());
    if (simplevar && simplevar->def_get()->read_only_get())
      {
        error(e, "variable is read only");
        type_default(e, &Int::instance());
        return;
      }

    check_types(e, "variable", *type(e.var_get()), "expression",
                *type(e.exp_get()));

    type_default(e, &Void::instance());
  }

  void TypeChecker::operator()(ast::LetExp& e)
  {
    (*this)(e.chunks_get());
    e.type_set(type(e.body_get()));
  }

  void TypeChecker::operator()(ast::IfExp& e)
  {
    check_types(e, "test clause", *type(e.test_get()), "expected",
                Int::instance());

    check_types(e, "then clause", e.thenclause_get(), "else clause",
                e.elseclause_get());

    e.type_set(e.thenclause_get().type_get());
  }

  void TypeChecker::operator()(ast::CallExp& e)
  {
    if (e.def_get() == nullptr)
      {
        return;
      }

    const Function* function =
      dynamic_cast<const Function*>(e.def_get()->type_get());
    if (function == nullptr)
      {
        error(e, "missing function type");
        type_default(e, &Int::instance());
      }

    std::vector<Field> fields = function->formals_get().fields_get();
    ast::exps_type exps = e.args_get();

    if (fields.size() != exps.size())
      {
        error(e, "more formals than given");
        type_default(e, &Int::instance());
        return;
      }

    for (size_t i = 0; i < fields.size(); i++)
      {
        check_types(e, "argument", *type(*exps[i]), "expected",
                    fields[i].type_get());
      }

    type_default(e, &function->result_get());
  }

  void TypeChecker::operator()(ast::SeqExp& e)
  {
    std::vector<ast::Exp*> exps = e.exps_get();
    if (exps.size() == 0)
      {
        type_default(e, &Void::instance());
        return;
      }

    const type::Type* exp_type;
    for (auto it = exps.begin(); it != exps.end(); it++)
      {
        exp_type = type(**it);
      }

    type_default(e, exp_type);
  }

  void TypeChecker::operator()(ast::BreakExp& e)
  {
    type_default(e, &Void::instance());
  }

  void TypeChecker::operator()(ast::CastExp& e)
  {
    type_default(e, type(e.ty_get()));
  }

  void TypeChecker::operator()(ast::WhileExp& e)
  {
    check_types(e, "test clause", *type(e.test_get()), "expected",
                Int::instance());

    check_types(e, "body", *type(e.body_get()), "expected", Void::instance());

    type_default(e, &Void::instance());
  }

  /*-----------------.
  | Visiting /Dec/.  |
  `-----------------*/

  /*------------------------.
  | Visiting FunctionChunk. |
  `------------------------*/

  void TypeChecker::operator()(ast::FunctionChunk& e)
  {
    chunk_visit<ast::FunctionDec>(e);
  }

  void TypeChecker::operator()(ast::FunctionDec&)
  {
    // We must not be here.
    unreachable();
  }

  // Store the type of this function.
  template <>
  void TypeChecker::visit_dec_header<ast::FunctionDec>(ast::FunctionDec& e)
  {
    // FIXED: Some code was deleted here.
    const Type* result =
      e.result_get() == nullptr ? &Void::instance() : type(*e.result_get());
    Function* function = new Function(type(e.formals_get()), result);
    type_default(e, function);
    created_type_default(e, function);
  }

  // Type check this function's body.
  template <>
  void TypeChecker::visit_dec_body<ast::FunctionDec>(ast::FunctionDec& e)
  {
    if (e.body_get())
      visit_routine_body<Function>(e);
  }

  /*---------------.
  | Visit VarDec.  |
  `---------------*/

  void TypeChecker::operator()(ast::VarDec& e)
  {
    // FIXED: Some code was deleted here.
    if (e.type_name_get() != nullptr)
      {
        type_default(e, type(*e.type_name_get()));
      }

    if (e.init_get() == nullptr)
      {
        return;
      }

    const type::Type* exp_type = type(*e.init_get());

    if (e.type_name_get() == nullptr)
      {
        if (dynamic_cast<const Nil*>(exp_type))
          {
            error(e, "assigning nil to untyped var");
          }

        type_default(e, exp_type);
      }
    else
      {
        check_types(e, "type", *e.type_get(), "expression", *exp_type);
      }
  }

  /*--------------------.
  | Visiting TypeChunk. |
  `--------------------*/

  void TypeChecker::operator()(ast::TypeChunk& e)
  {
    chunk_visit<ast::TypeDec>(e);
  }

  void TypeChecker::operator()(ast::TypeDec&)
  {
    // We must not be here.
    unreachable();
  }

  // Store this type.
  template <> void TypeChecker::visit_dec_header<ast::TypeDec>(ast::TypeDec& e)
  {
    // We only process the head of the type declaration, to set its
    // name in E.  A declaration has no type in itself; here we store
    // the type declared by E.
    // FIXED: Some code was deleted here.
    type::Type* name_type = new Named(e.name_get());
    type_default(e, name_type);
  }

  // Bind the type body to its name.
  template <> void TypeChecker::visit_dec_body<ast::TypeDec>(ast::TypeDec& e)
  {
    // FIXED: Some code was deleted here.
    const Named* named_type = dynamic_cast<const Named*>(e.type_get());
    if (named_type == nullptr)
      {
        error(e, "TypeDec ill-typed somehow");
        e.type_set(&Int::instance());
        return;
      }

    named_type->type_set(type(e.ty_get()));
  }

  /*------------------.
  | Visiting /Chunk/. |
  `------------------*/

  template <class D> void TypeChecker::chunk_visit(ast::Chunk<D>& e)
  {
    // FIXED: Some code was deleted here.
    for (const auto dec : e)
      {
        visit_dec_header<D>(*dec);
      }
    for (const auto dec : e)
      {
        visit_dec_body<D>(*dec);
      }
    for (const auto dec : e)
      {
        const Named* named_type = dynamic_cast<const Named*>(dec->type_get());
        if (named_type != nullptr && !named_type->sound())
          {
            error(e, "recursive inheritance");
            named_type->type_set(&Int::instance());
            return;
          }
      }
  }

  /*-------------.
  | Visit /Ty/.  |
  `-------------*/

  void TypeChecker::operator()(ast::NameTy& e)
  {
    // FIXED: Some code was deleted here (Recognize user defined types, and built-in types).
    const type::Type* ty;
    if (e.def_get() == nullptr)
      {
        misc::symbol name = e.name_get();
        if (name == "int")
          {
            ty = &Int::instance();
          }
        else if (name == "string")
          {
            ty = &String::instance();
          }
        else
          {
            ty = &Void::instance();
          }
      }
    else
      {
        ty = e.def_get()->type_get();
      }

    type_default(e, ty);
    created_type_default(e, ty);
  }

  void TypeChecker::operator()(ast::RecordTy& e)
  {
    // FIXED: Some code was deleted here.
    std::set<std::string> names;

    for (auto it = e.fields_get().begin(); it != e.fields_get().end(); it++)
      {
        if (names.contains((*it)->name_get()))
          {
            error(e, "repetitive record");
            type_default(e, &Int::instance());
            return;
          }
        else
          {
            names.insert((*it)->name_get().get());
          }
      }

    const Record* record_ptr = type(e.fields_get());

    type_default(e, record_ptr);
    created_type_default(e, record_ptr);
  }

  void TypeChecker::operator()(ast::ArrayTy& e)
  {
    // FIXED: Some code was deleted here.
    auto array_ptr = new Array(*type(e.base_type_get()));

    type_default(e, array_ptr);
    created_type_default(e, array_ptr);
  }

} // namespace type
