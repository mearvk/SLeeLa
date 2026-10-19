import java.util.Arrays;
import java.util.Collections;
import java.util.List;

/** Static reference metadata for Wi-Fi 4, not a hardware probe or PHY/MAC implementation. */
public final class WifiProfile {
    public final String generation = "Wi-Fi 4";
    public final String ieeeStandard = "IEEE 802.11n";
    public final String bands = "2.4 GHz and 5 GHz, subject to hardware and regulatory support";
    public final List<Integer> channelWidthsMHz = Collections.unmodifiableList(Arrays.asList(20, 40));
    public final int maxSpatialStreams = 4;
    public final int maxQamOrder = 64;
    public final int theoreticalPeakMbps = 600;
    public final List<String> features = Collections.unmodifiableList(Arrays.asList(
            "MIMO spatial streams",
            "OFDM",
            "A-MPDU/A-MSDU aggregation",
            "Short guard interval"));

    public boolean supportsWidth(int widthMHz) {
        return channelWidthsMHz.contains(widthMHz);
    }

    public void print() {
        System.out.println(generation + " (" + ieeeStandard + ")");
        System.out.println("Bands: " + bands);
        System.out.println("Maximum theoretical PHY rate: " + theoreticalPeakMbps + " Mbit/s (not application throughput)");
        System.out.println("Maximum standard spatial streams: " + maxSpatialStreams + "; highest listed modulation: " + maxQamOrder + "-QAM");
        System.out.println("Reference features:");
        for (String feature : features) System.out.println("  - " + feature);
    }
}
