/* A Bison parser, made by GNU Bison 2.3.  */

/* Skeleton interface for Bison's Yacc-like parsers in C

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
/* Tokens.  */
#define _CONST 258
#define _BOOL 259
#define _FLOAT 260
#define _DOUBLE 261
#define _INT 262
#define _UINT 263
#define _BREAK 264
#define _CONTINUE 265
#define _DO 266
#define _ELSE 267
#define _FOR 268
#define _IF 269
#define _DISCARD 270
#define _RETURN 271
#define _SWITCH 272
#define _CASE 273
#define _DEFAULT 274
#define _SUBROUTINE 275
#define _SMOOTH 276
#define _FLAT 277
#define _NOPERSPECTIVE 278
#define _LAYOUT 279
#define _VEC2 280
#define _VEC3 281
#define _VEC4 282
#define _MAT2 283
#define _MAT3 284
#define _MAT4 285
#define _CENTROID 286
#define _IN 287
#define _OUT 288
#define _INOUT 289
#define _UNIFORM 290
#define _PATCH 291
#define _SAMPLE 292
#define _MAT2X2 293
#define _MAT2X3 294
#define _MAT2X4 295
#define _MAT3X2 296
#define _MAT3X3 297
#define _MAT3X4 298
#define _MAT4X2 299
#define _MAT4X3 300
#define _MAT4X4 301
#define _SAMPLER1D 302
#define _SAMPLER2D 303
#define _SAMPLER3D 304
#define _SAMPLERCUBE 305
#define _SAMPLER1DARRAY 306
#define _SAMPLER2DARRAY 307
#define _SAMPLERCUBEARRAY 308
#define _STRUCT 309
#define _VOID 310
#define _WHILE 311
#define _IDENTIFIER 312
#define _TYPE_NAME 313
#define _FLOATCONSTANT 314
#define _DOUBLECONSTANT 315
#define _INTCONSTANT 316
#define _UINTCONSTANT 317
#define _BOOLCONSTANT 318
#define _FIELD_SELECTION 319
#define _LEFT_OP 320
#define _RIGHT_OP 321
#define _INC_OP 322
#define _DEC_OP 323
#define _LE_OP 324
#define _GE_OP 325
#define _EQ_OP 326
#define _NE_OP 327
#define _AND_OP 328
#define _OR_OP 329
#define _XOR_OP 330
#define _MUL_ASSIGN 331
#define _DIV_ASSIGN 332
#define _ADD_ASSIGN 333
#define _MOD_ASSIGN 334
#define _LEFT_ASSIGN 335
#define _RIGHT_ASSIGN 336
#define _AND_ASSIGN 337
#define _XOR_ASSIGN 338
#define _OR_ASSIGN 339
#define _SUB_ASSIGN 340
#define _LEFT_PAREN 341
#define _RIGHT_PAREN 342
#define _LEFT_BRACKET 343
#define _RIGHT_BRACKET 344
#define _LEFT_BRACE 345
#define _RIGHT_BRACE 346
#define _DOT 347
#define _COMMA 348
#define _COLON 349
#define _EQUAL 350
#define _SEMICOLON 351
#define _BANG 352
#define _DASH 353
#define _TILDE 354
#define _PLUS 355
#define _STAR 356
#define _SLASH 357
#define _PERCENT 358
#define _LEFT_ANGLE 359
#define _RIGHT_ANGLE 360
#define _VERTICAL_BAR 361
#define _CARET 362
#define _AMPERSAND 363
#define _QUESTION 364
#define _INVARIANT 365
#define _PRECISE 366
#define _HIGH_PRECISION 367
#define _MEDIUM_PRECISION 368
#define _LOW_PRECISION 369
#define _PRECISION 370




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef int YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif



