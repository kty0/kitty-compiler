/**
 ** \file bind/tasks.hh
 ** \brief Bind module related tasks.
 */

// FIXED: Some code was deleted here.
#pragma once

#include <task/libtask.hh>

namespace bind::tasks
{
  TASK_GROUP("4. Binding");

  TASK_DECLARE("b|bindings-compute",
               "compute the bindings in the AST",
               bindings_compute,
               "parse");

  TASK_DECLARE("B|bindings-display",
               "display the result of the bindings in the pretty printer",
               bindings_display,
               "bound");

  DISJUNCTIVE_TASK_DECLARE("bound",
                           "chooses whether to launch object or normal binding",
                           "bindings-compute object-bindings-compute");

  TASK_DECLARE("rename",
               "rename the identifiers so that they are all unique",
               rename,
               "bound");

} //namespace bind::tasks
