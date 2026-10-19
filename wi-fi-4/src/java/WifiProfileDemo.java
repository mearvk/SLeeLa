/** Runnable demonstration of the Wi-Fi 4 reference profile. */
public final class WifiProfileDemo {
    private WifiProfileDemo() { }

    public static void main(String[] args) {
        WifiProfile profile = new WifiProfile();
        profile.print();
        System.out.println();
        System.out.println("40 MHz profile support: "
                + (profile.supportsWidth(40) ? "yes" : "no"));
        System.out.println("Query the operating system/driver for actual adapter capabilities.");
    }
}
