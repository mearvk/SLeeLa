#include "sleela_server.hpp"
#include <algorithm>
#include <array>
#include <chrono>
#include <charconv>
#include <limits>
#include <ctime>
#include <fstream>
#include <cerrno>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <thread>
#include <cstring>
#include <openssl/ssl.h>
#include <openssl/x509.h>
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
template <typename T> bool parse_uint(const std::string& text, T& out) {
 if (text.empty()) return false;
 T value{};
 const auto r = std::from_chars(text.data(), text.data() + text.size(), value, 10);
 if (r.ec != std::errc{} || r.ptr != text.data() + text.size()) return false;
 out = value;
 return true;
}
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
 Packet candidate=p;
 std::istringstream in(block);std::string line;std::unordered_set<std::string>seen;
 try{while(std::getline(in,line)){if(!line.empty()&&line.back()=='\r')line.pop_back();if(line.empty())continue;auto eq=line.find('=');if(eq==std::string::npos||eq==0){err="bad_header";return false;}std::string k=line.substr(0,eq),v=line.substr(eq+1);if(k.size()>64||v.size()>4096||!printable(k)||k.find_first_not_of("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_.")!=std::string::npos||!printable(v)){err="header_bounds";return false;}if(!seen.insert(k).second){err="duplicate_header";return false;}
  if(k=="SERVICE-ID")candidate.service_id=v;
  else if(k=="OP-ID")candidate.operation_id=v;
  else if(k=="PROTOCOL-GRADE"){std::uint64_t n=0;if(!parse_uint(v,n)||n>255){err="invalid_protocol_grade";return false;}candidate.grade=static_cast<std::uint8_t>(n);}
  else if(k=="FLAGS"){std::uint64_t n=0;if(!parse_uint(v,n)||n>65535){err="invalid_flags";return false;}candidate.flags=static_cast<std::uint16_t>(n);}
  else if(k=="STREAM-ID"){if(!parse_uint(v,candidate.stream_id)){err="invalid_stream_id";return false;}}
  else if(k=="REQUEST-ID"){if(!parse_uint(v,candidate.request_id)){err="invalid_request_id";return false;}}
  else if(k=="SEQUENCE"){if(!parse_uint(v,candidate.sequence)){err="invalid_sequence";return false;}}
  else candidate.fields.emplace(std::move(k),std::move(v));
 } }catch(...){err="invalid_header";return false;}if(candidate.grade==0||candidate.service_id.empty()||candidate.operation_id.empty()){err="missing_required_header";return false;} p=std::move(candidate); return true;
}
SleelaServer::SleelaServer(ServerConfig c):config_(std::move(c)),logger_(config_),inspector_(config_),packet_logger_("logs"){}
SleelaServer::~SleelaServer(){stop();}
std::string SleelaServer::handler_key(const std::string& service,const std::string& operation){return service+char(31)+operation;}
bool SleelaServer::register_handler(const std::string& service,const std::string& operation,ServiceHandler handler){
 if(service.empty()||operation.empty()||service.size()>256||operation.size()>256||!handler||!printable(service)||!printable(operation))return false;
 std::lock_guard<std::mutex> lock(handlers_mutex_);
 return handlers_.emplace(handler_key(service,operation),std::move(handler)).second;
}
bool SleelaServer::start(){
 std::lock_guard<std::mutex> state_lock(state_mutex_);
 if(running_.load())return true;
 if(config_.port==0 || config_.max_connections==0 || config_.max_connections>65535 ||
    config_.max_packet_bytes<FIXED_HEADER || config_.max_header_bytes>config_.max_packet_bytes-FIXED_HEADER ||
    config_.max_payload_bytes>config_.max_packet_bytes || config_.idle_timeout_seconds==0 ||
    config_.max_packets_per_second==0 || config_.tls_certificate_file.empty()!=config_.tls_private_key_file.empty() || config_.tls_certificate_file.empty()!=config_.tls_client_ca_file.empty()) { logger_.write(Severity::ERROR,"invalid_server_configuration",nullptr,nullptr); return false; }
const bool tls_enabled=!config_.tls_certificate_file.empty();
 if(config_.bind_address!="127.0.0.1" && config_.bind_address.rfind("127.",0)!=0 && !tls_enabled){logger_.write(Severity::ERROR,"remote_bind_requires_tls",nullptr,nullptr);return false;}
 if(tls_enabled && (config_.allowed_peer_identities.empty()||config_.identity_permissions.empty())){logger_.write(Severity::ERROR,"tls_identity_policy_missing",nullptr,nullptr);return false;}
 if(tls_enabled){
  tls_context_=SSL_CTX_new(TLS_server_method());
  if(!tls_context_ || SSL_CTX_set_min_proto_version(tls_context_,TLS1_3_VERSION)!=1 || SSL_CTX_set_max_proto_version(tls_context_,TLS1_3_VERSION)!=1 || SSL_CTX_set1_groups_list(tls_context_,"X25519:P-256:P-384")!=1 ||
     SSL_CTX_use_certificate_chain_file(tls_context_,config_.tls_certificate_file.c_str())!=1 ||
     SSL_CTX_use_PrivateKey_file(tls_context_,config_.tls_private_key_file.c_str(),SSL_FILETYPE_PEM)!=1 ||
     SSL_CTX_check_private_key(tls_context_)!=1 || SSL_CTX_load_verify_locations(tls_context_,config_.tls_client_ca_file.c_str(),nullptr)!=1){
   if(tls_context_){SSL_CTX_free(tls_context_);tls_context_=nullptr;} logger_.write(Severity::ERROR,"tls_configuration_failed",nullptr,nullptr);return false;
  }
  SSL_CTX_set_verify(tls_context_,SSL_VERIFY_PEER|SSL_VERIFY_FAIL_IF_NO_PEER_CERT,nullptr);
  SSL_CTX_set_verify_depth(tls_context_,4);
 }
#ifdef _WIN32
 WSADATA w{};if(WSAStartup(MAKEWORD(2,2),&w)!=0){if(tls_context_){SSL_CTX_free(tls_context_);tls_context_=nullptr;}return false;}winsock_started_=true;
#endif
 listen_fd_=(int)::socket(AF_INET,SOCK_STREAM,0);if(listen_fd_<0){if(tls_context_){SSL_CTX_free(tls_context_);tls_context_=nullptr;}
#ifdef _WIN32
 if(winsock_started_){WSACleanup();winsock_started_=false;}
#endif
 return false;}int one=1;setsockopt(listen_fd_,SOL_SOCKET,SO_REUSEADDR,(char*)&one,sizeof(one));
 sockaddr_in a{};a.sin_family=AF_INET;a.sin_port=htons(config_.port);if(config_.bind_address=="0.0.0.0")a.sin_addr.s_addr=htonl(INADDR_ANY);else if(inet_pton(AF_INET,config_.bind_address.c_str(),&a.sin_addr)!=1){close_socket(listen_fd_);listen_fd_=-1;if(tls_context_){SSL_CTX_free(tls_context_);tls_context_=nullptr;}
#ifdef _WIN32
 if(winsock_started_){WSACleanup();winsock_started_=false;}
#endif
 return false;}
 if(::bind(listen_fd_,(sockaddr*)&a,sizeof(a))!=0){close_socket(listen_fd_);listen_fd_=-1;if(tls_context_){SSL_CTX_free(tls_context_);tls_context_=nullptr;}return false;}if(::listen(listen_fd_,(int)config_.max_connections)!=0){close_socket(listen_fd_);listen_fd_=-1;if(tls_context_){SSL_CTX_free(tls_context_);tls_context_=nullptr;}return false;}running_=true; sleela::LoggerLogic logic; logic.log_sent=true; logic.log_received=true; logic.minimum_severity=sleela::LogSeverity::Info; logic.minimum_admission=sleela::LogAdmission::Accepted; packet_logger_.set_logic(logic); if(!packet_logger_.open()){ logger_.write(Severity::ERROR,"packet_logger_open_failed",nullptr,nullptr); } logger_.write(Severity::NOTICE,"server_started",nullptr,nullptr);return true;
}
void SleelaServer::run(){
 bool expected=false;if(!run_active_.compare_exchange_strong(expected,true))throw std::runtime_error("SLeeLa server run already active");
 if(!running_.load()&&!start()){run_active_.store(false);throw std::runtime_error("SLeeLa server start failed");}
 while(running_.load()){
  sockaddr_in p{};socket_len_t n=sizeof(p);
  int fd=(int)::accept(listen_fd_,(sockaddr*)&p,&n);
  if(fd<0){if(!running_.load())break;continue;}
  const auto previous=active_connections_.fetch_add(1);
  if(previous>=config_.max_connections){
   active_connections_.fetch_sub(1);
   write_status(fd,nullptr,503,"connection_limit");
   close_socket(fd);
   logger_.write(Severity::NOTICE,"connection_limit_reached",nullptr,nullptr);
   continue;
  }
  char h[INET_ADDRSTRLEN]{};inet_ntop(AF_INET,&p.sin_addr,h,sizeof(h));
  {std::lock_guard<std::mutex> state_lock(state_mutex_);if(!running_.load()){close_socket(fd);active_connections_.fetch_sub(1);break;}client_sockets_.insert(fd);}
  std::lock_guard<std::mutex> workers_lock(workers_mutex_);
  // Bound retained thread handles even during long-lived sequential workloads.
  if(workers_.size()>=config_.max_connections*2){
   auto old=std::move(workers_.front());workers_.erase(workers_.begin());
   if(old.joinable())old.join();
  }
  workers_.emplace_back(&SleelaServer::client_loop,this,fd,std::string(h));
 }
 {
  std::lock_guard<std::mutex> workers_lock(workers_mutex_);
  for(auto& worker:workers_)if(worker.joinable())worker.join();
  workers_.clear();
 }
 packet_logger_.close();
 {std::lock_guard<std::mutex> state_lock(state_mutex_);if(listen_fd_>=0){close_socket(listen_fd_);listen_fd_=-1;}if(tls_context_){SSL_CTX_free(tls_context_);tls_context_=nullptr;}
#ifdef _WIN32
 if(winsock_started_){WSACleanup();winsock_started_=false;}
#endif
 } run_active_.store(false);
}
void SleelaServer::stop(){ std::lock_guard<std::mutex>l(state_mutex_);running_.store(false);for(int client:client_sockets_){
#ifdef _WIN32
 shutdown(client,SD_BOTH);
#else
 shutdown(client,SHUT_RDWR);
#endif
 } if(listen_fd_>=0){
#ifdef _WIN32
 shutdown(listen_fd_,SD_BOTH);
#else
 shutdown(listen_fd_,SHUT_RDWR);
#endif
 if(!run_active_.load()){close_socket(listen_fd_);listen_fd_=-1;if(tls_context_){SSL_CTX_free(tls_context_);tls_context_=nullptr;}
#ifdef _WIN32
 if(winsock_started_){WSACleanup();winsock_started_=false;}
#endif
 }}
}
bool SleelaServer::running()const noexcept{return running_.load();}
bool SleelaServer::read_exact(int fd,SSL* ssl,void*dst,std::size_t bytes,std::uint32_t ms){
 auto* p=static_cast<std::uint8_t*>(dst); std::size_t got=0;
 const auto deadline=std::chrono::steady_clock::now()+std::chrono::milliseconds(ms);
 while(got<bytes){
  const auto now=std::chrono::steady_clock::now(); if(now>=deadline)return false;
  const auto remaining=std::chrono::duration_cast<std::chrono::microseconds>(deadline-now);
  const auto us=remaining.count();
  fd_set set;FD_ZERO(&set);FD_SET(fd,&set);
  timeval tv{static_cast<long>(us/1000000),static_cast<long>(us%1000000)};
  if((!ssl || SSL_pending(ssl)==0) && select(fd+1,&set,nullptr,nullptr,&tv)<=0)return false;
 int n=0;
 if(ssl)n=SSL_read(ssl,p+got,static_cast<int>(std::min<std::size_t>(bytes-got,static_cast<std::size_t>(std::numeric_limits<int>::max()))));
 else {
#ifdef _WIN32
  n=recv(fd,(char*)p+got,(int)(bytes-got),0);
#else
  n=(int)recv(fd,p+got,bytes-got,0);
#endif
 }
 if(n<=0)return false;got+=(std::size_t)n;}return true;}
bool SleelaServer::read_packet(int fd,SSL* ssl,Packet&p,std::size_t&wire,std::string&err){std::array<std::uint8_t,FIXED_HEADER>h{};if(!read_exact(fd,ssl,h.data(),h.size(),config_.idle_timeout_seconds*1000U)){err="header_timeout_or_eof";return false;}if(be32(h.data())!=MAGIC||h[4]!=WIRE_VERSION){err="bad_magic_or_wire_version";return false;}std::uint32_t hl=be32(h.data()+8),pl=be32(h.data()+12);std::uint64_t total=FIXED_HEADER+(std::uint64_t)hl+pl;if(be16(h.data()+6)!=0){err="unsupported_flags";return false;}if(hl>config_.max_header_bytes||pl>config_.max_payload_bytes||total>config_.max_packet_bytes){err="frame_exceeds_limits";return false;}
 p.grade=h[5];p.flags=be16(h.data()+6);p.stream_id=be64(h.data()+16);p.request_id=be64(h.data()+24);p.sequence=be64(h.data()+32);std::string hs(hl,'\0');if(hl&&!read_exact(fd,ssl,hs.data(),hl,config_.idle_timeout_seconds*1000U)){err="header_body_truncated";return false;}if(!PacketInspector::parse_headers(hs,p,err))return false;if(p.grade!=h[5]){err="grade_mismatch";return false;}if(p.flags!=be16(h.data()+6)){err="flags_mismatch";return false;}p.payload.resize(pl);if(pl&&!read_exact(fd,ssl,p.payload.data(),pl,config_.idle_timeout_seconds*1000U)){err="payload_truncated";return false;}wire=(std::size_t)total;return true;}
bool SleelaServer::write_status(int fd,SSL* ssl,int status,const char*reason){
 const std::string s="SLPK/1 "+std::to_string(status)+" "+reason+"\n";
 std::size_t sent=0;
 while(sent<s.size()){
  int n=0;
  if(ssl)n=SSL_write(ssl,s.data()+sent,static_cast<int>(s.size()-sent));
  else {
#ifdef _WIN32
   n=send(fd,s.data()+sent,static_cast<int>(s.size()-sent),0);
#else
   n=static_cast<int>(send(fd,s.data()+sent,s.size()-sent,MSG_NOSIGNAL));
#endif
  }
  if(n<0){
#ifndef _WIN32
   if(errno==EINTR)continue;
#endif
   return false;
  }
  if(n==0)return false;
  sent+=static_cast<std::size_t>(n);
 }
 return true;
}
void SleelaServer::client_loop(int fd,std::string peer){
 SSL* ssl=nullptr; std::string identity;
 if(tls_context_){
#ifdef _WIN32
  DWORD handshake_timeout=10000;setsockopt(fd,SOL_SOCKET,SO_RCVTIMEO,reinterpret_cast<const char*>(&handshake_timeout),sizeof(handshake_timeout));
#else
  timeval handshake_timeout{10,0};setsockopt(fd,SOL_SOCKET,SO_RCVTIMEO,&handshake_timeout,sizeof(handshake_timeout));
#endif
  ssl=SSL_new(tls_context_);
  if(!ssl || SSL_set_fd(ssl,fd)!=1 || SSL_accept(ssl)!=1){
   logger_.write(Severity::WARN,"tls_handshake_failed",nullptr,nullptr,peer);
   if(ssl)SSL_free(ssl);close_socket(fd);{std::lock_guard<std::mutex> lock(state_mutex_);client_sockets_.erase(fd);}active_connections_.fetch_sub(1);return;
  }
#ifdef _WIN32
  DWORD no_timeout=0;setsockopt(fd,SOL_SOCKET,SO_RCVTIMEO,reinterpret_cast<const char*>(&no_timeout),sizeof(no_timeout));
#else
  timeval no_timeout{0,0};setsockopt(fd,SOL_SOCKET,SO_RCVTIMEO,&no_timeout,sizeof(no_timeout));
#endif
  X509* cert=SSL_get_peer_certificate(ssl);
  if(cert){X509_NAME* subject=X509_get_subject_name(cert);int index=X509_NAME_get_index_by_NID(subject,NID_commonName,-1);if(index>=0){X509_NAME_ENTRY* entry=X509_NAME_get_entry(subject,index);ASN1_STRING* data=X509_NAME_ENTRY_get_data(entry);unsigned char* utf8=nullptr;int n=ASN1_STRING_to_UTF8(&utf8,data);if(n>0&&static_cast<std::size_t>(n)<=256)identity.assign(reinterpret_cast<char*>(utf8),static_cast<std::size_t>(n));OPENSSL_free(utf8);}X509_free(cert);}
  if(identity.empty()||!config_.allowed_peer_identities.count(identity)){
   logger_.write(Severity::WARN,"tls_identity_not_allowlisted",nullptr,nullptr,peer);
   SSL_shutdown(ssl);SSL_free(ssl);close_socket(fd);{std::lock_guard<std::mutex> lock(state_mutex_);client_sockets_.erase(fd);}active_connections_.fetch_sub(1);return;
  }
 }
 struct ActiveGuard{std::atomic<std::size_t>& count;~ActiveGuard(){count.fetch_sub(1);}} active_guard{active_connections_};std::uint64_t packets=0,failures=0;auto started=std::chrono::steady_clock::now();logger_.write(Severity::DEBUG,"client_connected",nullptr,nullptr,peer+ (identity.empty()?"":(" identity="+identity)));
 while(running_.load()){Packet p;std::size_t wire=0;std::string err;if(!read_packet(fd,ssl,p,wire,err)){++failures;logger_.write(Severity::WARN,"packet_rejected",nullptr,nullptr,peer+":"+err);write_status(fd,ssl,400,err.c_str());break;}++packets; packet_logger_.record_received(peer,"SLeeLa-WIRE/"+std::to_string((int)p.grade),p.stream_id,p.request_id,p.sequence,wire,"raw_socket_to_sleela_server"); auto sec=std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now()-started).count();std::uint64_t rate=sec==0?packets:packets/(std::uint64_t)sec;Decision d=inspector_.inspect(p,wire,rate,failures);if(d.accept&&tls_context_){auto it=config_.identity_permissions.find(identity);const std::string permission=handler_key(p.service_id,p.operation_id);if(it==config_.identity_permissions.end()||!it->second.count(permission))d={false,403,Severity::NOTICE,"identity_not_authorized",.7};}logger_.write(d.severity,d.accept?"packet_accepted":"packet_rejected",&p,&d,peer);if(!d.accept){write_status(fd,ssl,d.status,d.reason.c_str());break;}ServiceHandler handler;{std::lock_guard<std::mutex> lock(handlers_mutex_);auto it=handlers_.find(handler_key(p.service_id,p.operation_id));if(it!=handlers_.end())handler=it->second;} ServiceResponse response; if(!handler){response.status=501;response.reason="service_not_implemented";}else{try{response=handler(p);}catch(...){response.status=500;response.reason="handler_error";}} if(response.status<200||response.status>599||response.reason.empty()||response.reason.find_first_of(" \r\n")!=std::string::npos){response.status=500;response.reason="invalid_handler_response";} if(!write_status(fd,ssl,response.status,response.reason.c_str()))break; packet_logger_.record_sent(peer,"SLeeLa-WIRE/"+std::to_string((int)p.grade),p.stream_id,p.request_id,p.sequence,(std::string("SLPK/1 ")+std::to_string(response.status)+" "+response.reason+"\n").size(),"server_to_os_socket");}
 if(ssl){SSL_shutdown(ssl);SSL_free(ssl);}close_socket(fd);{std::lock_guard<std::mutex> state_lock(state_mutex_);client_sockets_.erase(fd);}logger_.write(Severity::DEBUG,"client_disconnected",nullptr,nullptr,peer);}
void SleelaServer::close_socket(int fd){if(fd<0)return;
#ifdef _WIN32
 closesocket(fd);
#else
 ::close(fd);
#endif
}
}
