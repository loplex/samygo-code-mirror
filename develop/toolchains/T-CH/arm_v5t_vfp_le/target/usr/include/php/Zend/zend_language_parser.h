/* A Bison parser, made by GNU Bison 2.0.  */

/* Skeleton parser for Yacc-like parsing with Bison,
   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004 Free Software Foundation, Inc.

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
   Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.  */

/* As a special exception, when this file is copied by Bison into a
   Bison output file, you may use that output file without restriction.
   This special exception was added by the Free Software Foundation
   in version 1.24 of Bison.  */

/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     T_REQUIRE_ONCE = 258,
     T_REQUIRE = 259,
     T_EVAL = 260,
     T_INCLUDE_ONCE = 261,
     T_INCLUDE = 262,
     T_LOGICAL_OR = 263,
     T_LOGICAL_XOR = 264,
     T_LOGICAL_AND = 265,
     T_PRINT = 266,
     T_SR_EQUAL = 267,
     T_SL_EQUAL = 268,
     T_XOR_EQUAL = 269,
     T_OR_EQUAL = 270,
     T_AND_EQUAL = 271,
     T_MOD_EQUAL = 272,
     T_CONCAT_EQUAL = 273,
     T_DIV_EQUAL = 274,
     T_MUL_EQUAL = 275,
     T_MINUS_EQUAL = 276,
     T_PLUS_EQUAL = 277,
     T_BOOLEAN_OR = 278,
     T_BOOLEAN_AND = 279,
     T_IS_NOT_IDENTICAL = 280,
     T_IS_IDENTICAL = 281,
     T_IS_NOT_EQUAL = 282,
     T_IS_EQUAL = 283,
     T_IS_GREATER_OR_EQUAL = 284,
     T_IS_SMALLER_OR_EQUAL = 285,
     T_SR = 286,
     T_SL = 287,
     T_UNSET_CAST = 288,
     T_BOOL_CAST = 289,
     T_OBJECT_CAST = 290,
     T_ARRAY_CAST = 291,
     T_STRING_CAST = 292,
     T_DOUBLE_CAST = 293,
     T_INT_CAST = 294,
     T_DEC = 295,
     T_INC = 296,
     T_NEW = 297,
     T_EXIT = 298,
     T_IF = 299,
     T_ELSEIF = 300,
     T_ELSE = 301,
     T_ENDIF = 302,
     T_LNUMBER = 303,
     T_DNUMBER = 304,
     T_STRING = 305,
     T_STRING_VARNAME = 306,
     T_VARIABLE = 307,
     T_NUM_STRING = 308,
     T_INLINE_HTML = 309,
     T_CHARACTER = 310,
     T_BAD_CHARACTER = 311,
     T_ENCAPSED_AND_WHITESPACE = 312,
     T_CONSTANT_ENCAPSED_STRING = 313,
     T_ECHO = 314,
     T_DO = 315,
     T_WHILE = 316,
     T_ENDWHILE = 317,
     T_FOR = 318,
     T_ENDFOR = 319,
     T_FOREACH = 320,
     T_ENDFOREACH = 321,
     T_DECLARE = 322,
     T_ENDDECLARE = 323,
     T_AS = 324,
     T_SWITCH = 325,
     T_ENDSWITCH = 326,
     T_CASE = 327,
     T_DEFAULT = 328,
     T_BREAK = 329,
     T_CONTINUE = 330,
     T_OLD_FUNCTION = 331,
     T_FUNCTION = 332,
     T_CONST = 333,
     T_RETURN = 334,
     T_USE = 335,
     T_GLOBAL = 336,
     T_STATIC = 337,
     T_VAR = 338,
     T_UNSET = 339,
     T_ISSET = 340,
     T_EMPTY = 341,
     T_CLASS = 342,
     T_EXTENDS = 343,
     T_OBJECT_OPERATOR = 344,
     T_DOUBLE_ARROW = 345,
     T_LIST = 346,
     T_ARRAY = 347,
     T_CLASS_C = 348,
     T_FUNC_C = 349,
     T_LINE = 350,
     T_FILE = 351,
     T_COMMENT = 352,
     T_ML_COMMENT = 353,
     T_OPEN_TAG = 354,
     T_OPEN_TAG_WITH_ECHO = 355,
     T_CLOSE_TAG = 356,
     T_WHITESPACE = 357,
     T_START_HEREDOC = 358,
     T_END_HEREDOC = 359,
     T_DOLLAR_OPEN_CURLY_BRACES = 360,
     T_CURLY_OPEN = 361,
     T_PAAMAYIM_NEKUDOTAYIM = 362
   };
#endif
#define T_REQUIRE_ONCE 258
#define T_REQUIRE 259
#define T_EVAL 260
#define T_INCLUDE_ONCE 261
#define T_INCLUDE 262
#define T_LOGICAL_OR 263
#define T_LOGICAL_XOR 264
#define T_LOGICAL_AND 265
#define T_PRINT 266
#define T_SR_EQUAL 267
#define T_SL_EQUAL 268
#define T_XOR_EQUAL 269
#define T_OR_EQUAL 270
#define T_AND_EQUAL 271
#define T_MOD_EQUAL 272
#define T_CONCAT_EQUAL 273
#define T_DIV_EQUAL 274
#define T_MUL_EQUAL 275
#define T_MINUS_EQUAL 276
#define T_PLUS_EQUAL 277
#define T_BOOLEAN_OR 278
#define T_BOOLEAN_AND 279
#define T_IS_NOT_IDENTICAL 280
#define T_IS_IDENTICAL 281
#define T_IS_NOT_EQUAL 282
#define T_IS_EQUAL 283
#define T_IS_GREATER_OR_EQUAL 284
#define T_IS_SMALLER_OR_EQUAL 285
#define T_SR 286
#define T_SL 287
#define T_UNSET_CAST 288
#define T_BOOL_CAST 289
#define T_OBJECT_CAST 290
#define T_ARRAY_CAST 291
#define T_STRING_CAST 292
#define T_DOUBLE_CAST 293
#define T_INT_CAST 294
#define T_DEC 295
#define T_INC 296
#define T_NEW 297
#define T_EXIT 298
#define T_IF 299
#define T_ELSEIF 300
#define T_ELSE 301
#define T_ENDIF 302
#define T_LNUMBER 303
#define T_DNUMBER 304
#define T_STRING 305
#define T_STRING_VARNAME 306
#define T_VARIABLE 307
#define T_NUM_STRING 308
#define T_INLINE_HTML 309
#define T_CHARACTER 310
#define T_BAD_CHARACTER 311
#define T_ENCAPSED_AND_WHITESPACE 312
#define T_CONSTANT_ENCAPSED_STRING 313
#define T_ECHO 314
#define T_DO 315
#define T_WHILE 316
#define T_ENDWHILE 317
#define T_FOR 318
#define T_ENDFOR 319
#define T_FOREACH 320
#define T_ENDFOREACH 321
#define T_DECLARE 322
#define T_ENDDECLARE 323
#define T_AS 324
#define T_SWITCH 325
#define T_ENDSWITCH 326
#define T_CASE 327
#define T_DEFAULT 328
#define T_BREAK 329
#define T_CONTINUE 330
#define T_OLD_FUNCTION 331
#define T_FUNCTION 332
#define T_CONST 333
#define T_RETURN 334
#define T_USE 335
#define T_GLOBAL 336
#define T_STATIC 337
#define T_VAR 338
#define T_UNSET 339
#define T_ISSET 340
#define T_EMPTY 341
#define T_CLASS 342
#define T_EXTENDS 343
#define T_OBJECT_OPERATOR 344
#define T_DOUBLE_ARROW 345
#define T_LIST 346
#define T_ARRAY 347
#define T_CLASS_C 348
#define T_FUNC_C 349
#define T_LINE 350
#define T_FILE 351
#define T_COMMENT 352
#define T_ML_COMMENT 353
#define T_OPEN_TAG 354
#define T_OPEN_TAG_WITH_ECHO 355
#define T_CLOSE_TAG 356
#define T_WHITESPACE 357
#define T_START_HEREDOC 358
#define T_END_HEREDOC 359
#define T_DOLLAR_OPEN_CURLY_BRACES 360
#define T_CURLY_OPEN 361
#define T_PAAMAYIM_NEKUDOTAYIM 362




#if ! defined (YYSTYPE) && ! defined (YYSTYPE_IS_DECLARED)
typedef int YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif





