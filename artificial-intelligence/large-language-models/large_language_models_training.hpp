#ifndef SLEELA_AI_LLM_TRAINING_HPP
#define SLEELA_AI_LLM_TRAINING_HPP
#include "large_language_models_training.h"
#include <cstdint>
#include <string>
#include <vector>
namespace sleela::ai {
struct TrainingOptions { double learning_rate=0.0001; double gradient_clip_norm=1.0; std::uint32_t micro_batch_size=1; std::uint32_t accumulation_steps=1; std::uint64_t seed=0; };
struct TrainingMetrics { double loss=0; std::uint64_t tokens_processed=0; std::uint64_t optimizer_steps=0; };
class LanguageModelTrainer {
public:
 explicit LanguageModelTrainer(const sllmt_backend& backend);
 ~LanguageModelTrainer();
 LanguageModelTrainer(const LanguageModelTrainer&)=delete;
 LanguageModelTrainer& operator=(const LanguageModelTrainer&)=delete;
 LanguageModelTrainer(LanguageModelTrainer&&) noexcept;
 LanguageModelTrainer& operator=(LanguageModelTrainer&&) noexcept;
 void initialize(const std::string& model_id,const TrainingOptions& options={});
 void shutdown() noexcept;
 bool is_ready() const noexcept;
 TrainingMetrics train_batch(const std::vector<std::uint32_t>& input,const std::vector<std::uint32_t>& target,const TrainingOptions& options={});
 TrainingMetrics evaluate_batch(const std::vector<std::uint32_t>& input,const std::vector<std::uint32_t>& target);
 void save_checkpoint(const std::string& path);
private:
 sllmt_trainer* handle_=nullptr;
 static void check(sllmt_status status);
};
}
#endif
