#include <algorithm>
#include <cerrno>
#include <cmath>
#include <cctype>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>
#include "../email/sleela_email.h"
#include "../posting/sleela_post.h"
#include "../listener/sleela_listener.h"
#include "../router/sleela_router.h"
#include "../data-analytics/sleela_data_analytics.h"

namespace {
struct Node { std::string name; std::map<std::string,std::string> attr; std::vector<Node> child; std::string text; };

static std::string unescape(std::string s) {
    const std::pair<const char*,const char*> r[]={{"&lt;","<"},{"&gt;",">"},{"&quot;","\""},{"&apos;","'"},{"&amp;","&"}};
    for(const auto& p:r){std::size_t pos=0;while((pos=s.find(p.first,pos))!=std::string::npos){s.replace(pos,std::strlen(p.first),p.second);pos+=std::strlen(p.second);}}
    return s;
}
static void ws(const std::string& s,std::size_t& p){while(p<s.size()&&std::isspace(static_cast<unsigned char>(s[p])))++p;}
static std::string token(const std::string& s,std::size_t& p){
    ws(s,p);std::size_t b=p;
    while(p<s.size()&&(std::isalnum(static_cast<unsigned char>(s[p]))||s[p]=='_'||s[p]=='-'||s[p]=='.'||s[p]==':'))++p;
    if(b==p)throw std::runtime_error("expected XML name");return s.substr(b,p-b);
}
static std::string quoted(const std::string& s,std::size_t& p){
    ws(s,p);if(p>=s.size()||s[p]!='\"')throw std::runtime_error("expected quoted XML attribute");++p;
    std::string o;while(p<s.size()&&s[p]!='\"')o+=s[p++];if(p>=s.size())throw std::runtime_error("unterminated attribute");++p;return unescape(o);
}
static Node element(const std::string& s,std::size_t& p){
    ws(s,p);if(p>=s.size()||s[p]!='<')throw std::runtime_error("expected '<'");++p;
    if(p<s.size()&&s[p]=='/')throw std::runtime_error("unexpected closing tag");
    Node n;n.name=token(s,p);ws(s,p);
    while(p<s.size()&&s[p]!='>'&&!(s[p]=='/'&&p+1<s.size()&&s[p+1]=='>')){
        std::string k=token(s,p);ws(s,p);if(p>=s.size()||s[p++]!='=')throw std::runtime_error("expected '='");n.attr[k]=quoted(s,p);ws(s,p);
    }
    if(p>=s.size())throw std::runtime_error("unterminated element");
    if(s[p]=='/'){p+=2;return n;}++p;
    while(true){
        if(p>=s.size())throw std::runtime_error("missing closing tag for "+n.name);
        if(s[p]=='<'){
            if(p+1<s.size()&&s[p+1]=='/'){p+=2;std::string c=token(s,p);ws(s,p);if(p>=s.size()||s[p++]!='>')throw std::runtime_error("bad closing tag");if(c!=n.name)throw std::runtime_error("mismatched closing tag");break;}
            n.child.push_back(element(s,p));
        }else{std::size_t b=p;while(p<s.size()&&s[p]!='<')++p;n.text+=unescape(s.substr(b,p-b));}
    }
    return n;
}
static Node parse(const std::string& s){std::size_t p=0;ws(s,p);if(s.rfind("<?xml",0)==0){auto q=s.find("?>",p);if(q==std::string::npos)throw std::runtime_error("bad XML declaration");p=q+2;}Node n=element(s,p);ws(s,p);if(p!=s.size())throw std::runtime_error("trailing XML");return n;}
static const Node* child(const Node& n,const std::string& name){for(const auto& x:n.child)if(x.name==name)return &x;return nullptr;}
static std::vector<const Node*> children(const Node& n,const std::string& name){std::vector<const Node*>v;for(const auto&x:n.child)if(x.name==name)v.push_back(&x);return v;}
static std::string attr(const Node& n,const std::string& k,const std::string& d=""){auto i=n.attr.find(k);return i==n.attr.end()?d:i->second;}
static double number(const std::string& s){char*e=nullptr;errno=0;double v=std::strtod(s.c_str(),&e);if(e==s.c_str()||*e||errno==ERANGE||!std::isfinite(v))throw std::runtime_error("invalid numeric argument");return v;}
static std::vector<double> args(const Node& n){std::vector<double>v;for(const Node*x:children(n,"arg")){if(v.size()>=16)throw std::runtime_error("too many arguments");v.push_back(number(attr(*x,"value")));}return v;}
static void witness(const std::string&p,const std::string&s,const std::string&d,const std::string&o,const std::string&r,const std::string&st){std::cout<<"BODI witness project="<<p<<" sequence="<<s<<" domain="<<d<<" operation="<<o<<" result="<<r<<" status="<<st<<"\\n";}

static double mathop(const std::string&o,const std::vector<double>&x){
    if(o=="abs"&&x.size()==1)return std::fabs(x[0]);if(o=="sqrt"&&x.size()==1)return std::sqrt(std::max(0.0,x[0]));
    if(o=="exp"&&x.size()==1)return std::exp(x[0]);if(o=="log"&&x.size()==1)return x[0]>0?std::log(x[0]):0;
    if(o=="log10"&&x.size()==1)return x[0]>0?std::log10(x[0]):0;if(o=="pow"&&x.size()==2)return std::pow(x[0],x[1]);
    if(o=="sin"&&x.size()==1)return std::sin(x[0]);if(o=="cos"&&x.size()==1)return std::cos(x[0]);if(o=="tan"&&x.size()==1)return std::tan(x[0]);
    if(o=="hypot"&&x.size()==2)return std::hypot(x[0],x[1]);if(o=="min"&&x.size()==2)return std::min(x[0],x[1]);
    if(o=="max"&&x.size()==2)return std::max(x[0],x[1]);if(o=="clamp"&&x.size()==3)return std::min(std::max(x[0],x[1]),x[2]);
    throw std::runtime_error("unsupported math operation or arity");
}
static double chemistryop(const std::string&o,const std::vector<double>&x){
    if(o=="ratio"&&x.size()==2)return x[1]==0?0:x[0]/x[1];
    if(o=="similarity"&&x.size()==2){double d=std::fabs(x[0]-x[1]),m=std::max(std::fabs(x[0]),std::fabs(x[1]));return m==0?1.0:std::max(0.0,1.0-d/m);}
    throw std::runtime_error("unsupported chemistry operation or arity");
}
static double physicsop(const std::string&o,const std::vector<double>&x){
    constexpr double C=299792458.0,G=6.67430e-11;
    if(o=="velocity"&&x.size()==2)return x[0]/x[1];if(o=="acceleration"&&x.size()==2)return x[0]/x[1];
    if(o=="kinematic_position"&&x.size()==4)return x[0]+x[1]*x[3]+0.5*x[2]*x[3]*x[3];
    if(o=="force"&&x.size()==2)return x[0]*x[1];if(o=="kinetic_energy"&&x.size()==2)return 0.5*x[0]*x[1]*x[1];
    if(o=="gravitational_force"&&x.size()==3)return G*x[0]*x[1]/(x[2]*x[2]);if(o=="escape_velocity"&&x.size()==2)return std::sqrt(std::max(0.0,2*G*x[0]/x[1]));
    if(o=="lorentz_gamma"&&x.size()==1)return 1.0/std::sqrt(std::max(0.0,1-(x[0]*x[0])/(C*C)));
    if(o=="relativistic_energy"&&x.size()==2){double g=physicsop("lorentz_gamma",{x[1]});return g*x[0]*C*C;}
    if(o=="momentum"&&x.size()==2){double g=physicsop("lorentz_gamma",{x[1]});return g*x[0]*x[1];}
    if(o=="ohms_voltage"&&x.size()==2)return x[0]*x[1];if(o=="electric_power"&&x.size()==2)return x[0]*x[1];
    if(o=="wave_frequency"&&x.size()==2)return x[0]/x[1];if(o=="angular_frequency"&&x.size()==1)return 2.0*M_PI*x[0];
    if(o=="wavenumber"&&x.size()==1)return 2.0*M_PI/x[0];
    if(o=="neutrino_oscillation_probability"&&x.size()==4){double s1=std::sin(2*x[0]),s2=std::sin(1.267*x[1]*x[2]/x[3]);return s1*s1*s2*s2;}
    throw std::runtime_error("unsupported physics operation or arity");
}
static double analyticsop(const std::string&o,const std::vector<double>&x){
    if(x.empty())throw std::runtime_error("analytics requires at least one value");
    sleela_data_analytics_set_t a{x.data(),x.size()}; double r=0; char err[128]={0};
    if(sleela_data_analytics_validate(&a,err,sizeof(err)))throw std::runtime_error(err);
    int ok=0;
    if(o=="sum")ok=sleela_data_analytics_sum(&a,&r);
    else if(o=="mean")ok=sleela_data_analytics_mean(&a,&r);
    else if(o=="min")ok=sleela_data_analytics_min(&a,&r);
    else if(o=="max")ok=sleela_data_analytics_max(&a,&r);
    else if(o=="variance")ok=sleela_data_analytics_variance(&a,&r);
    else if(o=="stddev")ok=sleela_data_analytics_stddev(&a,&r);
    else if(o=="correlation"){
        if(x.size()<4||x.size()%2)throw std::runtime_error("correlation requires two equally sized data sets");
        sleela_data_analytics_set_t b{x.data()+x.size()/2,x.size()/2};
        a.count=x.size()/2; ok=sleela_data_analytics_correlation(&a,&b,&r);
    } else throw std::runtime_error("unsupported data-analytics operation or arity");
    if(ok)throw std::runtime_error("data-analytics operation failed"); return r;
}
static void run_science(const Node&root,const Node&project){
    const Node*s=child(root,"science");if(!s)throw std::runtime_error("science project requires <science>");
    for(const Node*d:children(*s,"discipline"))for(const Node*o:children(*d,"operation")){
        const std::string domain=attr(*d,"name"),op=attr(*o,"name"),seq=attr(*o,"sequence","001");
        try{double r=domain=="math"?mathop(op,args(*o)):domain=="physics"?physicsop(op,args(*o)):domain=="chemistry"?chemistryop(op,args(*o)):domain=="data-analytics"?analyticsop(op,args(*o)):throw std::runtime_error("no executable XML handler for discipline");
            std::ostringstream q;q<<std::setprecision(17)<<r;witness(attr(project,"id"),seq,domain,op,q.str(),"executed");
        }catch(const std::exception&e){witness(attr(project,"id"),seq,domain,op,e.what(),"rejected");}
    }
}
static sleela_email_tls_mode_t tlsmode(const std::string&s){
    if(s=="none")return SLEELA_EMAIL_TLS_NONE;if(s=="starttls")return SLEELA_EMAIL_TLS_STARTTLS;if(s=="implicit")return SLEELA_EMAIL_TLS_IMPLICIT;
    throw std::runtime_error("unknown SMTP TLS mode");
}
static void run_post(const Node&root,const Node&project){
    const Node* p=child(root,"post"); if(!p) throw std::runtime_error("post project requires <post>");
    std::string target=attr(*p,"target","/"),type=attr(*p,"content-type","text/plain"),body=attr(*p,"body");
    sleela_post_t post{"POST",target.c_str(),type.c_str(),body.c_str()}; char err[512]={0};
    int ok=sleela_post_validate(&post,err,sizeof(err));
    witness(attr(project,"id"),"001","post","POST",ok==0?"validated":err,ok==0?"executed":"rejected");
}
static void run_listener(const Node&root,const Node&project){
    const Node* l=child(root,"listener"); if(!l) throw std::runtime_error("listener project requires <listener>");
    unsigned long pv=std::stoul(attr(*l,"port","0")); if(pv>65535) throw std::runtime_error("listener port out of range");
    sleela_listener_t x{attr(*l,"bind","127.0.0.1").c_str(),(unsigned short)pv,attr(*l,"protocol","http").c_str(),attr(*l,"route","/").c_str()};
    char err[512]={0}; int ok=sleela_listener_validate(&x,err,sizeof(err));
    witness(attr(project,"id"),"001","listener","bind",ok==0?"validated":err,ok==0?"executed":"rejected");
}
static void run_router(const Node&root,const Node&project){
    const Node* rr=child(root,"router"); if(!rr) throw std::runtime_error("router project requires <router>");
    for(const Node* r:children(*rr,"route")){
        sleela_route_t x{attr(*r,"method","POST").c_str(),attr(*r,"path","/").c_str(),attr(*r,"target","/").c_str()}; char err[512]={0};
        int ok=sleela_router_validate(&x,err,sizeof(err));
        std::string seq=attr(*r,"sequence","001"); witness(attr(project,"id"),seq,"router","route",ok==0?"validated":err,ok==0?"executed":"rejected");
    }
}

static void run_email(const Node&root,const Node&project,bool send){
    const Node*e=child(root,"email");if(!e)throw std::runtime_error("email project requires <email>");const Node*s=child(*e,"smtp"),*m=child(*e,"message");
    if(!s||!m)throw std::runtime_error("email requires smtp and message");
    std::string ue=attr(*s,"username-env"),pe=attr(*s,"password-env");const char*user=ue.empty()?nullptr:std::getenv(ue.c_str());const char*pass=pe.empty()?nullptr:std::getenv(pe.c_str());
    std::string host=attr(*s,"host"),helo=attr(*s,"helo","sleela"),from=attr(*m,"from"),to=attr(*m,"to"),subject=attr(*m,"subject"),body=attr(*m,"body");
    unsigned long pv=std::stoul(attr(*s,"port","0"));if(pv>65535)throw std::runtime_error("SMTP port out of range");
    sleela_email_message_t msg{};msg.smtp_host=host.c_str();msg.smtp_port=(unsigned short)pv;msg.helo_name=helo.c_str();msg.tls_mode=tlsmode(attr(*s,"tls","starttls"));
    msg.username=user;msg.password=pass;msg.from=from.c_str();msg.to=to.c_str();msg.subject=subject.c_str();msg.body=body.c_str();
    if(!send){witness(attr(project,"id"),"001","email.smtp","send","dry-run","dry-run");return;}
    char err[1024]={0};int ok=sleela_email_send(&msg,err,sizeof(err));witness(attr(project,"id"),"001","email.smtp","send",ok?"accepted":err,ok?"executed":"rejected");
}
}

int main(int argc,char**argv){
    if(argc<2){std::cerr<<"usage: bodi-xml-runner PROJECT.xml [--send-email]\\n";return 2;}
    bool send=false;for(int i=2;i<argc;i++)if(std::string(argv[i])=="--send-email")send=true;
    std::ifstream f(argv[1],std::ios::binary);if(!f){std::cerr<<"cannot open XML project\\n";return 2;}
    std::string xml((std::istreambuf_iterator<char>(f)),{});if(xml.size()>4*1024*1024){std::cerr<<"XML project exceeds 4 MiB limit\\n";return 2;}
    try{Node root=parse(xml);if(root.name!="bodi"||attr(root,"version")!="1")throw std::runtime_error("root must be <bodi version=\"1\">");
        const Node*p=child(root,"project");if(!p)throw std::runtime_error("missing <project>");std::string kind=attr(*p,"kind");
        if(kind=="science")run_science(root,*p);else if(kind=="email")run_email(root,*p,send);else if(kind=="post")run_post(root,*p);else if(kind=="listener")run_listener(root,*p);else if(kind=="router")run_router(root,*p);else throw std::runtime_error("unsupported project kind");
        return 0;
    }catch(const std::exception&e){std::cerr<<"BODI XML error: "<<e.what()<<"\\n";return 1;}
}
