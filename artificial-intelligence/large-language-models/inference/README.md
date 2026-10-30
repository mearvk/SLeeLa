# Inference

The provider-neutral inference API remains in `../large_language_models.h/.c` with its C++17 facade in `../large_language_models.hpp/.cpp`. A backend loads a model package, applies that model's tokenizer, runs generation, and streams UTF-8 output through the callback.

Inference must use the tokenizer and special-token conventions associated with the loaded model. Never assume the simple whitespace vocabulary encoder is compatible with an arbitrary pretrained model.
