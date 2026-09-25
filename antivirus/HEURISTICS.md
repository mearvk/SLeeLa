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
