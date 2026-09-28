# Regex Feature Matrix

Core operations are compile/validate, full match, search, repeated matching through host APIs, captures, replacement, split, literal escaping, flags, offsets, and diagnostics.

The portable syntax contract is POSIX extended regular expressions: literals, dot, bracket expressions, ranges, anchors, grouping, alternation, ?, *, +, and bounded repetition where the host engine supports it.

C uses the POSIX regex interface. C++ uses std::regex. Perl-only constructs such as lookahead, lookbehind, atomic groups, recursion, and engine-specific backtracking controls are not silently treated as portable SLeeLa syntax. A future adapter can add such features under explicit capability reporting.
