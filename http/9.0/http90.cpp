#include "http90.hpp"
namespace sleeLa::http90 {
PacketMetadata::PacketMetadata(){http90_init(&metadata_);}
int PacketMetadata::setIdentity(const std::string& p,const std::string& i){return http90_set_identity(&metadata_,p.c_str(),i.c_str());}
int PacketMetadata::setPoliceScannerFrequency(const std::string& v,bool e){return http90_set_frequency(&metadata_.monitoring.police_scanner_frequency,v.c_str(),e?1:0);}
int PacketMetadata::setInternationalMonitoringFrequency(const std::string& v,bool e){return http90_set_frequency(&metadata_.monitoring.international_police_monitoring_frequency,v.c_str(),e?1:0);}
int PacketMetadata::setNationalSignalFrequency(const std::string& emblem,const std::string& signal,const std::string& unit,const std::string& range,const std::string& jurisdiction,const std::string& source,const std::string& timestamp){return http90_set_national_signal_frequency(&metadata_,emblem.c_str(),signal.c_str(),unit.c_str(),range.c_str(),jurisdiction.c_str(),source.c_str(),timestamp.c_str());}
int PacketMetadata::setInternationalData(const std::string& security,const std::string& safety,const std::string& police,const std::string& jurisdiction,const std::string& organization,const std::string& identifier,const std::string& classification,const std::string& source,const std::string& timestamp){return http90_set_international_data(&metadata_,security.c_str(),safety.c_str(),police.c_str(),jurisdiction.c_str(),organization.c_str(),identifier.c_str(),classification.c_str(),source.c_str(),timestamp.c_str());}
int PacketMetadata::setDarkBand(const std::string& name,const std::string& originalContentHex){return http90_set_dark_band(&metadata_,name.c_str(),originalContentHex.c_str());}
int PacketMetadata::validate() const{return http90_validate(&metadata_);}
bool PacketMetadata::canTransmit() const{return http90_can_transmit(&metadata_)!=0;}
bool PacketMetadata::canReceive() const{return http90_can_receive(&metadata_)!=0;}
}