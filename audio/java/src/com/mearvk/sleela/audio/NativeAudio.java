package com.mearvk.sleela.audio;
import java.nio.file.Path;import java.util.*;
public final class NativeAudio implements Audio {
 private final Path executable;
 public NativeAudio(Path executable){this.executable=executable.toAbsolutePath().normalize();}
 public boolean validate(Configuration c){return c!=null&&c.sampleRate()>0&&c.inputs()!=null&&!c.inputs().isEmpty()&&c.inputs().size()<=128&&c.output()!=null&&c.inputs().stream().allMatch(i->i!=null&&i.source()!=null&&!i.source().isBlank()&&i.startSeconds()>=0&&Double.isFinite(i.gainDb()));}
 public void process(Configuration c){if(!validate(c))throw new IllegalArgumentException("Invalid audio configuration");List<String>x=new ArrayList<>();x.add(executable.toString());x.add(c.output().toString());c.inputs().forEach(i->x.add(i.source()));try{int n=new ProcessBuilder(x).inheritIO().start().waitFor();if(n!=0)throw new IllegalStateException("Native audio exit "+n);}catch(Exception e){if(e instanceof InterruptedException)Thread.currentThread().interrupt();throw new IllegalStateException("Native audio processing failed",e);}}
}
