# AE6E66 Deployment Standard

AE6E66 is deployed as an application module. Host-level services are configured separately.

## Default posture

- **No automatic package installation**
- **No automatic service enablement**
- **No automatic firewall changes**
- **No automatic DNS changes**
- **No automatic network binding**
- **No automatic credential creation in Git-tracked paths**
- **Dry-run and preflight are the normal first steps**
- **Integrity failure stops execution**

## Deployment sequence

1. Obtain a reviewed, pinned repository commit.
2. Run `scripts/verify-integrity.sh` against the approved manifest.
3. Run the module preflight checks.
4. Supply deployment-specific values using the example configuration as a template.
5. Configure the database using an administrator-managed secret store.
6. Configure an already-approved MTA independently of AE6E66.
7. Run collection in dry-run mode.
8. Review the proposed records and message recipients.
9. Enable delivery explicitly, with a documented rate limit.
10. Monitor logs and verify the integrity manifest after changes.

## Mail architecture

The preferred architecture is:

`AE6E66 -> local submission interface -> administrator-managed MTA -> recipient MX`

AE6E66 should not expose SMTP directly to the Internet. It should not modify the MTA's public listener configuration.

## Database architecture

Use a local database endpoint where possible. The service account should have only the permissions required by the application. Schema administration belongs to a separate administrator account.

## Runtime directories

Keep mutable data outside the Git checkout when practical:

- state
- logs
- portraits
- downloaded documents
- sent-message archives
- database credentials
- cryptographic keys

This prevents accidental commits and makes backup/retention controls easier.

## Platform support

The repository may contain platform-specific helpers, but platform-specific installation is not considered portable merely because a script exists. Each platform requires an actual tested implementation and documented prerequisite set.

Until a platform is tested, it is documented as **unverified**, not production-ready.
