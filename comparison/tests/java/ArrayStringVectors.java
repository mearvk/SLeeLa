import java.nio.ByteBuffer;
import java.nio.charset.*;
import java.util.Arrays;
public final class ArrayStringVectors {
 static boolean valid(byte[] b){try{StandardCharsets.UTF_8.newDecoder().onMalformedInput(CodingErrorAction.REPORT).onUnmappableCharacter(CodingErrorAction.REPORT).decode(ByteBuffer.wrap(b));return true;}catch(CharacterCodingException e){return false;}}
 static boolean roundTrip(byte[] b){if(!valid(b))return false;String s=StandardCharsets.UTF_8.decode(ByteBuffer.wrap(b)).toString();return Arrays.equals(b,s.getBytes(StandardCharsets.UTF_8));}
 public static void main(String[]x){int[] empty={},one={7},a={11,22,33};byte[] s="Aé😀".getBytes(StandardCharsets.UTF_8),bad={(byte)0xC0,(byte)0xAF};
 System.out.println("EMPTY_LENGTH="+empty.length);System.out.println("SINGLE_LENGTH="+one.length);System.out.println("FIRST_VALUE="+a[0]);System.out.println("LAST_VALUE="+a[a.length-1]);
 try{int z=a[-1];System.out.println("NEGATIVE_INDEX=ACCEPTED");}catch(ArrayIndexOutOfBoundsException e){System.out.println("NEGATIVE_INDEX=REJECTED");}
 try{int z=a[a.length];System.out.println("ONE_PAST_END=ACCEPTED");}catch(ArrayIndexOutOfBoundsException e){System.out.println("ONE_PAST_END=REJECTED");}
 String decoded=new String(s,StandardCharsets.UTF_8);System.out.println("UTF8_ROUNDTRIP="+(roundTrip(s)?"PASS":"FAIL"));System.out.println("UTF8_INVALID="+(valid(bad)?"ACCEPTED":"REJECTED"));System.out.println("UTF8_BYTES="+s.length);System.out.println("UNICODE_CODEPOINTS="+decoded.codePointCount(0,decoded.length()));}
}