#ifndef SLEELA_COORENAGRAPH_H
#define SLEELA_COORENAGRAPH_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

#define COORENAGRAPH_ID_MAX 128u
#define COORENAGRAPH_LABEL_MAX 256u

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
