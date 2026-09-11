package implementations._001_.bodi;

import java.util.Collections;
import java.util.LinkedHashMap;
import java.util.Map;

/**
 * Canonical middle verbs for the Bodi witness/object-change layer.
 * Each verb has a conservative inverse so a change can be expressed as
 * an intentional positive or negative operation.
 */
public final class BodiVerb
{
    private static final Map<String, String> INVERSES;
    private static final Map<String, String> MEANINGS;

    static
    {
        LinkedHashMap<String, String> inverse = new LinkedHashMap<String, String>();
        LinkedHashMap<String, String> meanings = new LinkedHashMap<String, String>();

        add(inverse, meanings, "install", "uninstall", "place a named capability into a system");
        add(inverse, meanings, "connect", "disconnect", "establish or end a system relationship");
        add(inverse, meanings, "push", "pull", "publish or retrieve a datum/object reference");
        add(inverse, meanings, "enable", "disable", "permit or prevent an addressed capability");
        add(inverse, meanings, "start", "stop", "begin or end an active service sequence");
        add(inverse, meanings, "open", "close", "open or close an explicitly addressed resource");
        add(inverse, meanings, "attach", "detach", "join or remove an object from a system context");
        add(inverse, meanings, "bind", "unbind", "associate or remove a name from an object");
        add(inverse, meanings, "register", "unregister", "publish or withdraw a service identity");
        add(inverse, meanings, "mount", "unmount", "make or remove a resource from the active namespace");
        add(inverse, meanings, "activate", "deactivate", "make or cease making a capability operational");
        add(inverse, meanings, "admit", "reject", "accept or refuse an offered object/change");
        add(inverse, meanings, "approve", "revoke", "grant or withdraw an explicit authorization");
        add(inverse, meanings, "commit", "rollback", "make a witnessed change durable or reverse it");
        add(inverse, meanings, "publish", "withdraw", "make a descriptor/change publicly discoverable or remove it");
        add(inverse, meanings, "expose", "conceal", "make an interface discoverable or hide it");
        add(inverse, meanings, "observe", "ignore", "record or decline a witness observation");
        add(inverse, meanings, "propagate", "contain", "carry a change onward or stop it at the current boundary");

        INVERSES = Collections.unmodifiableMap(inverse);
        MEANINGS = Collections.unmodifiableMap(meanings);
    }

    private BodiVerb() { }

    private static void add(Map<String, String> inverse, Map<String, String> meanings,
                            String positive, String negative, String meaning)
    {
        inverse.put(positive, negative);
        inverse.put(negative, positive);
        meanings.put(positive, meaning);
        meanings.put(negative, "inverse of " + positive);
    }

    public static String inverse(String verb)
    {
        if (verb == null)
            return null;
        return INVERSES.get(verb.toLowerCase());
    }

    public static boolean known(String verb)
    {
        return verb != null && INVERSES.containsKey(verb.toLowerCase());
    }

    public static String meaning(String verb)
    {
        if (verb == null)
            return null;
        return MEANINGS.get(verb.toLowerCase());
    }

    public static Map<String, String> all()
    {
        return INVERSES;
    }
}
