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
    fixture.alpha.AlphaBase qualifiedBase = new fixture.alpha.AlphaBase();
    fixture.alpha.AlphaBase.Nested qualifiedNested = new fixture.alpha.AlphaBase.Nested();
    System.out.println(base.value() + nested.value());
    System.out.println(qualifiedBase.value() + qualifiedNested.value());
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

# static imports may import member types as well as static fields/methods.
# Exercise both on-demand and single-member forms, using fully qualified owner identity.
mkdir -p "$tmp/project-static-nested/src/fixture/alpha" "$tmp/project-static-nested/src/fixture/use" "$tmp/project-static-nested/cls" "$tmp/project-static-nested/sl"
cat > "$tmp/project-static-nested/src/fixture/alpha/Container.java" <<'JAVA'
package fixture.alpha;
public class Container {
  public static class Entry {
    public int value() { return 17; }
  }
  public static class Token {
    public int value() { return 19; }
  }
}
JAVA
cat > "$tmp/project-static-nested/src/fixture/use/WildcardMain.java" <<'JAVA'
package fixture.use;
import static fixture.alpha.Container.*;
public class WildcardMain {
  public static void main(String[] args) {
    Entry entry = new Entry();
    Token token = new Token();
    System.out.println(entry.value() + token.value());
  }
}
JAVA
cat > "$tmp/project-static-nested/src/fixture/use/SingleMain.java" <<'JAVA'
package fixture.use;
import static fixture.alpha.Container.Entry;
public class SingleMain {
  public static void main(String[] args) {
    Entry entry = new Entry();
    System.out.println(entry.value());
  }
}
JAVA
if javac -d "$tmp/project-static-nested/cls" "$tmp/project-static-nested/src/fixture/alpha/Container.java" "$tmp/project-static-nested/src/fixture/use/WildcardMain.java" "$tmp/project-static-nested/src/fixture/use/SingleMain.java" 2> "$tmp/project-static-nested/javac.err" &&
   java -cp "$tmp/project-static-nested/cls" fixture.use.WildcardMain > "$tmp/project-static-nested/expected-wildcard.out" 2>&1 &&
   java -cp "$tmp/project-static-nested/cls" fixture.use.SingleMain > "$tmp/project-static-nested/expected-single.out" 2>&1 &&
   "$conv" --jdk "$tmp/project-static-nested/src" --out "$tmp/project-static-nested/sl" --quiet; then
  (cd "$tmp/project-static-nested/sl" && timeout 300 "$sleela" run fixture/use/WildcardMain.sleela 2>&1 | filter) > "$tmp/project-static-nested/actual-wildcard.out"
  (cd "$tmp/project-static-nested/sl" && timeout 300 "$sleela" run fixture/use/SingleMain.sleela 2>&1 | filter) > "$tmp/project-static-nested/actual-single.out"
  if diff -u "$tmp/project-static-nested/expected-wildcard.out" "$tmp/project-static-nested/actual-wildcard.out" > "$tmp/project-static-nested/diff-wildcard" &&
     diff -u "$tmp/project-static-nested/expected-single.out" "$tmp/project-static-nested/actual-single.out" > "$tmp/project-static-nested/diff-single"; then
    printf 'PASS  %-12s static nested wildcard and single-type imports match the JDK\n' "static-nested"; pass=$((pass+1))
  else
    printf 'FAIL  project-static-nested\n'; head -40 "$tmp/project-static-nested/diff-wildcard"; head -40 "$tmp/project-static-nested/diff-single"; fail=$((fail+1))
  fi
else
  printf 'FAIL  project-static-nested (javac or conversion failed)\n'; cat "$tmp/project-static-nested/javac.err"; fail=$((fail+1))
fi

# protected inherited members remain accessible to a cross-package subclass.
mkdir -p "$tmp/project-protected/src/fixture/alpha" "$tmp/project-protected/src/fixture/child" "$tmp/project-protected/src/fixture/use" "$tmp/project-protected/cls" "$tmp/project-protected/sl"
cat > "$tmp/project-protected/src/fixture/alpha/Base.java" <<'JAVA'
package fixture.alpha;
public class Base {
  protected int value() { return 31; }
  protected int data = 31;
}
JAVA
cat > "$tmp/project-protected/src/fixture/child/Child.java" <<'JAVA'
package fixture.child;
import fixture.alpha.Base;
public class Child extends Base {
  public int expose() { return value(); }
  public int exposeData(Child other) { return data + other.data; }
}
JAVA
cat > "$tmp/project-protected/src/fixture/use/Main.java" <<'JAVA'
package fixture.use;
import fixture.child.Child;
public class Main {
  public static void main(String[] args) {
    System.out.println(new Child().expose());
    System.out.println(new Child().exposeData(new Child()));
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

# a subclass in another package may not select a protected instance field
# through a Base-typed receiver (JLS 6.6.2.1), even though it may use this
# or a receiver whose static type is the subclass.
mkdir -p "$tmp/project-protected-receiver/src/fixture/alpha" "$tmp/project-protected-receiver/src/fixture/child" "$tmp/project-protected-receiver/cls" "$tmp/project-protected-receiver/out"
cat > "$tmp/project-protected-receiver/src/fixture/alpha/Base.java" <<'JAVA'
package fixture.alpha;
public class Base { protected int data = 31; }
JAVA
cat > "$tmp/project-protected-receiver/src/fixture/child/ChildBad.java" <<'JAVA'
package fixture.child;
import fixture.alpha.Base;
public class ChildBad extends Base {
  public int invalid(Base other) { return other.data; }
}
JAVA
if javac -d "$tmp/project-protected-receiver/cls" "$tmp/project-protected-receiver/src/fixture/alpha/Base.java" "$tmp/project-protected-receiver/src/fixture/child/ChildBad.java" 2> "$tmp/project-protected-receiver/javac.err"; then
  printf 'FAIL  protected-receiver (javac unexpectedly accepted Base-typed protected field receiver)\n'; fail=$((fail+1))
else
  receiver_out="$("$conv" --jdk "$tmp/project-protected-receiver/src" --out "$tmp/project-protected-receiver/out" --quiet 2>&1)"
  receiver_rc=$?
  if [ "$receiver_rc" -eq 1 ] && printf '%s' "$receiver_out" | grep -q "\\[member-access\\]" && [ "$(find "$tmp/project-protected-receiver/out" -name '*.sleela' | wc -l)" -eq 0 ]; then
    printf 'PASS  %-12s cross-package protected field through Base receiver refused with no output\n' "protected-receiver"; pass=$((pass+1))
  else
    printf 'FAIL  protected-receiver (rc=%s)\n%s\n' "$receiver_rc" "$receiver_out"; fail=$((fail+1))
  fi
fi

# package-private members and top-level types are not visible across packages.
mkdir -p "$tmp/project-access/src/fixture/alpha" "$tmp/project-access/src/fixture/use" "$tmp/project-access/cls" "$tmp/project-access/out"
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

# package-private top-level types cannot be imported from another package.
mkdir -p "$tmp/project-type-access/src/fixture/alpha" "$tmp/project-type-access/src/fixture/use" "$tmp/project-type-access/cls" "$tmp/project-type-access/out"
cat > "$tmp/project-type-access/src/fixture/alpha/Hidden.java" <<'JAVA'
package fixture.alpha;
class Hidden { public int value() { return 1; } }
JAVA
cat > "$tmp/project-type-access/src/fixture/use/Main.java" <<'JAVA'
package fixture.use;
import fixture.alpha.Hidden;
public class Main { public static void main(String[] args) { System.out.println(new Hidden().value()); } }
JAVA
if javac -d "$tmp/project-type-access/cls" "$tmp/project-type-access/src/fixture/alpha/Hidden.java" "$tmp/project-type-access/src/fixture/use/Main.java" 2> "$tmp/project-type-access/javac.err"; then
  printf 'FAIL  project-type-access (javac unexpectedly accepted a non-public imported type)\n'; fail=$((fail+1))
else
  type_out="$("$conv" --jdk "$tmp/project-type-access/src" --out "$tmp/project-type-access/out" --quiet 2>&1)"
  type_rc=$?
  if [ "$type_rc" -eq 1 ] && printf '%s' "$type_out" | grep -q "\[type-access\]" && [ "$(find "$tmp/project-type-access/out" -name '*.sleela' | wc -l)" -eq 0 ]; then
    printf 'PASS  %-12s non-public cross-package type import refused with no output\n' "type-access"; pass=$((pass+1))
  else
    printf 'FAIL  project-type-access (rc=%s)\n%s\n' "$type_rc" "$type_out"; fail=$((fail+1))
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

# transactional publication: a late collision after an earlier staged file was
# rendered must not alter the previously published generation or its ownership list.
mkdir -p "$tmp/transaction/src" "$tmp/transaction/out"
cat > "$tmp/transaction/src/Main.java" <<'JAVA'
public class Main { public static void main(String[] args) { System.out.println(1); } }
JAVA
if "$conv" --jdk "$tmp/transaction/src" --out "$tmp/transaction/out" --quiet; then
  if ! python3 "$root/sleelactl/convert/verify-provenance.py" "$tmp/transaction/out/CONVERSION-PROVENANCE.json" "$root" "$tmp/transaction/src" "$tmp/transaction/out"; then
    printf 'FAIL  %-12s independent provenance validation failed\n' "provenance"; fail=$((fail+1))
  else
    printf 'PASS  %-12s source/model/converter/output SHA-256 provenance independently verified\n' "provenance"; pass=$((pass+1))
  fi
  find "$tmp/transaction/out" -type f -printf '%P\\t%T@\n' | sort > "$tmp/transaction/mtime.before"
  if "$conv" --jdk "$tmp/transaction/src" --out "$tmp/transaction/out" --quiet &&
     find "$tmp/transaction/out" -type f -printf '%P\\t%T@\n' | sort > "$tmp/transaction/mtime.after" &&
     diff -u "$tmp/transaction/mtime.before" "$tmp/transaction/mtime.after" >/dev/null; then
    printf 'PASS  %-12s repeat conversion preserves every output mtime\n' "repeat-run"; pass=$((pass+1))
  else
    printf 'FAIL  repeat-run (paths or mtimes changed on identical input)\n'; fail=$((fail+1))
  fi
  cp "$tmp/transaction/out/.javaconv-emitted" "$tmp/transaction/manifest.before"
  cat > "$tmp/transaction/src/Main.java" <<'JAVA'
public class Main { public static void main(String[] args) { System.out.println(2); } }
JAVA
  cat > "$tmp/transaction/src/ZCollision.java" <<'JAVA'
public class ZCollision { }
JAVA
  printf 'user-owned sentinel\n' > "$tmp/transaction/out/ZCollision.sleela"
  before_main="$(cat "$tmp/transaction/out/Main.sleela")"
  before_manifest="$(cat "$tmp/transaction/out/.javaconv-emitted")"
  tx_out="$("$conv" --jdk "$tmp/transaction/src" --out "$tmp/transaction/out" --quiet 2>&1)"
  tx_rc=$?
  after_main="$(cat "$tmp/transaction/out/Main.sleela")"
  after_manifest="$(cat "$tmp/transaction/out/.javaconv-emitted")"
  sentinel="$(cat "$tmp/transaction/out/ZCollision.sleela")"
  if [ "$tx_rc" -eq 2 ] && [ "$before_main" = "$after_main" ] && [ "$before_manifest" = "$after_manifest" ] && [ "$sentinel" = "user-owned sentinel" ]; then
    printf 'PASS  %-12s failed generation leaves previous output, manifest and unowned files intact\n' "transaction"; pass=$((pass+1))
  else
    printf 'FAIL  transaction (rc=%s)\n%s\n' "$tx_rc" "$tx_out"; fail=$((fail+1))
  fi
else
  printf 'FAIL  transaction (could not establish initial generation)\n'; fail=$((fail+1))
fi

# a public nested type is not accessible when an enclosing type is package-private.
mkdir -p "$tmp/enclosing-access/src/fixture/hidden" "$tmp/enclosing-access/src/fixture/use" "$tmp/enclosing-access/cls" "$tmp/enclosing-access/out"
cat > "$tmp/enclosing-access/src/fixture/hidden/HiddenOuter.java" <<'JAVA'
package fixture.hidden;
class HiddenOuter {
  public static class PublicNested { }
}
JAVA
cat > "$tmp/enclosing-access/src/fixture/use/Main.java" <<'JAVA'
package fixture.use;
import fixture.hidden.HiddenOuter.PublicNested;
public class Main { PublicNested value; }
JAVA
if javac -d "$tmp/enclosing-access/cls" "$tmp/enclosing-access/src/fixture/hidden/HiddenOuter.java" "$tmp/enclosing-access/src/fixture/use/Main.java" 2> "$tmp/enclosing-access/javac.err"; then
  printf 'FAIL  enclosing-access (javac unexpectedly accepted an inaccessible enclosing type)\n'; fail=$((fail+1))
else
  enclosing_out="$("$conv" --jdk "$tmp/enclosing-access/src" --out "$tmp/enclosing-access/out" --quiet 2>&1)"
  enclosing_rc=$?
  if [ "$enclosing_rc" -eq 1 ] && printf '%s' "$enclosing_out" | grep -q "\\[type-access\\]" && [ "$(find "$tmp/enclosing-access/out" -name '*.sleela' | wc -l)" -eq 0 ]; then
    printf 'PASS  %-12s inaccessible enclosing type blocks access to public nested type\n' "enclosing-access"; pass=$((pass+1))
  else
    printf 'FAIL  enclosing-access (rc=%s)\n%s\n' "$enclosing_rc" "$enclosing_out"; fail=$((fail+1))
  fi
fi

# duplicate declarations in the same compilation unit must be refused too
mkdir -p "$tmp/duplicate-same/src" "$tmp/duplicate-same/out"
cat > "$tmp/duplicate-same/src/Duplicate.java" <<'JAVA'
class Repeated { int first() { return 1; } }
class Repeated { int second() { return 2; } }
class DuplicateUse { Repeated value; }
JAVA
same_duplicate_out="$("$conv" --jdk "$tmp/duplicate-same/src" --out "$tmp/duplicate-same/out" --quiet 2>&1)"
same_duplicate_rc=$?
if [ "$same_duplicate_rc" -eq 1 ] && printf '%s' "$same_duplicate_out" | grep -q "\[type-ambiguous\]"; then
  printf 'PASS  %-12s duplicate declaration in one source unit refused\n' "duplicate-same-unit"; pass=$((pass+1))
else
  printf 'FAIL  duplicate-same-unit (rc=%s)\n%s\n' "$same_duplicate_rc" "$same_duplicate_out"; fail=$((fail+1))
fi


# a fully qualified name must not fall back to an unrelated project type with
# the same simple name in a different package.
mkdir -p "$tmp/qualified-mismatch/src/fixture/alpha" "$tmp/qualified-mismatch/src/fixture/use" "$tmp/qualified-mismatch/cls" "$tmp/qualified-mismatch/out"
cat > "$tmp/qualified-mismatch/src/fixture/alpha/Widget.java" <<'JAVA'
package fixture.alpha;
public class Widget { public int value() { return 7; } }
JAVA
cat > "$tmp/qualified-mismatch/src/fixture/use/Main.java" <<'JAVA'
package fixture.use;
public class Main {
  public static void main(String[] args) {
    System.out.println(new fixture.missing.Widget().value());
  }
}
JAVA
if javac -d "$tmp/qualified-mismatch/cls" "$tmp/qualified-mismatch/src/fixture/alpha/Widget.java" "$tmp/qualified-mismatch/src/fixture/use/Main.java" 2> "$tmp/qualified-mismatch/javac.err"; then
  printf 'FAIL  qualified-mismatch (javac unexpectedly accepted a nonexistent package type)\n'; fail=$((fail+1))
else
  qualified_out="$("$conv" --jdk "$tmp/qualified-mismatch/src" --out "$tmp/qualified-mismatch/out" --quiet 2>&1)"
  qualified_rc=$?
  if [ "$qualified_rc" -eq 1 ] && printf '%s' "$qualified_out" | grep -q "\\[type-qualified-not-found\\]" && [ "$(find "$tmp/qualified-mismatch/out" -name '*.sleela' | wc -l)" -eq 0 ]; then
    printf 'PASS  %-12s wrong-package qualified type refused instead of simple-name fallback\n' "qualified-mismatch"; pass=$((pass+1))
  else
    printf 'FAIL  qualified-mismatch (rc=%s)\n%s\n' "$qualified_rc" "$qualified_out"; fail=$((fail+1))
  fi
fi

echo "javaconv differential: pass=$pass fail=$fail"
[ "$fail" -eq 0 ]
