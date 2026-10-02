# Dynamite Connectors

Dynamite is an explicit SLeeLa import form. It identifies a source file whose construction is deferred to the compiler and VM loader because the class can use known defaults, a named configuration, or source properties.

## Syntax

    import dynamite connector /lib/db/MySQL.sleela;
    dynamite config database-defaults;
    dynamite property host = "localhost";
    dynamite property port = 3306;

The explicit import dynamite connector construct replaces the former comment-based @dynamite approach.

## Deferred construction

No explicit constructor or new expression is required at the source call site. The compiler resolves the imported source, then resolves configuration in this order:

    source properties > named configuration > known package/default configuration > class defaults

The VM/terminal loader performs initialization after those values are resolved. Secrets and credentials remain runtime configuration concerns.

## Pipeline

dynamite import -> source resolution -> /lib symbol resolution -> configuration/property resolution -> capability/security review -> deferred construction -> VM/terminal load

Dynamite does not bypass normal parsing, semantic analysis, dependency checks, capability checks, or security policy.
