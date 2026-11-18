class PerMemberStubPolicy {
  int supported(int value) {
    return value + 1;
  }

  int unsupported(int value) {
    return switch (value) {
      case 0 -> 10;
      default -> 20;
    };
  }
}
