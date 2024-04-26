* README

Tiger Abstract Syntax Tree nodes with their principal members.
Incomplete classes are tagged with a `*'.

/Ast/               (Location location)
  /Dec/             (symbol name)
                      the dec class which contains a name as a symbol
    FunctionDec     (VarChunk formals, NameTy result, Exp body)
                      contains the name of the function as a symbol,
                      the arguments as a Chunk of VarDec,
                      the type of the result as a NameTy*,
                      the body of the function as the pointer to an Exp
      MethodDec     ()
                      extends the fundctiondec
    TypeDec         (Ty ty)
    VarDec          (NameTy type_name, Exp init)

  /Exp/             ()
    /Var/           ()
      FieldVar      (Var var, symbol name)
      SimpleVar     (symbol name)
                      contains the name of the variable as a symbol
                      and a pointer to its declaration's node
      SubscriptVar  (Var var, Exp index)

    ArrayExp        (NameTy type_name, Exp size, Exp init)
    AssignExp       (Var var, Exp exp)
    BreakExp        ()
    CallExp         (symbol name, exps_type args)
      MethodCallExp (Var object)
    CastExp         (Exp exp, Ty ty)
    ForExp          (VarDec vardec, Exp hi, Exp body)
    IfExp           (Exp test, Exp thenclause, Exp elseclause)
                      contains three Exp* for the test,
                      the thenclause
                      and the elseclause
    IntExp          (int value)
                      container with an int inside
    LetExp          (ChunkList chunks, Exp body)
    NilExp          ()
    ObjectExp       (NameTy type_name)
    OpExp           (Exp left, Oper oper, Exp right)
    RecordExp       (NameTy type_name, fieldinits_type fields)
    SeqExp          (exps_type exps)
    StringExp       (string value)
                      container with a string inside
    WhileExp        (Exp test, Exp body)

  /Ty/              ()
    ArrayTy         (NameTy base_type)
    ClassTy         (NameTy super, ChunkList chunks)
    NameTy          (symbol name)
    RecordTy        (fields_type fields)

  ChunkList         (list_type chunks)

  Field             (symbol name, NameTy type_name)

  FieldInit         (symbol name, Exp init)


Some of these classes also inherit from other classes.

/Escapable/
  VarDec            (NameTy type_name, Exp init)

/Typable/
  /Dec/             (symbol name)
  /Exp/             ()
  /Ty/              ()

/TypeConstructor/
  /Ty/              ()
  FunctionDec       (VarChunk formals, NameTy result, Exp body)
  NilExp            ()
  TypeDec           (Ty ty)

Extra doc:

    Chunk<T>        contains a vector of T*,
                    can be directly iterated over
