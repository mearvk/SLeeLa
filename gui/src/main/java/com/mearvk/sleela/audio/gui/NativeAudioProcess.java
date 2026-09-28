package com.mearvk.sleela.audio.gui;

import java.io.IOException;
import java.nio.file.Path;
import java.util.ArrayList;
import java.util.List;

public final class NativeAudioProcess {
    private final Path executable;
    public NativeAudioProcess(Path executable) { this.executable=executable.toAbsolutePath().normalize(); }
    public void process(Path output,List<Path> inputs) {
        if(inputs==null||inputs.isEmpty()) throw new IllegalArgumentException("inputs");
        List<String> command=new ArrayList<>();
        command.add(executable.toString()); command.add(output.toString()); inputs.forEach(p->command.add(p.toString()));
        try {
            Process p=new ProcessBuilder(command).redirectErrorStream(true).start();
            String diagnostic=new String(p.getInputStream().readAllBytes());
            int exit=p.waitFor();
            if(exit!=0) throw new IllegalStateException("Native audio backend exited "+exit+": "+diagnostic.trim());
        } catch(IOException e){throw new IllegalStateException("Unable to start native audio backend",e);}
          catch(InterruptedException e){Thread.currentThread().interrupt();throw new IllegalStateException("Native audio processing interrupted",e);}
    }
}