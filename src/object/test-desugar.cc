/// Checking the removal of object constructs.

#include <cstdlib>
#include <iostream>

#include <ast/chunk-list.hh>
#include <ast/exp.hh>
#include <ast/libast.hh>
#include <bind/libbind.hh>
#include <object/desugar-visitor.hh>
#include <object/libobject.hh>
#include <parse/libparse.hh>
#include <type/libtype.hh>

static void test_desugar(const std::string& s)
{
  // Parse, bind and type-check the input.
  ast::Ast* tree = nullptr;
  object::class_names_type* class_names = nullptr;
  try
    {
      tree = parse::parse_unit(s, true);
      std::cout << "/* === Original tree...  */\n" << *tree << '\n';
      // bye bye little unimplemented line
      //object::bind(*tree).exit_on_error();
      object::types_check(*tree).exit_on_error();
      class_names = object::rename(*tree);
      std::cout << "/* === Renamed tree...  */\n" << *tree << '\n';
    }
  catch (const misc::error& error)
    {
      std::cerr << "error-->" << error << '\n';
      exit(error.status_get_value());
    }

  // Desugar the tree.
  object::DesugarVisitor desugar(*class_names);
  desugar(tree);
  delete tree;
  tree = nullptr;
  ast::Ast* desugared = desugar.result_get();
  std::cout << "/* === Desugared tree...  */\n" << *desugared << '\n';

  // Compute the bindings and the types on the desugared tree.
  try
    {
      bind::bind(*desugared).exit_on_error();
      type::types_check(*desugared).exit_on_error();
    }
  catch (const misc::error& error)
    {
      std::cerr << "error-->" << error << '\n';
      exit(error.status_get_value());
    }

  delete desugared;
  delete class_names;
}

const char* program_name = "test-desugar";
const char* test_1 =
  R"( let
   type A = class
   {
     var s := "Hello"
     method m() = ()
   }

   var a : A := new A
 in
   a.s;
   a.m()
 end)";

const char* test_2 =
  R"( let
   primitive print(s : string)

   class A
   {
     var s := "Hello"
     method m() = (print(self.s); print("\n"))
   }
   class B extends A
   {
     var s2 := " World!"
     method m() = (print(self.s); print(self.s2);
                   print("\n"))
   }

   var b : B := new B
   var a : A := b
   var a2 : A := new B
 in
   b.m();
   a.m();
   a2.m();
   a := a2;
   a := b
 end)";

const char* test_3 =
  R"( let
   class A {}
   class B extends A {}
   function id(a : A) : A = a
   var b := new B
   var a := id(b)
 in
 end)";

int main()
{
  std::cout << "/* Test 1.  */\n";
  test_desugar(test_1);
  std::cout << "\n\n"
            << "/* Test 2.  */\n";
  test_desugar(test_2);
  std::cout << "\n\n"
            << "/* Test 3.  */\n";
  test_desugar(test_3);
}
