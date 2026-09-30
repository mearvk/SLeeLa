// EXPECT: UnreachableStatement
class Unreachable {
  int test() {
    return 1;
    // unreachable statement follows
    // EXPECT-SITE: statement after return
    // int x = 2;
  }
}
