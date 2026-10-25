#ifndef SLEELA_CDMA_COMMON_HPP
#define SLEELA_CDMA_COMMON_HPP
#include "cdma_common.h"
#include <algorithm>
#include <climits>
#include <stdexcept>
#include <vector>
namespace sleela { namespace cdma {
inline std::vector<int> walsh_code(std::size_t n,unsigned i){std::vector<int> v(n);if(cdma_walsh_code(v.data(),n,i))throw std::invalid_argument("invalid Walsh code");return v;}
inline std::vector<int> ovsf_code(std::size_t n,unsigned i){std::vector<int> v(n);if(cdma_ovsf_code(v.data(),n,i))throw std::invalid_argument("invalid OVSF code");return v;}
inline std::vector<int> pn_sequence(std::size_t n,unsigned stages,unsigned taps,unsigned seed){std::vector<int> v(n);if(cdma_pn_sequence(v.data(),n,stages,taps,seed))throw std::invalid_argument("invalid PN parameters");return v;}
inline std::vector<int> spread_symbol(int symbol,const std::vector<int>& code){std::vector<int> v(code.size());if(cdma_spread_symbol(v.data(),v.size(),symbol,code.data()))throw std::invalid_argument("invalid symbol");return v;}
inline long correlate(const std::vector<int>& rx,const std::vector<int>& code){if(rx.size()!=code.size())throw std::invalid_argument("length mismatch");long c=cdma_correlate(rx.data(),rx.size(),code.data());if(c==LONG_MIN)throw std::invalid_argument("correlation error");return c;}
inline int decide_symbol(long c){return cdma_decide_symbol(c);}
class SharedChannel { std::vector<int> samples_; public: explicit SharedChannel(std::size_t n):samples_(n,0){} void transmit(const std::vector<int>& chips){if(chips.size()!=samples_.size())throw std::invalid_argument("length mismatch");cdma_channel_add(samples_.data(),samples_.size(),chips.data());} long correlation(const std::vector<int>& code)const{return correlate(samples_,code);} int receive(const std::vector<int>& code)const{return decide_symbol(correlation(code));} const std::vector<int>& raw()const{return samples_;} std::size_t length()const{return samples_.size();} void clear(){cdma_channel_clear(samples_.data(),samples_.size());} };
}}
#endif
