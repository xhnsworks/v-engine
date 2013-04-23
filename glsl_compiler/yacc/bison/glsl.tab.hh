
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
     CONST = 258,
     BOOL = 259,
     FLOAT = 260,
     DOUBLE = 261,
     INT = 262,
     UINT = 263,
     BREAK = 264,
     CONTINUE = 265,
     DO = 266,
     ELSE = 267,
     FOR = 268,
     IF = 269,
     DISCARD = 270,
     RETURN = 271,
     SWITCH = 272,
     CASE = 273,
     DEFAULT = 274,
     SUBROUTINE = 275,
     SMOOTH = 276,
     FLAT = 277,
     NOPERSPECTIVE = 278,
     LAYOUT = 279,
     VEC2 = 280,
     VEC3 = 281,
     VEC4 = 282,
     MAT2 = 283,
     MAT3 = 284,
     MAT4 = 285,
     CENTROID = 286,
     IN = 287,
     OUT = 288,
     INOUT = 289,
     UNIFORM = 290,
     PATCH = 291,
     SAMPLE = 292,
     MAT2X2 = 293,
     MAT2X3 = 294,
     MAT2X4 = 295,
     MAT3X2 = 296,
     MAT3X3 = 297,
     MAT3X4 = 298,
     MAT4X2 = 299,
     MAT4X3 = 300,
     MAT4X4 = 301,
     SAMPLER1D = 302,
     SAMPLER2D = 303,
     SAMPLER3D = 304,
     SAMPLERCUBE = 305,
     SAMPLER1DARRAY = 306,
     SAMPLER2DARRAY = 307,
     SAMPLERCUBEARRAY = 308,
     STRUCT = 309,
     VOID = 310,
     WHILE = 311,
     IDENTIFIER = 312,
     TYPE_NAME = 313,
     FLOATCONSTANT = 314,
     DOUBLECONSTANT = 315,
     INTCONSTANT = 316,
     UINTCONSTANT = 317,
     BOOLCONSTANT = 318,
     FIELD_SELECTION = 319,
     LEFT_OP = 320,
     RIGHT_OP = 321,
     INC_OP = 322,
     DEC_OP = 323,
     LE_OP = 324,
     GE_OP = 325,
     EQ_OP = 326,
     NE_OP = 327,
     AND_OP = 328,
     OR_OP = 329,
     XOR_OP = 330,
     MUL_ASSIGN = 331,
     DIV_ASSIGN = 332,
     ADD_ASSIGN = 333,
     MOD_ASSIGN = 334,
     LEFT_ASSIGN = 335,
     RIGHT_ASSIGN = 336,
     AND_ASSIGN = 337,
     XOR_ASSIGN = 338,
     OR_ASSIGN = 339,
     SUB_ASSIGN = 340,
     LEFT_PAREN = 341,
     RIGHT_PAREN = 342,
     LEFT_BRACKET = 343,
     RIGHT_BRACKET = 344,
     LEFT_BRACE = 345,
     RIGHT_BRACE = 346,
     DOT = 347,
     COMMA = 348,
     COLON = 349,
     EQUAL = 350,
     SEMICOLON = 351,
     BANG = 352,
     DASH = 353,
     TILDE = 354,
     PLUS = 355,
     STAR = 356,
     SLASH = 357,
     PERCENT = 358,
     LEFT_ANGLE = 359,
     RIGHT_ANGLE = 360,
     VERTICAL_BAR = 361,
     CARET = 362,
     AMPERSAND = 363,
     QUESTION = 364,
     INVARIANT = 365,
     PRECISE = 366,
     HIGH_PRECISION = 367,
     MEDIUM_PRECISION = 368,
     LOW_PRECISION = 369,
     PRECISION = 370
   };
#endif



#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef int YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif




