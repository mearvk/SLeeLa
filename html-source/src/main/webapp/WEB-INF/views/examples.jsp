<t:layout title="${pageTitle}" active="examples">
<div class="crumbs"><a href="${ctx}/">Home</a><span class="sep">/</span>Examples</div>
<h1>Examples by module</h1>
<p class="lede">Runnable SLeeLa programs, XML projects and reference code, grouped by the module they exercise.</p>
<input class="filter" type="search" placeholder="Filter collections..." data-filter-list=".example-card" aria-label="Filter collections">
<div class="grid">
  <c:forEach var="g" items="${groups}">
    <a class="card example-card" data-name="${fn:escapeXml(g.label)}" href="${ctx}${g.url}">
      <h3><c:out value="${g.label}"/></h3><p>${g.count} file(s)</p>
    </a>
  </c:forEach>
</div>
</t:layout>
