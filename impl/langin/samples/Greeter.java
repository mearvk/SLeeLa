package com.mearvk.samples;

public class Greeter {
    private String name;
    private int count;

    public String greet(String who) {
        return "hello " + who;
    }

    public int times() {
        return count;
    }

    public void reset() {
    }
}
