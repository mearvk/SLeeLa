<t:layout title="${pageTitle}" active="modules">
<div class="crumbs"><a href="${ctx}/">Home</a><span class="sep">/</span><a href="${ctx}/modules">Modules</a><span class="sep">/</span><c:out value="${module.name}"/></div>
<div class="section-title">
  <h1><c:out value="${module.name}"/></h1>
  <div class="btn-row" style="margin:0">
    <c:if test="${module.api}"><a class="btn small" href="${ctx}${module.apiUrl}">API docs</a></c:if>
  </div>
</div>
<p class="muted"><span class="pill"><c:out value="${module.kind == 'lib' ? 'library module' : 'system module'}"/></span> <code><c:out value="${module.relDir}"/></code></p>

<c:if test="${not empty examples}">
  <h2>Examples</h2>
  <div class="grid">
    <c:forEach var="g" items="${examples}">
      <a class="card" href="${ctx}${g.url}"><h3><c:out value="${g.label}"/></h3><p>${g.count} file(s)</p></a>
    </c:forEach>
  </div>
</c:if>

<c:if test="${not empty intro}"><div class="doc">${intro}</div></c:if>

<h2>Files <span class="pill">${fn:length(files)}</span></h2>
<c:choose>
  <c:when test="${empty files}"><p class="muted">No browsable text files.</p></c:when>
  <c:otherwise>
    <input class="filter" type="search" placeholder="Filter files..." data-filter-list=".file-item" aria-label="Filter files">
    <ul class="filelist">
      <c:forEach var="f" items="${files}">
        <li class="file-item" data-name="${fn:escapeXml(f.label)}"><a href="${ctx}${f.url}"><c:out value="${f.label}"/></a></li>
      </c:forEach>
    </ul>
  </c:otherwise>
</c:choose>
</t:layout>
