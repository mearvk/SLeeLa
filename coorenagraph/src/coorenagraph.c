#include "../include/coorenagraph.h"

#include <string.h>

static int copy_text(char *dst, size_t capacity, const char *src)
{
    size_t length;

    if (!dst || capacity == 0u || !src) {
        return -1;
    }

    length = strlen(src);
    if (length == 0u || length >= capacity) {
        return -1;
    }

    memcpy(dst, src, length + 1u);
    return 0;
}

int coorenagraph_node_init(coorenagraph_node_t *node, const char *id)
{
    if (!node) {
        return -1;
    }

    memset(node, 0, sizeof(*node));
    return copy_text(node->id, sizeof(node->id), id);
}

int coorenagraph_edge_init(
    coorenagraph_edge_t *edge,
    const char *source_id,
    const char *destination_id)
{
    if (!edge) {
        return -1;
    }

    memset(edge, 0, sizeof(*edge));

    if (copy_text(edge->source_id, sizeof(edge->source_id), source_id) != 0) {
        return -1;
    }

    if (copy_text(
            edge->destination_id,
            sizeof(edge->destination_id),
            destination_id) != 0) {
        return -1;
    }

    edge->directed = 1;
    return 0;
}

int coorenagraph_node_set_label(coorenagraph_node_t *node, const char *label)
{
    if (!node) {
        return -1;
    }

    return copy_text(node->label, sizeof(node->label), label);
}

int coorenagraph_node_set_coordinates(
    coorenagraph_node_t *node,
    double x,
    double y,
    double z)
{
    if (!node) {
        return -1;
    }

    node->x = x;
    node->y = y;
    node->z = z;
    node->has_coordinates = 1;
    return 0;
}

int coorenagraph_edge_set_label(coorenagraph_edge_t *edge, const char *label)
{
    if (!edge) {
        return -1;
    }

    return copy_text(edge->label, sizeof(edge->label), label);
}

int coorenagraph_validate_node(const coorenagraph_node_t *node)
{
    if (!node || node->id[0] == '\0') {
        return -1;
    }

    return 0;
}

int coorenagraph_validate_edge(const coorenagraph_edge_t *edge)
{
    if (!edge ||
        edge->source_id[0] == '\0' ||
        edge->destination_id[0] == '\0') {
        return -1;
    }

    return 0;
}
