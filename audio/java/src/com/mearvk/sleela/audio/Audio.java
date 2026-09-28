package com.mearvk.sleela.audio;
import java.nio.file.Path; import java.util.List;
public interface Audio {
 record Input(String id,String source,double startSeconds,double gainDb){}
 record Controls(double bassDb,double midDb,double trebleDb,double masterGainDb,double pan,double leftGain,double rightGain){}
 record Configuration(int sampleRate,List<Input> inputs,Controls controls,Path output){}
 boolean validate(Configuration configuration);
 void process(Configuration configuration);
}
