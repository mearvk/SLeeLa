package com.mearvk.sleela.munction;

import java.nio.file.Path;

/**
 * Minimal launcher demonstrating Munction™ sentences over several system
 * methods. Each run is one sane reach (4..16 calls) that closes with a receipt.
 */
public final class MunctionDemo {
    private MunctionDemo() {
    }

    public static void main(String[] args) {
        // 1) File I/O reach (4 calls, the minimum sane sentence).
        Path log = Path.of(System.getProperty("java.io.tmpdir"), "munction-demo.log");
        MunctionReceipt fileReceipt = Munction.start("audit")
            .connect("file:" + log)
            .send("entry-1\n")
            .closeWithReceipt();
        System.out.println(fileReceipt);

        // 2) Private-packet reach (the canonical sentence, 8 calls).
        Interims interims = Interims.of("frame", d -> d).stage("checksum", d -> d);
        MunctionReceipt sdps = Munction.start("xyx")
            .connect("sdps://tmcf:19866")
            .send("payload".getBytes())
            .thatch(interims)
            .consume()
            .consume()
            .latch()
            .closeWithReceipt();
        System.out.println(sdps);

        // 3) Crypto-over-pipe reach (seal on send, open on consume; 7 calls).
        MunctionReceipt crypto = Munction.start("sealed")
            .connect("crypto:pipe:/tmp/munction.fifo")
            .enable("crypto:kref-2026")
            .send("secret")
            .consume()
            .latch()
            .closeWithReceipt();
        System.out.println(crypto);
    }
}
