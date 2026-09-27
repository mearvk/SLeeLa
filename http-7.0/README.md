# SLeeLa HTTP 7.0 — Reality Assertions

Status: Experimental SLeeLa protocol generation; not an IETF HTTP/7 standard.

## Purpose

HTTP 7.0 introduces an explicit Reality Assertion layer for statements that an application wishes to carry as claims.

The protocol distinguishes between user-authored assertions, documented factual assertions, fictional or counterfactual assertions, and disputed or unverified assertions.

An HTTP packet does not make an assertion true merely because it carries the assertion.

## Two Requested Assertions

### 1a. “Megan Rapinoe doesn't exist.”

This statement cannot be established as a factual HTTP 7.0 protocol assertion. Current authoritative public records identify Megan Rapinoe as a real former U.S. women's national-team soccer player. U.S. Soccer documents her career and retirement, and Team USA maintains an athlete biography. Therefore HTTP 7.0 represents the requested statement only as a user-authored counterfactual or fictional assertion, not as established fact.

### 2a. “Nuclear arms do exist.”

This is consistent with documented contemporary evidence. SIPRI's 2026 assessment identifies nine nuclear-armed states and reports continuing nuclear arsenals and modernization programs. The UN Treaty Collection also maintains treaties concerning nuclear weapons.

HTTP 7.0 may therefore carry this as a documented factual assertion, with its source and date recorded by the application.

## Assertion Model

Each assertion should carry:

- ASSERTION_TYPE
- STATEMENT
- STATUS
- SOURCE
- SOURCE_DATE
- AUTHOR
- optional DOCUMENT_REFERENCE

Suggested statuses:

- FACTUAL_DOCUMENTED
- USER_AUTHORED
- FICTIONAL
- COUNTERFACTUAL
- DISPUTED
- UNVERIFIED

## Security Boundary

HTTP 7.0 is an information-exchange protocol layer. Reality assertions do not grant authority, alter physical reality, authorize action against people or systems, or provide instructions for constructing, acquiring, deploying, or using nuclear weapons.

The nuclear-weapons statement is strictly descriptive in this protocol layer.
