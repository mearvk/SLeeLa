package com.mearvk.sleela.connector.http;

import java.nio.charset.StandardCharsets;
import java.time.Instant;
import java.util.ArrayList;
import java.util.LinkedHashMap;
import java.util.List;
import java.util.Map;
import java.util.concurrent.atomic.AtomicLong;

/**
 * Sleeuum&trade; — server-side packet ledger for the SLeeLa HTTP 3.0 module.
 *
 * <p>The Java counterpart of {@code http-3.0/sleeuum.py}. It keeps track of the
 * HTTP 3.0 / HTTP 2.0+ packets that {@link SleelaH3Servlet} handles — with their
 * <b>dates</b> (ISO-8601 UTC) and their <b>numbers</b> (a monotonic sequence, the
 * H3 NONCE / DIGEST / request-id / status, byte counts, and the basket numerals
 * carried in each packet). It emits JSON that conforms to
 * {@code http-3.0/sleeuum.schema.json}.</p>
 *
 * <p>Observe-only, exactly like the client module: the ledger never alters a
 * packet or an integrity field; it records the H3 status precisely as the
 * pipeline returned it. Thread-safe for concurrent servlet requests.</p>
 */
public final class SleeuumLedger {

    /** Trademark rendering (unicode). ASCII fallback: {@code Sleeuum(TM)}. */
    public static final String SLEEUUM_TM = "Sleeuum\u2122";

    private final String client;
    private final AtomicLong seq = new AtomicLong(0);
    private final List<Map<String, Object>> records = new ArrayList<>();

    // Rolled-up numbers (guarded by `this`).
    private long sent, received, requests, responses, totalPayload, totalWire;
    private final Map<String, Long> byCarrier = new LinkedHashMap<>();
    private final Map<String, Long> byStatus = new LinkedHashMap<>();
    private String firstSeen, lastSeen;

    public SleeuumLedger(String client) {
        this.client = client == null ? "sleela-h3" : client;
    }

    /** Record a request packet the server received (raw H3 envelope wire bytes). */
    public synchronized void trackRequest(byte[] h3Wire, String carrier, int wireBytes) {
        Map<String, Object> r = base("received", carrier, "request", h3Wire, wireBytes);
        H3Text parsed = parseRequestText(h3Wire);
        if (parsed != null) {
            r.put("service_id", parsed.serviceId);
            r.put("op_id", parsed.opId);
            r.put("request_id", parsed.requestId);
            r.put("nonce", parsed.nonce);
            r.put("digest", parsed.digest);
            r.put("intactx", parsed.intactx);
            r.put("payload_bytes", parsed.payloadLen);
            r.put("numerals", parsed.numerals);
        }
        requests++;
        commit(r);
    }

    /** Record a response packet the server sent (raw H3R response wire bytes). */
    public synchronized void trackResponse(byte[] h3rWire, String carrier, int wireBytes) {
        Map<String, Object> r = base("sent", carrier, "response", h3rWire, wireBytes);
        H3Resp parsed = parseResponseText(h3rWire);
        if (parsed != null) {
            r.put("request_id", parsed.requestId);
            r.put("status", parsed.status);
            r.put("status_name", statusName(parsed.status));
            r.put("payload_bytes", parsed.resultLen);
            byStatus.merge(statusName(parsed.status), 1L, Long::sum);
        }
        responses++;
        commit(r);
    }

    /** Serialize the ledger to schema-conforming JSON (dates + numbers + packets). */
    public synchronized String toJson() {
        StringBuilder sb = new StringBuilder(256 + records.size() * 128);
        sb.append('{');
        kv(sb, "module", SLEEUUM_TM).append(',');
        kv(sb, "trademark", SLEEUUM_TM).append(',');
        kv(sb, "client", client).append(',');
        kv(sb, "generated", nowIso()).append(',');
        sb.append("\"stats\":").append(statsJson()).append(',');
        sb.append("\"packets\":[");
        for (int i = 0; i < records.size(); i++) {
            if (i > 0) sb.append(',');
            sb.append(mapJson(records.get(i)));
        }
        sb.append("]}");
        return sb.toString();
    }

    public synchronized long count() { return records.size(); }

    // ------------------------------------------------------------------ internals

    private Map<String, Object> base(String direction, String carrier, String kind,
                                     byte[] wire, int wireBytes) {
        String at = nowIso();
        Map<String, Object> r = new LinkedHashMap<>();
        r.put("seq", seq.incrementAndGet());
        r.put("at", at);
        r.put("direction", direction);
        r.put("carrier", carrier == null ? "h3-raw" : carrier);
        r.put("kind", kind);
        r.put("wire_form", wireFormOf(wire));
        r.put("payload_bytes", 0);
        r.put("wire_bytes", wireBytes);
        return r;
    }

    private void commit(Map<String, Object> r) {
        records.add(r);
        String carrier = (String) r.get("carrier");
        byCarrier.merge(carrier, 1L, Long::sum);
        if ("sent".equals(r.get("direction"))) sent++; else received++;
        totalPayload += ((Number) r.getOrDefault("payload_bytes", 0)).longValue();
        totalWire += ((Number) r.getOrDefault("wire_bytes", 0)).longValue();
        String at = (String) r.get("at");
        if (firstSeen == null) firstSeen = at;
        lastSeen = at;
    }

    private String statsJson() {
        StringBuilder sb = new StringBuilder(160);
        sb.append('{');
        sb.append("\"packets\":").append(records.size()).append(',');
        sb.append("\"sent\":").append(sent).append(',');
        sb.append("\"received\":").append(received).append(',');
        sb.append("\"requests\":").append(requests).append(',');
        sb.append("\"responses\":").append(responses).append(',');
        sb.append("\"total_payload_bytes\":").append(totalPayload).append(',');
        sb.append("\"total_wire_bytes\":").append(totalWire).append(',');
        sb.append("\"by_carrier\":").append(countMapJson(byCarrier)).append(',');
        sb.append("\"by_status\":").append(countMapJson(byStatus)).append(',');
        sb.append("\"first_seen\":").append(firstSeen == null ? "null" : quote(firstSeen)).append(',');
        sb.append("\"last_seen\":").append(lastSeen == null ? "null" : quote(lastSeen)).append(',');
        sb.append("\"duration_seconds\":").append(durationSeconds());
        sb.append('}');
        return sb.toString();
    }

    private double durationSeconds() {
        if (firstSeen == null || lastSeen == null) return 0.0;
        try {
            return (Instant.parse(lastSeen).toEpochMilli() - Instant.parse(firstSeen).toEpochMilli()) / 1000.0;
        } catch (RuntimeException e) {
            return 0.0;
        }
    }

    private static String nowIso() {
        // Instant.toString() is ISO-8601 UTC with a trailing 'Z' (schema pattern).
        return Instant.now().toString();
    }

    private static String wireFormOf(byte[] wire) {
        if (wire != null && wire.length >= 3 && wire[0] == 'H' && wire[1] == '3') {
            // "H3 " -> textual envelope; "H3R" -> textual response; both are text.
            return "text";
        }
        return "binary";
    }

    // ---- H3 textual parsers (mirror Envelope/Response.unpack_text) ------------

    private static final class H3Text {
        long serviceId, opId, requestId, nonce, digest, intactx;
        int payloadLen;
        List<Integer> numerals = new ArrayList<>();
    }

    private static final class H3Resp {
        int status;
        long requestId;
        int resultLen;
    }

    /**
     * Parse a textual H3 request line:
     * {@code H3 <ver> <flags> <sid> <oid> <rid> <nonce> <digest> <intactx> <basket-hex> <plen>:<payload>}
     */
    private static H3Text parseRequestText(byte[] wire) {
        int colon = indexOf(wire, (byte) ':');
        if (colon < 0) return null;
        String head = new String(wire, 0, colon, StandardCharsets.US_ASCII);
        String[] p = head.split(" ");
        if (p.length != 11 || !"H3".equals(p[0])) return null;
        try {
            H3Text h = new H3Text();
            h.serviceId = Long.parseLong(p[3]);
            h.opId = Long.parseLong(p[4]);
            h.requestId = Long.parseLong(p[5]);
            h.nonce = Long.parseLong(p[6]);
            h.digest = Long.parseUnsignedLong(p[7]);
            h.intactx = Long.parseUnsignedLong(p[8]);
            h.payloadLen = Integer.parseInt(p[10]);
            h.numerals = basketNumerals(p[9]);
            return h;
        } catch (RuntimeException e) {
            return null;
        }
    }

    /** Parse a textual H3 response line: {@code H3R <status> <rid> <rlen>:<result>}. */
    private static H3Resp parseResponseText(byte[] wire) {
        int colon = indexOf(wire, (byte) ':');
        if (colon < 0) return null;
        String head = new String(wire, 0, colon, StandardCharsets.US_ASCII);
        String[] p = head.split(" ");
        if (p.length != 4 || !"H3R".equals(p[0])) return null;
        try {
            H3Resp r = new H3Resp();
            r.status = Integer.parseInt(p[1]);
            r.requestId = Long.parseLong(p[2]);
            r.resultLen = Integer.parseInt(p[3]);
            return r;
        } catch (RuntimeException e) {
            return null;
        }
    }

    /**
     * Extract basket numerals (atomic-number item ids) from the basket hex token.
     * Canonical block: iso(2) + count(2), then per item number(4) + value(8),
     * big-endian — matches basket_serialize() / http3_basket_serialize().
     */
    private static List<Integer> basketNumerals(String basketHex) {
        List<Integer> out = new ArrayList<>();
        try {
            byte[] b = hexToBytes(basketHex);
            if (b.length < 4) return out;
            int count = ((b[2] & 0xFF) << 8) | (b[3] & 0xFF);
            int off = 4;
            for (int i = 0; i < count && off + 12 <= b.length; i++, off += 12) {
                int number = ((b[off] & 0xFF) << 24) | ((b[off + 1] & 0xFF) << 16)
                        | ((b[off + 2] & 0xFF) << 8) | (b[off + 3] & 0xFF);
                out.add(number);
            }
        } catch (RuntimeException e) {
            // leave numerals empty on any parse issue
        }
        return out;
    }

    private static String statusName(int status) {
        switch (status) {
            case 0: return "OK";
            case 1: return "APP_ERROR";
            case 2: return "UNKNOWN_SERVICE";
            case 3: return "UNKNOWN_OPERATION";
            case 4: return "BAD_ENVELOPE";
            case 5: return "TOO_LARGE";
            case 6: return "RETRY_DENIED";
            case 7: return "BAD_DIGEST";
            case 8: return "TAMPERED";
            case 9: return "REPLAYED";
            default: return "APP_ERROR";
        }
    }

    // ---- small helpers --------------------------------------------------------

    private static int indexOf(byte[] a, byte target) {
        for (int i = 0; i < a.length; i++) if (a[i] == target) return i;
        return -1;
    }

    private static byte[] hexToBytes(String hex) {
        int n = hex.length() / 2;
        byte[] out = new byte[n];
        for (int i = 0; i < n; i++) {
            out[i] = (byte) Integer.parseInt(hex.substring(i * 2, i * 2 + 2), 16);
        }
        return out;
    }

    private static StringBuilder kv(StringBuilder sb, String k, String v) {
        return sb.append(quote(k)).append(':').append(quote(v));
    }

    private static String countMapJson(Map<String, Long> m) {
        StringBuilder sb = new StringBuilder();
        sb.append('{');
        boolean first = true;
        for (Map.Entry<String, Long> e : m.entrySet()) {
            if (!first) sb.append(',');
            sb.append(quote(e.getKey())).append(':').append(e.getValue());
            first = false;
        }
        return sb.append('}').toString();
    }

    @SuppressWarnings("unchecked")
    private static String mapJson(Map<String, Object> m) {
        StringBuilder sb = new StringBuilder();
        sb.append('{');
        boolean first = true;
        for (Map.Entry<String, Object> e : m.entrySet()) {
            if (!first) sb.append(',');
            sb.append(quote(e.getKey())).append(':');
            Object v = e.getValue();
            if (v instanceof String) {
                sb.append(quote((String) v));
            } else if (v instanceof List) {
                sb.append('[');
                List<Object> list = (List<Object>) v;
                for (int i = 0; i < list.size(); i++) {
                    if (i > 0) sb.append(',');
                    sb.append(String.valueOf(list.get(i)));
                }
                sb.append(']');
            } else {
                sb.append(String.valueOf(v)); // numbers
            }
            first = false;
        }
        return sb.append('}').toString();
    }

    private static String quote(String s) {
        StringBuilder sb = new StringBuilder(s.length() + 2);
        sb.append('"');
        for (int i = 0; i < s.length(); i++) {
            char c = s.charAt(i);
            switch (c) {
                case '"':  sb.append("\\\""); break;
                case '\\': sb.append("\\\\"); break;
                case '\n': sb.append("\\n"); break;
                case '\r': sb.append("\\r"); break;
                case '\t': sb.append("\\t"); break;
                default:
                    if (c < 0x20 || c > 0x7E) {
                        sb.append(String.format("\\u%04x", (int) c));
                    } else {
                        sb.append(c);
                    }
            }
        }
        return sb.append('"').toString();
    }
}
