#!/usr/bin/env python3
"""Writes manifest.json: every page and facet (theme, mobile, filters, errors) captured for the screenshots."""
import json, os, sys

root = sys.argv[1] if len(sys.argv) > 1 else '../..'
tut = sorted(f[:-3] for f in os.listdir(root + '/tutorial') if f[:2].isdigit() and f.endswith('.md'))
M = []

def add(name, url, mode='dark', act=None):
    d = {'name': name, 'url': url, 'mode': mode}
    if act: d['actions'] = act
    M.append(d)

def facets(prefix, url, filt=None):
    add(prefix, url); add(prefix + '-light', url, 'light'); add(prefix + '-mobile', url, 'mobile')
    if filt: add(prefix + '-filtered', url, 'dark', [['type', filt[0], filt[1]]])

facets('home', '/')
add('home-theme-toggle-clicked', '/', 'dark', [['click', '#theme-toggle']])
facets('tutorials-index', '/tutorials')
for s in tut: add('tutorial-' + s, '/tutorials/' + s)
add('tutorial-11-light', '/tutorials/11-http3-and-colors', 'light')
add('tutorial-11-mobile', '/tutorials/11-http3-and-colors', 'mobile')
facets('examples-index', '/examples', ('input[data-filter-list]', 'gui'))
add('examples-group-root', '/examples/examples')
facets('examples-group-gui', '/examples/examples-gui', ('input[data-filter-list]', 'COMPONENT'))
add('examples-group-lib-gui', '/examples/lib-examples-gui')
add('examples-group-lib-compiler', '/examples/lib-examples-compiler')
facets('example-file-sleela', '/examples/examples-gui/CLASSIC-hello-window.sleela')
add('example-file-markdown', '/examples/examples-gui/README.md')
add('example-file-xml', '/examples/examples/01-math.xml')
add('example-file-java', '/examples/examples-java/differential/Base64Core.java')
facets('modules-index', '/modules', ('input[data-filter-list]', 'http'))
add('module-sys-http-servers', '/modules/sys/http-servers')
add('module-sys-http-3', '/modules/sys/http-3.0')
facets('module-lib-http-3-with-api', '/modules/lib/http-3.0', ('input[data-filter-list]', 'Route'))
add('module-lib-crypto', '/modules/lib/crypto')
add('module-file-readme', '/modules/sys/http-servers/f/README.md')
add('module-file-cpp', '/modules/sys/http-servers/f/3/http3/main.cpp')
add('module-file-sleela', '/modules/lib/http-3.0/f/Http30Route.sleela')
add('module-file-makefile', '/modules/sys/http-servers/f/3/Makefile')
facets('servers', '/servers', ('input[data-filter-list]', 'http-2'))
facets('api-home', '/api', ('input[data-filter]', 'crypto'))
add('api-packages', '/api/packages'); add('api-glossary', '/api/glossary'); add('api-authorship', '/api/authorship')
facets('api-package-http-3', '/api/package/http-3.0', ('input[data-filter]', 'route'))
add('api-package-crypto', '/api/package/crypto'); add('api-package-alphabet', '/api/package/alphabet')
facets('search-results-http', '/search?q=http')
add('search-results-colors', '/search?q=colors'); add('search-no-results', '/search?q=zzzzqq')
add('search-too-short', '/search?q=a'); add('search-empty', '/search')
add('error-404-tutorial', '/tutorials/nope'); add('error-404-api', '/api/package/nope', 'light'); add('error-404-module', '/modules/lib/nope', 'mobile')

json.dump(M, open(os.path.join(os.path.dirname(os.path.abspath(__file__)), 'manifest.json'), 'w'), indent=1)
print(len(M), 'entries')
