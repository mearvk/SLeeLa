/* operating-system/tests/test_os_theme.c -- desktop theme model (theming capability). */
#include "sleela_os_theme.h"
#include <assert.h>
#include <string.h>
#include <stdio.h>

int main(void) {
    unsigned int n = 0;
    const char *const *names = sleela_os_theme_names(&n);
    assert(names != NULL);
    assert(n == sleela_os_theme_count());
    assert(n >= 6);

    /* known themes resolve */
    const sleela_os_theme *sb = sleela_os_theme_by_id("slick-black");
    const sleela_os_theme *white = sleela_os_theme_by_id("white");
    assert(sb != NULL && white != NULL);
    assert(strcmp(sb->label, "Slick Black") == 0);
    assert(strcmp(white->label, "White Edition") == 0);
    assert(sleela_os_theme_by_id("nope") == NULL);
    assert(sleela_os_theme_by_id(NULL) == NULL);

    /* per-shape defaults */
    assert(strcmp(sleela_os_theme_default_for(SLEELA_OS_SHAPE_LINUX)->id,   "linux-default")   == 0);
    assert(strcmp(sleela_os_theme_default_for(SLEELA_OS_SHAPE_WINDOWS)->id, "windows-default") == 0);
    assert(strcmp(sleela_os_theme_default_for(SLEELA_OS_SHAPE_MACOS)->id,   "macos-default")   == 0);

    /* role lookup reuses the shared vocabulary */
    const sleela_os_theme_color *acc = sleela_os_theme_role(sb, "accent");
    assert(acc != NULL);
    assert(acc->hex[0] == '#');
    assert(sleela_os_theme_role(sb, "no-such-role") == NULL);
    assert(strcmp(sleela_os_theme_role_hex(white, "accent", "#000000"), "#E95420") == 0);
    assert(strcmp(sleela_os_theme_role_hex(white, "no-role", "#000000"), "#000000") == 0);

    /* every theme has the core roles + desktop asset identifiers */
    for (unsigned int i = 0; i < n; ++i) {
        const sleela_os_theme *t = sleela_os_theme_by_id(names[i]);
        assert(t != NULL);
        assert(sleela_os_theme_role(t, "background") != NULL);
        assert(sleela_os_theme_role(t, "foreground") != NULL);
        assert(sleela_os_theme_role(t, "accent") != NULL);
        assert(t->gtk_theme != NULL && t->icon_set != NULL
            && t->cursor != NULL && t->wallpaper != NULL);
    }

    /* the white edition's assets align with ubuntu-white */
    assert(strcmp(white->icon_set, "Yaru") == 0);

    printf("test_os_theme: ok\n");
    return 0;
}
