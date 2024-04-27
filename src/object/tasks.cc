/**
 ** \file object/tasks.cc
 ** \brief Object module related tasks' implementation.
 */

#include <memory>
#include <ast/tasks.hh>
#include <astclone/libastclone.hh>
#include <bind/libbind.hh>
#include <common.hh>
#include <misc/error.hh>
#include <object/libobject.hh>
#define DEFINE_TASKS 1
#include <object/tasks.hh>
#undef DEFINE_TASKS

namespace object::tasks
{
  void object_parse() {}

  // FIXED: Some code was deleted here.
  void object_bindings_compute()
  {
    misc::error result = bind::bind(*ast::tasks::the_program);

    task_error() << result;
    if (!result)
      task_error().exit();
  }

  void object_types_compute()
  {
    task_error() << ::object::types_check(*ast::tasks::the_program)
                 << &misc::error::exit_on_error;
  }

  static std::unique_ptr<class_names_type> class_names;

  void object_rename()
  {
    class_names.reset(::object::rename(*ast::tasks::the_program));
  }

  void object_desugar()
  {
    astclone::apply(::object::desugar, ast::tasks::the_program,
                    *class_names.get());
  }

  void raw_object_desugar()
  {
    astclone::apply(::object::raw_desugar, ast::tasks::the_program,
                    *class_names.get());
  }

} // namespace object::tasks
