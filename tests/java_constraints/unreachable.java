// EXPECT: UnreachableStatement
class Unreachable {
  int test() {
    return 1;
    int x = 2;
  }
}
