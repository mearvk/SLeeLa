// expect: print-stream
// SLeeLa print always ends the line and writes stdout; System.out.print (no newline) and anything
// on System.err have no SLeeLa form, so they are refused instead of being emitted unchanged.
public class PrintStreamMember {
    public static void main(String[] args) {
        System.out.print("no newline");
        System.out.println();
    }
}
