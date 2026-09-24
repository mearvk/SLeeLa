package com.mearvk.sleela.skya;

import javafx.application.Application;
import javafx.application.Platform;
import javafx.geometry.Insets;
import javafx.scene.Scene;
import javafx.scene.control.*;
import javafx.scene.layout.*;
import javafx.stage.Stage;
import java.io.*;
import java.net.*;
import java.nio.charset.StandardCharsets;

public final class SkyaConnectApp extends Application {
    private final TextField host=new TextField("127.0.0.1"), port=new TextField("8443"), room=new TextField("lobby");
    private final ComboBox<String> protocol=new ComboBox<>();
    private final TextField message=new TextField();
    private final TextArea log=new TextArea();
    private Label status;
    private Socket socket;
    private BufferedReader reader;
    private BufferedWriter writer;
    private volatile boolean listenerPaused;

    @Override public void start(Stage stage) {
        stage.setTitle("Skya — SLeeLa Remote Connection");
        protocol.getItems().addAll("HTTP/3","HTTP/2","SLeeLa TCP"); protocol.setValue("HTTP/3");
        log.setEditable(false); status=new Label("Disconnected");
        Button start=new Button("Start"), pause=new Button("Pause"), stop=new Button("Stop"), send=new Button("Send");
        start.setOnAction(e->connect()); pause.setOnAction(e->pauseConnection()); stop.setOnAction(e->disconnect()); send.setOnAction(e->sendMessage());
        GridPane endpoint=new GridPane(); endpoint.setHgap(8); endpoint.setVgap(8);
        endpoint.addRow(0,new Label("Remote server"),host); endpoint.addRow(1,new Label("Port"),port);
        endpoint.addRow(2,new Label("Room"),room); endpoint.addRow(3,new Label("Protocol"),protocol);
        HBox messaging=new HBox(8,message,send); HBox.setHgrow(message,Priority.ALWAYS);
        VBox root=new VBox(10,new Label("Skya Remote Server Connection"),endpoint,new HBox(8,start,pause,stop),
            new Separator(),new Label("Connection status"),status,new Label("Remote communication"),messaging,log);
        root.setPadding(new Insets(14)); VBox.setVgrow(log,Priority.ALWAYS);
        stage.setScene(new Scene(root,760,560)); stage.show();
    }

    private void connect() {
        disconnect();
        final String h=host.getText().trim(), r=room.getText().trim(); final int p;
        try{p=Integer.parseInt(port.getText().trim());if(p<1||p>65535)throw new NumberFormatException();}
        catch(NumberFormatException e){status.setText("Invalid port");return;}
        if(h.isEmpty()||r.isEmpty()){status.setText("Server and room are required");return;}
        listenerPaused=false; status.setText("Connecting to "+h+":"+p);
        append("CLIENT.CONNECT "+h+":"+p+" room="+r+" protocol="+protocol.getValue());
        Thread worker=new Thread(()->{
            try{
                Socket s=new Socket();s.connect(new InetSocketAddress(h,p),5000);
                BufferedReader rd=new BufferedReader(new InputStreamReader(s.getInputStream(),StandardCharsets.UTF_8));
                BufferedWriter wr=new BufferedWriter(new OutputStreamWriter(s.getOutputStream(),StandardCharsets.UTF_8));
                synchronized(this){socket=s;reader=rd;writer=wr;}
                wr.write("SKYA/1 client-hello room="+r+" protocol="+protocol.getValue());wr.newLine();wr.flush();
                Platform.runLater(()->{status.setText("Connected: "+h+":"+p);append("CLIENT.CONNECTED");append("SESSION.OPEN room="+r);append("LISTENER.START");});
                String line;while((line=rd.readLine())!=null){final String x=line;if(!listenerPaused)Platform.runLater(()->append("LISTENER.RECEIVE "+x));}
            }catch(IOException e){Platform.runLater(()->{status.setText("Connection failed: "+e.getMessage());append("CLIENT.ERROR "+e.getMessage());});}
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
        listenerPaused=false;close(reader);close(writer);close(socket);reader=null;writer=null;socket=null;
        if(status!=null){status.setText("Disconnected");append("LISTENER.STOP");append("CLIENT.DISCONNECTED");append("SESSION.CLOSED");}
    }
    private void append(String line){if(Platform.isFxApplicationThread())log.appendText(line+System.lineSeparator());else Platform.runLater(()->append(line));}
    private static void close(Closeable c){if(c!=null)try{c.close();}catch(IOException ignored){}}
    @Override public void stop(){disconnect();}
    public static void main(String[] args){launch(args);}
}
