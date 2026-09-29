/*
 * CommonRails Heritage Print — Java implementation.
 * Canonical Beautiful Design: fixed 80-column printing.
 */
package com.sleela.commonrails.heritage;

public final class HeritagePrint {
    public static final int PRINT_WIDTH = 80;
    public static final int SQUARE_SIZE = 21;

    private HeritagePrint() {}

    public static void printWidthLine(String content) {
        int pos = 0;
        while (pos < content.length()) {
            int remaining = content.length() - pos;
            int take = Math.min(remaining, PRINT_WIDTH);
            if (remaining > PRINT_WIDTH) {
                int cut = take;
                while (cut > 0 && content.charAt(pos + cut) != ' '
                        && content.charAt(pos + cut - 1) != ' ') {
                    --cut;
                }
                if (cut > 0) take = cut;
            }
            String part = content.substring(pos, pos + take);
            System.out.printf("%-" + PRINT_WIDTH + "s%n", part);
            pos += take;
            while (pos < content.length() && content.charAt(pos) == ' ') ++pos;
        }
    }

    public static void printField(String content, int fieldWidth) {
        System.out.printf("%-" + fieldWidth + "s", content);
    }

    public static void printComponent(String name, long objectId,
                                      long date, String message) {
        String line = String.format(
            "-- : [Object ID: %010d] [Date: %d] [Current: @%s] . %s .",
            objectId, date, name, message);
        printWidthLine(line);
    }

    public static void printSquare(int filled) {
        int total = SQUARE_SIZE * SQUARE_SIZE;
        filled = Math.max(0, Math.min(total, filled));
        for (int row = 0; row < SQUARE_SIZE; ++row) {
            StringBuilder line = new StringBuilder();
            for (int col = 0; col < SQUARE_SIZE; ++col) {
                int fillRow = (SQUARE_SIZE - 1) - row;
                int fillCol = (SQUARE_SIZE - 1) - col;
                int fillIndex = fillRow * SQUARE_SIZE + fillCol;
                line.append(fillIndex < filled ? '█' : '░');
            }
            System.out.println(line);
        }
    }

    public static void printProgress(int percent) {
        percent = Math.max(0, Math.min(100, percent));
        int cells = (percent * SQUARE_SIZE * SQUARE_SIZE) / 100;
        printWidthLine("  progress " + percent + "% (" + cells + "/"
                + (SQUARE_SIZE * SQUARE_SIZE) + " cells)");
        printSquare(cells);
    }

    public static void main(String[] args) {
        printComponent("CommonRails", 1234, 1, "printing initialized");
        printWidthLine("[START]   CommonRails Heritage printing initialized");
        printWidthLine("[WORKING] Content remains inside the 80-column contract");
        printWidthLine("[COMPLETE] Fixed-width output ready");
        printProgress(50);
    }
}
