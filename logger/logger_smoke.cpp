#include "sleela_logger.hpp"
#include <cassert>
#include <filesystem>
int main() {
    std::filesystem::remove_all("logs");
    sleela::Logger logger("logs");
    sleela::LoggerLogic logic;
    logic.log_sent = false;
    logic.log_received = true;
    logic.minimum_severity = sleela::LogSeverity::Info;
    logic.minimum_admission = sleela::LogAdmission::Accepted;
    logic.protocol_prefix = "HTTP/";
    logic.maximum_packet_bytes = 1024 * 1024;
    logger.set_logic(logic);
    assert(logger.open());
    sleela::PacketRecord received;
    received.direction = sleela::LogDirection::Received;
    received.protocol = "HTTP/8";
    received.service = "server";
    received.operation = "request";
    received.source = "192.0.2.10";
    received.destination = "198.51.100.20";
    received.sequence = 1;
    received.packet_bytes = 512;
    received.reason = "accepted";
    assert(logger.record(received));
    sleela::PacketRecord sent = received;
    sent.direction = sleela::LogDirection::Sent;
    assert(logger.record(sent));
    assert(logger.current_segment_bytes() > 0);
    logger.close();
    assert(std::filesystem::exists("logs/packets-000000.jsonl"));
    return 0;
}
