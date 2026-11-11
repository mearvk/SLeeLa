import com.sun.source.tree.*;
import com.sun.source.util.JavacTask;
import java.io.*;
import java.lang.module.*;
import java.lang.reflect.*;
import java.net.URI;
import java.nio.charset.StandardCharsets;
import java.util.*;
import java.util.stream.*;
import java.util.zip.*;
import javax.tools.*;

/**
 * Dump the exact Java signatures of the public JDK API as JSON lines, one
 * object per type, for lib/java/tools/jdk_api_signatures.py.
 *
 * Each method / constructor entry carries the Java modifiers, the type
 * parameters with their bounds, the generic return and parameter types
 * (fully qualified, from reflection), the varargs flag, the declared
 * exceptions, and the parameter NAMES as written in the JDK source (read
 * from lib/src.zip with the javac parser; only declarations are read, never
 * method bodies).
 *
 * usage: javac -d /tmp/sigs ApiSignatures.java
 *        java -cp /tmp/sigs ApiSignatures [path/to/src.zip] > ../api/jdk-signatures.jsonl
 */
public class ApiSignatures {
    static String q(String s) {
        StringBuilder b = new StringBuilder("\"");
        for (char c : s.toCharArray()) {
            if (c == '"' || c == '\\') b.append('\\').append(c);
            else if (c < 32 || c > 126) b.append(String.format("\\u%04x", (int) c));
            else b.append(c);
        }
        return b.append('"').toString();
    }

    static String typeText(Type t) {
        return t.getTypeName().replace('$', '.');
    }

    static String typeParams(TypeVariable<?>[] vars) {
        if (vars.length == 0) return "";
        List<String> out = new ArrayList<>();
        for (TypeVariable<?> v : vars) {
            Type[] b = v.getBounds();
            if (b.length == 1 && b[0] == Object.class) out.add(v.getName());
            else out.add(v.getName() + " extends " + Arrays.stream(b).map(ApiSignatures::typeText).collect(Collectors.joining(" & ")));
        }
        return "<" + String.join(", ", out) + ">";
    }

    /** Parameter names by "Type.member(arity)" key from the source of one file. */
    static Map<String, List<List<String>>> names = new HashMap<>();

    static void readSources(String srcZip) throws IOException {
        JavaCompiler javac = ToolProvider.getSystemJavaCompiler();
        try (ZipFile zip = new ZipFile(srcZip)) {
            List<JavaFileObject> files = new ArrayList<>();
            for (ZipEntry e : Collections.list(zip.entries())) {
                String n = e.getName();
                if (!n.endsWith(".java") || n.endsWith("module-info.java") || n.endsWith("package-info.java")) continue;
                String text = new String(zip.getInputStream(e).readAllBytes(), StandardCharsets.UTF_8);
                files.add(new SimpleJavaFileObject(URI.create("string:///" + n), JavaFileObject.Kind.SOURCE) {
                    @Override public CharSequence getCharContent(boolean b) { return text; }
                });
            }
            int batch = 400;
            for (int i = 0; i < files.size(); i += batch) {
                JavacTask task = (JavacTask) javac.getTask(null, null, d -> { }, List.of("-proc:none"), null,
                        files.subList(i, Math.min(files.size(), i + batch)));
                for (CompilationUnitTree cu : task.parse()) {
                    String pkg = cu.getPackageName() == null ? "" : cu.getPackageName().toString();
                    for (Tree t : cu.getTypeDecls()) if (t instanceof ClassTree ct) walk(pkg, ct);
                }
            }
        }
    }

    static void walk(String owner, ClassTree ct) {
        String name = owner.isEmpty() ? ct.getSimpleName().toString() : owner + "." + ct.getSimpleName();
        for (Tree m : ct.getMembers()) {
            if (m instanceof MethodTree mt) {
                String mname = mt.getName().toString();
                String key = name + "." + mname + "(" + mt.getParameters().size() + ")";
                List<String> ps = mt.getParameters().stream().map(p -> p.getName().toString()).toList();
                names.computeIfAbsent(key, k -> new ArrayList<>()).add(ps);
                List<String> simple = mt.getParameters().stream().map(p -> erasedSimple(p.getType().toString())).toList();
                names.computeIfAbsent(key + simple, k -> new ArrayList<>()).add(ps);
            } else if (m instanceof ClassTree inner) {
                walk(name, inner);
            }
        }
    }

    /** Simple erased spelling of a source type: java.util.List<E>[] -> List[] */
    static String erasedSimple(String t) {
        String s = t.replaceAll("@\\S+\\s+", "").replaceAll("<.*>", "").replace("...", "[]").trim();
        int dims = 0;
        while (s.endsWith("[]")) { dims++; s = s.substring(0, s.length() - 2).trim(); }
        int dot = s.lastIndexOf('.');
        if (dot >= 0) s = s.substring(dot + 1);
        return s + "[]".repeat(dims);
    }

    static String reflectSimple(Class<?> c, Type generic, Set<String> typeVars) {
        String t = generic.getTypeName().replace('$', '.');
        String s = t.replaceAll("<.*>", "");
        int dims = 0;
        while (s.endsWith("[]")) { dims++; s = s.substring(0, s.length() - 2); }
        int dot = s.lastIndexOf('.');
        if (dot >= 0) s = s.substring(dot + 1);
        return s + "[]".repeat(dims);
    }

    static List<String> paramNames(String type, String member, Executable ex) {
        int n = ex.getParameterCount();
        String key = type + "." + member + "(" + n + ")";
        List<String> simple = new ArrayList<>();
        Type[] g = ex.getGenericParameterTypes();
        if (g.length != n) g = ex.getParameterTypes();
        for (Type t : g) simple.add(reflectSimple(ex.getDeclaringClass(), t, Set.of()));
        List<List<String>> exact = names.get(key + simple);
        if (exact != null && exact.size() == 1) return exact.get(0);
        List<List<String>> byArity = names.get(key);
        if (byArity != null && byArity.size() == 1) return byArity.get(0);
        if (exact != null && !exact.isEmpty()) return exact.get(0);
        List<String> out = new ArrayList<>();
        for (int i = 0; i < n; i++) out.add("arg" + i);
        return out;
    }

    static String mods(int m, boolean isDefault) {
        List<String> out = new ArrayList<>();
        if (Modifier.isPublic(m)) out.add("public");
        if (Modifier.isProtected(m)) out.add("protected");
        if (Modifier.isAbstract(m)) out.add("abstract");
        if (isDefault) out.add("default");
        if (Modifier.isStatic(m)) out.add("static");
        if (Modifier.isFinal(m)) out.add("final");
        if (Modifier.isSynchronized(m)) out.add("synchronized");
        if (Modifier.isNative(m)) out.add("native");
        return String.join(" ", out);
    }

    static String entry(String type, Executable ex, boolean ctor) {
        int m = ex.getModifiers();
        boolean isDefault = ex instanceof Method mm && mm.isDefault();
        String name = ctor ? "<init>" : ex.getName();
        Type[] gp = ex.getGenericParameterTypes();
        if (gp.length != ex.getParameterCount()) gp = ex.getParameterTypes();
        List<String> pn = paramNames(type, ctor ? "<init>" : ex.getName(), ex);
        List<String> params = new ArrayList<>();
        for (int i = 0; i < gp.length; i++) {
            String t = typeText(gp[i]);
            boolean va = ex.isVarArgs() && i == gp.length - 1 && t.endsWith("[]");
            if (va) t = t.substring(0, t.length() - 2) + "...";
            params.add("[" + q(t) + "," + q(pn.get(i)) + "]");
        }
        String ret = ctor ? "" : typeText(((Method) ex).getGenericReturnType());
        String thr = Arrays.stream(ex.getGenericExceptionTypes()).map(e -> q(typeText(e))).collect(Collectors.joining(","));
        return "{\"name\":" + q(name) + ",\"mods\":" + q(mods(m, isDefault)) + ",\"tparams\":" + q(typeParams(ex.getTypeParameters()))
                + ",\"ret\":" + q(ret) + ",\"params\":[" + String.join(",", params) + "],\"throws\":[" + thr + "]"
                + ",\"static\":" + Modifier.isStatic(m) + ",\"abstract\":" + Modifier.isAbstract(m) + ",\"protected\":" + Modifier.isProtected(m) + "}";
    }

    public static void main(String[] a) throws Exception {
        String src = a.length > 0 ? a[0] : System.getProperty("java.home") + "/lib/src.zip";
        readSources(src);
        int types = 0;
        for (Module mod : ModuleLayer.boot().modules().stream().sorted(Comparator.comparing(Module::getName)).toList()) {
            String mn = mod.getName();
            if (!(mn.startsWith("java.") || mn.startsWith("jdk."))) continue;
            Set<String> exported = mod.getDescriptor().exports().stream().filter(e -> !e.isQualified()).map(ModuleDescriptor.Exports::source).collect(Collectors.toSet());
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
                boolean pub = true;
                for (Class<?> k = c; k != null; k = k.getEnclosingClass()) if (!Modifier.isPublic(k.getModifiers())) pub = false;
                if (!pub) continue;
                String type = cn.replace('$', '.');
                List<String> entries = new ArrayList<>();
                try {
                    for (Constructor<?> k : c.getDeclaredConstructors()) {
                        int m = k.getModifiers();
                        if ((Modifier.isPublic(m) || Modifier.isProtected(m)) && !k.isSynthetic()) entries.add(entry(type, k, true));
                    }
                    for (Method k : c.getDeclaredMethods()) {
                        int m = k.getModifiers();
                        if ((Modifier.isPublic(m) || Modifier.isProtected(m)) && !k.isSynthetic() && !k.isBridge()) entries.add(entry(type, k, false));
                    }
                } catch (Throwable e) { }
                System.out.println("{\"name\":" + q(type) + ",\"binary\":" + q(cn) + ",\"tparams\":" + q(typeParams(c.getTypeParameters()))
                        + ",\"members\":[" + String.join(",", entries) + "]}");
                types++;
            }
        }
        System.err.println("types=" + types + " parameter-name keys=" + names.size() + " java.version=" + System.getProperty("java.version"));
    }
}
