<t:layout title="${pageTitle}" active="search">
<div class="crumbs"><a href="${ctx}/">Home</a><span class="sep">/</span>Search</div>
<h1>Search</h1>
<form class="search-form" action="${ctx}/search" method="get" style="max-width:520px">
  <input name="q" type="search" value="${fn:escapeXml(q)}" placeholder="tutorials, examples, modules, API packages" aria-label="Search" maxlength="100" style="flex:1">
  <button class="btn" type="submit">Search</button>
</form>
<c:choose>
  <c:when test="${fn:length(q) lt 2}"><p class="muted">Type at least two characters.</p></c:when>
  <c:when test="${empty hits}"><p class="muted">No results for <strong><c:out value="${q}"/></strong>.</p></c:when>
  <c:otherwise>
    <p class="muted">${fn:length(hits)} result(s) for <strong><c:out value="${q}"/></strong></p>
    <c:forEach var="h" items="${hits}">
      <div class="hit">
        <h3><span class="pill"><c:out value="${h.kind}"/></span> <a href="${ctx}${h.url}"><c:out value="${h.title}"/></a></h3>
        <p><c:out value="${h.snippet}"/></p>
      </div>
    </c:forEach>
  </c:otherwise>
</c:choose>
</t:layout>
