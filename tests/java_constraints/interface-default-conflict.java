// EXPECT: InterfaceDefaultConflict
interface Left {
  default int value() { return 1; }
}
interface Right {
  default int value() { return 2; }
}
class Conflict implements Left, Right {
}
