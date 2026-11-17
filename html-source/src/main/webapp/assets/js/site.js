(function () {
  'use strict';
  var root = document.documentElement;

  // theme toggle (initial theme is applied by theme.js in <head>)
  var tbtn = document.getElementById('theme-toggle');
  if (tbtn) tbtn.addEventListener('click', function () {
    var cur = root.getAttribute('data-theme') ||
      (window.matchMedia && window.matchMedia('(prefers-color-scheme: light)').matches ? 'light' : 'dark');
    var next = cur === 'light' ? 'dark' : 'light';
    root.setAttribute('data-theme', next);
    try { localStorage.setItem('sleela-theme', next); } catch (e) {}
  });

  // generic list filter: <input data-filter-list="#id"> filters children by data-name
  function bindFilter(input, items, attr) {
    input.addEventListener('input', function () {
      var q = input.value.toLowerCase().trim();
      items().forEach(function (el) {
        var hay = (el.getAttribute(attr) || el.textContent).toLowerCase();
        el.style.display = !q || hay.indexOf(q) >= 0 ? '' : 'none';
      });
    });
  }
  document.querySelectorAll('input[data-filter-list]').forEach(function (inp) {
    var sel = inp.getAttribute('data-filter-list');
    bindFilter(inp, function () { return Array.prototype.slice.call(document.querySelectorAll(sel)); }, 'data-name');
  });
  // SleelaDoc API pages: per-unit filter (data-u) and package-card filter (data-p)
  document.querySelectorAll('input[data-filter]').forEach(function (inp) {
    var attr = inp.getAttribute('data-filter') === 'p' ? 'data-p' : 'data-u';
    bindFilter(inp, function () { return Array.prototype.slice.call(document.querySelectorAll('[' + attr + ']')); }, attr);
  });

  // lightweight syntax highlighting
  var KW = 'class struct interface enum import package public private protected static final abstract void int long short byte double float boolean char String ' +
    'if else for while do switch case default break continue return new this super true false null try catch finally throw throws record extends implements ' +
    'http service color unsigned usualmem accuratemem const var native namespace using include typedef union sizeof def lambda from as in is not and or elif echo fi then done function';
  var KWSET = {}; KW.split(' ').forEach(function (k) { KWSET[k] = true; });
  var rules = {
    sleela: /(\/\/[^\n]*|^[ \t]*#[^\n]*|<<[\s\S]*?>>|^[ \t]*<[^<\n][^\n]*>[ \t]*$)|("(?:\\.|[^"\\\n])*")|(\b\d[\d_.xXa-fA-F]*\b)|(\b[A-Za-z_][A-Za-z0-9_]*\b)/gm,
    c: /(\/\/[^\n]*|\/\*[\s\S]*?\*\/|^[ \t]*#[^\n]*)|("(?:\\.|[^"\\\n])*"|'(?:\\.|[^'\\\n])*')|(\b\d[\d_.xXa-fA-F]*\b)|(\b[A-Za-z_][A-Za-z0-9_]*\b)/gm,
    java: null, bash: /(^[ \t]*#[^\n]*|\s#[^\n]*)|("(?:\\.|[^"\\\n])*"|'[^'\n]*')|(\b\d+\b)|(\b[A-Za-z_][A-Za-z0-9_]*\b)/gm,
    python: null, xml: null, json: /()("(?:\\.|[^"\\\n])*")|(\b-?\d[\d.eE+-]*\b)|(\b(?:true|false|null)\b)/gm
  };
  rules.java = rules.c; rules.python = rules.bash; rules.py = rules.bash;
  function esc(s) { return s.replace(/&/g, '&amp;').replace(/</g, '&lt;').replace(/>/g, '&gt;'); }
  function highlight(el) {
    var m = /language-([\w+-]+)/.exec(el.className || '');
    var lang = m ? m[1] : (el.getAttribute('data-lang') || '');
    var re = rules[lang];
    var text = el.textContent;
    if (!re || text.length > 200000) return;
    re.lastIndex = 0;
    var out = '', last = 0, t;
    while ((t = re.exec(text)) !== null) {
      if (t[0] === '') { re.lastIndex++; continue; }
      out += esc(text.slice(last, t.index));
      var cls = t[1] ? 'tok-c' : t[2] ? 'tok-s' : t[3] ? 'tok-n' : (t[4] && KWSET.hasOwnProperty(t[4])) ? 'tok-k' : '';
      out += cls ? '<span class="' + cls + '">' + esc(t[0]) + '</span>' : esc(t[0]);
      last = t.index + t[0].length;
    }
    el.innerHTML = out + esc(text.slice(last));
  }
  document.querySelectorAll('pre code').forEach(highlight);

  // copy buttons on code blocks
  document.querySelectorAll('pre.code, pre.src').forEach(function (pre) {
    var b = document.createElement('button');
    b.type = 'button'; b.className = 'btn small ghost copy-btn'; b.textContent = 'Copy';
    b.addEventListener('click', function () {
      var txt = pre.querySelector('code').textContent;
      var done = function () { b.textContent = 'Copied'; setTimeout(function () { b.textContent = 'Copy'; }, 1400); };
      if (navigator.clipboard) navigator.clipboard.writeText(txt).then(done);
    });
    pre.appendChild(b);
  });

  // "/" focuses search
  document.addEventListener('keydown', function (e) {
    if (e.key === '/' && !/INPUT|TEXTAREA/.test((document.activeElement || {}).tagName || '')) {
      var s = document.getElementById('q'); if (s) { e.preventDefault(); s.focus(); }
    }
  });
})();
