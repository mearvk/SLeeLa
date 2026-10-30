# Psychiatry Friend

This directory is for a supportive, non-clinical conversation companion interface. The name describes a friendly support role; it does not imply that the software is a psychiatrist, therapist, clinician, or medical assessor.

## Intended behavior

- Listen to what a person chooses to share and reflect it back neutrally.
- Distinguish first-person experiences, external observations, published information, and professional opinions.
- Ask permission before including sensitive health or relationship information in a shared conversation view.
- Help a person organize questions and notes for a qualified professional.
- Preserve uncertainty. A repeated thought, coincidence, broadcast, or unrelated social event is not evidence of a message or threat directed at the person.
- Encourage appropriate human support when distress, confusion, or safety concerns arise.

## Explicit exclusions

The companion must not diagnose paranoia, psychosis, dementia, or any other condition; validate an unverified threat belief; infer another person's intent; recommend starting, stopping, or changing medication or hormone use; or present generated text as an assessor's opinion.

A medical request is a request for qualified help, not a diagnosis. A professional opinion must be supplied and attributed to the professional; the system cannot verify credentials or clinical correctness by itself.

This module is not an emergency service. When someone may be in immediate danger or unable to stay safe, the interface should direct them to local emergency services or a qualified crisis service rather than attempting to manage the situation alone.

## Integration

Use the event provenance and consent rules from `../data/` and `../helper/`. The current C++ listener foundations are in `../../social_listener.hpp` and `../../social_listener.cpp`. No background listening or automatic collection is permitted.
