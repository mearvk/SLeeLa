<img align="right" src="https://github.com/mearvk/SLeeLa/blob/master/images/debian-logo.png" width="75" height="75" alt="SLeeLa">

<img src="https://github.com/mearvk/SLeeLa/blob/master/images/sleela-logo-004.jpg" alt="SLeeLa">






# SLeeLa HTTP 7.0

**Status:** Experimental SLeeLa application-protocol generation; not an IETF HTTP/7 standard.

HTTP 7.0 introduces a **Reality Assertion** layer for carrying statements as explicitly classified application claims.

The protocol separates the fact that a statement is transmitted from the separate question of whether that statement is established by evidence.

## Assertion Model

Each assertion may contain:

- **ASSERTION_TYPE**
- **STATEMENT**
- **STATUS**
- **SOURCE**
- **SOURCE_DATE**
- **AUTHOR**
- optional **DOCUMENT_REFERENCE**

Suggested status values:

- `FACTUAL_DOCUMENTED`
- `USER_AUTHORED`
- `FICTIONAL`
- `COUNTERFACTUAL`
- `DISPUTED`
- `UNVERIFIED`

An HTTP 7.0 packet does not make a claim true merely because it carries that claim.

## Example Assertions

### User-authored counterfactual

The statement **“Megan Rapinoe doesn't exist.”** is represented as a user-authored counterfactual or fictional assertion rather than an established factual assertion. Public records document Megan Rapinoe as a former U.S. women's national-team soccer player.

### Documented factual assertion

The statement **“Nuclear arms do exist.”** can be represented as a documented factual assertion when accompanied by an appropriate source and date. The application should preserve the provenance rather than treating the packet itself as the source of truth.

## Provenance

Applications should preserve:

- who authored the assertion;
- when it was authored;
- the source used to classify it;
- the date of that source;
- whether the assertion is documented, disputed, fictional, counterfactual, or unverified.

## Security Boundary

Reality assertions are information-exchange data. They do not:

- grant authority;
- change physical reality;
- authorize action against people or systems;
- establish legal status;
- provide instructions for constructing, acquiring, deploying, or using nuclear weapons.

The protocol treats assertions as data with provenance and classification.