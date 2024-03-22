/**
 ** \file tc.cc
 ** \brief The compiler driver.
 */

#include <cstdlib>
#include <exception>
#include <iostream>
#include <stdexcept>
#include <common.hh>

#include <task/task-register.hh>

int main(int argc, char** argv)
{
  program_name = argv[0];

  try
    {
      task_timer.start();
      task_timer.push("rest");

      filename = task::TaskRegister::instance().parse_arg(argc, argv);
      task_error().exit_on_error();

      // If `help', `usage' or `version' is called, just exit.
      if (filename == nullptr)
        return 0;

      if (task::TaskRegister::instance().nb_of_task_to_execute_get() == 0)
        task::TaskRegister::instance().enable_task("parse");

      task::TaskRegister::instance().execute();
      task_timer << task::TaskRegister::instance().timer_get();
      task_error().exit_on_error();
    }

  // Required to enable stack unwinding.
  catch (const std::invalid_argument& e)
    {
      std::cout << "e1\n";
      return 64;
    }
  catch (const std::runtime_error& e)
    {
      std::cout << "e2\n";
      if (e.what() != std::string(""))
        std::cerr << e.what() << '\n';
    }
  catch (const misc::error& e)
    {
      std::cout << "e3\n";
      std::cerr << e;
      return e.status_get_value();
    }
}
