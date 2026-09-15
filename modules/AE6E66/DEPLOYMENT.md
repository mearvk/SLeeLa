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
- **Historical contact data is non-operational until provenance/freshness is revalidated**

## Deployment sequence

1. Obtain a reviewed, pinned repository commit or release.
2. Verify the approved SHA-256 manifest before build, execution, crawl, diagnostics, and message preparation.
3. Independently authenticate the manifest/release provenance; a digest of an untrusted manifest is insufficient.
4. Run the module preflight checks.
5. Supply deployment-specific values using the example configuration as a template.
6. Configure the database using an administrator-managed secret store and verify local scope, minimum grants, and TLS requirements.
7. Configure an already-approved MTA independently of AE6E66 and pass the MTA/DKIM preflights.
8. Run collection in dry-run mode.
9. Validate records against `contact-schema.json` and review provenance/freshness.
10. Review proposed recipients and message content.
11. Enable delivery explicitly, with a documented rate limit.
12. Monitor logs and verify the integrity manifest after changes.

## Contact-data release gate

The checked-in `contacts.csv` is a controlled historical/reference dataset. Before operational use, records must be revalidated against an authoritative current source and assigned current `source` and `retrieved_at` values. `legacy-unverified` and `legacy-public` are not equivalent to current verification and must not silently become delivery targets.

## Mail architecture

The preferred architecture is:

`AE6E66 -> local submission interface -> administrator-managed MTA -> recipient MX`

AE6E66 should not expose SMTP directly to the Internet. It should not modify the MTA's public listener configuration.

## Database architecture

Use a local database endpoint where possible. The service account should have only the permissions required by the application. Schema administration belongs to a separate administrator account. Verify actual grants rather than trusting configuration text alone.

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

Until a platform is tested, it is documented as **unverified**, not production-ready. See `TEST-PLAN.md`.
