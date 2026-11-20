// expect: overload-erasure
// float and double both store as a SLeeLa double, so these two constructors
// would lower to the same parameter list and SLeeLa could not tell them apart.
public class ConstructorErasure {
    String kind;
    public ConstructorErasure(float f) { kind = "float"; }
    public ConstructorErasure(double d) { kind = "double"; }
    public static void main(String[] args) {
        System.out.println(new ConstructorErasure(1.0f).kind);
    }
}
