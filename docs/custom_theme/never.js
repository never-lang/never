export default function(hljs) {
  var NEVER_KEYWORDS = {
    keyword: 'catch do enum else extern for func if in let match record throw var while',
    type: 'bool char float int string',
    literal: 'false nil true',
    built_in: 'assert assertb assertf chr cos exp length log ord pow print printb printc printf prints read sin sqrt str strf tan'
  }

  var NEVER_NUMBER = {
    className: 'number',
    begin: '\\b0[xX][0-9A-F]+\\b|\\b[0-9]+\\b',
    relevance: 0
  }

  var NEVER_TYPE = {
    className: 'type',
    begin: '\\b[A-Z][0-9A-Za-z$_]*',
    relevance: 0
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

  return {
    name: 'never',
    aliases: [ 'Never ' ],
    disableAutodetect: true,
    case_insensitive: false, // language is case-insensitive
    keywords: NEVER_KEYWORDS,
    contains: [
      NEVER_TYPE,
      NEVER_FUNCTION,
      NEVER_ENUM_RECORD,
      NEVER_NUMBER,
      {
        className: 'string',
        begin: '"', end: '"'
      },
      hljs.C_BLOCK_COMMENT_MODE,
      hljs.HASH_COMMENT_MODE
    ]
  }
}

