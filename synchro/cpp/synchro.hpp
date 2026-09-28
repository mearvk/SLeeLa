#pragma once
#include "../c/synchro.h"
#include <cstdint>
#include <optional>
#include <stdexcept>
#include <new>
#include <vector>
namespace sleela::synchro {
class Stats{synchro_stats s_{};public:explicit Stats(std::size_t w=1024){if(synchro_stats_init(&s_,w)!=0)throw std::bad_alloc();}~Stats(){synchro_stats_free(&s_);}Stats(const Stats&)=delete;Stats&operator=(const Stats&)=delete;void record(std::optional<double>r){synchro_stats_record(&s_,r.has_value(),r.value_or(0));}std::uint64_t sent()const{return s_.sent;}std::uint64_t acked()const{return s_.acked;}std::uint64_t lost()const{return s_.lost;}double lossRate()const{return synchro_loss_rate(&s_);}double deliveryRate()const{return synchro_delivery_rate(&s_);}double percentile(double p)const{return synchro_percentile(&s_,p);}};
struct Packet{std::uint32_t sequence{};std::uint64_t sentNs{};};
inline std::vector<std::uint8_t> encode(Packet p){std::vector<std::uint8_t>b(16);if(synchro_packet_encode(b.data(),b.size(),p.sequence,p.sentNs)!=16)throw std::runtime_error("encode");return b;}
inline Packet decode(const std::uint8_t*b,std::size_t n){Packet p;if(synchro_packet_decode(b,n,&p.sequence,&p.sentNs)!=0)throw std::runtime_error("invalid packet");return p;}
}
