// EXPECT: UseBeforeAssignment
class UseBeforeAssignment {
  void test() {
    int x;
    System.out.println(x);
  }
}
