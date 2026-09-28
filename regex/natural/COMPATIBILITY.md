# Natural Form Compatibility

Natural Form is the SLeeLa contract. C, C++, Java, and SLeeLa implementations should parse the same core grammar into the same conceptual AST.

POSIX ERE and C++ std::regex are adapters, not the definition of Natural Form. Unsupported constructs MUST produce a diagnostic.

Lookaround, recursion, atomic groups, engine-specific backtracking controls, and replacement-language extensions are outside the core contract until explicitly standardized.
