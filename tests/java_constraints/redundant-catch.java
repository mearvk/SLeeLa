// EXPECT: RedundantCatch
class RedundantCatch {
  void test() {
    try {
      throw new java.io.IOException();
    } catch (Exception e) {
    } catch (java.io.IOException e) {
    }
  }
}
