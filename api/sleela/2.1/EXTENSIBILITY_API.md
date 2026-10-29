# SLeeLa Extensibility and Source Routing API

## Purpose
SLeeLa should make expansion an architectural property rather than a collection of special cases. The extensibility layer provides a generic route from an implied hierarchy or chain of context to a concrete source binding.

**Scope → Area → Chain → Node → Decision → Target → Source Binding → Implementation**

A request can therefore move through system → network → tcp → transport, or program → process → service → handler, without requiring a new hard-coded central dispatcher branch for every future area.

## Generic method
SourceRouter::route(scope, area, context) selects a hierarchy, evaluates its ordered chain, records the selected target, and resolves the target through source bindings. A source identifier may name C++, generated source, a native provider, plugin, package, executable component, or another implementation boundary.

## Responsibility classes
RouteTarget, RouteNode, RouteContext, RouteDecision, RouteChain, SourceBinding, SourceResolver, ExtensionPoint, Extension, ExtensionDescriptor, ExtensionRegistry, HierarchyRouter, SourceRouter.

## Expansion rule
Prefer adding a scope, area, route node, extension point, or source binding before changing central dispatch. Core changes should be reserved for changes to routing semantics.

## Next architectural sets
**Network:** transports, protocols, addresses, connections, packet/frame boundaries, multiplexing, discovery, proxying, NAT, firewall awareness and network services.

**Program / Process:** program identity, process lifecycle, execution plans, executable discovery, environment inheritance, process groups, signals, IPC, workers, supervision, resource limits and process services.

**Cross-cutting:** configuration, capabilities, permissions, observability, persistence, plugins, scripting, generated code, diagnostics, testing and platform adapters.

**Max Rupplin — MEARVK LLC — 2026**
