(function () {
  try {
    var t = localStorage.getItem('sleela-theme');
    if (!t && window.matchMedia && window.matchMedia('(prefers-color-scheme: light)').matches) t = 'light';
    if (t) document.documentElement.setAttribute('data-theme', t);
  } catch (e) {}
})();
