package com.mearvk.sleela.connector;

/** Small public-field fixture for SLJIP reflection tests. */
public final class SleelaJdkProtocolFixture {
    public int counter;
    public static int sharedCounter;

    public SleelaJdkProtocolFixture() {
        this.counter = 3;
    }
}
