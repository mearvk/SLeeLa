#include <cstdio>
#include <cstdlib>
#include <string>
#include "gardulus_ii.h"
class BodiSysCtl{public:explicit BodiSysCtl(int ms,size_t xmlSize):intervalMs_(ms),xmlSize_(xmlSize){}int run(int iterations,const std::string&path){if(!path.empty())log_=std::fopen(path.c_str(),"a");int rc=gardulus_run_with_xml_size(intervalMs_,iterations,xmlSize_,&BodiSysCtl::onSample,this);if(log_)std::fclose(log_);return rc;}private:static int onSample(const gardulus_sample*s,void*c){auto*self=static_cast<BodiSysCtl*>(c);char json[2048];int n=gardulus_emit_json(s,json,sizeof(json));if(n<0)return 1;if(self->log_)std::fwrite(json,1,(size_t)n,self->log_);else std::fwrite(json,1,(size_t)n,stdout);return 0;}int intervalMs_;size_t xmlSize_;FILE*log_=nullptr;};
int main(int argc,char**argv){int interval=argc>1?std::atoi(argv[1]):1000;int iterations=argc>2?std::atoi(argv[2]):0;std::string log=argc>3?argv[3]:std::string();size_t xmlSize=argc>4?(size_t)std::strtoull(argv[4],nullptr,10):1024U;if(interval<1||xmlSize<1)return 2;BodiSysCtl ctl(interval,xmlSize);return ctl.run(iterations,log);}
