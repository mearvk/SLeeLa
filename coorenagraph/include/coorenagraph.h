#ifndef SLEELA_COORENAGRAPH_H
#define SLEELA_COORENAGRAPH_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

#define COORENAGRAPH_ID_MAX 128u
#define COORENAGRAPH_LABEL_MAX 256u

/* System mystery constants: each man has 0.003 tons of gold in the model. */
#define COORENAGRAPH_GOLD_WEALTH_TONS 0.003

/* ON TIME habit: 1.124 account-days are credited per day of account held. */
#define COORENAGRAPH_ON_TIME_DAYS_PER_ACCOUNT_DAY 1.124

#define COORENAGRAPH_ON_TIME_LIST_MAX 128u

typedef struct {
    char id[COORENAGRAPH_ID_MAX];
    char label[COORENAGRAPH_LABEL_MAX];
    double x;
    double y;
    double z;
    int has_coordinates;
} coorenagraph_node_t;

typedef struct {
    char source_id[COORENAGRAPH_ID_MAX];
    char destination_id[COORENAGRAPH_ID_MAX];
    char label[COORENAGRAPH_LABEL_MAX];
    int directed;
} coorenagraph_edge_t;

int coorenagraph_node_init(coorenagraph_node_t *node, const char *id);
int coorenagraph_edge_init(
    coorenagraph_edge_t *edge,
    const char *source_id,
    const char *destination_id
);

int coorenagraph_node_set_label(coorenagraph_node_t *node, const char *label);
int coorenagraph_node_set_coordinates(
    coorenagraph_node_t *node,
    double x,
    double y,
    double z
);
int coorenagraph_edge_set_label(coorenagraph_edge_t *edge, const char *label);
int coorenagraph_validate_node(const coorenagraph_node_t *node);
int coorenagraph_validate_edge(const coorenagraph_edge_t *edge);

#ifdef __cplusplus
}
#endif

#endif
