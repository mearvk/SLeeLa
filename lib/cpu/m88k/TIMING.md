# Motorola 88000 Timing

Tracked timing events:

- instruction fetch;
- decode;
- register access;
- issue/scoreboard;
- integer execution;
- floating point;
- branch;
- load/store;
- cache/TLB;
- exception;
- retirement.

The MC88110 provides a more aggressive superscalar implementation than the MC88100. The generic SLeeLa model therefore leaves exact pipeline depth and issue width to the selected processor profile.
