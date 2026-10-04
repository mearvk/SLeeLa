package com.mearvk.sleela.skya;

/**
 * Deployment defaults for the Skya GUI, backed by environment variables that
 * the launch scripts export from {@code config/skya.conf}. This is the single
 * place the Java apps read their default endpoints, so the GUI, the Guia
 * footer, and {@code skya.conf} stay in agreement.
 *
 * <p>Note: the {@code .sleela} programs cannot read config or environment (the
 * SLeeLa VM exposes no getenv/args builtin), so their listen/connect ports are
 * fixed in source at {@code 8443} (SKYA/1) and {@code 8700} (Guia). These
 * accessors configure the layers that CAN read them: the JavaFX GUI and the
 * native engine. Keep the defaults here aligned with those fixed ports.
 *
 * <p>Environment keys (exported by the launch scripts from skya.conf):
 * <ul>
 *   <li>{@code SKYA_SERVER_HOST} / {@code SKYA_SERVER_PORT} — the SKYA/1 server endpoint shown in the GUI</li>
 *   <li>{@code SKYA_DEFAULT_ROOM} — the default room</li>
 *   <li>{@code SKYA_GUIA_HOST} / {@code SKYA_GUIA_PORT} — the Guia control endpoint (see SkyaProtocolFooter)</li>
 * </ul>
 */
public final class SkyaConfig {
    private SkyaConfig() {}

    public static String serverHost() { return envOr("SKYA_SERVER_HOST", "localhost"); }
    public static String serverPort() { return envOr("SKYA_SERVER_PORT", "8443"); }
    public static String defaultRoom() { return envOr("SKYA_DEFAULT_ROOM", "lobby"); }
    public static String guiaHost() { return envOr("SKYA_GUIA_HOST", "127.0.0.1"); }
    public static int guiaPort() { return envIntOr("SKYA_GUIA_PORT", 8700); }

    static String envOr(String key, String def) {
        String v = System.getenv(key);
        return v == null || v.isBlank() ? def : v.trim();
    }

    static int envIntOr(String key, int def) {
        try {
            String v = System.getenv(key);
            return v == null || v.isBlank() ? def : Integer.parseInt(v.trim());
        } catch (NumberFormatException e) {
            return def;
        }
    }
}
