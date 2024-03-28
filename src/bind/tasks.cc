/**
 ** \file bind/tasks.cc
 ** \brief Bind module tasks implementation.
 */

#include <ast/libast.hh>
#include <ast/tasks.hh>
#include <bind/libbind.hh>
#include <common.hh>
#include <misc/error.hh>
#define DEFINE_TASKS 1
#include <bind/tasks.hh>
#undef DEFINE_TASKS

// FIXED: Some code was deleted here.
namespace bind::tasks
{
  void bindings_compute()
  {
    misc::error result = bind::bind(*ast::tasks::the_program);

    task_error() << result;
    if (!result)
      task_error().exit();
  }

  void bindings_display() { ast::bindings_display(std::cout) = true; }
} // namespace bind::tasks
