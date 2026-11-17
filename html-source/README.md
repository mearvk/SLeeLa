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
Java 17+, Maven 3.9+, a Jakarta Servlet 6 container (Tomcat 10.1 or 11; Docker image uses 10.1 on JRE 21).

## Deploy on Linux

**Docker (recommended).** Clones SLeeLa to `/srv/sleela`, builds the image, starts the site and waits for `/healthz`:

```sh
./deploy/deploy.sh                       # http://localhost:8080
SLEELA_SRC=/srv/sleela SITE_PORT=80 ./deploy/deploy.sh
```
Content is read live from the checkout, so `git -C /srv/sleela pull` updates the site without a rebuild
(restart the container to refresh cached indexes).

**Existing Tomcat.** Builds a self-contained WAR (tutorials, examples, modules, HTTP modules and API docs bundled
under `WEB-INF/content`) and installs it as `ROOT.war`:

```sh
CATALINA_BASE=/opt/tomcat ./deploy/deploy-tomcat.sh
```
`mvn package -Dnobundle` builds a small WAR instead; point it at a checkout with `SLEELA_HOME=/path` or `-Dsleela.home=/path`.

Content root lookup order: `-Dsleela.home`, `SLEELA_HOME`, `sleela.home` context-param, bundled `WEB-INF/content`,
then a checkout found by walking up from the working directory.
`deploy/nginx.conf` is a sample reverse proxy for TLS.

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
