package implementations._001_.bodi;

public @interface BodiAnnotation
{
    public String protocol() default "";

    public String classname() default "";

    public String methodname() default "";

    public String requirement() default "";
}
