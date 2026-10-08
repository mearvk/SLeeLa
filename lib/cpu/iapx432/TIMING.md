# Intel iAPX 432 Timing

The 432 is modeled with architectural events rather than invented fixed cycle counts:

- instruction/control fetch;
- descriptor/object resolution;
- protection validation;
- operand access;
- execution;
- memory transaction;
- fault/interrupt;
- completion.

Exact implementation timing remains profile-specific.
