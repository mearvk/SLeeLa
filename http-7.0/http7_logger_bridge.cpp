#include "http7_logger_bridge.h"
#include "../logger/sleela_logger.hpp"
#include <cstdint>
#include <mutex>
namespace {
std::mutex bridge_mutex;
sleela::Logger logger("logs/http7");
bool opened=false;
}
extern "C" int h7_logger_open(void){
 std::lock_guard<std::mutex> lock(bridge_mutex);
 if(opened)return 1;
 sleela::LoggerLogic logic;
 logic.log_received=true;logic.log_sent=true;
 logic.minimum_severity=sleela::LogSeverity::Info;
 logic.include_payload=false;
 logger.set_logic(logic);
 opened=logger.open();return opened?1:0;
}
extern "C" void h7_logger_close(void){
 std::lock_guard<std::mutex> lock(bridge_mutex);
 if (opened) logger.close();\n opened=false;
}
extern "C" int h7_logger_record(int sent,int admission,size_t bytes,const char*reason){
 std::lock_guard<std::mutex> lock(bridge_mutex);
 if(!opened)return 0;
 sleela::PacketRecord r;
 r.direction=sent?sleela::LogDirection::Sent:sleela::LogDirection::Received;
 r.severity=admission==2?sleela::LogSeverity::Error:(admission==1?sleela::LogSeverity::Warning:sleela::LogSeverity::Info);
 r.admission=admission==2?sleela::LogAdmission::Error:(admission==1?sleela::LogAdmission::Rejected:sleela::LogAdmission::Accepted);
 r.protocol="HTTP/7.0";r.source=sent?"sleela-http7":"unknown-peer";r.destination=sent?"unknown-peer":"sleela-http7";
 r.packet_bytes=static_cast<std::uint64_t>(bytes);r.reason=reason?reason:"unspecified";
 return logger.record(r)?1:0;
}
