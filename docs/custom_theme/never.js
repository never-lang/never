/*
Language: Never
Author: Slawomir Maludzinski <slawomir.maludzinski@gmail.com>
Description: Syntactically scoped, strongly typed, call by value, functional programming language
Website: https://never-lang.readthedocs.io/
Category: functional
*/

export default function(hljs) {
  var NEVER_KEYWORDS = {
    keyword: 'catch do enum else extern for func if in let match range record throw var while',
    type: 'bool char float int string',
    literal: 'false nil true',
    built_in: 'assert assertb assertf chr cos exp length log ord pow print printb printc printf prints read sin sqrt str strf tan'
  }

  var NEVER_NUMBER = {
    className: 'number',
    variants:
    [
      { begin: '\\b[\\d_]+(\\.[\\deE_]+)?\\b' },
      { begin: '\\b0[xX][a-fA-F0-9_]+(\\.[a-fA-F0-9p_]+)?\\b' }
    ]
  }

  var NEVER_STRING = {
    className: 'string',
    begin: '"', end: '"'
  }

  var NEVER_TYPE = {
    className: 'type',
    begin: '\\b[A-Z][0-9A-Za-z$_]*',
  }
  
  var NEVER_DIM = {
    className: 'array',
    begin: '\\[', end: '\\]'
  }

  var NEVER_FUNCTION = {
    className: 'function',
    beginKeywords: 'func', end: '{', excludeEnd: true,
    contains:
    [
      hljs.inherit(hljs.TITLE_MODE, {
        begin: /[A-Za-z$_][0-9A-Za-z$_]*/
      }),
      {
        className: 'params',
        begin: /\(/, end: /\)/,
        keywords: NEVER_KEYWORDS,
        contains:
        [
          NEVER_DIM,
          NEVER_TYPE
        ]
      },
      {
        className: 'params',
        begin: /->/, end: /\{/,
        excludeBegin: true, excludeEnd: true,
        endsParent: true,
        keywords: NEVER_KEYWORDS,
        contains:
        [
          NEVER_DIM,
          NEVER_TYPE
        ]
      }
    ]
  }

  var NEVER_ENUM_RECORD = {
    className: 'function',
    beginKeywords: 'enum record', end: '{', excludeEnd: true,
    contains:
    [
      hljs.inherit(hljs.TITLE_MODE, {
        begin: /[A-Za-z$_][0-9A-Za-z$_]*/
      }),
      {
        className: 'params',
        begin: /\{/, end: /\}/,
        endsParent: true,
        keywords: NEVER_KEYWORDS,
        contains:
        [
          NEVER_DIM,
          NEVER_TYPE
        ]
      }
    ]
  }

  var NEVER_EXTERN = {
    className: 'function',
    begin: 'extern', end: '[\n\r]',
    excludeBeging: true,
    keywords: NEVER_KEYWORDS,
    contains:
    [
      NEVER_STRING,
      {
        className: 'title',
        begin: '\\bfunc', end: /[A-Za-z$_][0-9A-Za-z$_]*/,
        excludeBegin: true
      },
      {
        className: 'params',
        begin: /\{/, end: /\}/,
        endsParent: true,
        contains:
        [
          NEVER_DIM,
          NEVER_TYPE
        ]
      }
    ]
  }

  return {
    name: 'never',
    aliases: [ 'Never ' ],
    disableAutodetect: true,
    case_insensitive: false, // language is case-sensitive
    keywords: NEVER_KEYWORDS,
    contains: [
      NEVER_TYPE,
      NEVER_FUNCTION,
      NEVER_ENUM_RECORD,
      NEVER_EXTERN,
      NEVER_NUMBER,
      NEVER_STRING,
      hljs.C_BLOCK_COMMENT_MODE,
      hljs.HASH_COMMENT_MODE
    ]
  }
}

