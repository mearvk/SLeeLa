package implementations._001_.bodi;

import org.w3c.dom.Document;
import org.w3c.dom.Element;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;

import javax.xml.XMLConstants;
import javax.xml.parsers.DocumentBuilderFactory;
import java.io.ByteArrayInputStream;
import java.nio.charset.StandardCharsets;
import java.util.ArrayList;
import java.util.List;

/** Minimal agreed XML representation for Bodi network requests. */
public final class BodiXmlDocument
{
    private BodiXmlDocument() { }

    public static BodiChange parse(String xml) throws Exception
    {
        DocumentBuilderFactory factory = DocumentBuilderFactory.newInstance();
        factory.setFeature("http://apache.org/xml/features/disallow-doctype-decl", true);
        factory.setFeature("http://xml.org/sax/features/external-general-entities", false);
        factory.setFeature("http://xml.org/sax/features/external-parameter-entities", false);
        factory.setXIncludeAware(false);
        factory.setExpandEntityReferences(false);
        try { factory.setAttribute(XMLConstants.ACCESS_EXTERNAL_DTD, ""); } catch (IllegalArgumentException ignored) { }
        try { factory.setAttribute(XMLConstants.ACCESS_EXTERNAL_SCHEMA, ""); } catch (IllegalArgumentException ignored) { }

        Document document = factory.newDocumentBuilder().parse(
            new ByteArrayInputStream(xml.getBytes(StandardCharsets.UTF_8)));
        Element root = document.getDocumentElement();
        if (root == null || !"bodi".equals(root.getTagName()))
            throw new IllegalArgumentException("Bodi XML root must be <bodi>");

        String system = root.getAttribute("system");
        String method = root.getAttribute("method");
        String sequence = root.getAttribute("sequence");
        String starter = root.getAttribute("starter");
        String man = root.getAttribute("man");
        String datum = textOf(root, "datum");
        if (datum.length() == 0)
            datum = orderedText(root);

        return new BodiChange(system, method, datum, sequence, starter, man);
    }

    public static String response(String status, String message)
    {
        return "<bodi-response status=\"" + escape(status) + "\"><message>" +
            escape(message) + "</message></bodi-response>";
    }

    private static String textOf(Element root, String name)
    {
        NodeList nodes = root.getElementsByTagName(name);
        return nodes.getLength() == 0 ? "" : nodes.item(0).getTextContent().trim();
    }

    private static String orderedText(Element root)
    {
        List<String> values = new ArrayList<String>();
        NodeList children = root.getChildNodes();
        for (int i = 0; i < children.getLength(); i++)
        {
            Node child = children.item(i);
            if (child.getNodeType() == Node.ELEMENT_NODE)
            {
                String text = child.getTextContent().trim();
                if (text.length() > 0)
                    values.add(text);
            }
        }
        return String.join("\n", values);
    }

    private static String escape(String value)
    {
        return value == null ? "" : value.replace("&", "&amp;")
            .replace("<", "&lt;").replace(">", "&gt;")
            .replace("\"", "&quot;").replace("'", "&apos;");
    }
}
