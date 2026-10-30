// Illustrative SLeeLa model descriptor; metadata only, not an inference engine.
#pragma once
#include <cstdint>
#include <string>
#include <vector>

namespace sleela::llm {
enum class ArchitectureFamily { NGram, HiddenMarkov, CRF, FeedForward, RNN, LSTM, GRU, Seq2Seq, TransformerEncoder, TransformerDecoder, EncoderDecoderTransformer, MixtureOfExperts, StateSpace, HybridAttentionStateSpace, NeuroSymbolic, Other };
enum class EvidenceStatus { Established, Emerging, Research, Speculative };
enum class Modality { Text, Image, Audio, Video, Code };
struct ModelDescriptor {
  std::string id, display_name;
  ArchitectureFamily architecture = ArchitectureFamily::Other;
  EvidenceStatus evidence_status = EvidenceStatus::Research;
  std::vector<Modality> modalities;
  std::uint64_t parameter_count = 0; // optional; zero means unspecified
  std::uint32_t layer_count = 0, hidden_width = 0, attention_heads = 0, context_tokens = 0;
  std::string tokenizer_id, weight_format, training_objective;
  std::string model_card_path, weights_manifest_path;
};
inline bool has_minimum_identity(const ModelDescriptor& m) { return !m.id.empty() && !m.display_name.empty(); }
// Before loading: validate tensor shapes, tokenizer compatibility, checksums,
// supported precision, declared limits, and required host/accelerator memory.
} // namespace sleela::llm
