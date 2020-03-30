export default function(hljs) {
  var NEVER_KEYWORDS = {
    keyword: 'catch do enum else extern for func if in let match record throw var while',
    type: 'bool char float int string',
    literal: 'false nil true',
    built_in: 'assert assertb assertf chr cos exp length log ord pow print printb printc printf prints read sin sqrt str strf tan'
  }

  var NEVER_TYPE = {
    className: 'type',
    begin: ': ', end: '[A-Za-z$_][0-9A-Za-z$_]*',
    excludeBegin: true
  }

  var NEVER_FUNCTION = {
    className: 'function',
    beginKeywords: 'func', end: '->', excludeEnd: true,
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
      })
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
      {
        className: 'identifier',
        begin: '/[A-Za-z$_][0-9A-Za-z$_]*/'
      },
      {
        className: 'number',
        begin: '\\b0[xX][0-9A-F]+|[0-9]+\\b'
      },
      {
        className: 'string',
        begin: '"', end: '"'
      },
      {
        className: 'array',
        begin: '\\[', end: '\\]'
      },
      hljs.HASH_COMMENT_MODE,
      hljs.COMMENT(
        '/\\*', // begin
        '\\*/', // end
        {
          contains: [ 'self' ]
        }
      )
    ]
  }
}

