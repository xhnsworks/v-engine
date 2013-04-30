/* A Bison parser, made by GNU Bison 2.3.  */

/* Skeleton implementation for Bison's Yacc-like parsers in C

   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.  */

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
#define YYBISON_VERSION "2.3"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 1

/* Using locations.  */
#define YYLSP_NEEDED 0



/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     __CONST = 258,
     __BOOL = 259,
     __FLOAT = 260,
     __DOUBLE = 261,
     __INT = 262,
     __UINT = 263,
     __BREAK = 264,
     __CONTINUE = 265,
     __DO = 266,
     __ELSE = 267,
     __FOR = 268,
     __IF = 269,
     __DISCARD = 270,
     __RETURN = 271,
     __SWITCH = 272,
     __CASE = 273,
     __DEFAULT = 274,
     __SUBROUTINE = 275,
     __SMOOTH = 276,
     __FLAT = 277,
     __NOPERSPECTIVE = 278,
     __LAYOUT = 279,
     __VEC2 = 280,
     __VEC3 = 281,
     __VEC4 = 282,
     __MAT2 = 283,
     __MAT3 = 284,
     __MAT4 = 285,
     __CENTROID = 286,
     __IN = 287,
     __OUT = 288,
     __INOUT = 289,
     __UNIFORM = 290,
     __PATCH = 291,
     __SAMPLE = 292,
     __MAT2X2 = 293,
     __MAT2X3 = 294,
     __MAT2X4 = 295,
     __MAT3X2 = 296,
     __MAT3X3 = 297,
     __MAT3X4 = 298,
     __MAT4X2 = 299,
     __MAT4X3 = 300,
     __MAT4X4 = 301,
     __SAMPLER1D = 302,
     __SAMPLER2D = 303,
     __SAMPLER3D = 304,
     __SAMPLERCUBE = 305,
     __SAMPLER1DARRAY = 306,
     __SAMPLER2DARRAY = 307,
     __SAMPLERCUBEARRAY = 308,
     __STRUCT = 309,
     __VOID = 310,
     __WHILE = 311,
     __IDENTIFIER = 312,
     __TYPE_NAME = 313,
     __FLOATCONSTANT = 314,
     __DOUBLECONSTANT = 315,
     __INTCONSTANT = 316,
     __UINTCONSTANT = 317,
     __BOOLCONSTANT = 318,
     __FIELD_SELECTION = 319,
     __LEFT_OP = 320,
     __RIGHT_OP = 321,
     __INC_OP = 322,
     __DEC_OP = 323,
     __LE_OP = 324,
     __GE_OP = 325,
     __EQ_OP = 326,
     __NE_OP = 327,
     __AND_OP = 328,
     __OR_OP = 329,
     __XOR_OP = 330,
     __MUL_ASSIGN = 331,
     __DIV_ASSIGN = 332,
     __ADD_ASSIGN = 333,
     __MOD_ASSIGN = 334,
     __LEFT_ASSIGN = 335,
     __RIGHT_ASSIGN = 336,
     __AND_ASSIGN = 337,
     __XOR_ASSIGN = 338,
     __OR_ASSIGN = 339,
     __SUB_ASSIGN = 340,
     __LEFT_PAREN = 341,
     __RIGHT_PAREN = 342,
     __LEFT_BRACKET = 343,
     __RIGHT_BRACKET = 344,
     __LEFT_BRACE = 345,
     __RIGHT_BRACE = 346,
     __DOT = 347,
     __COMMA = 348,
     __COLON = 349,
     __EQUAL = 350,
     __SEMICOLON = 351,
     __BANG = 352,
     __DASH = 353,
     __TILDE = 354,
     __PLUS = 355,
     __STAR = 356,
     __SLASH = 357,
     __PERCENT = 358,
     __LEFT_ANGLE = 359,
     __RIGHT_ANGLE = 360,
     __VERTICAL_BAR = 361,
     __CARET = 362,
     __AMPERSAND = 363,
     __QUESTION = 364,
     __INVARIANT = 365,
     __PRECISE = 366,
     __HIGH_PRECISION = 367,
     __MEDIUM_PRECISION = 368,
     __LOW_PRECISION = 369,
     __PRECISION = 370
   };
#endif
/* Tokens.  */
#define __CONST 258
#define __BOOL 259
#define __FLOAT 260
#define __DOUBLE 261
#define __INT 262
#define __UINT 263
#define __BREAK 264
#define __CONTINUE 265
#define __DO 266
#define __ELSE 267
#define __FOR 268
#define __IF 269
#define __DISCARD 270
#define __RETURN 271
#define __SWITCH 272
#define __CASE 273
#define __DEFAULT 274
#define __SUBROUTINE 275
#define __SMOOTH 276
#define __FLAT 277
#define __NOPERSPECTIVE 278
#define __LAYOUT 279
#define __VEC2 280
#define __VEC3 281
#define __VEC4 282
#define __MAT2 283
#define __MAT3 284
#define __MAT4 285
#define __CENTROID 286
#define __IN 287
#define __OUT 288
#define __INOUT 289
#define __UNIFORM 290
#define __PATCH 291
#define __SAMPLE 292
#define __MAT2X2 293
#define __MAT2X3 294
#define __MAT2X4 295
#define __MAT3X2 296
#define __MAT3X3 297
#define __MAT3X4 298
#define __MAT4X2 299
#define __MAT4X3 300
#define __MAT4X4 301
#define __SAMPLER1D 302
#define __SAMPLER2D 303
#define __SAMPLER3D 304
#define __SAMPLERCUBE 305
#define __SAMPLER1DARRAY 306
#define __SAMPLER2DARRAY 307
#define __SAMPLERCUBEARRAY 308
#define __STRUCT 309
#define __VOID 310
#define __WHILE 311
#define __IDENTIFIER 312
#define __TYPE_NAME 313
#define __FLOATCONSTANT 314
#define __DOUBLECONSTANT 315
#define __INTCONSTANT 316
#define __UINTCONSTANT 317
#define __BOOLCONSTANT 318
#define __FIELD_SELECTION 319
#define __LEFT_OP 320
#define __RIGHT_OP 321
#define __INC_OP 322
#define __DEC_OP 323
#define __LE_OP 324
#define __GE_OP 325
#define __EQ_OP 326
#define __NE_OP 327
#define __AND_OP 328
#define __OR_OP 329
#define __XOR_OP 330
#define __MUL_ASSIGN 331
#define __DIV_ASSIGN 332
#define __ADD_ASSIGN 333
#define __MOD_ASSIGN 334
#define __LEFT_ASSIGN 335
#define __RIGHT_ASSIGN 336
#define __AND_ASSIGN 337
#define __XOR_ASSIGN 338
#define __OR_ASSIGN 339
#define __SUB_ASSIGN 340
#define __LEFT_PAREN 341
#define __RIGHT_PAREN 342
#define __LEFT_BRACKET 343
#define __RIGHT_BRACKET 344
#define __LEFT_BRACE 345
#define __RIGHT_BRACE 346
#define __DOT 347
#define __COMMA 348
#define __COLON 349
#define __EQUAL 350
#define __SEMICOLON 351
#define __BANG 352
#define __DASH 353
#define __TILDE 354
#define __PLUS 355
#define __STAR 356
#define __SLASH 357
#define __PERCENT 358
#define __LEFT_ANGLE 359
#define __RIGHT_ANGLE 360
#define __VERTICAL_BAR 361
#define __CARET 362
#define __AMPERSAND 363
#define __QUESTION 364
#define __INVARIANT 365
#define __PRECISE 366
#define __HIGH_PRECISION 367
#define __MEDIUM_PRECISION 368
#define __LOW_PRECISION 369
#define __PRECISION 370




/* Copy the first part of user declarations.  */


#include "glsl_lex.h"
using namespace GLSL;
#define YYSTYPE GLSLSymbolValue



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

#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef int YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif



/* Copy the second part of user declarations.  */


/* Line 216 of yacc.c.  */


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
# if defined YYENABLE_NLS && YYENABLE_NLS
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
YYID (int i)
#else
static int
YYID (i)
    int i;
#endif
{
  return i;
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
  yytype_int16 yyss;
  YYSTYPE yyvs;
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
# define YYSTACK_RELOCATE(Stack)					\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack, Stack, yysize);				\
	Stack = &yyptr->Stack;						\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

#endif

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  72
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   2447

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  116
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  83
/* YYNRULES -- Number of rules.  */
#define YYNRULES  244
/* YYNRULES -- Number of states.  */
#define YYNSTATES  366

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   370

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
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    98,    99,   100,   101,   102,   103,   104,
     105,   106,   107,   108,   109,   110,   111,   112,   113,   114,
     115
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     5,     8,    10,    12,    15,    18,    21,
      26,    33,    39,    42,    46,    49,    53,    58,    60,    63,
      66,    70,    74,    79,    81,    85,    90,    97,   103,   105,
     108,   112,   118,   123,   125,   128,   130,   133,   135,   137,
     139,   141,   143,   145,   147,   149,   150,   152,   154,   156,
     158,   160,   162,   164,   166,   168,   170,   172,   174,   176,
     178,   180,   182,   184,   186,   188,   190,   192,   194,   196,
     198,   200,   204,   208,   210,   214,   216,   220,   222,   226,
     228,   232,   234,   238,   240,   244,   246,   252,   254,   258,
     260,   264,   269,   271,   275,   277,   279,   281,   283,   285,
     287,   289,   291,   293,   295,   298,   302,   304,   306,   309,
     313,   315,   318,   320,   323,   329,   333,   335,   337,   342,
     349,   351,   353,   357,   360,   366,   374,   380,   382,   384,
     386,   388,   391,   395,   398,   401,   404,   408,   411,   413,
     415,   417,   419,   421,   423,   425,   427,   429,   431,   433,
     435,   439,   441,   443,   445,   447,   449,   452,   456,   461,
     463,   467,   469,   472,   474,   476,   478,   480,   482,   484,
     487,   491,   493,   496,   498,   501,   505,   509,   512,   516,
     519,   521,   524,   526,   528,   530,   532,   534,   536,   538,
     540,   542,   544,   546,   548,   550,   552,   557,   559,   563,
     565,   569,   571,   573,   575,   577,   579,   581,   585,   587,
     592,   594,   598,   601,   604,   606,   608,   612,   616,   620,
     622,   626,   630,   632,   636,   640,   642,   646,   650,   654,
     658,   660,   662,   665,   668,   671,   673,   676,   680,   683,
     685,   687,   689,   692,   695
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int16 yyrhs[] =
{
     117,     0,    -1,   118,    -1,   117,   118,    -1,   119,    -1,
     120,    -1,   171,   147,    -1,   171,    96,    -1,   123,    96,
      -1,   115,   164,   121,    96,    -1,   126,    57,    90,   165,
      91,    96,    -1,   126,    57,    90,   165,    91,    -1,    57,
      96,    -1,    57,   122,    96,    -1,   126,    96,    -1,   126,
      57,    96,    -1,   126,    57,   170,    96,    -1,   130,    -1,
     130,   122,    -1,    88,    89,    -1,    88,   163,    89,    -1,
     122,    88,    89,    -1,   122,    88,   163,    89,    -1,   124,
      -1,   123,    93,    57,    -1,   123,    93,    57,   122,    -1,
     123,    93,    57,   122,    95,   140,    -1,   123,    93,    57,
      95,   140,    -1,   125,    -1,   125,    57,    -1,   125,    57,
     122,    -1,   125,    57,   122,    95,   140,    -1,   125,    57,
      95,   140,    -1,   121,    -1,   126,   121,    -1,   169,    -1,
     126,   169,    -1,    57,    -1,   111,    -1,   100,    -1,    98,
      -1,    97,    -1,    99,    -1,    55,    -1,     5,    -1,    -1,
       7,    -1,     4,    -1,    25,    -1,    26,    -1,    27,    -1,
      28,    -1,    29,    -1,    30,    -1,    38,    -1,    39,    -1,
      40,    -1,    41,    -1,    42,    -1,    43,    -1,    44,    -1,
      45,    -1,    46,    -1,    47,    -1,    48,    -1,    49,    -1,
      50,    -1,    51,    -1,    52,    -1,    53,    -1,   190,    -1,
     131,    71,   190,    -1,   131,    72,   190,    -1,   131,    -1,
     132,   108,   131,    -1,   132,    -1,   133,   107,   132,    -1,
     133,    -1,   134,   106,   133,    -1,   134,    -1,   135,    73,
     134,    -1,   135,    -1,   136,    75,   135,    -1,   136,    -1,
     137,    74,   136,    -1,   137,    -1,   137,   109,   162,    94,
     139,    -1,   138,    -1,   198,   161,   139,    -1,   139,    -1,
      90,   141,    91,    -1,    90,   141,    93,    91,    -1,   140,
      -1,   141,    93,   140,    -1,   120,    -1,   145,    -1,   144,
      -1,   142,    -1,   149,    -1,   150,    -1,   153,    -1,   155,
      -1,   156,    -1,   160,    -1,    90,    91,    -1,    90,   148,
      91,    -1,   147,    -1,   144,    -1,    90,    91,    -1,    90,
     148,    91,    -1,   143,    -1,   148,   143,    -1,    96,    -1,
     162,    96,    -1,    14,    86,   162,    87,   151,    -1,   143,
      12,   143,    -1,   143,    -1,   162,    -1,   125,    57,    95,
     140,    -1,    17,    86,   162,    87,    90,   154,    -1,    91,
      -1,   148,    -1,    18,   162,    94,    -1,    19,    94,    -1,
      56,    86,   152,    87,   146,    -1,    11,   143,    56,    86,
     162,    87,    96,    -1,    13,    86,   157,   159,    87,    -1,
     146,    -1,   149,    -1,   142,    -1,   152,    -1,   158,    96,
      -1,   158,    96,   162,    -1,    10,    96,    -1,     9,    96,
      -1,    16,    96,    -1,    16,   162,    96,    -1,    15,    96,
      -1,    95,    -1,    76,    -1,    77,    -1,    79,    -1,    78,
      -1,    85,    -1,    80,    -1,    81,    -1,    82,    -1,    83,
      -1,    84,    -1,   139,    -1,   162,    93,   139,    -1,   138,
      -1,   112,    -1,   113,    -1,   114,    -1,   166,    -1,   165,
     166,    -1,   121,   167,    96,    -1,   126,   121,   167,    96,
      -1,   168,    -1,   167,    93,   168,    -1,    57,    -1,    57,
     122,    -1,   178,    -1,   181,    -1,   164,    -1,   180,    -1,
     179,    -1,   128,    -1,    93,    57,    -1,   170,    93,    57,
      -1,   174,    -1,   172,    87,    -1,   173,    -1,   174,   176,
      -1,   173,    93,   176,    -1,   125,    57,    86,    -1,   121,
      57,    -1,   121,    57,   122,    -1,   126,   175,    -1,   175,
      -1,   126,   177,    -1,   177,    -1,   121,    -1,     3,    -1,
      34,    -1,    32,    -1,    33,    -1,    31,    -1,    36,    -1,
      37,    -1,    35,    -1,   110,    -1,    21,    -1,    22,    -1,
      23,    -1,    24,    86,   182,    87,    -1,   183,    -1,   182,
      93,   183,    -1,    57,    -1,    57,    95,    61,    -1,   127,
      -1,    61,    -1,    62,    -1,    59,    -1,    63,    -1,    60,
      -1,    86,   162,    87,    -1,   184,    -1,   185,    88,   186,
      89,    -1,   191,    -1,   185,    92,    64,    -1,   185,    67,
      -1,   185,    68,    -1,   162,    -1,   198,    -1,   187,   101,
     198,    -1,   187,   102,   198,    -1,   187,   103,   198,    -1,
     187,    -1,   188,   100,   187,    -1,   188,    98,   187,    -1,
     188,    -1,   189,    65,   188,    -1,   189,    66,   188,    -1,
     189,    -1,   190,   104,   189,    -1,   190,   105,   189,    -1,
     190,    69,   189,    -1,   190,    70,   189,    -1,   192,    -1,
     193,    -1,   195,    87,    -1,   194,    87,    -1,   196,    55,
      -1,   196,    -1,   196,   139,    -1,   195,    93,   139,    -1,
     197,    86,    -1,   121,    -1,   185,    -1,   185,    -1,    67,
     198,    -1,    68,   198,    -1,   129,   198,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   141,   141,   142,   145,   146,   149,   152,   153,   154,
     155,   156,   157,   159,   160,   161,   162,   165,   166,   169,
     170,   171,   172,   175,   176,   177,   178,   179,   182,   183,
     184,   185,   186,   190,   191,   194,   195,   198,   201,   204,
     205,   206,   207,   210,   211,   211,   212,   213,   214,   215,
     216,   217,   218,   219,   220,   221,   222,   223,   224,   225,
     226,   227,   228,   229,   230,   231,   232,   233,   234,   235,
     238,   239,   240,   242,   243,   245,   246,   248,   249,   251,
     252,   254,   255,   257,   258,   260,   261,   263,   264,   267,
     268,   269,   272,   273,   276,   279,   280,   284,   285,   286,
     287,   288,   289,   290,   292,   293,   295,   296,   298,   299,
     301,   302,   304,   305,   307,   309,   310,   313,   314,   316,
     317,   320,   322,   323,   325,   326,   327,   328,   330,   331,
     333,   336,   337,   339,   340,   341,   342,   343,   347,   348,
     349,   350,   351,   352,   353,   354,   355,   356,   357,   359,
     360,   362,   365,   366,   367,   374,   375,   378,   379,   382,
     383,   386,   387,   390,   391,   392,   393,   394,   395,   398,
     399,   402,   403,   406,   408,   409,   411,   413,   414,   416,
     417,   418,   419,   421,   424,   425,   426,   427,   428,   429,
     430,   431,   438,   440,   441,   442,   444,   446,   447,   449,
     450,   453,   454,   455,   456,   457,   458,   459,   461,   462,
     463,   464,   465,   466,   468,   472,   473,   474,   475,   477,
     478,   479,   481,   482,   483,   485,   486,   487,   488,   489,
     492,   494,   496,   497,   499,   500,   502,   503,   506,   512,
     513,   515,   516,   517,   518
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "__CONST", "__BOOL", "__FLOAT",
  "__DOUBLE", "__INT", "__UINT", "__BREAK", "__CONTINUE", "__DO", "__ELSE",
  "__FOR", "__IF", "__DISCARD", "__RETURN", "__SWITCH", "__CASE",
  "__DEFAULT", "__SUBROUTINE", "__SMOOTH", "__FLAT", "__NOPERSPECTIVE",
  "__LAYOUT", "__VEC2", "__VEC3", "__VEC4", "__MAT2", "__MAT3", "__MAT4",
  "__CENTROID", "__IN", "__OUT", "__INOUT", "__UNIFORM", "__PATCH",
  "__SAMPLE", "__MAT2X2", "__MAT2X3", "__MAT2X4", "__MAT3X2", "__MAT3X3",
  "__MAT3X4", "__MAT4X2", "__MAT4X3", "__MAT4X4", "__SAMPLER1D",
  "__SAMPLER2D", "__SAMPLER3D", "__SAMPLERCUBE", "__SAMPLER1DARRAY",
  "__SAMPLER2DARRAY", "__SAMPLERCUBEARRAY", "__STRUCT", "__VOID",
  "__WHILE", "__IDENTIFIER", "__TYPE_NAME", "__FLOATCONSTANT",
  "__DOUBLECONSTANT", "__INTCONSTANT", "__UINTCONSTANT", "__BOOLCONSTANT",
  "__FIELD_SELECTION", "__LEFT_OP", "__RIGHT_OP", "__INC_OP", "__DEC_OP",
  "__LE_OP", "__GE_OP", "__EQ_OP", "__NE_OP", "__AND_OP", "__OR_OP",
  "__XOR_OP", "__MUL_ASSIGN", "__DIV_ASSIGN", "__ADD_ASSIGN",
  "__MOD_ASSIGN", "__LEFT_ASSIGN", "__RIGHT_ASSIGN", "__AND_ASSIGN",
  "__XOR_ASSIGN", "__OR_ASSIGN", "__SUB_ASSIGN", "__LEFT_PAREN",
  "__RIGHT_PAREN", "__LEFT_BRACKET", "__RIGHT_BRACKET", "__LEFT_BRACE",
  "__RIGHT_BRACE", "__DOT", "__COMMA", "__COLON", "__EQUAL", "__SEMICOLON",
  "__BANG", "__DASH", "__TILDE", "__PLUS", "__STAR", "__SLASH",
  "__PERCENT", "__LEFT_ANGLE", "__RIGHT_ANGLE", "__VERTICAL_BAR",
  "__CARET", "__AMPERSAND", "__QUESTION", "__INVARIANT", "__PRECISE",
  "__HIGH_PRECISION", "__MEDIUM_PRECISION", "__LOW_PRECISION",
  "__PRECISION", "$accept", "translation_unit", "external_declaration",
  "function_definition", "declaration", "type_specifier",
  "array_specifier", "init_declarator_list", "single_declaration",
  "fully_specified_type", "type_qualifier", "variable_identifier",
  "precise_qualifier", "unary_operator", "type_specifier_nonarray",
  "equality_expression", "and_expression", "exclusive_or_expression",
  "inclusive_or_expression", "logical_and_expression",
  "logical_xor_expression", "logical_or_expression",
  "conditional_expression", "assignment_expression", "initializer",
  "initializer_list", "declaration_statement", "statement",
  "simple_statement", "compound_statement", "statement_no_new_scope",
  "compound_statement_no_new_scope", "statement_list",
  "expression_statement", "selection_statement",
  "selection_rest_statement", "condition", "switch_statement",
  "switch_statement_list", "case_label", "iteration_statement",
  "for_init_statement", "conditionopt", "for_rest_statement",
  "jump_statement", "assignment_operator", "expression",
  "constant_expression", "precision_qualifier", "struct_declaration_list",
  "struct_declaration", "struct_declarator_list", "struct_declarator",
  "single_type_qualifier", "identifier_list", "function_prototype",
  "function_declarator", "function_header_with_parameters",
  "function_header", "parameter_declarator", "parameter_declaration",
  "parameter_type_specifier", "storage_qualifier", "invariant_qualifier",
  "interpolation_qualifier", "layout_qualifier",
  "layout_qualifier_id_list", "layout_qualifier_id", "primary_expression",
  "postfix_expression", "integer_expression", "multiplicative_expression",
  "additive_expression", "shift_expression", "relational_expression",
  "function_call", "function_call_or_method", "function_call_generic",
  "function_call_header_no_parameters",
  "function_call_header_with_parameters", "function_call_header",
  "function_identifier", "unary_expression", 0
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
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313,   314,
     315,   316,   317,   318,   319,   320,   321,   322,   323,   324,
     325,   326,   327,   328,   329,   330,   331,   332,   333,   334,
     335,   336,   337,   338,   339,   340,   341,   342,   343,   344,
     345,   346,   347,   348,   349,   350,   351,   352,   353,   354,
     355,   356,   357,   358,   359,   360,   361,   362,   363,   364,
     365,   366,   367,   368,   369,   370
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,   116,   117,   117,   118,   118,   119,   120,   120,   120,
     120,   120,   120,   120,   120,   120,   120,   121,   121,   122,
     122,   122,   122,   123,   123,   123,   123,   123,   124,   124,
     124,   124,   124,   125,   125,   126,   126,   127,   128,   129,
     129,   129,   129,   130,   130,   130,   130,   130,   130,   130,
     130,   130,   130,   130,   130,   130,   130,   130,   130,   130,
     130,   130,   130,   130,   130,   130,   130,   130,   130,   130,
     131,   131,   131,   132,   132,   133,   133,   134,   134,   135,
     135,   136,   136,   137,   137,   138,   138,   139,   139,   140,
     140,   140,   141,   141,   142,   143,   143,   144,   144,   144,
     144,   144,   144,   144,   145,   145,   146,   146,   147,   147,
     148,   148,   149,   149,   150,   151,   151,   152,   152,   153,
     153,   154,   155,   155,   156,   156,   156,   156,   157,   157,
     158,   159,   159,   160,   160,   160,   160,   160,   161,   161,
     161,   161,   161,   161,   161,   161,   161,   161,   161,   162,
     162,   163,   164,   164,   164,   165,   165,   166,   166,   167,
     167,   168,   168,   169,   169,   169,   169,   169,   169,   170,
     170,   171,   171,   172,   173,   173,   174,   175,   175,   176,
     176,   176,   176,   177,   178,   178,   178,   178,   178,   178,
     178,   178,   179,   180,   180,   180,   181,   182,   182,   183,
     183,   184,   184,   184,   184,   184,   184,   184,   185,   185,
     185,   185,   185,   185,   186,   187,   187,   187,   187,   188,
     188,   188,   189,   189,   189,   190,   190,   190,   190,   190,
     191,   192,   193,   193,   194,   194,   195,   195,   196,   197,
     197,   198,   198,   198,   198
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     2,     1,     1,     2,     2,     2,     4,
       6,     5,     2,     3,     2,     3,     4,     1,     2,     2,
       3,     3,     4,     1,     3,     4,     6,     5,     1,     2,
       3,     5,     4,     1,     2,     1,     2,     1,     1,     1,
       1,     1,     1,     1,     1,     0,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     3,     3,     1,     3,     1,     3,     1,     3,     1,
       3,     1,     3,     1,     3,     1,     5,     1,     3,     1,
       3,     4,     1,     3,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     2,     3,     1,     1,     2,     3,
       1,     2,     1,     2,     5,     3,     1,     1,     4,     6,
       1,     1,     3,     2,     5,     7,     5,     1,     1,     1,
       1,     2,     3,     2,     2,     2,     3,     2,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       3,     1,     1,     1,     1,     1,     2,     3,     4,     1,
       3,     1,     2,     1,     1,     1,     1,     1,     1,     2,
       3,     1,     2,     1,     2,     3,     3,     2,     3,     2,
       1,     2,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     4,     1,     3,     1,
       3,     1,     1,     1,     1,     1,     1,     3,     1,     4,
       1,     3,     2,     2,     1,     1,     3,     3,     3,     1,
       3,     3,     1,     3,     3,     1,     3,     3,     3,     3,
       1,     1,     2,     2,     2,     1,     2,     3,     2,     1,
       1,     1,     2,     2,     2
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
      45,   184,    47,    44,    46,   193,   194,   195,     0,    48,
      49,    50,    51,    52,    53,   188,   186,   187,   185,   191,
     189,   190,    54,    55,    56,    57,    58,    59,    60,    61,
      62,    63,    64,    65,    66,    67,    68,    69,    43,     0,
     192,    38,   152,   153,   154,     0,    45,     2,     4,     5,
      33,     0,    23,    28,    45,   168,    17,   165,    35,     0,
       0,   173,    45,   163,   167,   166,   164,     0,    45,    12,
       0,    45,     1,     3,     0,     8,    29,     0,    14,    34,
      36,    18,    45,     7,     6,   172,    45,   183,    45,   180,
     174,   182,   199,     0,   197,    37,   204,   206,   202,   203,
     205,    45,    45,    45,    19,    41,    40,    42,    39,   239,
     201,    45,    73,    75,    77,    79,    81,    83,    85,   151,
       0,   208,   241,   219,   222,   225,    70,   210,   230,   231,
       0,     0,    45,     0,   215,    45,    13,     0,    24,   176,
      45,    30,    45,     0,    15,     0,     0,     0,    45,     0,
       0,     0,    45,     0,    45,     0,     0,    37,    45,   108,
     112,    94,    33,    87,   149,    97,   110,    96,    95,   127,
     106,    45,    98,    99,   100,   101,   102,   103,     0,     0,
     215,   175,   177,   179,   181,     0,   196,     0,   242,   243,
       0,   244,    45,    45,    45,    45,    45,    45,    45,    45,
      45,    20,   212,   213,    45,     0,    45,    45,    45,    45,
      45,    45,    45,    45,    45,    45,    45,   233,   232,    45,
      43,   236,   238,    21,     0,     9,    45,    25,    45,    89,
      32,    45,     0,    45,    45,   155,   169,     0,    16,   134,
     133,   120,     0,    45,    45,   137,   135,     0,    45,     0,
     123,    45,   104,    45,   109,   111,    45,   113,   139,   140,
     142,   141,   144,   145,   146,   147,   148,   143,   138,    45,
     178,   200,   198,   207,    71,    72,    74,    76,    78,    80,
      82,    84,     0,   214,     0,   211,   216,   217,   218,   221,
     220,   223,   224,   228,   229,   226,   227,   237,    22,    27,
      45,    92,     0,    31,   161,     0,   159,     0,    11,   156,
     170,     0,   129,   128,    45,     0,   136,     0,   122,     0,
      45,     0,   117,   105,   150,    88,    45,   209,    26,    90,
      45,   162,     0,   157,     0,    10,    45,   130,     0,     0,
      45,     0,     0,    45,    86,    91,    93,   160,   158,     0,
      45,   126,   116,   114,    45,    45,   107,   124,     0,   132,
      45,    45,   119,   118,   125,   115
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,    46,    47,    48,   161,   109,    70,    51,    52,    53,
      54,   110,    55,   111,    56,   112,   113,   114,   115,   116,
     117,   118,   163,   164,   230,   302,   165,   166,   167,   168,
     169,   170,   171,   172,   173,   353,   321,   174,   362,   175,
     176,   314,   338,   339,   177,   269,   178,   120,    57,   234,
     235,   305,   306,    58,   145,   179,    60,    61,    62,    89,
      90,    91,    63,    64,    65,    66,    93,    94,   121,   122,
     284,   123,   124,   125,   126,   127,   128,   129,   130,   131,
     132,   133,   180
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -248
static const yytype_int16 yypact[] =
{
    1328,  -248,  -248,  -248,  -248,  -248,  -248,  -248,   -74,  -248,
    -248,  -248,  -248,  -248,  -248,  -248,  -248,  -248,  -248,  -248,
    -248,  -248,  -248,  -248,  -248,  -248,  -248,  -248,  -248,  -248,
    -248,  -248,  -248,  -248,  -248,  -248,  -248,  -248,  -248,   -65,
    -248,  -248,  -248,  -248,  -248,     4,   341,  -248,  -248,  -248,
    -248,   -28,  -248,    34,  1521,  -248,   -52,  -248,  -248,   -43,
     -26,     6,  1615,  -248,  -248,  -248,  -248,    57,  1956,  -248,
     -62,   395,  -248,  -248,    62,  -248,   -71,   -51,  -248,  -248,
    -248,    41,   567,  -248,  -248,  -248,  1803,    74,  1803,  -248,
    -248,  -248,    54,   -38,  -248,  -248,  -248,  -248,  -248,  -248,
    -248,  2347,  2347,  2347,  -248,  -248,  -248,  -248,  -248,  -248,
    -248,  2347,   -20,    44,    46,    53,    87,    86,   -66,  -248,
      73,  -248,   -48,    20,   -13,    38,   -41,  -248,  -248,  -248,
      77,   -37,  2021,    79,  -248,  2086,  -248,    67,   -63,  -248,
    2151,   -58,  1803,   109,  -248,   -16,    71,    72,   680,    83,
      84,    80,  2217,    93,  2347,    78,    95,   -65,   793,  -248,
    -248,  -248,    96,  -248,  -248,  -248,  -248,  -248,  -248,  -248,
    -248,   906,  -248,  -248,  -248,  -248,  -248,  -248,   -15,    88,
      56,  -248,   -52,  -248,  -248,   112,  -248,    57,  -248,  -248,
     -21,  -248,  2347,  2347,  2347,  2347,  2347,  2347,  2347,  2347,
    2347,  -248,  -248,  -248,  2347,   119,  2347,  2347,  2347,  2347,
    2347,  2347,  2347,  2347,  2347,  2347,  2347,  -248,  -248,  2347,
      98,  -248,  -248,  -248,    97,  -248,  2151,   -47,  2151,  -248,
    -248,  2151,   131,  1803,  1709,  -248,  -248,   132,  -248,  -248,
    -248,  -248,   134,  1230,  2347,  -248,  -248,    -4,  2347,    19,
    -248,  1423,  -248,  1019,  -248,  -248,  2347,  -248,  -248,  -248,
    -248,  -248,  -248,  -248,  -248,  -248,  -248,  -248,  -248,  2347,
      41,  -248,  -248,  -248,   -41,   -41,   -20,    44,    46,    53,
      87,    86,    31,   100,   102,  -248,  -248,  -248,  -248,    20,
      20,   -13,   -13,    38,    38,    38,    38,  -248,  -248,  -248,
    2151,  -248,    16,  -248,   -52,     5,  -248,   131,    99,  -248,
    -248,   108,  -248,  -248,  1423,   -18,  -248,   -17,  -248,   139,
    1803,   110,   100,  -248,  -248,  -248,  2347,  -248,  -248,  -248,
    1880,    41,   131,  -248,    15,  -248,  2347,  -248,   103,   111,
     680,   113,   114,  1132,  -248,  -248,  -248,  -248,  -248,   -14,
    2282,  -248,   193,  -248,   680,  2151,  -248,  -248,   116,   100,
     680,   454,  -248,  -248,  -248,  -248
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -248,  -248,   160,  -248,    11,     0,   -54,  -248,  -248,  -247,
     -59,  -248,  -248,  -248,  -248,    14,    12,    17,    18,    21,
      22,  -248,   -61,  -126,  -210,  -248,   -33,  -138,  -129,  -248,
    -127,   158,  -153,   -25,  -248,  -248,   -90,  -248,  -248,  -248,
    -248,  -248,  -248,  -248,  -248,  -248,   -94,    90,   181,  -248,
      -7,   -79,  -102,   -53,  -248,    13,  -248,  -248,  -248,   143,
     146,   147,  -248,  -248,  -248,  -248,  -248,    49,  -248,  -248,
    -248,   -83,   -56,  -119,   -46,  -248,  -248,  -248,  -248,  -248,
    -248,  -248,    76
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -241
static const yytype_int16 yytable[] =
{
      50,    80,    81,    88,   319,   253,   221,   119,   199,   190,
     242,    49,    67,    59,   229,   139,   299,    68,   301,   202,
     203,   303,   141,    68,   140,    68,   135,    88,   213,   214,
     135,    69,   226,   255,   136,    80,    68,   231,  -240,   142,
     204,   135,   143,   200,   205,   144,    50,    82,   300,   186,
     218,   192,   193,    83,    79,   187,   219,    49,   247,    59,
     249,    85,    87,   215,   216,    74,   273,   319,    75,   340,
     341,   137,   256,   358,   119,   256,   256,   237,   256,   256,
     238,   257,   162,   233,   227,   209,    87,   210,    87,   256,
     328,    76,   316,   297,   293,   294,   295,   296,   332,    86,
     229,   333,   229,   211,   212,   229,   282,   329,   332,   330,
     283,   348,   256,   318,    92,   255,    42,    43,    44,   138,
     346,   206,   207,   208,   256,   326,   289,   290,   270,   135,
     324,   182,   258,   259,   260,   261,   262,   263,   264,   265,
     266,   267,   232,   325,   134,   363,   274,   275,   162,   185,
     315,   268,   194,   195,   317,   291,   292,   322,   162,   196,
     197,   198,   201,   225,   217,   222,   236,   239,   240,   243,
     244,   162,   250,   271,   229,   233,   245,   188,   189,   248,
      80,   251,  -239,   285,    83,  -234,   298,   191,   304,   310,
     311,   327,   320,   256,   336,   335,   342,   343,   351,   350,
     344,   361,   352,   354,   229,   360,    73,   277,   276,   355,
     312,   134,   364,   278,   356,   279,   357,    84,   313,   280,
     322,   281,   365,   255,   337,   224,    71,   309,   334,   229,
     347,   183,   181,   307,   232,   184,   272,     0,     0,     0,
       0,     0,   349,   162,     0,     0,     0,     0,     0,     0,
     331,   162,     0,   162,     0,   320,   359,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    80,   134,   134,
     134,   134,   134,   134,   134,   134,     0,     0,     0,     0,
       0,     0,   286,   287,   288,   134,   134,   134,   134,   134,
     134,   134,   134,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   162,     0,     0,     0,     0,     0,
      79,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     162,    72,     0,   162,     1,     2,     3,     0,     4,     0,
       0,     0,     0,     0,   162,     0,     0,     0,     0,     0,
     162,   162,     5,     6,     7,     8,     9,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    32,
      33,    34,    35,    36,    37,     0,    38,     0,    39,     2,
       3,     0,     4,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       9,    10,    11,    12,    13,    14,     0,     0,     0,     0,
       0,     0,     0,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    32,    33,    34,    35,    36,    37,     0,
      38,    40,    41,    42,    43,    44,    45,     1,     2,     3,
       0,     4,     0,   146,   147,   148,  -121,   149,   150,   151,
     152,   153,   154,   155,     0,     5,     6,     7,     8,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    33,    34,    35,    36,    37,     0,    38,
     156,   157,     0,    96,    97,    98,    99,   100,     0,     0,
       0,   101,   102,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     103,     0,     0,     0,   158,   241,     0,     0,     0,     0,
     160,   105,   106,   107,   108,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    40,    41,    42,    43,    44,    45,
       1,     2,     3,     0,     4,     0,   146,   147,   148,     0,
     149,   150,   151,   152,   153,   154,   155,     0,     5,     6,
       7,     8,     9,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    32,    33,    34,    35,    36,
      37,     0,    38,   156,   157,     0,    96,    97,    98,    99,
     100,     0,     0,     0,   101,   102,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   103,     0,     0,     0,   158,   159,     0,
       0,     0,     0,   160,   105,   106,   107,   108,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    40,    41,    42,
      43,    44,    45,     1,     2,     3,     0,     4,     0,   146,
     147,   148,     0,   149,   150,   151,   152,   153,   154,   155,
       0,     5,     6,     7,     8,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    33,
      34,    35,    36,    37,     0,    38,   156,   157,     0,    96,
      97,    98,    99,   100,     0,     0,     0,   101,   102,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   103,     0,     0,     0,
     158,   241,     0,     0,     0,     0,   160,   105,   106,   107,
     108,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      40,    41,    42,    43,    44,    45,     1,     2,     3,     0,
       4,     0,   146,   147,   148,     0,   149,   150,   151,   152,
     153,   154,   155,     0,     5,     6,     7,     8,     9,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    32,    33,    34,    35,    36,    37,     0,    38,   156,
     157,     0,    96,    97,    98,    99,   100,     0,     0,     0,
     101,   102,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   103,
       0,     0,     0,   158,   252,     0,     0,     0,     0,   160,
     105,   106,   107,   108,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    40,    41,    42,    43,    44,    45,     1,
       2,     3,     0,     4,     0,   146,   147,   148,     0,   149,
     150,   151,   152,   153,   154,   155,     0,     5,     6,     7,
       8,     9,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    33,    34,    35,    36,    37,
       0,    38,   156,   157,     0,    96,    97,    98,    99,   100,
       0,     0,     0,   101,   102,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   103,     0,     0,     0,   158,   254,     0,     0,
       0,     0,   160,   105,   106,   107,   108,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    40,    41,    42,    43,
      44,    45,     1,     2,     3,     0,     4,     0,   146,   147,
     148,     0,   149,   150,   151,   152,   153,   154,   155,     0,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,     0,    38,   156,   157,     0,    96,    97,
      98,    99,   100,     0,     0,     0,   101,   102,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   103,     0,     0,     0,   158,
     323,     0,     0,     0,     0,   160,   105,   106,   107,   108,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    40,
      41,    42,    43,    44,    45,     1,     2,     3,     0,     4,
       0,   146,   147,   148,     0,   149,   150,   151,   152,   153,
     154,   155,     0,     5,     6,     7,     8,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    33,    34,    35,    36,    37,     0,    38,   156,   157,
       0,    96,    97,    98,    99,   100,     0,     0,     0,   101,
     102,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   103,     0,
       0,     0,    82,   241,     0,     0,     0,     0,   160,   105,
     106,   107,   108,     1,     2,     3,     0,     4,     0,     0,
       0,     0,    40,    41,    42,    43,    44,    45,     0,     0,
       0,     5,     6,     7,     8,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    33,
      34,    35,    36,    37,     0,    38,     0,   157,     0,    96,
      97,    98,    99,   100,     0,     0,     0,   101,   102,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   103,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   160,   105,   106,   107,
     108,     1,     2,     3,     0,     4,     0,     0,     0,     0,
      40,    41,    42,    43,    44,    45,     0,     0,     0,     5,
       6,     7,     8,     9,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    31,    32,    33,    34,    35,
      36,    37,     0,    38,     0,    39,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     1,     2,     3,     0,
       4,     0,     0,     0,     0,     0,     0,     0,    40,    41,
      42,    43,    44,    45,     5,     6,     7,     8,     9,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    32,    33,    34,    35,    36,    37,     0,    38,     0,
      95,     0,    96,    97,    98,    99,   100,     0,     0,     0,
     101,   102,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   103,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     105,   106,   107,   108,     1,     2,     3,     0,     4,     0,
       0,     0,     0,    40,    41,    42,    43,    44,     0,     0,
       0,     0,     5,     6,     7,     8,     9,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    32,
      33,    34,    35,    36,    37,     0,    38,     0,    77,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    78,     1,     2,
       3,     0,     4,     0,     0,     0,     0,     0,     0,     0,
       0,    40,    41,    42,    43,    44,     5,     6,     7,     8,
       9,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    32,    33,    34,    35,    36,    37,     0,
      38,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  -171,     0,     0,     0,     0,
       0,  -171,     1,     2,     3,     0,     4,     0,     0,     0,
       0,     0,     0,     0,     0,    40,    41,    42,    43,    44,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,     0,    38,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     308,     0,     0,     0,     0,     0,     1,     2,     3,     0,
       4,     0,     0,     0,     0,     0,     0,     0,     0,    40,
      41,    42,    43,    44,     5,     6,     7,     8,     9,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    32,    33,    34,    35,    36,    37,     0,    38,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     2,     3,     0,     4,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     9,    10,    11,    12,    13,
      14,     0,     0,    40,    41,    42,    43,    44,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    33,
      34,    35,    36,    37,     0,    38,     0,    95,     0,    96,
      97,    98,    99,   100,     0,     0,     0,   101,   102,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       2,     3,     0,     4,     0,     0,   103,     0,     0,     0,
     228,   345,     0,     0,     0,     0,     0,   105,   106,   107,
     108,     9,    10,    11,    12,    13,    14,     0,     0,     0,
       0,     0,     0,     0,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    33,    34,    35,    36,    37,
       0,    38,     0,    95,     0,    96,    97,    98,    99,   100,
       0,     0,     0,   101,   102,     2,     3,     0,     4,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   103,     0,     0,   104,     9,    10,    11,    12,
      13,    14,     0,   105,   106,   107,   108,     0,     0,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    32,
      33,    34,    35,    36,    37,     0,   220,     0,    95,     0,
      96,    97,    98,    99,   100,     0,     0,     0,   101,   102,
       2,     3,     0,     4,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   103,  -235,     0,
       0,     9,    10,    11,    12,    13,    14,     0,   105,   106,
     107,   108,     0,     0,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    33,    34,    35,    36,    37,
       0,    38,     0,    95,     0,    96,    97,    98,    99,   100,
       0,     0,     0,   101,   102,     2,     3,     0,     4,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   103,     0,     0,   223,     9,    10,    11,    12,
      13,    14,     0,   105,   106,   107,   108,     0,     0,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    32,
      33,    34,    35,    36,    37,     0,    38,     0,    95,     0,
      96,    97,    98,    99,   100,     0,     0,     0,   101,   102,
       0,     2,     3,     0,     4,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   103,     0,     0,
       0,   228,     9,    10,    11,    12,    13,    14,   105,   106,
     107,   108,     0,     0,     0,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    32,    33,    34,    35,    36,
      37,     0,    38,     0,    95,     0,    96,    97,    98,    99,
     100,     0,     0,     0,   101,   102,     2,     3,     0,     4,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   103,     0,     0,     0,     9,    10,    11,
      12,    13,    14,   246,   105,   106,   107,   108,     0,     0,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    33,    34,    35,    36,    37,     0,    38,     0,    95,
       0,    96,    97,    98,    99,   100,     0,     0,     0,   101,
     102,     2,     3,     0,     4,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   103,  -131,
       0,     0,     9,    10,    11,    12,    13,    14,     0,   105,
     106,   107,   108,     0,     0,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    32,    33,    34,    35,    36,
      37,     0,    38,     0,    95,     0,    96,    97,    98,    99,
     100,     0,     0,     0,   101,   102,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   103,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   105,   106,   107,   108
};

static const yytype_int16 yycheck[] =
{
       0,    54,    56,    62,   251,   158,   132,    68,    74,   103,
     148,     0,    86,     0,   140,    86,   226,    88,   228,    67,
      68,   231,    76,    88,    95,    88,    88,    86,    69,    70,
      88,    96,    95,   171,    96,    88,    88,    95,    86,    90,
      88,    88,    93,   109,    92,    96,    46,    90,    95,    87,
      87,    71,    72,    96,    54,    93,    93,    46,   152,    46,
     154,    87,    62,   104,   105,    93,    87,   314,    96,    87,
      87,    71,    93,    87,   135,    93,    93,    93,    93,    93,
      96,    96,    82,   142,   138,    98,    86,   100,    88,    93,
     300,    57,    96,   219,   213,   214,   215,   216,    93,    93,
     226,    96,   228,    65,    66,   231,   200,    91,    93,    93,
     204,    96,    93,    94,    57,   253,   112,   113,   114,    57,
     330,   101,   102,   103,    93,    94,   209,   210,   182,    88,
     256,    57,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,   142,   269,    68,   355,   192,   193,   148,    95,
     244,    95,   108,   107,   248,   211,   212,   251,   158,   106,
      73,    75,    89,    96,    87,    86,    57,    96,    96,    86,
      86,   171,    94,    61,   300,   234,    96,   101,   102,    86,
     233,    86,    86,    64,    96,    87,    89,   111,    57,    57,
      56,    89,   251,    93,    86,    96,    57,    87,    87,    96,
     326,   354,   340,    90,   330,    12,    46,   195,   194,    95,
     243,   135,    96,   196,   343,   197,   343,    59,   243,   198,
     314,   199,   360,   361,   314,   135,    45,   234,   307,   355,
     332,    88,    86,   233,   234,    88,   187,    -1,    -1,    -1,
      -1,    -1,   336,   243,    -1,    -1,    -1,    -1,    -1,    -1,
     304,   251,    -1,   253,    -1,   314,   350,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   320,   192,   193,
     194,   195,   196,   197,   198,   199,    -1,    -1,    -1,    -1,
      -1,    -1,   206,   207,   208,   209,   210,   211,   212,   213,
     214,   215,   216,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   314,    -1,    -1,    -1,    -1,    -1,
     320,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     340,     0,    -1,   343,     3,     4,     5,    -1,     7,    -1,
      -1,    -1,    -1,    -1,   354,    -1,    -1,    -1,    -1,    -1,
     360,   361,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    32,    33,    34,    35,    36,    37,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    47,    48,
      49,    50,    51,    52,    53,    -1,    55,    -1,    57,     4,
       5,    -1,     7,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      25,    26,    27,    28,    29,    30,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    -1,
      55,   110,   111,   112,   113,   114,   115,     3,     4,     5,
      -1,     7,    -1,     9,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    -1,    21,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    31,    32,    33,    34,    35,
      36,    37,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    47,    48,    49,    50,    51,    52,    53,    -1,    55,
      56,    57,    -1,    59,    60,    61,    62,    63,    -1,    -1,
      -1,    67,    68,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      86,    -1,    -1,    -1,    90,    91,    -1,    -1,    -1,    -1,
      96,    97,    98,    99,   100,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   110,   111,   112,   113,   114,   115,
       3,     4,     5,    -1,     7,    -1,     9,    10,    11,    -1,
      13,    14,    15,    16,    17,    18,    19,    -1,    21,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    32,
      33,    34,    35,    36,    37,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    47,    48,    49,    50,    51,    52,
      53,    -1,    55,    56,    57,    -1,    59,    60,    61,    62,
      63,    -1,    -1,    -1,    67,    68,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    86,    -1,    -1,    -1,    90,    91,    -1,
      -1,    -1,    -1,    96,    97,    98,    99,   100,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   110,   111,   112,
     113,   114,   115,     3,     4,     5,    -1,     7,    -1,     9,
      10,    11,    -1,    13,    14,    15,    16,    17,    18,    19,
      -1,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    33,    34,    35,    36,    37,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    47,    48,    49,
      50,    51,    52,    53,    -1,    55,    56,    57,    -1,    59,
      60,    61,    62,    63,    -1,    -1,    -1,    67,    68,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    86,    -1,    -1,    -1,
      90,    91,    -1,    -1,    -1,    -1,    96,    97,    98,    99,
     100,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     110,   111,   112,   113,   114,   115,     3,     4,     5,    -1,
       7,    -1,     9,    10,    11,    -1,    13,    14,    15,    16,
      17,    18,    19,    -1,    21,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    32,    33,    34,    35,    36,
      37,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      47,    48,    49,    50,    51,    52,    53,    -1,    55,    56,
      57,    -1,    59,    60,    61,    62,    63,    -1,    -1,    -1,
      67,    68,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    86,
      -1,    -1,    -1,    90,    91,    -1,    -1,    -1,    -1,    96,
      97,    98,    99,   100,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   110,   111,   112,   113,   114,   115,     3,
       4,     5,    -1,     7,    -1,     9,    10,    11,    -1,    13,
      14,    15,    16,    17,    18,    19,    -1,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    33,
      34,    35,    36,    37,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    47,    48,    49,    50,    51,    52,    53,
      -1,    55,    56,    57,    -1,    59,    60,    61,    62,    63,
      -1,    -1,    -1,    67,    68,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    86,    -1,    -1,    -1,    90,    91,    -1,    -1,
      -1,    -1,    96,    97,    98,    99,   100,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   110,   111,   112,   113,
     114,   115,     3,     4,     5,    -1,     7,    -1,     9,    10,
      11,    -1,    13,    14,    15,    16,    17,    18,    19,    -1,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    32,    33,    34,    35,    36,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    47,    48,    49,    50,
      51,    52,    53,    -1,    55,    56,    57,    -1,    59,    60,
      61,    62,    63,    -1,    -1,    -1,    67,    68,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    86,    -1,    -1,    -1,    90,
      91,    -1,    -1,    -1,    -1,    96,    97,    98,    99,   100,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   110,
     111,   112,   113,   114,   115,     3,     4,     5,    -1,     7,
      -1,     9,    10,    11,    -1,    13,    14,    15,    16,    17,
      18,    19,    -1,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    33,    34,    35,    36,    37,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    47,
      48,    49,    50,    51,    52,    53,    -1,    55,    56,    57,
      -1,    59,    60,    61,    62,    63,    -1,    -1,    -1,    67,
      68,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    86,    -1,
      -1,    -1,    90,    91,    -1,    -1,    -1,    -1,    96,    97,
      98,    99,   100,     3,     4,     5,    -1,     7,    -1,    -1,
      -1,    -1,   110,   111,   112,   113,   114,   115,    -1,    -1,
      -1,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    33,    34,    35,    36,    37,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    47,    48,    49,
      50,    51,    52,    53,    -1,    55,    -1,    57,    -1,    59,
      60,    61,    62,    63,    -1,    -1,    -1,    67,    68,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    86,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    96,    97,    98,    99,
     100,     3,     4,     5,    -1,     7,    -1,    -1,    -1,    -1,
     110,   111,   112,   113,   114,   115,    -1,    -1,    -1,    21,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    33,    34,    35,    36,    37,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    47,    48,    49,    50,    51,
      52,    53,    -1,    55,    -1,    57,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,    -1,
       7,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   110,   111,
     112,   113,   114,   115,    21,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    32,    33,    34,    35,    36,
      37,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      47,    48,    49,    50,    51,    52,    53,    -1,    55,    -1,
      57,    -1,    59,    60,    61,    62,    63,    -1,    -1,    -1,
      67,    68,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    86,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      97,    98,    99,   100,     3,     4,     5,    -1,     7,    -1,
      -1,    -1,    -1,   110,   111,   112,   113,   114,    -1,    -1,
      -1,    -1,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    32,    33,    34,    35,    36,    37,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    47,    48,
      49,    50,    51,    52,    53,    -1,    55,    -1,    57,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    96,     3,     4,
       5,    -1,     7,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   110,   111,   112,   113,   114,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    -1,
      55,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    90,    -1,    -1,    -1,    -1,
      -1,    96,     3,     4,     5,    -1,     7,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   110,   111,   112,   113,   114,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    32,    33,    34,    35,    36,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    47,    48,    49,    50,
      51,    52,    53,    -1,    55,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      91,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,    -1,
       7,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   110,
     111,   112,   113,   114,    21,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    32,    33,    34,    35,    36,
      37,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      47,    48,    49,    50,    51,    52,    53,    -1,    55,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,     4,     5,    -1,     7,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    25,    26,    27,    28,    29,
      30,    -1,    -1,   110,   111,   112,   113,   114,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    47,    48,    49,
      50,    51,    52,    53,    -1,    55,    -1,    57,    -1,    59,
      60,    61,    62,    63,    -1,    -1,    -1,    67,    68,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
       4,     5,    -1,     7,    -1,    -1,    86,    -1,    -1,    -1,
      90,    91,    -1,    -1,    -1,    -1,    -1,    97,    98,    99,
     100,    25,    26,    27,    28,    29,    30,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    47,    48,    49,    50,    51,    52,    53,
      -1,    55,    -1,    57,    -1,    59,    60,    61,    62,    63,
      -1,    -1,    -1,    67,    68,     4,     5,    -1,     7,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    86,    -1,    -1,    89,    25,    26,    27,    28,
      29,    30,    -1,    97,    98,    99,   100,    -1,    -1,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    47,    48,
      49,    50,    51,    52,    53,    -1,    55,    -1,    57,    -1,
      59,    60,    61,    62,    63,    -1,    -1,    -1,    67,    68,
       4,     5,    -1,     7,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    86,    87,    -1,
      -1,    25,    26,    27,    28,    29,    30,    -1,    97,    98,
      99,   100,    -1,    -1,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    47,    48,    49,    50,    51,    52,    53,
      -1,    55,    -1,    57,    -1,    59,    60,    61,    62,    63,
      -1,    -1,    -1,    67,    68,     4,     5,    -1,     7,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    86,    -1,    -1,    89,    25,    26,    27,    28,
      29,    30,    -1,    97,    98,    99,   100,    -1,    -1,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    47,    48,
      49,    50,    51,    52,    53,    -1,    55,    -1,    57,    -1,
      59,    60,    61,    62,    63,    -1,    -1,    -1,    67,    68,
      -1,     4,     5,    -1,     7,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    86,    -1,    -1,
      -1,    90,    25,    26,    27,    28,    29,    30,    97,    98,
      99,   100,    -1,    -1,    -1,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    47,    48,    49,    50,    51,    52,
      53,    -1,    55,    -1,    57,    -1,    59,    60,    61,    62,
      63,    -1,    -1,    -1,    67,    68,     4,     5,    -1,     7,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    86,    -1,    -1,    -1,    25,    26,    27,
      28,    29,    30,    96,    97,    98,    99,   100,    -1,    -1,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    47,
      48,    49,    50,    51,    52,    53,    -1,    55,    -1,    57,
      -1,    59,    60,    61,    62,    63,    -1,    -1,    -1,    67,
      68,     4,     5,    -1,     7,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    86,    87,
      -1,    -1,    25,    26,    27,    28,    29,    30,    -1,    97,
      98,    99,   100,    -1,    -1,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    47,    48,    49,    50,    51,    52,
      53,    -1,    55,    -1,    57,    -1,    59,    60,    61,    62,
      63,    -1,    -1,    -1,    67,    68,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    86,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    97,    98,    99,   100
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     3,     4,     5,     7,    21,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    31,    32,    33,    34,    35,
      36,    37,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    47,    48,    49,    50,    51,    52,    53,    55,    57,
     110,   111,   112,   113,   114,   115,   117,   118,   119,   120,
     121,   123,   124,   125,   126,   128,   130,   164,   169,   171,
     172,   173,   174,   178,   179,   180,   181,    86,    88,    96,
     122,   164,     0,   118,    93,    96,    57,    57,    96,   121,
     169,   122,    90,    96,   147,    87,    93,   121,   126,   175,
     176,   177,    57,   182,   183,    57,    59,    60,    61,    62,
      63,    67,    68,    86,    89,    97,    98,    99,   100,   121,
     127,   129,   131,   132,   133,   134,   135,   136,   137,   138,
     163,   184,   185,   187,   188,   189,   190,   191,   192,   193,
     194,   195,   196,   197,   198,    88,    96,   121,    57,    86,
      95,   122,    90,    93,    96,   170,     9,    10,    11,    13,
      14,    15,    16,    17,    18,    19,    56,    57,    90,    91,
      96,   120,   121,   138,   139,   142,   143,   144,   145,   146,
     147,   148,   149,   150,   153,   155,   156,   160,   162,   171,
     198,   176,    57,   175,   177,    95,    87,    93,   198,   198,
     162,   198,    71,    72,   108,   107,   106,    73,    75,    74,
     109,    89,    67,    68,    88,    92,   101,   102,   103,    98,
     100,    65,    66,    69,    70,   104,   105,    87,    87,    93,
      55,   139,    86,    89,   163,    96,    95,   122,    90,   139,
     140,    95,   121,   126,   165,   166,    57,    93,    96,    96,
      96,    91,   143,    86,    86,    96,    96,   162,    86,   162,
      94,    86,    91,   148,    91,   143,    93,    96,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    95,   161,
     122,    61,   183,    87,   190,   190,   131,   132,   133,   134,
     135,   136,   162,   162,   186,    64,   198,   198,   198,   187,
     187,   188,   188,   189,   189,   189,   189,   139,    89,   140,
      95,   140,   141,   140,    57,   167,   168,   121,    91,   166,
      57,    56,   142,   149,   157,   162,    96,   162,    94,   125,
     126,   152,   162,    91,   139,   139,    94,    89,   140,    91,
      93,   122,    93,    96,   167,    96,    86,   152,   158,   159,
      87,    87,    57,    87,   139,    91,   140,   168,    96,   162,
      96,    87,   143,   151,    90,    95,   144,   146,    87,   162,
      12,   148,   154,   140,    96,   143
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
# if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL
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
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, GLSLParserEnv* e)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep, e)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
    GLSLParserEnv* e;
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
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, GLSLParserEnv* e)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep, e)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
    GLSLParserEnv* e;
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
yy_stack_print (yytype_int16 *bottom, yytype_int16 *top)
#else
static void
yy_stack_print (bottom, top)
    yytype_int16 *bottom;
    yytype_int16 *top;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; bottom <= top; ++bottom)
    YYFPRINTF (stderr, " %d", *bottom);
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
yy_reduce_print (YYSTYPE *yyvsp, int yyrule, GLSLParserEnv* e)
#else
static void
yy_reduce_print (yyvsp, yyrule, e)
    YYSTYPE *yyvsp;
    int yyrule;
    GLSLParserEnv* e;
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
      fprintf (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       		       , e);
      fprintf (stderr, "\n");
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
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep, GLSLParserEnv* e)
#else
static void
yydestruct (yymsg, yytype, yyvaluep, e)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
    GLSLParserEnv* e;
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
int yyparse (GLSLParserEnv* e);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */






/*----------.
| yyparse.  |
`----------*/

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
yyparse (GLSLParserEnv* e)
#else
int
yyparse (e)
    GLSLParserEnv* e;
#endif
#endif
{
  /* The look-ahead symbol.  */
int yychar;

/* The semantic value of the look-ahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;

  int yystate;
  int yyn;
  int yyresult;
  /* Number of tokens to shift before error messages enabled.  */
  int yyerrstatus;
  /* Look-ahead token as an internal (translated) token number.  */
  int yytoken = 0;
#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

  /* Three stacks and their tools:
     `yyss': related to states,
     `yyvs': related to semantic values,
     `yyls': related to locations.

     Refer to the stacks thru separate pointers, to allow yyoverflow
     to reallocate them elsewhere.  */

  /* The state stack.  */
  yytype_int16 yyssa[YYINITDEPTH];
  yytype_int16 *yyss = yyssa;
  yytype_int16 *yyssp;

  /* The semantic value stack.  */
  YYSTYPE yyvsa[YYINITDEPTH];
  YYSTYPE *yyvs = yyvsa;
  YYSTYPE *yyvsp;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  YYSIZE_T yystacksize = YYINITDEPTH;

  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;


  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY;		/* Cause a token to be read.  */

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
	YYSTACK_RELOCATE (yyss);
	YYSTACK_RELOCATE (yyvs);

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

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     look-ahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to look-ahead token.  */
  yyn = yypact[yystate];
  if (yyn == YYPACT_NINF)
    goto yydefault;

  /* Not known => get a look-ahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid look-ahead symbol.  */
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

  if (yyn == YYFINAL)
    YYACCEPT;

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the look-ahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token unless it is eof.  */
  if (yychar != YYEOF)
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
      
/* Line 1267 of yacc.c.  */

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
      /* If just tried and failed to reuse look-ahead token after an
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

  /* Else will try to reuse look-ahead token after shifting the error
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

  if (yyn == YYFINAL)
    YYACCEPT;

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

#ifndef yyoverflow
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (e, YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEOF && yychar != YYEMPTY)
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




