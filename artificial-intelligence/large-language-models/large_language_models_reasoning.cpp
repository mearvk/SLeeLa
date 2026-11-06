#include "large_language_models_reasoning.hpp"
#include <cstring>
#include <utility>

namespace sleela::ai {

namespace {

ReasoningStep to_cpp(const sllmr_step& s){
 ReasoningStep r;
 r.index=s.index; r.depth=s.depth; r.kind=s.kind;
 r.text = s.text? s.text : "";
 r.verdict=s.verdict; r.confidence=s.confidence;
 return r;
}

// Context threaded through the C callbacks via user_data.
struct Bridge {
 ReasoningModel::StepObserver* observer;
 StepVerifier* verifier;
};

int on_step_trampoline(void* ud, const sllmr_step* step){
 auto* b=static_cast<Bridge*>(ud);
 if(b && b->observer && *b->observer){
  return (*b->observer)(to_cpp(*step)) ? 1 : 0;
 }
 return 1;
}

sllmr_verdict verify_trampoline(void* ud, sllmr_subject subject, const char* question,
                                const sllmr_step* prior, size_t prior_count,
                                const sllmr_step* candidate,
                                char* note_out, size_t note_cap){
 auto* v=static_cast<StepVerifier*>(ud);
 if(!v || !*v) return SLLMR_VERDICT_UNKNOWN;
 std::vector<ReasoningStep> pv; pv.reserve(prior_count);
 for(size_t i=0;i<prior_count;i++) pv.push_back(to_cpp(prior[i]));
 std::string note;
 sllmr_verdict verdict = (*v)(subject, question? question:"", pv, to_cpp(*candidate), note);
 if(note_out && note_cap){
  std::strncpy(note_out, note.c_str(), note_cap-1);
  note_out[note_cap-1]=0;
 }
 return verdict;
}

} // namespace

struct ReasoningModel::Impl {
 sllmr_engine* engine=nullptr;
 StepVerifier verifier;
 sllmr_verifier c_verifier{};  // stable storage; user_data points at `verifier`
 bool has_verifier=false;
};

ReasoningModel::ReasoningModel(LanguageModel& llm, StepVerifier verifier){
 impl_=new Impl();
 impl_->verifier=std::move(verifier);
 impl_->has_verifier = static_cast<bool>(impl_->verifier);

 const sllmr_verifier* vptr=nullptr;
 if(impl_->has_verifier){
  impl_->c_verifier.abi_version=SLLMR_ABI_VERSION;
  impl_->c_verifier.name="cpp-step-verifier";
  impl_->c_verifier.user_data=&impl_->verifier;
  impl_->c_verifier.check=&verify_trampoline;
  vptr=&impl_->c_verifier;
 }
 sllmr_status st=sllmr_engine_create(llm.native_handle(), vptr, &impl_->engine);
 if(st!=SLLMR_OK){
  delete impl_; impl_=nullptr;
  throw std::runtime_error(std::string("reasoning engine create failed: ")+sllmr_status_string(st));
 }
}

ReasoningModel::~ReasoningModel(){
 if(impl_){ sllmr_engine_destroy(impl_->engine); delete impl_; }
}
ReasoningModel::ReasoningModel(ReasoningModel&& o) noexcept : impl_(std::exchange(o.impl_,nullptr)){}
ReasoningModel& ReasoningModel::operator=(ReasoningModel&& o) noexcept{
 if(this!=&o){ if(impl_){ sllmr_engine_destroy(impl_->engine); delete impl_; } impl_=std::exchange(o.impl_,nullptr); }
 return *this;
}

std::string ReasoningModel::verifier_name() const{
 return impl_? sllmr_engine_verifier_name(impl_->engine) : "(none)";
}

ReasoningOutcome ReasoningModel::reason(const std::string& question,
                                        const ReasoningOptions& options,
                                        StepObserver on_step){
 if(!impl_) throw std::logic_error("reasoning model is moved-from");

 sllmr_options o; sllmr_options_init(&o);
 o.subject=options.subject; o.max_steps=options.max_steps; o.max_backtracks=options.max_backtracks;
 o.reflection_every=options.reflection_every; o.min_confidence=options.min_confidence;
 o.require_verified=options.require_verified?1:0; o.seed=options.seed;

 Bridge bridge{ on_step? &on_step : nullptr, impl_->has_verifier? &impl_->verifier : nullptr };
 sllmr_trace trace{};
 trace.user_data=&bridge;
 trace.on_step = on_step? &on_step_trampoline : nullptr;
 trace.on_backtrack=nullptr;

 sllmr_result res{};
 sllmr_status st=sllmr_reason(impl_->engine, question.c_str(), &o,
                              (trace.on_step)? &trace : nullptr, &res);
 if(st!=SLLMR_OK && st!=SLLMR_NO_SOUND_PATH){
  sllmr_result_free(&res);
  throw std::runtime_error(std::string("reason failed: ")+sllmr_status_string(st));
 }
 ReasoningOutcome out;
 out.answer = res.answer? res.answer : "";
 out.summary = res.summary? res.summary : "";
 out.confidence=res.confidence; out.verified=res.verified!=0;
 out.tentative=res.tentative!=0; out.backtracks=res.backtracks;
 out.steps.reserve(res.step_count);
 for(size_t i=0;i<res.step_count;i++) out.steps.push_back(to_cpp(res.steps[i]));
 sllmr_result_free(&res);
 return out;
}

} // namespace sleela::ai
