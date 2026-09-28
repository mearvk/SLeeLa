# SLeeLa HTTP Annotation Forwarding

## Uniform HTTP model
All SLeeLa HTTP generations use the same architectural forwarding vocabulary:

**Holding Document → Forwarding Annotation → Nexter Colony**

The HTTP generation is then selected as an explicit protocol/transport boundary.

## Generation map
1.0 → http-1.0/
2.0 → http-2.0/
2.1 → http-2.0/ negotiation and pipeline family
3.0 → http-3.0/
4.0 → http-4.0/
5.0 → http-5.0/
6.0 → http-6.0/
7.0 → http-7.0/
8.0 → http-8.0/
9.0 → http-9.0/

HTTP 4.0–9.0 are SLeeLa project protocols, not claims of official Internet standardization.

## Rule
The forwarding layer is uniform. Generation-specific semantics remain inside their generation adapter. This prevents annotations from becoming hidden protocol logic.

Max Rupplin — MEARVK LLC — 2026
