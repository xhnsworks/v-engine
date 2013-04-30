
/* A Bison parser, made by GNU Bison 2.4.1.  */

/* Skeleton implementation for Bison's Yacc-like parsers in C
   
      Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.
   
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.
   
   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "2.4.1"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 1

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1

/* Using locations.  */
#define YYLSP_NEEDED 0



/* Copy the first part of user declarations.  */

/* Line 189 of yacc.c  */
#line 5 "test.y"

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


/* Line 189 of yacc.c  */
#line 138 "test.tab.c"

/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 1
#endif

/* Enabling the token table.  */
#ifndef YYTOKEN_TABLE
# define YYTOKEN_TABLE 0
#endif


/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     left_brace = 258,
     right_brace = 259,
     left_bracket = 260,
     right_bracket = 261,
     left_parenthese = 262,
     right_parenthese = 263,
     _sym = 264,
     _float = 265,
     _int = 266,
     _numerical = 267,
     _text = 268,
     _struct = 269,
     _dot = 270,
     float_type = 271,
     int_type = 272,
     comma = 273,
     _if = 274,
     _else = 275,
     _for = 276,
     _var = 277,
     _lightdef = 278,
     _structdef = 279,
     _funcdef = 280,
     line_end = 281,
     _equal = 282,
     _less = 283,
     _greater = 284,
     _less_equal = 285,
     _greater_equal = 286,
     _assign = 287,
     _init_assign = 288,
     _add = 289,
     _sub = 290,
     _mul = 291,
     _div = 292,
     _print = 293,
     _function = 294,
     _risk = 295,
     empty = 296
   };
#endif



#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef int YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif


/* Copy the second part of user declarations.  */


/* Line 264 of yacc.c  */
#line 221 "test.tab.c"

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#elif (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
typedef signed char yytype_int8;
#else
typedef short int yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(e) ((void) (e))
#else
# define YYUSE(e) /* empty */
#endif

/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(n) (n)
#else
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static int
YYID (int yyi)
#else
static int
YYID (yyi)
    int yyi;
#endif
{
  return yyi;
}
#endif

#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#     ifndef _STDLIB_H
#      define _STDLIB_H 1
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (YYID (0))
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined _STDLIB_H \
       && ! ((defined YYMALLOC || defined malloc) \
	     && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef _STDLIB_H
#    define _STDLIB_H 1
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
	 || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  YYSIZE_T yyi;				\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (YYID (0))
#  endif
# endif

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)				\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack_alloc, Stack, yysize);			\
	Stack = &yyptr->Stack_alloc;					\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

#endif

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  11
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   346

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  42
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  31
/* YYNRULES -- Number of rules.  */
#define YYNRULES  73
/* YYNRULES -- Number of states.  */
#define YYNSTATES  153

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   296

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint8 yyprhs[] =
{
       0,     0,     3,     5,     7,    10,    13,    18,    21,    26,
      32,    34,    39,    43,    46,    49,    53,    57,    62,    65,
      69,    74,    78,    83,    88,    90,    96,   103,   106,   108,
     111,   113,   116,   118,   121,   123,   125,   135,   138,   141,
     144,   146,   148,   150,   154,   157,   160,   164,   168,   170,
     172,   174,   176,   178,   180,   184,   189,   191,   195,   199,
     203,   207,   211,   213,   217,   222,   226,   230,   234,   238,
     242,   246,   250,   254
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      43,     0,    -1,    48,    -1,    44,    -1,    43,    48,    -1,
      43,    44,    -1,    45,     3,    46,    47,    -1,    14,     9,
      -1,    22,     9,    68,    26,    -1,    46,    22,     9,    68,
      26,    -1,     4,    -1,    49,     3,    67,     4,    -1,    50,
      51,    52,    -1,    50,    51,    -1,    25,     9,    -1,     7,
      53,     8,    -1,     7,    53,     8,    -1,     9,    68,    18,
      53,    -1,     9,    68,    -1,    22,     9,    68,    -1,    22,
       9,    32,    71,    -1,    58,    67,    57,    -1,    55,    59,
      67,    57,    -1,    55,    60,    67,    57,    -1,     4,    -1,
      19,     7,    71,     8,     3,    -1,    20,    19,     7,    71,
       8,     3,    -1,    20,     3,    -1,    26,    -1,    71,    26,
      -1,    26,    -1,    71,    26,    -1,    26,    -1,    71,    26,
      -1,    21,    -1,     4,    -1,    64,     7,    61,    62,    63,
       8,     3,    67,    65,    -1,    71,    26,    -1,    54,    26,
      -1,    72,    26,    -1,    55,    -1,    56,    -1,    66,    -1,
      67,    71,    26,    -1,    67,    55,    -1,    67,    56,    -1,
      67,    54,    26,    -1,    67,    72,    26,    -1,    16,    -1,
      17,    -1,    10,    -1,    11,    -1,     9,    -1,    13,    -1,
       9,     7,     8,    -1,     9,     7,    70,     8,    -1,    69,
      -1,    70,    18,    69,    -1,    71,    34,    71,    -1,    71,
      35,    71,    -1,    71,    36,    71,    -1,    71,    37,    71,
      -1,    69,    -1,    39,     7,     8,    -1,    39,     7,    70,
       8,    -1,     7,    71,     8,    -1,    70,    32,    71,    -1,
       9,    33,    71,    -1,    71,    27,    71,    -1,    71,    28,
      71,    -1,    71,    29,    71,    -1,    71,    30,    71,    -1,
      71,    31,    71,    -1,    38,    71,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint8 yyrline[] =
{
       0,   127,   127,   128,   129,   130,   134,   136,   138,   139,
     141,   145,   148,   149,   152,   154,   156,   158,   159,   163,
     164,   168,   169,   171,   174,   177,   179,   181,   184,   185,
     187,   188,   190,   191,   194,   197,   200,   204,   205,   206,
     207,   208,   209,   210,   211,   212,   213,   214,   217,   218,
     221,   222,   223,   224,   225,   226,   229,   230,   233,   234,
     235,   236,   237,   238,   239,   240,   241,   242,   243,   244,
     245,   246,   247,   250
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "left_brace", "right_brace",
  "left_bracket", "right_bracket", "left_parenthese", "right_parenthese",
  "_sym", "_float", "_int", "_numerical", "_text", "_struct", "_dot",
  "float_type", "int_type", "comma", "_if", "_else", "_for", "_var",
  "_lightdef", "_structdef", "_funcdef", "line_end", "_equal", "_less",
  "_greater", "_less_equal", "_greater_equal", "_assign", "_init_assign",
  "_add", "_sub", "_mul", "_div", "_print", "_function", "_risk", "empty",
  "$accept", "program", "struct_def", "struct_def_begin",
  "struct_body_def", "struct_def_end", "func_def", "func_head_def",
  "func_def_begin", "param_set_def", "output_set_def", "params", "var_def",
  "half_if_block", "full_if_block", "block_end", "prev_if_def",
  "else_if_def", "post_if_def", "for_loop_init", "for_loop_cond",
  "for_loop_inc", "for_begin", "for_end", "for_loop", "lines", "valuetype",
  "value", "args", "expr", "print_output", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    42,    43,    43,    43,    43,    44,    45,    46,    46,
      47,    48,    49,    49,    50,    51,    52,    53,    53,    54,
      54,    55,    55,    56,    57,    58,    59,    60,    61,    61,
      62,    62,    63,    63,    64,    65,    66,    67,    67,    67,
      67,    67,    67,    67,    67,    67,    67,    67,    68,    68,
      69,    69,    69,    69,    69,    69,    70,    70,    71,    71,
      71,    71,    71,    71,    71,    71,    71,    71,    71,    71,
      71,    71,    71,    72
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     1,     2,     2,     4,     2,     4,     5,
       1,     4,     3,     2,     2,     3,     3,     4,     2,     3,
       4,     3,     4,     4,     1,     5,     6,     2,     1,     2,
       1,     2,     1,     2,     1,     1,     9,     2,     2,     2,
       1,     1,     1,     3,     2,     2,     3,     3,     1,     1,
       1,     1,     1,     1,     3,     4,     1,     3,     3,     3,
       3,     3,     1,     3,     4,     3,     3,     3,     3,     3,
       3,     3,     3,     2
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,     0,     0,     0,     3,     0,     2,     0,     0,     7,
      14,     1,     5,     4,     0,     0,     0,    13,     0,     0,
       0,    52,    50,    51,    53,     0,    34,     0,     0,     0,
       0,    40,    41,     0,     0,    42,     0,    62,     0,     0,
       0,     0,     0,     0,    12,     0,    10,     0,     6,     0,
       0,     0,     0,     0,    73,     0,    38,     0,     0,     0,
       0,     0,    11,     0,    44,    45,     0,     0,     0,     0,
      37,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      39,    48,    49,    18,    15,     0,     0,     0,    65,    54,
      52,    56,     0,    67,     0,     0,    19,    63,     0,    27,
       0,     0,     0,    24,    21,    28,     0,     0,    46,    43,
      47,    57,    66,    68,    69,    70,    71,    72,    58,    59,
      60,    61,     0,    16,     8,     0,    55,     0,    20,    64,
       0,    22,    23,    30,     0,     0,    29,    17,     9,    25,
       0,    32,     0,     0,    31,     0,     0,    33,    26,     0,
       0,    35,    36
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     3,     4,     5,    19,    48,     6,     7,     8,    17,
      44,    42,    30,    31,    32,   104,    33,    58,    59,   106,
     134,   142,    34,   152,    35,    36,    83,    37,    38,    39,
      40
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -44
static const yytype_int16 yypact[] =
{
       0,    -2,    15,     3,   -44,     9,   -44,    33,    38,   -44,
     -44,   -44,   -44,   -44,    42,   145,    59,    73,    82,    17,
      56,    -3,   -44,   -44,   -44,    91,   -44,    95,    56,   105,
      87,   100,   -44,   145,   108,   -44,    83,    28,    29,   210,
      92,    57,   115,    59,   -44,    57,   -44,   117,   -44,   167,
      98,    56,    56,     6,   269,   333,   -44,    31,   145,   145,
     124,    90,   -44,   110,   100,   -44,   222,   118,   169,    56,
     -44,    56,    56,    56,    56,    56,    56,    56,    56,    56,
     -44,   -44,   -44,   106,   -44,   142,   127,    57,   -44,   -44,
     152,   -44,     1,   104,   185,    56,   -44,   -44,    54,   -44,
     154,   124,   124,   -44,   -44,   -44,   161,   234,   -44,   -44,
     -44,   -44,   104,   269,   279,   288,   296,   303,   104,   172,
     128,   -44,    59,   -44,   -44,   143,   -44,   170,   269,   -44,
      56,   -44,   -44,   -44,   179,   246,   -44,   -44,   -44,   -44,
     198,   -44,   173,   258,   -44,   182,   188,   -44,   -44,   145,
     138,   -44,   -44
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
     -44,   -44,   196,   -44,   -44,   -44,   207,   -44,   -44,   -44,
     -44,   -37,   -31,   -25,   -23,   -13,   -44,   -44,   -44,   -44,
     -44,   -44,   -44,   -44,   -44,   -32,   -43,   -35,    -7,   -20,
     -18
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -57
static const yytype_int16 yytable[] =
{
      49,    60,    86,    11,    50,    63,    85,     9,    54,   126,
      96,    64,    14,    65,     1,    91,    66,     1,    67,    68,
      91,    46,    81,    82,    10,     2,   101,   102,     2,    63,
      51,    93,    94,   111,    99,    64,    15,    65,    95,    47,
      66,   107,    67,    92,   125,    16,   -56,    68,    98,   112,
     100,   113,   114,   115,   116,   117,   118,   119,   120,   121,
     -56,    69,   129,    20,    18,    21,    22,    23,    41,    24,
      63,    63,    68,    81,    82,   128,    64,    64,    65,    65,
      43,    66,    66,    67,    67,   137,   135,    62,   131,   132,
      20,    45,    21,    22,    23,    29,    24,    20,    52,    21,
      22,    23,    25,    24,    53,    27,    89,    90,    22,    23,
     140,    24,    55,    56,   143,    61,   105,   150,    80,    63,
      57,    28,    29,    84,   122,    64,    87,    65,   103,    29,
      66,    20,    67,    21,    22,    23,   108,    24,    76,    77,
      78,    79,   151,    25,   110,    20,    27,    21,    22,    23,
     123,    24,    20,   124,    21,    22,    23,    25,    24,    50,
      27,   130,    28,    29,    25,    79,    26,    27,    20,   138,
      21,    22,    23,   139,    24,    88,    28,    29,    90,    22,
      23,   146,    24,    28,    29,   148,    20,   133,    21,    22,
      23,   149,    24,   127,    71,    72,    73,    74,    75,    12,
      29,    76,    77,    78,    79,   141,   145,    77,    78,    79,
      13,     0,    71,    72,    73,    74,    75,     0,    29,    76,
      77,    78,    79,     0,     0,    71,    72,    73,    74,    75,
       0,     0,    76,    77,    78,    79,    70,    71,    72,    73,
      74,    75,     0,     0,    76,    77,    78,    79,   109,    71,
      72,    73,    74,    75,     0,     0,    76,    77,    78,    79,
     136,    71,    72,    73,    74,    75,     0,     0,    76,    77,
      78,    79,   144,    71,    72,    73,    74,    75,     0,     0,
      76,    77,    78,    79,   147,    71,    72,    73,    74,    75,
       0,     0,    76,    77,    78,    79,    71,    72,    73,    74,
      75,     0,     0,    76,    77,    78,    79,    72,    73,    74,
      75,     0,     0,    76,    77,    78,    79,    73,    74,    75,
       0,     0,    76,    77,    78,    79,    74,    75,     0,     0,
      76,    77,    78,    79,    75,     0,     0,    76,    77,    78,
      79,    97,    90,    22,    23,     0,    24
};

static const yytype_int16 yycheck[] =
{
      20,    33,    45,     0,     7,    36,    43,     9,    28,     8,
      53,    36,     3,    36,    14,    50,    36,    14,    36,    18,
      55,     4,    16,    17,     9,    25,    58,    59,    25,    60,
      33,    51,    52,    68,     3,    60,     3,    60,    32,    22,
      60,    61,    60,    50,    87,     7,    18,    18,    55,    69,
      19,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      32,    32,     8,     7,    22,     9,    10,    11,     9,    13,
     101,   102,    18,    16,    17,    95,   101,   102,   101,   102,
       7,   101,   102,   101,   102,   122,   106,     4,   101,   102,
       7,     9,     9,    10,    11,    39,    13,     7,     7,     9,
      10,    11,    19,    13,     9,    22,     8,     9,    10,    11,
     130,    13,     7,    26,   134,     7,    26,   149,    26,   150,
      20,    38,    39,     8,    18,   150,     9,   150,     4,    39,
     150,     7,   150,     9,    10,    11,    26,    13,    34,    35,
      36,    37,     4,    19,    26,     7,    22,     9,    10,    11,
       8,    13,     7,    26,     9,    10,    11,    19,    13,     7,
      22,     7,    38,    39,    19,    37,    21,    22,     7,    26,
       9,    10,    11,     3,    13,     8,    38,    39,     9,    10,
      11,     8,    13,    38,    39,     3,     7,    26,     9,    10,
      11,     3,    13,     8,    27,    28,    29,    30,    31,     3,
      39,    34,    35,    36,    37,    26,     8,    35,    36,    37,
       3,    -1,    27,    28,    29,    30,    31,    -1,    39,    34,
      35,    36,    37,    -1,    -1,    27,    28,    29,    30,    31,
      -1,    -1,    34,    35,    36,    37,    26,    27,    28,    29,
      30,    31,    -1,    -1,    34,    35,    36,    37,    26,    27,
      28,    29,    30,    31,    -1,    -1,    34,    35,    36,    37,
      26,    27,    28,    29,    30,    31,    -1,    -1,    34,    35,
      36,    37,    26,    27,    28,    29,    30,    31,    -1,    -1,
      34,    35,    36,    37,    26,    27,    28,    29,    30,    31,
      -1,    -1,    34,    35,    36,    37,    27,    28,    29,    30,
      31,    -1,    -1,    34,    35,    36,    37,    28,    29,    30,
      31,    -1,    -1,    34,    35,    36,    37,    29,    30,    31,
      -1,    -1,    34,    35,    36,    37,    30,    31,    -1,    -1,
      34,    35,    36,    37,    31,    -1,    -1,    34,    35,    36,
      37,     8,     9,    10,    11,    -1,    13
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    14,    25,    43,    44,    45,    48,    49,    50,     9,
       9,     0,    44,    48,     3,     3,     7,    51,    22,    46,
       7,     9,    10,    11,    13,    19,    21,    22,    38,    39,
      54,    55,    56,    58,    64,    66,    67,    69,    70,    71,
      72,     9,    53,     7,    52,     9,     4,    22,    47,    71,
       7,    33,     7,     9,    71,     7,    26,    20,    59,    60,
      67,     7,     4,    54,    55,    56,    71,    72,    18,    32,
      26,    27,    28,    29,    30,    31,    34,    35,    36,    37,
      26,    16,    17,    68,     8,    53,    68,     9,     8,     8,
       9,    69,    70,    71,    71,    32,    68,     8,    70,     3,
      19,    67,    67,     4,    57,    26,    61,    71,    26,    26,
      26,    69,    71,    71,    71,    71,    71,    71,    71,    71,
      71,    71,    18,     8,    26,    68,     8,     8,    71,     8,
       7,    57,    57,    26,    62,    71,    26,    53,    26,     3,
      71,    26,    63,    71,    26,     8,     8,    26,     3,     3,
      67,     4,    65
};

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */

#define YYFAIL		goto yyerrlab

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      yytoken = YYTRANSLATE (yychar);				\
      YYPOPSTACK (1);						\
      goto yybackup;						\
    }								\
  else								\
    {								\
      yyerror (e, YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (YYID (0))


#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#define YYRHSLOC(Rhs, K) ((Rhs)[K])
#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)				\
    do									\
      if (YYID (N))                                                    \
	{								\
	  (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;	\
	  (Current).first_column = YYRHSLOC (Rhs, 1).first_column;	\
	  (Current).last_line    = YYRHSLOC (Rhs, N).last_line;		\
	  (Current).last_column  = YYRHSLOC (Rhs, N).last_column;	\
	}								\
      else								\
	{								\
	  (Current).first_line   = (Current).last_line   =		\
	    YYRHSLOC (Rhs, 0).last_line;				\
	  (Current).first_column = (Current).last_column =		\
	    YYRHSLOC (Rhs, 0).last_column;				\
	}								\
    while (YYID (0))
#endif


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if YYLTYPE_IS_TRIVIAL
#  define YY_LOCATION_PRINT(File, Loc)			\
     fprintf (File, "%d.%d-%d.%d",			\
	      (Loc).first_line, (Loc).first_column,	\
	      (Loc).last_line,  (Loc).last_column)
# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX yylex (&yylval, YYLEX_PARAM)
#else
# define YYLEX yylex (&yylval, e)
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (YYID (0))

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)			  \
do {									  \
  if (yydebug)								  \
    {									  \
      YYFPRINTF (stderr, "%s ", Title);					  \
      yy_symbol_print (stderr,						  \
		  Type, Value, e); \
      YYFPRINTF (stderr, "\n");						  \
    }									  \
} while (YYID (0))


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, ParserEnv* e)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep, e)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
    ParserEnv* e;
#endif
{
  if (!yyvaluep)
    return;
  YYUSE (e);
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# else
  YYUSE (yyoutput);
# endif
  switch (yytype)
    {
      default:
	break;
    }
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, ParserEnv* e)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep, e)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
    ParserEnv* e;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep, e);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
#else
static void
yy_stack_print (yybottom, yytop)
    yytype_int16 *yybottom;
    yytype_int16 *yytop;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (YYID (0))


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_reduce_print (YYSTYPE *yyvsp, int yyrule, ParserEnv* e)
#else
static void
yy_reduce_print (yyvsp, yyrule, e)
    YYSTYPE *yyvsp;
    int yyrule;
    ParserEnv* e;
#endif
{
  int yynrhs = yyr2[yyrule];
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
	     yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       		       , e);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, Rule, e); \
} while (YYID (0))

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif



#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static YYSIZE_T
yystrlen (const char *yystr)
#else
static YYSIZE_T
yystrlen (yystr)
    const char *yystr;
#endif
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static char *
yystpcpy (char *yydest, const char *yysrc)
#else
static char *
yystpcpy (yydest, yysrc)
    char *yydest;
    const char *yysrc;
#endif
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
	switch (*++yyp)
	  {
	  case '\'':
	  case ',':
	    goto do_not_strip_quotes;

	  case '\\':
	    if (*++yyp != '\\')
	      goto do_not_strip_quotes;
	    /* Fall through.  */
	  default:
	    if (yyres)
	      yyres[yyn] = *yyp;
	    yyn++;
	    break;

	  case '"':
	    if (yyres)
	      yyres[yyn] = '\0';
	    return yyn;
	  }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into YYRESULT an error message about the unexpected token
   YYCHAR while in state YYSTATE.  Return the number of bytes copied,
   including the terminating null byte.  If YYRESULT is null, do not
   copy anything; just return the number of bytes that would be
   copied.  As a special case, return 0 if an ordinary "syntax error"
   message will do.  Return YYSIZE_MAXIMUM if overflow occurs during
   size calculation.  */
static YYSIZE_T
yysyntax_error (char *yyresult, int yystate, int yychar)
{
  int yyn = yypact[yystate];

  if (! (YYPACT_NINF < yyn && yyn <= YYLAST))
    return 0;
  else
    {
      int yytype = YYTRANSLATE (yychar);
      YYSIZE_T yysize0 = yytnamerr (0, yytname[yytype]);
      YYSIZE_T yysize = yysize0;
      YYSIZE_T yysize1;
      int yysize_overflow = 0;
      enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
      char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
      int yyx;

# if 0
      /* This is so xgettext sees the translatable formats that are
	 constructed on the fly.  */
      YY_("syntax error, unexpected %s");
      YY_("syntax error, unexpected %s, expecting %s");
      YY_("syntax error, unexpected %s, expecting %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s");
# endif
      char *yyfmt;
      char const *yyf;
      static char const yyunexpected[] = "syntax error, unexpected %s";
      static char const yyexpecting[] = ", expecting %s";
      static char const yyor[] = " or %s";
      char yyformat[sizeof yyunexpected
		    + sizeof yyexpecting - 1
		    + ((YYERROR_VERBOSE_ARGS_MAXIMUM - 2)
		       * (sizeof yyor - 1))];
      char const *yyprefix = yyexpecting;

      /* Start YYX at -YYN if negative to avoid negative indexes in
	 YYCHECK.  */
      int yyxbegin = yyn < 0 ? -yyn : 0;

      /* Stay within bounds of both yycheck and yytname.  */
      int yychecklim = YYLAST - yyn + 1;
      int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
      int yycount = 1;

      yyarg[0] = yytname[yytype];
      yyfmt = yystpcpy (yyformat, yyunexpected);

      for (yyx = yyxbegin; yyx < yyxend; ++yyx)
	if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
	  {
	    if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
	      {
		yycount = 1;
		yysize = yysize0;
		yyformat[sizeof yyunexpected - 1] = '\0';
		break;
	      }
	    yyarg[yycount++] = yytname[yyx];
	    yysize1 = yysize + yytnamerr (0, yytname[yyx]);
	    yysize_overflow |= (yysize1 < yysize);
	    yysize = yysize1;
	    yyfmt = yystpcpy (yyfmt, yyprefix);
	    yyprefix = yyor;
	  }

      yyf = YY_(yyformat);
      yysize1 = yysize + yystrlen (yyf);
      yysize_overflow |= (yysize1 < yysize);
      yysize = yysize1;

      if (yysize_overflow)
	return YYSIZE_MAXIMUM;

      if (yyresult)
	{
	  /* Avoid sprintf, as that infringes on the user's name space.
	     Don't have undefined behavior even if the translation
	     produced a string with the wrong number of "%s"s.  */
	  char *yyp = yyresult;
	  int yyi = 0;
	  while ((*yyp = *yyf) != '\0')
	    {
	      if (*yyp == '%' && yyf[1] == 's' && yyi < yycount)
		{
		  yyp += yytnamerr (yyp, yyarg[yyi++]);
		  yyf += 2;
		}
	      else
		{
		  yyp++;
		  yyf++;
		}
	    }
	}
      return yysize;
    }
}
#endif /* YYERROR_VERBOSE */


/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep, ParserEnv* e)
#else
static void
yydestruct (yymsg, yytype, yyvaluep, e)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
    ParserEnv* e;
#endif
{
  YYUSE (yyvaluep);
  YYUSE (e);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {

      default:
	break;
    }
}

/* Prevent warnings from -Wmissing-prototypes.  */
#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int yyparse (void *YYPARSE_PARAM);
#else
int yyparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int yyparse (ParserEnv* e);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */





/*-------------------------.
| yyparse or yypush_parse.  |
`-------------------------*/

#ifdef YYPARSE_PARAM
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void *YYPARSE_PARAM)
#else
int
yyparse (YYPARSE_PARAM)
    void *YYPARSE_PARAM;
#endif
#else /* ! YYPARSE_PARAM */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (ParserEnv* e)
#else
int
yyparse (e)
    ParserEnv* e;
#endif
#endif
{
/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;

    /* Number of syntax errors so far.  */
    int yynerrs;

    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       `yyss': related to states.
       `yyvs': related to semantic values.

       Refer to the stacks thru separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yytoken = 0;
  yyss = yyssa;
  yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */
  yyssp = yyss;
  yyvsp = yyvs;

  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack.  Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	yytype_int16 *yyss1 = yyss;

	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow (YY_("memory exhausted"),
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),
		    &yystacksize);

	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	yytype_int16 *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyexhaustedlab;
	YYSTACK_RELOCATE (yyss_alloc, yyss);
	YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yyn == YYPACT_NINF)
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yyn == 0 || yyn == YYTABLE_NINF)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  *++yyvsp = yylval;

  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 7:

/* Line 1455 of yacc.c  */
#line 136 "test.y"
    { NewStruct(e, (yyvsp[(2) - (2)])); EXCE_TEST; ;}
    break;

  case 8:

/* Line 1455 of yacc.c  */
#line 138 "test.y"
    { MarkStructMember(e, (yyvsp[(2) - (4)]), (yyvsp[(3) - (4)])); EXCE_TEST; ;}
    break;

  case 9:

/* Line 1455 of yacc.c  */
#line 139 "test.y"
    { MarkStructMember(e, (yyvsp[(3) - (5)]), (yyvsp[(4) - (5)])); EXCE_TEST; ;}
    break;

  case 10:

/* Line 1455 of yacc.c  */
#line 141 "test.y"
    { CompleteStruct(e); EXCE_TEST; ;}
    break;

  case 11:

/* Line 1455 of yacc.c  */
#line 145 "test.y"
    { CompleteFuncDef(e); EXCE_TEST; ;}
    break;

  case 13:

/* Line 1455 of yacc.c  */
#line 149 "test.y"
    { CompleteOutputDef(e); EXCE_TEST; ;}
    break;

  case 14:

/* Line 1455 of yacc.c  */
#line 152 "test.y"
    { BeginFuncDef(e, (yyvsp[(2) - (2)])); ;}
    break;

  case 15:

/* Line 1455 of yacc.c  */
#line 154 "test.y"
    { CompleteParamDef(e); EXCE_TEST; ;}
    break;

  case 16:

/* Line 1455 of yacc.c  */
#line 156 "test.y"
    { CompleteOutputDef(e); EXCE_TEST; ;}
    break;

  case 17:

/* Line 1455 of yacc.c  */
#line 158 "test.y"
    { AddParam(e, (yyvsp[(1) - (4)]), (yyvsp[(2) - (4)])); EXCE_TEST; ;}
    break;

  case 18:

/* Line 1455 of yacc.c  */
#line 159 "test.y"
    { AddParam(e, (yyvsp[(1) - (2)]), (yyvsp[(2) - (2)])); EXCE_TEST; ;}
    break;

  case 19:

/* Line 1455 of yacc.c  */
#line 163 "test.y"
    { (yyval) = VarDef(e, (yyvsp[(2) - (3)]), (yyvsp[(3) - (3)])); EXCE_TEST; ;}
    break;

  case 24:

/* Line 1455 of yacc.c  */
#line 174 "test.y"
    { MarkEnd(e); EXCE_TEST; ;}
    break;

  case 25:

/* Line 1455 of yacc.c  */
#line 177 "test.y"
    { (yyval) = MarkIf(e, (yyvsp[(3) - (5)])); EXCE_TEST; ;}
    break;

  case 26:

/* Line 1455 of yacc.c  */
#line 179 "test.y"
    { (yyval) = MarkElseIf(e, (yyvsp[(4) - (6)])); EXCE_TEST; ;}
    break;

  case 27:

/* Line 1455 of yacc.c  */
#line 181 "test.y"
    { (yyval) = MarkElse(e); EXCE_TEST; ;}
    break;

  case 28:

/* Line 1455 of yacc.c  */
#line 184 "test.y"
    { MarkForCondBegin(e); EXCE_TEST; ;}
    break;

  case 29:

/* Line 1455 of yacc.c  */
#line 185 "test.y"
    { MarkForCondBegin(e); EXCE_TEST; ;}
    break;

  case 30:

/* Line 1455 of yacc.c  */
#line 187 "test.y"
    { MarkForAlways(e); EXCE_TEST; ;}
    break;

  case 31:

/* Line 1455 of yacc.c  */
#line 188 "test.y"
    { MarkForCond(e, (yyvsp[(1) - (2)])); EXCE_TEST; ;}
    break;

  case 33:

/* Line 1455 of yacc.c  */
#line 191 "test.y"
    { ForInc(e); EXCE_TEST; ;}
    break;

  case 34:

/* Line 1455 of yacc.c  */
#line 194 "test.y"
    { ForBegin(e); EXCE_TEST; ;}
    break;

  case 35:

/* Line 1455 of yacc.c  */
#line 197 "test.y"
    { ForEnd(e); EXCE_TEST; ;}
    break;

  case 37:

/* Line 1455 of yacc.c  */
#line 204 "test.y"
    {;}
    break;

  case 43:

/* Line 1455 of yacc.c  */
#line 210 "test.y"
    {;}
    break;

  case 48:

/* Line 1455 of yacc.c  */
#line 217 "test.y"
    { (yyval) = FloatTypeSym(); EXCE_TEST; ;}
    break;

  case 49:

/* Line 1455 of yacc.c  */
#line 218 "test.y"
    { (yyval) = IntTypeSym(); EXCE_TEST; ;}
    break;

  case 56:

/* Line 1455 of yacc.c  */
#line 229 "test.y"
    { (yyval) = NewValueArray(e, (yyvsp[(1) - (1)])); EXCE_TEST; ;}
    break;

  case 57:

/* Line 1455 of yacc.c  */
#line 230 "test.y"
    { (yyval) = ValueArrayPush(e, (yyvsp[(1) - (3)]), (yyvsp[(3) - (3)])); EXCE_TEST;  ;}
    break;

  case 58:

/* Line 1455 of yacc.c  */
#line 233 "test.y"
    { (yyval) = AddProc(e, (yyvsp[(1) - (3)]), (yyvsp[(3) - (3)])); EXCE_TEST; ;}
    break;

  case 59:

/* Line 1455 of yacc.c  */
#line 234 "test.y"
    { (yyval) = SubProc(e, (yyvsp[(1) - (3)]), (yyvsp[(3) - (3)])); EXCE_TEST; ;}
    break;

  case 60:

/* Line 1455 of yacc.c  */
#line 235 "test.y"
    { (yyval) = MulProc(e, (yyvsp[(1) - (3)]), (yyvsp[(3) - (3)])); EXCE_TEST; ;}
    break;

  case 61:

/* Line 1455 of yacc.c  */
#line 236 "test.y"
    { (yyval) = DivProc(e, (yyvsp[(1) - (3)]), (yyvsp[(3) - (3)])); EXCE_TEST; ;}
    break;

  case 62:

/* Line 1455 of yacc.c  */
#line 237 "test.y"
    { (yyval) = (yyvsp[(1) - (1)]); EXCE_TEST; ;}
    break;

  case 63:

/* Line 1455 of yacc.c  */
#line 238 "test.y"
    { (yyval) = FuncCall(e, (yyvsp[(1) - (3)]));      PrintSymbolValue(e, "FUNC CALL: ", (yyval)); EXCE_TEST; ;}
    break;

  case 64:

/* Line 1455 of yacc.c  */
#line 239 "test.y"
    { (yyval) = FuncCall2(e, (yyvsp[(1) - (4)]), (yyvsp[(3) - (4)])); PrintSymbolValue(e, "FUNC CALL: ", (yyval)); EXCE_TEST; ;}
    break;

  case 65:

/* Line 1455 of yacc.c  */
#line 240 "test.y"
    { (yyval) = (yyvsp[(2) - (3)]); ;}
    break;

  case 66:

/* Line 1455 of yacc.c  */
#line 241 "test.y"
    { (yyval) = AssignProc(e, (yyvsp[(1) - (3)]), (yyvsp[(3) - (3)])); PrintSymbolValue(e, "ASSIGN DST: ", (yyvsp[(1) - (3)])); PrintSymbolValue(e, "ASSIGN DST: ", (yyvsp[(3) - (3)])); EXCE_TEST; ;}
    break;

  case 67:

/* Line 1455 of yacc.c  */
#line 242 "test.y"
    { (yyval) = InitAssignProc(e, (yyvsp[(1) - (3)]), (yyvsp[(3) - (3)])); EXCE_TEST; ;}
    break;

  case 68:

/* Line 1455 of yacc.c  */
#line 243 "test.y"
    { (yyval) = EqualProc(e, (yyvsp[(1) - (3)]), (yyvsp[(3) - (3)])); EXCE_TEST; ;}
    break;

  case 69:

/* Line 1455 of yacc.c  */
#line 244 "test.y"
    { (yyval) = LessProc(e, (yyvsp[(1) - (3)]), (yyvsp[(3) - (3)])); EXCE_TEST; ;}
    break;

  case 70:

/* Line 1455 of yacc.c  */
#line 245 "test.y"
    { (yyval) = GreaterProc(e, (yyvsp[(1) - (3)]), (yyvsp[(3) - (3)])); EXCE_TEST; ;}
    break;

  case 71:

/* Line 1455 of yacc.c  */
#line 246 "test.y"
    { (yyval) = LessEqualProc(e, (yyvsp[(1) - (3)]), (yyvsp[(3) - (3)])); EXCE_TEST; ;}
    break;

  case 72:

/* Line 1455 of yacc.c  */
#line 247 "test.y"
    { (yyval) = GreaterEqualProc(e, (yyvsp[(1) - (3)]), (yyvsp[(3) - (3)])); EXCE_TEST; ;}
    break;

  case 73:

/* Line 1455 of yacc.c  */
#line 250 "test.y"
    { (yyval) = PrintProc(e, (yyvsp[(2) - (2)])); EXCE_TEST; ;}
    break;



/* Line 1455 of yacc.c  */
#line 1910 "test.tab.c"
      default: break;
    }
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (e, YY_("syntax error"));
#else
      {
	YYSIZE_T yysize = yysyntax_error (0, yystate, yychar);
	if (yymsg_alloc < yysize && yymsg_alloc < YYSTACK_ALLOC_MAXIMUM)
	  {
	    YYSIZE_T yyalloc = 2 * yysize;
	    if (! (yysize <= yyalloc && yyalloc <= YYSTACK_ALLOC_MAXIMUM))
	      yyalloc = YYSTACK_ALLOC_MAXIMUM;
	    if (yymsg != yymsgbuf)
	      YYSTACK_FREE (yymsg);
	    yymsg = (char *) YYSTACK_ALLOC (yyalloc);
	    if (yymsg)
	      yymsg_alloc = yyalloc;
	    else
	      {
		yymsg = yymsgbuf;
		yymsg_alloc = sizeof yymsgbuf;
	      }
	  }

	if (0 < yysize && yysize <= yymsg_alloc)
	  {
	    (void) yysyntax_error (yymsg, yystate, yychar);
	    yyerror (e, yymsg);
	  }
	else
	  {
	    yyerror (e, YY_("syntax error"));
	    if (yysize != 0)
	      goto yyexhaustedlab;
	  }
      }
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
	 error, discard it.  */

      if (yychar <= YYEOF)
	{
	  /* Return failure if at end of input.  */
	  if (yychar == YYEOF)
	    YYABORT;
	}
      else
	{
	  yydestruct ("Error: discarding",
		      yytoken, &yylval, e);
	  yychar = YYEMPTY;
	}
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule which action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (yyn != YYPACT_NINF)
	{
	  yyn += YYTERROR;
	  if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
	    {
	      yyn = yytable[yyn];
	      if (0 < yyn)
		break;
	    }
	}

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
	YYABORT;


      yydestruct ("Error: popping",
		  yystos[yystate], yyvsp, e);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  *++yyvsp = yylval;


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#if !defined(yyoverflow) || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (e, YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
     yydestruct ("Cleanup: discarding lookahead",
		 yytoken, &yylval, e);
  /* Do not reclaim the symbols of the rule which action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp, e);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  /* Make sure YYID is used.  */
  return YYID (yyresult);
}



/* Line 1675 of yacc.c  */
#line 252 "test.y"


