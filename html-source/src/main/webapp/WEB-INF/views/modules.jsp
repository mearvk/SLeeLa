<t:layout title="${pageTitle}" active="modules">
<div class="crumbs"><a href="${ctx}/">Home</a><span class="sep">/</span>Modules</div>
<h1>Modules</h1>
<p class="lede">System modules (compiler, runtime, HTTP servers, native subsystems) and the standard library packages.</p>
<input class="filter" type="search" placeholder="Filter modules..." data-filter-list=".mod-card" aria-label="Filter modules">
<h2>System modules <span class="pill">${fn:length(sysModules)}</span></h2>
<div class="grid">
  <c:forEach var="m" items="${sysModules}">
    <a class="card mod-card" data-name="${fn:escapeXml(m.name)}" href="${ctx}${m.url}">
      <h3><c:out value="${m.name}"/></h3><p><c:if test="${m.readme}"><span class="pill">README</span></c:if></p>
    </a>
  </c:forEach>
</div>
<h2>Library modules <span class="pill">${fn:length(libModules)}</span></h2>
<div class="grid">
  <c:forEach var="m" items="${libModules}">
    <a class="card mod-card" data-name="${fn:escapeXml(m.name)}" href="${ctx}${m.url}">
      <h3><c:out value="${m.name}"/></h3>
      <p><c:if test="${m.readme}"><span class="pill">README</span></c:if><c:if test="${m.api}"><span class="pill green">API</span></c:if></p>
    </a>
  </c:forEach>
</div>
</t:layout>
