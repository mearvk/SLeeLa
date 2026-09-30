// EXPECT: Ambiguous
class OverloadAmbiguity {
  void pick(String value) {}
  void pick(Integer value) {}
  void test() {
    pick(null);
  }
}
