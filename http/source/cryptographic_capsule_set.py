"""Horizontal jurisdictional isolation for HTTP 3.0 identity capsules."""

from __future__ import annotations

from dataclasses import dataclass
from typing import Any, Mapping

from crypto_identity_capsule import CapsuleMetadata, IdentityCapsule, new_capsule_id


@dataclass(frozen=True)
class JurisdictionRecipient:
    jurisdiction_id: str
    public_key: Any
    emergency_endpoint_reference: str


class CryptographicCapsuleSet:
    """Build independently encrypted capsules for selected jurisdictions.

    Every capsule is encrypted to a different recipient public key. There is
    deliberately no shared master key and no operation that combines keys
    across jurisdictions.
    """

    def __init__(self, recipients: list[JurisdictionRecipient]) -> None:
        self._recipients = {r.jurisdiction_id: r for r in recipients}
        if len(self._recipients) != len(recipients):
            raise ValueError("jurisdiction IDs must be unique")

    def create_for_sender(
        self,
        sender_reference: str,
        jurisdictions: list[str],
        *,
        expiration: int,
        authorized_contact_reference: str | None = None,
        identity_registry_reference: str | None = None,
        flow_context: str | None = None,
        attributes: Mapping[str, Any] | None = None,
    ) -> dict[str, dict[str, Any]]:
        """Create one independently encrypted capsule per jurisdiction."""
        result: dict[str, dict[str, Any]] = {}
        for jurisdiction in jurisdictions:
            recipient = self._recipients.get(jurisdiction)
            if recipient is None:
                raise KeyError(f"no recipient configured for {jurisdiction}")

            metadata = CapsuleMetadata(
                capsule_id=new_capsule_id(),
                jurisdiction_id=jurisdiction,
                expiration=expiration,
                emergency_endpoint_reference=recipient.emergency_endpoint_reference,
                sender_reference=sender_reference,
                authorized_contact_reference=authorized_contact_reference,
                identity_registry_reference=identity_registry_reference,
                flow_context=flow_context,
            )
            result[jurisdiction] = IdentityCapsule.create(
                metadata,
                recipient.public_key,
                extra_attributes=attributes,
            )
        return result

    @staticmethod
    def authorized_payload(
        capsules: Mapping[str, Mapping[str, Any]],
        jurisdiction_id: str,
    ) -> Mapping[str, Any]:
        """Return only the selected jurisdiction's capsule.

        This function does not decrypt anything. It is a routing primitive that
        prevents a caller from accidentally treating all capsules as one shared
        authorization domain.
        """
        try:
            return capsules[jurisdiction_id]
        except KeyError as exc:
            raise PermissionError(
                f"no capsule is authorized for jurisdiction {jurisdiction_id}"
            ) from exc
