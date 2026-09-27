#ifndef HTTP90_HPP
#define HTTP90_HPP
#include "http90.h"
#include <string>
namespace sleeLa::http90 {
class PacketMetadata {
public:
    PacketMetadata();
    int setIdentity(const std::string& policeId,const std::string& internationalId);
    int setPoliceScannerFrequency(const std::string& value,bool enabled);
    int setInternationalMonitoringFrequency(const std::string& value,bool enabled);
    int setNationalSignalFrequency(const std::string& emblem,const std::string& signal,const std::string& unit,const std::string& range,const std::string& jurisdiction,const std::string& source,const std::string& timestamp);
    int setInternationalData(const std::string& security,const std::string& safety,const std::string& police,const std::string& jurisdiction,const std::string& organization,const std::string& identifier,const std::string& classification,const std::string& source,const std::string& timestamp);
    int validate() const;
    bool canTransmit() const;
    bool canReceive() const;
    http90_packet_metadata& native(){return metadata_;}
    const http90_packet_metadata& native() const{return metadata_;}
private:
    http90_packet_metadata metadata_{};
};
}
#endif
