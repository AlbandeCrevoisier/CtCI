#!/usr/bin/env python
#
# Copyright (c) 2009 Google Inc. All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are
# met:
#
#    * Redistributions of source code must retain the above copyright
# notice, this list of conditions and the following disclaimer.
#    * Redistributions in binary form must reproduce the above
# copyright notice, this list of conditions and the following disclaimer
# in the documentation and/or other materials provided with the
# distribution.
#    * Neither the name of Google Inc. nor the names of its
# contributors may be used to endorse or promote products derived from
# this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
# "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
# LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
# A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
# OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
# SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
# LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
# DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
# THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
# OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

"""Does google-lint on c++ files.

The goal of this script is to identify places in the code that *may*
be in non-compliance with google style.  It does not attempt to fix
up these problems -- the point is to educate.  It does also not
attempt to find all problems, or to ensure that everything it does
find is legitimately a problem.

In particular, we can get very confused by /* and // inside strings!
We do a small hack, which is to ignore //'s with "'s after them on the
same line, but it is far from perfect (in either direction).
"""

import codecs
import copy
import getopt
import math  # for log
import os
import re
import sre_compile
import string
import sys
import unicodedata
import sysconfig

try:
  xrange          # Python 2
except NameError:
  xrange = range  # Python 3


_USAGE = """
Syntax: cpplint.py [--verbose=#] [--output=vs7] [--filter=-x,+y,...]
                   [--counting=total|toplevel|detailed] [--root=subdir]
                   [--linelength=digits] [--headers=x,y,...]
                   [--quiet]
        <file> [file] ...

  The style guidelines this tries to follow are those in
    https://google-styleguide.googlecode.com/svn/trunk/cppguide.xml

  Every problem is given a confidence score from 1-5, with 5 meaning we are
  certain of the problem, and 1 meaning it could be a legitimate construct.
  This will miss some errors, and is not a substitute for a code review.

  To suppress false-positive errors of a certain category, add a
  'NOLINT(category)' comment to the line.  NOLINT or NOLINT(*)
  suppresses errors of all categories on that line.

  The files passed in will be linted; at least one file must be provided.
  Default linted extensions are .cc, .cpp, .cu, .cuh and .h.  Change the
  extensions with the --extensions flag.

  Flags:

    output=vs7
      By default, the output is formatted to ease emacs parsing.  Visual Studio
      compatible output (vs7) may also be used.  Other formats are unsupported.

    verbose=#
      Specify a number 0-5 to restrict errors to certain verbosity levels.

    quiet
      Don't print anything if no errors are found.

    filter=-x,+y,...
      Specify a comma-separated list of category-filters to apply: only
      error messages whose category names pass the filters will be printed.
      (Category names are printed with the message and look like
      "[whitespace/indent]".)  Filters are evaluated left to right.
      "-FOO" and "FOO" means "do not print categories that start with FOO".
      "+FOO" means "do print categories that start with FOO".

      Examples: --filter=-whitespace,+whitespace/braces
                --filter=whitespace,runtime/printf,+runtime/printf_format
                --filter=-,+build/include_what_you_use

      To see a list of all the categories used in cpplint, pass no arg:
         --filter=

    counting=total|toplevel|detailed
      The total number of errors found is always printed. If
      'toplevel' is provided, then the count of errors in each of
      the top-level categories like 'build' and 'whitespace' will
      also be printed. If 'detailed' is provided, then a count
      is provided for each category like 'build/class'.

    root=subdir
      The root directory used for deriving header guard CPP variable.
      By default, the header guard CPP variable is calculated as the relative
      path to the directory that contains .git, .hg, or .svn.  When this flag
      is specified, the relative path is calculated from the specified
      directory. If the specified directory does not exist, this flag is
      ignored.

      Examples:
        Assuming that top/src/.git exists (and cwd=top/src), the header guard
        CPP variables for top/src/chrome/browser/ui/browser.h are:

        No flag => CHROME_BROWSER_UI_BROWSER_H_
        --root=chrome => BROWSER_UI_BROWSER_H_
        --root=chrome/browser => UI_BROWSER_H_
        --root=.. => SRC_CHROME_BROWSER_UI_BROWSER_H_

    linelength=digits
      This is the allowed line length for the project. The default value is
      80 characters.

      Examples:
        --linelength=120

    extensions=extension,extension,...
      The allowed file extensions that cpplint will check

      Examples:
        --extensions=hpp,cpp

    headers=x,y,...
      The header extensions that cpplint will treat as .h in checks. Values are
      automatically added to --extensions list.

      Examples:
        --headers=hpp,hxx
        --headers=hpp

    cpplint.py supports per-directory configurations specified in CPPLINT.cfg
    files. CPPLINT.cfg file can contain a number of key=value pairs.
    Currently the following options are supported:

      set noparent
      filter=+filter1,-filter2,...
      exclude_files=regex
      linelength=80
      root=subdir
      headers=x,y,...

    "set noparent" option prevents cpplint from traversing directory tree
    upwards looking for more .cfg files in parent directories. This option
    is usually placed in the top-level project directory.

    The "filter" option is similar in function to --filter flag. It specifies
    message filters in addition to the |_DEFAULT_FILTERS| and those specified
    through --filter command-line flag.

    "exclude_files" allows to specify a regular expression to be matched against
    a file name. If the expression matches, the file is skipped and not run
    through liner.

    "linelength" allows to specify the allowed line length for the project.

    The "root" option is similar in function to the --root flag (see example
    above). Paths are relative to the directory of the CPPLINT.cfg.

    The "headers" option is similar in function to the --headers flag
    (see example above).

    CPPLINT.cfg has an effect on files in the same directory and all
    sub-directories, unless overridden by a nested configuration file.

      Example file:
        filter=-build/include_order,+build/include_alpha
        exclude_files=.*\.cc

    The above example disables build/include_order warning and enables
    build/include_alpha as well as excludes all .cc from being
    processed by linter, in the current directory (where the .cfg
    file is located) and all sub-directories.
"""

# We categorize each error message we print.  Here are the categories.
# We want an explicit list so we can list them all in cpplint --filter=.
# If you add a new error message with a new category, add it to the list
# here!  cpplint_unittest.py should tell you if you forget to do this.
_ERROR_CATEGORIES = [
    'build/class',
    'build/c++11',
    'build/c++14',
    'build/c++tr1',
    'build/deprecated',
    'build/endif_comment',
    'build/explicit_make_pair',
    'build/forward_decl',
    'build/header_guard',
    'build/include',
    'build/include_alpha',
    'build/include_order',
    'build/include_what_you_use',
    'build/namespaces',
    'build/printf_format',
    'build/storage_class',
    'legal/copyright',
    'readability/alt_tokens',
    'readability/braces',
    'readability/casting',
    'readability/check',
    'readability/constructors',
    'readability/fn_size',
    'readability/inheritance',
    'readability/multiline_comment',
    'readability/multiline_string',
    'readability/namespace',
    'readability/nolint',
    'readability/nul',
    'readability/strings',
    'readability/todo',
    'readability/utf8',
    'runtime/arrays',
    'runtime/casting',
    'runtime/explicit',
    'runtime/int',
    'runtime/init',
    'runtime/invalid_increment',
    'runtime/member_string_references',
    'runtime/memset',
    'runtime/indentation_namespace',
    'runtime/operator',
    'runtime/printf',
    'runtime/printf_format',
    'runtime/references',
    'runtime/string',
    'runtime/threadsafe_fn',
    'runtime/vlog',
    'whitespace/blank_line',
    'whitespace/braces',
    'whitespace/comma',
    'whitespace/comments',
    'whitespace/empty_conditional_body',
    'whitespace/empty_if_body',
    'whitespace/empty_loop_body',
    'whitespace/end_of_line',
    'whitespace/ending_newline',
    'whitespace/forcolon',
    'whitespace/indent',
    'whitespace/line_length',
    'whitespace/newline',
    'whitespace/operators',
    'whitespace/parens',
    'whitespace/semicolon',
    'whitespace/tab',
    'whitespace/todo',
    ]

# These error categories are no longer enforced by cpplint, but for backwards-
# compatibility they may still appear in NOLINT comments.
_LEGACY_ERROR_CATEGORIES = [
    'readability/streams',
    'readability/function',
    ]

# The default state of the category filter. This is overridden by the --filter=
# flag. By default all errors are on, so only add here categories that should be
# off by default (i.e., categories that must be enabled by the --filter= flags).
# All entries here should start with a '-' or '+', as in the --filter= flag.
_DEFAULT_FILTERS = ['-build/include_alpha']

# The default list of categories suppressed for C (not C++) files.
_DEFAULT_C_SUPPRESSED_CATEGORIES = [
    'readability/casting',
    ]

# The default list of categories suppressed for Linux Kernel files.
_DEFAULT_KERNEL_SUPPRESSED_CATEGORIES = [
    'whitespace/tab',
    ]

# We used to check for high-bit characters, but after much discussion we
# decided those were OK, as long as they were in UTF-8 and didn't represent
# hard-coded international strings, which belong in a separate i18n file.

# C++ headers
_CPP_HEADERS = frozenset([
    # Legacy
    'algobase.h',
    'algo.h',
    'alloc.h',
    'builtinbuf.h',
    'bvector.h',
    'complex.h',
    'defalloc.h',
    'deque.h',
    'editbuf.h',
    'fstream.h',
    'function.h',
    'hash_map',
    'hash_map.h',
    'hash_set',
    'hash_set.h',
    'hashtable.h',
    'heap.h',
    'indstream.h',
    'iomanip.h',
    'iostream.h',
    'istream.h',
    'iterator.h',
    'list.h',
    'map.h',
    'multimap.h',
    'multiset.h',
    'ostream.h',
    'pair.h',
    'parsestream.h',
    'pfstream.h',
    'procbuf.h',
    'pthread_alloc',
    'pthread_alloc.h',
    'rope',
    'rope.h',
    'ropeimpl.h',
    'set.h',
    'slist',
    'slist.h',
    'stack.h',
    'stdiostream.h',
    'stl_alloc.h',
    'stl_relops.h',
    'streambuf.h',
    'stream.h',
    'strfile.h',
    'strstream.h',
    'tempbuf.h',
    'tree.h',
    'type_traits.h',
    'vector.h',
    # 17.6.1.2 C++ library headers
    'algorithm',
    'array',
    'atomic',
    'bitset',
    'chrono',
    'codecvt',
    'complex',
    'condition_variable',
    'deque',
    'exception',
    'forward_list',
    'fstream',
    'functional',
    'future',
    'initializer_list',
    'iomanip',
    'ios',
    'iosfwd',
    'iostream',
    'istream',
    'iterator',
    'limits',
    'list',
    'locale',
    'map',
    'memory',
    'mutex',
    'new',
    'numeric',
    'ostream',
    'queue',
    'random',
    'ratio',
    'regex',
    'scoped_allocator',
    'set',
    'sstream',
    'stack',
    'stdexcept',
    'streambuf',
    'string',
    'strstream',
    'system_error',
    'thread',
    'tuple',
    'typeindex',
    'typeinfo',
    'type_traits',
    'unordered_map',
    'unordered_set',
    'utility',
    'valarray',
    'vector',
    # 17.6.1.2 C++ headers for C library facilities
    'cassert',
    'ccomplex',
    'cctype',
    'cerrno',
    'cfenv',
    'cfloat',
    'cinttypes',
    'ciso646',
    'climits',
    'clocale',
    'cmath',
    'csetjmp',
    'csignal',
    'cstdalign',
    'cstdarg',
    'cstdbool',
    'cstddef',
    'cstdint',
    'cstdio',
    'cstdlib',
    'cstring',
    'ctgmath',
    'ctime',
    'cuchar',
    'cwchar',
    'cwctype',
    ])

# Type names
_TYPES = re.compile(
    r'^(?:'
    # [dcl.type.simple]
    r'(char(16_t|32_t)?)|wchar_t|'
    r'bool|short|int|long|signed|unsigned|float|double|'
    # [support.types]
    r'(ptrdiff_t|size_t|max_align_t|nullptr_t)|'
    # [cstdint.syn]
    r'(u?int(_fast|_least)?(8|16|32|64)_t)|'
    r'(u?int(max|ptr)_t)|'
    r')$')


# These headers are excluded from [build/include] and [build/include_order]
# checks:
# - Anything not following google file name conventions (containing an
#   uppercase character, such as Python.h or nsStringAPI.h, for example).
# - Lua headers.
_THIRD_PARTY_HEADERS_PATTERN = re.compile(
    r'^(?:[^/]*[A-Z][^/]*\.h|lua\.h|lauxlib\.h|lualib\.h)$')

# Pattern for matching FileInfo.BaseName() against test file name
_TEST_FILE_SUFFIX = r'(_test|_unittest|_regtest)$'

# Pattern that matches only complete whitespace, possibly across multiple lines.
_EMPTY_CONDITIONAL_BODY_PATTERN = re.compile(r'^\s*$', re.DOTALL)

# Assertion macros.  These are defined in base/logging.h and
# testing/base/public/gunit.h.
_CHECK_MACROS = [
    'DCHECK', 'CHECK',
    'EXPECT_TRUE', 'ASSERT_TRUE',
    'EXPECT_FALSE', 'ASSERT_FALSE',
    ]

# Replacement macros for CHECK/DCHECK/EXPECT_TRUE/EXPECT_FALSE
_CHECK_REPLACEMENT = dict([(m, {}) for m in _CHECK_MACROS])

for op, replacement in [('==', 'EQ'), ('!=', 'NE'),
                        ('>=', 'GE'), ('>', 'GT'),
                        ('<=', 'LE'), ('<', 'LT')]:
  _CHECK_REPLACEMENT['DCHECK'][op] = 'DCHECK_%s' % replacement
  _CHECK_REPLACEMENT['CHECK'][op] = 'CHECK_%s' % replacement
  _CHECK_REPLACEMENT['EXPECT_TRUE'][op] = 'EXPECT_%s' % replacement
  _CHECK_REPLACEMENT['ASSERT_TRUE'][op] = 'ASSERT_%s' % replacement

for op, inv_replacement in [('==', 'NE'), ('!=', 'EQ'),
                            ('>=', 'LT'), ('>', 'LE'),
                            ('<=', 'GT'), ('<', 'GE')]:
  _CHECK_REPLACEMENT['EXPECT_FALSE'][op] = 'EXPECT_%s' % inv_replacement
  _CHECK_REPLACEMENT['ASSERT_FALSE'][op] = 'ASSERT_%s' % inv_replacement

# Alternative tokens and their replacements.  For full list, see section 2.5
# Alternative tokens [lex.digraph] in the C++ standard.
#
# Digraphs (such as '%:') are not included here since it's a mess to
# match those on a word boundary.
_ALT_TOKEN_REPLACEMENT = {
    'and': '&&',
    'bitor': '|',
    'or': '||',
    'xor': '^',
    'compl': '~',
    'bitand': '&',
    'and_eq': '&=',
    'or_eq': '|=',
    'xor_eq': '^=',
    'not': '!',
    'not_eq': '!='
    }

# Compile regular expression that matches all the above keywords.  The "[ =()]"
# bit is meant to avoid matching these keywords outside of boolean expressions.
#
# False positives include C-style multi-line comments and multi-line strings
# but those have always been troublesome for cpplint.
_ALT_TOKEN_REPLACEMENT_PATTERN = re.compile(
    r'[ =()](' + ('|'.join(_ALT_TOKEN_REPLACEMENT.keys())) + r')(?=[ (]|$)')


# These constants define types of headers for use with
# _IncludeState.CheckNextIncludeOrder().
_C_SYS_HEADER = 1
_CPP_SYS_HEADER = 2
_LIKELY_MY_HEADER = 3
_POSSIBLE_MY_HEADER = 4
_OTHER_HEADER = 5

# These constants define the current inline assembly state
_NO_ASM = 0       # Outside of inline assembly block
_INSIDE_ASM = 1   # Inside inline assembly block
_END_ASM = 2      # Last line of inline assembly block
_BLOCK_ASM = 3    # The whole block is an inline assembly block

# Match start of assembly blocks
_MATCH_ASM = re.compile(r'^\s*(?:asm|_asm|__asm|__asm__)'
                        r'(?:\s+(volatile|__volatile__))?'
                        r'\s*[{(]')

# Match strings that indicate we're working on a C (not C++) file.
_SEARCH_C_FILE = re.compile(r'\b(?:LINT_C_FILE|'
                            r'vim?:\s*.*(\s*|:)filetype=c(\s*|:|$))')

# Match string that indicates we're working on a Linux Kernel file.
_SEARCH_KERNEL_FILE = re.compile(r'\b(?:LINT_KERNEL_FILE)')

_regexp_compile_cache = {}

# {str, set(int)}: a map from error categories to sets of linenumbers
# on which those errors are expected and should be suppressed.
_error_suppressions = {}

# The root directory used for deriving header guard CPP variable.
# This is set by --root flag.
_root = None
_root_debug = False

# The allowed line length of files.
# This is set by --linelength flag.
_line_length = 80

# The allowed extensions for file names
# This is set by --extensions flag.
_valid_extensions = set(['cc', 'h', 'cpp', 'cu', 'cuh'])

# Treat all headers starting with 'h' equally: .h, .hpp, .hxx etc.
# This is set by --headers flag.
_hpp_headers = set(['h'])

# {str, bool}: a map from error categories to booleans which indicate if the
# category should be suppressed for every line.
_global_error_suppressions = {}

def ProcessHppHeadersOption(val):
  global _hpp_headers
  try:
    _hpp_headers = set(val.split(','))
    # Automatically append to extensions list so it does not have to be set 2 times
    _valid_extensions.update(_hpp_headers)
  except ValueError:
    PrintUsage('Header extensions must be comma separated list.')

def IsHeaderExtension(file_extension):
  return file_extension in _hpp_headers

def ParseNolintSuppressions(filename, raw_line, linenum, error):
  """Updates the global list of line error-suppressions.

  Parses any NOLINT comments on the current line, updating the global
  error_suppressions store.  Reports an error if the NOLINT comment
  was malformed.

  Args:
    filename: str, the name of the input file.
    raw_line: str, the line of input text, with comments.
    linenum: int, the number of the current line.
    error: function, an error handler.
  """
  matched = Search(r'\bNOLINT(NEXTLINE)?\b(\([^)]+\))?', raw_line)
  if matched:
    if matched.group(1):
      suppressed_line = linenum + 1
    else:
      suppressed_line = linenum
    category = matched.group(2)
    if category in (None, '(*)'):  # => "suppress all"
      _error_suppressions.setdefault(None, set()).add(suppressed_line)
    else:
      if category.startswith('(') and category.endswith(')'):
        category = category[1:-1]
        if category in _ERROR_CATEGORIES:
          _error_suppressions.setdefault(category, set()).add(suppressed_line)
        elif category not in _LEGACY_ERROR_CATEGORIES:
          error(filename, linenum, 'readability/nolint', 5,
                'Unknown NOLINT error category: %s' % category)


def ProcessGlobalSuppresions(lines):
  """Updates the list of global error suppressions.

  Parses any lint directives in the file that have global effect.

  Args:
    lines: An array of strings, each representing a line of the file, with the
           last element being empty if the file is terminated with a newline.
  """
  for line in lines:
    if _SEARCH_C_FILE.search(line):
      for category in _DEFAULT_C_SUPPRESSED_CATEGORIES:
        _global_error_suppressions[category] = True
    if _SEARCH_KERNEL_FILE.search(line):
      for category in _DEFAULT_KERNEL_SUPPRESSED_CATEGORIES:
        _global_error_suppressions[category] = True


def ResetNolintSuppressions():
  """Resets the set of NOLINT suppressions to empty."""
  _error_suppressions.clear()
  _global_error_suppressions.clear()


def IsErrorSuppressedByNolint(category, linenum):
  """Returns true if the specified error category is suppressed on this line.

  Consults the global error_suppressions map populated by
  ParseNolintSuppressions/ProcessGlobalSuppresions/ResetNolintSuppressions.

  Args:
    category: str, the category of the error.
    linenum: int, the current line number.
  Returns:
    bool, True iff the error should be suppressed due to a NOLINT comment or
    global suppression.
  """
  return (_global_error_suppressions.get(category, False) or
          linenum in _error_suppressions.get(category, set()) or
          linenum in _error_suppressions.get(None, set()))


def Match(pattern, s):
  """Matches the string with the pattern, caching the compiled regexp."""
  # The regexp compilation caching is inlined in both Match and Search for
  # performance reasons; factoring it out into a separate function turns out
  # to be noticeably expensive.
  if pattern not in _regexp_compile_cache:
    _regexp_compile_cache[pattern] = sre_compile.compile(pattern)
  return _regexp_compile_cache[pattern].match(s)


def ReplaceAll(pattern, rep, s):
  """Replaces instances of pattern in a string with a replacement.

  The compiled regex is kept in a cache shared by Match and Search.

  Args:
    pattern: regex pattern
    rep: replacement text
    s: search string

  Returns:
    string with replacements made (or original string if no replacements)
  """
  if pattern not in _regexp_compile_cache:
    _regexp_compile_cache[pattern] = sre_compile.compile(pattern)
  return _regexp_compile_cache[pattern].sub(rep, s)


def Search(pattern, s):
  """Searches the string for the pattern, caching the compiled regexp."""
  if pattern not in _regexp_compile_cache:
    _regexp_compile_cache[pattern] = sre_compile.compile(pattern)
  return _regexp_compile_cache[pattern].search(s)


def _IsSourceExtension(s):
  """File extension (excluding dot) matches a source file extension."""
  return s in ('c', 'cc', 'cpp', 'cxx')


class _IncludeState(object):
  """Tracks line numbers for includes, and the order in which includes appear.

  include_list contains list of lists of (header, line number) pairs.
  It's a lists of lists rather than just one flat list to make it
  easier to update across preprocessor boundaries.

  Call CheckNextIncludeOrder() once for each header in the file, passing
  in the type constants defined above. Calls in an illegal order will
  raise an _IncludeError with an appropriate error message.

  """
  # self._section will move monotonically through this set. If it ever
  # needs to move backwards, CheckNextIncludeOrder will raise an error.
  _INITIAL_SECTION = 0
  _MY_H_SECTION = 1
  _C_SECTION = 2
  _CPP_SECTION = 3
  _OTHER_H_SECTION = 4

  _TYPE_NAMES = {
      _C_SYS_HEADER: 'C system header',
      _CPP_SYS_HEADER: 'C++ system header',
      _LIKELY_MY_HEADER: 'header this file implements',
      _POSSIBLE_MY_HEADER: 'header this file may implement',
      _OTHER_HEADER: 'other header',
      }
  _SECTION_NAMES = {
      _INITIAL_SECTION: "... nothing. (This can't be an error.)",
      _MY_H_SECTION: 'a header this file implements',
      _C_SECTION: 'C system header',
      _CPP_SECTION: 'C++ system header',
      _OTHER_H_SECTION: 'other header',
      }

  def __init__(self):
    self.include_list = [[]]
    self.ResetSection('')

  def FindHeader(self, header):
    """Check if a header has already been included.

    Args:
      header: header to check.
    Returns:
      Line number of previous occurrence, or -1 if the header has not
      been seen before.
    """
    for section_list in self.include_list:
      for f in section_list:
        if f[0] == header:
          return f[1]
    return -1

  def ResetSection(self, directive):
    """Reset section checking for preprocessor directive.

    Args:
      directive: preprocessor directive (e.g. "if", "else").
    """
    # The name of the current section.
    self._section = self._INITIAL_SECTION
    # The path of last found header.
    self._last_header = ''

    # Update list of includes.  Note that we never pop from the
    # include list.
    if directive in ('if', 'ifdef', 'ifndef'):
      self.include_list.append([])
    elif directive in ('else', 'elif'):
      self.include_list[-1] = []

  def SetLastHeader(self, header_path):
    self._last_header = header_path

  def CanonicalizeAlphabeticalOrder(self, header_path):
    """Returns a path canonicalized for alphabetical comparison.

    - replaces "-" with "_" so they both cmp the same.
    - removes '-inl' since we don't require them to be after the main header.
    - lowercase everything, just in case.

    Args:
      header_path: Path to be canonicalized.

    Returns:
      Canonicalized path.
    """
    return header_path.replace('-inl.h', '.h').replace('-', '_').lower()

  def IsInAlphabeticalOrder(self, clean_lines, linenum, header_path):
    """Check if a header is in alphabetical order with the previous header.

    Args:
      clean_lines: A CleansedLines instance containing the file.
      linenum: The number of the line to check.
      header_path: Canonicalized header to be checked.

    Returns:
      Returns true if the header is in alphabetical order.
    """
    # If previous section is different from current section, _last_header will
    # be reset to empty string, so it's always less than current header.
    #
    # If previous line was a blank line, assume that the headers are
    # intentionally sorted the way they are.
    if (self._last_header > header_path and
        Match(r'^\s*#\s*include\b', clean_lines.elided[linenum - 1])):
      return False
    return True

  def CheckNextIncludeOrder(self, header_type):
    """Returns a non-empty error message if the next header is out of order.

    This function also updates the internal state to be ready to check
    the next include.

    Args:
      header_type: One of the _XXX_HEADER constants defined above.

    Returns:
      The empty string if the header is in the right order, or an
      error message describing what's wrong.

    """
    error_message = ('Found %s after %s' %
                     (self._TYPE_NAMES[header_type],
                      self._SECTION_NAMES[self._section]))

    last_section = self._section

    if header_type == _C_SYS_HEADER:
      if self._section <= self._C_SECTION:
        self._section = self._C_SECTION
      else:
        self._last_header = ''
        return error_message
    elif header_type == _CPP_SYS_HEADER:
      if self._section <= self._CPP_SECTION:
        self._section = self._CPP_SECTION
      else:
        self._last_header = ''
        return error_message
    elif header_type == _LIKELY_MY_HEADER:
      if self._section <= self._MY_H_SECTION:
        self._section = self._MY_H_SECTION
      else:
        self._section = self._OTHER_H_SECTION
    elif header_type == _POSSIBLE_MY_HEADER:
      if self._section <= self._MY_H_SECTION:
        self._section = self._MY_H_SECTION
      else:
        # This will always be the fallback because we're not sure
        # enough that the header is associated with this file.
        self._section = self._OTHER_H_SECTION
    else:
      assert header_type == _OTHER_HEADER
      self._section = self._OTHER_H_SECTION

    if last_section != self._section:
      self._last_header = ''

    return ''


class _CppLintState(object):
  """Maintains module-wide state.."""

  def __init__(self):
    self.verbose_level = 1  # global setting.
    self.error_count = 0    # global count of reported errors
    # filters to apply when emitting error messages
    self.filters = _DEFAULT_FILTERS[:]
    # backup of filter list. Used to restore the state after each file.
    self._filters_backup = self.filters[:]
    self.counting = 'total'  # In what way are we counting errors?
    self.errors_by_category = {}  # string to int dict storing error counts
    self.quiet = False  # Suppress non-error messagess?

    # output format:
    # "emacs" - format that emacs can parse (default)
    # "vs7" - format that Microsoft Visual Studio 7 can parse
    self.output_format = 'emacs'

  def SetOutputFormat(self, output_format):
    """Sets the output format for errors."""
    self.output_format = output_format

  def SetQuiet(self, quiet):
    """Sets the module's quiet settings, and returns the previous setting."""
    last_quiet = self.quiet
    self.quiet = quiet
    return last_quiet

  def SetVerboseLevel(self, level):
    """Sets the module's verbosity, and returns the previous setting."""
    last_verbose_level = self.verbose_level
    self.verbose_level = level
    return last_verbose_level

  def SetCountingStyle(self, counting_style):
    """Sets the module's counting options."""
    self.counting = counting_style

  def SetFilters(self, filters):
    """Sets the error-message filters.

    These filters are applied when deciding whether to emit a given
    error message.

    Args:
      filters: A string of comma-separated filters (eg "+whitespace/indent").
               Each filter should start with + or -; else we die.

    Raises:
      ValueError: The comma-separated filters did not all start with '+' or '-'.
                  E.g. "-,+whitespace,-whitespace/indent,whitespace/badfilter"
    """
    # Default filters always have less priority than the flag ones.
    self.filters = _DEFAULT_FILTERS[:]
    self.AddFilters(filters)

  def AddFilters(self, filters):
    """ Adds more filters to the existing list of error-message filters. """
    for filt in filters.split(','):
      clean_filt = filt.strip()
      if clean_filt:
        self.filters.append(clean_filt)
    for filt in self.filters:
      if not (filt.startswith('+') or filt.startswith('-')):
        raise ValueError('Every filter in --filters must start with + or -'
                         ' (%s does not)' % filt)

  def BackupFilters(self):
    """ Saves the current filter list to backup storage."""
    self._filters_backup = self.filters[:]

  def RestoreFilters(self):
    """ Restores filters previously backed up."""
    self.filters = self._filters_backup[:]

  def ResetErrorCounts(self):
    """Sets the module's error statistic back to zero."""
    self.error_count = 0
    self.errors_by_category = {}

  def IncrementErrorCount(self, category):
    """Bumps the module's error statistic."""
    self.error_count += 1
    if self.counting in ('toplevel', 'detailed'):
      if self.counting != 'detailed':
        category = category.split('/')[0]
      if category not in self.errors_by_category:
        self.errors_by_category[category] = 0
      self.errors_by_category[category] += 1

  def PrintErrorCounts(self):
    """Print a summary of errors by category, and the total."""
    for category, count in self.errors_by_category.iteritems():
      sys.stderr.write('Category \'%s\' errors found: %d\n' %
                       (category, count))
    sys.stdout.write('Total errors found: %d\n' % self.error_count)

_cpplint_state = _CppLintState()


def _OutputFormat():
  """Gets the module's output format."""
  return _cpplint_state.output_format


def _SetOutputFormat(output_format):
  """Sets the module's output format."""
  _cpplint_state.SetOutputFormat(output_format)

def _Quiet():
  """Return's the module's quiet setting."""
  return _cpplint_state.quiet

def _SetQuiet(quiet):
  """Set the module's quiet status, and return previous setting."""
  return _cpplint_state.SetQuiet(quiet)


def _VerboseLevel():
  """Returns the module's verbosity setting."""
  return _cpplint_state.verbose_level


def _SetVerboseLevel(level):
  """Sets the module's verbosity, and returns the previous setting."""
  return _cpplint_state.SetVerboseLevel(level)


def _SetCountingStyle(level):
  """Sets the module's counting options."""
  _cpplint_state.SetCountingStyle(level)


def _Filters():
  """Returns the module's list of output filters, as a list."""
  return _cpplint_state.filters


def _SetFilters(filters):
  """Sets the module's error-message filters.

  These filters are applied when deciding whether to emit a given
  error message.

  Args:
    filters: A string of comma-separated filters (eg "whitespace/indent").
             Each filter should start with + or -; else we die.
  """
  _cpplint_state.SetFilters(filters)

def _AddFilters(filters):
  """Adds more filter overrides.

  Unlike _SetFilters, this fu