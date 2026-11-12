import java.lang.module.*;
import java.lang.reflect.*;
import java.util.*;
import java.util.stream.*;

/**
 * Dump the public JDK API as JSON lines (one object per type) for
 * lib/java/tools/jdk_api_coverage.py and jdk_api_declare.py.
 *
 * Covers the unqualified exports of every java.* and jdk.* module in the boot
 * layer, public types only (nested types included when every enclosing type
 * is public). Members are public and protected constructors, methods and
 * fields; method entries are [name, return, [params], static, abstract,
 * default]; field entries are [name, type, static(, constant value)].
 *
 * usage: javac -d /tmp ApiDump.java && java -cp /tmp ApiDump > ../api/jdk-api.jsonl
 */
public class ApiDump {
    static String q(String s) {
        StringBuilder b = new StringBuilder("\"");
        for (char c : s.toCharArray()) {
            if (c == '"' || c == '\\') b.append('\\').append(c);
            else if (c < 32 || c > 126) b.append(String.format("\\u%04x", (int) c));
            else b.append(c);
        }
        return b.append('"').toString();
    }
    static String tn(Class<?> c) { return c.getCanonicalName() != null ? c.getCanonicalName() : c.getName(); }
    static String constVal(Field f) {
        try {
            Object v = f.get(null);
            if (v == null) return null;
            Class<?> t = f.getType();
            if (t == String.class) return q((String) v);
            if (t == char.class) return Integer.toString((Character) v);
            if (t == boolean.class || t == int.class || t == long.class || t == short.class || t == byte.class) return v.toString();
            if (t == double.class || t == float.class) {
                double d = ((Number) v).doubleValue();
                if (Double.isNaN(d) || Double.isInfinite(d)) return null;
                return q(t == float.class ? Float.toString((Float) v) : Double.toString(d));
            }
        } catch (Throwable e) { }
        return null;
    }
    public static void main(String[] a) throws Exception {
        List<String> out = new ArrayList<>();
        for (Module m : ModuleLayer.boot().modules().stream().sorted(Comparator.comparing(Module::getName)).toList()) {
            String mn = m.getName();
            if (!(mn.startsWith("java.") || mn.startsWith("jdk."))) continue;
            ModuleDescriptor d = m.getDescriptor();
            Set<String> exported = d.exports().stream().filter(e -> !e.isQualified()).map(ModuleDescriptor.Exports::source).collect(Collectors.toSet());
            Optional<ModuleReference> ref = ModuleFinder.ofSystem().find(mn);
            if (ref.isEmpty()) continue;
            List<String> classes;
            try (ModuleReader r = ref.get().open()) {
                classes = r.list().filter(n -> n.endsWith(".class") && !n.equals("module-info.class")).map(n -> n.substring(0, n.length() - 6).replace('/', '.')).sorted().toList();
            }
            for (String cn : classes) {
                int dot = cn.lastIndexOf('.');
                if (dot < 0 || !exported.contains(cn.substring(0, dot))) continue;
                Class<?> c;
                try { c = Class.forName(cn, false, ClassLoader.getSystemClassLoader()); } catch (Throwable e) { continue; }
                if (c.isAnonymousClass() || c.isLocalClass() || c.isSynthetic()) continue;
                // public, and every enclosing class public
                boolean pub = true;
                for (Class<?> k = c; k != null; k = k.getEnclosingClass()) if (!Modifier.isPublic(k.getModifiers())) pub = false;
                if (!pub) continue;
                String kind = c.isAnnotation() ? "annotation" : c.isInterface() ? "interface" : c.isEnum() ? "enum" : c.isRecord() ? "record" : "class";
                StringBuilder s = new StringBuilder();
                s.append("{\"module\":").append(q(mn)).append(",\"name\":").append(q(cn.replace('$', '.')))
                 .append(",\"binary\":").append(q(cn)).append(",\"kind\":").append(q(kind));
                if (c.getSuperclass() != null) s.append(",\"super\":").append(q(c.getSuperclass().getName().replace('$', '.')));
                s.append(",\"abstract\":").append(Modifier.isAbstract(c.getModifiers()));
                s.append(",\"interfaces\":[").append(Arrays.stream(c.getInterfaces()).map(i -> q(i.getName().replace('$', '.'))).collect(Collectors.joining(","))).append("]");
                if (c.isEnum()) s.append(",\"constants\":[").append(Arrays.stream(c.getEnumConstants()).map(e -> q(((Enum<?>) e).name())).collect(Collectors.joining(","))).append("]");
                if (c.isRecord()) s.append(",\"components\":[").append(Arrays.stream(c.getRecordComponents()).map(rc -> "[" + q(rc.getName()) + "," + q(tn(rc.getType())) + "]").collect(Collectors.joining(","))).append("]");
                List<String> fields = new ArrayList<>();
                try {
                    for (Field f : c.getDeclaredFields()) {
                        int mo = f.getModifiers();
                        if (!(Modifier.isPublic(mo) || Modifier.isProtected(mo)) || f.isEnumConstant() || f.isSynthetic()) continue;
                        String v = (Modifier.isStatic(mo) && Modifier.isFinal(mo)) ? constVal(f) : null;
                        if (v != null && (c.isInterface() || true)) { try { f.setAccessible(true); } catch (Throwable e) {} }
                        fields.add("[" + q(f.getName()) + "," + q(tn(f.getType())) + "," + Modifier.isStatic(mo) + (v != null ? "," + v : "") + "]");
                    }
                } catch (Throwable e) { }
                s.append(",\"fields\":[").append(String.join(",", fields)).append("]");
                List<String> methods = new ArrayList<>();
                try {
                    for (Constructor<?> k : c.getDeclaredConstructors()) {
                        int mo = k.getModifiers();
                        if (!(Modifier.isPublic(mo) || Modifier.isProtected(mo)) || k.isSynthetic()) continue;
                        methods.add("[\"<init>\",\"void\",[" + Arrays.stream(k.getParameterTypes()).map(p -> q(tn(p))).collect(Collectors.joining(",")) + "],false,false,false]");
                    }
                    for (Method k : c.getDeclaredMethods()) {
                        int mo = k.getModifiers();
                        if (!(Modifier.isPublic(mo) || Modifier.isProtected(mo)) || k.isSynthetic() || k.isBridge()) continue;
                        methods.add("[" + q(k.getName()) + "," + q(tn(k.getReturnType())) + ",[" + Arrays.stream(k.getParameterTypes()).map(p -> q(tn(p))).collect(Collectors.joining(",")) + "]," + Modifier.isStatic(mo) + "," + Modifier.isAbstract(mo) + "," + k.isDefault() + "]");
                    }
                } catch (Throwable e) { }
                s.append(",\"methods\":[").append(String.join(",", methods)).append("]}");
                out.add(s.toString());
            }
        }
        for (String l : out) System.out.println(l);
        System.err.println("types=" + out.size() + " java.version=" + System.getProperty("java.version"));
    }
}
