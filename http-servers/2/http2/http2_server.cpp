#include "http2_server.h"
#include <nghttp2/nghttp2.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <signal.h>
#include <cerrno>
#include <cstring>
#include <atomic>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <string>
#include <unordered_map>
#include <vector>
#include <algorithm>
namespace fs=std::filesystem; namespace {
constexpr size_t MAX_HEADERS=65536,MAX_BODY=8*1024*1024,MAX_STREAMS=256;
std::atomic<bool> stop{false}; void sig(int){stop=true;}
struct Stream{std::string method,path;std::vector<char> body;size_t headers=0;};
struct Ctx{int fd;fs::path root;std::ofstream log;std::mutex m;std::unordered_map<int32_t,Stream> streams;};
void log(Ctx*c,const std::string&s){std::lock_guard<std::mutex>g(c->m);std::cerr<<s<<"\n";if(c->log)c->log<<s<<"\n";}
ssize_t send_cb(nghttp2_session*,const uint8_t*d,size_t n,int,void*p){Ctx*c=(Ctx*)p;size_t o=0;while(o<n){ssize_t w=send(c->fd,d+o,n-o,MSG_NOSIGNAL);if(w<0){if(errno==EINTR)continue;return NGHTTP2_ERR_WOULDBLOCK;}if(!w)return NGHTTP2_ERR_WOULDBLOCK;o+=(size_t)w;}return n;}
int begin_cb(nghttp2_session*,const nghttp2_frame*f,void*p){Ctx*c=(Ctx*)p;if(f->hd.stream_id>0&&c->streams.size()<MAX_STREAMS)c->streams.emplace(f->hd.stream_id,Stream{});return 0;}
int header_cb(nghttp2_session*,const nghttp2_frame*f,const uint8_t*n,size_t nl,const uint8_t*v,size_t vl,uint8_t,void*p){Ctx*c=(Ctx*)p;auto i=c->streams.find(f->hd.stream_id);if(i==c->streams.end())return 0;if(i->second.headers+nl+vl>MAX_HEADERS)return NGHTTP2_ERR_TEMPORAL_CALLBACK_FAILURE;i->second.headers+=nl+vl;std::string k((char*)n,nl),x((char*)v,vl);if(k==":method")i->second.method=x;else if(k==":path")i->second.path=x;return 0;}
int data_cb(nghttp2_session*,uint8_t,int32_t id,const uint8_t*d,size_t n,void*p){Ctx*c=(Ctx*)p;auto i=c->streams.find(id);if(i==c->streams.end())return 0;if(i->second.body.size()+n>MAX_BODY)return NGHTTP2_ERR_TEMPORAL_CALLBACK_FAILURE;i->second.body.insert(i->second.body.end(),d,d+n);return 0;}
bool safe(const fs::path&r,const std::string&u,fs::path&o){if(u.empty()||u[0]!='/'||u.size()>16384)return false;auto q=u.find('?');auto s=u.substr(0,q);for(char c:s)if(c=='%'||c=='\\'||c=='\r'||c=='\n'||c=='\0')return false;fs::path p=fs::path(s).lexically_normal();for(auto&x:p)if(x=="..")return false;std::error_code a,b;auto rr=fs::weakly_canonical(r,a),cc=fs::weakly_canonical(r/p.relative_path(),b);if(a||b)return false;auto i=rr.begin(),j=cc.begin();for(;i!=rr.end()&&j!=cc.end()&&*i==*j;++i,++j){}if(i!=rr.end())return false;o=cc;return true;}
std::string mime(const fs::path&p){auto e=p.extension().string();if(e==".html"||e==".htm")return"text/html; charset=utf-8";if(e==".css")return"text/css";if(e==".js")return"text/javascript";if(e==".json")return"application/json";if(e==".txt"||e==".md")return"text/plain; charset=utf-8";return"application/octet-stream";}
struct Data{std::string b;size_t o=0;};
ssize_t read_cb(nghttp2_session*,int32_t,uint8_t*out,size_t n,uint32_t*flags,nghttp2_data_source*s,void*){auto*d=(Data*)s->ptr;size_t k=std::min(n,d->b.size()-d->o);if(k)memcpy(out,d->b.data()+d->o,k);d->o+=k;if(d->o==d->b.size()){*flags|=NGHTTP2_DATA_FLAG_EOF;delete d;}return k;}
void respond(nghttp2_session*s,Ctx*c,int32_t id){auto i=c->streams.find(id);if(i==c->streams.end())return;auto&q=i->second;int code=200;std::string type="text/plain; charset=utf-8",body;if(q.method!="GET"&&q.method!="HEAD"&&q.method!="POST"){code=405;body="Method Not Allowed\n";}else if(q.path=="/__sleela/health")body="SLeeLa HTTP/2 server OK\n";else if(q.path=="/__sleela/status"){type="application/json";body="{\"server\":\"SLeeLa\",\"http\":\"2.0/2.1\",\"multiplexed\":true}\n";}else{fs::path p;if(!safe(c->root,q.path,p)){code=403;body="Forbidden\n";}else{std::error_code e;if(fs::is_directory(p,e))p/="index.html";if(!fs::is_regular_file(p,e)){code=404;body="Not Found\n";}else{auto n=fs::file_size(p,e);if(e||n>MAX_BODY){code=413;body="Content Too Large\n";}else{std::ifstream f(p,std::ios::binary);if(!f){code=403;body="Forbidden\n";}else{type=mime(p);body.resize((size_t)n);f.read(body.data(),(std::streamsize)n);}}}}}}std::string sc=std::to_string(code),len=std::to_string(body.size());std::vector<nghttp2_nv> h={{(uint8_t*)":status",(uint8_t*)sc.data(),7,sc.size(),NGHTTP2_NV_FLAG_NONE},{(uint8_t*)"content-type",(uint8_t*)type.data(),12,type.size(),NGHTTP2_NV_FLAG_NONE},{(uint8_t*)"content-length",(uint8_t*)len.data(),14,len.size(),NGHTTP2_NV_FLAG_NONE},{(uint8_t*)"server",(uint8_t*)"SLeeLa-HTTP/2",6,13,NGHTTP2_NV_FLAG_NONE}};Data*d=new Data{std::move(body)};nghttp2_data_provider dp{d,read_cb};int rc=nghttp2_submit_response(s,id,h.data(),h.size(),q.method=="HEAD"?nullptr:&dp);if(rc<0)delete d;log(c,"stream="+std::to_string(id)+" method="+q.method+" path="+q.path+" status="+sc);c->streams.erase(i);}
int frame_cb(nghttp2_session*s,const nghttp2_frame*f,void*p){Ctx*c=(Ctx*)p;if(f->hd.type==NGHTTP2_HEADERS&&f->hd.flags&NGHTTP2_FLAG_END_STREAM)respond(s,c,f->hd.stream_id);if(f->hd.type==NGHTTP2_DATA&&f->hd.flags&NGHTTP2_FLAG_END_STREAM)respond(s,c,f->hd.stream_id);if(f->hd.type==NGHTTP2_GOAWAY)stop=true;return 0;}
int conn(int fd,fs::path root,fs::path logp){Ctx c{fd,std::move(root)};c.log.open(logp,std::ios::app);nghttp2_session_callbacks*cb=nullptr;nghttp2_session_callbacks_new(&cb);nghttp2_session_callbacks_set_send_callback(cb,send_cb);nghttp2_session_callbacks_set_on_begin_headers_callback(cb,begin_cb);nghttp2_session_callbacks_set_on_header_callback(cb,header_cb);nghttp2_session_callbacks_set_on_data_chunk_recv_callback(cb,data_cb);nghttp2_session_callbacks_set_on_frame_recv_callback(cb,frame_cb);nghttp2_session*s=nullptr;if(nghttp2_session_server_new(&s,cb,&c)){nghttp2_session_callbacks_del(cb);close(fd);return 1;}nghttp2_session_callbacks_del(cb);nghttp2_settings_entry st[]={{NGHTTP2_SETTINGS_MAX_CONCURRENT_STREAMS,MAX_STREAMS},{NGHTTP2_SETTINGS_MAX_FRAME_SIZE,16384},{NGHTTP2_SETTINGS_INITIAL_WINDOW_SIZE,1024*1024}};nghttp2_submit_settings(s,0,st,3);nghttp2_session_send(s);uint8_t b[16384];while(!stop){ssize_t n=recv(fd,b,sizeof b,0);if(n<=0)break;if(nghttp2_session_mem_recv(s,b,(size_t)n)<0)break;if(nghttp2_session_send(s)<0)break;}nghttp2_session_del(s);close(fd);return 0;}
int listenfd(int p){int f=socket(AF_INET,SOCK_STREAM,0),one=1;if(f<0)return-1;setsockopt(f,SOL_SOCKET,SO_REUSEADDR,&one,sizeof one);sockaddr_in a{};a.sin_family=AF_INET;a.sin_addr.s_addr=htonl(INADDR_ANY);a.sin_port=htons((uint16_t)p);if(bind(f,(sockaddr*)&a,sizeof a)||listen(f,128)){close(f);return-1;}return f;}
}
extern "C" int sleela_http2_server_run(int argc,char**argv){int port=8081,threads=16;fs::path root=".",logp;for(int i=1;i<argc;i++){std::string a=argv[i];if(a=="--port"&&i+1<argc)port=atoi(argv[++i]);else if(a=="--threads"&&i+1<argc)threads=atoi(argv[++i]);else if(a=="--root"&&i+1<argc)root=argv[++i];else if(a=="--log"&&i+1<argc)logp=argv[++i];else if(a=="--help"){std::cout<<"--port N --threads N --root DIR --log FILE\n";return 0;}else return 2;}std::error_code e;root=fs::weakly_canonical(root,e);if(e||!fs::is_directory(root)||port<1||port>65535||threads<1||threads>128)return 2;if(logp.empty())logp=root/".sleela-http-2.log";signal(SIGINT,sig);signal(SIGTERM,sig);int l=listenfd(port);if(l<0)return 1;std::vector<std::thread>w;std::mutex m;std::condition_variable cv;std::vector<int>q;bool done=false;auto work=[&]{for(;;){int fd;{std::unique_lock<std::mutex>g(m);cv.wait(g,[&]{return done||!q.empty();});if(done&&q.empty())return;fd=q.back();q.pop_back();}conn(fd,root,logp);}};for(int i=0;i<threads;i++)w.emplace_back(work);while(!stop){int fd=accept(l,nullptr,nullptr);if(fd<0){if(errno==EINTR)continue;break;}std::lock_guard<std::mutex>g(m);if(q.size()>=256)close(fd);else{q.push_back(fd);cv.notify_one();}}{std::lock_guard<std::mutex>g(m);done=true;}cv.notify_all();close(l);for(auto&t:w)t.join();return 0;}
