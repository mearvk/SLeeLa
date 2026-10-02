#include "sleela_server.hpp"
#include <algorithm>
#include <array>
#include <chrono>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <thread>
#ifdef _WIN32
#define NOMINMAX
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib,"Ws2_32.lib")
using socket_len_t=int;
#else
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
using socket_len_t=socklen_t;
#endif
namespace sleela::server {
namespace {
constexpr std::uint32_t MAGIC=0x534C504B;
constexpr std::uint8_t WIRE_VERSION=1;
constexpr std::size_t FIXED_HEADER=40;
std::uint16_t be16(const std::uint8_t*p){return (std::uint16_t)((p[0]<<8)|p[1]);}
std::uint32_t be32(const std::uint8_t*p){return ((std::uint32_t)p[0]<<24)|((std::uint32_t)p[1]<<16)|((std::uint32_t)p[2]<<8)|p[3];}
std::uint64_t be64(const std::uint8_t*p){std::uint64_t v=0;for(int i=0;i<8;++i)v=(v<<8)|p[i];return v;}
bool known_grade(std::uint8_t g){return g>=1&&g<=9;}
bool printable(const std::string&s){for(unsigned char c:s)if(c<0x20&&c!='\t')return false;return true;}
std::string esc(const std::string&s){std::string o;for(unsigned char c:s){if(c=='\\'||c=='"'){o+='\\';o+=(char)c;}else if(c=='\n')o+="\\n";else if(c=='\r')o+="\\r";else if(c<0x20){std::ostringstream x;x<<"\\u"<<std::hex<<std::setw(4)<<std::setfill('0')<<(int)c;o+=x.str();}else o+=(char)c;}return o;}
}
StructuredLogger::StructuredLogger(const ServerConfig&c):cfg_(c){}
std::string StructuredLogger::level_name(Severity s)const{switch(s){case Severity::DEBUG:return"DEBUG";case Severity::INFO:return"INFO";case Severity::NOTICE:return"NOTICE";case Severity::WARN:return"WARN";case Severity::ERROR:return"ERROR";default:return"ALERT";}}
void StructuredLogger::write(Severity l,const std::string&e,const Packet*p,const Decision*d,const std::string&detail){
 if((int)l<(int)cfg_.minimum_log_level)return;std::lock_guard<std::mutex>lock(mutex_);std::ofstream f(cfg_.log_file,std::ios::app);std::ostream&os=f?f:std::cerr;
 auto tt=std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());char ts[32];std::strftime(ts,sizeof(ts),"%Y-%m-%dT%H:%M:%SZ",std::gmtime(&tt));
 os<<"{\"time\":\"" << ts << "\",\"level\":\"" << level_name(l) << "\",\"event\":\"" << esc(e) << "\"";
 if(p)os<<",\"grade\":"<<(int)p->grade<<",\"stream\":"<<p->stream_id<<",\"request\":"<<p->request_id<<",\"sequence\":"<<p->sequence<<",\"service\":\"" << esc(p->service_id) << "\",\"operation\":\"" << esc(p->operation_id) << "\""<<(cfg_.log_payload?",\"payload_bytes\":"+std::to_string(p->payload.size()):"");
 if(d)os<<",\"status\":"<<d->status<<",\"heuristic_score\":"<<std::fixed<<std::setprecision(2)<<d->heuristic_score<<",\"reason\":\"" << esc(d->reason) << "\"";
 if(!detail.empty())os<<",\"detail\":\"" << esc(detail) << "\"";os<<"}\n";
}
PacketInspector::PacketInspector(const ServerConfig&c):cfg_(c){}
Decision PacketInspector::inspect(const Packet&p,std::size_t wire,std::uint64_t rate,std::uint64_t failures)const{
 if(cfg_.require_known_grade&&!known_grade(p.grade))return{false,400,Severity::WARN,"unknown_protocol_grade",.9};
 if(wire>cfg_.max_packet_bytes)return{false,413,Severity::WARN,"packet_too_large",1.0};
 if(p.service_id.empty()||p.operation_id.empty())return{false,422,Severity::WARN,"missing_routing_fields",.8};
 if(p.service_id.size()>256||p.operation_id.size()>256)return{false,422,Severity::WARN,"routing_field_too_long",.9};
 if(!printable(p.service_id)||!printable(p.operation_id))return{false,422,Severity::WARN,"non_printable_routing_field",.95};
 if(!cfg_.allowed_services.empty()&&!cfg_.allowed_services.count(p.service_id))return{false,403,Severity::NOTICE,"service_not_allowlisted",.35};
 if(cfg_.denied_services.count(p.service_id))return{false,403,Severity::NOTICE,"service_denied",.7};
 if(cfg_.denied_operations.count(p.operation_id))return{false,403,Severity::NOTICE,"operation_denied",.7};
 if(p.payload.size()>cfg_.max_payload_bytes)return{false,413,Severity::WARN,"payload_too_large",1.0};
 if(rate>cfg_.max_packets_per_second)return{false,429,Severity::ALERT,"packet_rate_exceeded",.95};
 double score=0;if(failures>cfg_.max_failures_per_minute)score+=.15;
 if(p.grade>=8&&p.fields.find("SOURCE")==p.fields.end())score+=.20;
 if(p.grade>=9&&p.fields.find("DARK-BAND-NAME")==p.fields.end())score+=.10;
 if(p.sequence==0&&p.stream_id!=0)score+=.10;
 Severity sev=Severity::INFO;std::string reason="accepted";
 if(score>=.75){sev=Severity::ALERT;reason="heuristic_threshold";}else if(score>=.40){sev=Severity::WARN;reason="heuristic_anomaly";}
 return{true,200,sev,reason,score};
}
bool PacketInspector::parse_headers(const std::string&block,Packet&p,std::string&err){
 std::istringstream in(block);std::string line;std::unordered_set<std::string>seen;
 try{while(std::getline(in,line)){if(!line.empty()&&line.back()=='\r')line.pop_back();if(line.empty())continue;auto eq=line.find('=');if(eq==std::string::npos||eq==0){err="bad_header";return false;}std::string k=line.substr(0,eq),v=line.substr(eq+1);if(k.size()>64||v.size()>4096||!printable(k)){err="header_bounds";return false;}if(!seen.insert(k).second){err="duplicate_header";return false;}
  if(k=="SERVICE-ID")p.service_id=v;else if(k=="OP-ID")p.operation_id=v;else if(k=="PROTOCOL-GRADE")p.grade=(std::uint8_t)std::stoul(v);else if(k=="FLAGS")p.flags=(std::uint16_t)std::stoul(v);else if(k=="STREAM-ID")p.stream_id=std::stoull(v);else if(k=="REQUEST-ID")p.request_id=std::stoull(v);else if(k=="SEQUENCE")p.sequence=std::stoull(v);else p.fields.emplace(std::move(k),std::move(v));
 } }catch(...){err="invalid_numeric_header";return false;}if(p.grade==0||p.service_id.empty()||p.operation_id.empty()){err="missing_required_header";return false;}return true;
}
SleelaServer::SleelaServer(ServerConfig c):config_(std::move(c)),logger_(config_),inspector_(config_),packet_logger_("logs"){}
SleelaServer::~SleelaServer(){stop();}
bool SleelaServer::start(){
#ifdef _WIN32
 WSADATA w{};if(WSAStartup(MAKEWORD(2,2),&w)!=0)return false;
#endif
 listen_fd_=(int)::socket(AF_INET,SOCK_STREAM,0);if(listen_fd_<0)return false;int one=1;setsockopt(listen_fd_,SOL_SOCKET,SO_REUSEADDR,(char*)&one,sizeof(one));
 sockaddr_in a{};a.sin_family=AF_INET;a.sin_port=htons(config_.port);if(config_.bind_address=="0.0.0.0")a.sin_addr.s_addr=htonl(INADDR_ANY);else if(inet_pton(AF_INET,config_.bind_address.c_str(),&a.sin_addr)!=1){close_socket(listen_fd_);listen_fd_=-1;return false;}
 if(::bind(listen_fd_,(sockaddr*)&a,sizeof(a))!=0){close_socket(listen_fd_);listen_fd_=-1;return false;}if(::listen(listen_fd_,(int)config_.max_connections)!=0){close_socket(listen_fd_);listen_fd_=-1;return false;}running_=true; sleela::LoggerLogic logic; logic.log_sent=true; logic.log_received=true; logic.minimum_severity=sleela::LogSeverity::Info; logic.minimum_admission=sleela::LogAdmission::Accepted; packet_logger_.set_logic(logic); if(!packet_logger_.open()){ logger_.write(Severity::ERROR,"packet_logger_open_failed",nullptr,nullptr); } logger_.write(Severity::NOTICE,"server_started",nullptr,nullptr);return true;
}
void SleelaServer::run(){if(!running_&&!start())throw std::runtime_error("SLeeLa server start failed");while(running_){sockaddr_in p{};socket_len_t n=sizeof(p);int fd=(int)::accept(listen_fd_,(sockaddr*)&p,&n);if(fd<0){if(!running_)break;continue;}char h[INET_ADDRSTRLEN]{};inet_ntop(AF_INET,&p.sin_addr,h,sizeof(h));std::thread(&SleelaServer::client_loop,this,fd,std::string(h)).detach();}}
void SleelaServer::stop(){ packet_logger_.close(); std::lock_guard<std::mutex>l(state_mutex_);if(!running_&&listen_fd_<0)return;running_=false;if(listen_fd_>=0){close_socket(listen_fd_);listen_fd_=-1;}
#ifdef _WIN32
 WSACleanup();
#endif
}
bool SleelaServer::running()const noexcept{return running_;}
bool SleelaServer::read_exact(int fd,void*dst,std::size_t bytes,std::uint32_t ms){auto*p=(std::uint8_t*)dst;std::size_t got=0;while(got<bytes){fd_set set;FD_ZERO(&set);FD_SET(fd,&set);timeval tv{(long)(ms/1000),(long)((ms%1000)*1000)};if(select(fd+1,&set,nullptr,nullptr,&tv)<=0)return false;
#ifdef _WIN32
 int n=recv(fd,(char*)p+got,(int)(bytes-got),0);
#else
 int n=(int)recv(fd,p+got,bytes-got,0);
#endif
 if(n<=0)return false;got+=(std::size_t)n;}return true;}
bool SleelaServer::read_packet(int fd,Packet&p,std::size_t&wire,std::string&err){std::array<std::uint8_t,FIXED_HEADER>h{};if(!read_exact(fd,h.data(),h.size(),config_.idle_timeout_seconds*1000U)){err="header_timeout_or_eof";return false;}if(be32(h.data())!=MAGIC||h[4]!=WIRE_VERSION){err="bad_magic_or_wire_version";return false;}std::uint32_t hl=be32(h.data()+8),pl=be32(h.data()+12);std::uint64_t total=FIXED_HEADER+(std::uint64_t)hl+pl;if(hl>config_.max_header_bytes||pl>config_.max_payload_bytes||total>config_.max_packet_bytes){err="frame_exceeds_limits";return false;}
 p.grade=h[5];p.flags=be16(h.data()+6);p.stream_id=be64(h.data()+16);p.request_id=be64(h.data()+24);p.sequence=be64(h.data()+32);std::string hs(hl,'\0');if(hl&&!read_exact(fd,hs.data(),hl,config_.idle_timeout_seconds*1000U)){err="header_body_truncated";return false;}if(!PacketInspector::parse_headers(hs,p,err))return false;if(p.grade!=h[5]){err="grade_mismatch";return false;}p.payload.resize(pl);if(pl&&!read_exact(fd,p.payload.data(),pl,config_.idle_timeout_seconds*1000U)){err="payload_truncated";return false;}wire=(std::size_t)total;return true;}
bool SleelaServer::write_status(int fd,int status,const char*reason){std::string s="SLPK/1 "+std::to_string(status)+" "+reason+"\n";
#ifdef _WIN32
 return send(fd,s.data(),(int)s.size(),0)==(int)s.size();
#else
 return send(fd,s.data(),s.size(),MSG_NOSIGNAL)==(ssize_t)s.size();
#endif
}
void SleelaServer::client_loop(int fd,std::string peer){std::uint64_t packets=0,failures=0;auto started=std::chrono::steady_clock::now();logger_.write(Severity::DEBUG,"client_connected",nullptr,nullptr,peer);
 while(running_){Packet p;std::size_t wire=0;std::string err;if(!read_packet(fd,p,wire,err)){++failures;logger_.write(Severity::WARN,"packet_rejected",nullptr,nullptr,peer+":"+err);write_status(fd,400,err.c_str());break;}++packets; packet_logger_.record_received(peer,"SLeeLa-WIRE/"+std::to_string((int)p.grade),p.stream_id,p.request_id,p.sequence,wire,"raw_socket_to_sleela_server"); auto sec=std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now()-started).count();std::uint64_t rate=sec==0?packets:packets/(std::uint64_t)sec;Decision d=inspector_.inspect(p,wire,rate,failures);logger_.write(d.severity,d.accept?"packet_accepted":"packet_rejected",&p,&d,peer);if(!d.accept){write_status(fd,d.status,d.reason.c_str());break;}packet_logger_.record_sent(peer,"SLeeLa-WIRE/"+std::to_string((int)p.grade),p.stream_id,p.request_id,p.sequence,0,"server_to_os_socket"); if(!write_status(fd,200,"accepted"))break;}
 close_socket(fd);logger_.write(Severity::DEBUG,"client_disconnected",nullptr,nullptr,peer);}
void SleelaServer::close_socket(int fd){if(fd<0)return;
#ifdef _WIN32
 closesocket(fd);
#else
 ::close(fd);
#endif
}
}
