package org.sleela.slvm;
import java.io.*;
import java.util.concurrent.atomic.AtomicLong;
/** Reference JVM-side contract for the SLVM object broker. */
public final class SlvmObjectBroker {
    public static final int MAGIC=0x534C4A56, VERSION=1;
    private final DataInputStream in; private final DataOutputStream out; private final AtomicLong ids=new AtomicLong(1);
    public SlvmObjectBroker(DataInputStream in,DataOutputStream out){this.in=in;this.out=out;}
    public synchronized long declareGui(String className)throws IOException{long id=ids.getAndIncrement();byte[] n=className.getBytes(java.nio.charset.StandardCharsets.UTF_8);writeFrame(5,id,n);return id;}
    public synchronized void writeFrame(int type,long objectId,byte[] payload)throws IOException{
        out.writeInt(MAGIC);out.writeShort(VERSION);out.writeShort(type);out.writeInt(0);out.writeLong(ids.get());out.writeLong(objectId);out.writeLong(payload.length);out.write(payload);out.flush();
    }
    public void readFrame()throws IOException{int m=in.readInt();if(m!=MAGIC)throw new IOException("Invalid SLVM broker magic");int v=in.readUnsignedShort();if(v!=VERSION)throw new IOException("Unsupported SLVM broker version");in.readUnsignedShort();in.readInt();in.readLong();in.readLong();long n=in.readLong();if(n<0||n>Integer.MAX_VALUE)throw new IOException("Invalid broker payload size");in.readNBytes((int)n);}
}