#ifndef SLEELA_AI_PARAMETRY_H
#define SLEELA_AI_PARAMETRY_H
/*
 * SLeeLa parametry — the arrangement that binds a model to a consumer
 * (a SLeeLa LLM or a SLeeLa neural net), resolving EITHER a known/supported
 * model by name from a registry, OR a user's local model by path.
 *
 * "Parametry" here means the declared parameter arrangement of a model: its
 * source (known-by-name vs local), its kind (LLM vs neural net), and the
 * capacity/shape parameters that let a consumer size itself. SLeeLa ships the
 * REGISTRY of known model NAMES and their declared parameters (no weights) and
 * the BINDING logic; the user supplies a local model (path) when they do not
 * use a known name. Nothing here loads weights.
 *
 * Resolution rule (the "either/or"):
 *   - If a binding names a known model, parametry looks it up in the registry
 *     and fills the parameters from that entry.
 *   - Else, if a binding gives a local path, parametry accepts the caller's
 *     declared parameters as-is (local model, user-owned).
 *   - A binding that gives neither, or a name the registry does not know with
 *     no local fallback, fails closed.
 */
#include <stddef.h>
#include <stdint.h>
#ifdef __cplusplus
extern "C" {
#endif
#if defined(_WIN32) && defined(SLEELA_AI_SHARED)
# if defined(SLEELA_AI_BUILD)
#  define SLLMP_API __declspec(dllexport)
# else
#  define SLLMP_API __declspec(dllimport)
# endif
#elif defined(__GNUC__) && defined(SLEELA_AI_SHARED)
# define SLLMP_API __attribute__((visibility("default")))
#else
# define SLLMP_API
#endif

#define SLLMP_ABI_VERSION 1u

/* What kind of model the parameters describe. */
typedef enum sllmp_kind {
 SLLMP_KIND_LLM=0,         /* a language / code model (weights, 2..16 GB envelope) */
 SLLMP_KIND_NEURAL_NET=1   /* a fully-connected net (slnn_*): layer shape, not GB weights */
} sllmp_kind;

/* Where the model comes from. */
typedef enum sllmp_source {
 SLLMP_SOURCE_UNSET=0,
 SLLMP_SOURCE_KNOWN=1,     /* a supported model named in the registry */
 SLLMP_SOURCE_LOCAL=2      /* a user-supplied local model by path */
} sllmp_source;

typedef enum sllmp_status {
 SLLMP_OK=0, SLLMP_INVALID_ARGUMENT=1, SLLMP_UNKNOWN_MODEL=2,
 SLLMP_NO_SOURCE=3, SLLMP_KIND_MISMATCH=4, SLLMP_UNSUPPORTED=5
} sllmp_status;

/* The resolved parameter arrangement (parametry) of a model. For an LLM,
   weight_bytes/context_tokens are meaningful (feed the capacity profile); for a
   neural net, layer_count/layer_sizes describe the fully-connected shape. */
typedef struct sllmp_parameters {
 uint32_t abi_version;
 sllmp_kind kind;
 sllmp_source source;
 const char *name_or_path;   /* registry name (KNOWN) or filesystem path (LOCAL) */
 /* LLM parameters */
 uint64_t weight_bytes;      /* declared size; 0 if n/a or unspecified */
 uint32_t context_tokens;    /* 0 => let the capacity layer derive it */
 const char *weight_format;  /* informational, e.g. "gguf-q4_k_m" */
 /* Neural-net parameters (fully-connected; slnn_*) */
 size_t   layer_count;       /* number of transitions (layers = count+1) */
 const size_t *layer_sizes;  /* layer_count+1 entries, borrowed */
} sllmp_parameters;

/* A caller's request to bind a model. Fill name (for KNOWN) OR local_path (for
   LOCAL). For a LOCAL neural net the caller also supplies the layer shape. */
typedef struct sllmp_binding {
 uint32_t abi_version;
 sllmp_kind kind;
 const char *name;           /* a known model name, or NULL */
 const char *local_path;     /* a local model path, or NULL */
 /* Local overrides / declarations (used when SOURCE_LOCAL, or to override): */
 uint64_t weight_bytes;
 uint32_t context_tokens;
 const char *weight_format;
 size_t   layer_count;
 const size_t *layer_sizes;
} sllmp_binding;

/* An entry in the registry of KNOWN, supported models. Declares NAME and
   parameters only -- never weights. */
typedef struct sllmp_known_model {
 const char *name;
 sllmp_kind  kind;
 uint64_t    weight_bytes;    /* LLM: declared size in the 2..16 GB envelope */
 uint32_t    context_tokens;  /* LLM: default context */
 const char *weight_format;   /* LLM: informational */
 size_t      layer_count;     /* NN: transitions */
 const size_t *layer_sizes;   /* NN: shape */
 const char *note;
} sllmp_known_model;

SLLMP_API const char *sllmp_status_string(sllmp_status status);
SLLMP_API const char *sllmp_kind_name(sllmp_kind kind);
SLLMP_API const char *sllmp_source_name(sllmp_source source);

/* The built-in registry of known/supported models (names + declared params). */
SLLMP_API size_t sllmp_known_model_count(void);
SLLMP_API const sllmp_known_model *sllmp_known_model_at(size_t index);
SLLMP_API const sllmp_known_model *sllmp_find_known_model(const char *name);

/* Resolve a binding to a concrete parameter arrangement (the either/or rule).
   On SLLMP_OK, *out is filled: SOURCE_KNOWN with registry params (caller
   overrides applied where nonzero), or SOURCE_LOCAL with the caller's params.
   Fails closed on an unknown name with no local fallback, a kind mismatch, or
   no source at all. Any borrowed pointers in *out alias the binding/registry
   and must outlive *out. */
SLLMP_API sllmp_status sllmp_resolve(const sllmp_binding *binding, sllmp_parameters *out);

#ifdef __cplusplus
}
#endif
#endif
