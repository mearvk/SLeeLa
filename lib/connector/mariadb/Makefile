all:
	$(MAKE) -C ../mysql MYSQL_CFLAGS="$(shell pkg-config --cflags libmariadb 2>/dev/null)" MYSQL_LIBS="$(shell pkg-config --libs libmariadb 2>/dev/null || echo -lmariadb)"
clean:
	$(MAKE) -C ../mysql clean
.PHONY: all clean

# Explicit alias for scripts and IDE build actions.
build: all
.PHONY: build
