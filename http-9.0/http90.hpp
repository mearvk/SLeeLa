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
