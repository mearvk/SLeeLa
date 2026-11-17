<t:layout title="${pageTitle}" active="runtime">
<div class="crumbs"><a href="${ctx}/">Home</a><span class="sep">/</span>Runtime</div>
<h1>SLeeLa Runtime</h1>
<p class="lede">Write a SLeeLa program, run it on the server and see the output. Each run is isolated, short and has no access to files, the network or other programs.</p>

<c:choose>
  <c:when test="${not empty unavailable}">
    <div class="notice warn" role="status" data-runtime-state="disabled">
      <strong>The runtime is not available right now.</strong>
      <span><c:out value="${unavailable}"/></span>
    </div>
  </c:when>
  <c:otherwise><span data-runtime-state="ready" hidden></span></c:otherwise>
</c:choose>

<div class="split runtime">
  <section>
    <form class="sample-picker" action="${ctx}/runtime" method="get">
      <label for="sample">Start from</label>
      <select id="sample" name="sample">
        <c:forEach var="s" items="${samples}"><option value="${fn:escapeXml(s.id)}" ${s.id == sampleId ? 'selected' : ''}><c:out value="${s.title}"/></option></c:forEach>
      </select>
      <button class="btn ghost small" type="submit">Load</button>
      <c:if test="${not empty loaded}"><span class="muted">Loaded: <code><c:out value="${loaded}"/></code></span></c:if>
    </form>

    <form action="${ctx}/runtime" method="post" id="runtime-form">
      <input type="hidden" name="token" value="${fn:escapeXml(token)}">
      <label class="sr-only" for="source">SLeeLa source</label>
      <textarea id="source" name="source" class="code-input" rows="22" spellcheck="false" autocapitalize="off" autocomplete="off" wrap="off"><c:out value="${source}"/></textarea>
      <div class="btn-row">
        <button class="btn" type="submit" name="action" value="run" ${empty unavailable ? '' : 'disabled'}>Run</button>
        <button class="btn ghost" type="submit" name="action" value="check" ${empty unavailable ? '' : 'disabled'}>Check syntax</button>
      </div>
    </form>

    <c:if test="${not empty result}">
      <h2 id="result">Result</h2>
      <c:choose>
        <c:when test="${result.status == 'OK'}">
          <div class="notice ok" role="status" data-result="ok">
            <strong><c:out value="${action == 'check' ? 'Checked' : 'Finished'}"/></strong>
            <span>in ${result.millis} ms, exit code ${result.exitCode}.</span>
          </div>
        </c:when>
        <c:when test="${result.status == 'ERROR'}">
          <div class="notice bad" role="alert" data-result="error">
            <strong>The program ended with an error</strong>
            <span>exit code ${result.exitCode} after ${result.millis} ms.</span>
          </div>
        </c:when>
        <c:when test="${result.status == 'TIMEOUT'}">
          <div class="notice bad" role="alert" data-result="timeout"><strong>Time limit reached</strong><span>The program was stopped.</span></div>
        </c:when>
        <c:when test="${result.status == 'DENIED' || result.status == 'TOO_LARGE'}">
          <div class="notice bad" role="alert" data-result="denied">
            <strong>This program was not run</strong>
            <span>The website only runs programs that stay inside the SLeeLa language.</span>
          </div>
        </c:when>
        <c:otherwise>
          <div class="notice warn" role="alert" data-result="${fn:escapeXml(result.status)}"><strong>Not run</strong></div>
        </c:otherwise>
      </c:choose>
      <c:if test="${not empty result.notes}">
        <ul class="notes"><c:forEach var="n" items="${result.notes}"><li><c:out value="${n}"/></li></c:forEach></ul>
      </c:if>
      <c:if test="${not empty result.output}">
        <pre class="out" tabindex="0" aria-label="Program output"><c:out value="${result.output}"/></pre>
      </c:if>
      <c:if test="${empty result.output and result.status == 'OK'}"><p class="muted">The program printed nothing.</p></c:if>
    </c:if>
  </section>

  <aside class="side info">
    <h2>Limits per run</h2>
    <ul>
      <li>${limits.timeout} seconds of real time, ${limits.cpu} seconds of CPU</li>
      <li>${limits.memoryMB} MB of memory</li>
      <li>${limits.outputKB} KB of output, ${limits.sourceKB} KB of source</li>
      <li>${limits.perMinute} runs per minute per visitor</li>
    </ul>
    <h2>Isolation</h2>
    <ul>
      <li>No network at all</li>
      <li>Read-only system and SLeeLa files, private empty scratch space deleted after the run</li>
      <li>No server environment variables or secrets</li>
      <li>Runs as an unprivileged user in its own namespace</li>
    </ul>
    <h2>Refused before running</h2>
    <p class="muted">Programs that name any of these, in your code or in a library unit they pull in:</p>
    <ul>
      <li>files, pipes and directories: <code>openFile</code>, <code>unlinkFile</code>, <code>osRemove</code> ...</li>
      <li>processes: <code>osRun</code>, <code>osSpawn</code></li>
      <li>network: <code>connect</code>, <code>listen</code>, <code>Munction</code>, <code>synchro*</code></li>
      <li>environment and host details: <code>osGetEnv</code>, <code>osls*</code></li>
      <li>native code: <code>nativeOpen</code>, <code>native</code> members</li>
      <li>threads and locks: <code>spawn</code>, <code>join</code>, <code>lock</code></li>
    </ul>
    <p class="muted">Runnable repo examples are marked with a Run button on <a href="${ctx}/examples">Examples</a>.</p>
  </aside>
</div>
</t:layout>
