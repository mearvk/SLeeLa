package com.mearvk.sleela.connector;

import java.io.IOException;
import java.lang.reflect.Constructor;
import java.lang.reflect.Method;
import java.lang.reflect.Modifier;
import java.nio.file.Path;
import java.util.Objects;

/**
 * Executes explicitly allow-listed SLeeLa Java bindings through the host JVM.
 *
 * Class lookup is non-initializing. Construction and method invocation use
 * public reflective members and therefore initialize classes only when the
 * requested operation actually executes.
 */
public final class SleelaJdkBridge {
    private static final SleelaClassLoader LOADER = new SleelaClassLoader();

    private SleelaJdkBridge() {}

    /** Close configured class roots and release cached Java classes. */
    public static void closeRuntime() {
        LOADER.close();
    }

    /** Add one explicitly permitted Java binary class name. */
    public static void allowClass(String binaryName) {
        LOADER.allowClass(binaryName);
    }

    /** Permit a Java package namespace and its subpackages. */
    public static void allowPackage(String packagePrefix) {
        LOADER.allowPackage(packagePrefix);
    }

    /** Add an operator-selected class directory or JAR root. */
    public static void addRoot(Path directoryOrJar) throws IOException {
        LOADER.addRoot(directoryOrJar);
    }

    /** Policy check only; it does not resolve or initialize the class. */
    public static boolean isAllowed(String binaryName) {
        return LOADER.isAllowed(binaryName);
    }

    /** Resolve a class without running its static initializer. */
    public static Class<?> type(String binaryName) throws ClassNotFoundException {
        try {
            return LOADER.resolve(binaryName, false);
        } catch (SleelaClassLoader.ClassResolutionException e) {
            throw new ClassNotFoundException(e.getMessage(), e);
        }
    }

    public static Object construct(String binaryName, Class<?>[] parameterTypes, Object[] arguments)
            throws ReflectiveOperationException {
        validateInvocation(parameterTypes, arguments);
        Constructor<?> constructor = type(binaryName).getConstructor(parameterTypes);
        return constructor.newInstance(arguments);
    }

    public static Object invoke(Object receiver, String methodName,
            Class<?>[] parameterTypes, Object[] arguments) throws ReflectiveOperationException {
        Objects.requireNonNull(receiver, "receiver");
        validateMemberName(methodName);
        validateInvocation(parameterTypes, arguments);
        Class<?> owner = type(receiver.getClass().getName());
        if (owner != receiver.getClass()) {
            throw new IllegalArgumentException("receiver was not resolved by the configured Java loader");
        }
        Method method = owner.getMethod(methodName, parameterTypes);
        if (Modifier.isStatic(method.getModifiers())) {
            throw new IllegalArgumentException("instance invocation cannot target a static method: "
                    + method);
        }
        return method.invoke(receiver, arguments);
    }

    public static Object invokeStatic(String binaryName, String methodName,
            Class<?>[] parameterTypes, Object[] arguments) throws ReflectiveOperationException {
        validateMemberName(methodName);
        validateInvocation(parameterTypes, arguments);
        Class<?> owner = type(binaryName);
        Method method = owner.getMethod(methodName, parameterTypes);
        if (!Modifier.isStatic(method.getModifiers())) {
            throw new IllegalArgumentException("static invocation requires a static method: " + method);
        }
        return method.invoke(null, arguments);
    }

    public static boolean available(String binaryName) {
        if (!LOADER.isAllowed(binaryName)) {
            return false;
        }
        try {
            type(binaryName);
            return true;
        } catch (ClassNotFoundException | LinkageError e) {
            return false;
        }
    }

    private static void validateInvocation(Class<?>[] parameterTypes, Object[] arguments) {
        Objects.requireNonNull(parameterTypes, "parameterTypes");
        Objects.requireNonNull(arguments, "arguments");
        if (parameterTypes.length != arguments.length) {
            throw new IllegalArgumentException("parameter type count does not match argument count");
        }
        for (Class<?> parameterType : parameterTypes) {
            Objects.requireNonNull(parameterType, "parameter type");
        }
    }

    private static void validateMemberName(String name) {
        Objects.requireNonNull(name, "methodName");
        if (!name.matches("[A-Za-z_$][A-Za-z0-9_$]*")) {
            throw new IllegalArgumentException("invalid Java member name: " + name);
        }
    }
}
