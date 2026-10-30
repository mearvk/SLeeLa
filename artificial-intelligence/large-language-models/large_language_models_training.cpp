#include "large_language_models_training.hpp"
#include <stdexcept>
#include <utility>
namespace sleela::ai {
void LanguageModelTrainer::check(sllmt_status s){if(s!=SLLMT_OK)throw std::runtime_error("SLeeLa LLM training error: "+std::to_string(static_cast<int>(s)));}
LanguageModelTrainer::LanguageModelTrainer(const sllmt_backend& b){check(sllmt_trainer_create(&b,&handle_));}
LanguageModelTrainer::~LanguageModelTrainer(){sllmt_trainer_destroy(handle_);}
LanguageModelTrainer::LanguageModelTrainer(LanguageModelTrainer&& x) noexcept:handle_(std::exchange(x.handle_,nullptr)){}
LanguageModelTrainer& LanguageModelTrainer::operator=(LanguageModelTrainer&& x) noexcept{if(this!=&x){sllmt_trainer_destroy(handle_);handle_=std::exchange(x.handle_,nullptr);}return *this;}
void LanguageModelTrainer::initialize(const std::string& id,const TrainingOptions& o){if(!handle_)throw std::logic_error("trainer is empty");sllmt_options c{};c.abi_version=SLLMT_ABI_VERSION;c.learning_rate=o.learning_rate;c.gradient_clip_norm=o.gradient_clip_norm;c.micro_batch_size=o.micro_batch_size;c.accumulation_steps=o.accumulation_steps;c.seed=o.seed;check(sllmt_trainer_initialize(handle_,id.c_str(),&c));}
void LanguageModelTrainer::shutdown() noexcept{sllmt_trainer_shutdown(handle_);}
bool LanguageModelTrainer::is_ready() const noexcept{return sllmt_trainer_is_ready(handle_)!=0;}
static sllmt_options convert(const TrainingOptions& o){sllmt_options c{};c.abi_version=SLLMT_ABI_VERSION;c.learning_rate=o.learning_rate;c.gradient_clip_norm=o.gradient_clip_norm;c.micro_batch_size=o.micro_batch_size;c.accumulation_steps=o.accumulation_steps;c.seed=o.seed;return c;}
static TrainingMetrics convert(const sllmt_metrics& m){return {m.loss,m.tokens_processed,m.optimizer_steps};}
TrainingMetrics LanguageModelTrainer::train_batch(const std::vector<std::uint32_t>& in,const std::vector<std::uint32_t>& target,const TrainingOptions& o){if(in.empty()||in.size()!=target.size())throw std::invalid_argument("input and target token batches must have equal non-zero lengths");sllmt_metrics m{};auto c=convert(o);check(sllmt_trainer_train_batch(handle_,in.data(),target.data(),in.size(),&c,&m));return convert(m);}
TrainingMetrics LanguageModelTrainer::evaluate_batch(const std::vector<std::uint32_t>& in,const std::vector<std::uint32_t>& target){if(in.empty()||in.size()!=target.size())throw std::invalid_argument("input and target token batches must have equal non-zero lengths");sllmt_metrics m{};check(sllmt_trainer_evaluate_batch(handle_,in.data(),target.data(),in.size(),&m));return convert(m);}
void LanguageModelTrainer::save_checkpoint(const std::string& path){check(sllmt_trainer_save_checkpoint(handle_,path.c_str()));}
}
