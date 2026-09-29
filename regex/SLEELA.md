# Regex and SLeeLa

Version: 1.2.0-dev

SLeeLa is the authoritative language-facing layer for the regex subsystem. Native C/C++ and Java implementations operate below explicit language and backend boundaries.

## Complete SLeeLa Object Family

The current `lib/regex/` inventory contains 27 independent SLeeLa source objects:

- `Regex`
- `RegexCapability`
- `RegexCapture`
- `RegexCompiler`
- `RegexDialect`
- `RegexEngine`
- `RegexError`
- `RegexFlags`
- `RegexIterator`
- `RegexLiteral`
- `RegexMatch`
- `RegexMatcher`
- `RegexNatural`
- `RegexNaturalGrammar`
- `RegexNaturalGroup`
- `RegexNaturalParser`
- `RegexNaturalSymbol`
- `RegexOptions`
- `RegexPattern`
- `RegexReplacement`
- `RegexReplacer`
- `RegexResult`
- `RegexScanner`
- `RegexSplitter`
- `RegexSubject`
- `RegexSystem`
- `RegexValidator`

Each object remains separately addressable so IDE indexing, compiler diagnostics, loader discovery, and function-by-function testing can operate at object granularity.

## Contract

Natural Form is the portable SLeeLa contract. Native backends must preserve the SLeeLa object model and must not create a second incompatible regex language.

The loader and compiler should discover every `lib/regex/Regex*.sleela` object, not only the Natural Form subset.

SLeeLa — MEARVK LLC — 2026
