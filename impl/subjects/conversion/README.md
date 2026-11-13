# conversion subject (Java -> SLeeLa)

`conversion.model` is the declarative model of the laws under which Java source
becomes functionally equivalent SLeeLa source: types, boxing, numeric promotion,
operator lowering, narrowing casts, compound assignment, string conversion, library
result types, and the constructs refused (fail closed).

It is consumed by `sleelactl/convert` (C lexer + model loader, C++ translator),
registered in sleelactl as `java-convert`. Unlike math/physics it is not compiled
into the `sleela` runtime: it governs a tool, and its executable half is
`lib/java/JavaOperands.sleela`. Rules: `.kiro/steering/java-to-sleela-conversion.md`.
