# Regex Library Index

Version: 1.2.0-dev

The regex library is the complete SLeeLa-facing object family. The current repository contains **27 separate SLeeLa source files**.

| Object | Role |
|---|---|
| Regex | top-level regex object |
| RegexCapability | backend capability description |
| RegexCapture | captured value metadata |
| RegexCompiler | pattern compilation |
| RegexDialect | syntax/backend dialect |
| RegexEngine | execution engine abstraction |
| RegexError | regex diagnostics/errors |
| RegexFlags | matching flags |
| RegexIterator | repeated-match iteration |
| RegexLiteral | literal construction/escaping |
| RegexMatch | individual match |
| RegexMatcher | matching/search operations |
| RegexNatural | Natural Form contract |
| RegexNaturalGrammar | Natural Form grammar |
| RegexNaturalGroup | Natural Form grouping |
| RegexNaturalParser | Natural Form parser |
| RegexNaturalSymbol | Natural Form symbols/aliases |
| RegexOptions | execution/compile options |
| RegexPattern | compiled/pattern representation |
| RegexReplacement | replacement description |
| RegexReplacer | replacement operations |
| RegexResult | operation result |
| RegexScanner | scanning operations |
| RegexSplitter | split operations |
| RegexSubject | input subject |
| RegexSystem | regex subsystem coordination |
| RegexValidator | validation and diagnostics |

Every object is a separate SLeeLa source file so IDE indexing, compiler diagnostics, loader discovery, library discovery, and test coverage can operate per object.

## Inventory Rule

The authoritative source inventory is the actual contents of `lib/regex/`. The regex test suite compares that directory against the 27-object manifest in `test-suites/test_sleela_sources.sh` and fails on missing, empty, or unexpected regex source files.

SLeeLa — MEARVK LLC — 2026
