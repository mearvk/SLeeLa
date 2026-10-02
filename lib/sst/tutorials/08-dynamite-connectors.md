# SST Tutorial 08 — Dynamite Connectors

A Dynamite Connector marks a class whose configuration can be implied from defaults, source properties, or a named configuration.

Example:

~~~text
// @dynamite class=Resolver
// @dynamite.config=network-defaults
// @dynamite.property family=dual
~~~

The compiler discovers the marker before semantic compilation. The VM/terminal loader may then admit the class as an implicit-load candidate after normal symbol, configuration, capability, and security checks.

Configuration precedence is:

~~~text
source property > named configuration > known default > class default
~~~

Do not place secrets in source.

## Exercise

Model a Resolver, Network, and SQL connector with named configurations and non-secret properties.
