<t:layout title="${pageTitle}" active="api">
<div class="split">
  <aside class="side" aria-label="API packages">
    <a href="${ctx}/api"><strong>API home</strong></a>
    <a href="${ctx}/api/packages">All packages</a>
    <a href="${ctx}/api/glossary">Glossary</a>
    <a href="${ctx}/api/authorship">Authorship</a>
    <h2>Packages</h2>
    <input class="filter" type="search" placeholder="Filter..." data-filter-list=".api-pkg" aria-label="Filter packages">
    <c:forEach var="p" items="${apiPackages}">
      <a class="api-pkg ${p == currentPackage ? 'current' : ''}" data-name="${fn:escapeXml(p)}" href="${ctx}/api/package/${p}"><c:out value="${p}"/></a>
    </c:forEach>
  </aside>
  <article class="doc full">${apiBody}</article>
</div>
</t:layout>
