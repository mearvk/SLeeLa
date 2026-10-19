/** Runnable demonstration of the Wi-Fi 6 reference profile. */
public final class WifiProfileDemo { private WifiProfileDemo(){} public static void main(String[] args){WifiProfile p=new WifiProfile();p.print();System.out.println("\n80 MHz profile support: "+(p.supportsWidth(80)?"yes":"no"));System.out.println("Actual adapter capabilities must be queried from the operating system/driver.");} }
