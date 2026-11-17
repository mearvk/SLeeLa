<t:layout title="${pageTitle}" active="tutorials">
<div class="crumbs"><a href="${ctx}/">Home</a><span class="sep">/</span>Tutorials</div>
<h1>Tutorials</h1>
<div class="grid wide">
  <c:forEach var="tt" items="${tutorials}" varStatus="st">
    <a class="card" href="${ctx}${tt.url}"><p>Lesson ${st.count}</p><h3><c:out value="${tt.title}"/></h3></a>
  </c:forEach>
</div>
<div class="doc">${intro}</div>
</t:layout>
