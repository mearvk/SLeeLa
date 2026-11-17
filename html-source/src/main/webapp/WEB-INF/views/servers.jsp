<t:layout title="${pageTitle}" active="servers">
<div class="crumbs"><a href="${ctx}/">Home</a><span class="sep">/</span>HTTP servers</div>
<h1>HTTP servers and protocols</h1>
<p class="lede">Native HTTP/1.x, HTTP/2 and HTTP/3 server grades, every protocol generation, and the web server technology modules.</p>
<p>
  <span class="hc green"><i></i>green</span><span class="hc amber"><i></i>amber</span>
  <span class="hc red"><i></i>red</span><span class="hc black"><i></i>black</span>
  <a href="${ctx}/tutorials/11-http3-and-colors">HTTP colors tutorial</a>
</p>
<input class="filter" type="search" placeholder="Filter HTTP modules..." data-filter-list=".http-card" aria-label="Filter HTTP modules">
<div class="grid">
  <c:forEach var="m" items="${httpModules}">
    <a class="card http-card" data-name="${fn:escapeXml(m.name)}" href="${ctx}${m.url}">
      <h3><c:out value="${m.name}"/></h3>
      <p><span class="pill"><c:out value="${m.kind == 'lib' ? 'library' : 'system'}"/></span><c:if test="${m.api}"><span class="pill green">API</span></c:if></p>
    </a>
  </c:forEach>
</div>
<c:if test="${not empty intro}"><h2>HTTP servers README</h2><div class="doc">${intro}</div></c:if>
</t:layout>
