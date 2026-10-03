import com.sleela.commonrails.heritage.printing.CommonRailsPrinting.PrintProgress;
public final class PrintContractTest { public static void main(String[] args){assert PrintProgress.clamp(-1)==0;assert PrintProgress.clamp(101)==100;assert PrintProgress.cells(0)==0;assert PrintProgress.cells(50)==220;assert PrintProgress.cells(100)==441;} }
