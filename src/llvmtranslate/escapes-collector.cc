#include <ast/all.hh>
#include <ast/default-visitor.hh>
#include <ast/non-object-visitor.hh>
#include <llvmtranslate/escapes-collector.hh>

namespace llvmtranslate
{
  /// LLVM IR doesn't support static link and nested functions.
  /// In order to translate those functions to LLVM IR, we use a technique
  /// called Lambda Lifting, which consists in passing a pointer to
  /// the escaped variables to the nested function using that variable.
  ///
  /// In order to do that, we need a visitor to collect these kind of
  /// variables and associate them to each function.

  class EscapesCollector
    : public ast::DefaultConstVisitor
    , public ast::NonObjectConstVisitor
  {
  public:
    /// Super class.
    using super_type = ast::DefaultConstVisitor;
    /// Import overloaded operator() methods.
    using super_type::operator();

    EscapesCollector()
      : did_modify_{false}
      , escaped_{}
    {}

    escaped_map_type& escaped_get() { return escaped_; }

    void operator()(const ast::FunctionChunk& e) override
    {
      bool saved_did_modify = did_modify_;

      // Iterate on the chunk in order to iteratively collect all the callee
      // functions' escaped variables.
      did_modify_ = !e.empty();
      while (did_modify_)
        {
          did_modify_ = false;
          super_type::operator()(e);
        }

      did_modify_ = saved_did_modify;
    }

    void operator()(const ast::FunctionDec& e) override
    {
      // Keep track of the current function
      // FIXED: Some code was deleted here.
      auto previous_function = current_function_;
      current_function_ = &e;
      super_type::operator()(e);
      current_function_ = previous_function;
    }

    void operator()(const ast::CallExp& e) override
    {
      super_type::operator()(e);

      // FIXED: Some code was deleted here.
      auto call_func =
        dynamic_cast<const type::Function*>(e.def_get()->type_get());

      if (escaped_.contains(call_func)) // if the function is found in the map
        {
          const type::Function* function =
            dynamic_cast<const type::Function*>(current_function_->type_get());

          // Check whether there are any newly collected escaped variables.
          // If there are, mark the iteration as modified.
          // FIXED: Some code was deleted here.
          for (auto& esc : escaped_[call_func])
            {
              /*
               * if the escaped variable used in the function of the callexp
               * has the same definition site as the address as the current function ?
               * YES : do nothing
               * NO : add the current function and the variable in escaped_ and
               *      mark the iteration as modified
               */
              if (esc->def_site_get() != current_function_)
                {
                  if (escaped_[function].contains(esc))
                  {
                    continue;
                  }

                  escaped_[function].insert(esc);
                  did_modify_ = true;
                }
            }
        }
    }

    void operator()(const ast::SimpleVar& e) override
    {
      // Associate escaped variables declared in parent frames with their
      // functions
      // FIXED: Some code was deleted here.
      const type::Function* function =
        dynamic_cast<const type::Function*>(current_function_->type_get());

      if (e.def_get()->is_escaped())
        {
          escaped_[function].insert(e.def_get());
        }
    }

  private:
    /// Whether any modification was done during the iteration.
    bool did_modify_ = false;

    /// Associate a set of variables with their function.
    escaped_map_type escaped_;

    /// Current visiting function.
    // FIXED: Some code was deleted here.
    const ast::FunctionDec* current_function_ = nullptr;
  };

  escaped_map_type collect_escapes(const ast::Ast& ast)
  {
    EscapesCollector collect;
    collect(ast);

    return std::move(collect.escaped_get());
  }

} // namespace llvmtranslate
