package sleela.regex;
public final class SleelaRegexNaturalTest { public static void main(String[] args){SleelaRegexNatural.validate("begin digit+ end");if(!"[0-9]".equals(SleelaRegexNatural.canonicalSymbol("digit")))throw new AssertionError();if(SleelaRegexNaturalParser.parse("mystery").valid())throw new AssertionError();} }
