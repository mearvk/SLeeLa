"""Tests for the HTTP 3.0 cryptographic reference components."""

import time

import pytest

from crypto_identity_capsule import CapsuleMetadata, IdentityCapsule, new_capsule_id
from crypto_route_map import CryptoGrade, CryptographicRouteMap, RouteNode, RoutePolicy
from cryptographic_capsule_set import CryptographicCapsuleSet, JurisdictionRecipient


def test_capsule_round_trip_and_sensitive_field_rejection() -> None:
    private, public = IdentityCapsule.generate_recipient_keypair()
    metadata = CapsuleMetadata(
        capsule_id=new_capsule_id(),
        jurisdiction_id="US",
        expiration=int(time.time()) + 3600,
        emergency_endpoint_reference="emergency.example/US",
        sender_reference="sender-ref-001",
    )

    capsule = IdentityCapsule.create(
        metadata,
        public,
        extra_attributes={"emergency_class": "critical"},
    )
    opened = IdentityCapsule.open(capsule, private)
    assert opened["sender_reference"] == "sender-ref-001"
    assert opened["jurisdiction_id"] == "US"

    with pytest.raises(ValueError):
        IdentityCapsule.create(
            metadata,
            public,
            extra_attributes={"national_id": "MUST-NOT-BE-A-PACKET-FIELD"},
        )


def test_jurisdictions_are_independently_encrypted() -> None:
    private_a, public_a = IdentityCapsule.generate_recipient_keypair()
    private_b, public_b = IdentityCapsule.generate_recipient_keypair()
    recipients = CryptographicCapsuleSet(
        [
            JurisdictionRecipient("A", public_a, "emergency.example/A"),
            JurisdictionRecipient("B", public_b, "emergency.example/B"),
        ]
    )
    capsules = recipients.create_for_sender(
        "sender-ref-002",
        ["A", "B"],
        expiration=int(time.time()) + 3600,
    )

    assert capsules["A"]["ciphertext"] != capsules["B"]["ciphertext"]
    opened_a = IdentityCapsule.open(capsules["A"], private_a)
    opened_b = IdentityCapsule.open(capsules["B"], private_b)
    assert opened_a["jurisdiction_id"] == "A"
    assert opened_b["jurisdiction_id"] == "B"

    with pytest.raises(Exception):
        IdentityCapsule.open(capsules["A"], private_b)


def test_route_policy_is_monotonic() -> None:
    route = CryptographicRouteMap(
        RoutePolicy(
            source_grade=CryptoGrade.CR2,
            destination_grade=CryptoGrade.CR2,
            minimum_grade=CryptoGrade.CR2,
            hop_threshold=2,
        )
    )
    decision = route.evaluate(
        [
            RouteNode("r0", "US", measured_risk=1),
            RouteNode("r1", "CA", measured_risk=2),
            RouteNode("r2", "DE", measured_risk=1),
            RouteNode("r3", "IS", measured_risk=0),
        ]
    )
    assert decision.hop_count == 3
    assert decision.grade >= CryptoGrade.CR3
