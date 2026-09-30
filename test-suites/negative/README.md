<img src="https://github.com/mearvk/SLeeLa/blob/master/images/sleela-logo-004.jpg" alt="SLeeLa">

<img align="right" src="https://github.com/mearvk/SLeeLa/blob/master/images/debian-logo.png" width="75" height="75" alt="SLeeLa">

# Negative and Security Tests

Negative tests prove invalid or unsafe inputs are rejected without crashes, hangs, leaks, or partial authorization.

Families:
- malformed packet/envelope;
- invalid annotation syntax and unsafe @next;
- truncated/oversized HTTP;
- invalid method/version/framing;
- invalid generation values;
- resource exhaustion;
- traversal and malformed percent encoding;
- compiler/parser rejection.

Every case has a bounded runtime and explicit expected rejection.