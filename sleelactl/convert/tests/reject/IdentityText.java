// expect: object-text
// A user class without toString() shown as text prints an identity hash in Java (Plain@1b6d3586):
// no equivalent output exists, so the converter refuses it instead of printing a VM handle.
public class IdentityText {
    static class Plain { int v = 1; }
    public static void main(String[] args) {
        System.out.println(new Plain());
    }
}
