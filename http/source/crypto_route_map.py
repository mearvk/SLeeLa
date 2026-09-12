"""HTTP 3.0 Cryptographic Route Map reference model.

This module models route-aware cryptographic policy without performing packet
interception or exposing sensitive identity data. It is intentionally a policy
layer: QUIC/HTTP/3 packet processing remains the responsibility of the actual
transport implementation.
"""

from __future__ import annotations

from dataclasses import dataclass, field
from enum import IntEnum
from typing import Iterable, Sequence


class CryptoGrade(IntEnum):
    """Cryptographic protection policy level."""

    CR1 = 1
    CR2 = 2
    CR3 = 3
    CR4 = 4


@dataclass(frozen=True)
class RouteNode:
    """One logical country/network point in a route."""

    node_id: str
    country_code: str
    technology_grade: int = 0
    socialism_standard: str = "S-N/A"
    measured_risk: int = 0
    emergency_capsule_allowed: bool = False

    def __post_init__(self) -> None:
        if not self.node_id:
            raise ValueError("node_id must not be empty")
        if not (0 <= self.technology_grade <= 4):
            raise ValueError("technology_grade must be between 0 and 4")
        if not (0 <= self.measured_risk <= 4):
            raise ValueError("measured_risk must be between 0 and 4")


@dataclass(frozen=True)
class RoutePolicy:
    """Policy inputs used to select cryptographic protection."""

    source_grade: CryptoGrade = CryptoGrade.CR2
    destination_grade: CryptoGrade = CryptoGrade.CR2
    minimum_grade: CryptoGrade = CryptoGrade.CR2
    hop_threshold: int = 4
    strengthen_after_threshold: bool = True
    allow_jurisdictional_capsules: bool = True

    def __post_init__(self) -> None:
        if self.hop_threshold < 0:
            raise ValueError("hop_threshold must not be negative")


@dataclass(frozen=True)
class RouteDecision:
    """Result of evaluating a route."""

    grade: CryptoGrade
    hop_count: int
    countries: tuple[str, ...]
    capsule_countries: tuple[str, ...] = field(default_factory=tuple)
    reason: str = ""


class CryptographicRouteMap:
    """Calculate a monotonic cryptographic policy for a route.

    The policy never uses a country's political classification as a direct
    security score. Country metadata may inform external governance policy,
    while measured network conditions and explicit minimums control protection.
    """

    def __init__(self, policy: RoutePolicy | None = None) -> None:
        self.policy = policy or RoutePolicy()

    @staticmethod
    def _grade_for_risk(risk: int) -> CryptoGrade:
        return CryptoGrade(max(1, min(4, risk + 1)))

    def evaluate(self, nodes: Sequence[RouteNode]) -> RouteDecision:
        if not nodes:
            raise ValueError("route must contain at least one node")

        hop_count = max(0, len(nodes) - 1)
        risk_grade = max(
            (self._grade_for_risk(node.measured_risk) for node in nodes),
            default=CryptoGrade.CR1,
        )
        grade = max(
            self.policy.minimum_grade,
            self.policy.source_grade,
            self.policy.destination_grade,
            risk_grade,
        )

        if self.policy.strengthen_after_threshold and hop_count > self.policy.hop_threshold:
            grade = max(grade, CryptoGrade.CR3)

        capsule_countries: list[str] = []
        if self.policy.allow_jurisdictional_capsules:
            for node in nodes:
                if node.emergency_capsule_allowed and node.country_code not in capsule_countries:
                    capsule_countries.append(node.country_code)

        return RouteDecision(
            grade=CryptoGrade(grade),
            hop_count=hop_count,
            countries=tuple(node.country_code for node in nodes),
            capsule_countries=tuple(capsule_countries),
            reason=(
                "Protection is monotonic: route conditions may strengthen the "
                "selected grade but do not silently downgrade it."
            ),
        )

    def evaluate_country_path(self, countries: Iterable[str]) -> RouteDecision:
        """Convenience evaluation for a path where only country order is known."""
        nodes = [
            RouteNode(node_id=f"country-{index}", country_code=country)
            for index, country in enumerate(countries)
        ]
        return self.evaluate(nodes)
