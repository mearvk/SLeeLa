<img align="right" src="https://github.com/mearvk/SLeeLa/blob/master/images/debian/debian-logo.png" width="75" height="75" alt="SLeeLa">

# MariaDB connector profile

Uses the MySQL-compatible C ABI in `../mysql`, built against MariaDB Connector/C when `pkg-config libmariadb` is available. The implementation is shared intentionally to avoid divergent protocol code. Build with `make -C lib/connector/mariadb`. This is a compatibility profile, not a separately validated MariaDB implementation; test against the exact server/client versions in deployment.