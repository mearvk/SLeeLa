<t:layout title="${pageTitle}" active="${section}">
<div class="crumbs"><a href="${ctx}/">Home</a>
  <c:forEach var="c" items="${crumbs}"><span class="sep">/</span><a href="${ctx}${c.url}"><c:out value="${c.label}"/></a></c:forEach>
  <span class="sep">/</span><c:out value="${fileName}"/></div>
<div class="section-title">
  <h1><c:out value="${fileName}"/></h1>
  <div class="btn-row" style="margin:0"><a class="btn ghost small" href="${fn:escapeXml(rawUrl)}">Raw text</a></div>
</div>
<p class="muted"><code><c:out value="${repoPath}"/></code> &middot; ${lines} line(s) &middot; <c:out value="${lang}"/></p>
<c:choose>
  <c:when test="${not empty html}"><article class="doc">${html}</article></c:when>
  <c:otherwise><pre class="src"><code class="language-${fn:escapeXml(lang)}"><c:out value="${text}"/></code></pre></c:otherwise>
</c:choose>
</t:layout>
