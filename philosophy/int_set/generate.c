#include "int_set.h"
#include <stdio.h>
#include <string.h>

/*
 * Emits the INT set to int_set.json and int_set.xml in the current directory
 * (or to stdout with --stdout json|xml). Pure data generation, no network.
 */
int main(int argc, char **argv) {
    const sl_int_set *set = sl_int_set_default();

    if (argc >= 3 && strcmp(argv[1], "--stdout") == 0) {
        if (strcmp(argv[2], "json") == 0) { sl_int_set_write_json(set, stdout); return 0; }
        if (strcmp(argv[2], "xml") == 0)  { sl_int_set_write_xml(set, stdout);  return 0; }
        fprintf(stderr, "usage: %s [--stdout json|xml]\n", argv[0]);
        return 2;
    }

    {
        FILE *j = fopen("int_set.json", "w");
        FILE *x = fopen("int_set.xml", "w");
        if (!j || !x) { fprintf(stderr, "cannot open output files\n"); return 1; }
        sl_int_set_write_json(set, j);
        sl_int_set_write_xml(set, x);
        fclose(j);
        fclose(x);
        printf("wrote int_set.json and int_set.xml (%zu traits, %zu age bands)\n",
               set->trait_count, set->band_count);
    }
    return 0;
}
