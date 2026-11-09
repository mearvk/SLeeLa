package com.mearvk.sleela.connector;

import java.util.Objects;
import java.io.IOException;
import java.nio.file.Path;

/** Differential checks between direct Java execution and the SLeeLa JDK bridge. */
public final class SleelaJdkConformance {
    private static final String INIT_PROBE_KEY = "sleela.jdk.bridge.init.probe";

    public static final class InitializationProbe {
        static { System.setProperty(INIT_PROBE_KEY, "initialized"); }
        public static String touch() { return "initialized"; }
    }

    private SleelaJdkConformance() {}

    public static void run() throws Exception {
        // The bridge must not resolve arbitrary host classes by default.
        assert !SleelaJdkBridge.isAllowed("java.lang.Runtime");
        assert !SleelaJdkBridge.available("java.lang.Runtime");

        // Each class used by this conformance vector is explicitly permitted.
        SleelaJdkBridge.allowClass("java.lang.String");
        SleelaJdkBridge.allowClass("java.util.ArrayList");
        assert SleelaJdkBridge.isAllowed("java.lang.String");
        assert SleelaJdkBridge.available("java.lang.String");

        // Type discovery must not run the target class static initializer.
        System.clearProperty(INIT_PROBE_KEY);
        String probeName = InitializationProbe.class.getName();
        SleelaJdkBridge.allowClass(probeName);
        SleelaJdkBridge.type(probeName);
        assert System.getProperty(INIT_PROBE_KEY) == null;
        Object initialized = SleelaJdkBridge.invokeStatic(probeName, "touch",
                new Class<?>[0], new Object[0]);
        assert Objects.equals(initialized, "initialized");
        assert Objects.equals(System.getProperty(INIT_PROBE_KEY), "initialized");

        Object direct = String.valueOf(42);
        Object bridged = SleelaJdkBridge.invokeStatic("java.lang.String", "valueOf",
                new Class<?>[]{int.class}, new Object[]{42});
        assert Objects.equals(direct, bridged);

        Object directLength = "SLeeLa".length();
        Object bridgedLength = SleelaJdkBridge.invoke("SLeeLa", "length",
                new Class<?>[0], new Object[0]);
        assert Objects.equals(directLength, bridgedLength);

        Object directList = new java.util.ArrayList<String>();
        Object bridgedList = SleelaJdkBridge.construct("java.util.ArrayList",
                new Class<?>[0], new Object[0]);
        SleelaJdkBridge.invoke(bridgedList, "add",
                new Class<?>[]{Object.class}, new Object[]{"SLeeLa"});
        directList.add("SLeeLa");
        assert Objects.equals(directList, bridgedList);

        boolean rejectedStaticAsInstance = false;
        try {
            SleelaJdkBridge.invoke("SLeeLa", "valueOf",
                    new Class<?>[]{int.class}, new Object[]{7});
        } catch (IllegalArgumentException expected) {
            rejectedStaticAsInstance = true;
        }
        assert rejectedStaticAsInstance;

        // Adding roots after resolution could split class identity and invalidate
        // object handles, so the loader must freeze its search roots at first use.
        boolean rejectedLateRoot = false;
        try {
            SleelaJdkBridge.addRoot(Path.of("."));
        } catch (IOException expected) {
            rejectedLateRoot = true;
        }
        assert rejectedLateRoot;
    }

    public static void main(String[] args) throws Exception {
        run();
        System.out.println("SLeeLa JDK conformance: PASS");
    }
}
