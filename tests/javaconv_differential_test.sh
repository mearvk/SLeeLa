#!/usr/bin/env bash
# Differential test for the sleelactl Java -> SLeeLa converter.
#
# For every sleelactl/convert/tests/java/<Main>.java:
#   1. run it on a real JDK (javac + java)           -> expected output
#   2. convert it with javaconv                      -> SLeeLa source
#   3. run the converted program on the SLeeLa VM    -> actual output
#   4. require the two outputs to be identical.
# Also asserts that constructs the model does not cover FAIL CLOSED (exit 1, no output).
#
# usage: tests/javaconv_differential_test.sh [path-to-sleela] [path-to-javaconv]
set -u
set -o pipefail
here="$(cd "$(dirname "$0")" && pwd)"
root="$(cd "$here/.." && pwd)"
conv="${2:-$root/sleelactl/convert/build/javaconv}"
sleela="${1:-}"
if [ -z "$sleela" ]; then
  if [ -x "$root/impl/build/sleela" ]; then sleela="$root/impl/build/sleela"; else sleela="$root/working-binaries/sleela"; fi
fi
[ -x "$conv" ] || { echo "javaconv not built: make -C sleelactl/convert"; exit 2; }
command -v javac >/dev/null 2>&1 || { echo "SKIP: no JDK on PATH (javac)"; exit 0; }
export SLEELA_HOME="$root" SLEELA_LIB="$root/lib" SLEELA_SHEET="$root/SHEET.sheet"
export SLEELA_SHA256_MANIFEST="${SLEELA_SHA256_MANIFEST:-$root/security/sha256-manifest.json}"
tests="$root/sleelactl/convert/tests"
tmp="$(mktemp -d)"
trap 'rm -rf "$tmp"' EXIT
filter() { grep -v 'memory-manager\|^verification\|\[security\]\|\[defender\]'; }
fail=0; pass=0

for j in "$tests"/java/*.java; do
  name="$(basename "$j" .java)"
  mkdir -p "$tmp/$name/cls" "$tmp/$name/sl"
  if ! javac -d "$tmp/$name/cls" "$j" 2> "$tmp/$name/javac.err"; then echo "FAIL  $name (javac)"; cat "$tmp/$name/javac.err"; fail=$((fail+1)); continue; fi
  java -cp "$tmp/$name/cls" "$name" > "$tmp/$name/expected.out" 2>&1
  flags="$(sed -n 's|^// flags: ||p' "$j" | head -1)"
  # shellcheck disable=SC2086
  if ! "$conv" --file "$j" --out "$tmp/$name/sl" --quiet $flags; then
    echo "FAIL  $name (converter failed closed)"; "$conv" --file "$j" --out "$tmp/$name/sl" $flags | sed 's/^/      /'; fail=$((fail+1)); continue
  fi
  (cd "$tmp/$name/sl" && timeout 300 "$sleela" run "$name.sleela" 2>&1 | filter) > "$tmp/$name/actual.out"
  if diff -u "$tmp/$name/expected.out" "$tmp/$name/actual.out" > "$tmp/$name/diff"; then
    printf 'PASS  %-12s %s lines identical to the JDK\n' "$name" "$(wc -l < "$tmp/$name/expected.out")"; pass=$((pass+1))
  else
    printf 'FAIL  %s\n' "$name"; head -40 "$tmp/$name/diff"; fail=$((fail+1))
  fi
done

# fail-closed checks: each must be refused with a diagnostic, and must emit nothing
for j in "$tests"/reject/*.java; do
  [ -e "$j" ] || continue
  name="$(basename "$j" .java)"
  want="$(sed -n 's|^// expect: ||p' "$j" | head -1)"
  mkdir -p "$tmp/rej/$name"
  out="$("$conv" --file "$j" --out "$tmp/rej/$name" 2>&1)"; rc=$?
  produced="$(ls "$tmp/rej/$name"/*.sleela 2>/dev/null | wc -l)"
  if [ "$rc" -eq 1 ] && [ "$produced" -eq 0 ] && printf '%s' "$out" | grep -q "\[$want\]"; then
    printf 'PASS  %-12s refused [%s], no output written\n' "reject/$name" "$want"; pass=$((pass+1))
  else
    printf 'FAIL  reject/%s (rc=%s produced=%s wanted [%s])\n%s\n' "$name" "$rc" "$produced" "$want" "$out"; fail=$((fail+1))
  fi
done

# stale output: a file that converted on an earlier run and now fails closed must leave no output behind
mkdir -p "$tmp/stale/src"
printf 'public class Stale { public static void main(String[] a) { System.out.println(1); } }\n' > "$tmp/stale/src/Stale.java"
"$conv" --jdk "$tmp/stale/src" --out "$tmp/stale/out" --quiet
printf 'public class Stale { public static void main(String[] a) { Object o = new Object(); synchronized (o) { System.out.println(1); } } }\n' > "$tmp/stale/src/Stale.java"
"$conv" --jdk "$tmp/stale/src" --out "$tmp/stale/out" --quiet
if [ "$?" -eq 1 ] && [ ! -e "$tmp/stale/out/Stale.sleela" ]; then
  printf 'PASS  %-12s output of an earlier run removed when the file now fails closed\n' "stale-output"; pass=$((pass+1))
else
  printf 'FAIL  stale-output (a failed-closed file left Stale.sleela behind)\n'; fail=$((fail+1))
fi

# project-wide resolution: declarations are indexed before any source unit is translated
mkdir -p "$tmp/project/src" "$tmp/project/cls" "$tmp/project/sl"
cat > "$tmp/project/src/Widget.java" <<'JAVA'
public class Widget {
  int value() { return 23; }
}
JAVA
cat > "$tmp/project/src/Main.java" <<'JAVA'
public class Main {
  public static void main(String[] args) {
    Widget widget = new Widget();
    System.out.println(widget.value());
  }
}
JAVA
if javac -d "$tmp/project/cls" "$tmp/project/src/Widget.java" "$tmp/project/src/Main.java" 2> "$tmp/project/javac.err" &&
   java -cp "$tmp/project/cls" Main > "$tmp/project/expected.out" 2>&1 &&
   "$conv" --jdk "$tmp/project/src" --out "$tmp/project/sl" --quiet; then
  (cd "$tmp/project/sl" && timeout 300 "$sleela" run Main.sleela 2>&1 | filter) > "$tmp/project/actual.out"
  if diff -u "$tmp/project/expected.out" "$tmp/project/actual.out" > "$tmp/project/diff"; then
    printf 'PASS  %-12s cross-file type and member resolution identical to the JDK\n' "project-index"; pass=$((pass+1))
  else
    printf 'FAIL  project-index\n'; head -40 "$tmp/project/diff"; fail=$((fail+1))
  fi
else
  printf 'FAIL  project-index (project build or conversion failed)\n'; cat "$tmp/project/javac.err"; fail=$((fail+1))
fi

# cross-file generic substitution: a type declared in another source unit keeps
# its class type argument through field writes, method returns, and call resolution.
mkdir -p "$tmp/project-generic/src" "$tmp/project-generic/cls" "$tmp/project-generic/sl"
cat > "$tmp/project-generic/src/Box.java" <<'JAVA'
public class Box<T> {
  private T value;
  public void set(T value) { this.value = value; }
  public T get() { return value; }
}
JAVA
cat > "$tmp/project-generic/src/Main.java" <<'JAVA'
public class Main {
  public static void main(String[] args) {
    Box<String> box = new Box<String>();
    box.set("project-generic");
    System.out.println(box.get());
  }
}
JAVA
if javac -d "$tmp/project-generic/cls" "$tmp/project-generic/src/Box.java" "$tmp/project-generic/src/Main.java" 2> "$tmp/project-generic/javac.err" &&
   java -cp "$tmp/project-generic/cls" Main > "$tmp/project-generic/expected.out" 2>&1 &&
   "$conv" --jdk "$tmp/project-generic/src" --out "$tmp/project-generic/sl" --quiet; then
  (cd "$tmp/project-generic/sl" && timeout 300 "$sleela" run Main.sleela 2>&1 | filter) > "$tmp/project-generic/actual.out"
  if diff -u "$tmp/project-generic/expected.out" "$tmp/project-generic/actual.out" > "$tmp/project-generic/diff"; then
    printf 'PASS  %-12s cross-file class-generic substitution identical to the JDK\n' "project-generic"; pass=$((pass+1))
  else
    printf 'FAIL  project-generic\n'; head -40 "$tmp/project-generic/diff"; fail=$((fail+1))
  fi
else
  printf 'FAIL  project-generic (project build or conversion failed)\n'; cat "$tmp/project-generic/javac.err"; fail=$((fail+1))
fi

# package identity and wildcard/nested imports across source packages.
mkdir -p "$tmp/project-package/src/fixture/alpha" "$tmp/project-package/src/fixture/use" "$tmp/project-package/cls" "$tmp/project-package/sl"
cat > "$tmp/project-package/src/fixture/alpha/AlphaBase.java" <<'JAVA'
package fixture.alpha;
public class AlphaBase {
  public int value() { return 20; }
  public static class Nested {
    public int value() { return 22; }
  }
}
JAVA
cat > "$tmp/project-package/src/fixture/use/Main.java" <<'JAVA'
package fixture.use;
import fixture.alpha.*;
import fixture.alpha.AlphaBase.Nested;
public class Main {
  public static void main(String[] args) {
    AlphaBase base = new AlphaBase();
    Nested nested = new Nested();
    System.out.println(base.value() + nested.value());
  }
}
JAVA
if javac -d "$tmp/project-package/cls" "$tmp/project-package/src/fixture/alpha/AlphaBase.java" "$tmp/project-package/src/fixture/use/Main.java" 2> "$tmp/project-package/javac.err" &&
   java -cp "$tmp/project-package/cls" fixture.use.Main > "$tmp/project-package/expected.out" 2>&1 &&
   "$conv" --jdk "$tmp/project-package/src" --out "$tmp/project-package/sl" --quiet; then
  (cd "$tmp/project-package/sl" && timeout 300 "$sleela" run fixture/use/Main.sleela 2>&1 | filter) > "$tmp/project-package/actual.out"
  if diff -u "$tmp/project-package/expected.out" "$tmp/project-package/actual.out" > "$tmp/project-package/diff"; then
    printf 'PASS  %-12s package-qualified project index, wildcard import and nested single-type import match the JDK\n' "package-import"; pass=$((pass+1))
  else
    printf 'FAIL  project-package\n'; head -40 "$tmp/project-package/diff"; fail=$((fail+1))
  fi
else
  printf 'FAIL  project-package (project build or conversion failed)\n'; cat "$tmp/project-package/javac.err"; fail=$((fail+1))
fi

# protected inherited members remain accessible to a cross-package subclass.
mkdir -p "$tmp/project-protected/src/fixture/alpha" "$tmp/project-protected/src/fixture/child" "$tmp/project-protected/src/fixture/use" "$tmp/project-protected/cls" "$tmp/project-protected/sl"
cat > "$tmp/project-protected/src/fixture/alpha/Base.java" <<'JAVA'
package fixture.alpha;
public class Base {
  protected int value() { return 31; }
}
JAVA
cat > "$tmp/project-protected/src/fixture/child/Child.java" <<'JAVA'
package fixture.child;
import fixture.alpha.Base;
public class Child extends Base {
  public int expose() { return value(); }
}
JAVA
cat > "$tmp/project-protected/src/fixture/use/Main.java" <<'JAVA'
package fixture.use;
import fixture.child.Child;
public class Main {
  public static void main(String[] args) {
    System.out.println(new Child().expose());
  }
}
JAVA
if javac -d "$tmp/project-protected/cls" "$tmp/project-protected/src/fixture/alpha/Base.java" "$tmp/project-protected/src/fixture/child/Child.java" "$tmp/project-protected/src/fixture/use/Main.java" 2> "$tmp/project-protected/javac.err" &&
   java -cp "$tmp/project-protected/cls" fixture.use.Main > "$tmp/project-protected/expected.out" 2>&1 &&
   "$conv" --jdk "$tmp/project-protected/src" --out "$tmp/project-protected/sl" --quiet; then
  (cd "$tmp/project-protected/sl" && timeout 300 "$sleela" run fixture/use/Main.sleela 2>&1 | filter) > "$tmp/project-protected/actual.out"
  if diff -u "$tmp/project-protected/expected.out" "$tmp/project-protected/actual.out" > "$tmp/project-protected/diff"; then
    printf 'PASS  %-12s protected inherited member access in subclass matches the JDK\n' "protected-access"; pass=$((pass+1))
  else
    printf 'FAIL  project-protected\n'; head -40 "$tmp/project-protected/diff"; fail=$((fail+1))
  fi
else
  printf 'FAIL  project-protected (project build or conversion failed)\n'; cat "$tmp/project-protected/javac.err"; fail=$((fail+1))
fi

# package-private members and top-level types are not visible across packages.
mkdir -p "$tmp/project-access/src/fixture/alpha" "$tmp/project-access/src/fixture/use" "$tmp/project-access/out"
cat > "$tmp/project-access/src/fixture/alpha/Visible.java" <<'JAVA'
package fixture.alpha;
public class Visible {
  int hidden = 7;
  public Visible() {}
}
JAVA
cat > "$tmp/project-access/src/fixture/use/Main.java" <<'JAVA'
package fixture.use;
import fixture.alpha.Visible;
public class Main {
  public static void main(String[] args) {
    Visible value = new Visible();
    System.out.println(value.hidden);
  }
}
JAVA
if javac -d "$tmp/project-access/cls" "$tmp/project-access/src/fixture/alpha/Visible.java" "$tmp/project-access/src/fixture/use/Main.java" 2> "$tmp/project-access/javac.err"; then
  printf 'FAIL  project-access (javac unexpectedly accepted package-private field access)\n'; fail=$((fail+1))
else
  access_out="$("$conv" --jdk "$tmp/project-access/src" --out "$tmp/project-access/out" --quiet 2>&1)"
  access_rc=$?
  if [ "$access_rc" -eq 1 ] && printf '%s' "$access_out" | grep -q "\[member-access\]" && [ "$(find "$tmp/project-access/out" -name '*.sleela' | wc -l)" -eq 0 ]; then
    printf 'PASS  %-12s cross-package package-private field access refused with no output\n' "member-access"; pass=$((pass+1))
  else
    printf 'FAIL  project-access (rc=%s)\n%s\n' "$access_rc" "$access_out"; fail=$((fail+1))
  fi
fi

# duplicate simple declarations must never be resolved by source traversal order
mkdir -p "$tmp/duplicate/src/a" "$tmp/duplicate/src/b" "$tmp/duplicate/out"
cat > "$tmp/duplicate/src/a/First.java" <<'JAVA'
class Clash { int first() { return 1; } }
JAVA
cat > "$tmp/duplicate/src/b/Second.java" <<'JAVA'
class Clash { int second() { return 2; } }
JAVA
cat > "$tmp/duplicate/src/Use.java" <<'JAVA'
class Use { Clash value; }
JAVA
duplicate_out="$("$conv" --jdk "$tmp/duplicate/src" --out "$tmp/duplicate/out" --quiet 2>&1)"
duplicate_rc=$?
if [ "$duplicate_rc" -eq 1 ] && printf '%s' "$duplicate_out" | grep -q "\[type-ambiguous\]"; then
  printf 'PASS  %-12s ambiguous project-wide simple name refused\n' "duplicate-type"; pass=$((pass+1))
else
  printf 'FAIL  duplicate-type (rc=%s)\n%s\n' "$duplicate_rc" "$duplicate_out"; fail=$((fail+1))
fi

# project-wide generic inheritance: type arguments must be substituted through
# a cross-file superclass and interface edge before inherited calls are emitted.
mkdir -p "$tmp/project-inherited/src" "$tmp/project-inherited/cls" "$tmp/project-inherited/sl"
cat > "$tmp/project-inherited/src/Value.java" <<'JAVA'
public interface Value<T> { T get(); }
JAVA
cat > "$tmp/project-inherited/src/Base.java" <<'JAVA'
public class Base<T> implements Value<T> {
  private T value;
  public Base(T value) { this.value = value; }
  public T get() { return value; }
}
JAVA
cat > "$tmp/project-inherited/src/Mid.java" <<'JAVA'
public class Mid<T> extends Base<T> {
  public Mid(T value) { super(value); }
}
JAVA
cat > "$tmp/project-inherited/src/TextValue.java" <<'JAVA'
public class TextValue extends Mid<String> {
  public TextValue(String value) { super(value); }
}
JAVA
cat > "$tmp/project-inherited/src/Main.java" <<'JAVA'
public class Main {
  public static void main(String[] args) {
    Value<String> value = new TextValue("project-inherited");
    System.out.println(value.get());
  }
}
JAVA
if javac -d "$tmp/project-inherited/cls" "$tmp/project-inherited/src/"*.java 2> "$tmp/project-inherited/javac.err" &&
   java -cp "$tmp/project-inherited/cls" Main > "$tmp/project-inherited/expected.out" 2>&1 &&
   "$conv" --jdk "$tmp/project-inherited/src" --out "$tmp/project-inherited/sl" --quiet; then
  (cd "$tmp/project-inherited/sl" && timeout 300 "$sleela" run Main.sleela 2>&1 | filter) > "$tmp/project-inherited/actual.out"
  if diff -u "$tmp/project-inherited/expected.out" "$tmp/project-inherited/actual.out" > "$tmp/project-inherited/diff"; then
    printf 'PASS  %-12s cross-file generic superclass/interface substitution identical to the JDK\n' "project-inherited"; pass=$((pass+1))
  else
    printf 'FAIL  project-inherited\n'; head -40 "$tmp/project-inherited/diff"; fail=$((fail+1))
  fi
else
  printf 'FAIL  project-inherited (project build or conversion failed)\n'; cat "$tmp/project-inherited/javac.err"; fail=$((fail+1))
fi

echo "javaconv differential: pass=$pass fail=$fail"
[ "$fail" -eq 0 ]
