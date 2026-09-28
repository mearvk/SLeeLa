#ifndef SLEELA_AUDIO_HPP
#define SLEELA_AUDIO_HPP
#include "../c/Audio.h"
#include <string>
#include <vector>
namespace sleela::audio {
class AudioInput{public:std::string path;double startSeconds{0},gainDb{0};bool validate()const;};
class AudioControls{public:double bassDb{0},midDb{0},trebleDb{0},masterGainDb{0},pan{0},leftGain{1},rightGain{1};bool validate()const;};
class AudioConfiguration{public:int sampleRate{0};std::string outputPath;std::vector<AudioInput> inputs;AudioControls controls;bool validate()const;};
class AudioNative{public:explicit AudioNative(std::string executable={});bool validateExecutable()const;bool render(const AudioConfiguration&)const;void interrupt()const;private:std::string executable_;};
class AudioDevice{public:std::string name,identifier;int sampleRate{0},channels{0};bool input{false},output{false};bool available()const;};
class AudioStream{public:AudioDevice device;int sampleRate{0},channels{0};bool opened{false},running{false};bool open();bool start();void stop();void close();};
class AudioMixer{public:bool add(const AudioInput&);bool setControls(const AudioControls&);bool render(const std::string&);void reset();private:std::vector<AudioInput> inputs_;AudioControls controls_{};};
class AudioSystem{public:AudioSystem();std::string platform()const;std::vector<AudioDevice> enumerateDevices()const;AudioDevice defaultInput()const;AudioDevice defaultOutput()const;AudioStream createStream(const AudioDevice&,int,int)const;private:SleelaAudioSystem system_{};};
class Audio{public:explicit Audio(AudioConfiguration c={});bool validate()const;bool mix()const;void close();private:AudioConfiguration configuration_;};
}
#endif
