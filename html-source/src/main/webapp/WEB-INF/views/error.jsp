<t:layout title="${pageTitle}" active="">
<h1><c:out value="${status}"/> &middot; <c:out value="${pageTitle}"/></h1>
<p class="lede"><c:out value="${message}"/></p>
<div class="btn-row"><a class="btn" href="${ctx}/">Home</a><a class="btn ghost" href="${ctx}/search">Search</a></div>
</t:layout>
