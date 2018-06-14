/* A Bison parser, made by GNU Bison 3.0.2.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2013 Free Software Foundation, Inc.

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
#define YYBISON_VERSION "3.0.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 1

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* Copy the first part of user declarations.  */
#line 2 "front/parser.y" /* yacc.c:339  */

#include <stdio.h>
#include "utils.h"
#include "types.h"
#include "scanner.h"
#include "never.h"

int parse_result;
extern int line_no;

int yylex(token * tokp)
{
    return lex_scan(tokp);
}

int yyerror(never ** nev, char * str)
{
    parse_result = 1;
    print_error_msg(line_no, "%s\n", str);
    
    return 1;
}

#line 90 "front/parser.c" /* yacc.c:339  */

# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* In a future release of Bison, this section will be replaced
   by #include "parser.h".  */
#ifndef YY_YY_FRONT_PARSER_H_INCLUDED
# define YY_YY_FRONT_PARSER_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    TOK_ID = 258,
    TOK_NUM_FLOAT = 259,
    TOK_NUM_INT = 260,
    TOK_FLOAT = 261,
    TOK_INT = 262,
    TOK_FUNC = 263,
    TOK_RET = 264,
    TOK_RETURN = 265,
    TOK_OR = 266,
    TOK_AND = 267,
    TOK_EQ = 268,
    TOK_NEQ = 269,
    TOK_LTE = 270,
    TOK_GTE = 271,
    TOK_NOT = 272
  };
#endif

/* Value type.  */



int yyparse (never ** nev);

#endif /* !YY_YY_FRONT_PARSER_H_INCLUDED  */

/* Copy the second part of user declarations.  */

#line 153 "front/parser.c" /* yacc.c:358  */

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
#else
typedef signed char yytype_int8;
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
# elif ! defined YYSIZE_T
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
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif

#ifndef YY_ATTRIBUTE
# if (defined __GNUC__                                               \
      && (2 < __GNUC__ || (__GNUC__ == 2 && 96 <= __GNUC_MINOR__)))  \
     || defined __SUNPRO_C && 0x5110 <= __SUNPRO_C
#  define YY_ATTRIBUTE(Spec) __attribute__(Spec)
# else
#  define YY_ATTRIBUTE(Spec) /* empty */
# endif
#endif

#ifndef YY_ATTRIBUTE_PURE
# define YY_ATTRIBUTE_PURE   YY_ATTRIBUTE ((__pure__))
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# define YY_ATTRIBUTE_UNUSED YY_ATTRIBUTE ((__unused__))
#endif

#if !defined _Noreturn \
     && (!defined __STDC_VERSION__ || __STDC_VERSION__ < 201112)
# if defined _MSC_VER && 1200 <= _MSC_VER
#  define _Noreturn __declspec (noreturn)
# else
#  define _Noreturn YY_ATTRIBUTE ((__noreturn__))
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN \
    _Pragma ("GCC diagnostic push") \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")\
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
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
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
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
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
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

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYSIZE_T yynewbytes;                                            \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / sizeof (*yyptr);                          \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, (Count) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYSIZE_T yyi;                         \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  7
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   237

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  33
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  9
/* YYNRULES -- Number of rules.  */
#define YYNRULES  45
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  102

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   272

#define YYTRANSLATE(YYX)                                                \
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, without out-of-bounds checking.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,    25,     2,     2,
      27,    28,    23,    21,    29,    22,     2,    24,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    12,    31,
      17,     2,    18,    11,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    30,     2,    32,     2,     2,     2,     2,
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
       5,     6,     7,     8,     9,    10,    13,    14,    15,    16,
      19,    20,    26
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,    71,    71,    77,    83,    89,    95,   101,   107,   113,
     119,   125,   131,   137,   143,   149,   155,   161,   167,   173,
     179,   185,   191,   196,   202,   208,   214,   220,   226,   232,
     238,   244,   250,   256,   262,   268,   274,   280,   286,   292,
     302,   307,   312,   317,   323,   329
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "TOK_ID", "TOK_NUM_FLOAT", "TOK_NUM_INT",
  "TOK_FLOAT", "TOK_INT", "TOK_FUNC", "TOK_RET", "TOK_RETURN", "'?'",
  "':'", "TOK_OR", "TOK_AND", "TOK_EQ", "TOK_NEQ", "'<'", "'>'", "TOK_LTE",
  "TOK_GTE", "'+'", "'-'", "'*'", "'/'", "'%'", "TOK_NOT", "'('", "')'",
  "','", "'{'", "';'", "'}'", "$accept", "expr", "expr_list", "var",
  "var_list", "func", "func_body", "func_list", "never", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,    63,    58,   266,   267,   268,   269,    60,    62,   270,
     271,    43,    45,    42,    47,    37,   272,    40,    41,    44,
     123,    59,   125
};
# endif

#define YYPACT_NINF -51

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-51)))

#define YYTABLE_NINF -1

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
      52,    69,   -51,    52,    78,     6,   -51,   -51,   -51,    29,
      54,    76,    80,    31,    73,   -51,    -4,    37,   -51,   -51,
      77,     1,    64,    96,    64,    99,    13,    64,   115,    57,
      64,   -51,    64,   118,   -51,    64,    44,   -51,    57,   -51,
      64,   -51,    58,   -51,    45,   -51,   -51,   -51,   -51,   -51,
      58,    58,    58,    79,   -51,    58,    61,    61,   132,    58,
      58,    58,    58,    58,    58,    58,    58,    58,    58,    58,
      58,    58,    58,    23,    75,    98,   -51,   150,   179,   192,
     203,   203,   210,   210,   210,   210,    50,    50,    61,    61,
      61,   -51,   165,    40,   -51,   100,    58,   -51,    58,   -51,
     165,   165
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,     0,    43,    45,     0,     0,    44,     1,    39,     0,
       0,    29,    27,     0,     0,    35,     0,     0,    30,    28,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    36,     0,     0,    31,     0,     0,    37,     0,    33,
       0,    32,     0,    42,     0,    38,    34,     2,     4,     3,
       0,     0,     0,     0,    22,     0,     5,    19,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    20,     0,    18,    17,
      15,    16,    11,    13,    12,    14,     6,     7,     8,     9,
      10,    23,    25,     0,    41,     0,     0,    24,     0,    40,
      21,    26
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -51,   -50,   -51,   104,    -9,     3,    51,    94,   -51
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
      -1,    53,    93,    15,    16,    54,    37,     3,     4
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_uint8 yytable[] =
{
      56,    57,    58,     2,    21,    75,     6,     8,    26,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    92,    23,    24,    47,    48,    49,    28,
      24,     1,    10,     9,    10,    11,    12,    11,    12,     2,
      10,    33,    24,    11,    12,    50,   100,     6,   101,    51,
      52,    91,     1,     1,    42,    55,    13,    14,    13,    20,
       1,    47,    48,    49,    13,    25,     1,    10,    97,    98,
      11,    12,     5,    70,    71,    72,    43,    73,     7,    18,
      50,    17,    22,    19,    51,    52,    27,    36,    73,    45,
      59,    13,    60,    61,    62,    63,    64,    65,    66,    67,
      68,    69,    70,    71,    72,    30,    73,    94,    32,    59,
      74,    60,    61,    62,    63,    64,    65,    66,    67,    68,
      69,    70,    71,    72,    35,    73,    29,    40,    31,    95,
      44,    34,    99,     0,    38,     0,    39,     0,     0,    41,
       0,     0,     0,    59,    46,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,     0,    73,
      76,    59,    96,    60,    61,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    59,    73,    60,    61,
      62,    63,    64,    65,    66,    67,    68,    69,    70,    71,
      72,     0,    73,    61,    62,    63,    64,    65,    66,    67,
      68,    69,    70,    71,    72,     0,    73,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,     0,    73,
      64,    65,    66,    67,    68,    69,    70,    71,    72,     0,
      73,    68,    69,    70,    71,    72,     0,    73
};

static const yytype_int8 yycheck[] =
{
      50,    51,    52,     0,    13,    55,     3,     1,    17,    59,
      60,    61,    62,    63,    64,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    28,    29,     3,     4,     5,    28,
      29,     8,     3,    27,     3,     6,     7,     6,     7,    36,
       3,    28,    29,     6,     7,    22,    96,    44,    98,    26,
      27,    28,     8,     8,    10,    10,    27,    28,    27,    28,
       8,     3,     4,     5,    27,    28,     8,     3,    28,    29,
       6,     7,     3,    23,    24,    25,    32,    27,     0,     3,
      22,    27,     9,     3,    26,    27,     9,    30,    27,    38,
      11,    27,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    25,     9,    27,    32,     9,    11,
      31,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,    24,    25,     9,    27,    22,     9,    24,    31,
      36,    27,    32,    -1,    30,    -1,    32,    -1,    -1,    35,
      -1,    -1,    -1,    11,    40,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    -1,    27,
      28,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    22,    23,    24,    25,    11,    27,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    -1,    27,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    -1,    27,    15,    16,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    -1,    27,
      17,    18,    19,    20,    21,    22,    23,    24,    25,    -1,
      27,    21,    22,    23,    24,    25,    -1,    27
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     8,    38,    40,    41,     3,    38,     0,     1,    27,
       3,     6,     7,    27,    28,    36,    37,    27,     3,     3,
      28,    37,     9,    28,    29,    28,    37,     9,    28,    36,
       9,    36,     9,    28,    36,     9,    30,    39,    36,    36,
       9,    36,    10,    32,    40,    39,    36,     3,     4,     5,
      22,    26,    27,    34,    38,    10,    34,    34,    34,    11,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      23,    24,    25,    27,    31,    34,    28,    34,    34,    34,
      34,    34,    34,    34,    34,    34,    34,    34,    34,    34,
      34,    28,    34,    35,    32,    31,    12,    28,    29,    32,
      34,    34
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    33,    34,    34,    34,    34,    34,    34,    34,    34,
      34,    34,    34,    34,    34,    34,    34,    34,    34,    34,
      34,    34,    34,    34,    34,    35,    35,    36,    36,    36,
      36,    36,    36,    36,    36,    37,    37,    38,    38,    38,
      39,    39,    39,    40,    40,    41
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     1,     1,     2,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     2,
       3,     5,     1,     3,     4,     1,     3,     1,     2,     1,
       2,     4,     5,     5,     6,     1,     3,     7,     8,     3,
       6,     5,     2,     1,     2,     1
};


#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)
#define YYEMPTY         (-2)
#define YYEOF           0

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                  \
do                                                              \
  if (yychar == YYEMPTY)                                        \
    {                                                           \
      yychar = (Token);                                         \
      yylval = (Value);                                         \
      YYPOPSTACK (yylen);                                       \
      yystate = *yyssp;                                         \
      goto yybackup;                                            \
    }                                                           \
  else                                                          \
    {                                                           \
      yyerror (nev, YY_("syntax error: cannot back up")); \
      YYERROR;                                                  \
    }                                                           \
while (0)

/* Error token number */
#define YYTERROR        1
#define YYERRCODE       256



/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)

/* This macro is provided for backward compatibility. */
#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


# define YY_SYMBOL_PRINT(Title, Type, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Type, Value, nev); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*----------------------------------------.
| Print this symbol's value on YYOUTPUT.  |
`----------------------------------------*/

static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, never ** nev)
{
  FILE *yyo = yyoutput;
  YYUSE (yyo);
  YYUSE (nev);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# endif
  YYUSE (yytype);
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, never ** nev)
{
  YYFPRINTF (yyoutput, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep, nev);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yytype_int16 *yyssp, YYSTYPE *yyvsp, int yyrule, never ** nev)
{
  unsigned long int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       yystos[yyssp[yyi + 1 - yynrhs]],
                       &(yyvsp[(yyi + 1) - (yynrhs)])
                                              , nev);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule, nev); \
} while (0)

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
#ifndef YYINITDEPTH
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
static YYSIZE_T
yystrlen (const char *yystr)
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
static char *
yystpcpy (char *yydest, const char *yysrc)
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

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYSIZE_T *yymsg_alloc, char **yymsg,
                yytype_int16 *yyssp, int yytoken)
{
  YYSIZE_T yysize0 = yytnamerr (YY_NULLPTR, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
  int yycount = 0;

  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[*yyssp];
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                {
                  YYSIZE_T yysize1 = yysize + yytnamerr (YY_NULLPTR, yytname[yyx]);
                  if (! (yysize <= yysize1
                         && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
                    return 2;
                  yysize = yysize1;
                }
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    YYSIZE_T yysize1 = yysize + yystrlen (yyformat);
    if (! (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
      return 2;
    yysize = yysize1;
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          yyp++;
          yyformat++;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep, never ** nev)
{
  YYUSE (yyvaluep);
  YYUSE (nev);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  switch (yytype)
    {
          case 3: /* TOK_ID  */
#line 56 "front/parser.y" /* yacc.c:1257  */
      { free(((*yyvaluep).val.str_value)); }
#line 1064 "front/parser.c" /* yacc.c:1257  */
        break;

    case 34: /* expr  */
#line 57 "front/parser.y" /* yacc.c:1257  */
      { expr_delete(((*yyvaluep).val.expr_value)); }
#line 1070 "front/parser.c" /* yacc.c:1257  */
        break;

    case 35: /* expr_list  */
#line 58 "front/parser.y" /* yacc.c:1257  */
      { expr_list_delete(((*yyvaluep).val.expr_list_value)); }
#line 1076 "front/parser.c" /* yacc.c:1257  */
        break;

    case 36: /* var  */
#line 59 "front/parser.y" /* yacc.c:1257  */
      { var_delete(((*yyvaluep).val.var_value)); }
#line 1082 "front/parser.c" /* yacc.c:1257  */
        break;

    case 37: /* var_list  */
#line 60 "front/parser.y" /* yacc.c:1257  */
      { var_list_delete(((*yyvaluep).val.var_list_value)); }
#line 1088 "front/parser.c" /* yacc.c:1257  */
        break;

    case 38: /* func  */
#line 61 "front/parser.y" /* yacc.c:1257  */
      { func_delete(((*yyvaluep).val.func_value)); }
#line 1094 "front/parser.c" /* yacc.c:1257  */
        break;

    case 39: /* func_body  */
#line 63 "front/parser.y" /* yacc.c:1257  */
      { func_body_delete(((*yyvaluep).val.func_body_value)); }
#line 1100 "front/parser.c" /* yacc.c:1257  */
        break;

    case 40: /* func_list  */
#line 62 "front/parser.y" /* yacc.c:1257  */
      { func_list_delete(((*yyvaluep).val.func_list_value)); }
#line 1106 "front/parser.c" /* yacc.c:1257  */
        break;

    case 41: /* never  */
#line 64 "front/parser.y" /* yacc.c:1257  */
      {  }
#line 1112 "front/parser.c" /* yacc.c:1257  */
        break;


      default:
        break;
    }
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}




/*----------.
| yyparse.  |
`----------*/

int
yyparse (never ** nev)
{
/* The lookahead symbol.  */
int yychar;


/* The semantic value of the lookahead symbol.  */
/* Default value used for initialization, for pacifying older GCCs
   or non-GCC compilers.  */
YY_INITIAL_VALUE (static YYSTYPE yyval_default;)
YYSTYPE yylval YY_INITIAL_VALUE (= yyval_default);

    /* Number of syntax errors so far.  */
    int yynerrs;

    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       'yyss': related to states.
       'yyvs': related to semantic values.

       Refer to the stacks through separate pointers, to allow yyoverflow
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
  int yytoken = 0;
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

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */
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
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = yylex (&yylval);
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
      if (yytable_value_is_error (yyn))
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
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

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
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:
#line 72 "front/parser.y" /* yacc.c:1646  */
    {
    (yyval.val.expr_value) = expr_new_id((yyvsp[0].val.str_value));
    (yyval.val.expr_value)->line_no = (yyvsp[0].line_no);
}
#line 1383 "front/parser.c" /* yacc.c:1646  */
    break;

  case 3:
#line 78 "front/parser.y" /* yacc.c:1646  */
    {
    (yyval.val.expr_value) = expr_new_int((yyvsp[0].val.int_value));
    (yyval.val.expr_value)->line_no = (yyvsp[0].line_no);
}
#line 1392 "front/parser.c" /* yacc.c:1646  */
    break;

  case 4:
#line 84 "front/parser.y" /* yacc.c:1646  */
    {
    (yyval.val.expr_value) = expr_new_float((yyvsp[0].val.float_value));
    (yyval.val.expr_value)->line_no = (yyvsp[0].line_no);
}
#line 1401 "front/parser.c" /* yacc.c:1646  */
    break;

  case 5:
#line 90 "front/parser.y" /* yacc.c:1646  */
    {
    (yyval.val.expr_value) = expr_new_one(EXPR_NEG, (yyvsp[0].val.expr_value));
    (yyval.val.expr_value)->line_no = (yyvsp[-1].line_no);
}
#line 1410 "front/parser.c" /* yacc.c:1646  */
    break;

  case 6:
#line 96 "front/parser.y" /* yacc.c:1646  */
    {
    (yyval.val.expr_value) = expr_new_two(EXPR_ADD, (yyvsp[-2].val.expr_value), (yyvsp[0].val.expr_value));
    (yyval.val.expr_value)->line_no = (yyvsp[-1].line_no);
}
#line 1419 "front/parser.c" /* yacc.c:1646  */
    break;

  case 7:
#line 102 "front/parser.y" /* yacc.c:1646  */
    {
    (yyval.val.expr_value) = expr_new_two(EXPR_SUB, (yyvsp[-2].val.expr_value), (yyvsp[0].val.expr_value));
    (yyval.val.expr_value)->line_no = (yyvsp[-1].line_no);
}
#line 1428 "front/parser.c" /* yacc.c:1646  */
    break;

  case 8:
#line 108 "front/parser.y" /* yacc.c:1646  */
    {
    (yyval.val.expr_value) = expr_new_two(EXPR_MUL, (yyvsp[-2].val.expr_value), (yyvsp[0].val.expr_value));
    (yyval.val.expr_value)->line_no = (yyvsp[-1].line_no);
}
#line 1437 "front/parser.c" /* yacc.c:1646  */
    break;

  case 9:
#line 114 "front/parser.y" /* yacc.c:1646  */
    {
    (yyval.val.expr_value) = expr_new_two(EXPR_DIV, (yyvsp[-2].val.expr_value), (yyvsp[0].val.expr_value));
    (yyval.val.expr_value)->line_no = (yyvsp[-1].line_no);
}
#line 1446 "front/parser.c" /* yacc.c:1646  */
    break;

  case 10:
#line 120 "front/parser.y" /* yacc.c:1646  */
    {
    (yyval.val.expr_value) = expr_new_two(EXPR_MOD, (yyvsp[-2].val.expr_value), (yyvsp[0].val.expr_value));
    (yyval.val.expr_value)->line_no = (yyvsp[-1].line_no);
}
#line 1455 "front/parser.c" /* yacc.c:1646  */
    break;

  case 11:
#line 126 "front/parser.y" /* yacc.c:1646  */
    {
    (yyval.val.expr_value) = expr_new_two(EXPR_LT, (yyvsp[-2].val.expr_value), (yyvsp[0].val.expr_value));
    (yyval.val.expr_value)->line_no = (yyvsp[-1].line_no);
}
#line 1464 "front/parser.c" /* yacc.c:1646  */
    break;

  case 12:
#line 132 "front/parser.y" /* yacc.c:1646  */
    {
    (yyval.val.expr_value) = expr_new_two(EXPR_LTE, (yyvsp[-2].val.expr_value), (yyvsp[0].val.expr_value));
    (yyval.val.expr_value)->line_no = (yyvsp[-1].line_no);
}
#line 1473 "front/parser.c" /* yacc.c:1646  */
    break;

  case 13:
#line 138 "front/parser.y" /* yacc.c:1646  */
    {
    (yyval.val.expr_value) = expr_new_two(EXPR_GT, (yyvsp[-2].val.expr_value), (yyvsp[0].val.expr_value));
    (yyval.val.expr_value)->line_no = (yyvsp[-1].line_no);
}
#line 1482 "front/parser.c" /* yacc.c:1646  */
    break;

  case 14:
#line 144 "front/parser.y" /* yacc.c:1646  */
    {
    (yyval.val.expr_value) = expr_new_two(EXPR_GTE, (yyvsp[-2].val.expr_value), (yyvsp[0].val.expr_value));
    (yyval.val.expr_value)->line_no = (yyvsp[-1].line_no);
}
#line 1491 "front/parser.c" /* yacc.c:1646  */
    break;

  case 15:
#line 150 "front/parser.y" /* yacc.c:1646  */
    {
    (yyval.val.expr_value) = expr_new_two(EXPR_EQ, (yyvsp[-2].val.expr_value), (yyvsp[0].val.expr_value));
    (yyval.val.expr_value)->line_no = (yyvsp[-1].line_no);
}
#line 1500 "front/parser.c" /* yacc.c:1646  */
    break;

  case 16:
#line 156 "front/parser.y" /* yacc.c:1646  */
    {
    (yyval.val.expr_value) = expr_new_two(EXPR_NEQ, (yyvsp[-2].val.expr_value), (yyvsp[0].val.expr_value));
    (yyval.val.expr_value)->line_no = (yyvsp[-1].line_no);
}
#line 1509 "front/parser.c" /* yacc.c:1646  */
    break;

  case 17:
#line 162 "front/parser.y" /* yacc.c:1646  */
    {
    (yyval.val.expr_value) = expr_new_two(EXPR_AND, (yyvsp[-2].val.expr_value), (yyvsp[0].val.expr_value));
    (yyval.val.expr_value)->line_no = (yyvsp[-1].line_no);
}
#line 1518 "front/parser.c" /* yacc.c:1646  */
    break;

  case 18:
#line 168 "front/parser.y" /* yacc.c:1646  */
    {
    (yyval.val.expr_value) = expr_new_two(EXPR_OR, (yyvsp[-2].val.expr_value), (yyvsp[0].val.expr_value));
    (yyval.val.expr_value)->line_no = (yyvsp[-1].line_no);
}
#line 1527 "front/parser.c" /* yacc.c:1646  */
    break;

  case 19:
#line 174 "front/parser.y" /* yacc.c:1646  */
    {
    (yyval.val.expr_value) = expr_new_one(EXPR_NOT, (yyvsp[0].val.expr_value));
    (yyval.val.expr_value)->line_no = (yyvsp[-1].line_no);
}
#line 1536 "front/parser.c" /* yacc.c:1646  */
    break;

  case 20:
#line 180 "front/parser.y" /* yacc.c:1646  */
    {
    (yyval.val.expr_value) = expr_new_one(EXPR_SUP, (yyvsp[-1].val.expr_value));
    (yyval.val.expr_value)->line_no = (yyvsp[-2].line_no);
}
#line 1545 "front/parser.c" /* yacc.c:1646  */
    break;

  case 21:
#line 186 "front/parser.y" /* yacc.c:1646  */
    {
    (yyval.val.expr_value) = expr_new_three(EXPR_COND, (yyvsp[-4].val.expr_value), (yyvsp[-2].val.expr_value), (yyvsp[0].val.expr_value));
    (yyval.val.expr_value)->line_no = (yyvsp[-3].line_no);
}
#line 1554 "front/parser.c" /* yacc.c:1646  */
    break;

  case 22:
#line 192 "front/parser.y" /* yacc.c:1646  */
    {
    (yyval.val.expr_value) = expr_new_func((yyvsp[0].val.func_value));
}
#line 1562 "front/parser.c" /* yacc.c:1646  */
    break;

  case 23:
#line 197 "front/parser.y" /* yacc.c:1646  */
    {
    (yyval.val.expr_value) = expr_new_call((yyvsp[-2].val.expr_value), NULL);
    (yyval.val.expr_value)->line_no = (yyvsp[-2].val.expr_value)->line_no;
}
#line 1571 "front/parser.c" /* yacc.c:1646  */
    break;

  case 24:
#line 203 "front/parser.y" /* yacc.c:1646  */
    {
    (yyval.val.expr_value) = expr_new_call((yyvsp[-3].val.expr_value), (yyvsp[-1].val.expr_list_value));
    (yyval.val.expr_value)->line_no = (yyvsp[-3].val.expr_value)->line_no;
}
#line 1580 "front/parser.c" /* yacc.c:1646  */
    break;

  case 25:
#line 209 "front/parser.y" /* yacc.c:1646  */
    {
    (yyval.val.expr_list_value) = expr_list_new();
    expr_list_add_end((yyval.val.expr_list_value), (yyvsp[0].val.expr_value));
}
#line 1589 "front/parser.c" /* yacc.c:1646  */
    break;

  case 26:
#line 215 "front/parser.y" /* yacc.c:1646  */
    {
    expr_list_add_end((yyvsp[-2].val.expr_list_value), (yyvsp[0].val.expr_value));
    (yyval.val.expr_list_value) = (yyvsp[-2].val.expr_list_value);
}
#line 1598 "front/parser.c" /* yacc.c:1646  */
    break;

  case 27:
#line 221 "front/parser.y" /* yacc.c:1646  */
    {
    (yyval.val.var_value) = var_new_int(NULL);
    (yyval.val.var_value)->line_no = (yyvsp[0].line_no);
}
#line 1607 "front/parser.c" /* yacc.c:1646  */
    break;

  case 28:
#line 227 "front/parser.y" /* yacc.c:1646  */
    {
    (yyval.val.var_value) = var_new_int((yyvsp[0].val.str_value));
    (yyval.val.var_value)->line_no = (yyvsp[0].line_no);
}
#line 1616 "front/parser.c" /* yacc.c:1646  */
    break;

  case 29:
#line 233 "front/parser.y" /* yacc.c:1646  */
    {
    (yyval.val.var_value) = var_new_float(NULL);
    (yyval.val.var_value)->line_no = (yyvsp[0].line_no);
}
#line 1625 "front/parser.c" /* yacc.c:1646  */
    break;

  case 30:
#line 239 "front/parser.y" /* yacc.c:1646  */
    {
    (yyval.val.var_value) = var_new_float((yyvsp[0].val.str_value));
    (yyval.val.var_value)->line_no = (yyvsp[0].line_no);
}
#line 1634 "front/parser.c" /* yacc.c:1646  */
    break;

  case 31:
#line 245 "front/parser.y" /* yacc.c:1646  */
    {
    (yyval.val.var_value) = var_new_func(NULL, NULL, (yyvsp[0].val.var_value));
    (yyval.val.var_value)->line_no = (yyvsp[0].line_no);
}
#line 1643 "front/parser.c" /* yacc.c:1646  */
    break;

  case 32:
#line 251 "front/parser.y" /* yacc.c:1646  */
    {
    (yyval.val.var_value) = var_new_func(NULL, (yyvsp[-3].val.var_list_value), (yyvsp[0].val.var_value));
    (yyval.val.var_value)->line_no = (yyvsp[0].line_no);
}
#line 1652 "front/parser.c" /* yacc.c:1646  */
    break;

  case 33:
#line 257 "front/parser.y" /* yacc.c:1646  */
    {
    (yyval.val.var_value) = var_new_func((yyvsp[-4].val.str_value), NULL, (yyvsp[0].val.var_value));
    (yyval.val.var_value)->line_no = (yyvsp[0].line_no);
}
#line 1661 "front/parser.c" /* yacc.c:1646  */
    break;

  case 34:
#line 263 "front/parser.y" /* yacc.c:1646  */
    {
    (yyval.val.var_value) = var_new_func((yyvsp[-5].val.str_value), (yyvsp[-3].val.var_list_value), (yyvsp[0].val.var_value));
    (yyval.val.var_value)->line_no = (yyvsp[0].line_no);
}
#line 1670 "front/parser.c" /* yacc.c:1646  */
    break;

  case 35:
#line 269 "front/parser.y" /* yacc.c:1646  */
    {
    (yyval.val.var_list_value) = var_list_new();
    var_list_add_end((yyval.val.var_list_value), (yyvsp[0].val.var_value));
}
#line 1679 "front/parser.c" /* yacc.c:1646  */
    break;

  case 36:
#line 275 "front/parser.y" /* yacc.c:1646  */
    {
    var_list_add_end((yyvsp[-2].val.var_list_value), (yyvsp[0].val.var_value));
    (yyval.val.var_list_value) = (yyvsp[-2].val.var_list_value);
}
#line 1688 "front/parser.c" /* yacc.c:1646  */
    break;

  case 37:
#line 281 "front/parser.y" /* yacc.c:1646  */
    {
    (yyval.val.func_value) = func_new((yyvsp[-5].val.str_value), NULL, (yyvsp[-1].val.var_value), (yyvsp[0].val.func_body_value));
    (yyval.val.func_value)->line_no = (yyvsp[-5].line_no);
}
#line 1697 "front/parser.c" /* yacc.c:1646  */
    break;

  case 38:
#line 287 "front/parser.y" /* yacc.c:1646  */
    {
    (yyval.val.func_value) = func_new((yyvsp[-6].val.str_value), (yyvsp[-4].val.var_list_value), (yyvsp[-1].val.var_value), (yyvsp[0].val.func_body_value));
    (yyval.val.func_value)->line_no = (yyvsp[-6].line_no);
}
#line 1706 "front/parser.c" /* yacc.c:1646  */
    break;

  case 39:
#line 293 "front/parser.y" /* yacc.c:1646  */
    {
    print_error_msg(line_no, "error in function %s defined at %d\n", (yyvsp[-1].val.str_value), (yyvsp[-1].line_no));
    free((yyvsp[-1].val.str_value));
    
    yyclearin;
    yyerrok;
    (yyval.val.func_value) = NULL;
}
#line 1719 "front/parser.c" /* yacc.c:1646  */
    break;

  case 40:
#line 303 "front/parser.y" /* yacc.c:1646  */
    {
    (yyval.val.func_body_value) = func_body_new((yyvsp[-4].val.func_list_value), (yyvsp[-2].val.expr_value));
}
#line 1727 "front/parser.c" /* yacc.c:1646  */
    break;

  case 41:
#line 308 "front/parser.y" /* yacc.c:1646  */
    {
    (yyval.val.func_body_value) = func_body_new(NULL, (yyvsp[-2].val.expr_value));
}
#line 1735 "front/parser.c" /* yacc.c:1646  */
    break;

  case 42:
#line 313 "front/parser.y" /* yacc.c:1646  */
    {
    (yyval.val.func_body_value) = func_body_new(NULL, NULL);
}
#line 1743 "front/parser.c" /* yacc.c:1646  */
    break;

  case 43:
#line 318 "front/parser.y" /* yacc.c:1646  */
    {
    (yyval.val.func_list_value) = func_list_new();
    func_list_add_end((yyval.val.func_list_value), (yyvsp[0].val.func_value));
}
#line 1752 "front/parser.c" /* yacc.c:1646  */
    break;

  case 44:
#line 324 "front/parser.y" /* yacc.c:1646  */
    {
    func_list_add_end((yyvsp[-1].val.func_list_value), (yyvsp[0].val.func_value));
    (yyval.val.func_list_value) = (yyvsp[-1].val.func_list_value);
}
#line 1761 "front/parser.c" /* yacc.c:1646  */
    break;

  case 45:
#line 330 "front/parser.y" /* yacc.c:1646  */
    {
    (yyval.val.never_value) = *nev = never_new((yyvsp[0].val.func_list_value));
}
#line 1769 "front/parser.c" /* yacc.c:1646  */
    break;


#line 1773 "front/parser.c" /* yacc.c:1646  */
      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (nev, YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = (char *) YYSTACK_ALLOC (yymsg_alloc);
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (nev, yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
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
                      yytoken, &yylval, nev);
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

  /* Do not reclaim the symbols of the rule whose action triggered
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
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
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
                  yystos[yystate], yyvsp, nev);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


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

#if !defined yyoverflow || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (nev, YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval, nev);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  yystos[*yyssp], yyvsp, nev);
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
  return yyresult;
}
#line 334 "front/parser.y" /* yacc.c:1906  */



