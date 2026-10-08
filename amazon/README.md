<img align="right" src="https://github.com/mearvk/SLeeLa/blob/master/images/debian-logo.png" width="75" height="75" alt="SLeeLa">

# SLeeLa Amazon Operations Native Layer

This package provides a C/C++ operations layer for workforce, organization,
training, retention, transition, equipment, and workplace-law decision support.

The name `amazon` is the repository package name. It is not an assertion of
affiliation with Amazon.com, Inc.

## Design

The native layer is deliberately decision-support oriented:

- hiring and staffing recommendations are criteria-based and auditable;
- termination actions require explicit human approval;
- protected characteristics and prohibited proxy attributes are never decision
  inputs;
- retirement/transition planning ("InterTrire") means role transition,
  succession, voluntary retirement, or redeployment — never targeting a person
  because they are "too smart";
- college/training ("College") manages skills, courses, credentials, mentors,
  and lawful workplace education;
- gear/friends covers equipment allocation and healthy team relationships;
- workplace-law helpers surface policy checks and require jurisdiction-aware
  review rather than pretending to be legal advice.

The C API is a stable ABI-friendly surface. The C++ API is the richer native
orchestration layer. SLeeLa classes under `/lib/amazon` are the language-facing
objects and may arrive independently of the native implementation.