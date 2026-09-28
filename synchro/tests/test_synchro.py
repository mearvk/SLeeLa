"""Self-contained tests for Synchro. No network egress required.

The UDP test runs a real loopback echo server in a thread and measures genuine
RTTs, so it validates the whole send/ack/timing/stats path end to end.
"""

import socket
import threading
import time

import pytest

from synchro import (
    LatencyStats,
    Sample,
    SlaReporter,
    UdpDispatcher,
    load_backend,
    synchro,
)
from synchro.dispatcher import run_echo_server
from synchro.http2 import RateMeter


def test_stats_percentiles_and_loss():
    s = LatencyStats("d")
    for i in range(1, 101):  # 1..100 ms
        s.record(Sample("d", float(i), 0.0, i))
    s.record(Sample("d", None, 0.0, 101))  # one loss
    assert s.min == 1.0
    assert s.max == 100.0
    assert s.p50 == 50.0
    assert s.p95 == 95.0
    assert s.p99 == 99.0
    assert s.sent == 101 and s.acked == 100 and s.lost == 1
    assert abs(s.loss_rate - (1 / 101)) < 1e-9


def _free_port() -> int:
    s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    s.bind(("127.0.0.1", 0))
    port = s.getsockname()[1]
    s.close()
    return port


def test_udp_roundtrip_loopback():
    port = _free_port()
    rounds = 50
    t = threading.Thread(
        target=run_echo_server, args=(("127.0.0.1", port),),
        kwargs={"stop_after": rounds}, daemon=True,
    )
    t.start()
    time.sleep(0.1)

    disp = UdpDispatcher([("127.0.0.1", port)], timeout_s=0.5)
    result = disp.run(rounds=rounds, interval_s=0.001)
    key = f"127.0.0.1:{port}"
    s = result.stats[key]
    # On loopback essentially everything should be delivered and measurable.
    assert s.acked >= rounds - 2
    assert s.p99 is not None and s.p99 >= 0.0


def test_sla_reporter():
    s = LatencyStats("d")
    for i in range(100):
        s.record(Sample("d", 0.5, 0.0, i))  # all 0.5 ms
    rep = SlaReporter(1.0, percentile=99).evaluate({"d": s})
    assert rep.dests_meeting == 1
    assert rep.dest_compliance == 1.0
    assert rep.sample_compliance == 1.0

    rep2 = SlaReporter(0.1, percentile=99).evaluate({"d": s})
    assert rep2.dests_meeting == 0


def test_rate_meter_paces():
    m = RateMeter(rate_per_s=50.0, burst=1.0)
    m.acquire(1.0)  # drains initial token
    start = time.monotonic()
    m.acquire(1.0)  # must wait ~1/50 s
    elapsed = time.monotonic() - start
    assert elapsed >= 0.015  # allow scheduler slack around the 0.02s target


def test_backend_lazy_loading_and_annotation():
    disp = load_backend("udp", [("127.0.0.1", 9)], cache=False)
    assert isinstance(disp, UdpDispatcher)

    @synchro("udp", [("127.0.0.1", 9)], cache=False)
    def probe(*, backend):
        return type(backend).__name__

    assert probe() == "UdpDispatcher"
    assert probe.synchro_backend == "udp"


def test_unknown_backend_raises():
    from synchro import SynchroError

    with pytest.raises(SynchroError):
        load_backend("does-not-exist")


def test_empty_stats_and_invalid_percentiles():
    s = LatencyStats("empty")
    assert s.n == 0
    assert s.mean is None
    assert s.p99 is None
    with pytest.raises(ValueError):
        s.percentile(-1)
    with pytest.raises(ValueError):
        s.percentile(101)


def test_stats_window_retains_only_recent_samples():
    s = LatencyStats("d", window=2)
    for i in (1.0, 2.0, 3.0):
        s.record(Sample("d", i, 0.0, int(i)))
    assert s.sent == s.acked == 3
    assert s.n == 2
    assert s.min == 2.0
    assert s.max == 3.0
    assert s.mean == 2.5


def test_invalid_runtime_configuration_is_rejected():
    with pytest.raises(ValueError):
        LatencyStats("d", window=0)
    with pytest.raises(ValueError):
        UdpDispatcher([], timeout_s=0)
    with pytest.raises(ValueError):
        UdpDispatcher([], window=0)
    with pytest.raises(ValueError):
        UdpDispatcher([]).run(rounds=-1)
    with pytest.raises(ValueError):
        UdpDispatcher([]).run(interval_s=-1)
    with pytest.raises(ValueError):
        RateMeter(10, burst=0)
    meter = RateMeter(10, burst=1)
    with pytest.raises(ValueError):
        meter.acquire(2)
    with pytest.raises(ValueError):
        meter.acquire(0)


def test_backend_without_constructor_args_is_instantiated_and_cached():
    from synchro import clear_backend_cache

    clear_backend_cache()
    first = load_backend("udp")
    second = load_backend("udp")
    assert isinstance(first, UdpDispatcher)
    assert first is second


def test_backend_constructor_and_factory_errors_are_normalized():
    from synchro import SynchroError, register_backend

    register_backend("bad-factory", "synchro.tests.test_synchro:no_such_symbol")
    with pytest.raises(SynchroError):
        load_backend("bad-factory", cache=False)
    register_backend("not-callable", "synchro.tests.test_synchro:pytest")
    with pytest.raises(SynchroError):
        load_backend("not-callable", cache=False)


def test_udp_ignores_malformed_and_unknown_packets():
    port = _free_port()
    ready = threading.Event()

    def server():
        sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        sock.bind(("127.0.0.1", port))
        ready.set()
        try:
            data, addr = sock.recvfrom(65535)
            sock.sendto(b"not-synchro", addr)
            sock.sendto(data, addr)
        finally:
            sock.close()

    threading.Thread(target=server, daemon=True).start()
    assert ready.wait(1.0)
    result = UdpDispatcher([("127.0.0.1", port)], timeout_s=0.2).run(rounds=1)
    stats = result.stats[f"127.0.0.1:{port}"]
    assert stats.acked == 1
    assert stats.lost == 0


def test_sla_empty_destination_and_threshold_boundary():
    s = LatencyStats("d")
    s.record(Sample("d", 1.0, 0.0, 1))
    rep = SlaReporter(1.0, percentile=99).evaluate({"d": s})
    assert rep.dests_meeting == 1
    assert rep.sample_compliance == 1.0
    empty = SlaReporter(1.0).evaluate({})
    assert empty.total_dests == 0
    assert empty.dest_compliance == 0.0
    assert empty.sample_compliance == 0.0
