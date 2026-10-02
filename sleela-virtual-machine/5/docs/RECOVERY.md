# SLVM/5 Recovery

Recovery is checkpoint based.

A checkpoint is accepted only when its integrity, transaction context, policy identity, artifact identity, and resource constraints remain valid.

Recovery states are explicit:

OPEN -> CHECKPOINTED -> RECOVERING -> RESUMED

or

OPEN -> CHECKPOINTED -> ABORTED

Recovery must not silently expand capabilities or bypass certificate, isolation, or resolver policy.
