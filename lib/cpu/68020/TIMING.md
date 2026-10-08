# Motorola 68020 Timing

Tracked timing events:

- instruction prefetch;
- cache lookup;
- decode;
- effective-address calculation;
- integer execution;
- memory cycle;
- coprocessor cycle;
- exception;
- interrupt;
- completion.

The 68020 is represented as a pipelined CISC processor, but SLeeLa does not invent a superscalar issue width or modern retirement structure.
