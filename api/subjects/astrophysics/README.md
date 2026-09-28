# Astrophysics Subject Library

Astrophysics is a SLeeLa subject library built on Mathematics and Physics.

## Native scope

The first tested vocabulary covers luminosity/flux, wavelength/redshift, Planck spectral radiance, Wien displacement, Kepler orbital period, gravitational parameter, escape velocity, angular size and distance modulus.

C is the numerical ABI in astrophysics_math.c. C++ provides the frontend-facing subject class and typed facade.

## Units and observations

Quantities use declared units; SI-oriented units are the default contract. Astronomical XML may use VOUnits-style strings. IVOA's Units recommendation defines a machine-readable syntax for unit strings. citeturn0search6

A measured observation contains id, subject, target, timestamp, quantity, value, unit, instrument/source and provenance. The native store appends escaped XML records. For larger tables, the XML layer can interoperate with VOTable-style tabular data. IVOA VOTable 1.5 is a current XML interchange recommendation, and the IVOA Provenance Data Model provides a provenance model for astronomical datasets and processing activities. citeturn2search5turn2search2

## Model/procedure separation

An XML model defines quantities, constants and formulas. An XML procedure defines ordered allow-listed operations. An observation is data, not executable code, and its existence never implicitly runs a procedure.

New astrophysics source should enter only after units, valid ranges, numerical behavior and tests are documented.
