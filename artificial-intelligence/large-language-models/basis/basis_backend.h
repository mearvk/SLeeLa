#ifndef SLEELA_LLM_BASIS_BACKEND_H
#define SLEELA_LLM_BASIS_BACKEND_H
#include "../large_language_models.h"
#ifdef __cplusplus
extern "C" {
#endif
/*
 * The provider adapter must implement this function and return a process-
 * lifetime sllm_backend descriptor. Its callbacks and user_data must remain
 * valid until all example engines have been destroyed.
 *
 * No credentials, provider SDK, model weights, or pretend inference runtime
 * are bundled with these examples.
 */
const sllm_backend *sllm_basis_backend(void);
#ifdef __cplusplus
}
#endif
#endif
