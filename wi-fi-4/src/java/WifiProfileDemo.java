/** Runnable demonstration of the Wi-Fi 4 reference profile. */
public final class WifiProfileDemo {
    private WifiProfileDemo() { }

    public static void main(String[] args) {
        WifiProfile profile = new WifiProfile();
        profile.print();
        System.out.println();
        System.out.println("Reference check: 40 MHz channel "
                + (profile.supportsWidth(40) ? "is listed as supported" : "is not listed as supported")
                + " by this generation profile.");
        System.out.println("Note: query the operating system/driver for actual adapter capabilities.");
    }
}
