# SLeeLa site (JSP + Servlet)

A read-only documentation site for the whole repository, in SLeeLa brand colors
(SLeeLa Green `#00D68F`, Deep Green `#008F62`, Charcoal `#0B0F0F`, White, Slate `#A0A9AD`; see `docs/BRAND-GUIDE.md`).
Dark by default, light theme and toggle included.

| URL | Servlet | View |
|---|---|---|
| `/` | `HomeServlet` | `home.jsp` |
| `/tutorials`, `/tutorials/{lesson}` | `TutorialServlet` | `tutorials.jsp`, `tutorial.jsp` |
| `/examples`, `/examples/{collection}`, `/examples/{collection}/{file}` | `ExampleServlet` | `examples.jsp`, `example-group.jsp`, `source.jsp` |
| `/modules`, `/modules/{lib\|sys}/{name}`, `.../f/{file}` | `ModuleServlet` | `modules.jsp`, `module.jsp`, `source.jsp` |
| `/servers` | `ServersServlet` | `servers.jsp` (every HTTP server and protocol module) |
| `/api`, `/api/package/{name}`, `/api/{glossary\|authorship\|packages}` | `ApiServlet` | `api.jsp` (SleelaDoc pages re-skinned) |
| `/search?q=` | `SearchServlet` | `search.jsp` |
| `/healthz` | `HealthServlet` | JSON, 200 when content is loaded |

Append `?raw=1` to any example or module file for plain text.

## Requirements
- **Runtime:** Java 11+ and a Jakarta Servlet 6 container: Tomcat 10.1 or 11. Tomcat 9 and 10.0 are refused.
- **Build:** Maven 3.9+ with Java 11+, or Docker (the scripts fall back to a Maven container).

## Deploy on a host running Apache 2 + Tomcat (Debian/Ubuntu)

```sh
sudo ./deploy/deploy-apache-tomcat.sh --dry-run   # preview every step, change nothing
sudo ./deploy/deploy-apache-tomcat.sh             # deploy to both
```

1. **Detect:** finds Tomcat (`/var/lib/tomcat10`, `/var/lib/tomcat11`, `/opt/tomcat`, ... or `CATALINA_BASE`), its version,
   systemd unit and HTTP port. Finds Apache (`/etc/apache2`, `a2enmod`, `mod_proxy_http`). A target that is missing or
   unsuitable is skipped with the reason.
2. **Back up** everything served at `/` now into `/backup/$DATE` (`YYYY-MM-DD_HHMMSS`):
   - Tomcat: `webapps/ROOT`, `ROOT.war`, `conf/Catalina/localhost/ROOT.xml` and `server.xml`.
   - Apache: every `DocumentRoot`, all of `/etc/apache2`, and the previous static dir.
   - `MANIFEST.tsv` lists every copy; `restore.sh [tomcat|apache|all]` puts it all back.
3. **Content:** clones `mearvk/SLeeLa` to `/srv/sleela`, or pulls if it is already there. The site reads it live.
4. **Tomcat:** installs the WAR as `ROOT.war` and writes `conf/Catalina/localhost/ROOT.xml`, which points the app at the checkout.
   Then it restarts the service and waits for `/healthz`. If that fails, it restores the Tomcat backup.
5. **Apache:** enables `proxy`, `proxy_http` and `headers`. Apache serves `/assets` and `/favicon.svg` itself from
   `/var/www/sleela-site` and proxies everything else to Tomcat. The config is `conf-available/sleela-site.conf`.
   It runs `apache2ctl configtest` before reloading and checks `/healthz` through Apache. If either fails, it restores `/etc/apache2`.

The Apache config applies to **every virtual host** on the server, which is what "the site at `/`" means here.
DocumentRoot files are backed up but not changed. Turn the site off with `sudo a2disconf sleela-site && sudo systemctl reload apache2`.

Options: `--tomcat-only`, `--apache-only`, `--no-rollback`, `--war FILE`.
Overrides: `SLEELA_SRC`, `CATALINA_BASE`, `TOMCAT_SERVICE`, `TOMCAT_PORT`, `APACHE_ETC`, `APACHE_URL`, `STATIC_DIR`, `BACKUP_ROOT`.
To update the content later, run `git -C /srv/sleela pull && sudo systemctl restart tomcat10`; the site caches its file lists at startup.

## Deploy with Docker instead

```sh
./deploy/deploy.sh                       # http://localhost:8080
SLEELA_SRC=/srv/sleela SITE_PORT=80 ./deploy/deploy.sh
```
Without root, the checkout defaults to `~/sleela`. Rerunning pulls new content and recreates the container.
`deploy/nginx.conf` is a sample reverse proxy for TLS.

Content root lookup order: `-Dsleela.home`, `SLEELA_HOME`, `sleela.home` context parameter, bundled `WEB-INF/content`,
then a checkout found by walking up from the working directory. `mvn package` without `-Dnobundle` builds a
self-contained WAR with the content inside.

## Screenshots
All pages and facets are captured as PNGs in [`preview/`](preview/SCREENSHOTS.md). See [`preview/README.md`](preview/README.md) to regenerate them.

## Layout
```
src/main/java/org/sleela/site/        ContentIndex, Markdown, Routes, ApiPages, SiteConfig, Model
src/main/java/org/sleela/site/web/    servlets, SiteListener, SecurityHeadersFilter
src/main/webapp/WEB-INF/views/        JSP views (+ _prelude.jspf)
src/main/webapp/WEB-INF/tags/         layout.tag
src/main/webapp/assets/               css, js, svg
```

## Notes
- No scripting in JSPs; all dynamic text goes through `c:out` / `fn:escapeXml`. Markdown source HTML is escaped, not passed through.
- Files are served only from an allow-listed set of text extensions, never from hidden paths, and never outside the content root (symlinks resolved).
- CSP allows only same-origin scripts; the API pages' inline handlers are rewritten to `data-filter` attributes.
