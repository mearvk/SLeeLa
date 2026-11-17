<t:layout title="${pageTitle}" active="examples">
<div class="crumbs"><a href="${ctx}/">Home</a><span class="sep">/</span><a href="${ctx}/examples">Examples</a><span class="sep">/</span><c:out value="${group.label}"/></div>
<h1><c:out value="${group.label}"/></h1>
<p class="muted">${group.count} file(s) under <code><c:out value="${group.baseRel}"/></code></p>
<input class="filter" type="search" placeholder="Filter files..." data-filter-list=".example-file" aria-label="Filter files">
<div class="grid">
  <c:forEach var="f" items="${group.files}">
    <a class="card example-file" data-name="${fn:escapeXml(f.rel)}" href="${ctx}${f.url}">
      <h3><c:out value="${f.name}"/></h3>
      <p><span class="pill"><c:out value="${f.lang}"/></span> <c:out value="${f.rel}"/></p>
    </a>
  </c:forEach>
</div>
<c:if test="${not empty intro}"><h2>About this collection</h2><div class="doc">${intro}</div></c:if>
</t:layout>
