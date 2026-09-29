package com.sleela.commonrails.heritage.printing;

import java.io.PrintWriter;
import java.util.Locale;

public final class CommonRailsPrinting {
  private CommonRailsPrinting() {}
  public static final class PrintLayout { public static final int WIDTH=80, OBJECT_ID_WIDTH=10, CURRENT_WIDTH=39, SIDE=21, CELLS=441; private PrintLayout(){} }
  public enum PrintState { START, WORKING, PROGRESS, COMPLETE, WARN, ERROR }
  public static final class PrintGlyphs { public static final String FULL="█", EMPTY="░"; private PrintGlyphs(){} }
  public static final class PrintField { public static String pad(String s,int width){ if(s.length()>=width)return s; return s+" ".repeat(width-s.length()); } }
  public static final class PrintLine { public static void write(PrintWriter out,String s){ int p=0; while(p<s.length()){ int rem=s.length()-p,n=Math.min(WIDTH,rem); if(rem>WIDTH){int c=n;while(c>0&&s.charAt(p+c-1)!=' '&&(p+c>=s.length()||s.charAt(p+c)!=' '))c--;if(c>0)n=c;} out.print(s.substring(p,p+n)); out.print(" ".repeat(WIDTH-n));out.println();p+=n;while(p<s.length()&&s.charAt(p)==' ')p++; } } }
  public static final class PrintProgress { public static int clamp(int p){return Math.max(0,Math.min(100,p));} public static int cells(int p){return clamp(p)*CELLS()/100;} public static int CELLS(){return PrintLayout.CELLS;} public static void write(PrintWriter out,int p){p=clamp(p);out.printf(Locale.ROOT,"  progress %d%% (%d/441 cells)%n",p,cells(p));} }
  public static final class PrintComponent { public static void write(PrintWriter out,String name,long id,long date,String message){PrintLine.write(out,String.format(Locale.ROOT,"-- : [Object ID: %010d] [Date: %d] [Current: @%s] . %s .",id,date,name,message));} }
  public static final class PrintFormatter { public static void status(PrintWriter out,PrintState state,String message){PrintLine.write(out,"["+state.name()+"] "+message);} }
  public static final class PrintRenderer { public static void square(PrintWriter out,int filled){filled=Math.max(0,Math.min(441,filled));for(int r=0;r<21;r++){for(int c=0;c<21;c++){int idx=(20-r)*21+(20-c);out.print(idx<filled?PrintGlyphs.FULL:PrintGlyphs.EMPTY);}out.println();}} }
  public static final class PrintWriterTarget { public final PrintWriter out; public PrintWriterTarget(PrintWriter out){this.out=out;} }
}
