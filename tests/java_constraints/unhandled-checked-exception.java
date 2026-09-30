// EXPECT: UnhandledCheckedException
class UnhandledCheckedException {
  void test() {
    java.io.IOException value = new java.io.IOException();
    throw value;
  }
}
