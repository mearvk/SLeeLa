// for loops with several declarators, several initializer expressions and several updates
// (JLS 14.14.1): init runs once in order; each pass tests the condition, runs the body, then
// every update in order, including after continue. Labeled continue/break target the loop.
public class ForLoops {
    public static void main(String[] args) {
        int sum = 0;
        for (int i = 0, j = 10; i < j; i++) sum += i * j;
        System.out.println(sum);

        StringBuilder sb = new StringBuilder();
        for (int i = 0, j = 5; i < j; i++, j--) sb.append(i).append(':').append(j).append(' ');
        System.out.println(sb);

        int a, b;
        for (a = 1, b = 1; a < 50; a += b, b = a - b) { }
        System.out.println(a + " " + b);

        int odd = 0, steps = 0;
        for (int i = 0, k = 100; i < 20; i++, k -= 3) {
            steps++;
            if (i % 2 == 0) continue;
            if (k < 60) break;
            odd += i;
        }
        System.out.println(odd + " " + steps);

        int hits = 0;
        outer:
        for (int i = 0, n = 4; i < n; i++, n += 0) {
            for (int j = 0; j < 4; j++) {
                if (j > i) continue outer;
                if (i == 3 && j == 2) break outer;
                hits++;
            }
        }
        System.out.println(hits);

        for (int i = 7, j = 1; j < 3; j++) sum += i;
        System.out.println(sum);
    }
}
