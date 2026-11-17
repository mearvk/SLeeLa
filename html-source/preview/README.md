# Offline preview and screenshots

Use this to capture screenshots without a servlet container. Tomcat stays the real runtime.

- `Preview.java` runs the real servlets against a SLeeLa checkout through a mocked servlet API.
  It renders the real JSP views with `MiniJsp.java` and writes static HTML plus `routes.json`.
- `MiniJsp.java` is a small renderer for the JSP subset the views use: EL, `c:*`, `fn:length`, `fn:escapeXml`, `t:layout`.
- `make_manifest.py` writes `manifest.json`, the list of pages and facets to capture.
- `make_batches.py` writes Playwright scripts, 6 shots per script. Each shot uses a fresh page and explicit timeouts,
  so one hang cannot block a whole run.

```sh
cd html-source
javac -d target/pv -cp jakarta.servlet-api-6.x.jar $(find src/main/java -name '*.java') preview/*.java
python3 preview/make_manifest.py ..
java -cp target/pv:jakarta.servlet-api-6.x.jar Preview .. src/main/webapp target/preview-site \
  $(python3 -c "import json;print(' '.join(sorted({m['url'] for m in json.load(open('preview/manifest.json'))})))")
python3 preview/make_batches.py      # then run each target/batches/batchNN.js in Playwright, one at a time
```

Run the batches one at a time. In testing, parallel runs on a single browser context closed it mid-run.
