# Regex and SLeeLa

SLeeLa is the authoritative language-facing layer. Native C/C++ exists below an explicit native bridge.

The library objects are under lib/regex/: Regex.sleela, RegexPattern.sleela, RegexMatch.sleela, RegexCapture.sleela, RegexFlags.sleela, RegexResult.sleela, RegexError.sleela, and RegexEngine.sleela.

Native implementations preserve these object-level contracts and do not create a second incompatible SLeeLa regex language.
