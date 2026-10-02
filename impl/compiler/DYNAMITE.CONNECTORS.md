# Dynamite Connectors

Dynamite is an explicit SLeeLa import form. A connector may defer construction to class defaults, a named configuration, or source properties.

## Single connector

Both original forms remain valid:

    import dynamite connector /lib/db/MySQL.sleela;
    dynamite config database-defaults;
    dynamite property host = "localhost";
    dynamite property port = 3306;

and:

    import :: dynamite :: connector :: /lib/db/MySQL.sleela;
    dynamite config database-defaults;
    dynamite property port = 3306;

## Multiple connectors

When a source document contains more than one Dynamite Connector, each connector can have a reference name.

Named form:

    import dynamite connector mysql = /lib/db/MySQL.sleela;
    dynamite config mysql = database-defaults;
    dynamite property mysql :: port = 3306;

Namespace-softened form:

    import :: dynamite :: connector :: mysql :: /lib/db/MySQL.sleela;
    dynamite config mysql :: database-defaults;
    dynamite property mysql :: port = 3306;

A second connector can be independently addressed:

    import :: dynamite :: connector :: postgres :: /lib/db/PostgreSQL.sleela;
    dynamite config postgres :: database-defaults;
    dynamite property postgres :: port = 5432;

The namespace form and named form identify the same kind of compiler reference. The reference name is scoped to the source document.

## Ambiguity rules

An unnamed Dynamite Connector is permitted when the source has exactly one connector. With multiple connectors, configuration and property statements should be explicitly scoped to a connector reference. The compiler must not guess which connector an unqualified setting belongs to.

Duplicate connector reference names are invalid. A reference must map to exactly one imported source path.

## Deferred construction

No explicit constructor or new expression is required at the source call site. The compiler resolves each connector independently:

    connector reference -> source resolution -> /lib symbol resolution
    -> configuration/property resolution -> capability/security review
    -> deferred construction -> VM/terminal load

Configuration precedence for each connector remains:

    source properties > named configuration > known package/default configuration > class defaults

Dynamite does not bypass normal parsing, semantic analysis, dependency checks, capability checks, or security policy. Secrets and credentials remain runtime configuration concerns.
