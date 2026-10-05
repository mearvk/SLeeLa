#include "Audio.hpp"
#include <cstdio>
#include <utility>
namespace sleela::audio {
bool AudioInput::validate()const{SleelaAudioInput x{path.c_str(),startSeconds,gainDb};return sleela_audio_validate_input(&x)!=0;}
bool AudioControls::validate()const{SleelaAudioControls x{bassDb,midDb,trebleDb,masterGainDb,pan,leftGain,rightGain};return sleela_audio_validate_controls(&x)!=0;}
bool AudioConfiguration::validate()const{if(sampleRate<=0||outputPath.empty()||inputs.empty()||inputs.size()>128||!controls.validate())return false;for(const auto&i:inputs)if(!i.validate())return false;return true;}
AudioNative::AudioNative(std::string x):executable_(std::move(x)){}
bool AudioNative::validateExecutable()const{return !executable_.empty();}
bool AudioNative::render(const AudioConfiguration&c)const{
 if(!validateExecutable()||!c.validate())return false;
 std::vector<SleelaAudioInput> in;in.reserve(c.inputs.size());for(const auto&i:c.inputs)in.push_back({i.path.c_str(),i.startSeconds,i.gainDb});
 SleelaAudioControls ctl{c.controls.bassDb,c.controls.midDb,c.controls.trebleDb,c.controls.masterGainDb,c.controls.pan,c.controls.leftGain,c.controls.rightGain};
 SleelaAudioConfiguration x{c.sampleRate,c.outputPath.c_str(),in.data(),in.size(),ctl};return sleela_audio_native_render(executable_.c_str(),&x)!=0;
}
void AudioNative::interrupt()const{sleela_audio_native_interrupt();}
bool AudioDevice::available()const{return !identifier.empty();}
bool AudioStream::open(){SleelaAudioDevice d{};std::snprintf(d.identifier,sizeof(d.identifier),"%s",device.identifier.c_str());d.sample_rate=device.sampleRate;d.channels=device.channels;d.input=device.input;d.output=device.output;SleelaAudioStream x{d,sampleRate,channels,0,0};if(!sleela_audio_stream_open(&x))return false;opened=x.opened;running=x.running;return true;}
bool AudioStream::start(){if(!opened)return false;running=true;return true;}void AudioStream::stop(){running=false;}void AudioStream::close(){running=false;opened=false;}
bool AudioMixer::add(const AudioInput&i){if(!i.validate()||inputs_.size()>=128)return false;inputs_.push_back(i);return true;}
bool AudioMixer::setControls(const AudioControls&c){if(!c.validate())return false;controls_=c;return true;}
bool AudioMixer::render(const std::string&o){AudioConfiguration c;c.sampleRate=48000;c.outputPath=o;c.inputs=inputs_;c.controls=controls_;return c.validate();}
void AudioMixer::reset(){inputs_.clear();controls_=AudioControls{};}
AudioSystem::AudioSystem(){sleela_audio_system_init(&system_);}std::string AudioSystem::platform()const{return sleela_audio_system_platform(&system_);}std::vector<AudioDevice> AudioSystem::enumerateDevices()const{return {};}AudioDevice AudioSystem::defaultInput()const{return {};}AudioDevice AudioSystem::defaultOutput()const{return {};}AudioStream AudioSystem::createStream(const AudioDevice&d,int r,int c)const{AudioStream s;s.device=d;s.sampleRate=r;s.channels=c;return s;}
Audio::Audio(AudioConfiguration c):configuration_(std::move(c)){}bool Audio::validate()const{return configuration_.validate();}bool Audio::mix()const{return configuration_.validate();}void Audio::close(){}
}
