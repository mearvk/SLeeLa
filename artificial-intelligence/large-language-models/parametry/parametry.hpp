#ifndef SLEELA_AI_PARAMETRY_HPP
#define SLEELA_AI_PARAMETRY_HPP
#include "parametry.h"
#include "../large_language_models_capacity.hpp"
#include <cstdint>
#include <stdexcept>
#include <string>
#include <vector>
namespace sleela::ai {

// A resolved parameter arrangement for a SLeeLa LLM or neural net, bound to
// EITHER a known/supported model (by name) OR a user's local model (by path).
class Parametry {
public:
 // Bind a known model by name (optionally overriding declared params).
 static Parametry known(sllmp_kind kind, const std::string& name) {
  sllmp_binding b{}; b.abi_version = SLLMP_ABI_VERSION; b.kind = kind;
  b.name = name.c_str();
  return resolve(b);
 }
 // Bind a user's LOCAL LLM by path (+ declared size/context).
 static Parametry local_llm(const std::string& path, std::uint64_t weight_bytes,
                            std::uint32_t context_tokens = 0,
                            const std::string& weight_format = "") {
  sllmp_binding b{}; b.abi_version = SLLMP_ABI_VERSION; b.kind = SLLMP_KIND_LLM;
  b.local_path = path.c_str(); b.weight_bytes = weight_bytes;
  b.context_tokens = context_tokens;
  b.weight_format = weight_format.empty() ? nullptr : weight_format.c_str();
  return resolve(b);
 }
 // Bind a user's LOCAL neural net by path + fully-connected shape.
 static Parametry local_neural_net(const std::string& path,
                                   const std::vector<std::size_t>& layer_sizes) {
  if (layer_sizes.size() < 2)
   throw std::invalid_argument("neural net needs at least an input and output layer");
  sllmp_binding b{}; b.abi_version = SLLMP_ABI_VERSION; b.kind = SLLMP_KIND_NEURAL_NET;
  b.local_path = path.c_str();
  b.layer_count = layer_sizes.size() - 1;
  b.layer_sizes = layer_sizes.data();
  return resolve(b);
 }

 sllmp_kind kind() const noexcept { return p_.kind; }
 sllmp_source source() const noexcept { return p_.source; }
 std::string name_or_path() const { return p_.name_or_path ? p_.name_or_path : ""; }
 std::uint64_t weight_bytes() const noexcept { return p_.weight_bytes; }
 std::uint32_t context_tokens() const noexcept { return p_.context_tokens; }

 // For an LLM, derive the capacity profile (2..16 GB envelope) this binding
 // implies, so the reasoning engine can size itself.
 CapacityProfile capacity() const {
  if (p_.kind != SLLMP_KIND_LLM)
   throw std::logic_error("capacity() is only meaningful for an LLM binding");
  CapacityProfile cp;
  cp.model_id = name_or_path();
  cp.weight_bytes = p_.weight_bytes;
  cp.context_tokens = p_.context_tokens;
  return cp;
 }

 // For a neural net, the fully-connected layer shape (slnn_* config input).
 std::vector<std::size_t> layer_sizes() const {
  if (p_.kind != SLLMP_KIND_NEURAL_NET)
   throw std::logic_error("layer_sizes() is only meaningful for a neural-net binding");
  std::vector<std::size_t> v;
  for (std::size_t i = 0; i <= p_.layer_count; ++i) v.push_back(p_.layer_sizes[i]);
  return v;
 }

 const sllmp_parameters& raw() const noexcept { return p_; }

private:
 explicit Parametry(const sllmp_parameters& p) : p_(p),
   shape_(p.layer_sizes, p.layer_sizes + (p.layer_sizes ? p.layer_count + 1 : 0)) {
  // Re-point the copied parameters at our owned shape so the vector outlives use.
  if (!shape_.empty()) p_.layer_sizes = shape_.data();
 }
 static Parametry resolve(const sllmp_binding& b) {
  sllmp_parameters out{};
  sllmp_status st = sllmp_resolve(&b, &out);
  if (st != SLLMP_OK)
   throw std::runtime_error(std::string("parametry resolve failed: ") + sllmp_status_string(st));
  return Parametry(out);
 }
 sllmp_parameters p_{};
 std::vector<std::size_t> shape_; // owns the NN shape when present
};

} // namespace sleela::ai
#endif
