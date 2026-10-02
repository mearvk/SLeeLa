# Dynamite Connectors

A Dynamite Connector marks a primary SLeeLa source file whose class can be loaded by implication from known defaults, source properties, or a named configuration rather than requiring an explicit constructor/instance expression.

## Marker

~~~text
// @dynamite class=Resolver
// @dynamite.config=network-defaults
// @dynamite.property family=dual
~~~

The marker is comment-based so existing SLeeLa grammar remains valid.

## Lifecycle

source marker -> discovery -> /lib symbol resolution -> configuration resolution -> capability/security review -> VM/terminal loader

The marker never bypasses normal compiler, capability, security, or VM checks.

## Candidate classes

Initial conservative candidates include runtime/configuration services, Resolver, Network, Terminal, HTTP services, SQL connectors, LibraryIndex, SystemMonitor, Logger, Metrics, SecuritySupervisor, CertificateManager, Path, Time, Runtime, and VM startup/compiler-management services.

A class becomes Dynamite-loadable only when its primary source carries the marker and loader policy admits it.

## Configuration precedence

source property > named configuration > known package/default configuration > class default

Secrets must remain outside source.

## No-constructor rule

The marker removes the requirement for an explicit source-level constructor/instance expression for loader admission. Native bootstrap initialization may still occur using resolved configuration.
