package com.mearvk.sleela.connector;

/** Small public-field fixture for SLJIP reflection tests. */
public final class SleelaJdkProtocolFixture {
    public int counter;
    public static int sharedCounter;

    public static long plusOne(long value) { return value + 1L; }
    public static float scale(float value) { return value * 2.0f; }
    public static char echo(char value) { return value; }

    public SleelaJdkProtocolFixture() {
        this.counter = 3;
    }
}
