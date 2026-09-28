package com.mearvk.sleela.skya;

import javafx.application.Application;
import javafx.application.Platform;
import javafx.geometry.Insets;
import javafx.scene.Scene;
import javafx.scene.control.*;
import javafx.scene.layout.*;
import javafx.stage.Stage;
import javafx.stage.FileChooser;
import java.util.*;
import java.io.*;
import java.net.*;
import java.nio.charset.StandardCharsets;

public final class SkyaConnectApp extends Application {
    private final TextField host=new TextField("127.0.0.1"), port=new TextField("8443");
    private final ComboBox<String> room=new ComboBox<>();
    private final List<SkyaRoomListManager.Room> rooms=new ArrayList<>();
    private final ComboBox<String> protocol=new ComboBox<>();
    private final TextField message=new TextField();
    private final TextArea log=new TextArea();
    private Label status;
    private Socket socket;
    private BufferedReader reader;
    private BufferedWriter writer;
    private volatile boolean listenerPaused;
    private volatile long connectionAttempt;
    private static final int CONNECT_TIMEOUT_MS=5000;

    @Override public void start(Stage stage) {
        stage.setTitle("Skya — SLeeLa Remote Connection");
        protocol.getItems().addAll("HTTP/3","HTTP/2","SLeeLa TCP"); protocol.setValue("HTTP/3");
        rooms.addAll(SkyaRoomListManager.defaults()); for(var x:rooms)room.getItems().add(x.name()); room.setValue("Lobby"); room.setOnAction(e->selectRoom());
        log.setEditable(false); status=new Label("Disconnected");
        Button start=new Button("Start"), pause=new Button("Pause"), stop=new Button("Stop"), send=new Button("Send");
        start.setOnAction(e->connect()); pause.setOnAction(e->pauseConnection()); stop.setOnAction(e->disconnect()); send.setOnAction(e->sendMessage());
        GridPane endpoint=new GridPane(); endpoint.setHgap(8); endpoint.setVgap(8);
        endpoint.addRow(0,new Label("Remote server"),host); endpoint.addRow(1,new Label("Port"),port);
        endpoint.addRow(2,new Label("Room"),room); endpoint.addRow(3,new Label("Protocol"),protocol);
        MenuBar menu=createMenu(stage);
        HBox messaging=new HBox(8,message,send); HBox.setHgrow(message,Priority.ALWAYS);
        VBox root=new VBox(10,menu,new Label("Skya Remote Server Connection"),endpoint,new HBox(8,start,pause,stop),
            new Separator(),new Label("Connection status"),status,new Label("Remote communication"),messaging,log);
        root.setPadding(new Insets(14)); VBox.setVgrow(log,Priority.ALWAYS);
        stage.setScene(new Scene(root,760,560)); stage.show();
    }


    private void selectRoom(){
        String name=room.getValue(); if(name==null)return;
        for(var x:rooms) if(x.name().equals(name)){host.setText(x.host());port.setText(Integer.toString(x.port()));append("ROOM.SELECTED "+x.name()+" host="+x.host()+" dns="+x.dns()+" port="+x.port());break;}
    }
    private MenuBar createMenu(Stage stage){
        Menu m=new Menu("Rooms");
        MenuItem add=new MenuItem("Add Room"), remove=new MenuItem("Remove Selected"), imp=new MenuItem("Import Room List"), exp=new MenuItem("Export Room List");
        add.setOnAction(e->addRoom()); remove.setOnAction(e->removeRoom()); imp.setOnAction(e->importRooms(stage)); exp.setOnAction(e->exportRooms(stage));
        m.getItems().addAll(add,remove,new SeparatorMenuItem(),imp,exp);
        Menu help=new Menu("Help"); MenuItem about=new MenuItem("About Skya Rooms");
        about.setOnAction(e->new Alert(Alert.AlertType.INFORMATION,"Skya room lists support XML, JSON, TXT and Markdown (MD). Entries carry room name, IP/host, port, DNS name and description.").showAndWait());
        help.getItems().add(about); return new MenuBar(m,help);
    }
    private void addRoom(){
        String n=prompt("Add Room","Room name","New Room"); if(n==null)return;
        String h=prompt("Add Room","IP or host",host.getText()); if(h==null)return;
        String ps=prompt("Add Room","Port",port.getText()); if(ps==null)return;
        int p; try{p=Integer.parseInt(ps);if(p<1||p>65535)throw new Exception();}catch(Exception e){status.setText("Invalid room port");return;}
        String d=prompt("Add Room","DNS name (optional)",h); if(d==null)return;
        String desc=prompt("Add Room","Description (optional)",n+" room"); if(desc==null)return;
        rooms.removeIf(x->x.name().equalsIgnoreCase(n)); rooms.add(new SkyaRoomListManager.Room(n,h,p,d,desc));
        room.getItems().remove(n); room.getItems().add(n); room.setValue(n); selectRoom();
    }
    private String prompt(String title,String header,String initial){TextInputDialog d=new TextInputDialog(initial);d.setTitle(title);d.setHeaderText(header);return d.showAndWait().map(String::trim).filter(x->!x.isBlank()).orElse(null);}
    private void removeRoom(){String n=room.getValue();if(n==null)return;if(rooms.size()<=1){status.setText("At least one room must remain");return;}rooms.removeIf(x->x.name().equals(n));room.getItems().remove(n);room.setValue(room.getItems().get(0));selectRoom();}
    private void exportRooms(Stage stage){FileChooser fc=new FileChooser();fc.setTitle("Export Skya Room List");fc.getExtensionFilters().addAll(new FileChooser.ExtensionFilter("Skya Room List","*.xml","*.json","*.txt","*.md"));File f=fc.showSaveDialog(stage);if(f==null)return;try{SkyaRoomListManager.write(f.toPath(),rooms);status.setText("Room list exported: "+f.getName());append("ROOM.LIST.EXPORTED "+f);}catch(Exception e){new Alert(Alert.AlertType.ERROR,"Export failed: "+e.getMessage()).showAndWait();}}
    private void importRooms(Stage stage){FileChooser fc=new FileChooser();fc.setTitle("Import Skya Room List");fc.getExtensionFilters().addAll(new FileChooser.ExtensionFilter("Skya Room List","*.xml","*.json","*.txt","*.md"));File f=fc.showOpenDialog(stage);if(f==null)return;try{var loaded=SkyaRoomListManager.read(f.toPath());if(loaded.isEmpty())throw new IOException("No rooms found");rooms.clear();rooms.addAll(loaded);room.getItems().clear();for(var x:rooms)room.getItems().add(x.name());room.setValue(room.getItems().get(0));selectRoom();status.setText("Room list imported: "+f.getName());append("ROOM.LIST.IMPORTED "+f);}catch(Exception e){new Alert(Alert.AlertType.ERROR,"Import failed: "+e.getMessage()).showAndWait();}}

    private void connect() {
        disconnect();
        final String h=host.getText().trim(), r=room.getValue()==null?"":room.getValue().trim(); final int p;
        try{p=Integer.parseInt(port.getText().trim());if(p<1||p>65535)throw new NumberFormatException();}
        catch(NumberFormatException e){status.setText("Invalid port");return;}
        if(h.isEmpty()||r.isEmpty()){status.setText("Server and room are required");return;}
        listenerPaused=false; final long attempt=++connectionAttempt; status.setText("Connecting to "+h+":"+p+" (timeout 5s)");
        append("CLIENT.CONNECT "+h+":"+p+" room="+r+" protocol="+protocol.getValue());
        Thread worker=new Thread(()->{
            try{
                Socket s=new Socket();
                s.connect(new InetSocketAddress(h,p),CONNECT_TIMEOUT_MS);
                BufferedReader rd=new BufferedReader(new InputStreamReader(s.getInputStream(),StandardCharsets.UTF_8));
                BufferedWriter wr=new BufferedWriter(new OutputStreamWriter(s.getOutputStream(),StandardCharsets.UTF_8));
                synchronized(this){socket=s;reader=rd;writer=wr;}
                wr.write("SKYA/1 client-hello room="+r+" protocol="+protocol.getValue());wr.newLine();wr.flush();
                Platform.runLater(()->{if(attempt!=connectionAttempt)return;status.setText("Connected: "+h+":"+p);append("CLIENT.CONNECTED");append("SESSION.OPEN room="+r);append("LISTENER.START");});
                String line;while((line=rd.readLine())!=null){final String x=line;if(!listenerPaused)Platform.runLater(()->append("LISTENER.RECEIVE "+x));}
            }catch(SocketTimeoutException e){Platform.runLater(()->{if(attempt!=connectionAttempt)return;status.setText("Connection timeout after 5s");append("CLIENT.TIMEOUT after 5s");});
            }catch(ConnectException e){Platform.runLater(()->{if(attempt!=connectionAttempt)return;status.setText("Connection failed: "+e.getMessage());append("CLIENT.ERROR "+e.getMessage());});
            }catch(IOException e){Platform.runLater(()->{if(attempt!=connectionAttempt)return;status.setText("Connection failed: "+e.getMessage());append("CLIENT.ERROR "+e.getMessage());});}
        },"skya-remote-connection");
        worker.setDaemon(true);worker.start();
    }

    private synchronized void pauseConnection(){
        if(socket==null||socket.isClosed()){status.setText("Disconnected");append("LISTENER.PAUSE — not connected");return;}
        listenerPaused=true;status.setText("Paused: connection retained");append("LISTENER.PAUSE");append("SESSION.SUSPENDED");
    }
    private synchronized void sendMessage(){
        String text=message.getText();if(text==null||text.isBlank())return;
        if(writer==null||socket==null||socket.isClosed()){append("COMMAND.ERROR not connected");return;}
        try{writer.write(text);writer.newLine();writer.flush();append("COMMAND.INVOKE "+text);message.clear();}
        catch(IOException e){append("TRANSPORT.ERROR "+e.getMessage());disconnect();}
    }
    private synchronized void disconnect(){
        connectionAttempt++;
        listenerPaused=false;close(reader);close(writer);close(socket);reader=null;writer=null;socket=null;
        if(status!=null){status.setText("Disconnected");append("LISTENER.STOP");append("CLIENT.DISCONNECTED");append("SESSION.CLOSED");}
    }
    private void append(String line){if(Platform.isFxApplicationThread())log.appendText(line+System.lineSeparator());else Platform.runLater(()->append(line));}
    private static void close(Closeable c){if(c!=null)try{c.close();}catch(IOException ignored){}}
    @Override public void stop(){disconnect();}
    public static void main(String[] args){launch(args);}
}
