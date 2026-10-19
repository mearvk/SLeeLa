/** Runnable demonstration of the Wi-Fi 6 reference profile. */
public final class WifiProfileDemo {
    private WifiProfileDemo() { }

    public static void main(String[] args) {
        WifiProfile profile = new WifiProfile();
        profile.print();
        System.out.println();
        System.out.println("80 MHz profile support: "
                + (profile.supportsWidth(80) ? "yes" : "no"));
        System.out.println("Query the operating system/driver for actual adapter capabilities.");
    }
}
