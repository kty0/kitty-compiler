/**
 ** \file object/tasks.cc
 ** \brief Object module related tasks' implementation.
 */

#include <ast/tasks.hh>
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

} // namespace object::tasks
