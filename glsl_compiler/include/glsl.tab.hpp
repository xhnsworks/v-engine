
/* A Bison parser, made by GNU Bison 2.4.1.  */

/* Skeleton interface for Bison's Yacc-like parsers in C
   
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


/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     _CONST = 258,
     _BOOL = 259,
     _FLOAT = 260,
     _DOUBLE = 261,
     _INT = 262,
     _UINT = 263,
     _BREAK = 264,
     _CONTINUE = 265,
     _DO = 266,
     _ELSE = 267,
     _FOR = 268,
     _IF = 269,
     _DISCARD = 270,
     _RETURN = 271,
     _SWITCH = 272,
     _CASE = 273,
     _DEFAULT = 274,
     _SUBROUTINE = 275,
     _SMOOTH = 276,
     _FLAT = 277,
     _NOPERSPECTIVE = 278,
     _LAYOUT = 279,
     _VEC2 = 280,
     _VEC3 = 281,
     _VEC4 = 282,
     _MAT2 = 283,
     _MAT3 = 284,
     _MAT4 = 285,
     _CENTROID = 286,
     _IN = 287,
     _OUT = 288,
     _INOUT = 289,
     _UNIFORM = 290,
     _PATCH = 291,
     _SAMPLE = 292,
     _MAT2X2 = 293,
     _MAT2X3 = 294,
     _MAT2X4 = 295,
     _MAT3X2 = 296,
     _MAT3X3 = 297,
     _MAT3X4 = 298,
     _MAT4X2 = 299,
     _MAT4X3 = 300,
     _MAT4X4 = 301,
     _SAMPLER1D = 302,
     _SAMPLER2D = 303,
     _SAMPLER3D = 304,
     _SAMPLERCUBE = 305,
     _SAMPLER1DARRAY = 306,
     _SAMPLER2DARRAY = 307,
     _SAMPLERCUBEARRAY = 308,
     _STRUCT = 309,
     _VOID = 310,
     _WHILE = 311,
     _IDENTIFIER = 312,
     _TYPE_NAME = 313,
     _FLOATCONSTANT = 314,
     _DOUBLECONSTANT = 315,
     _INTCONSTANT = 316,
     _UINTCONSTANT = 317,
     _BOOLCONSTANT = 318,
     _FIELD_SELECTION = 319,
     _LEFT_OP = 320,
     _RIGHT_OP = 321,
     _INC_OP = 322,
     _DEC_OP = 323,
     _LE_OP = 324,
     _GE_OP = 325,
     _EQ_OP = 326,
     _NE_OP = 327,
     _AND_OP = 328,
     _OR_OP = 329,
     _XOR_OP = 330,
     _MUL_ASSIGN = 331,
     _DIV_ASSIGN = 332,
     _ADD_ASSIGN = 333,
     _MOD_ASSIGN = 334,
     _LEFT_ASSIGN = 335,
     _RIGHT_ASSIGN = 336,
     _AND_ASSIGN = 337,
     _XOR_ASSIGN = 338,
     _OR_ASSIGN = 339,
     _SUB_ASSIGN = 340,
     _LEFT_PAREN = 341,
     _RIGHT_PAREN = 342,
     _LEFT_BRACKET = 343,
     _RIGHT_BRACKET = 344,
     _LEFT_BRACE = 345,
     _RIGHT_BRACE = 346,
     _DOT = 347,
     _COMMA = 348,
     _COLON = 349,
     _EQUAL = 350,
     _SEMICOLON = 351,
     _BANG = 352,
     _DASH = 353,
     _TILDE = 354,
     _PLUS = 355,
     _STAR = 356,
     _SLASH = 357,
     _PERCENT = 358,
     _LEFT_ANGLE = 359,
     _RIGHT_ANGLE = 360,
     _VERTICAL_BAR = 361,
     _CARET = 362,
     _AMPERSAND = 363,
     _QUESTION = 364,
     _INVARIANT = 365,
     _PRECISE = 366,
     _HIGH_PRECISION = 367,
     _MEDIUM_PRECISION = 368,
     _LOW_PRECISION = 369,
     _PRECISION = 370
   };
#endif



#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef int YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif




