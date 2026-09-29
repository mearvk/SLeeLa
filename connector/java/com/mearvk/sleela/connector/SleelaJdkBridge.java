package com.mearvk.sleela.connector;
import java.lang.reflect.Constructor;
import java.lang.reflect.Method;
import java.lang.reflect.Modifier;
import java.util.Map;
import java.util.concurrent.ConcurrentHashMap;

/** Executes supported SLeeLa JDK bindings through the actual JVM implementation. */
public final class SleelaJdkBridge {
    private static final Map<String, Class<?>> TYPES = new ConcurrentHashMap<>();
    private SleelaJdkBridge() {}
    public static Class<?> type(String binaryName) throws ClassNotFoundException {
        try {
            return TYPES.computeIfAbsent(binaryName, name -> {
                try { return Class.forName(name, true, ClassLoader.getSystemClassLoader()); }
                catch (ClassNotFoundException e) { throw new TypeLookupException(e); }
            });
        } catch (TypeLookupException e) {
            throw (ClassNotFoundException)e.getCause();
        }
    }
    public static Object construct(String binaryName, Class<?>[] parameterTypes, Object[] arguments)
            throws ReflectiveOperationException {
        Constructor<?> c = type(binaryName).getConstructor(parameterTypes);
        return c.newInstance(arguments);
    }
    public static Object invoke(Object receiver, String methodName,
            Class<?>[] parameterTypes, Object[] arguments) throws ReflectiveOperationException {
        Class<?> owner = receiver.getClass();
        Method method = owner.getMethod(methodName, parameterTypes);
        if (!Modifier.isPublic(method.getModifiers())) throw new IllegalAccessException(method.toString());
        return method.invoke(receiver, arguments);
    }
    public static Object invokeStatic(String binaryName, String methodName,
            Class<?>[] parameterTypes, Object[] arguments) throws ReflectiveOperationException {
        Class<?> owner = type(binaryName);
        Method method = owner.getMethod(methodName, parameterTypes);
        if (!Modifier.isStatic(method.getModifiers())) throw new IllegalArgumentException(method.toString());
        return method.invoke(null, arguments);
    }
    public static boolean available(String binaryName) {
        try { type(binaryName); return true; }
        catch (ClassNotFoundException e) { return false; }
    }
    private static final class TypeLookupException extends RuntimeException {
        TypeLookupException(Throwable cause) { super(cause); }
    }
}
