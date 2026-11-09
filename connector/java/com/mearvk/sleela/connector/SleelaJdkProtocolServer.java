package com.mearvk.sleela.connector;

import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.EOFException;
import java.io.IOException;
import java.lang.reflect.InvocationTargetException;
import java.nio.ByteBuffer;
import java.nio.charset.CharacterCodingException;
import java.nio.charset.CodingErrorAction;
import java.nio.charset.StandardCharsets;
import java.nio.CharBuffer;
import java.nio.file.Path;
import java.util.HashMap;
import java.util.Map;
import java.util.concurrent.atomic.AtomicLong;

/**
 * SLJIP v1 binary endpoint. Reads frames from stdin and writes only protocol
 * frames to stdout. Diagnostics are intentionally not printed to stdout.
 */
public final class SleelaJdkProtocolServer {
    private static final int VERSION = 1;
    private static final int HEADER_BYTES = 15;
    private static final int MAX_FRAME = 1_048_576;
    private static final int MAX_STRING = 262_144;
    private static final int MAX_PARAMETERS = 256;
    private static final int MAX_HANDLES = 65_536;
    private static final int PING = 1, RESOLVE = 2, CONSTRUCT = 3,
            INVOKE_INSTANCE = 4, INVOKE_STATIC = 5, RELEASE = 6, CLOSE = 7;

    private final Map<Long, Object> handles = new HashMap<>();
    private final AtomicLong nextHandle = new AtomicLong(1);

    private SleelaJdkProtocolServer() {}

    public static void main(String[] args) throws Exception {
        configureAllowList();
        if (args.length != 0) {
            System.err.println("SLJIP server accepts no command-line arguments");
            System.exit(2);
        }
        new SleelaJdkProtocolServer().serve();
    }

    private static void configureAllowList() throws Exception {
        addCsv(System.getenv("SLEELA_JAVA_ALLOW_CLASSES"), false);
        addCsv(System.getenv("SLEELA_JAVA_ALLOW_PACKAGES"), true);
        String roots = System.getenv("SLEELA_JAVA_CLASS_PATHS");
        if (roots != null && !roots.isBlank()) {
            for (String root : roots.split(java.io.File.pathSeparator, -1)) {
                if (root.isBlank()) throw new IllegalArgumentException("empty Java class-path root");
                SleelaJdkBridge.addRoot(Path.of(root));
            }
        }
    }

    private static void addCsv(String values, boolean packages) {
        if (values == null || values.isBlank()) return;
        for (String item : values.split(",", -1)) {
            String name = item.trim();
            if (name.isEmpty()) throw new IllegalArgumentException("empty Java allow-list entry");
            if (packages) SleelaJdkBridge.allowPackage(name);
            else SleelaJdkBridge.allowClass(name);
        }
    }

    private void serve() throws IOException {
        DataInputStream input = new DataInputStream(System.in);
        DataOutputStream output = new DataOutputStream(System.out);
        while (true) {
            byte[] header = new byte[HEADER_BYTES];
            try {
                input.readFully(header);
            } catch (EOFException end) {
                break;
            }
            DataInputStream h = new DataInputStream(new ByteArrayInputStream(header));
            byte[] magic = new byte[4];
            h.readFully(magic);
            int version = h.readUnsignedShort();
            long requestId = Integer.toUnsignedLong(h.readInt());
            int opcode = h.readUnsignedByte();
            long payloadLength = Integer.toUnsignedLong(h.readInt());
            if (magic[0] != 'S' || magic[1] != 'L' || magic[2] != 'J' || magic[3] != 'V'
                    || version != VERSION || payloadLength > MAX_FRAME) {
                throw new IOException("invalid SLJIP frame header");
            }
            byte[] payload = new byte[(int) payloadLength];
            input.readFully(payload);
            byte[] response;
            try {
                response = dispatch(opcode, payload);
            } catch (Exception | LinkageError failure) {
                response = errorPayload(failure);
            }
            writeFrame(output, requestId, opcode | 0x80, response);
            if (opcode == CLOSE) break;
        }
        handles.clear();
        SleelaJdkBridge.closeRuntime();
    }

    private byte[] dispatch(int opcode, byte[] payload) throws Exception {
        DataInputStream in = new DataInputStream(new ByteArrayInputStream(payload));
        Object result;
        switch (opcode) {
            case PING:
                result = null;
                break;
            case RESOLVE: {
                String name = readString(in);
                result = SleelaJdkBridge.type(name).getName();
                break;
            }
            case CONSTRUCT: {
                String name = readString(in);
                Class<?>[] types = readTypes(in);
                Object[] values = readArguments(in, types.length);
                result = SleelaJdkBridge.construct(name, types, values);
                break;
            }
            case INVOKE_INSTANCE: {
                long handle = in.readLong();
                Object receiver = requireHandle(handle);
                String member = readString(in);
                Class<?>[] types = readTypes(in);
                Object[] values = readArguments(in, types.length);
                result = SleelaJdkBridge.invoke(receiver, member, types, values);
                break;
            }
            case INVOKE_STATIC: {
                String name = readString(in);
                String member = readString(in);
                Class<?>[] types = readTypes(in);
                Object[] values = readArguments(in, types.length);
                result = SleelaJdkBridge.invokeStatic(name, member, types, values);
                break;
            }
            case RELEASE: {
                long handle = in.readLong();
                if (handles.remove(handle) == null) {
                    throw new IllegalArgumentException("unknown or released object handle");
                }
                result = null;
                break;
            }
            case CLOSE:
                handles.clear();
                result = null;
                break;
            default:
                throw new IllegalArgumentException("unsupported SLJIP opcode: " + opcode);
        }
        if (in.available() != 0) throw new IOException("trailing bytes in SLJIP request payload");
        return successPayload(result);
    }

    private Class<?>[] readTypes(DataInputStream in) throws Exception {
        int count = in.readUnsignedShort();
        if (count > MAX_PARAMETERS) throw new IOException("too many Java parameters");
        Class<?>[] result = new Class<?>[count];
        for (int i = 0; i < count; i++) result[i] = resolveType(readString(in));
        return result;
    }

    private Class<?> resolveType(String name) throws Exception {
        switch (name) {
            case "boolean": return boolean.class;
            case "byte": return byte.class;
            case "char": return char.class;
            case "short": return short.class;
            case "int": return int.class;
            case "long": return long.class;
            case "float": return float.class;
            case "double": return double.class;
            default: return SleelaJdkBridge.type(name);
        }
    }

    private Object[] readArguments(DataInputStream in, int expected) throws IOException {
        int count = in.readUnsignedShort();
        if (count != expected) throw new IOException("parameter and argument counts differ");
        Object[] values = new Object[count];
        for (int i = 0; i < count; i++) values[i] = readValue(in);
        return values;
    }

    private Object readValue(DataInputStream in) throws IOException {
        switch (in.readUnsignedByte()) {
            case 0: return null;
            case 1: return Boolean.FALSE;
            case 2: return Boolean.TRUE;
            case 3: return in.readInt();
            case 4: return in.readLong();
            case 5: return in.readDouble();
            case 6: return readString(in);
            case 7: return requireHandle(in.readLong());
            default: throw new IOException("unknown SLJIP value tag");
        }
    }

    private Object requireHandle(long id) throws IOException {
        Object value = handles.get(id);
        if (id == 0 || value == null) throw new IOException("unknown or released object handle");
        return value;
    }

    private byte[] successPayload(Object value) throws IOException {
        ByteArrayOutputStream bytes = new ByteArrayOutputStream();
        DataOutputStream out = new DataOutputStream(bytes);
        out.writeByte(0);
        writeValue(out, value);
        out.flush();
        return bytes.toByteArray();
    }

    private byte[] errorPayload(Throwable failure) throws IOException {
        Throwable cause = failure;
        if (failure instanceof InvocationTargetException
                && ((InvocationTargetException) failure).getCause() != null) {
            cause = ((InvocationTargetException) failure).getCause();
        }
        ByteArrayOutputStream bytes = new ByteArrayOutputStream();
        DataOutputStream out = new DataOutputStream(bytes);
        out.writeByte(1);
        writeString(out, cause.getClass().getName());
        String message = cause.getMessage();
        writeString(out, message == null ? "" : message);
        out.flush();
        return bytes.toByteArray();
    }

    private void writeValue(DataOutputStream out, Object value) throws IOException {
        if (value == null) {
            out.writeByte(0);
        } else if (value instanceof Boolean) {
            out.writeByte((Boolean) value ? 2 : 1);
        } else if (value instanceof Byte || value instanceof Short
                || value instanceof Integer || value instanceof Character) {
            out.writeByte(3);
            out.writeInt(value instanceof Character ? (Character) value : ((Number) value).intValue());
        } else if (value instanceof Long) {
            out.writeByte(4);
            out.writeLong((Long) value);
        } else if (value instanceof Float || value instanceof Double) {
            out.writeByte(5);
            out.writeDouble(((Number) value).doubleValue());
        } else if (value instanceof String) {
            out.writeByte(6);
            writeString(out, (String) value);
        } else if (value.getClass().isArray()) {
            throw new IOException("array results are not supported by SLJIP v1");
        } else {
            if (handles.size() >= MAX_HANDLES) throw new IOException("Java object handle limit reached");
            long id = nextHandle.getAndIncrement();
            if (id <= 0) throw new IOException("Java object handle space exhausted");
            handles.put(id, value);
            out.writeByte(7);
            out.writeLong(id);
        }
    }

    private static String readString(DataInputStream in) throws IOException {
        long length = Integer.toUnsignedLong(in.readInt());
        if (length > MAX_STRING || length > in.available()) throw new IOException("invalid SLJIP string length");
        byte[] bytes = new byte[(int) length];
        in.readFully(bytes);
        try {
            return StandardCharsets.UTF_8.newDecoder()
                    .onMalformedInput(CodingErrorAction.REPORT)
                    .onUnmappableCharacter(CodingErrorAction.REPORT)
                    .decode(ByteBuffer.wrap(bytes)).toString();
        } catch (CharacterCodingException invalid) {
            throw new IOException("malformed UTF-8 in SLJIP string", invalid);
        }
    }

    private static void writeString(DataOutputStream out, String value) throws IOException {
        try {
            ByteBuffer encoded = StandardCharsets.UTF_8.newEncoder()
                    .onMalformedInput(CodingErrorAction.REPORT)
                    .onUnmappableCharacter(CodingErrorAction.REPORT)
                    .encode(CharBuffer.wrap(value));
            if (encoded.remaining() > MAX_STRING) throw new IOException("SLJIP string exceeds limit");
            byte[] bytes = new byte[encoded.remaining()];
            encoded.get(bytes);
            out.writeInt(bytes.length);
            out.write(bytes);
        } catch (CharacterCodingException invalid) {
            throw new IOException("Java result contains malformed UTF-16", invalid);
        }
    }

    private static void writeFrame(DataOutputStream out, long requestId, int opcode,
            byte[] payload) throws IOException {
        if (payload.length > MAX_FRAME) throw new IOException("SLJIP response exceeds frame limit");
        out.writeByte('S'); out.writeByte('L'); out.writeByte('J'); out.writeByte('V');
        out.writeShort(VERSION);
        out.writeInt((int) requestId);
        out.writeByte(opcode);
        out.writeInt(payload.length);
        out.write(payload);
        out.flush();
    }
}
