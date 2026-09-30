# Java Qualification Manifest

The qualification manifest coordinates the Java/SLeeLa source/API congruence layers as one deterministic system.

Layers: source equivalence; expression equivalence; normalized declaration/signature comparison; negative/constraint corpus; definite assignment/reachability; checked exceptions; Java API dependency closure; platform/reproducibility observation.

PASS means the invoked child process returned zero. FAIL means non-zero. The aggregate result is PASS only when every configured layer passes. Platform observation does not assert that remote operating systems were executed. Java SE 27 is the released specification baseline; Java 28 remains a forward compatibility target. No JVM, bytecode, SLVM, or runtime equivalence is implied.

Run: python3 tests/java_qualification_manifest.py --json qualification.json


## 0.3.21 Development Upgrade — Java Flow Qualification Closure

This upgrade makes Java control-flow qualification a first-class closure task. Qualification now requires a source fixture, normalized representation, semantic rule, positive or negative result, and unified-manifest evidence.

Required next coverage: boolean-path-sensitive `&&`, `||`, `!`, and `?:`; constant boolean expressions; break/continue joins and labels; while/do/for completion; switch completion; try/catch/finally abrupt paths; constructor and blank-final definite-unassignment; lambda capture boundaries; and checked-exception propagation.

The work remains source/API qualification and does not imply JVM, bytecode, SLVM, or runtime equivalence.
