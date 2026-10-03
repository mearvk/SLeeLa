#ifndef SLEELA_ROUTE_DATA_H
#define SLEELA_ROUTE_DATA_H
#ifdef __cplusplus
extern "C" {
#endif
#define SLEELA_ROUTE_DATA_VERSION "1.0"
typedef struct {
    int vm_generation;
    const char *vm_name;
    const char *vm_path;
    const char *protocol;
    const char *server_surface;
} sleela_route_data;
int sleela_route_data_for_protocol(const char *protocol, sleela_route_data *out);
int sleela_route_data_validate(const sleela_route_data *data);
int sleela_route_data_required_vm(const char *protocol);
const char *sleela_route_data_vm_name(int generation);
const char *sleela_route_data_vm_path(int generation);
#ifdef __cplusplus
}
#endif
#endif
