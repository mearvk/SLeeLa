# Nordshrift Tutorial 04 — Invalid and Ambiguous Symbols

An unresolved reference such as example.DoesNotExist remains unresolved and produces a diagnostic.

If multiple candidates could satisfy an insufficiently qualified reference, Nordshrift must report the ambiguity rather than silently selecting one.

Correct examples include:

~~~text
sst.SSTSymbol
nordshrift.NordshriftSymbol
~~~

Successful compilation therefore requires deterministic symbol resolution.
