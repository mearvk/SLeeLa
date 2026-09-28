package com.mearvk.sleela.synchro;
import java.util.Map;
public final class SlaReporter {
 public record Result(double thresholdMs,double percentile,int destinations,int meeting,long samples,long samplesWithin){
  public double destinationCompliance(){return destinations==0?0:(double)meeting/destinations;}
  public double sampleCompliance(){return samples==0?0:(double)samplesWithin/samples;}
  public String summary(){return String.format("Synchro SLA report (measured, not guaranteed)%n threshold : %.3f ms%n evaluated at : p%.0f%n destinations : %d/%d (%.2f%%)%n samples within threshold : %d/%d (%.2f%%)",thresholdMs,percentile,meeting,destinations,destinationCompliance()*100,samplesWithin,samples,sampleCompliance()*100);}
 }
 public static Result evaluate(Map<String,SynchroStats>m,double threshold,double p){if(threshold<=0||p<0||p>100)throw new IllegalArgumentException();int meet=0;long n=0,within=0;for(SynchroStats s:m.values()){Double q=s.percentile(p);if(q!=null&&q<=threshold)meet++;n+=s.samples();within+=s.samplesWithin(threshold);}return new Result(threshold,p,m.size(),meet,n,within);}
}