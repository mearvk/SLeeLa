#ifndef SLEELA_PREFERRED_ROUTER_H
#define SLEELA_PREFERRED_ROUTER_H
#ifdef __cplusplus
extern "C" {
#endif
#define SLEELA_PR_MAX_TEXT 256
typedef struct {
  char country[16];
  char role[64];
  char candidate[256];
  int routing_preference;
} sleela_preferred_router;
int sleela_preferred_router_select(const char *config_path, const char *protocol,
                                   sleela_preferred_router *out);
int sleela_preferred_router_startup(const char *config_path, const char *protocol);
/* Packet-layer policy hook. Returns 1 when the configured preferred tier is usable. */
int sleela_preferred_router_packet_policy(const char *protocol, int minimum_preference);
#ifdef __cplusplus
}
#endif
#endif
