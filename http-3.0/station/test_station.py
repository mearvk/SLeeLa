"""Python parity tests for the SLeeLa HTTP 3.0 Station (J-STA-0001).

Mirrors the C classifier (station_classifier.c) and session state machine
(station_session.c) so the Station's judgment can be checked independently and
kept consistent across languages. Runs with plain `python3 test_station.py`
(pytest optional; not required in this sandbox).

The numbers here MUST match station_abi.h.
"""
from __future__ import annotations

from dataclasses import dataclass, field
from enum import IntEnum
from typing import List, Optional

# ---- ABI mirror (must equal station_abi.h) --------------------------------
SL_TCP_FIN, SL_TCP_SYN, SL_TCP_RST, SL_TCP_PSH = 0x01, 0x02, 0x04, 0x08
SL_TCP_ACK, SL_TCP_URG, SL_TCP_ECE, SL_TCP_CWR = 0x10, 0x20, 0x40, 0x80
URG_MAX = 8


class Cond(IntEnum):
    NONE = 0
    INTENTIONAL = 1
    LEGAL = 2
    WARNING = 3
    EMERGENCY = 4


class Hop(IntEnum):
    CANCEL = -1
    HOLD = 0
    ADVANCE = 1


class Premise(IntEnum):
    START = 0
    INTERMEDIARY = 1
    CANCELLING = 2


class State(IntEnum):
    IDLE = 0
    OPEN = 1
    CLEAR = 2
    UNCLEAR = 3
    HUNG_UP = 4
    DISASTROUS = 5


PARTY_A, PARTY_B = 0, 1


# ---- classifier parity (station_classifier.c) -----------------------------
def classify_hop(hop_delta: int) -> Hop:
    if hop_delta < 0:
        return Hop.CANCEL
    if hop_delta > 0:
        return Hop.ADVANCE
    return Hop.HOLD


def classify_condition(flags: int, urg_units: int) -> Cond:
    if not (flags & SL_TCP_URG) or urg_units == 0:
        return Cond.NONE
    if (flags & (SL_TCP_URG | SL_TCP_PSH | SL_TCP_ACK)) == (SL_TCP_URG | SL_TCP_PSH | SL_TCP_ACK) and urg_units == URG_MAX:
        return Cond.EMERGENCY
    if (flags & (SL_TCP_URG | SL_TCP_PSH)) == (SL_TCP_URG | SL_TCP_PSH):
        return Cond.WARNING
    if (flags & (SL_TCP_URG | SL_TCP_ACK)) == (SL_TCP_URG | SL_TCP_ACK) and urg_units == URG_MAX:
        return Cond.LEGAL
    return Cond.INTENTIONAL


def classify_premise(hop: Hop, current_open: Optional[Premise]) -> Premise:
    if hop == Hop.CANCEL:
        return Premise.CANCELLING
    if hop == Hop.ADVANCE:
        if current_open is None or current_open == Premise.CANCELLING:
            return Premise.START
        if current_open == Premise.START:
            return Premise.INTERMEDIARY
        return Premise.INTERMEDIARY
    # HOLD
    return current_open if current_open is not None else Premise.START


# ---- session/state-machine parity (station_session.c) ---------------------
@dataclass
class PartyState:
    open_premise: Optional[Premise] = None
    acked_peer: bool = False
    finished: bool = False


@dataclass
class Session:
    state: State = State.OPEN
    parties: List[PartyState] = field(default_factory=lambda: [PartyState(), PartyState()])
    legal_asserted: bool = False
    records: int = 0

    def signal(self, party: int, urg: int, hop_delta: int, extra: int = 0) -> Optional[Cond]:
        if self.state in (State.CLEAR, State.DISASTROUS):
            return None
        ps = self.parties[party]
        flags = extra | (SL_TCP_URG if urg > 0 else 0)

        if flags & SL_TCP_RST:
            self.state = State.DISASTROUS
            self.records += 1
            return Cond.WARNING
        if flags & SL_TCP_FIN:
            ps.finished = True
        if (flags & SL_TCP_ACK) and not (flags & SL_TCP_URG):
            ps.acked_peer = True

        cond = classify_condition(flags, urg)
        if cond != Cond.NONE:
            hop = classify_hop(hop_delta)
            prem = classify_premise(hop, ps.open_premise)
            self.records += 1
            if cond == Cond.LEGAL:
                self.legal_asserted = True
            ps.open_premise = prem
            if flags & SL_TCP_ACK:
                ps.acked_peer = True

        self._recompute()
        return cond

    def _recompute(self) -> None:
        a, b = self.parties
        if (a.open_premise == Premise.INTERMEDIARY and b.open_premise == Premise.INTERMEDIARY
                and a.acked_peer and b.acked_peer):
            self.state = State.CLEAR
        elif a.finished or b.finished:
            self.state = State.HUNG_UP
        elif a.open_premise == Premise.CANCELLING or b.open_premise == Premise.CANCELLING:
            self.state = State.UNCLEAR
        else:
            self.state = State.OPEN


# ---- tests ----------------------------------------------------------------
def test_hop_verbs():
    assert classify_hop(-1) == Hop.CANCEL
    assert classify_hop(0) == Hop.HOLD
    assert classify_hop(1) == Hop.ADVANCE


def test_condition_table():
    assert classify_condition(0, 5) == Cond.NONE            # no URG bit
    assert classify_condition(SL_TCP_URG, 0) == Cond.NONE   # zero magnitude
    assert classify_condition(SL_TCP_URG, 3) == Cond.INTENTIONAL
    assert classify_condition(SL_TCP_URG | SL_TCP_ACK, URG_MAX) == Cond.LEGAL
    assert classify_condition(SL_TCP_URG | SL_TCP_PSH, 4) == Cond.WARNING
    assert classify_condition(SL_TCP_URG | SL_TCP_PSH | SL_TCP_ACK, URG_MAX) == Cond.EMERGENCY


def test_premise_triple():
    # start -> intermediary via successive ADVANCE, then cancelling via CANCEL
    assert classify_premise(Hop.ADVANCE, None) == Premise.START
    assert classify_premise(Hop.ADVANCE, Premise.START) == Premise.INTERMEDIARY
    assert classify_premise(Hop.CANCEL, Premise.INTERMEDIARY) == Premise.CANCELLING


def test_clear_agreement():
    s = Session()
    s.signal(PARTY_A, 3, +1)
    s.signal(PARTY_B, 3, +1)
    s.signal(PARTY_A, 5, +1, SL_TCP_ACK)
    s.signal(PARTY_B, 5, +1, SL_TCP_ACK)
    assert s.state == State.CLEAR
    assert s.records == 4


def test_unclear_on_cancel():
    s = Session()
    s.signal(PARTY_A, 4, +1)
    s.signal(PARTY_B, 4, +1)
    s.signal(PARTY_A, 4, -1)
    assert s.state == State.UNCLEAR


def test_hung_up_on_fin():
    s = Session()
    s.signal(PARTY_A, 3, +1)
    s.signal(PARTY_B, 0, 0, SL_TCP_FIN)
    assert s.state == State.HUNG_UP


def test_disastrous_on_rst():
    s = Session()
    s.signal(PARTY_A, 3, +1)
    s.signal(PARTY_B, 0, 0, SL_TCP_RST)
    assert s.state == State.DISASTROUS


def _run_all() -> int:
    failures = 0
    for name, fn in sorted(globals().items()):
        if name.startswith("test_") and callable(fn):
            try:
                fn()
                print(f"  [ok]   {name}")
            except AssertionError as exc:  # pragma: no cover
                print(f"  [FAIL] {name}: {exc}")
                failures += 1
    print("Station parity tests:", "PASS" if failures == 0 else f"FAIL ({failures})")
    return failures


if __name__ == "__main__":
    raise SystemExit(_run_all())
