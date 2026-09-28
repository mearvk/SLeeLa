#pragma once
#include "../c/munction.h"
#include <cstdint>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>
namespace sleela::munction {
class Channel{sleela_munction_channel c_{};public:explicit Channel(sleela_munction_channel c):c_(c){}const sleela_munction_channel&native()const noexcept{return c_;}};
class Receipt{sleela_munction_outcome o_;std::string text_;public:Receipt(sleela_munction_outcome o,std::string t):o_(o),text_(std::move(t)){}sleela_munction_outcome outcome()const noexcept{return o_;}const std::string&text()const noexcept{return text_;}bool reached()const noexcept{return o_==SLEELA_MUNCTION_REACHED;}};
class Munction{
    sleela_munction* raw_{};
    static void require(int r,const char*op){if(r)throw std::runtime_error(std::string("Munction ")+op+" failed");}
    Receipt finish(bool aborting){if(!raw_)throw std::logic_error("Munction already closed");char text[SLEELA_MUNCTION_MAX_RECEIPT];auto o=aborting?sleela_munction_abort(raw_,text,sizeof text):sleela_munction_close(raw_,text,sizeof text);raw_=nullptr;return Receipt(o,text);}
public:
    explicit Munction(const std::string&n):raw_(sleela_munction_start(n.c_str())){if(!raw_)throw std::invalid_argument("Munction.start(name) requires a name");}
    static Munction start(const std::string&n){return Munction(n);}
    Munction(const Munction&)=delete;Munction&operator=(const Munction&)=delete;
    Munction(Munction&&x)noexcept:raw_(std::exchange(x.raw_,nullptr)){}
    Munction&operator=(Munction&&x)noexcept{if(this!=&x){reset();raw_=std::exchange(x.raw_,nullptr);}return *this;}
    ~Munction(){reset();}
    Munction&connect(const Channel&c,const std::string&a){require(sleela_munction_connect(raw_,&c.native(),a.c_str()),"connect");return *this;}
    Munction&enable(const std::string&p){require(sleela_munction_enable(raw_,p.c_str()),"enable");return *this;}
    Munction&send(const std::vector<std::uint8_t>&d){auto n=sleela_munction_send(raw_,d.data(),d.size());if(n!=(std::int64_t)d.size())throw std::runtime_error("Munction send was not coherent");return *this;}
    Munction&send(const std::string&d){return send(std::vector<std::uint8_t>(d.begin(),d.end()));}
    Munction&thatch(const std::string&s){require(sleela_munction_thatch(raw_,s.c_str()),"thatch");return *this;}
    std::vector<std::uint8_t>consume(std::size_t cap=4096){std::vector<std::uint8_t>o(cap);auto n=sleela_munction_consume(raw_,o.data(),o.size());if(n<0)throw std::runtime_error("Munction reception absent or boundary-stopped");o.resize((std::size_t)n);return o;}
    std::string observe(std::size_t cap=256){std::string o(cap,'\0');require(sleela_munction_observe(raw_,o.data(),o.size()),"observe");o.resize(std::char_traits<char>::length(o.c_str()));return o;}
    Munction&latch(){require(sleela_munction_latch(raw_),"latch");return *this;}
    Receipt close(){return finish(false);}Receipt closeWithReceipt(){return finish(false);}Receipt abort(){return finish(true);}
    int callCount()const noexcept{return sleela_munction_call_count(raw_);}bool coherent()const noexcept{return sleela_munction_coherent(raw_)!=0;}
private:
    void reset()noexcept{if(raw_){char t[SLEELA_MUNCTION_MAX_RECEIPT];sleela_munction_abort(raw_,t,sizeof t);raw_=nullptr;}}
};
}
