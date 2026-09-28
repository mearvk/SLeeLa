#pragma once
#include <array>
#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>
#include <sys/socket.h>

namespace sleela::packet {
constexpr std::size_t kHeaderSize = 32;
constexpr std::uint32_t kMaxPayload = 16u * 1024u * 1024u;
enum class Type : std::uint8_t {
    OPEN=1, DATA=2, END=3, RESET=4, WINDOW=5, PING=6, PONG=7, RESUME=8, CAPSULE=9,
    FRIENDS_PACK=10, BONUS_OFFER=11, FP_UPDATE=12, AUDIT=13,
    CONSOLIDATED_FRIENDS_BET=14, TEAMSTER_DEBATE=15, CONSOLIDATE_IQ=16,
    TEAM_AREA=17, DEBATE_TOPIC=18, DEBATE_POSITION=19, RECIPIENT_LABEL=20
};
struct Frame {
    std::uint8_t version{};
    Type type{Type::DATA};
    std::uint16_t flags{};
    std::uint64_t stream_id{};
    std::uint64_t request_id{};
    std::uint64_t sequence{};
    std::vector<std::uint8_t> payload;
};
inline std::uint16_t get16(const std::uint8_t* p){return(std::uint16_t(p[0])<<8)|p[1];}
inline std::uint32_t get32(const std::uint8_t* p){return(std::uint32_t(p[0])<<24)|(std::uint32_t(p[1])<<16)|(std::uint32_t(p[2])<<8)|p[3];}
inline std::uint64_t get64(const std::uint8_t* p){std::uint64_t v=0;for(int i=0;i<8;++i)v=(v<<8)|p[i];return v;}
inline void put16(std::uint8_t*p,std::uint16_t v){p[0]=std::uint8_t(v>>8);p[1]=std::uint8_t(v);}
inline void put32(std::uint8_t*p,std::uint32_t v){p[0]=std::uint8_t(v>>24);p[1]=std::uint8_t(v>>16);p[2]=std::uint8_t(v>>8);p[3]=std::uint8_t(v);}
inline void put64(std::uint8_t*p,std::uint64_t v){for(int i=7;i>=0;--i){p[i]=std::uint8_t(v);v>>=8;}}
inline bool send_all(int fd,const std::uint8_t*data,std::size_t n){while(n){auto r=::send(fd,data,n,0);if(r<=0)return false;data+=r;n-=std::size_t(r);}return true;}
inline bool recv_all(int fd,std::uint8_t*data,std::size_t n){while(n){auto r=::recv(fd,data,n,MSG_WAITALL);if(r<=0)return false;data+=r;n-=std::size_t(r);}return true;}
inline bool valid_type(std::uint8_t type,std::uint8_t max_type){return type>=1&&type<=max_type;}
inline bool valid_payload_length(std::uint32_t length){return length<=kMaxPayload;}
inline bool read(int fd,Frame&f,std::uint8_t max_type){
    std::array<std::uint8_t,kHeaderSize> h{};
    if(!recv_all(fd,h.data(),h.size()))return false;
    if(!valid_type(h[1],max_type)||!valid_payload_length(get32(h.data()+28)))return false;
    f.version=h[0];f.type=static_cast<Type>(h[1]);f.flags=get16(h.data()+2);
    f.stream_id=get64(h.data()+4);f.request_id=get64(h.data()+12);f.sequence=get64(h.data()+20);
    const auto length=get32(h.data()+28);f.payload.assign(length,0);
    return length==0||recv_all(fd,f.payload.data(),length);
}
inline bool write(int fd,const Frame&f){
    if(f.payload.size()>kMaxPayload)return false;
    std::array<std::uint8_t,kHeaderSize>h{};
    h[0]=f.version;h[1]=static_cast<std::uint8_t>(f.type);put16(h.data()+2,f.flags);
    put64(h.data()+4,f.stream_id);put64(h.data()+12,f.request_id);put64(h.data()+20,f.sequence);
    put32(h.data()+28,static_cast<std::uint32_t>(f.payload.size()));
    return send_all(fd,h.data(),h.size())&&(f.payload.empty()||send_all(fd,f.payload.data(),f.payload.size()));
}
inline std::string payload_text(const Frame&f){return std::string(reinterpret_cast<const char*>(f.payload.data()),f.payload.size());}
inline Frame text_response(const Frame&request,std::uint8_t version,Type type,const std::string&body){
    Frame r;r.version=version;r.type=type;r.flags=0;r.stream_id=request.stream_id;r.request_id=request.request_id;r.sequence=request.sequence;
    r.payload.assign(body.begin(),body.end());return r;
}
} // namespace sleela::packet
