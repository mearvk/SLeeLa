import java.util.Locale;
public final class ArithmeticSpeed {
 static final int N=4096, ITERS=2_000_000; static int state=0x53435350; static final int BASE_SEED=Integer.decode(System.getenv().getOrDefault("SLEELA_BENCH_SEED","0x53435350")); static boolean quiet=false;
 static long next(){state^=state<<13;state^=state>>>17;state^=state<<5;return Integer.toUnsignedLong(state);}
 static void emit(String t,char o,long s,long sum){if(!quiet){long ns=System.nanoTime()-s;System.out.printf(Locale.ROOT,"%s,%c,%d,%.3f,%d%n",t,o,ITERS,(double)ns/ITERS,sum);}}
 static void runInt(String t,int bits,boolean signed,char op,long[] a,long[] b){
  long mask=bits==64?-1L:(1L<<bits)-1L,sum=0,start=System.nanoTime();
  for(int i=0;i<ITERS;i++){int k=i&(N-1);long x;
   switch(op){case '+':x=a[k]+b[k];break;case '-':x=a[k]-b[k];break;case '*':x=a[k]*b[k];break;default:x=(bits==64&&!signed)?Long.divideUnsigned(a[k],b[k]):a[k]/b[k];}
   x&=mask;if(signed&&bits<64&&(x&(1L<<(bits-1)))!=0)x|=~mask;sum^=x;
  }emit(t,op,start,sum);
 }
 static void runF32(char op,float[] a,float[] b){long sum=0,start=System.nanoTime();for(int i=0;i<ITERS;i++){int k=i&(N-1);float x=op=='+'?a[k]+b[k]:op=='-'?a[k]-b[k]:op=='*'?a[k]*b[k]:a[k]/b[k];sum^=(long)(x*1_000_000.0);}emit("f32",op,start,sum);}
 static void runF64(char op,double[] a,double[] b){long sum=0,start=System.nanoTime();for(int i=0;i<ITERS;i++){int k=i&(N-1);double x=op=='+'?a[k]+b[k]:op=='-'?a[k]-b[k]:op=='*'?a[k]*b[k]:a[k]/b[k];sum^=(long)(x*1_000_000.0);}emit("f64",op,start,sum);}
 static void intType(String n,int bits,boolean signed){state=BASE_SEED;long[] a=new long[N],b=new long[N];for(int i=0;i<N;i++){a[i]=5+Long.remainderUnsigned(next(),6);b[i]=1+Long.remainderUnsigned(next(),5);}for(char o:new char[]{'+','-','*','/'})runInt(n,bits,signed,o,a,b);}
 static void sweep(){
  intType("i8",8,true);intType("u8",8,false);intType("i16",16,true);intType("u16",16,false);intType("i32",32,true);intType("u32",32,false);intType("i64",64,true);intType("u64",64,false);
  state=BASE_SEED;float[] a=new float[N],b=new float[N];double[] c=new double[N],d=new double[N];for(int i=0;i<N;i++){float x=5+(next()%6),y=1+(next()%5);a[i]=x;b[i]=y;c[i]=x;d[i]=y;}
  for(char o:new char[]{'+','-','*','/'})runF32(o,a,b);for(char o:new char[]{'+','-','*','/'})runF64(o,c,d);
 }
 public static void main(String[] z){System.out.println("type,operator,operations,ns_per_operation,checksum");for(int i=0;i<3;i++){quiet=true;sweep();}quiet=false;sweep();}
}