import java.util.Arrays;
import java.util.Collections;
import java.util.List;
/** Static reference metadata for Wi-Fi 6, not a hardware probe or PHY/MAC implementation. */
public final class WifiProfile {
 public final String generation="Wi-Fi 6",ieeeStandard="IEEE 802.11ax",bands="2.4 GHz and 5 GHz; 6 GHz for Wi-Fi 6E where permitted";
 public final List<Integer> channelWidthsMHz=Collections.unmodifiableList(Arrays.asList(20,40,80,160));
 public final int maxSpatialStreams=8,maxQamOrder=1024,theoreticalPeakMbps=9608;
 public final List<String> features=Collections.unmodifiableList(Arrays.asList("OFDMA uplink/downlink","MU-MIMO uplink/downlink","1024-QAM","BSS coloring/spatial reuse","Target Wake Time"));
 public boolean supportsWidth(int widthMHz){return channelWidthsMHz.contains(widthMHz);}
 public void print(){System.out.println(generation+" ("+ieeeStandard+")");System.out.println("Bands: "+bands);System.out.println("Maximum theoretical PHY rate: "+theoreticalPeakMbps+" Mbit/s (not application throughput)");System.out.println("Maximum standard spatial streams: "+maxSpatialStreams+"; highest listed modulation: "+maxQamOrder+"-QAM");System.out.println("Reference features:");for(String f:features)System.out.println("  - "+f);}
}
