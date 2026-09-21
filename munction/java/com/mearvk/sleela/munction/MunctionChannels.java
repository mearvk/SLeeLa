package com.mearvk.sleela.munction;

import com.mearvk.sleela.munction.channels.CryptoChannel;
import com.mearvk.sleela.munction.channels.FileChannel;
import com.mearvk.sleela.munction.channels.HttpChannel;
import com.mearvk.sleela.munction.channels.NetChannel;
import com.mearvk.sleela.munction.channels.PacketChannel;
import com.mearvk.sleela.munction.channels.PipeChannel;

import java.util.Locale;
import java.util.Map;
import java.util.Objects;
import java.util.concurrent.ConcurrentHashMap;
import java.util.function.Supplier;

/**
 * Scheme → {@link MunctionChannel} registry (MUNCTION.md §4). A Munction™
 * {@code connect(uri)} resolves the channel for the URI's scheme here, so
 * reaching a new <em>system method</em> is a channel registration, not a grammar
 * change. The built-in schemes cover pipes, file I/O, the internet (TCP + HTTP),
 * private packets, and crypto.
 */
public final class MunctionChannels {

    private static final Map<String, Supplier<MunctionChannel>> REGISTRY = new ConcurrentHashMap<>();

    static {
        register("pipe", PipeChannel::new);          // OS pipe / FIFO / stdio
        register("file", FileChannel::new);          // file I/O
        register("tcp", NetChannel::new);            // internet (TCP)
        register("net", NetChannel::new);            // internet (TCP, alias)
        register("http", HttpChannel::new);          // internet (HTTP)
        register("https", HttpChannel::new);         // internet (HTTPS)
        register("sdps", PacketChannel::new);        // private packets (SDPS)
        register("crypto", CryptoChannel::new);      // crypto envelope (wraps inner)
    }

    private MunctionChannels() {
    }

    /** Register (or replace) the channel factory for a scheme. */
    public static void register(String scheme, Supplier<MunctionChannel> factory) {
        REGISTRY.put(normalize(scheme), Objects.requireNonNull(factory, "factory"));
    }

    /** True when a channel is registered for {@code scheme}. */
    public static boolean supports(String scheme) {
        return REGISTRY.containsKey(normalize(scheme));
    }

    /**
     * Resolve a channel from a full URI: split the leading {@code scheme:} and
     * construct the registered channel. Supports nested schemes such as
     * {@code crypto:tcp://host:port}, where {@code crypto} wraps the inner
     * {@code tcp} channel.
     */
    public static MunctionChannel resolve(String uri) {
        Objects.requireNonNull(uri, "uri");
        int colon = uri.indexOf(':');
        if (colon <= 0) {
            throw new IllegalArgumentException("Munction connect URI needs a scheme: " + uri);
        }
        String scheme = normalize(uri.substring(0, colon));
        Supplier<MunctionChannel> factory = REGISTRY.get(scheme);
        if (factory == null) {
            throw new IllegalArgumentException("No Munction channel for scheme '" + scheme + "'");
        }
        return factory.get();
    }

    /** The scheme portion of a URI, lower-cased. */
    public static String schemeOf(String uri) {
        int colon = uri.indexOf(':');
        return colon <= 0 ? "" : normalize(uri.substring(0, colon));
    }

    /**
     * The address portion a channel binds to: everything after {@code scheme:},
     * with a leading {@code //} stripped. For a nested {@code crypto:} URI the
     * remainder is the inner URI (handled by the crypto channel itself).
     */
    public static String addressOf(String uri) {
        int colon = uri.indexOf(':');
        String rest = colon < 0 ? uri : uri.substring(colon + 1);
        if (rest.startsWith("//")) {
            rest = rest.substring(2);
        }
        return rest;
    }

    private static String normalize(String scheme) {
        return Objects.requireNonNull(scheme, "scheme").trim().toLowerCase(Locale.ROOT);
    }
}
