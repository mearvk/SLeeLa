# Language-model families: a representative taxonomy

This catalogue provides illustrative examples of model architectures and system patterns relevant to large language models (LLMs). It is a conceptual map for SLeeLa, not a list of commercial model names, checkpoints, or a claim that exactly 2,000–2,500 distinct architectures exist.

## Scope and time horizon

The technically documented families below are primarily those known through 2025. The interval 2026–2500 is speculative: no one can reliably enumerate future discoveries. Keep forecasts labelled as hypotheses and never mix them with established architectures.

## Main families

### Statistical language model (`n-gram`)

- **Period:** 2000s
- **Evidence status:** established
- **Idea:** Predicts tokens from local frequency context; useful baseline, limited long-range structure.

### Probabilistic sequence model (`hidden-markov-model`)

- **Period:** pre-2010
- **Evidence status:** established
- **Idea:** Models hidden states and observed token sequences; historically important for speech and NLP.

### Structured prediction (`conditional-random-field`)

- **Period:** 2000s
- **Evidence status:** established
- **Idea:** Labels sequences using contextual feature dependencies in classical NLP pipelines.

### Neural language model (`feed-forward-neural-lm`)

- **Period:** 2000s
- **Evidence status:** established
- **Idea:** Predicts a token from a fixed context window using dense neural layers.

### Recurrent neural network (`rnn`)

- **Period:** 2010s
- **Evidence status:** established
- **Idea:** Processes sequences with recurrent state; foundational neural sequence architecture.

### Gated recurrent network (`lstm`)

- **Period:** 2010s
- **Evidence status:** established
- **Idea:** Uses input, forget, and output gates to retain information over longer sequences.

### Gated recurrent network (`gru`)

- **Period:** 2010s
- **Evidence status:** established
- **Idea:** A streamlined gated recurrent architecture with update and reset gates.

### Encoder-decoder sequence model (`seq2seq`)

- **Period:** 2010s
- **Evidence status:** established
- **Idea:** Maps an input sequence to an output sequence, often with recurrent or transformer components.

### Attention-based encoder-decoder (`attention-seq2seq`)

- **Period:** 2010s
- **Evidence status:** established
- **Idea:** Allows decoder steps to focus on relevant encoded input positions.

### Bidirectional encoder (`transformer-encoder`)

- **Period:** 2018 onward
- **Evidence status:** established
- **Idea:** Uses self-attention and feed-forward blocks for contextual representations; BERT-style family.

### Autoregressive decoder (`transformer-decoder`)

- **Period:** 2018 onward
- **Evidence status:** established
- **Idea:** Uses causal self-attention to predict the next token; GPT-style family.

### Text-to-text transformer (`encoder-decoder-transformer`)

- **Period:** 2019 onward
- **Evidence status:** established
- **Idea:** Uses an encoder and autoregressive decoder for translation, summarization, and general text tasks.

### Conditional-compute model (`sparse-mixture-of-experts`)

- **Period:** 2020s
- **Evidence status:** established
- **Idea:** Routes tokens or examples to a subset of expert networks to scale capacity with conditional computation.

### Retrieval-grounded model system (`retrieval-augmented-generation`)

- **Period:** 2020s
- **Evidence status:** established
- **Idea:** Combines a language model with search or document retrieval; retrieved evidence is distinct from model weights.

### Extended-context model (`long-context-transformer`)

- **Period:** 2020s
- **Evidence status:** established
- **Idea:** Uses attention variants, memory, or position strategies to process longer sequences.

### State-space sequence model (`state-space-language-model`)

- **Period:** 2020s
- **Evidence status:** established
- **Idea:** Uses structured state-space dynamics or selective scans as an alternative to full attention in some settings.

### Hybrid sequence model (`hybrid-attention-state-space`)

- **Period:** 2020s
- **Evidence status:** emerging
- **Idea:** Combines attention layers with state-space or recurrent components.

### Multimodal model (`multimodal-language-model`)

- **Period:** 2020s
- **Evidence status:** established
- **Idea:** Connects text with image, audio, video, or other modality encoders and decoders.

### Audio/speech model (`speech-language-model`)

- **Period:** 2020s
- **Evidence status:** established
- **Idea:** Processes or generates speech/audio, sometimes through discrete audio tokens or aligned encoders.

### Code-specialized model (`code-language-model`)

- **Period:** 2020s
- **Evidence status:** established
- **Idea:** Trains on source code and text for completion, explanation, transformation, and tool-assisted coding.

### Vision-text model (`vision-language-model`)

- **Period:** 2020s
- **Evidence status:** established
- **Idea:** Aligns visual representations with language for image understanding and visual question answering.

### Generative hybrid (`diffusion-language-hybrid`)

- **Period:** 2020s
- **Evidence status:** research
- **Idea:** Combines language modeling with diffusion-style generation or refinement; implementations vary.

### Tool-using model system (`agentic-model-system`)

- **Period:** 2020s
- **Evidence status:** established
- **Idea:** Combines an LLM with planning, tools, memory, and feedback loops; system design rather than a single neural architecture.

### Continual adaptation system (`continual-learning-model`)

- **Period:** ongoing
- **Evidence status:** research
- **Idea:** Updates or adapts from sequential data while attempting to limit catastrophic forgetting.

### Brain-inspired computing model (`neuromorphic-language-model`)

- **Period:** ongoing
- **Evidence status:** research
- **Idea:** Explores spiking or event-driven computation for language-related workloads.

### Compressed deployment variant (`quantized-language-model`)

- **Period:** 2020s
- **Evidence status:** established
- **Idea:** Uses reduced-precision weights or activations to lower memory and compute; orthogonal to base architecture.

### Compressed student model (`distilled-language-model`)

- **Period:** 2010s onward
- **Evidence status:** established
- **Idea:** Transfers behavior from a larger teacher to a smaller model; orthogonal to base architecture.

### Distributed training system (`federated-language-model`)

- **Period:** 2020s
- **Evidence status:** research
- **Idea:** Trains across decentralized data holders with aggregation and privacy constraints; a training topology, not an architecture.

### Hybrid symbolic-neural system (`neuro-symbolic-language-system`)

- **Period:** ongoing
- **Evidence status:** research
- **Idea:** Combines neural language processing with explicit rules, logic, knowledge graphs, or verifiers.

### Speculative category (`future-hypothetical-model`)

- **Period:** 2026–2500
- **Evidence status:** speculative
- **Idea:** Potential post-2025 systems through 2500 cannot be catalogued as known facts; record proposals separately with evidence and uncertainty.


## Important distinctions

Some entries are neural architectures (RNN, LSTM, Transformer, state-space models). Others are system patterns (retrieval-augmented generation, agentic systems), training/deployment methods (distillation, quantization, federated learning), or application specializations (code, speech, vision-language). They are not mutually exclusive: one deployed model may be a decoder-only Transformer, quantized, distilled, retrieval-augmented, and used in an agentic system.

## Suggested SLeeLa model-card fields

Stable identifier; architecture and system-pattern tags; training objective; tokenizer; context length; positional strategy; layer count; hidden width; attention and expert dimensions; modalities; data provenance, license and cutoff; weight format and precision; evaluation results and limitations; safety and deployment constraints; evidence status.

A model configuration is not a trained model: weights, tokenizer, model card, checksums, and compatible inference implementation are separate artifacts. Do not infer capability from parameter count alone.
