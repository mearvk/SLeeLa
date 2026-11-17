<t:layout title="${pageTitle}" active="home">
<section class="hero">
  <div class="tag">The Now. The Ingredient.</div>
  <h1>SL<span class="ee">ee</span>La</h1>
  <p class="lede">A language, compiler, runtime, standard library, HTTP servers and tools in one family.
     Learn it from the tutorials, run the examples, browse every module, read the API.</p>
  <div class="btn-row">
    <a class="btn" href="${ctx}/tutorials">Start the tutorials</a>
    <a class="btn ghost" href="${ctx}/examples">Examples by module</a>
    <a class="btn ghost" href="${ctx}/runtime">Run SLeeLa</a>
    <a class="btn ghost" href="${ctx}/api">API docs</a>
    <a class="btn ghost" href="${ctx}/servers">HTTP servers</a>
  </div>
</section>

<div class="grid">
  <a class="card stat" href="${ctx}/tutorials"><b>${fn:length(tutorials)}</b><p>tutorial lessons</p></a>
  <a class="card stat" href="${ctx}/examples"><b>${exampleGroupCount}</b><p>example collections</p></a>
  <a class="card stat" href="${ctx}/modules"><b>${libCount + sysCount}</b><p>modules (${libCount} library, ${sysCount} system)</p></a>
  <a class="card stat" href="${ctx}/api"><b>${apiCount}</b><p>API packages</p></a>
</div>

<div class="section-title"><h2>Tutorials</h2><a href="${ctx}/tutorials">All lessons</a></div>
<div class="grid wide">
  <c:forEach var="tt" items="${tutorials}" end="5">
    <a class="card" href="${ctx}${tt.url}"><h3><c:out value="${tt.title}"/></h3></a>
  </c:forEach>
</div>

<div class="section-title"><h2>HTTP servers and protocols</h2><a href="${ctx}/servers">Everything HTTP</a></div>
<p class="muted">HTTP behavior in SLeeLa can be named by color. A color selects wire form and integrity protections.</p>
<p>
  <span class="hc green"><i></i>green: text, core only</span>
  <span class="hc amber"><i></i>amber: text, MAC and replay guard</span>
  <span class="hc red"><i></i>red: binary, full integrity and basket</span>
  <span class="hc black"><i></i>black: binary, compressed, full integrity</span>
</p>
<div class="grid">
  <c:forEach var="m" items="${httpModules}" end="7">
    <a class="card" href="${ctx}${m.url}"><h3><c:out value="${m.name}"/></h3><p><span class="pill"><c:out value="${m.kind == 'lib' ? 'library' : 'system'}"/></span><c:if test="${m.api}"><span class="pill green">API</span></c:if></p></a>
  </c:forEach>
</div>
</t:layout>
