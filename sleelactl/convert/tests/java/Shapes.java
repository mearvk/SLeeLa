interface Shape { double area(); String name(); }
class Rect implements Shape {
    int w; int h;
    Rect(int w, int h) { this.w = w; this.h = h; }
    public double area() { return w * h; }
    public String name() { return "rect"; }
}
class Square extends Rect {
    Square(int s) { super(s, s); }
    public String name() { return "square"; }
}
class Counter {
    static int made = 0;
    int id;
    Counter() { made++; id = made; }
    int next() { id = id + 1; return id; }
}
public class Shapes {
    static int sum(int[] a) { int s = 0; for (int i = 0; i < a.length; i++) { s += a[i]; } return s; }
    public static void main(String[] args) {
        Shape a = new Rect(3, 4);
        Shape b = new Square(5);
        System.out.println(a.name() + " " + a.area());
        System.out.println(b.name() + " " + b.area());
        System.out.println(b instanceof Rect);
        Counter c1 = new Counter();
        Counter c2 = new Counter();
        System.out.println(Counter.made + " " + c1.id + " " + c2.id + " " + c2.next());
        int[] arr = new int[5];
        for (int i = 0; i < arr.length; i++) { arr[i] = i * i; }
        System.out.println(sum(arr));
        Rect r = (Rect) b;
        System.out.println(r.w + r.h);
        Object o = null;
        System.out.println(o == null);
    }
}
