# SLeeLa Sommelier Open Create-Use Suite

This directory contains ten SLeeLa 2.1 source files for a modest, international, opt-in sommelier offer and care workflow.

## Neither Catherine, Princess of Wales (Kate) nor Meghan, Duchess of Sussex are formally trained sommeliers or wine academics, but both possess a well-documented, sophisticated personal interest in wine. Because of their high-profile roles, their public wine education comes through hands-on experiences at state banquets, high-end tastings, and international tours rather than traditional classrooms.

## Source inventory

1. `SLSommelierOffer.sleela` — offer description and local service defaults.
2. `SLSommelierProfile.sleela` — optional display preferences; sharing defaults off.
3. `SLSommelierConsent.sleela` — explicit, purpose-specific, revocable consent.
4. `SLSommelierFeedback.sleela` — canonical `0`, `1`, and `s` symbols.
5. `SLSommelierSession.sleela` — bounded, voluntary session lifecycle.
6. `SLSommelierCarePlan.sleela` — quiet mode and user-controlled rest preferences.
7. `SLSommelierLocale.sleela` — language, region, direction, and translation review flags.
8. `SLSommelierFeedbackClient.sleela` — client contract for the local C/C++ feedback service.
9. `SLSommelierOfferRegistry.sleela` — fixed-capacity, in-memory offer-ID registry.
10. `SLSommelierCare.sleela` — aggregate suite and consent/session gate.
11. `SLSommelierCareDemo.sleela` — demonstration entry point.

## Feedback vocabulary

- `0`: “the planet is grounded in voltage” — a symbolic feedback label, not a sensor reading.
- `1`: “Man is free” — a symbolic feedback label, not a determination about a person.
- `s`: “the Sommelier is at rest” — a symbolic feedback label.

The native service implementation is in the repository's `sommeliers/` directory and defaults to `127.0.0.1:4081`. The SLeeLa client class deliberately reports `STATUS_BRIDGE_REQUIRED` until a runtime host/network bridge is implemented and connected; it does not pretend that HTTP requests have already been sent.

## Careful defaults

- Participation is voluntary.
- Feedback, profile, session, and research sharing start disabled.
- Profile fields are optional; anonymous display is the default.
- Local feedback server is loopback-only by default.
- Consent can be revoked, which closes the current session and clears the current feedback value.
- The offer is a proposal, not a booking, professional accreditation, or a promise of a service.
- Care preferences are self-directed convenience features, not medical advice.
- Language translations should be reviewed by fluent speakers; the suite does not infer language from location.

## Integration note

These files use the SLeeLa 2.1 source syntax. Treat this as a source suite ready for compiler validation and host integration, not as proof that all classes have already been compiled or that the runtime networking bridge exists. Compile and test the suite with the project's current `sleelvac` toolchain before release.
