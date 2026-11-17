<%@ tag language="java" pageEncoding="UTF-8" body-content="scriptless" trimDirectiveWhitespaces="true" %>
<%@ attribute name="title" required="true" %>
<%@ attribute name="active" required="false" %>
<%@ taglib prefix="c" uri="jakarta.tags.core" %>
<%@ taglib prefix="fn" uri="jakarta.tags.functions" %>
<c:set var="ctx" value="${pageContext.request.contextPath}"/>
<!DOCTYPE html>
<html lang="en" data-theme="dark">
<head>
<meta charset="utf-8">
<meta name="viewport" content="width=device-width, initial-scale=1">
<meta name="theme-color" content="#0B0F0F">
<meta name="sleela-site" content="1">
<title>${fn:escapeXml(title)} · SLeeLa</title>
<link rel="icon" type="image/svg+xml" href="${ctx}/favicon.svg">
<link rel="stylesheet" href="${ctx}/assets/css/sleela.css">
<script src="${ctx}/assets/js/theme.js"></script>
</head>
<body>
<a class="skip" href="#content">Skip to content</a>
<header class="site-header">
  <div class="bar">
    <a class="brand" href="${ctx}/">
      <img src="${ctx}/assets/img/sleela-mark.svg" alt="" width="34" height="34">
      <span><span class="brand-name">SL<span class="ee">ee</span>La</span><small>THE NOW. THE INGREDIENT.</small></span>
    </a>
    <nav class="main-nav" aria-label="Main">
      <a href="${ctx}/tutorials" class="${active == 'tutorials' ? 'active' : ''}">Tutorials</a>
      <a href="${ctx}/examples" class="${active == 'examples' ? 'active' : ''}">Examples</a>
      <a href="${ctx}/modules" class="${active == 'modules' ? 'active' : ''}">Modules</a>
      <a href="${ctx}/runtime" class="${active == 'runtime' ? 'active' : ''}">Runtime</a>
      <a href="${ctx}/servers" class="${active == 'servers' ? 'active' : ''}">HTTP servers</a>
      <a href="${ctx}/api" class="${active == 'api' ? 'active' : ''}">API docs</a>
    </nav>
    <div class="header-tools">
      <form class="search-form" action="${ctx}/search" method="get" role="search">
        <input id="q" name="q" type="search" placeholder="Search ( / )" aria-label="Search" maxlength="100" value="${fn:escapeXml(param.q)}">
        <button class="btn" type="submit">Go</button>
      </form>
      <button class="btn ghost icon" id="theme-toggle" type="button" aria-label="Toggle light and dark theme" title="Toggle theme"><svg width="16" height="16" viewBox="0 0 16 16" aria-hidden="true"><circle cx="8" cy="8" r="6.5" fill="none" stroke="currentColor" stroke-width="1.5"/><path d="M8 1.5a6.5 6.5 0 0 1 0 13Z" fill="currentColor"/></svg></button>
    </div>
  </div>
</header>
<main id="content" class="page">
  <jsp:doBody/>
</main>
<footer class="site-footer">
  <div class="inner">
    <div>SLeeLa documentation site. Language, compiler, runtime, libraries and tools by MEARVK LLC.
      Source: <a href="https://github.com/mearvk/SLeeLa">github.com/mearvk/SLeeLa</a></div>
    <div class="swatches" title="SLeeLa Green, Deep Green, Charcoal, White, Slate">
      <i style="background:#00D68F"></i><i style="background:#008F62"></i><i style="background:#0B0F0F"></i><i style="background:#FFFFFF"></i><i style="background:#A0A9AD"></i>
    </div>
  </div>
</footer>
<script src="${ctx}/assets/js/site.js"></script>
</body>
</html>
