#include "http_server.h"
#include <atomic>
#include <cerrno>
#include <chrono>
#include <condition_variable>
#include <cctype>
#include <deque>
#include <filesystem>
#include <fstream>
#include <functional>
#include <iostream>
#include <mutex>
#include <netinet/in.h>
#include <poll.h>
#include <sstream>
#include <string>
#include <string_view>
#include <sys/socket.h>
#include <sys/time.h>
#include <thread>
#include <unordered_map>
#include <vector>
#include <signal.h>
#include <unistd.h>
namespace fs=std::filesystem;
namespace {
constexpr size_t HMAX=64*1024, BMAX=8*1024*1024, QMAX=128, NMAX=100;
constexpr unsigned TMAX=128, TDEFAULT=16; constexpr int TIMEOUT=15000;
std::atomic<bool> stop{false}; void sig(int){stop=true;}
struct Req{std::string method,target,path,version;std::unordered_map<std::string,std::string> h;std::vector<char> body;bool keep=false;};
struct Resp{int code=200;std::string type="text/plain; charset=utf-8";std::vector<char> body;bool keep=false,head=false;};
std::string lo(std::string s){for(char&c:s)c=(char)std::tolower((unsigned char)c);return s;}
const char* reason(int s){switch(s){case 200:return"OK";case 204:return"No Content";case 400:return"Bad Request";case 403:return"Forbidden";case 404:return"Not Found";case 405:return"Method Not Allowed";case 413:return"Content Too Large";case 431:return"Request Header Fields Too Large";case 500:return"Internal Server Error";default:return"Error";}}
bool sendall(int fd,const char*p,size_t n){while(n){ssize_t w=send(fd,p,n,MSG_NOSIGNAL);if(w<0){if(errno==EINTR)continue;return false;}if(!w)return false;p+=w;n-=(size_t)w;}return true;}
bool recvmore(int fd,std::string&b){char x[16384];ssize_t n=recv(fd,x,sizeof x,0);if(n<0){if(errno==EINTR)return true;return false;}if(!n)return false;if(b.size()+(size_t)n>HMAX+BMAX+16384)return false;b.append(x,(size_t)n);return true;}
bool token(std::string_view s){if(s.empty())return false;for(unsigned char c:s)if(c<=32||c>=127||std::string_view("()<>@,;:\\"/[]?={} \t").find((char)c)!=std::string_view::npos)return false;return true;}
bool parse(int fd,std::string&b,Req&r,std::string&e){
 size_t z=b.find("\r\n\r\n");if(z==std::string::npos)return false;std::istringstream in(b.substr(0,z));std::string line,extra;
 if(!std::getline(in,line)){e="bad request";return false;}if(!line.empty()&&line.back()=='\r')line.pop_back();std::istringstream f(line);
 if(!(f>>r.method>>r.target>>r.version)||(f>>extra)){e="bad request line";return false;}
 if(r.method.size()>32||!token(r.method)){e="bad method";return false;}if(r.target.empty()||r.target.size()>16384){e="bad target";return false;}
 if(r.version!="HTTP/1.0"&&r.version!="HTTP/1.1"){e="unsupported HTTP version";return false;}
 size_t hc=0;while(std::getline(in,line)){if(!line.empty()&&line.back()=='\r')line.pop_back();if(line.empty())continue;if(++hc>NMAX){e="too many headers";return false;}
  size_t c=line.find(':');if(c==std::string::npos||c==0){e="bad header";return false;}std::string n=lo(line.substr(0,c)),v=line.substr(c+1);while(!v.empty()&&(v[0]==' '||v[0]=='\t'))v.erase(v.begin());
  if(!token(n)||v.size()>16384){e="bad header";return false;}if(r.h.count(n)&&n=="content-length"){e="duplicate content-length";return false;}r.h[n]=v;}
 if(r.version=="HTTP/1.1"&&!r.h.count("host")){e="Host required";return false;}
 bool cl=false;uint64_t len=0;if(r.h.count("content-length")){cl=true;for(char c:r.h["content-length"]){if(c<'0'||c>'9'){e="bad content-length";return false;}uint64_t n=len*10+(c-'0');if(n<len||n>BMAX){e="body too large";return false;}len=n;}}
 if(r.h.count("transfer-encoding")){if(cl||lo(r.h["transfer-encoding"])!="chunked"){e="invalid transfer-encoding";return false;}}
 size_t used=z+4;
 if(r.h.count("transfer-encoding")){
  for(;;){size_t le=b.find("\r\n",used);while(le==std::string::npos){if(!recvmore(fd,b)){e="incomplete chunk";return false;}le=b.find("\r\n",used);}
   std::string s=b.substr(used,le-used);size_t q=s.find(';');if(q!=std::string::npos)s.resize(q);if(s.empty()||s.size()>16){e="bad chunk size";return false;}uint64_t n=0;
   for(char c:s){int d=(c>='0'&&c<='9')?c-'0':(c>='a'&&c<='f')?c-'a'+10:(c>='A'&&c<='F')?c-'A'+10:-1;if(d<0){e="bad chunk size";return false;}n=n*16+d;if(n>BMAX){e="body too large";return false;}}
   used=le+2;while(b.size()<used+n+2)if(!recvmore(fd,b)){e="incomplete chunk body";return false;}r.body.insert(r.body.end(),b.data()+used,b.data()+used+n);used+=(size_t)n;if(b.compare(used,2,"\r\n")){e="bad chunk terminator";return false;}used+=2;if(!n)break;
  }
 }else if(len){while(b.size()<used+len)if(!recvmore(fd,b)){e="incomplete body";return false;}r.body.assign(b.data()+used,b.data()+used+len);used+=(size_t)len;}
 b.erase(0,used);std::string cv=r.h.count("connection")?lo(r.h["connection"]):"";r.keep=r.version=="HTTP/1.1"?cv.find("close")==std::string::npos:cv.find("keep-alive")!=std::string::npos;
 size_t q=r.target.find('?');r.path=q==std::string::npos?r.target:r.target.substr(0,q);if(r.path.empty())r.path="/";return true;
}
int hx(char c){if(c>='0'&&c<='9')return c-'0';if(c>='a'&&c<='f')return c-'a'+10;if(c>='A'&&c<='F')return c-'A'+10;return -1;}
bool safe(const fs::path&root,const std::string&u,fs::path&o){std::string d;for(size_t i=0;i<u.size();++i){unsigned char c=u[i];if(c=='%'){if(i+2>=u.size())return false;int a=hx(u[i+1]),b=hx(u[i+2]);if(a<0||b<0)return false;c=(a<<4)|b;i+=2;}if(!c||c=='\\'||c=='\r'||c=='\n')return false;d.push_back((char)c);}fs::path p=fs::path(d).lexically_normal();if(p.is_absolute())return false;for(auto&x:p)if(x=="..")return false;std::error_code ec,ec2;auto rr=fs::weakly_canonical(root,ec),cc=fs::weakly_canonical(root/p,ec2);if(ec||ec2)return false;auto a=rr.begin(),b=cc.begin();for(;a!=rr.end()&&b!=cc.end()&&*a==*b;++a,++b){}if(a!=rr.end())return false;o=cc;return true;}
std::string mime(const fs::path&p){auto e=lo(p.extension().string());if(e==".html"||e==".htm")return"text/html; charset=utf-8";if(e==".css")return"text/css; charset=utf-8";if(e==".js")return"text/javascript; charset=utf-8";if(e==".json")return"application/json";if(e==".txt"||e==".md")return"text/plain; charset=utf-8";if(e==".xml")return"application/xml";if(e==".svg")return"image/svg+xml";if(e==".png")return"image/png";if(e==".jpg"||e==".jpeg")return"image/jpeg";return"application/octet-stream";}
class Log{std::mutex m;std::ofstream f;public:explicit Log(const fs::path&p){std::error_code e;fs::create_directories(p.parent_path(),e);f.open(p,std::ios::app);}void put(const std::string&s){std::lock_guard<std::mutex>g(m);std::cerr<<s<<'\n';if(f)f<<s<<'\n';}};
class Pool{std::mutex m;std::condition_variable cv;std::deque<int>q;std::vector<std::thread>w;bool end=false;public:Pool(unsigned n,std::function<void(int)>fn){for(unsigned i=0;i<n;++i)w.emplace_back([this,fn]{for(;;){int fd;{std::unique_lock<std::mutex>g(m);cv.wait(g,[this]{return end||!q.empty();});if(end&&q.empty())return;fd=q.front();q.pop_front();}fn(fd);}});}bool add(int fd){std::lock_guard<std::mutex>g(m);if(end||q.size()>=QMAX)return false;q.push_back(fd);cv.notify_one();return true;}void stop(){{std::lock_guard<std::mutex>g(m);end=true;}cv.notify_all();for(auto&t:w)if(t.joinable())t.join();}};
bool response(int fd,const Req&q,const Resp&r,const std::string&server){std::ostringstream h;h<<q.version<<' '<<r.code<<' '<<reason(r.code)<<"\r\nServer: "<<server<<"\r\nContent-Type: "<<r.type<<"\r\nContent-Length: "<<r.body.size()<<"\r\nConnection: "<<(r.keep?"keep-alive":"close")<<"\r\n\r\n";auto s=h.str();return sendall(fd,s.data(),s.size())&& (r.head||r.body.empty()||sendall(fd,r.body.data(),r.body.size()));}
Resp make(const Req&q,const fs::path&root,const std::string&g){Resp r;r.keep=q.keep;r.head=q.method=="HEAD";if(q.method!="GET"&&q.method!="HEAD"&&q.method!="POST"&&q.method!="OPTIONS"){r.code=405;r.keep=false;std::string b="Method Not Allowed\n";r.body.assign(b.begin(),b.end());return r;}if(q.method=="OPTIONS"){r.code=204;return r;}if(q.path=="/__sleela/health"){std::string b="SLeeLa HTTP server grade "+g+" OK\n";r.body.assign(b.begin(),b.end());return r;}if(q.path=="/__sleela/status"){std::string b="{\"server\":\"SLeeLa\",\"http_grade\":\""+g+"\",\"lower_http_compatibility\":true}\n";r.type="application/json";r.body.assign(b.begin(),b.end());return r;}fs::path p;if(!safe(root,q.path,p)){r.code=403;std::string b="Forbidden\n";r.body.assign(b.begin(),b.end());r.keep=false;return r;}std::error_code e;if(fs::is_directory(p,e))p/="index.html";if(!fs::is_regular_file(p,e)){r.code=404;std::string b="Not Found\n";r.body.assign(b.begin(),b.end());return r;}auto n=fs::file_size(p,e);if(e||n>BMAX){r.code=413;std::string b="Content Too Large\n";r.body.assign(b.begin(),b.end());r.keep=false;return r;}std::ifstream f(p,std::ios::binary);if(!f){r.code=403;std::string b="Forbidden\n";r.body.assign(b.begin(),b.end());r.keep=false;return r;}r.type=mime(p);r.body.resize((size_t)n);if(n)f.read(r.body.data(),(std::streamsize)n);if(!f&&n){r.code=500;r.body.clear();r.keep=false;}return r;}
void serve(int fd,const fs::path&root,const std::string&g,Log&log){timeval tv{15,0};setsockopt(fd,SOL_SOCKET,SO_RCVTIMEO,&tv,sizeof tv);setsockopt(fd,SOL_SOCKET,SO_SNDTIMEO,&tv,sizeof tv);std::string b;b.reserve(16384);for(;;){if(stop)break;if(b.find("\r\n\r\n")==std::string::npos){if(b.size()>HMAX||!recvmore(fd,b))break;}Req q;std::string e;if(!parse(fd,b,q,e)){log.put("request status=400 detail="+e);break;}auto st=std::chrono::steady_clock::now();Resp r=make(q,root,g);if(!response(fd,q,r,"SLeeLa-HTTP/"+g))break;auto us=std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now()-st).count();log.put("request method="+q.method+" path="+q.path+" version="+q.version+" status="+std::to_string(r.code)+" bytes="+std::to_string(r.body.size())+" duration_us="+std::to_string(us));if(!q.keep||!r.keep)break;}close(fd);}
int listenfd(int port){int f=socket(AF_INET,SOCK_STREAM,0);if(f<0)return-1;int one=1;setsockopt(f,SOL_SOCKET,SO_REUSEADDR,&one,sizeof one);sockaddr_in a{};a.sin_family=AF_INET;a.sin_addr.s_addr=htonl(INADDR_ANY);a.sin_port=htons((uint16_t)port);if(bind(f,(sockaddr*)&a,sizeof a)<0||listen(f,256)<0){close(f);return-1;}return f;}
int run(const std::string&g,int ac,char**av){int port=g=="1"?8080:g=="2"?8081:8082;unsigned threads=TDEFAULT;fs::path root=".",logp;bool once=false;for(int i=1;i<ac;++i){std::string a=av[i];if(a=="--port"&&i+1<ac)port=std::atoi(av[++i]);else if(a=="--threads"&&i+1<ac)threads=(unsigned)std::strtoul(av[++i],nullptr,10);else if(a=="--root"&&i+1<ac)root=av[++i];else if(a=="--log"&&i+1<ac)logp=av[++i];else if(a=="--once")once=true;else if(a=="--help"){std::cout<<"--port N --threads N --root DIR --log FILE [--once]\n";return 0;}else{std::cerr<<"unknown option: "<<a<<"\n";return 2;}}if(port<1||port>65535||threads<1||threads>TMAX)return 2;std::error_code e;root=fs::weakly_canonical(root,e);if(e||!fs::is_directory(root)){std::cerr<<"invalid document root\n";return 2;}if(logp.empty())logp=root/(std::string(".sleela-http-")+g+".log");signal(SIGINT,sig);signal(SIGTERM,sig);int l=listenfd(port);if(l<0){perror("listen");return 1;}Log log(logp);log.put("server=start grade="+g+" port="+std::to_string(port));Pool pool(threads,[&](int fd){serve(fd,root,g,log);});while(!stop){pollfd p{l,POLLIN,0};int rc=poll(&p,1,500);if(rc<=0)continue;int fd=accept(l,nullptr,nullptr);if(fd<0)continue;if(!pool.add(fd)){close(fd);log.put("connection rejected reason=worker-queue-full");}if(once)stop=true;}shutdown(l,SHUT_RDWR);close(l);pool.stop();log.put("server=stop grade="+g);return 0;}
}
extern "C" int sleela_http_server_run(const char*v,int argc,char**argv){return run(v?v:"",argc,argv);}
