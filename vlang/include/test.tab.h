
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




