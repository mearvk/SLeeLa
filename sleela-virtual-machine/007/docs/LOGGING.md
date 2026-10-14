# SLVM/7 Log Manager

The Log Manager is part of the VM control plane.

Security, recovery, checkpoint, manager-health, integrity, and quarantine events receive ordered records. Records are chained so an omitted or altered record can be detected.

The manager must:

- redact secrets;
- preserve security and recovery evidence;
- align recovery markers with checkpoints;
- expose sink health;
- detect dropped required events;
- distinguish informational loss from security-evidence loss.

If required security evidence cannot be retained or verified, policy may transition the VM to checkpointing or quarantine. Logging never grants authority to the logger.
