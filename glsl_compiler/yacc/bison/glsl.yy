%pure-parser
%lex-param {GLSLParserEnv * e}
%parse-param {GLSLParserEnv* e}

%{
#include "glsl_lex.h"

#define YYSTYPE GLSLSymbolValue

int yylex   (GLSLSymbolValue *lvalp, {GLSLParserEnv* e);
void yyerror{GLSLParserEnv* e, const char* error);

%}

%error-verbose

%token CONST
%token BOOL
%token FLOAT
%token DOUBLE
%token INT
%token UINT

%token BREAK
%token CONTINUE
%token DO
%token ELSE
%token FOR
%token IF
%token DISCARD
%token RETURN
%token SWITCH
%token CASE
%token DEFAULT
%token SUBROUTINE

%token SMOOTH
%token FLAT
%token NOPERSPECTIVE

%token LAYOUT 

%token VEC2 
%token VEC3
%token VEC4

%token MAT2
%token MAT3
%token MAT4
%token CENTROID

%token IN
%token OUT
%token INOUT 
%token UNIFORM
%token PATCH
%token SAMPLE

%token MAT2X2
%token MAT2X3
%token MAT2X4
%token MAT3X2
%token MAT3X3
%token MAT3X4
%token MAT4X2
%token MAT4X3
%token MAT4X4


%token SAMPLER1D
%token SAMPLER2D
%token SAMPLER3D
%token SAMPLERCUBE
%token SAMPLER1DARRAY
%token SAMPLER2DARRAY
%token SAMPLERCUBEARRAY

%token STRUCT
%token VOID
%token WHILE
%token IDENTIFIER
%token TYPE_NAME 
%token FLOATCONSTANT
%token DOUBLECONSTANT
%token INTCONSTANT
%token UINTCONSTANT
%token BOOLCONSTANT
%token FIELD_SELECTION
%token LEFT_OP
%token RIGHT_OP
%token INC_OP
%token DEC_OP
%token LE_OP
%token GE_OP
%token EQ_OP
%token NE_OP
%token AND_OP
%token OR_OP
%token XOR_OP
%token MUL_ASSIGN
%token DIV_ASSIGN
%token ADD_ASSIGN
%token MOD_ASSIGN
%token LEFT_ASSIGN
%token RIGHT_ASSIGN
%token AND_ASSIGN
%token XOR_ASSIGN
%token OR_ASSIGN
%token SUB_ASSIGN
%token LEFT_PAREN
%token RIGHT_PAREN
%token LEFT_BRACKET
%token RIGHT_BRACKET
%token LEFT_BRACE
%token RIGHT_BRACE
%token DOT
%token COMMA
%token COLON
%token EQUAL
%token SEMICOLON
%token BANG
%token DASH
%token TILDE
%token PLUS
%token STAR
%token SLASH
%token PERCENT
%token LEFT_ANGLE
%token RIGHT_ANGLE
%token VERTICAL_BAR
%token CARET
%token AMPERSAND
%token QUESTION
%token INVARIANT
%token PRECISE
%token HIGH_PRECISION
%token MEDIUM_PRECISION
%token LOW_PRECISION
%token PRECISION

%%

translation_unit:
 external_declaration                                                                |
 translation_unit external_declaration                                               ;

external_declaration:                                                                
 function_definition                                                                 |
 declaration                                                                         ;

function_definition:
 function_prototype compound_statement_no_new_scope                                  ;

declaration:
 function_prototype SEMICOLON                                                        |
 init_declarator_list SEMICOLON                                                      |
 PRECISION precision_qualifier type_specifier SEMICOLON                              |
 type_qualifier IDENTIFIER LEFT_BRACE struct_declaration_list RIGHT_BRACE SEMICOLON  |
 type_qualifier IDENTIFIER LEFT_BRACE struct_declaration_list RIGHT_BRACE            |
 IDENTIFIER SEMICOLON                                                                |
 ///type_qualifier IDENTIFIER LEFT_BRACE struct_declaration_list RIGHT_BRACE            |
 IDENTIFIER array_specifier SEMICOLON                                                |
 type_qualifier SEMICOLON                                                            |
 type_qualifier IDENTIFIER SEMICOLON                                                 |
 type_qualifier IDENTIFIER identifier_list SEMICOLON                                 ;

type_specifier:
 type_specifier_nonarray                                                             |
 type_specifier_nonarray array_specifier                                             ;

array_specifier:
 LEFT_BRACKET RIGHT_BRACKET                                                          |
 LEFT_BRACKET constant_expression RIGHT_BRACKET                                      |
 array_specifier LEFT_BRACKET RIGHT_BRACKET                                          |
 array_specifier LEFT_BRACKET constant_expression RIGHT_BRACKET                      ;

init_declarator_list:
 single_declaration                                                                  |
 init_declarator_list COMMA IDENTIFIER                                               |
 init_declarator_list COMMA IDENTIFIER array_specifier                               |
 init_declarator_list COMMA IDENTIFIER array_specifier EQUAL initializer             |
 init_declarator_list COMMA IDENTIFIER EQUAL initializer                             ;

single_declaration:
 fully_specified_type                                                                |
 fully_specified_type IDENTIFIER                                                     |
 fully_specified_type IDENTIFIER array_specifier                                     |
 fully_specified_type IDENTIFIER array_specifier EQUAL initializer                   |
 fully_specified_type IDENTIFIER EQUAL initializer                                   ;

// Grammar Note: No 'enum', or 'typedef'.
fully_specified_type:
 type_specifier                                                                      |
 type_qualifier type_specifier                                                       ;

type_qualifier:
 single_type_qualifier                                                               |
 type_qualifier single_type_qualifier                                                ;

variable_identifier:
 IDENTIFIER                                                                          ;

precise_qualifier:
 PRECISE                                                                             ;

unary_operator:
 PLUS                                                                                |
 DASH                                                                                |
 BANG                                                                                |
 TILDE                                                                               ;

type_specifier_nonarray:
 VOID                                                                                |
 FLOAT                                                                               |                                                                            |
 INT                                                                                 |
 BOOL                                                                                |
 VEC2                                                                                |
 VEC3                                                                                |
 VEC4                                                                                |
 MAT2                                                                                |
 MAT3                                                                                |
 MAT4                                                                                |
 MAT2X2                                                                              |
 MAT2X3                                                                              |
 MAT2X4                                                                              |
 MAT3X2                                                                              |
 MAT3X3                                                                              |
 MAT3X4                                                                              |
 MAT4X2                                                                              |
 MAT4X3                                                                              |
 MAT4X4                                                                              |
 SAMPLER1D                                                                           |
 SAMPLER2D                                                                           |
 SAMPLER3D                                                                           |
 SAMPLERCUBE                                                                         |
 SAMPLER1DARRAY                                                                      |
 SAMPLER2DARRAY                                                                      |
 SAMPLERCUBEARRAY                                                                    ;

equality_expression:
 relational_expression                                                               |
 equality_expression EQ_OP relational_expression                                     |
 equality_expression NE_OP relational_expression                                     ;
and_expression:
 equality_expression                                                                 |
 and_expression AMPERSAND equality_expression                                        ;
exclusive_or_expression:
 and_expression                                                                      |
 exclusive_or_expression CARET and_expression                                        ;
inclusive_or_expression:
 exclusive_or_expression                                                             |
 inclusive_or_expression VERTICAL_BAR exclusive_or_expression                        ;
logical_and_expression:
 inclusive_or_expression                                                             |
 logical_and_expression AND_OP inclusive_or_expression                               ;
logical_xor_expression:
 logical_and_expression                                                              |
 logical_xor_expression XOR_OP logical_and_expression                                ;
logical_or_expression:
 logical_xor_expression                                                              |
 logical_or_expression OR_OP logical_xor_expression                                  ;
conditional_expression:
 logical_or_expression                                                               |
 logical_or_expression QUESTION expression COLON assignment_expression               ;
assignment_expression:
 conditional_expression                                                              |
 unary_expression assignment_operator assignment_expression                          ;

initializer:
 assignment_expression                                                               |
 LEFT_BRACE initializer_list RIGHT_BRACE                                             |
 LEFT_BRACE initializer_list COMMA RIGHT_BRACE                                       ;

initializer_list:
 initializer                                                                         |
 initializer_list COMMA initializer                                                  ;

declaration_statement:
 declaration                                                                         ;

statement:
 compound_statement                                                                  |
 simple_statement                                                                    ;
// Grammar Note: labeled statements for SWITCH only; 'goto' is not supported.

simple_statement:
 declaration_statement                                                               |
 expression_statement                                                                |
 selection_statement                                                                 |
 switch_statement                                                                    |
 case_label                                                                          |
 iteration_statement                                                                 |
 jump_statement                                                                      ;
compound_statement:
 LEFT_BRACE RIGHT_BRACE                                                              |
 LEFT_BRACE statement_list RIGHT_BRACE                                               ;
statement_no_new_scope:
 compound_statement_no_new_scope                                                     |
 simple_statement                                                                    ;
compound_statement_no_new_scope:
 LEFT_BRACE RIGHT_BRACE                                                              |
 LEFT_BRACE statement_list RIGHT_BRACE                                               ;
statement_list:
 statement                                                                           |
 statement_list statement                                                            ;
expression_statement:
 SEMICOLON                                                                           |
 expression SEMICOLON                                                                ;
selection_statement:
 IF LEFT_PAREN expression RIGHT_PAREN selection_rest_statement                       ;
selection_rest_statement:
 statement ELSE statement                                                            |
 statement                                                                           ;

condition:
 expression                                                                          |
 fully_specified_type IDENTIFIER EQUAL initializer                                   ;
switch_statement:
 SWITCH LEFT_PAREN expression RIGHT_PAREN LEFT_BRACE switch_statement_list           |
 RIGHT_BRACE                                                                         ;
switch_statement_list:
 /* nothing */
 statement_list                                                                      ;
case_label:
 CASE expression COLON                                                               |
 DEFAULT COLON                                                                       ;
iteration_statement:
 WHILE LEFT_PAREN condition RIGHT_PAREN statement_no_new_scope                       |
 DO statement WHILE LEFT_PAREN expression RIGHT_PAREN SEMICOLON                      |
 FOR LEFT_PAREN for_init_statement for_rest_statement RIGHT_PAREN                    |
 statement_no_new_scope                                                              ;
for_init_statement:
 expression_statement                                                                |
 declaration_statement                                                               ;
conditionopt:
 condition                                                                           ;
 /* empty */
for_rest_statement:
 conditionopt SEMICOLON                                                              |
 conditionopt SEMICOLON expression                                                   ;
jump_statement:
 CONTINUE SEMICOLON                                                                  |
 BREAK SEMICOLON                                                                     |
 RETURN SEMICOLON                                                                    |
 RETURN expression SEMICOLON                                                         |
 DISCARD SEMICOLON                                                                   ; // Fragment shader only.
// Grammar Note: No 'goto'. Gotos are not supported.

assignment_operator:
 EQUAL                                                                               |
 MUL_ASSIGN                                                                          |
 DIV_ASSIGN                                                                          |
 MOD_ASSIGN                                                                          |
 ADD_ASSIGN                                                                          |
 SUB_ASSIGN                                                                          |
 LEFT_ASSIGN                                                                         |
 RIGHT_ASSIGN                                                                        |
 AND_ASSIGN                                                                          |
 XOR_ASSIGN                                                                          |
 OR_ASSIGN                                                                           ;
expression:
 assignment_expression                                                               |
 expression COMMA assignment_expression                                              ;
constant_expression:
 conditional_expression                                                              ;

precision_qualifier:
 HIGH_PRECISION                                                                      |
 MEDIUM_PRECISION                                                                    |
 LOW_PRECISION                                                                       ;

///struct_specifier:
 ///STRUCT IDENTIFIER LEFT_BRACE struct_declaration_list RIGHT_BRACE                    |
 ///STRUCT LEFT_BRACE struct_declaration_list RIGHT_BRACE                               ;

struct_declaration_list:
 struct_declaration                                                                  |
 struct_declaration_list struct_declaration                                          ;

struct_declaration:
 type_specifier struct_declarator_list SEMICOLON                                     |
 type_qualifier type_specifier struct_declarator_list SEMICOLON                      ;

struct_declarator_list:
 struct_declarator                                                                   |
 struct_declarator_list COMMA struct_declarator                                      ;

struct_declarator:
 IDENTIFIER                                                                          |
 IDENTIFIER array_specifier                                                          ;

single_type_qualifier:
 storage_qualifier                                                                   |
 layout_qualifier                                                                    |
 precision_qualifier                                                                 |
 interpolation_qualifier                                                             |
 invariant_qualifier                                                                 |
 precise_qualifier                                                                   ;

identifier_list:
 COMMA IDENTIFIER                                                                    |
 identifier_list COMMA IDENTIFIER                                                    ;

function_prototype:
 function_header                                                                     |
 function_declarator RIGHT_PAREN                                                     ;
function_declarator:
 ///function_header                                                                     |
 function_header_with_parameters                                                     ;
function_header_with_parameters:
 function_header parameter_declaration                                               |
 function_header_with_parameters COMMA parameter_declaration                         ;
function_header:
 fully_specified_type IDENTIFIER LEFT_PAREN                                          ;
parameter_declarator:
 type_specifier IDENTIFIER                                                           |
 type_specifier IDENTIFIER array_specifier                                           ;
parameter_declaration:
 type_qualifier parameter_declarator                                                 |
 parameter_declarator                                                                |
 type_qualifier parameter_type_specifier                                             |
 parameter_type_specifier                                                            ;
parameter_type_specifier:
 type_specifier                                                                      ;

storage_qualifier:                                                                   
 CONST                                                                               |
 INOUT                                                                               |
 IN                                                                                  |
 OUT                                                                                 |
 CENTROID                                                                            |
 PATCH                                                                               |
 SAMPLE                                                                              |
 UNIFORM                                                                             ;
///type_name_list:
 ///type_name_list COMMA TYPE_NAME                                                      |
 ///struct_specifier                                                                    |
 ///TYPE_NAME                                                                           ;

invariant_qualifier:
 INVARIANT                                                                           ;
interpolation_qualifier:
 SMOOTH                                                                              |
 FLAT                                                                                |
 NOPERSPECTIVE                                                                       ;
layout_qualifier:
 LAYOUT LEFT_PAREN layout_qualifier_id_list RIGHT_PAREN                              ;
layout_qualifier_id_list:
 layout_qualifier_id                                                                 |
 layout_qualifier_id_list COMMA layout_qualifier_id                                  ;
layout_qualifier_id:
 IDENTIFIER                                                                          |
 IDENTIFIER EQUAL INTCONSTANT                                                        ;

primary_expression:
 variable_identifier                                                                 |
 INTCONSTANT                                                                         |
 UINTCONSTANT                                                                        |
 FLOATCONSTANT                                                                       |
 BOOLCONSTANT                                                                        |
 DOUBLECONSTANT                                                                      |
 LEFT_PAREN expression RIGHT_PAREN                                                   ;
postfix_expression:
 primary_expression                                                                  |
 postfix_expression LEFT_BRACKET integer_expression RIGHT_BRACKET                    |
 function_call                                                                       |
 postfix_expression DOT FIELD_SELECTION                                              |
 postfix_expression INC_OP                                                           |
 postfix_expression DEC_OP                                                           ;
integer_expression:                                                                                          
 expression                                                                          ; 

// Grammar Note: No '*' or '&' unary ops. Pointers are not supported.
multiplicative_expression:
 unary_expression                                                                    |
 multiplicative_expression STAR unary_expression                                     |
 multiplicative_expression SLASH unary_expression                                    |
 multiplicative_expression PERCENT unary_expression                                  ;
additive_expression:                                                                 
 multiplicative_expression                                                           |
 additive_expression PLUS multiplicative_expression                                  |
 additive_expression DASH multiplicative_expression                                  ;
shift_expression:
 additive_expression                                                                 |
 shift_expression LEFT_OP additive_expression                                        |
 shift_expression RIGHT_OP additive_expression                                       ;
relational_expression:
 shift_expression                                                                    |
 relational_expression LEFT_ANGLE shift_expression                                   |
 relational_expression RIGHT_ANGLE shift_expression                                  |
 relational_expression LE_OP shift_expression                                        |
 relational_expression GE_OP shift_expression                                        ;

function_call:
 function_call_or_method                                                             ;
function_call_or_method:
 function_call_generic                                                               ;
function_call_generic:
 function_call_header_with_parameters RIGHT_PAREN                                    |
 function_call_header_no_parameters RIGHT_PAREN                                      ;
function_call_header_no_parameters:
 function_call_header VOID                                                           |
 function_call_header                                                                ;
function_call_header_with_parameters:
 function_call_header assignment_expression                                          |
 function_call_header_with_parameters COMMA assignment_expression                    ;

function_call_header:
 function_identifier LEFT_PAREN                                                      ;

// Grammar Note: Constructors look like functions, but lexical analysis recognized most of them as
// keywords. They are now recognized through ¡°type_specifier¡±.
// Methods (.length), subroutine array calls, and identifiers are recognized through postfix_expression.
function_identifier:
 type_specifier                                                                      |
 postfix_expression                                                                  ;
unary_expression:
 postfix_expression                                                                  |
 INC_OP unary_expression                                                             |
 DEC_OP unary_expression                                                             |
 unary_operator unary_expression                                                     ;
// Grammar Note: No traditional style type casts.
///unary_operator:
/// PLUS                                                                                |
/// DASH                                                                                |
/// BANG                                                                                |
/// TILDE                                                                               ;
%%
