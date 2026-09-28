package sleela.regex;
/** Parser front-end with deterministic diagnostics. */
public final class SleelaRegexNaturalParser { public record Result(boolean valid,String message){} private SleelaRegexNaturalParser(){} public static Result parse(String source){try{SleelaRegexNatural.validate(source);return new Result(true,"");}catch(IllegalArgumentException e){return new Result(false,e.getMessage());}} }
