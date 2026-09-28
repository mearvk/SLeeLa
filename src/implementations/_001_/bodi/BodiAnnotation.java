package implementations._001_.bodi;

import java.lang.annotation.ElementType;
import java.lang.annotation.Retention;
import java.lang.annotation.RetentionPolicy;
import java.lang.annotation.Target;

/** Declarative metadata for Bodi-discoverable operations. */
@Retention(RetentionPolicy.RUNTIME)
@Target({ElementType.TYPE, ElementType.METHOD})
public @interface BodiAnnotation
{
    String protocol() default "";
    String classname() default "";
    String methodname() default "";
    String requirement() default "";
}
