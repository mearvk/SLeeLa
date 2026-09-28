package com.mearvk.sleela.synchro;
import java.util.ArrayDeque; import java.util.Arrays; import java.util.Deque;
public final class SynchroStats {
 private final String destination; private final int window; private final Deque<Double> rtts=new ArrayDeque<>();
 private long sent,acked,lost; private double sum,sumsq;
 public SynchroStats(String d){this(d,1024);} public SynchroStats(String d,int w){if(w<=0)throw new IllegalArgumentException("window");destination=d;window=w;}
 public synchronized void record(Double r){sent++;if(r==null){lost++;return;}acked++;double v=r;rtts.addLast(v);sum+=v;sumsq+=v*v;if(rtts.size()>window){double o=rtts.removeFirst();sum-=o;sumsq-=o*o;}}
 public synchronized long sent(){return sent;} public synchronized long acked(){return acked;} public synchronized long lost(){return lost;}
 public synchronized double lossRate(){return sent==0?0:(double)lost/sent;} public synchronized double deliveryRate(){return sent==0?0:(double)acked/sent;}
 public synchronized int samples(){return rtts.size();} public synchronized Double percentile(double p){if(rtts.isEmpty())return null;if(p<0||p>100)throw new IllegalArgumentException("percentile");double[]a=rtts.stream().mapToDouble(Double::doubleValue).toArray();Arrays.sort(a);if(p==0)return a[0];int rank=(int)Math.ceil(p/100*a.length);return a[Math.min(rank,a.length)-1];}
 public synchronized Double min(){return rtts.stream().min(Double::compare).orElse(null);} public synchronized Double max(){return rtts.stream().max(Double::compare).orElse(null);}
 public synchronized Double mean(){return rtts.isEmpty()?null:sum/rtts.size();} public synchronized Double jitter(){if(rtts.size()<2)return null;return Math.sqrt(Math.max(0,(sumsq-sum*sum/rtts.size())/(rtts.size()-1)));}
 public String destination(){return destination;}
}