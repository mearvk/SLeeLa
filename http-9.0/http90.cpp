/* Module path mirror of http/9.0/http90.cpp; canonical source remains http/9.0/http90.cpp. */
#include "http90.hpp"
namespace sleeLa::http90 {
PacketMetadata::PacketMetadata(){http90_init(&metadata_);}
int PacketMetadata::setIdentity(const std::string& p,const std::string& i){return http90_set_identity(&metadata_,p.c_str(),i.c_str());}
int PacketMetadata::setPoliceScannerFrequency(const std::string& v,bool e){return http90_set_frequency(&metadata_.monitoring.police_scanner_frequency,v.c_str(),e?1:0);}
int PacketMetadata::setInternationalMonitoringFrequency(const std::string& v,bool e){return http90_set_frequency(&metadata_.monitoring.international_police_monitoring_frequency,v.c_str(),e?1:0);}
int PacketMetadata::validate() const{return http90_validate(&metadata_);}
bool PacketMetadata::canTransmit() const{return http90_can_transmit(&metadata_)!=0;}
bool PacketMetadata::canReceive() const{return http90_can_receive(&metadata_)!=0;}
}
