import java.awt.image.BufferedImage;
import java.io.File;
import javax.imageio.ImageIO;

// Trim the title-bar logo to its minimum 2D content bounding box, ignoring the
// faint background vignette, and write a transparent-background PNG.
// Usage: java Trim <in> <out.png> <threshold> <minBandFraction%>
public class Trim {
    public static void main(String[] args) throws Exception {
        BufferedImage img = ImageIO.read(new File(args[0]));
        int w = img.getWidth(), h = img.getHeight();
        int threshold = Integer.parseInt(args[2]);
        double bandPct = args.length >= 4 ? Double.parseDouble(args[3]) : 1.0;

        int[] col = new int[w];
        int[] row = new int[h];
        int[][] d = new int[h][w];
        for (int y = 0; y < h; y++)
            for (int x = 0; x < w; x++) {
                int argb = img.getRGB(x, y);
                int r = (argb >> 16) & 0xff, g = (argb >> 8) & 0xff, b = argb & 0xff;
                int dist = Math.max(255 - r, Math.max(255 - g, 255 - b));
                d[y][x] = dist;
                if (dist > threshold) { col[x]++; row[y]++; }
            }

        int minCol = (int) Math.max(2, h * bandPct / 100.0);
        int minRow = (int) Math.max(2, w * bandPct / 100.0);
        int x0 = -1, x1 = -1, y0 = -1, y1 = -1;
        for (int x = 0; x < w; x++) if (col[x] >= minCol) { if (x0 < 0) x0 = x; x1 = x; }
        for (int y = 0; y < h; y++) if (row[y] >= minRow) { if (y0 < 0) y0 = y; y1 = y; }
        if (x1 < 0) { System.out.println("no content"); return; }

        int bw = x1 - x0 + 1, bh = y1 - y0 + 1;
        BufferedImage out = new BufferedImage(bw, bh, BufferedImage.TYPE_INT_ARGB);
        for (int y = 0; y < bh; y++)
            for (int x = 0; x < bw; x++) {
                int sx = x0 + x, sy = y0 + y;
                int argb = img.getRGB(sx, sy);
                int r = (argb >> 16) & 0xff, g = (argb >> 8) & 0xff, b = argb & 0xff;
                int dist = d[sy][sx];
                int alpha;
                if (dist <= threshold) alpha = 0;
                else if (dist >= threshold + 24) alpha = 255;
                else alpha = (int) (255.0 * (dist - threshold) / 24.0);
                out.setRGB(x, y, (alpha << 24) | (r << 16) | (g << 8) | b);
            }
        ImageIO.write(out, "png", new File(args[1]));
        System.out.printf("wrote %s %dx%d from bbox (%d,%d)-(%d,%d)%n",
                args[1], bw, bh, x0, y0, x1, y1);
    }
}
