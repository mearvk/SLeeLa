#!/usr/bin/env python3
"""Writes Playwright batch scripts (6 shots each, fresh page per shot, explicit timeouts) that screenshot the preview site.
   usage: make_batches.py            all facets
          make_batches.py redo NAME..  only the named facets (mobile/tutorial subset by default)"""
import json,os
site=os.environ.get('PREVIEW_SITE','../target/preview-site')
routes=json.load(open(site+'/routes.json'))
M=json.load(open(os.path.join(os.path.dirname(os.path.abspath(__file__)),'manifest.json')))
import sys
if len(sys.argv)>1 and sys.argv[1]=='redo': M=[m for m in M if m['mode']=='mobile' or m['name'].startswith('tutorial-')]
if len(sys.argv)>2: M=[m for m in M if m['name'] in sys.argv[2:]]
def page(url): return {'s':int(routes['status:'+url]),'b':open(os.path.join(site,routes[url]),encoding='utf-8').read()}
assets={}
for d,_,fs in os.walk(site):
    for f in fs:
        p=os.path.join(d,f); rel='/'+os.path.relpath(p,site)
        if rel.startswith('/assets/') or rel=='/favicon.svg': assets[rel]=open(p,encoding='utf-8').read()
out=os.environ.get('BATCH_DIR','../target/batches/')
dest=os.path.abspath(os.path.join(os.path.dirname(os.path.abspath(__file__)),'..','screenshots'))+'/'
T=r"""async (page) => {
const PAGES=__P__; const ASSETS=__A__; const SHOTS=__S__; const DEST=__D__;
const types={'.css':'text/css','.js':'application/javascript','.svg':'image/svg+xml'};
const ctx = page.context(); const res=[];
for (const s of SHOTS) {
  const mobile = s.mode==='mobile'; const vw = mobile?390:1280;
  const p = await ctx.newPage(); p.setDefaultTimeout(10000);
  try {
    await p.addInitScript(() => { try { if (!sessionStorage.getItem('kept')) { localStorage.clear(); sessionStorage.setItem('kept','1'); } } catch (e) {} });
    await p.route('**/*', async (route) => {
      const raw = route.request().url().replace(/^https?:\/\/sleela\.test/, '').split('#')[0];
      const qi = raw.indexOf('?'); let key = decodeURIComponent(qi<0?raw:raw.slice(0,qi));
      if (key.length>1 && key.endsWith('/')) key = key.slice(0,-1);
      const pg = PAGES[raw] || (qi<0 ? PAGES[key] : null);
      if (pg) return route.fulfill({status:pg.s, contentType:'text/html; charset=utf-8', body:pg.b});
      if (ASSETS[key]) return route.fulfill({status:200, contentType:types[key.slice(key.lastIndexOf('.'))]||'text/plain', body:ASSETS[key]});
      return route.fulfill({status:404, contentType:'text/plain', body:''});
    });
    await p.setViewportSize(mobile?{width:390,height:844}:{width:1280,height:800});
    await p.emulateMedia({colorScheme: s.mode==='light'?'light':'dark'});
    await p.goto('http://sleela.test'+s.url, {waitUntil:'load', timeout:15000});
    if (s.actions) for (const a of s.actions) { if (a[0]==='click') await p.click(a[1]); else await p.fill(a[1], a[2]); await p.waitForTimeout(100); }
    const info = await p.evaluate(() => ({h: document.documentElement.scrollHeight, w: document.documentElement.scrollWidth, t: document.documentElement.getAttribute('data-theme')}));
    const opts = {path: DEST+s.name+'.png', fullPage:true, timeout:15000};
    if (info.h>2600) { opts.fullPage=false; opts.clip={x:0,y:0,width:vw,height:2600}; await p.setViewportSize({width:vw,height:2600}); }
    await p.screenshot(opts);
    res.push(s.name+' ok h='+info.h+' '+info.t+(info.w>vw+2?' OVERFLOW-X '+info.w:''));
  } catch (e) { res.push(s.name+' FAIL '+String(e).split('\n')[0].slice(0,160)); }
  finally { await p.close(); }
}
return res;
}"""
N=6
PFX='redo' if len(sys.argv)>1 else 'batch'
for b in range(0,len(M),N):
    part=M[b:b+N]
    js=T.replace('__P__',json.dumps({m['url']:page(m['url']) for m in part})).replace('__A__',json.dumps(assets)).replace('__S__',json.dumps(part)).replace('__D__',json.dumps(dest))
    os.makedirs(out,exist_ok=True); open(out+PFX+'%02d.js'%(b//N),'w').write(js)
print(len(range(0,len(M),N)))
