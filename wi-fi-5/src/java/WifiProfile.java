import java.util.Arrays;
import java.util.Collections;
import java.util.List;

/** Static reference metadata for Wi-Fi 5; not a hardware probe or Wi-Fi PHY/MAC implementation. */
public final class WifiProfile {
    public final String generation = "Wi-Fi 5";
    public final String ieeeStandard = "IEEE 802.11ac";
    public final String bands = "Primarily 5 GHz";
    public final List<Integer> channelWidthsMHz = Collections.unmodifiableList(Arrays.asList(20, 40, 80, 160));
    public final int maxSpatialStreams = 8;
    public final int maxQamOrder = 256;
    public final int theoreticalPeakMbps = 6933;
    public final List<String> features = Collections.unmodifiableList(Arrays.asList(
            "Wide channels",
            "256-QAM",
            "Downlink MU-MIMO",
            "Beamforming",
            "A-MPDU/A-MSDU aggregation"));

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
