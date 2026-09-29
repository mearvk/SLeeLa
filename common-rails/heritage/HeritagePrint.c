/*
 * CommonRails Heritage Print — C implementation.
 * Canonical Beautiful Design: fixed 80-column printing.
 */
#include <stdio.h>
#include <string.h>

#define PRINT_WIDTH 80
#define SQUARE_SIZE 21
#define OID_WIDTH 10

static void repeat_spaces(int count) {
    while (count-- > 0) putchar(' ');
}

void heritage_print_width_line(const char *content) {
    size_t len = strlen(content);
    if (len <= PRINT_WIDTH) {
        fputs(content, stdout);
        repeat_spaces((int)(PRINT_WIDTH - len));
        putchar('\n');
        return;
    }

    /* Preserve the 80-column contract by wrapping at word boundaries. */
    const char *p = content;
    while (*p) {
        size_t remaining = strlen(p);
        size_t take = remaining <= PRINT_WIDTH ? remaining : PRINT_WIDTH;
        if (remaining > PRINT_WIDTH) {
            size_t cut = take;
            while (cut > 0 && p[cut] != ' ' && p[cut - 1] != ' ') --cut;
            if (cut > 0) take = cut;
        }
        fwrite(p, 1, take, stdout);
        if (take < PRINT_WIDTH) repeat_spaces((int)(PRINT_WIDTH - take));
        putchar('\n');
        p += take;
        while (*p == ' ') ++p;
    }
}

void heritage_print_field(const char *content, int field_width) {
    size_t len = strlen(content);
    fputs(content, stdout);
    if (len < (size_t)field_width) repeat_spaces(field_width - (int)len);
}

void heritage_print_component(const char *name, unsigned long object_id,
                              unsigned long date, const char *message) {
    char line[1024];
    snprintf(line, sizeof(line),
             "-- : [Object ID: %010lu] [Date: %lu] [Current: @%s] . %s .",
             object_id, date, name, message);
    heritage_print_width_line(line);
}

void heritage_print_square(int filled) {
    const int total = SQUARE_SIZE * SQUARE_SIZE;
    if (filled < 0) filled = 0;
    if (filled > total) filled = total;

    for (int row = 0; row < SQUARE_SIZE; ++row) {
        for (int col = 0; col < SQUARE_SIZE; ++col) {
            int fill_row = (SQUARE_SIZE - 1) - row;
            int fill_col = (SQUARE_SIZE - 1) - col;
            int fill_index = fill_row * SQUARE_SIZE + fill_col;
            fputs(fill_index < filled ? "█" : "░", stdout);
        }
        putchar('\n');
    }
}

void heritage_print_progress(int percent) {
    if (percent < 0) percent = 0;
    if (percent > 100) percent = 100;
    int cells = (percent * SQUARE_SIZE * SQUARE_SIZE) / 100;
    char line[128];
    snprintf(line, sizeof(line), "  progress %d%% (%d/%d cells)",
             percent, cells, SQUARE_SIZE * SQUARE_SIZE);
    heritage_print_width_line(line);
    heritage_print_square(cells);
}

#ifdef HERITAGE_PRINT_DEMO
int main(void) {
    heritage_print_component("CommonRails", 1234, 1, "printing initialized");
    heritage_print_width_line("[START]   CommonRails Heritage printing initialized");
    heritage_print_width_line("[WORKING] Content remains inside the 80-column contract");
    heritage_print_width_line("[COMPLETE] Fixed-width output ready");
    heritage_print_progress(50);
    return 0;
}
#endif
