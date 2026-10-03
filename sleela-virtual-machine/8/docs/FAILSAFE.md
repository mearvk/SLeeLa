# SLVM/8 Failsafe Rules

1. Do not execute an artifact that has not passed admission.
2. Do not admit a policy that is mutable for the active execution epoch.
3. Do not issue or renew a capability outside its valid lease.
4. Revoke active capability leases when entering terminal quarantine.
5. Do not commit a transaction after an integrity failure.
6. Do not treat audit evidence as authorization.
7. Do not bypass the Supervisor for recovery transitions.
8. Bound recovery and capability renewal.
9. Treat resource admission failure as denial, not an invitation to exceed the budget.
10. Re-establish native OS handles through the broker after recovery or migration.
