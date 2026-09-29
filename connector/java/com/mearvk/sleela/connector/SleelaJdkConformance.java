package com.mearvk.sleela.connector;
import java.util.Objects;

/** Differential checks between direct Java execution and the SLeeLa JDK bridge. */
public final class SleelaJdkConformance {
    private SleelaJdkConformance() {}
    public static void run() throws Exception {
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
    }
    public static void main(String[] args) throws Exception {
        run();
        System.out.println("SLeeLa JDK conformance: PASS");
    }
}
