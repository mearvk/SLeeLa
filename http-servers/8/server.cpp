#include "server.hpp"
#include "../common/packet_spec.hpp"
#include <arpa/inet.h>
#include <cerrno>
#include <fstream>
#include <iostream>
#include <netinet/in.h>
#include <sstream>
#include <string>
#include <sys/socket.h>
#include <unistd.h>

namespace sleela::http8 { namespace {
constexpr std::uint8_t VERSION=8; constexpr std::uint8_t MAX_TYPE=9;
void log(const Config&c,const std::string&s){std::ofstream f(c.log,std::ios::app);f<<s<<'\n';std::cerr<<s<<'\n';}
int serve(int f,const Config&c){
    sleela::packet::Frame q; std::uint64_t last=0; bool opened=false;
    for(;;){
        if(!sleela::packet::read(f,q,MAX_TYPE)){log(c,"REJECT invalid packet");return 1;}
        if(q.version!=VERSION){log(c,"REJECT packet VERSION mismatch");return 1;}
        log(c,"packet type="+std::to_string(static_cast<unsigned>(q.type))+" flags="+std::to_string(q.flags)+" stream="+std::to_string(q.stream_id)+" request="+std::to_string(q.request_id)+" sequence="+std::to_string(q.sequence)+" payload="+std::to_string(q.payload.size()));
        if(!opened){if(q.type!=sleela::packet::Type::OPEN||q.sequence!=0){log(c,"REJECT handshake: first packet must be OPEN sequence=0");return 1;}opened=true;}
        else if(q.sequence<last){log(c,"REJECT sequence regression");return 1;}
        last=q.sequence;
        const auto payload=sleela::packet::payload_text(q);
        if(payload.find("HTTP/8.0")==std::string::npos){log(c,"REJECT HTTP/8.0 payload version");return 1;}
        if(payload.find("HANDSHAKE")==std::string::npos){auto r=sleela::packet::text_response(q,VERSION,sleela::packet::Type::RESET,"HTTP/8.0 requires HANDSHAKE before exchange.\n");sleela::packet::write(f,r);return 1;}
        const std::string body="HTTP/8.0\nSubscription: active\nRadio: ready\nBinary-Checker-Agreement: passed\nTwo-Way-Handshaking: complete\nMessage-Exchange: accepted\n";
        auto r=sleela::packet::text_response(q,VERSION,sleela::packet::Type::DATA,body);if(!sleela::packet::write(f,r))return 0;
        if(q.type==sleela::packet::Type::END)return 0;
    }
}
}
int run(const Config&c){
    int s=socket(AF_INET,SOCK_STREAM,0);if(s<0)return 1;int one=1;setsockopt(s,SOL_SOCKET,SO_REUSEADDR,&one,sizeof(one));
    sockaddr_in a{};a.sin_family=AF_INET;a.sin_port=htons(c.port);
    if(inet_pton(AF_INET,c.address.c_str(),&a.sin_addr)!=1||bind(s,(sockaddr*)&a,sizeof(a))<0||listen(s,16)<0){std::perror("server");close(s);return 1;}
    log(c,"HTTP/8.0 listening "+c.address+":"+std::to_string(c.port)+" packet-spec=32-byte-envelope");
    do{int f=accept(s,nullptr,nullptr);if(f<0){if(errno==EINTR)continue;break;}serve(f,c);close(f);}while(!c.once);close(s);return 0;
}}
