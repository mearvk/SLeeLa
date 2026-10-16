# SLVM/9 Failsafe Rules

1. Never execute filesystem operations before SLVM/8 admission succeeds.
2. Never treat a successful API call as proof of durable persistence.
3. Never assume an inode/path model for a filesystem that advertises different identity semantics.
4. Reject unsupported filesystem features instead of guessing.
5. Reject stale filesystem generations and rediscover the target.
6. Invalidate native handles across recovery, migration, and confirmed mount replacement.
7. Never let a filesystem adapter grant SLeeLa capabilities.
8. Do not expose administrative filesystem state as user data unless the adapter declares it as such.
9. Preserve filesystem-specific recovery metadata through the broker boundary.
10. If integrity or generation validation fails, degrade, recover, or quarantine according to the SLVM supervisor policy.
