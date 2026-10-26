#include "../sleela_server.hpp"
#include <cassert>
#include <iostream>
#include <string>

using sleela::server::Packet;
using sleela::server::PacketInspector;
using sleela::server::SleelaServer;
using sleela::server::ServiceResponse;
using sleela::server::SleelaServer;

int main() {
    {
        Packet p;
        std::string error;
        const std::string valid =
            "PROTOCOL-GRADE=1\nSERVICE-ID=health\nOP-ID=ping\nREQUEST-ID=42\n";
        assert(PacketInspector::parse_headers(valid, p, error));
        assert(p.grade == 1);
        assert(p.service_id == "health");
        assert(p.operation_id == "ping");
        assert(p.request_id == 42);
    }
    {
        Packet p;
        p.service_id = "original";
        std::string error;
        const std::string invalid =
            "PROTOCOL-GRADE=1\nSERVICE-ID=changed\nOP-ID=ping\nREQUEST-ID=18446744073709551616\n";
        assert(!PacketInspector::parse_headers(invalid, p, error));
        assert(p.service_id == "original"); // Parse failure must not partially mutate caller state.
    }
    {
        Packet p;
        std::string error;
        const std::string duplicate =
            "PROTOCOL-GRADE=1\nSERVICE-ID=one\nSERVICE-ID=two\nOP-ID=ping\n";
        assert(!PacketInspector::parse_headers(duplicate, p, error));
        assert(error == "duplicate_header");
    }
    {
        Packet p;
        std::string error;
        const std::string invalid_name =
            "PROTOCOL-GRADE=1\nSERVICE ID=health\nOP-ID=ping\n";
        assert(!PacketInspector::parse_headers(invalid_name, p, error));
    }
    {
        SleelaServer server;
        assert(server.register_handler("health", "ping", [](const Packet&) { return ServiceResponse{200, "pong"}; }));
        assert(!server.register_handler("health", "ping", [](const Packet&) { return ServiceResponse{200, "duplicate"}; }));
        assert(!server.register_handler("", "ping", [](const Packet&) { return ServiceResponse{200, "invalid"}; }));
    }
    std::cout << "server_protocol_test: PASS\n";
}
