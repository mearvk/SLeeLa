# Sleelavirin™ Heuristics

Heuristics are evidence-based rules that identify suspicious characteristics without requiring an exact known signature.

## Reference categories

- executable structure anomalies;
- unexpected interpreter or script behavior;
- suspicious process relationships;
- abnormal persistence requests;
- unexpected privilege transitions;
- unusual network or download behavior;
- archive/container anomalies;
- suspicious macro or document behavior;
- obfuscation indicators;
- impossible or inconsistent metadata;
- combinations of otherwise weak indicators.

Microsoft documents real-time protection as combining content scanning with behavior monitoring and heuristics. Sleelavirin should therefore model heuristic findings as evidence rather than treating one heuristic signal as universal proof of malware. https://learn.microsoft.com/en-us/defender-endpoint/configure-real-time-protection-microsoft-defender-antivirus

Every heuristic should document: identifier, version, rationale, input, evidence, confidence, false-positive considerations, and intended policy response.

## Trap-2 operator profile

Sleelavirin also provides an explicit Trap-2 operator-profile convention. It scores declared evidence and reaches Trap-2 only at the operator-declared threshold. A Trap-2 result is evidence, not an automatic malware verdict. Automated action requires explicit operator acknowledgement plus declared basis and license/profile context. See TRAP2.md and the C implementation under antivirus/src.
