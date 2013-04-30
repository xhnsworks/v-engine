%pure-parser
%lex-param {ParserEnv * e}
%parse-param {ParserEnv* e}

%{
#include "common.h"
#include "etypes.h"
#include "parser_env.h"
#include "array.h"
#define YYSTYPE SymbolValue

int yylex   (SymbolValue *lvalp, ParserEnv* e);
void yyerror(ParserEnv* e, const char* error);

#define EXCE_TEST { if (array_n(e->exce_array)) {YYERROR;} }

SymbolValue AddProc(ParserEnv* e, SymbolValue a, SymbolValue b);
SymbolValue SubProc(ParserEnv* e, SymbolValue a, SymbolValue b);
SymbolValue MulProc(ParserEnv* e, SymbolValue a, SymbolValue b);
SymbolValue DivProc(ParserEnv* e, SymbolValue a, SymbolValue b);
SymbolValue AssignProc(ParserEnv* e, SymbolValue dst, SymbolValue src);
SymbolValue InitAssignProc(ParserEnv* e, SymbolValue dst, SymbolValue src);

SymbolValue LessProc(ParserEnv* e, SymbolValue a, SymbolValue b);
SymbolValue GreaterProc(ParserEnv* e, SymbolValue a, SymbolValue b);
SymbolValue EqualProc(ParserEnv* e, SymbolValue a, SymbolValue b);
SymbolValue LessEqualProc(ParserEnv* e, SymbolValue a, SymbolValue b);
SymbolValue GreaterEqualProc(ParserEnv* e, SymbolValue a, SymbolValue b);

/// if分支
SymbolValue MarkIf(ParserEnv* e, SymbolValue bool_value);
SymbolValue MarkElseIf(ParserEnv* e, SymbolValue bool_value);
SymbolValue MarkElse(ParserEnv* e);
void MarkEnd(ParserEnv* e);

/// for循环
void ForBegin(ParserEnv* e);
void ForEnd(ParserEnv* e);
void MarkForCondBegin(ParserEnv* e);
void MarkForCond(ParserEnv* e, SymbolValue bool_value);
void MarkForAlways(ParserEnv* e);
void ForInc(ParserEnv* e);

SymbolValue PrintProc(ParserEnv* e, SymbolValue value);

void NewStruct(ParserEnv* e, SymbolValue struct_name);
void MarkStructMember(ParserEnv* e, SymbolValue struct_member, SymbolValue member_type);
void CompleteStruct(ParserEnv* e);

/// 函数相关
void BeginFuncDef(ParserEnv* e, SymbolValue _func_name);
void AddParam(ParserEnv* e, SymbolValue _sym, SymbolValue valuetype);
void CompleteParamDef(ParserEnv* e);
void CompleteOutputDef(ParserEnv* e);
void CompleteFuncDef(ParserEnv* e);
SymbolValue FuncCall(ParserEnv* e, SymbolValue func);
SymbolValue FuncCall2(ParserEnv* e, SymbolValue func, SymbolValue a);
/// 数值组
SymbolValue NewValueArray(ParserEnv* e, SymbolValue v);
SymbolValue ValueArrayPush(ParserEnv* e, SymbolValue dst, SymbolValue v);
/// 变量相关
SymbolValue FloatTypeSym();
SymbolValue IntTypeSym();
SymbolValue VarDef(ParserEnv* e, SymbolValue sym, SymbolValue var_type);

void PrintSymbolValue(ParserEnv* e, const char* str, SymbolValue value);
%}

%error-verbose

%token left_brace
%token right_brace

%token left_bracket
%token right_bracket

%left left_parenthese
%token right_parenthese

%token _sym
%token _float
%token _int
%token _numerical
%token _text

%token _struct
%token _dot

%token float_type
%token int_type

%left comma

%left _if
%token _else

%token _for

%token _var

%token _lightdef
%token _structdef
%token _funcdef

%token line_end
%right _equal
%right _less
%right _greater
%right _less_equal
%right _greater_equal

%right _assign
%right _init_assign
%right _add
%right _sub
%left _mul
%left _div

%token _print
%token _function

%token _risk
%token empty

%%
/// 程序定义
program : func_def
        | struct_def
        | program func_def
        | program struct_def
        ;

/// 结构体定义
struct_def : struct_def_begin left_brace struct_body_def struct_def_end
           ;
struct_def_begin : _struct _sym                    { NewStruct(e, $2); EXCE_TEST; }
                 ;
struct_body_def : _var _sym valuetype line_end                 { MarkStructMember(e, $2, $3); EXCE_TEST; }
                | struct_body_def _var _sym valuetype line_end { MarkStructMember(e, $3, $4); EXCE_TEST; }
                ;
struct_def_end : right_brace                       { CompleteStruct(e); EXCE_TEST; }
               ;

/// 函数定义
func_def : func_head_def left_brace lines right_brace { CompleteFuncDef(e); EXCE_TEST; }
         ;

func_head_def : func_def_begin param_set_def output_set_def
              | func_def_begin param_set_def                { CompleteOutputDef(e); EXCE_TEST; }
              ;

func_def_begin : _funcdef _sym { BeginFuncDef(e, $2); };

param_set_def : left_parenthese params right_parenthese { CompleteParamDef(e); EXCE_TEST; };

output_set_def : left_parenthese params right_parenthese { CompleteOutputDef(e); EXCE_TEST; };

params : _sym valuetype comma params { AddParam(e, $1, $2); EXCE_TEST; }
       | _sym valuetype              { AddParam(e, $1, $2); EXCE_TEST; }
       ;

/// 变量定义
var_def : _var _sym valuetype   { $$ = VarDef(e, $2, $3); EXCE_TEST; }
       | _var _sym _assign expr
       ;

/// if分支块定义
half_if_block  : prev_if_def                       lines block_end
               | half_if_block     else_if_def     lines block_end
               ;
full_if_block : half_if_block      post_if_def     lines block_end
              ;

block_end : right_brace { MarkEnd(e); EXCE_TEST; }
          ;

prev_if_def  : _if       left_parenthese expr right_parenthese left_brace  { $$ = MarkIf(e, $3); EXCE_TEST; }
             ;
else_if_def  : _else _if left_parenthese expr right_parenthese left_brace  { $$ = MarkElseIf(e, $4); EXCE_TEST; }
             ;
post_if_def  : _else                                           left_brace  { $$ = MarkElse(e); EXCE_TEST; }
             ;
/// for循环块定义
for_loop_init : line_end      { MarkForCondBegin(e); EXCE_TEST; }
              | expr line_end { MarkForCondBegin(e); EXCE_TEST; }
              ;
for_loop_cond : line_end      { MarkForAlways(e); EXCE_TEST; }
              | expr line_end { MarkForCond(e, $1); EXCE_TEST; }
              ;
for_loop_inc : line_end
             | expr line_end  { ForInc(e); EXCE_TEST; }
             ;

for_begin : _for { ForBegin(e); EXCE_TEST; }
        ;

for_end : right_brace { ForEnd(e); EXCE_TEST; }
        ;

for_loop : for_begin left_parenthese for_loop_init for_loop_cond for_loop_inc right_parenthese left_brace lines for_end
         ;

/// 函数的代码行
lines : expr line_end       {}
      | var_def line_end
      | print_output line_end
      | half_if_block
      | full_if_block
      | for_loop
      | lines expr line_end {}
      | lines half_if_block
      | lines full_if_block
      | lines var_def line_end
      | lines print_output line_end
     ;

valuetype : float_type { $$ = FloatTypeSym(); EXCE_TEST; }
          | int_type   { $$ = IntTypeSym(); EXCE_TEST; }
          ;

value : _float
      | _int
      | _sym
      | _text
      | _sym left_parenthese right_parenthese
      | _sym left_parenthese args right_parenthese
      ;

args : value             { $$ = NewValueArray(e, $1); EXCE_TEST; }
     | args comma value  { $$ = ValueArrayPush(e, $1, $3); EXCE_TEST;  }
     ;

expr : expr _add expr   { $$ = AddProc(e, $1, $3); EXCE_TEST; }
     | expr _sub expr   { $$ = SubProc(e, $1, $3); EXCE_TEST; }
     | expr _mul expr   { $$ = MulProc(e, $1, $3); EXCE_TEST; }
     | expr _div expr   { $$ = DivProc(e, $1, $3); EXCE_TEST; }
     | value            { $$ = $1; EXCE_TEST; }
     | _function left_parenthese right_parenthese      { $$ = FuncCall(e, $1);      PrintSymbolValue(e, "FUNC CALL: ", $$); EXCE_TEST; }
     | _function left_parenthese args right_parenthese { $$ = FuncCall2(e, $1, $3); PrintSymbolValue(e, "FUNC CALL: ", $$); EXCE_TEST; }
     | left_parenthese expr right_parenthese           { $$ = $2; }
     | args _assign expr                               { $$ = AssignProc(e, $1, $3); PrintSymbolValue(e, "ASSIGN DST: ", $1); PrintSymbolValue(e, "ASSIGN DST: ", $3); EXCE_TEST; }
     | _sym _init_assign expr                          { $$ = InitAssignProc(e, $1, $3); EXCE_TEST; }
     | expr _equal expr                                { $$ = EqualProc(e, $1, $3); EXCE_TEST; }
     | expr _less expr                                 { $$ = LessProc(e, $1, $3); EXCE_TEST; }
     | expr _greater expr                              { $$ = GreaterProc(e, $1, $3); EXCE_TEST; }
     | expr _less_equal expr                           { $$ = LessEqualProc(e, $1, $3); EXCE_TEST; }
     | expr _greater_equal expr                        { $$ = GreaterEqualProc(e, $1, $3); EXCE_TEST; }
     ;

print_output : _print expr { $$ = PrintProc(e, $2); EXCE_TEST; };

%%
