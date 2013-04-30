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




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef int YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif



