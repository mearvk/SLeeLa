#include "../../core/sleela_core.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ARTIFACT_HEADER_BYTES 47u
#define FUNCTION_METADATA_BYTES 20u

static int make_v3_legacy_copy(const char *source, const char *target) {
    FILE *in = fopen(source, "rb"), *out = NULL;
    unsigned char *bytes = NULL;
    long file_size;
    uint32_t counts[9];
    size_t offset;
    int ok = 0;
    if (!in) return 0;
    if (fseek(in, 0, SEEK_END) != 0 || (file_size = ftell(in)) < 0 ||
        fseek(in, 0, SEEK_SET) != 0) goto done;
    bytes = (unsigned char *)malloc((size_t)file_size);
    if (!bytes || fread(bytes, 1, (size_t)file_size, in) != (size_t)file_size)
        goto done;
    if ((size_t)file_size < ARTIFACT_HEADER_BYTES ||
        memcmp(bytes, "SLEELA-ART\x01", 11) != 0) goto done;
    memcpy(counts, bytes + 11, sizeof(counts));
    /* This fixture intentionally has no constants, strings, globals, or
     * struct layouts, so its only v4 additions are the per-function records. */
    if (counts[3] || counts[4] || counts[5] || counts[7] || counts[6] != 1u)
        goto done;
    offset = ARTIFACT_HEADER_BYTES + (size_t)counts[2] * 5u;
    if (offset + 4u > (size_t)file_size) goto done;
    {
        uint32_t name_length;
        memcpy(&name_length, bytes + offset, sizeof(name_length));
        offset += 4u + (size_t)name_length + 12u;
    }
    if (offset + FUNCTION_METADATA_BYTES != (size_t)file_size) goto done;
    memmove(bytes + offset, bytes + offset + FUNCTION_METADATA_BYTES,
            (size_t)file_size - offset - FUNCTION_METADATA_BYTES);
    {
        const uint32_t version = 3u;
        memcpy(bytes + 11, &version, sizeof(version));
    }
    out = fopen(target, "wb");
    if (!out) goto done;
    ok = fwrite(bytes, 1, (size_t)file_size - FUNCTION_METADATA_BYTES, out) ==
         (size_t)file_size - FUNCTION_METADATA_BYTES;
done:
    if (out && fclose(out) != 0) ok = 0;
    if (in) fclose(in);
    free(bytes);
    return ok;
}

static int check_v4_round_trip(const char *path, const char *tampered_path) {
    SLVM *vm = slvm_new(), *loaded = NULL;
    int native_function, ordinary_function;
    uint32_t flags = 0, abi = 0;
    uint64_t identity = 0;
    const char *signature = NULL;
    int ok = 0;
    if (!vm) return 0;

    native_function = slvm_begin_func(vm, "MathEngine::sum", 2, 2);
    if (native_function < 0) goto done;
    slvm_emit(vm, OP_LOADL, 0);
    slvm_emit(vm, OP_LOADL, 1);
    slvm_emit0(vm, OP_ADD);
    slvm_emit0(vm, OP_RET);
    if (slvm_set_func_native_metadata(
            vm, native_function,
            SL_NATIVE_METHOD_FLAG_ANNOTATED | SL_NATIVE_METHOD_FLAG_STATIC,
            SL_NATIVE_METHOD_ABI_VERSION,
            "MathEngine::sum|static|argc=2|3:int3:int|return=3:int|abi=1.0") != 0)
        goto done;
    if (slvm_set_func_native_metadata(
            vm, native_function,
            SL_NATIVE_METHOD_FLAG_ANNOTATED | SL_NATIVE_METHOD_FLAG_STATIC,
            SL_NATIVE_METHOD_ABI_VERSION,
            "MathEngine::sum|static|argc=2|3:int3:int|return=3:int|abi=1.0") == 0)
        goto done;
    slvm_end_func(vm);

    ordinary_function = slvm_begin_func(vm, "Application::main", 0, 0);
    if (ordinary_function < 0) goto done;
    slvm_emit0(vm, OP_RET);
    slvm_end_func(vm);
    slvm_set_entry(vm, ordinary_function);

    if (slvm_get_func_native_metadata(vm, native_function, &flags, &abi,
                                      &identity, &signature) != 1 ||
        flags != (SL_NATIVE_METHOD_FLAG_ANNOTATED | SL_NATIVE_METHOD_FLAG_STATIC) ||
        abi != SL_NATIVE_METHOD_ABI_VERSION || identity == 0 || !signature ||
        strcmp(signature, "MathEngine::sum|static|argc=2|3:int3:int|return=3:int|abi=1.0") != 0)
        goto done;
    if (slvm_get_func_native_metadata(vm, ordinary_function, &flags, &abi,
                                      &identity, &signature) != 0 ||
        flags != 0 || abi != 0 || identity != 0 || signature != NULL)
        goto done;
    if (slvm_save_file(vm, path) != 0 ||
        !slvm_validate_artifact_file(path, NULL, 0)) goto done;

    loaded = slvm_load_file(path);
    if (!loaded) goto done;
    if (slvm_get_func_native_metadata(loaded, native_function, &flags, &abi,
                                      &identity, &signature) != 1 ||
        flags != (SL_NATIVE_METHOD_FLAG_ANNOTATED | SL_NATIVE_METHOD_FLAG_STATIC) ||
        abi != SL_NATIVE_METHOD_ABI_VERSION || identity == 0 || !signature ||
        strcmp(signature, "MathEngine::sum|static|argc=2|3:int3:int|return=3:int|abi=1.0") != 0)
        goto done;
    if (slvm_get_func_native_metadata(loaded, ordinary_function, &flags, &abi,
                                      &identity, &signature) != 0 ||
        flags != 0 || abi != 0 || identity != 0 || signature != NULL)
        goto done;

    /* Corrupt only the persisted digest. The loader must reject the artifact
     * rather than trusting a mismatched signature identity. */
    {
        FILE *in = fopen(path, "rb"), *out = NULL;
        unsigned char *bytes = NULL;
        long size;
        uint32_t code_count, name_length;
        size_t metadata_offset;
        if (!in || fseek(in, 0, SEEK_END) != 0 || (size = ftell(in)) < 0 ||
            fseek(in, 0, SEEK_SET) != 0) { if (in) fclose(in); goto done; }
        bytes = (unsigned char *)malloc((size_t)size);
        if (!bytes || fread(bytes, 1, (size_t)size, in) != (size_t)size) {
            fclose(in); free(bytes); goto done;
        }
        fclose(in);
        memcpy(&code_count, bytes + 19, sizeof(code_count));
        metadata_offset = ARTIFACT_HEADER_BYTES + (size_t)code_count * 5u;
        memcpy(&name_length, bytes + metadata_offset, sizeof(name_length));
        metadata_offset += 4u + (size_t)name_length + 12u + 8u;
        if (metadata_offset + 8u > (size_t)size) { free(bytes); goto done; }
        bytes[metadata_offset] ^= 0x5au;
        out = fopen(tampered_path, "wb");
        if (!out) { free(bytes); goto done; }
        if (fwrite(bytes, 1, (size_t)size, out) != (size_t)size) {
            fclose(out); free(bytes); goto done;
        }
        fclose(out);
        free(bytes);
    }
    if (slvm_validate_artifact_file(tampered_path, NULL, 0)) goto done;
    ok = 1;
done:
    if (loaded) slvm_free(loaded);
    slvm_free(vm);
    remove(path);
    remove(tampered_path);
    return ok;
}

static int check_v3_compatibility(const char *path, const char *legacy_path) {
    SLVM *vm = slvm_new(), *loaded = NULL;
    int function;
    uint32_t flags = 99, abi = 99;
    uint64_t identity = 99;
    const char *signature = (const char *)1;
    int ok = 0;
    if (!vm) return 0;
    function = slvm_begin_func(vm, "Legacy::main", 0, 0);
    if (function < 0) goto done;
    slvm_emit0(vm, OP_RET);
    slvm_end_func(vm);
    slvm_set_entry(vm, function);
    if (slvm_save_file(vm, path) != 0 ||
        !make_v3_legacy_copy(path, legacy_path)) goto done;
    loaded = slvm_load_file(legacy_path);
    if (!loaded) goto done;
    if (slvm_get_func_native_metadata(loaded, 0, &flags, &abi,
                                      &identity, &signature) != 0 ||
        flags != 0 || abi != 0 || identity != 0 || signature != NULL)
        goto done;
    ok = 1;
done:
    if (loaded) slvm_free(loaded);
    slvm_free(vm);
    remove(path);
    remove(legacy_path);
    return ok;
}

static int authorize_exact_path(const char *path, void *context) {
    const char *approved = (const char *)context;
    return path && approved && strcmp(path, approved) == 0;
}

static int check_ordinary_native_dispatch(const char *artifact, const char *plugin) {
    SLVM *vm = slvm_load_file(artifact);
    SLNativeHandle handle = SL_NATIVE_HANDLE_INVALID;
    SLValue result;
    int ok = 0;
    if (!vm) return 0;
    if (slvm_set_native_authorizer(vm, authorize_exact_path, (void *)plugin) != 0 ||
        slvm_native_module_open(vm, plugin, &handle) != SL_NATIVE_OK ||
        slvm_bind_native_method(vm, 0, handle) != 0)
        goto done;
    /* A bound module cannot be unloaded while any method points at it. */
    if (slvm_native_module_close(vm, handle) == SL_NATIVE_OK) goto done;
    if (slvm_run(vm) != SLR_OK) goto done;
    result = slvm_result(vm);
    if (result.type != SL_INT || result.as.i != 143) goto done;
    if (slvm_unbind_native_method(vm, 0) != 0) goto done;
    if (slvm_native_module_close(vm, handle) != SL_NATIVE_OK) goto done;
    handle = SL_NATIVE_HANDLE_INVALID;
    ok = 1;
done:
    if (handle != SL_NATIVE_HANDLE_INVALID) {
        (void)slvm_unbind_native_method(vm, 0);
        (void)slvm_native_module_close(vm, handle);
    }
    slvm_free(vm);
    return ok;
}

static int check_interpreted_fallback(const char *artifact) {
    SLVM *vm = slvm_load_file(artifact);
    SLValue result;
    int ok = 0;
    if (!vm) return 0;
    if (slvm_run(vm) == SLR_OK) {
        result = slvm_result(vm);
        ok = result.type == SL_INT && result.as.i == 43;
    }
    slvm_free(vm);
    return ok;
}

static int check_compiler_artifact(const char *path) {
    SLVM *vm = slvm_load_file(path);
    uint32_t flags = 0, abi = 0;
    uint64_t identity = 0;
    const char *signature = NULL;
    int ok = 0;
    if (!vm) return 0;
    if (slvm_get_func_native_metadata(vm, 0, &flags, &abi,
                                      &identity, &signature) == 1 &&
        flags == (SL_NATIVE_METHOD_FLAG_ANNOTATED | SL_NATIVE_METHOD_FLAG_STATIC) &&
        abi == SL_NATIVE_METHOD_ABI_VERSION && identity != 0 && signature &&
        strcmp(signature, "MathEngine::sum|static|argc=2|3:int3:int|return=3:int|abi=1.0") == 0)
        ok = 1;
    if (ok && slvm_get_func_native_metadata(vm, 1, &flags, &abi,
                                            &identity, &signature) != 1)
        ok = 0;
    if (ok && slvm_get_func_native_metadata(vm, 2, &flags, &abi,
                                            &identity, &signature) != 0)
        ok = 0;
    slvm_free(vm);
    return ok;
}

int main(int argc, char **argv) {
    if (!check_v4_round_trip("build/native_method_metadata_v4.sla",
                             "build/native_method_metadata_bad.sla")) {
        fprintf(stderr, "native method metadata v4 round-trip/validation failed\n");
        return 1;
    }
    if (!check_v3_compatibility("build/native_method_metadata_legacy_source.sla",
                                "build/native_method_metadata_v3.sla")) {
        fprintf(stderr, "native method metadata v3 compatibility failed\n");
        return 1;
    }
    if (argc > 1 && !check_compiler_artifact(argv[1])) {
        fprintf(stderr, "compiler @native metadata persistence failed\n");
        return 1;
    }
    if (argc > 2 && (!check_interpreted_fallback(argv[1]) ||
                     !check_ordinary_native_dispatch(argv[1], argv[2]))) {
        fprintf(stderr, "ordinary @native dispatch / interpreted fallback failed\n");
        return 1;
    }
    puts("native method metadata and dispatch: PASS (compiler registration, v4 round-trip, digest validation, v3 compatibility, unbound bytecode fallback, authorized ordinary-call dispatch, bound-module close protection)");
    return 0;
}
