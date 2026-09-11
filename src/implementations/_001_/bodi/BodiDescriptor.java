package implementations._001_.bodi;

import java.lang.reflect.Method;
import java.rmi.Remote;

/** Produces a small XML description of a registered Bodi object. */
public final class BodiDescriptor
{
    private BodiDescriptor() { }

    public static String describe(String system, Remote remote)
    {
        StringBuilder xml = new StringBuilder();
        xml.append("<bodi-system system=\"").append(escape(system)).append("\">");
        xml.append("<protocol>urn:sleela:bodi:1</protocol>");
        for (Method method : remote.getClass().getMethods())
        {
            if (method.getDeclaringClass() == Object.class)
                continue;
            xml.append("<method name=\"").append(escape(method.getName())).append("\">");
            Class<?>[] parameters = method.getParameterTypes();
            for (Class<?> parameter : parameters)
                xml.append("<parameter type=\"").append(escape(parameter.getName())).append("\"/>");
            xml.append("</method>");
        }
        xml.append("</bodi-system>");
        return xml.toString();
    }

    private static String escape(String value)
    {
        return value == null ? "" : value.replace("&", "&amp;")
            .replace("<", "&lt;").replace(">", "&gt;")
            .replace("\"", "&quot;").replace("'", "&apos;");
    }
}
