package com.mearvk.sleela.java28;

/**
 * A second allow-listed demo object: a running balance. Demonstrates a method
 * that returns another heap object (a Counter), which the link exposes to
 * Sleela as a fresh handle.
 */
public final class Ledger {
    public double balance;

    public Ledger() { this.balance = 0.0; }
    public Ledger(double opening) { this.balance = opening; }

    public double credit(double amount) { balance += amount; return balance; }
    public double debit(double amount) { balance -= amount; return balance; }
    public double getBalance() { return balance; }

    /** Returns a NEW heap object; the memory host will hand Sleela a handle. */
    public Counter cents() { return new Counter(Math.round(balance * 100)); }
}
