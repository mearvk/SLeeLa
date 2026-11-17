<t:layout title="${pageTitle}" active="tutorials">
<div class="crumbs"><a href="${ctx}/">Home</a><span class="sep">/</span><a href="${ctx}/tutorials">Tutorials</a><span class="sep">/</span><c:out value="${tutorial.slug}"/></div>
<div class="split">
  <aside class="side" aria-label="Lessons">
    <h2>Lessons</h2>
    <c:forEach var="tt" items="${tutorials}">
      <a href="${ctx}${tt.url}" class="${tt.slug == tutorial.slug ? 'current' : ''}"><c:out value="${tt.title}"/></a>
    </c:forEach>
  </aside>
  <article class="doc">
    ${html}
    <div class="btn-row">
      <c:if test="${not empty prev}"><a class="btn ghost" href="${ctx}${prev.url}">&laquo; <c:out value="${prev.title}"/></a></c:if>
      <c:if test="${not empty next}"><a class="btn" href="${ctx}${next.url}"><c:out value="${next.title}"/> &raquo;</a></c:if>
    </div>
  </article>
</div>
</t:layout>
