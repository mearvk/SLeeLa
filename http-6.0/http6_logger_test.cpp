#include "http6_logger_bridge.h"
#include <cassert>
#include <filesystem>
#include <fstream>
#include <iterator>
#include <string>
int main() {
    std::filesystem::remove_all("logs/http6");
    assert(h6_logger_open() == 1);
    assert(h6_logger_record(0, 0, 123, "http_request_received") == 1);
    assert(h6_logger_record(0, 1, 47, "http_request_parse_rejected") == 1);
    assert(h6_logger_record(1, 0, 89, "http_response_before_socket_write") == 1);
    h6_logger_close();
    const std::filesystem::path path("logs/http6/packets-000000.jsonl");
    assert(std::filesystem::exists(path));
    std::ifstream input(path, std::ios::binary);
    const std::string data((std::istreambuf_iterator<char>(input)), std::istreambuf_iterator<char>());
    assert(data.find("SLeeLa-PacketLog-1") != std::string::npos);
    assert(data.find("http_request_received") != std::string::npos);
    assert(data.find("http_request_parse_rejected") != std::string::npos);
    assert(data.find("http_response_before_socket_write") != std::string::npos);
    assert(data.find("payload_base64") == std::string::npos);
    std::filesystem::remove_all("logs/http6");
    return 0;
}
