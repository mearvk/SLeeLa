# Tutorial 02 — Compiler Manager and Profiles

## BASIC_COMPLETE

Use this profile for a complete practical compiler path. It still accounts for required VM objects and dependencies.

## ADVANCED_TOTAL

Use this profile when the plan must explicitly inventory semantic requirements, capabilities, dependencies, security constraints, provenance, target architecture, VM fitment, and output requirements.

## Flow

Compiler request -> profile -> source/semantic validation -> dependency inventory -> VM object inventory -> capability/security checks -> target fitment -> compile report.

The Compiler Manager is a completeness gate. It rejects missing requirements rather than inventing them.

C provides a stable ABI and C++ may orchestrate. Neither bypasses SLeeLa capability, resolver, memory, certificate, security, or VM boundaries.
