# Philosophy Module

The `module/` directory separates reusable Conversation Mask and Social Listener concerns into three areas:

- `data/`: event records, provenance, units, timestamps, and storage contracts.
- `helper/`: validation, time-decay, indexing, and safe transformation utilities.
- `psychiatry-friend/`: a supportive, non-clinical conversation interface and its safety boundaries.

The existing `../event_tree.h`, `../event_tree.c`, `../social_listener.hpp`, and `../social_listener.cpp` remain the current event-tree implementation. This structure organizes future module work; it does not silently replace or duplicate those APIs.

All data capture is explicit and consent-based. The module does not passively monitor people or media, infer private intent, diagnose conditions, score human worth, or recommend medication.
