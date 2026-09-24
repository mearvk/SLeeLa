package com.mearvk.sleela.skya;

import java.io.IOException;
import java.nio.charset.StandardCharsets;
import java.nio.file.*;
import java.util.*;
import java.util.regex.Matcher;
import java.util.regex.Pattern;
import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.transform.*;
import javax.xml.transform.dom.DOMSource;
import javax.xml.transform.stream.StreamResult;
import org.w3c.dom.*;

final class SkyaRoomListManager {
    record Room(String name, String host, int port, String dns, String description) {}

    private SkyaRoomListManager() {}

    static List<Room> defaults() {
        String[] names={"Lobby","General","Welcome","Announcements","Community","Friends","Family","Work","Study","Science","Mathematics","Engineering","Technology","Programming","Linux","Windows","Mac","Servers","Networking","Security","Design","Music","Movies","Books","Games","Sports","Travel","Cooking","Photography","Art","Nature","History","Language","News","Local","Events","Projects","Support","Help","Quiet"};
        var out=new ArrayList<Room>(); for(String n:names) out.add(new Room(n,"127.0.0.1",8443,"localhost",n+" room")); return out;
    }

    static void write(Path file,List<Room> rooms)throws Exception{
        String n=file.getFileName().toString().toLowerCase(Locale.ROOT);
        if(n.endsWith(".xml"))writeXml(file,rooms); else if(n.endsWith(".json"))writeJson(file,rooms); else writeText(file,rooms,n.endsWith(".md"));
    }
    static List<Room> read(Path file)throws Exception{
        String n=file.getFileName().toString().toLowerCase(Locale.ROOT);
        if(n.endsWith(".xml"))return readXml(file); if(n.endsWith(".json"))return readJson(file); return readText(file);
    }
    private static void writeXml(Path f,List<Room> rs)throws Exception{
        var d=DocumentBuilderFactory.newInstance().newDocumentBuilder().newDocument(); var root=d.createElement("skya-room-list"); d.appendChild(root);
        for(Room r:rs){var e=d.createElement("room");root.appendChild(e);put(e,"name",r.name());put(e,"host",r.host());put(e,"port",""+r.port());put(e,"dns",r.dns());put(e,"description",r.description());}
        var t=TransformerFactory.newInstance().newTransformer();t.setOutputProperty(OutputKeys.INDENT,"yes");t.transform(new DOMSource(d),new StreamResult(f.toFile()));
    }
    private static void put(Element p,String k,String v){var e=p.getOwnerDocument().createElement(k);e.setTextContent(v==null?"":v);p.appendChild(e);}
    private static List<Room> readXml(Path f)throws Exception{
        var d=DocumentBuilderFactory.newInstance().newDocumentBuilder().parse(f.toFile());var out=new ArrayList<Room>();var ns=d.getElementsByTagName("room");
        for(int i=0;i<ns.getLength();i++){var e=(Element)ns.item(i);out.add(room(text(e,"name"),text(e,"host"),text(e,"port"),text(e,"dns"),text(e,"description")));}return out;
    }
    private static String text(Element e,String n){var x=e.getElementsByTagName(n);return x.getLength()==0?"":x.item(0).getTextContent();}
    private static void writeJson(Path f,List<Room> rs)throws IOException{
        try(var w=Files.newBufferedWriter(f,StandardCharsets.UTF_8)){w.write("{\n  \"rooms\": [\n");for(int i=0;i<rs.size();i++){Room r=rs.get(i);w.write("    {\"name\":\""+j(r.name())+"\",\"host\":\""+j(r.host())+"\",\"port\":"+r.port()+",\"dns\":\""+j(r.dns())+"\",\"description\":\""+j(r.description())+"\"}"+(i+1<rs.size()?",":"")+"\n");}w.write("  ]\n}\n");}
    }
    private static String j(String s){return s==null?"":s.replace("\\","\\\\").replace("\"","\\\"").replace("\n","\\n").replace("\r","\\r");}
    private static List<Room> readJson(Path f)throws IOException{
        String s=Files.readString(f);
        var out=new ArrayList<Room>();
        Pattern p=Pattern.compile("\"name\"\\s*:\\s*\"([^\"]*)\"\\s*,\\s*\"host\"\\s*:\\s*\"([^\"]*)\"\\s*,\\s*\"port\"\\s*:\\s*(\\d+)\\s*,\\s*\"dns\"\\s*:\\s*\"([^\"]*)\"\\s*,\\s*\"description\"\\s*:\\s*\"([^\"]*)\"");
        Matcher m=p.matcher(s);
        while(m.find()) out.add(new Room(un(m.group(1)),un(m.group(2)),Integer.parseInt(m.group(3)),un(m.group(4)),un(m.group(5))));
        return out;
    }
    private static String un(String s){return s.replace("\\\"","\"").replace("\\\\","\\").replace("\\n","\n").replace("\\r","\r");}
    private static void writeText(Path f,List<Room> rs,boolean md)throws IOException{
        try(var w=Files.newBufferedWriter(f,StandardCharsets.UTF_8)){if(md)w.write("# Skya Room List\n\n| Room | Host/IP | Port | DNS | Description |\n|---|---|---:|---|---|\n");else w.write("# Skya Room List\n# name|host|port|dns|description\n");for(Room r:rs)w.write(md?"| "+r.name()+" | "+r.host()+" | "+r.port()+" | "+r.dns()+" | "+r.description()+" |\n":r.name()+"|"+r.host()+"|"+r.port()+"|"+r.dns()+"|"+r.description()+"\n");}
    }
    private static List<Room> readText(Path f)throws IOException{
        var out=new ArrayList<Room>();for(String line:Files.readAllLines(f,StandardCharsets.UTF_8)){line=line.trim();if(line.isEmpty()||line.startsWith("#")||line.startsWith("|---")||line.startsWith("| Room"))continue;if(line.startsWith("|"))line=line.substring(1,line.length()-1).trim().replace(" | ","|");String[] a=line.split("\\|",5);if(a.length==5)out.add(room(a[0],a[1],a[2],a[3],a[4]));}return out;
    }
    private static Room room(String n,String h,String p,String d,String x){int port;try{port=Integer.parseInt(p.trim());}catch(Exception e){port=8443;}return new Room(n.trim(),h.trim(),port,d.trim(),x.trim());}
}
