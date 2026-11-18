// expect: boxed-null
// An uninitialized Java wrapper field defaults to null, not the primitive zero value.
public class BoxedNullField {
    Integer value;
}
