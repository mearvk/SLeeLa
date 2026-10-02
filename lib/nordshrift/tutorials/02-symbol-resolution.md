# Nordshrift Tutorial 02 — Symbol Resolution

## Lifecycle

~~~text
SSTSymbol
    |
    v
SSTSymbolResolution
    |
    +-- resolved
    +-- unresolved
    +-- ambiguous
    |
    v
NordshriftSymbol
~~~

Symbols use the canonical identity <package>.<symbol>. For example, sst.SSTSymbol and nordshrift.NordshriftSymbol are distinct symbols.

## Rule

Do not resolve a cross-package symbol by short name alone.

## Exercise

Create two references with the same short name in different packages and verify that their package-qualified identities remain distinct.
