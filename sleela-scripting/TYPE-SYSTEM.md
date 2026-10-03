# Sleela Script Turing 5 Type System

Turing 5 has a dynamic surface with strong runtime contracts at host boundaries.

## Core values

null, boolean, integer, real, rational, complex, string, list, map, tuple, function, quantity, matrix, interval, result and opaque host handle.

## Scientific metadata

Values may carry unit, dimension, precision, uncertainty, provenance, evidence status, timestamp and source identifier. Metadata is preserved through operations where mathematically meaningful.

## Host boundary

Native SLeeLa objects are represented by opaque handles or immutable snapshots. Native pointers are never direct script values.

## Errors

Type mismatch, dimensional mismatch, unknown state name, permission violation, queue-schema violation and expired context are distinct errors.
