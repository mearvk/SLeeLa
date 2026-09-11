package implementations._001_.bodi;

import java.io.StringReader;
import java.util.EnumMap;
import java.util.HashMap;
import java.util.HashSet;
import java.util.Map;
import java.util.Set;

import javax.xml.XMLConstants;
import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;

import org.w3c.dom.Document;
import org.w3c.dom.Element;
import org.w3c.dom.NamedNodeMap;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;
import org.xml.sax.InputSource;

/** Conservative XML structural inference for Wiggle. */
public final class WiggleInference
{
    private WiggleInference() { }

    public static WiggleStructuralProfile infer(String xml) throws Exception
    {
        if (xml == null || xml.trim().length() == 0)
            throw new IllegalArgumentException("XML input is required");

        DocumentBuilderFactory factory = DocumentBuilderFactory.newInstance();
        factory.setNamespaceAware(true);
        try { factory.setFeature(XMLConstants.FEATURE_SECURE_PROCESSING, true); } catch (Exception ignored) { }
        try { factory.setFeature("http://apache.org/xml/features/disallow-doctype-decl", true); } catch (Exception ignored) { }
        try { factory.setFeature("http://xml.org/sax/features/external-general-entities", false); } catch (Exception ignored) { }
        try { factory.setFeature("http://xml.org/sax/features/external-parameter-entities", false); } catch (Exception ignored) { }
        factory.setXIncludeAware(false);
        factory.setExpandEntityReferences(false);

        DocumentBuilder builder = factory.newDocumentBuilder();
        return infer(builder.parse(new InputSource(new StringReader(xml))));
    }

    public static WiggleStructuralProfile infer(Document document)
    {
        if (document == null || document.getDocumentElement() == null)
            throw new IllegalArgumentException("XML document is required");

        Metrics metrics = new Metrics();
        walk(document.getDocumentElement(), 1, metrics);

        EnumMap<WiggleModel, Double> scores = new EnumMap<WiggleModel, Double>(WiggleModel.class);
        scores.put(WiggleModel.TREE, scoreTree(metrics));
        scores.put(WiggleModel.SEQUENCE, scoreSequence(metrics));
        scores.put(WiggleModel.COLLECTION, scoreCollection(metrics));
        scores.put(WiggleModel.RECORD, scoreRecord(metrics));
        scores.put(WiggleModel.REFERENCE, scoreReference(metrics));
        scores.put(WiggleModel.STATE, scoreState(metrics));
        scores.put(WiggleModel.COMMAND, scoreCommand(metrics));
        scores.put(WiggleModel.DESCRIPTION, scoreDescription(metrics));
        scores.put(WiggleModel.RELATION, scoreRelation(metrics));
        scores.put(WiggleModel.STREAM, scoreStream(metrics));

        return new WiggleStructuralProfile(scores, metrics.elementCount,
            metrics.attributeCount, metrics.maxDepth, metrics.repeatedKinds,
            metrics.referenceCandidates, Math.min(5, 1 + metrics.elementCount));
    }

    private static void walk(Element element, int depth, Metrics metrics)
    {
        metrics.elementCount++;
        metrics.maxDepth = Math.max(metrics.maxDepth, depth);
        metrics.names.add(element.getNodeName());

        NamedNodeMap attributes = element.getAttributes();
        metrics.attributeCount += attributes.getLength();
        for (int i = 0; i < attributes.getLength(); i++)
        {
            String name = attributes.item(i).getNodeName().toLowerCase();
            String value = attributes.item(i).getNodeValue();
            if (looksLikeReference(name, value)) metrics.referenceCandidates++;
            if (looksLikeState(name, value)) metrics.stateSignals++;
            if (looksLikeCommand(name, value)) metrics.commandSignals++;
            if (looksLikeDescription(name, value)) metrics.descriptionSignals++;
        }

        Map<String, Integer> childCounts = new HashMap<String, Integer>();
        NodeList children = element.getChildNodes();
        int elementChildren = 0;
        for (int i = 0; i < children.getLength(); i++)
        {
            Node node = children.item(i);
            if (node.getNodeType() != Node.ELEMENT_NODE) continue;
            elementChildren++;
            String name = node.getNodeName();
            Integer count = childCounts.get(name);
            childCounts.put(name, count == null ? 1 : count + 1);
            walk((Element) node, depth + 1, metrics);
        }
        for (Integer count : childCounts.values())
            if (count.intValue() > 1) metrics.repeatedKinds++;
        if (elementChildren > 1) metrics.orderedParents++;
        if (elementChildren == 2) metrics.relationSignals++;
        if (elementChildren > 3) metrics.recordSignals++;
    }

    private static boolean looksLikeReference(String name, String value)
    {
        return name.endsWith("id") || name.endsWith("ref") || name.endsWith("uri")
            || name.endsWith("url") || name.equals("name")
            || (value != null && (value.startsWith("urn:") || value.startsWith("http://") || value.startsWith("https://")));
    }

    private static boolean looksLikeState(String name, String value)
    {
        return name.contains("state") || name.contains("status") || name.contains("phase")
            || name.contains("mode") || name.contains("enabled") || name.contains("active")
            || "running".equalsIgnoreCase(value) || "stopped".equalsIgnoreCase(value);
    }

    private static boolean looksLikeCommand(String name, String value)
    {
        return name.contains("command") || name.contains("action") || name.contains("method")
            || name.contains("operation") || name.contains("verb") || "install".equalsIgnoreCase(value)
            || "connect".equalsIgnoreCase(value) || "start".equalsIgnoreCase(value);
    }

    private static boolean looksLikeDescription(String name, String value)
    {
        return name.contains("schema") || name.contains("descriptor") || name.contains("description")
            || name.contains("metadata") || name.contains("namespace") || name.contains("version");
    }

    private static double scoreTree(Metrics m) { return clamp(0.55 + Math.min(0.44, m.maxDepth * 0.08)); }
    private static double scoreSequence(Metrics m) { return clamp(0.35 + Math.min(0.60, m.orderedParents * 0.10)); }
    private static double scoreCollection(Metrics m) { return clamp(0.10 + Math.min(0.85, m.repeatedKinds * 0.22)); }
    private static double scoreRecord(Metrics m) { return clamp(0.25 + Math.min(0.70, m.recordSignals * 0.10 + m.attributeCount * 0.03)); }
    private static double scoreReference(Metrics m) { return clamp(0.05 + Math.min(0.90, m.referenceCandidates * 0.20)); }
    private static double scoreState(Metrics m) { return clamp(0.05 + Math.min(0.90, m.stateSignals * 0.22)); }
    private static double scoreCommand(Metrics m) { return clamp(0.05 + Math.min(0.90, m.commandSignals * 0.22)); }
    private static double scoreDescription(Metrics m) { return clamp(0.05 + Math.min(0.90, m.descriptionSignals * 0.22)); }
    private static double scoreRelation(Metrics m) { return clamp(0.10 + Math.min(0.80, m.relationSignals * 0.16 + m.referenceCandidates * 0.08)); }
    private static double scoreStream(Metrics m) { return clamp(0.05 + Math.min(0.85, m.repeatedKinds * 0.16 + m.orderedParents * 0.05)); }

    private static double clamp(double value) { return Math.max(0.0, Math.min(0.99, value)); }

    private static final class Metrics
    {
        int elementCount;
        int attributeCount;
        int maxDepth;
        int repeatedKinds;
        int orderedParents;
        int referenceCandidates;
        int stateSignals;
        int commandSignals;
        int descriptionSignals;
        int relationSignals;
        int recordSignals;
        final Set<String> names = new HashSet<String>();
    }
}
