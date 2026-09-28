"""bonding_collar.py -- SLeeLa's bonding collar for conferring with an OS
capability layer (e.g. a TAC3-class filesystem/OS) across new or unknown hosts.

PURPOSE
    When a SLeeLa executable lands on a *new or otherwise unknown operating
    system*, it needs to answer one question safely: "what advantage, if any,
    does this host actually confer -- and can I trust it enough to use it?"
    The bonding collar is that careful negotiator. It:

      1. PROBES the host for a capability responder (never assumes one exists);
      2. CONFERS over a small, MAC-backed, tiered handshake (mirrors the H3
         capability handshake in http3_flow.py);
      3. BONDS only up to the highest tier BOTH sides prove they support, under
         a shared secret;
      4. FAILS CLOSED to a safe baseline on any unknown/unattested host -- it
         never assumes a capability it has not verified.

PROTECTING OUR ADVANTAGE (design contract)
    This is the SLeeLa *client* side only. It deliberately embeds NO proprietary
    OS/filesystem internals: no on-disk format, no kernel structures, no
    TAC3-specific field layouts, no magic numbers from the host implementation.
    It negotiates by neutral capability *names* and an opaque attestation tag.
    Consequences:
      - On a trusted, attested host the collar unlocks the advantage tiers.
      - On a new/unknown/hostile host it reveals nothing proprietary and drops
        to the baseline (NONE), so our advantage is never leaked by probing.
    The collar tells SLeeLa *whether* an advantage is present and *which tier*
    to use; it never encodes *how* the host provides it.

RELATION TO TAC3
    TAC3 (the operator's own OS filesystem work) is one possible responder that
    can answer this handshake. The collar does not depend on TAC3 and contains
    none of its internals; TAC3 is simply a host that MAY attest to these
    capability names. Any conforming responder works.
"""
from __future__ import annotations

import os
import struct
from dataclasses import dataclass
from typing import Optional

# Reuse SLeeLa's audited keyed MAC so attestations are unforgeable without the
# shared secret (identical primitive to the H3 handshake).
from http3_flow import siphash24, MAC_KEY_BYTES

# Domain-separation tag for this handshake (distinct from H3's "H3CP").
_COLLAR_TAG = b"SLBC"          # "SLeeLa Bonding Collar"
COLLAR_PROTOCOL = 1            # collar handshake generation


# --------------------------------------------------------------------------
# Capability tiers -- negotiated by NAME, not by any host-internal layout.
# Cumulative: a higher tier implies the ones below it. These describe *what
# advantage* a host confers, never *how* it implements it.
# --------------------------------------------------------------------------
CAP_NONE       = 0x00          # no attested advantage (safe baseline / unknown OS)
CAP_T1_PRESENT = 0x01          # a capability responder is present and attested
CAP_T2_STORAGE = 0x02          # durable/redundant storage advantage available
CAP_T3_CONTEXT = 0x04          # contextual-identity / health metadata available
CAP_T4_RECOVERY= 0x08          # protected boot/recovery + integrity attestation

CAP_BASELINE = CAP_T1_PRESENT
CAP_ALL = CAP_T1_PRESENT | CAP_T2_STORAGE | CAP_T3_CONTEXT | CAP_T4_RECOVERY

_TIER_NAMES = {
    CAP_NONE:        "NONE",
    CAP_T1_PRESENT:  "T1_PRESENT",
    CAP_T2_STORAGE:  "T2_STORAGE",
    CAP_T3_CONTEXT:  "T3_CONTEXT",
    CAP_T4_RECOVERY: "T4_RECOVERY",
}


def tier_name(tier: int) -> str:
    return _TIER_NAMES.get(tier, "MIXED")


def _attest(caps: int, protocol: int, key: bytes) -> int:
    """Keyed MAC over (tag, protocol, caps). Unforgeable without `key`."""
    msg = _COLLAR_TAG + struct.pack(">II", protocol & 0xFFFFFFFF, caps & 0xFFFFFFFF)
    return siphash24(key, msg)


# --------------------------------------------------------------------------
# Attested capability offer (one side of the confer)
# --------------------------------------------------------------------------
@dataclass
class CollarOffer:
    """A side's advertised OS-advantage capabilities + a MAC attesting to them."""

    capabilities: int
    protocol: int = COLLAR_PROTOCOL
    mac: int = 0

    @staticmethod
    def make(capabilities: int, key: bytes, *, protocol: int = COLLAR_PROTOCOL) -> "CollarOffer":
        # An offer always at least claims baseline presence; the MAC binds it.
        caps = (capabilities | CAP_BASELINE) & 0xFFFFFFFF
        return CollarOffer(capabilities=caps, protocol=protocol,
                           mac=_attest(caps, protocol, key))

    def verify(self, key: bytes) -> bool:
        return _attest(self.capabilities, self.protocol, key) == self.mac


def negotiate(local_caps: int, remote_caps: int) -> int:
    """Highest single tier present in BOTH sides (cumulative, baseline fallback).

    Returns CAP_NONE if either side lacks the baseline -- i.e. no attested
    advantage, so SLeeLa runs in its safe, host-agnostic mode.
    """
    common = local_caps & remote_caps
    if not (common & CAP_BASELINE):
        return CAP_NONE
    if common & CAP_T4_RECOVERY:
        return CAP_T4_RECOVERY
    if common & CAP_T3_CONTEXT:
        return CAP_T3_CONTEXT
    if common & CAP_T2_STORAGE:
        return CAP_T2_STORAGE
    return CAP_T1_PRESENT


# --------------------------------------------------------------------------
# The bonding collar
# --------------------------------------------------------------------------
@dataclass
class BondResult:
    """Outcome of a confer: the agreed tier, whether we bonded, and why."""
    tier: int
    bonded: bool
    reason: str

    @property
    def tier_name(self) -> str:
        return tier_name(self.tier)


class BondingCollar:
    """Confers with a host's capability responder and bonds at a safe tier.

    Usage:
        collar = BondingCollar(local_caps=CAP_ALL, key=shared_secret)
        result = collar.confer(probe_host())   # probe returns a CollarOffer|None
        if result.bonded and result.tier >= CAP_T2_STORAGE:
            ...use the durable-storage advantage...
        else:
            ...run in host-agnostic baseline mode...
    """

    def __init__(self, local_caps: int, key: bytes) -> None:
        if len(key) != MAC_KEY_BYTES:
            raise ValueError(f"key must be {MAC_KEY_BYTES} bytes")
        # We never advertise more than we actually support.
        self.local_caps = (local_caps | CAP_BASELINE) & 0xFFFFFFFF
        self._key = key

    def local_offer(self) -> CollarOffer:
        """Our own attested offer, to send to the host responder."""
        return CollarOffer.make(self.local_caps, self._key)

    def confer(self, remote_offer: Optional[CollarOffer]) -> BondResult:
        """Confer with the host's offer and decide the bonded tier.

        FAIL-CLOSED rules (protecting the advantage on unknown hosts):
          - No responder / no offer            -> NONE, not bonded.
          - Offer MAC fails (untrusted host)   -> NONE, not bonded.
          - Protocol mismatch                  -> NONE, not bonded.
          - Otherwise                          -> highest mutually-attested tier.
        """
        if remote_offer is None:
            return BondResult(CAP_NONE, False, "no capability responder on host (unknown OS)")
        if remote_offer.protocol != COLLAR_PROTOCOL:
            return BondResult(CAP_NONE, False,
                              f"protocol mismatch (host={remote_offer.protocol}, "
                              f"collar={COLLAR_PROTOCOL})")
        if not remote_offer.verify(self._key):
            # A host that cannot attest under the shared secret is untrusted;
            # we disclose nothing and stay at baseline.
            return BondResult(CAP_NONE, False, "host attestation MAC failed (untrusted host)")
        tier = negotiate(self.local_caps, remote_offer.capabilities)
        if tier == CAP_NONE:
            return BondResult(CAP_NONE, False, "no common baseline capability")
        return BondResult(tier, True, f"bonded at {tier_name(tier)}")


# --------------------------------------------------------------------------
# Host probe (deliberately conservative)
# --------------------------------------------------------------------------
def probe_env_responder(key: bytes, *, env: Optional[dict] = None) -> Optional[CollarOffer]:
    """A minimal, safe probe: read a host-provided attestation from the
    environment (SLEELA_OS_CAPS = integer bitmask, SLEELA_OS_CAP_MAC = integer).

    This is intentionally boring and side-effect-free: it makes NO privileged
    calls, mounts nothing, and loads no host code. A real deployment can add
    responders that read a well-known device node or socket, but the contract is
    the same -- return a CollarOffer the collar will independently MAC-verify, or
    None. A host that provides nothing yields None -> the collar fails closed.
    """
    e = env if env is not None else os.environ
    caps_raw = e.get("SLEELA_OS_CAPS")
    mac_raw = e.get("SLEELA_OS_CAP_MAC")
    if not caps_raw or not mac_raw:
        return None
    try:
        caps = int(caps_raw, 0) & 0xFFFFFFFF
        mac = int(mac_raw, 0) & 0xFFFFFFFFFFFFFFFF
    except ValueError:
        return None
    return CollarOffer(capabilities=caps, protocol=COLLAR_PROTOCOL, mac=mac)


if __name__ == "__main__":
    # Self-demonstration (no host, no network): show fail-closed + a good bond.
    key = b"\x2a" * MAC_KEY_BYTES
    collar = BondingCollar(local_caps=CAP_ALL, key=key)

    # 1) Unknown OS: no responder -> fail closed.
    print("unknown OS   ->", collar.confer(None).__dict__)

    # 2) Attested host offering storage+context (e.g. a TAC3-class layer).
    host = CollarOffer.make(CAP_T2_STORAGE | CAP_T3_CONTEXT, key)
    print("attested host->", collar.confer(host).__dict__)

    # 3) Forged host (wrong key) -> fail closed.
    forged = CollarOffer.make(CAP_ALL, b"\x99" * MAC_KEY_BYTES)
    print("forged host  ->", collar.confer(forged).__dict__)
