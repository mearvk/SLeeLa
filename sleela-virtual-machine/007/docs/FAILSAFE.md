# SLVM/7 Failsafe Rules

1. Never continue after an unverified artifact, policy, checkpoint, or manager dependency.
2. Never promote degraded state to healthy without fresh evidence.
3. Never let a manager grant itself capability.
4. Never treat resolver output as authority.
5. Never restore native OS handles from an untrusted checkpoint.
6. Bound recovery attempts.
7. Preserve evidence before quarantine when safe.
8. Prefer checkpoint-and-throttle for resource pressure.
9. Prefer quarantine for integrity compromise.
10. Revalidate the complete manager set after migration.
