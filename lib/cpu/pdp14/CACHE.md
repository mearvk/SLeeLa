# PDP-14 Cache Policy

A modern CPU cache hierarchy is not part of the baseline PDP-14 model.

- Input sampling and output commits are governed by the configured control-scan semantics.
- Any host-side optimization must preserve deterministic logic evaluation.
- Do not cache external input values across sampling boundaries unless the selected profile explicitly specifies that behavior.
- Output writes must preserve the selected commit ordering.

This is emulator policy, not a claim about the internal electronics of a particular PDP-14 model.