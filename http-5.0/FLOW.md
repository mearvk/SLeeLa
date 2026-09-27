# SLeeLa HTTP 5.0 Flow

1. Carrier accepts authenticated bytes.
2. HTTP 5.0 validates frame header and payload bounds.
3. Session validates capabilities and stream state.
4. Sequence state checks ordering and replay policy.
5. OPEN establishes the logical request.
6. FRIENDS_PACK establishes optional pack metadata.
7. BONUS_OFFER advertises optional references.
8. The receiver evaluates each offer under local policy.
9. FP_UPDATE changes the application-level FP balance.
10. DATA carries ordinary application information.
11. WINDOW communicates receive capacity.
12. RESUME continues an accepted transfer.
13. END closes a successful exchange.
14. RESET closes an exchange with an explicit error condition.
15. AUDIT records defensive conformance events.

Zero FP is a normal application state. It declines new optional bonuses without disabling ordinary communication.

Router interoperability is limited to authorized conformance and lab testing. No protocol feature is used to alter, disable, flood, sting, or otherwise interfere with routers.
