package implementations.one.nordshrift;

public @interface NordshriftAnnotation
{
    public String requirement() default "";

    public int id() default -1;

    public String[] ref() default "";
}
