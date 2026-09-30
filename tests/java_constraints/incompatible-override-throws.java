// EXPECT: IncompatibleOverrideThrows
class Base {
  void read() throws java.io.IOException {}
}
class Derived extends Base {
  void read() throws java.sql.SQLException {}
}
