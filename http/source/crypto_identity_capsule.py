"""Cryptographic Identity Capsule (CIC) reference implementation.

Dependencies:
    pip install cryptography

The implementation deliberately stores references to sensitive identity
records instead of raw National ID, telephone, or biometric/DNA data.

This is a reference component, not a production HTTP/3 implementation. The
transport binding and jurisdictional authorization model must be reviewed
before deployment.
"""

from __future__ import annotations

import base64
import json
import os
import secrets
import time
from dataclasses import dataclass
from typing import Any, Mapping

from cryptography.hazmat.primitives import hashes, serialization
from cryptography.hazmat.primitives.asymmetric import x25519
from cryptography.hazmat.primitives.ciphers.aead import AESGCM
from cryptography.hazmat.primitives.kdf.hkdf import HKDF


VERSION = 1
NONCE_SIZE = 12
KEY_SIZE = 32
CAPSULE_ID_SIZE = 16


def _b64(value: bytes) -> str:
    return base64.urlsafe_b64encode(value).decode("ascii").rstrip("=")


def _unb64(value: str) -> bytes:
    return base64.urlsafe_b64decode(value + "=" * (-len(value) % 4))


def _canonical_json(value: Mapping[str, Any]) -> bytes:
    return json.dumps(value, sort_keys=True, separators=(",", ":")).encode("utf-8")


@dataclass(frozen=True)
class CapsuleMetadata:
    capsule_id: str
    jurisdiction_id: str
    expiration: int
    emergency_endpoint_reference: str
    sender_reference: str
    authorized_contact_reference: str | None = None
    identity_registry_reference: str | None = None
    flow_context: str | None = None


class IdentityCapsule:
    """Create and open a jurisdiction-specific encrypted identity capsule."""

    @staticmethod
    def generate_recipient_keypair() -> tuple[x25519.X25519PrivateKey, x25519.X25519PublicKey]:
        private = x25519.X25519PrivateKey.generate()
        return private, private.public_key()

    @staticmethod
    def public_key_bytes(public_key: x25519.X25519PublicKey) -> bytes:
        return public_key.public_bytes(
            serialization.Encoding.Raw,
            serialization.PublicFormat.Raw,
        )

    @staticmethod
    def _derive_key(shared_secret: bytes, capsule_id: bytes) -> bytes:
        return HKDF(
            algorithm=hashes.SHA256(),
            length=KEY_SIZE,
            salt=capsule_id,
            info=b"SLeeLa/HTTP3/CIC/v1",
        ).derive(shared_secret)

    @classmethod
    def create(
        cls,
        metadata: CapsuleMetadata,
        recipient_public_key: x25519.X25519PublicKey,
        *,
        extra_attributes: Mapping[str, Any] | None = None,
    ) -> dict[str, Any]:
        """Create a capsule encrypted only for the supplied recipient key.

        ``extra_attributes`` should contain only the minimum information needed
        for an authorized emergency purpose. Raw identity documents and raw
        biometric/DNA data are intentionally rejected by convention.
        """
        if metadata.expiration <= int(time.time()):
            raise ValueError("capsule expiration must be in the future")
        if not metadata.jurisdiction_id:
            raise ValueError("jurisdiction_id must not be empty")
        if not metadata.sender_reference:
            raise ValueError("sender_reference must not be empty")

        capsule_id = _unb64(metadata.capsule_id)
        if len(capsule_id) != CAPSULE_ID_SIZE:
            raise ValueError("capsule_id must encode exactly 128 bits")

        forbidden = {
            "national_id",
            "national_id_number",
            "phone_number",
            "telephone_number",
            "dna_id",
            "biometric_id",
            "raw_dna",
        }
        supplied = set((extra_attributes or {}).keys())
        forbidden_supplied = supplied & forbidden
        if forbidden_supplied:
            raise ValueError(
                "raw sensitive identifiers are not packet fields: "
                + ", ".join(sorted(forbidden_supplied))
            )

        ephemeral_private = x25519.X25519PrivateKey.generate()
        ephemeral_public = ephemeral_private.public_key()
        shared = ephemeral_private.exchange(recipient_public_key)
        key = cls._derive_key(shared, capsule_id)

        payload: dict[str, Any] = {
            "version": VERSION,
            "capsule_id": metadata.capsule_id,
            "jurisdiction_id": metadata.jurisdiction_id,
            "expiration": metadata.expiration,
            "emergency_endpoint_reference": metadata.emergency_endpoint_reference,
            "sender_reference": metadata.sender_reference,
            "authorized_contact_reference": metadata.authorized_contact_reference,
            "identity_registry_reference": metadata.identity_registry_reference,
            "flow_context": metadata.flow_context,
            "attributes": dict(extra_attributes or {}),
        }

        plaintext = _canonical_json(payload)
        nonce = os.urandom(NONCE_SIZE)
        aad = _canonical_json(
            {
                "version": VERSION,
                "capsule_id": metadata.capsule_id,
                "jurisdiction_id": metadata.jurisdiction_id,
            }
        )
        ciphertext = AESGCM(key).encrypt(nonce, plaintext, aad)

        return {
            "version": VERSION,
            "capsule_id": metadata.capsule_id,
            "jurisdiction_id": metadata.jurisdiction_id,
            "ephemeral_public_key": _b64(cls.public_key_bytes(ephemeral_public)),
            "nonce": _b64(nonce),
            "ciphertext": _b64(ciphertext),
        }

    @classmethod
    def open(
        cls,
        capsule: Mapping[str, Any],
        recipient_private_key: x25519.X25519PrivateKey,
    ) -> dict[str, Any]:
        """Decrypt a capsule with the corresponding recipient private key."""
        if int(capsule.get("version", 0)) != VERSION:
            raise ValueError("unsupported capsule version")

        capsule_id = _unb64(str(capsule["capsule_id"]))
        if len(capsule_id) != CAPSULE_ID_SIZE:
            raise ValueError("invalid capsule ID")

        ephemeral_public = x25519.X25519PublicKey.from_public_bytes(
            _unb64(str(capsule["ephemeral_public_key"]))
        )
        shared = recipient_private_key.exchange(ephemeral_public)
        key = cls._derive_key(shared, capsule_id)
        nonce = _unb64(str(capsule["nonce"]))
        aad = _canonical_json(
            {
                "version": VERSION,
                "capsule_id": capsule["capsule_id"],
                "jurisdiction_id": capsule["jurisdiction_id"],
            }
        )
        plaintext = AESGCM(key).decrypt(nonce, _unb64(str(capsule["ciphertext"])), aad)
        payload = json.loads(plaintext.decode("utf-8"))

        if payload.get("expiration", 0) <= int(time.time()):
            raise ValueError("capsule has expired")
        if payload.get("jurisdiction_id") != capsule.get("jurisdiction_id"):
            raise ValueError("jurisdiction binding failed")

        return payload


def new_capsule_id() -> str:
    """Generate a 128-bit public capsule identifier."""
    return _b64(secrets.token_bytes(CAPSULE_ID_SIZE))
